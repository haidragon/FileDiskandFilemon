#pragma once

// Variables/definitions for the driver that performs the actual monitoring.
#define	SYS_FILEFILTER			_T("FileFilter.sys")
#define	FILEFILTER_NAME			_T("FileFilter")

// File Disk driver.
#define	SYS_FILEDISK			_T("FileDisk.sys")
#define	FILEDISK_NAME			_T("FileDisk")

// performance counter frequency
extern LARGE_INTEGER			g_PerfFrequency;

//Error Message Dialog
extern CErrorMsgDlg				g_dlgErrorMsg;

extern char						g_cDriveLetter;//盘符
extern char						g_strAppPath[MAXPATHLEN];//应用程序所在完整路径

//extern CString g_strAppPath;//应用程序所在完整路径

extern CFileDisk_Driver			g_clsFileDisk_Driver;
extern CFileFilter_Driver		g_clsFileFilter_Driver;

// Handle to File System Filter device driver
extern HANDLE					g_hSys_FileFilter;

// Filter-related
extern FILTER					g_FilterDefinition;

extern POPEN_FILE_INFORMATION	g_pOpenFileInformation;

//该变量的每一位表示了相应的盘有无被加载文件过滤驱动，
//每一位的位置与Windows表示磁盘的位置一致（可以参看GetLogicalDrives()函数的帮助说明）
extern DWORD					g_dwCurDriveSet;// drives that are hooked

// Buffer into which driver can copy statistics
extern char						g_strStats[ LOGBUFSIZE ];
// Current fraction of buffer filled
extern DWORD					g_dwStatsLen;
