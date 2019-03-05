/////////////////////////////////////////////////////////////////////////////
// WebWrapperSession.h : header file
//
// WebWrapperSession - CInternetSession wrapper class
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

#if !defined(AFX_WebWrapperSession_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_)
#define AFX_WebWrapperSession_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebWrapperSession.h : header file
//

#include <afxinet.h>

/////////////////////////////////////////////////////////////////////////////
// CWebWrapperSession 

class _WEBDRIVER_API CWebWrapperSession : public CInternetSession
{
// Construction
public:
	CWebWrapperSession(LPCTSTR szAgentName);
	CWebWrapperSession(LPCTSTR szAgentName, CWnd* pStatusWnd);
	virtual ~CWebWrapperSession();
    void CommonConstruct();

//Attributes
public:

// Operations
public:
    void SetStatus(LPCTSTR fmt, ...);
    void SetStatusWnd(CWnd* pWnd)     { m_pStatusWnd = pWnd; }

// Overrides
public:
	CString GetErrorMessage() { return m_strErrorMessage; } ;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebWrapperSession)
	//}}AFX_VIRTUAL
   
	// Generated message map functions
	//{{AFX_MSG(CWebWrapperSession)
	//}}AFX_MSG

// Attributes
protected:
    CWnd* m_pStatusWnd;
	CString m_strErrorMessage;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WebWrapperSession_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_)
