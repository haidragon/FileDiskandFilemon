/////////////////////////////////////////////////////////////////////////////
// WebWrapperSession.cpp : implementation file
//
// CWebWrapperSession - CInternetSession wrapper class
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// History: 19 Nov 1999 - Release
//          26 Jan 2002 - Update by Bryce to include Proxy support and
//                        property accessors (transfer rate, error msg
//                        etc)
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebWrapperSession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebWrapperSession

CWebWrapperSession::CWebWrapperSession(LPCTSTR szAgentName) 
: CInternetSession(szAgentName) // , 1, INTERNET_OPEN_TYPE_PRECONFIG, 
{
	CommonConstruct();
}

CWebWrapperSession::CWebWrapperSession(LPCTSTR szAgentName, CWnd* pStatusWnd) 
: CInternetSession(szAgentName) //, 1, INTERNET_OPEN_TYPE_PRECONFIG, 
//                  NULL, NULL, INTERNET_FLAG_ASYNC)
{
	CommonConstruct();
	m_pStatusWnd = pStatusWnd;
}

CWebWrapperSession::~CWebWrapperSession()
{
}

void CWebWrapperSession::CommonConstruct() 
{
	m_pStatusWnd = NULL;
	if(!SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000))
	{
		//AfxMessageBox("设置超时时间失败！");
	}
	try
	{
		EnableStatusCallback(TRUE);
	}
	catch (...)
	{}
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CWebWrapperSession, CInternetSession)
	//{{AFX_MSG_MAP(CWebWrapperSession)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CWebWrapperSession member functions

void CWebWrapperSession::SetStatus(LPCTSTR fmt, ...)
{
	va_list args;
	TCHAR buffer[512];

	va_start(args, fmt);
	_vstprintf(buffer, fmt, args);
	va_end(args);

	TRACE1("\nCWebWrapperSession::SetStatus: %s\n", buffer);
	m_strErrorMessage = (CString) buffer;
	if (m_pStatusWnd)
	{
		m_pStatusWnd->SetWindowText(buffer);
		m_pStatusWnd->RedrawWindow();
	}
}
