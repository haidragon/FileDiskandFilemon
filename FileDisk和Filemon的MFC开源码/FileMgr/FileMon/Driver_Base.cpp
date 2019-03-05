#include "stdafx.h"

#include ".\driver_base.h"

CDriver_Base::CDriver_Base(void)
{
}

CDriver_Base::~CDriver_Base(void)
{
}

char CDriver_Base::GetDriveLetter(void)
{
	//  Search for an unused drive letter
	DWORD dwDriveBitmaps = GetLogicalDrives();
	const int BITS_OF_DWORD = 32;
	int iDrive;
	DWORD dwMask = 0x8;
	char cDriveLetter = 'a';

	for( iDrive = 3; iDrive < BITS_OF_DWORD; iDrive ++ )
	{
		if( dwMask & dwDriveBitmaps )
		{
			dwMask <<= 1;
		}
		else
		{
			break;
		}
	}

	cDriveLetter += iDrive;

	return cDriveLetter;
}

void CDriver_Base::PrintLastError(IN char* Prefix)
{
	/*LPVOID lpMsgBuf;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), 0, (LPTSTR) &lpMsgBuf, 0, NULL);
	fprintf(stderr, "%s %s", Prefix, (LPTSTR) lpMsgBuf);
	LocalFree(lpMsgBuf);//*/

	char strErrorMsg[1024];
	GetLastErrorMsg(Prefix, strErrorMsg);
	TRACE0(strErrorMsg);
	fprintf(stderr, "%s", strErrorMsg);
}

void CDriver_Base::GetLastErrorMsg(IN char* pstrPrefix, IN OUT char* pstrErrorMsg)
{
	LPVOID lpMsgBuf;

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, GetLastError(), 0, (LPTSTR) &lpMsgBuf, 0, NULL);

	sprintf(pstrErrorMsg, "%s. ´íÎóÔ­ÒòÎª: %s", pstrPrefix, (LPTSTR) lpMsgBuf);

	TRACE0(pstrErrorMsg);

	LocalFree(lpMsgBuf);
}
