#include "stdafx.h"

#include ".\filefilter_driver.h"

CFileFilter_Driver::CFileFilter_Driver(void)
{
}

CFileFilter_Driver::~CFileFilter_Driver(void)
{
}

/****************************************************************************
*
*    FUNCTION: InstallDriver( IN SC_HANDLE, IN LPCTSTR, IN LPCTSTR)
*
*    PURPOSE: Creates a driver service.
*
****************************************************************************/
BOOL CFileFilter_Driver::InstallDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName, IN LPCTSTR ServiceExe )
{
	SC_HANDLE  schService;
	//
	// NOTE: This creates an entry for a standalone driver. If this
	//       is modified for use with a driver that requires a Tag,
	//       Group, and/or Dependencies, it may be necessary to
	//       query the registry for existing driver information
	//       (in order to determine a unique Tag, etc.).
	//
	schService = CreateService( SchSCManager,          // SCManager database
								DriverName,           // name of service
								DriverName,           // name to display
								SERVICE_ALL_ACCESS,    // desired access
								SERVICE_KERNEL_DRIVER, // service type
								SERVICE_DEMAND_START,  // start type
								SERVICE_ERROR_NORMAL,  // error control type
								ServiceExe,            // service's binary
								NULL,                  // no load ordering group
								NULL,                  // no tag identifier
								NULL,                  // no dependencies
								NULL,                  // LocalSystem account
								NULL                   // no password
								);
	if ( schService == NULL )
		return FALSE;

	CloseServiceHandle( schService );

	return TRUE;
}

/****************************************************************************
*
*    FUNCTION: StartDriver( IN SC_HANDLE, IN LPCTSTR)
*
*    PURPOSE: Starts the driver service.
*
****************************************************************************/
BOOL CFileFilter_Driver::StartDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
	SC_HANDLE  schService;
	BOOL       ret;

	schService = OpenService( SchSCManager, DriverName, SERVICE_ALL_ACCESS);
	if ( schService == NULL )
		return FALSE;

	ret = StartService( schService, 0, NULL )
		|| GetLastError() == ERROR_SERVICE_ALREADY_RUNNING 
		|| GetLastError() == ERROR_SERVICE_DISABLED;

	CloseServiceHandle( schService );

	return ret;
}

/****************************************************************************
*
*    FUNCTION: OpenDevice( IN LPCTSTR, HANDLE *)
*
*    PURPOSE: Opens the device and returns a handle if desired.
*
****************************************************************************/
BOOL CFileFilter_Driver::OpenDevice( IN LPCTSTR DriverName, HANDLE * lphDevice )
{
	TCHAR    completeDeviceName[64];
	HANDLE   hDevice;
	//
	// Create a \\.\XXX device name that CreateFile can use
	//
	// NOTE: We're making an assumption here that the driver
	//       has created a symbolic link using it's own name
	//       (i.e. if the driver has the name "XXX" we assume
	//       that it used IoCreateSymbolicLink to create a
	//       symbolic link "\DosDevices\XXX". Usually, there
	//       is this understanding between related apps/drivers.
	//
	//       An application might also peruse the DEVICEMAP
	//       section of the registry, or use the QueryDosDevice
	//       API to enumerate the existing symbolic links in the
	//       system.
	//
	if( (GetVersion() & 0xFF) >= 5 )
	{
		// We reference the global name so that the application can
		// be executed in Terminal Services sessions on Win2K
		wsprintf( completeDeviceName, TEXT("\\\\.\\Global\\%s"), DriverName );
	}
	else
	{
		wsprintf( completeDeviceName, TEXT("\\\\.\\%s"), DriverName );
	}
	hDevice = CreateFile( completeDeviceName, GENERIC_READ | GENERIC_WRITE,
							0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hDevice == ((HANDLE)-1) )
		return FALSE;

	// If user wants handle, give it to them.  Otherwise, just close it.
	if ( lphDevice )
		*lphDevice = hDevice;
	else
		CloseHandle( hDevice );

	return TRUE;
}

/****************************************************************************
*
*    FUNCTION: StopDriver( IN SC_HANDLE, IN LPCTSTR)
*
*    PURPOSE: Has the configuration manager stop the driver (unload it)
*
****************************************************************************/
BOOL CFileFilter_Driver::StopDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
	SC_HANDLE       schService;
	BOOL            ret;
	SERVICE_STATUS  serviceStatus;

	schService = OpenService( SchSCManager, DriverName, SERVICE_ALL_ACCESS );
	if ( schService == NULL )
		return FALSE;

	ret = ControlService( schService, SERVICE_CONTROL_STOP, &serviceStatus );

	CloseServiceHandle( schService );

	return ret;
}

/****************************************************************************
*
*    FUNCTION: RemoveDriver( IN SC_HANDLE, IN LPCTSTR)
*
*    PURPOSE: Deletes the driver service.
*
****************************************************************************/
BOOL CFileFilter_Driver::RemoveDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
	SC_HANDLE  schService;
	BOOL       ret;

	schService = OpenService( SchSCManager, DriverName, SERVICE_ALL_ACCESS);

	if ( schService == NULL )
		return FALSE;

	ret = DeleteService( schService );

	CloseServiceHandle( schService );

	return ret;
}

/****************************************************************************
*
*    FUNCTION: UnloadDeviceDriver( LPCTSTR)
*
*    PURPOSE: Stops the driver and has the configuration manager unload it.
*
****************************************************************************/
BOOL CFileFilter_Driver::UnloadDeviceDriver( LPCTSTR Name )
{
	SC_HANDLE	schSCManager;

	schSCManager = OpenSCManager(	NULL,                 // machine (NULL == local)
									NULL,                 // database (NULL == default)
									SC_MANAGER_ALL_ACCESS // access required
									);

	StopDriver( schSCManager, Name );
	RemoveDriver( schSCManager, Name );

	CloseServiceHandle( schSCManager );

	return TRUE;
}

/****************************************************************************
*
*    FUNCTION: LoadDeviceDriver( LPCTSTR, LPCTSTR, HANDLE *, PDWORD Error )
*
*    PURPOSE: Registers a driver with the system configuration manager 
*	 and then loads it.
*
****************************************************************************/
BOOL CFileFilter_Driver::LoadDeviceDriver(LPCTSTR Name, LPCTSTR Path, 
									HANDLE * lphDevice, PDWORD Error )
{
	SC_HANDLE	schSCManager;
	BOOL		okay = TRUE;

	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	// Remove previous instance
	RemoveDriver( schSCManager, Name );
	// Ignore success of installation: it may already be installed.
	InstallDriver( schSCManager, Name, Path );
	// Ignore success of start: it may already be started.
	StartDriver( schSCManager, Name );
	// Do make sure we can open it.
	okay = OpenDevice( Name, lphDevice );
	*Error = GetLastError();
	CloseServiceHandle( schSCManager );

	return okay;
}

// 自动加载FileFilter驱动
BOOL CFileFilter_Driver::AutoLoadAndStart(void)
{
	BOOL	bRet = TRUE;
	CString strErrorMsg = "";
	CString strErrorPos = "CFileFilter_Driver::AutoLoadAndStart()";//出错位置描述
	DWORD	dwError = 0;
	DWORD	dwVersionNumber = 0;
	DWORD	dwBytesReturned = 0;

	TCHAR	strWinDriverPath[ MAX_PATH ];
	TCHAR	strSysFileFilterPath[ MAX_PATH ];
	TCHAR	strSystemRoot[ MAX_PATH ];

	// open the handle to the device
	// copy the driver to <winnt>\system32\drivers so that we
	// can run off of a CD or network drive
	if( !GetEnvironmentVariable( "SYSTEMROOT", strSystemRoot, sizeof(strSystemRoot)))
	{
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_GET_WINPATH_FAILURE);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}
	_stprintf(strSysFileFilterPath, _T("%s\\%s"), g_strAppPath, SYS_FILEFILTER );

	sprintf( strWinDriverPath, _T("%s\\system32\\drivers\\%s"), strSystemRoot, SYS_FILEFILTER );
	if( !CopyFile( strSysFileFilterPath, strWinDriverPath, FALSE ))
	{
		//strErrorMsg.Format("将文件[%s]复制到[%s]失败！", strSysFileFilterPath, strWinDriverPath);
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_COPY_FAILURE);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}
	SetFileAttributes( strWinDriverPath, FILE_ATTRIBUTE_NORMAL );

	if( !LoadDeviceDriver( FILEFILTER_NAME, strWinDriverPath, &g_hSys_FileFilter, &dwError ) )
	{
		if( !LoadDeviceDriver( FILEFILTER_NAME, strWinDriverPath, &g_hSys_FileFilter, &dwError ) )
		{
			UnloadDeviceDriver( FILEFILTER_NAME );
			if( !LoadDeviceDriver( FILEFILTER_NAME, strWinDriverPath, &g_hSys_FileFilter, &dwError ) )
			{
				//strErrorMsg.Format("加载驱动文件[%s]失败！", strSysFileFilterPath);
				g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_LOAD_FAILURE);
				g_dlgErrorMsg.DoModal();
				bRet = FALSE;
				DeleteFile( strWinDriverPath );
				return bRet;
			}
		}
		DeleteFile( strWinDriverPath );
	}
	// Correct driver version?
	if( !DeviceIoControl(	g_hSys_FileFilter, IOCTL_FILEMON_VERSION,
							NULL, 0, &dwVersionNumber, sizeof(DWORD), &dwBytesReturned, NULL ) ||
							dwVersionNumber != FILEMONVERSION )
	{
		//strErrorMsg.Format("加载的驱动文件[%s]版本号不正确！如果您安装了一个新的版本，您必须在使用新版本前重新启动您的计算机！", strSysFileFilterPath);
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_WRONGVERSION);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}
	// Have driver zero information
	if ( ! DeviceIoControl(	g_hSys_FileFilter, IOCTL_FILEMON_ZEROSTATS,
							NULL, 0, NULL, 0, &dwBytesReturned, NULL ) )
	{
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_ACCESS_FAILURE);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}
	// Set up the filter
	g_FilterDefinition.excludefilter[0] = 0;
	g_FilterDefinition.includefilter[0] = 0;
	strcpy( g_FilterDefinition.excludefilter, " " );
	strcpy( g_FilterDefinition.includefilter, "*" );
	// tell the driver the initial filter
	if ( ! DeviceIoControl(	g_hSys_FileFilter, IOCTL_FILEMON_SETFILTER,
							&g_FilterDefinition, sizeof(FILTER), NULL, 0, &dwError, NULL ) )
	{
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_ACCESS_FAILURE);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}	
	// Tell driver to start filtering
	if ( ! DeviceIoControl(	g_hSys_FileFilter, IOCTL_FILEMON_STARTFILTER,
							NULL, 0, NULL, 0, &dwError, NULL ) )
	{
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_ACCESS_FAILURE);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}	

	return bRet;
}

//对特定的盘进行文件系统的过滤
DWORD CFileFilter_Driver::HookDriver(DWORD dwNewDriveSet)
{
	DWORD dwRet = 0;
	DWORD dwBytesReturned = 0;

	// Tell device driver which drives to monitor
	if ( ! DeviceIoControl(	g_hSys_FileFilter, IOCTL_FILEMON_SETDRIVES,
							&dwNewDriveSet, sizeof dwNewDriveSet,
							&dwNewDriveSet, sizeof dwNewDriveSet, &dwBytesReturned, NULL ) )
	{
		dwRet = 0;
		return dwRet;
	}
	else
		dwRet = dwNewDriveSet;

	return dwRet;
}

//默认对 FileDisk 虚拟出来的盘加载文件过滤驱动进行过滤
BOOL CFileFilter_Driver::DefaultHookDriver()
{
	BOOL bRet = TRUE;
	DWORD dwDrive = 0;
	DWORD dwNewDriveSet = 0;

	dwDrive = g_cDriveLetter - 'a';
	dwNewDriveSet = g_dwCurDriveSet ^ (1 << dwDrive);
	g_dwCurDriveSet = HookDriver(dwNewDriveSet);

	return bRet;
}

/******************************************************************************
*
*	FUNCTION:	Split
*
*	PURPOSE:	Split a delimited line into components
*
******************************************************************************/
int CFileFilter_Driver::Split( char * line, char delimiter, char * items[] )
{
	int		cnt = 0;

	for (;;)  {
		// Add prefix to list of components		
		items[cnt++] = line;

		// Check for more components
		line = strchr( line, delimiter );
		if ( line == NULL )
			return cnt;

		// Terminate previous component and move to next
		*line++ = '\0';
	}		
}
