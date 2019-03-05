
#include "stdafx.h"
#include "Commonfunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString _COMMON_API  GetApplicationPath(IN HMODULE hMod)
{
	// returns the path of the current application
	char    filename[_MAX_PATH];
	char	drive[_MAX_DRIVE];
	char	path[_MAX_PATH];

	GetModuleFileName(hMod, filename, MAX_PATH);
	_splitpath(filename, drive, path, NULL, NULL);

	CString	location(drive);

	location += path;
	return location;
}

int _COMMON_API GetApplicationPath(IN HMODULE hModule, IN OUT PCHAR pstrAppPath)
{// returns the path of the current application
	int nRet = 0;
	char	strFileName[_MAX_PATH];
	char	strDrive[_MAX_DRIVE];
	char	strPath[_MAX_PATH];

	GetModuleFileName(hModule, strFileName, MAX_PATH);
	_splitpath(strFileName, strDrive, strPath, NULL, NULL);

	sprintf(pstrAppPath, "%s%s", strDrive, strPath);

	return nRet;
}