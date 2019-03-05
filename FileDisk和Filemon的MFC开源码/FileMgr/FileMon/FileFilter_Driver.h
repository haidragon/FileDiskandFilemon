#pragma once

#include "driver_base.h"

class CFileFilter_Driver : public CDriver_Base
{
public:
	CFileFilter_Driver(void);
	~CFileFilter_Driver(void);

	BOOL InstallDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName, IN LPCTSTR ServiceExe );
	BOOL StartDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
	BOOL OpenDevice( IN LPCTSTR DriverName, HANDLE * lphDevice );
	BOOL StopDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
	BOOL RemoveDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
	BOOL UnloadDeviceDriver( LPCTSTR Name );
	BOOL LoadDeviceDriver( LPCTSTR Name, LPCTSTR Path, HANDLE * lphDevice, PDWORD Error );
	//自动加载FileFilter驱动
	BOOL AutoLoadAndStart(void);
	//对特定的盘进行文件系统的过滤
	DWORD HookDriver(DWORD dwNewDriveSet);
	//默认对 FileDisk 虚拟出来的盘加载文件过滤驱动进行过滤
	BOOL DefaultHookDriver();

	/******************************************************************************
	*
	*	FUNCTION:	Split
	*
	*	PURPOSE:	Split a delimited line into components
	*
	******************************************************************************/
	int Split( char * line, char delimiter, char * items[] );
};
