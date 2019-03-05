#if !defined(AFX_WEBDRIVERLIB_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_)
#define AFX_WEBDRIVERLIB_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebDriverLib.h : header file
//

#ifdef _WEBDRIVER_LIBRARY_EXPORT
#define _WEBDRIVER_API __declspec(dllexport)
#else
#define _WEBDRIVER_API __declspec(dllimport)
#endif

#ifndef _WEBDRIVER_NOAUTOLIB
#ifdef _DEBUG
#pragma comment(lib, "..\\WebDriver\\Debug\\WebDriver.lib")
#else
#pragma comment(lib, "..\\WebDriver\\Release\\WebDriver.lib")
#endif
#endif

// actual class declarations
#include <afxtempl.h>
#include "HttpDataPackage.h"
#include "WebWrapper.h"
#include "WebDA.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBDRIVERLIB_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_)
