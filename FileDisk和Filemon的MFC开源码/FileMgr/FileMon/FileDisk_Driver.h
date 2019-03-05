#pragma once
#include "driver_base.h"
#include "..\Include\FileDisk.h"

#define FILEDISK_FILENAME "File.img"
#define FILEDISK_SIZE  80 * 1024 * 1024  //80M的磁盘

#define ROOT_MAILBOX "收件箱"
#define ROOT_PRIVATE "个人文件夹"
#define ROOT_SHARE   "共享文件夹"

class CFileDisk_Driver : public CDriver_Base
{
public:
	CFileDisk_Driver(void);
	~CFileDisk_Driver(void);

	int CreateDirectoryAndFile(void);

	int DefaultMount();
	int DefaultUnMount();

	int Mount(int DeviceNumber, POPEN_FILE_INFORMATION OpenFileInformation, char DriveLetter);
	int Umount(char DriveLetter);

	BOOL InstallAndStartService(LPCTSTR ServiceName, LPCTSTR ServiceExe);
	BOOL InstallService(SC_HANDLE hSCManager, LPCTSTR ServiceName, LPCTSTR ServiceExe);
	BOOL StartService(SC_HANDLE hSCManager, LPCTSTR ServiceName);

	BOOL StopAndRemoveService(LPCTSTR ServiceName);
	BOOL StopService(SC_HANDLE hSCManager, LPCTSTR ServiceName);
	BOOL RemoveService(SC_HANDLE hSCManager, LPCTSTR ServiceName);

};
