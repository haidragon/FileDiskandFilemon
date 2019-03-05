#include "stdafx.h"

#include ".\filedisk_driver.h"

CFileDisk_Driver::CFileDisk_Driver(void)
{
	g_cDriveLetter = 'a';
	sprintf(g_strAppPath, "");
}

CFileDisk_Driver::~CFileDisk_Driver(void)
{
	if (DefaultUnMount() >= 0)
	{
		StopAndRemoveService(FILEDISK_NAME);
	}
}

//
//  InstallService
//  安装服务
//  参数：
//      [IN] SC_HANDLE hSCManager   服务管理器句柄
//      [IN] LPCTSTR   ServiceName  服务名称
//      [IN] LPCTSTR   ServiceExe   可执行文件(需全路径)
//  输出：
//  成功：返回TRUE，否则返回FALSE
//
BOOL CFileDisk_Driver::InstallService(SC_HANDLE hSCManager, LPCTSTR ServiceName, LPCTSTR ServiceExe)
{
	SC_HANDLE  schService;

	//
	// so #$%@! ...:)
	//
	schService = CreateService( hSCManager,				 // SCManager database
		ServiceName,             // name of service
		ServiceName,             // name to display
		SERVICE_ALL_ACCESS,      // desired access
		SERVICE_KERNEL_DRIVER,   // service type
		SERVICE_DEMAND_START,    // start type
		SERVICE_ERROR_NORMAL,    // error control type
		ServiceExe,              // service's binary
		NULL,                    // no load ordering group
		NULL,                    // no tag identifier
		NULL,                    // no dependencies
		NULL,                    // LocalSystem account
		NULL                     // no password
		);
	if (schService == NULL)
	{
		if(GetLastError() == ERROR_SERVICE_EXISTS)
		{
			PrintLastError("Service has already installed!\n");
		}
		else
		{
			PrintLastError("======== CreateService() Failed at CFileDisk_Driver::InstallService() ! ===========\n");
			return FALSE;
		}
	}
	CloseServiceHandle(schService);
	return TRUE;
}

//
//  StartService
//  启动服务
//  ----------------
//  参数：
//      [IN] SC_HANDLE hSCManager 服务管理器句柄
//      [IN] LPCTSTR   ServiceName 驱动名称
//  返回值：
//      成功返回TRUE，否则返回FALSE
//
BOOL CFileDisk_Driver::StartService(SC_HANDLE hSCManager, LPCTSTR ServiceName)
{
	SC_HANDLE	schService = NULL;
	BOOL		bRet = FALSE;

	schService = OpenService(hSCManager, ServiceName, SERVICE_ALL_ACCESS);
	if(schService == NULL)
	{
		if(GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST)
		{
			PrintLastError("Service is not exist!\n");
		}
		else
			PrintLastError("========= OpenService() Failed at CFileDisk_Driver::StartService ! =========\n");
		bRet = FALSE;
		return bRet;
	}

	bRet = ::StartService(schService, 0, NULL);
	if(!bRet)
	{
		if(GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
		{
			PrintLastError("Service is already running!\n");
		}
		else
			PrintLastError("========= StartService() Failed at CFileDisk_Driver::StartService ! =========\n");
	}
	CloseServiceHandle(schService);
	return bRet;
}

BOOL CFileDisk_Driver::InstallAndStartService(LPCTSTR ServiceName, LPCTSTR ServiceExe)
{
	SC_HANDLE		hSCManager = NULL;
	BOOL			bRet = FALSE;
	// 打开服务控制管理器
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSCManager == NULL)
	{
		PrintLastError("OpenSCManager() failed. -- Error is: \n");
		bRet = FALSE;
		return bRet;
	}
	if( !InstallService(hSCManager, ServiceName, ServiceExe) )
	{
		PrintLastError("Install Service failed!\n");
		bRet = FALSE;
		return bRet;
	}
	if( !StartService(hSCManager, ServiceName) )
	{
		PrintLastError("Start Service failed!\n");
		bRet = FALSE;
		return bRet;
	}
	CloseServiceHandle(hSCManager);
	bRet = TRUE;
	return bRet;
}
//
//  StopService
//  停止驱动
//  ---------------
//  参数：
//      [IN] SC_HANDLE hSCManager  服务管理器句柄
//      [IN] LPCTSTR   ServiceName 服务名称
//  返回值：
//      成功返回TRUE，否则返回FALSE
//
BOOL CFileDisk_Driver::StopService(SC_HANDLE hSCManager, LPCTSTR ServiceName)
{
	SC_HANDLE      schService = NULL;
	SERVICE_STATUS ServiceStatus;
	BOOL          bRet = FALSE;

	schService = OpenService(hSCManager, ServiceName, SERVICE_ALL_ACCESS);
	if(schService == NULL)
	{
		bRet = FALSE;
		return bRet;
	}
	bRet = ControlService(schService, SERVICE_CONTROL_STOP, &ServiceStatus);
	if(!bRet)
	{
		switch(GetLastError())
		{
		case ERROR_SERVICE_NOT_ACTIVE:
			PrintLastError("Service has stopped!\n");
			break;

		case ERROR_INVALID_SERVICE_CONTROL:
			PrintLastError("The requested control code is not valid!\n");
			break;

		default:
			PrintLastError("====== ControlService() Failed at CFileDisk_Driver::StopService ! ========\n");
			break;
		}
	}
	CloseServiceHandle(schService);
	return bRet;
}

//
//  RemoveService
//  卸载服务
//  ------------
//  参数：
//      [IN] SC_HANDLE hSCManager   服务管理器句柄
//      [IN] LPCTSTR   ServiceName  服务名称
//  返回值：
//      成功返回TRUE，否则返回FALSE
//
BOOL CFileDisk_Driver::RemoveService(SC_HANDLE hSCManager, LPCTSTR ServiceName)
{
	SC_HANDLE	schService;
	BOOL		bRet = 0;

	schService = OpenService(hSCManager, ServiceName, SERVICE_ALL_ACCESS);
	if(schService == NULL)
	{
		bRet = FALSE;
		return bRet;
	}
	bRet = DeleteService(schService);
	if(!bRet)
	{
		PrintLastError("======== DeleteService() Failed at CFileDisk_Driver::RemoveService ! =======\n");
	}
	CloseServiceHandle(schService);
	return bRet;
}

BOOL CFileDisk_Driver::StopAndRemoveService(LPCTSTR ServiceName)
{
	SC_HANDLE		hSCManager = NULL;
	BOOL			bRet = FALSE;
	// 打开服务控制管理器
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSCManager == NULL)
	{
		PrintLastError("OpenSCManager() failed. -- Error is: \n");
		bRet = FALSE;
		return bRet;
	}
	if( !StopService(hSCManager, ServiceName) )
	{
		PrintLastError("Stop Service failed!\n");
		bRet = FALSE;
		return bRet;
	}
	if( !RemoveService(hSCManager, ServiceName) )
	{
		PrintLastError("Remove Service failed!\n");
		bRet = FALSE;
		return bRet;
	}
	CloseServiceHandle(hSCManager);
	bRet = TRUE;
	return bRet;
}

int CFileDisk_Driver::Mount(int DeviceNumber, POPEN_FILE_INFORMATION  OpenFileInformation,
							char DriveLetter)
{
	char    VolumeName[] = "\\\\.\\ :";
	char    DeviceName[255];
	HANDLE  Device;
	DWORD   BytesReturned;

	VolumeName[4] = DriveLetter;

	Device = CreateFile(
		VolumeName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING,
		NULL
		);

	if (Device != INVALID_HANDLE_VALUE)
	{
		SetLastError(ERROR_BUSY);
		//PrintLastError(&VolumeName[4]);
		PrintLastError("First CreateFile failed! At CFileDisk_Driver::Mount()");
		return -1;
	}

	sprintf(DeviceName, DEVICE_NAME_PREFIX "%u", DeviceNumber);

	if (!DefineDosDevice(
		DDD_RAW_TARGET_PATH,
		&VolumeName[4],
		DeviceName
		))
	{
		//PrintLastError(&VolumeName[4]);
		PrintLastError("DefineDosDevice failed! At CFileDisk_Driver::Mount()");
		return -1;
	}

	Device = CreateFile(
		VolumeName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING,
		NULL
		);

	if (Device == INVALID_HANDLE_VALUE)
	{
		//PrintLastError(&VolumeName[4]);
		PrintLastError("Sencond CreateFile failed! At CFileDisk_Driver::Mount()");
		DefineDosDevice(DDD_REMOVE_DEFINITION, &VolumeName[4], NULL);
		return -1;
	}

	if (!DeviceIoControl(
		Device,
		IOCTL_FILE_DISK_OPEN_FILE,
		OpenFileInformation,
		sizeof(OPEN_FILE_INFORMATION) + OpenFileInformation->FileNameLength - 1,
		NULL,
		0,
		&BytesReturned,
		NULL
		))
	{
		//PrintLastError("FileDisk:");
		PrintLastError("DeviceIoControl failed! At CFileDisk_Driver::Mount()");
		DefineDosDevice(DDD_REMOVE_DEFINITION, &VolumeName[4], NULL);
		CloseHandle(Device);
		return -1;
	}
	CloseHandle(Device);

	//CreateDirectoryAndFile();
	return 0;
}

int CFileDisk_Driver::DefaultMount()
{
	int                     nCount = 0;
	char                    strSysFileName[MAXPATHLEN];
	char                    FileName[MAXPATHLEN];
	//POPEN_FILE_INFORMATION    OpenFileInformation;

	g_cDriveLetter = GetDriveLetter();//'f';

	sprintf(strSysFileName, "%s%s", g_strAppPath, SYS_FILEDISK);

	sprintf(FileName, "%s%s", g_strAppPath, FILEDISK_FILENAME);

	if (DefaultUnMount() >= 0)
	{
		StopAndRemoveService(FILEDISK_NAME);
	}
	if(!InstallAndStartService(FILEDISK_NAME, strSysFileName))
	{
		//尝试一次卸载
		g_cDriveLetter -= 1;
		if (Umount(g_cDriveLetter) >= 0)
		{
			StopAndRemoveService(FILEDISK_NAME);
		}
		if(!InstallAndStartService(FILEDISK_NAME, strSysFileName))
		{
			PrintLastError("InstallAndStartService() Failed!\n");
			return -1;
		}
	}

	nCount = (int) strlen(FileName);
	g_pOpenFileInformation = (POPEN_FILE_INFORMATION) malloc(sizeof(OPEN_FILE_INFORMATION) + nCount + 4);
	memset(g_pOpenFileInformation, 0, sizeof(OPEN_FILE_INFORMATION) + nCount + 4);

	strcpy((char *)g_pOpenFileInformation->FileName, _T("\\??\\"));
	strcat((char *)g_pOpenFileInformation->FileName, FileName);

	g_pOpenFileInformation->FileNameLength = (int) strlen((char *)g_pOpenFileInformation->FileName);

	g_pOpenFileInformation->FileSize.QuadPart = FILEDISK_SIZE;//80M File Disk

	return Mount(0, g_pOpenFileInformation, g_cDriveLetter);
}

int CFileDisk_Driver::Umount(char DriveLetter)
{
	char    VolumeName[] = "\\\\.\\ :";
	HANDLE  Device;
	DWORD   BytesReturned;

	VolumeName[4] = DriveLetter;

	Device = CreateFile(
		VolumeName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING,
		NULL
		);

	if (Device == INVALID_HANDLE_VALUE)
	{
		//PrintLastError(&VolumeName[4]);
		PrintLastError("CreateFile failed! At CFileDisk_Driver::Umount()");
		return -1;
	}

	//去掉这里的 FSCTL_LOCK_VOLUME 和后面的 FSCTL_UNLOCK_VOLUME
	//是为了避免在资源管理器里焦点还停留在虚拟盘时，该函数返回“拒绝访问”的错误
	/*if (!DeviceIoControl( Device, FSCTL_LOCK_VOLUME, NULL, 0,
							NULL, 0, &BytesReturned, NULL))
	{
		PrintLastError(&VolumeName[4]);
		CloseHandle(Device);
		return -1;
	}//*/

	if (!DeviceIoControl(
		Device,
		IOCTL_FILE_DISK_CLOSE_FILE,
		NULL,
		0,
		NULL,
		0,
		&BytesReturned,
		NULL
		))
	{
		//PrintLastError("FileDisk:");
		PrintLastError("DeviceIoControl IOCTL_FILE_DISK_CLOSE_FILE failed! At CFileDisk_Driver::Umount()");
		CloseHandle(Device);
		return -1;
	}

	if (!DeviceIoControl(
		Device,
		FSCTL_DISMOUNT_VOLUME,
		NULL,
		0,
		NULL,
		0,
		&BytesReturned,
		NULL
		))
	{
		//PrintLastError(&VolumeName[4]);
		PrintLastError("DeviceIoControl FSCTL_DISMOUNT_VOLUME failed! At CFileDisk_Driver::Umount()");
		CloseHandle(Device);
		return -1;
	}

	//去掉这里的 FSCTL_LOCK_VOLUME 和后面的 FSCTL_UNLOCK_VOLUME
	//是为了避免在资源管理器里焦点还停留在虚拟盘时，该函数返回“拒绝访问”的错误
	/*if (!DeviceIoControl( Device, FSCTL_UNLOCK_VOLUME, NULL, 0,
							NULL, 0, &BytesReturned, NULL))
	{
		PrintLastError(&VolumeName[4]);
		CloseHandle(Device);
		return -1;
	}//*/

	CloseHandle(Device);

	if (!DefineDosDevice(
		DDD_REMOVE_DEFINITION,
		&VolumeName[4],
		NULL
		))
	{
		//PrintLastError(&VolumeName[4]);
		PrintLastError("DefineDosDevice failed! At CFileDisk_Driver::Umount()");
		return -1;
	}

	return 0;
}

int CFileDisk_Driver::DefaultUnMount()
{
	int nRet = 0;
	nRet = Umount(g_cDriveLetter);
	return nRet;
}

int CFileDisk_Driver::CreateDirectoryAndFile(void)
{
	int nRet = 0;

	HANDLE hFile = NULL;
	char strDirectoryName[MAXPATHLEN];
	char strFileName[MAXPATHLEN];

	sprintf(strDirectoryName, "%c:\\%s", g_cDriveLetter, ROOT_MAILBOX);
	if(CreateDirectory(_T(strDirectoryName), NULL) == FALSE)
	{
		if(GetLastError() != ERROR_ALREADY_EXISTS)
		{
			PrintLastError("创建目录失败！");
			nRet = -1;
			return nRet;
		}
	}
	sprintf(strFileName, "%s\\Dir.txt", strDirectoryName);
	hFile = CreateFile(_T(strFileName), GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, NULL, CREATE_NEW, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		PrintLastError("创建文件失败！");
		nRet = -1;
		return nRet;
	}
	CloseHandle(hFile);

	sprintf(strDirectoryName, "%c:\\%s", g_cDriveLetter, ROOT_PRIVATE);
	if(CreateDirectory(_T(strDirectoryName), NULL) == FALSE)
	{
		if(GetLastError() != ERROR_ALREADY_EXISTS)
		{
			PrintLastError("创建目录失败！");
			nRet = -1;
			return nRet;
		}
	}

	sprintf(strDirectoryName, "%c:\\%s", g_cDriveLetter, ROOT_SHARE);
	if(CreateDirectory(_T(strDirectoryName), NULL) == FALSE)
	{
		if(GetLastError() != ERROR_ALREADY_EXISTS)
		{
			PrintLastError("创建目录失败！");
			nRet = -1;
			return nRet;
		}
	}

	return nRet;
}