#ifdef _COMMON_LIBRARY_EXPORT
#define _COMMON_API __declspec(dllexport)
#else
#define _COMMON_API __declspec(dllimport)
#endif

#ifndef _COMMON_NOAUTOLIB
#ifdef _DEBUG
#pragma comment(lib, "..\\CommonLib\\Debug\\CommonLib.lib")
#else
#pragma comment(lib, "..\\CommonLib\\Release\\CommonLib.lib")
#endif
#endif

// actual class declarations
#include <afxtempl.h>

#include "Resource.h"
#include "CommonObject.h"
#include "CommonFunction.h"
#include "StringEx.h"
//#include "Md5Encrypt.h"
//#include "Markup.h"
#include "SortHeaderCtrl.h"
#include "ImageListCtrl.h"
#include "ErrorMsgDlg.h"