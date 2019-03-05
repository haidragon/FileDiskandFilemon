#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Are we running on NT or 9x?
//BOOL					g_bIsNT;

// performance counter frequency
LARGE_INTEGER			g_PerfFrequency;

//Error Message Dialog
CErrorMsgDlg			g_dlgErrorMsg;

char					g_cDriveLetter;//盘符
char					g_strAppPath[MAXPATHLEN];//应用程序所在完整路径

//CString					g_strAppPath;//应用程序所在完整路径

CFileDisk_Driver		g_clsFileDisk_Driver;
CFileFilter_Driver		g_clsFileFilter_Driver;

// Handle to File System Filter device driver
HANDLE					g_hSys_FileFilter = INVALID_HANDLE_VALUE;

// Filter-related
FILTER					g_FilterDefinition;

POPEN_FILE_INFORMATION	g_pOpenFileInformation;

//该变量的每一位表示了相应的盘有无被加载文件过滤驱动，
//每一位的位置与Windows表示磁盘的位置一致（可以参看GetLogicalDrives()函数的帮助说明）
DWORD					g_dwCurDriveSet;// drives that are hooked

// Buffer into which driver can copy statistics
char					g_strStats[ LOGBUFSIZE ];
// Current fraction of buffer filled
DWORD					g_dwStatsLen;
