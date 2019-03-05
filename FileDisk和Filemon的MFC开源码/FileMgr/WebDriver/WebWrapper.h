/////////////////////////////////////////////////////////////////////////////
// WebWrapper.h : header file
//
// CWebWrapper - CHttpFile wrapper class
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

#if !defined(AFX_WebWrapper_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_)
#define AFX_WebWrapper_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebWrapper.h : header file
//

#include <afxinet.h>
#include "WebWrapperSession.h"

/////////////////////////////////////////////////////////////////////////////
// CWebWrapper 

class _WEBDRIVER_API CWebWrapper : public CObject
{
public:
	CWebWrapper();
	virtual ~CWebWrapper();

// Arributes
public:

// Arributes
private:
	CString m_strServerAddress;
	int		m_nServerPort;
	CString m_strServerPath;

	bool	m_bIsShowErrorMessage;//是否显示错误提示信息
	bool	m_bIsAnalyzeServerReply;//是否对服务器返回的数据进行分析
	CString m_strLastErrorMessage;//最后产生的一个错误信息

	double  m_transferRate;
	DWORD	m_infoStatusCode;
	CString	m_rawHeaders;

protected:
	bool    m_bForceReload;

// Operations
public:

	//Http的POST连接
	CWebWrapperSession* m_pSession_POST;
	CHttpConnection* m_pConnection_POST;
	CHttpFile* m_pFile_POST;
	void ReleaseConnect_POST();
	bool InitSession_POST(LPCTSTR szAgentName=NULL, CWnd* pWnd=NULL);
	BOOL PostCommand(CString strHttpHeaders_Send, CString strHttpContent_Send, 
					CHttpDataPackage_Receive* pHttpData_Receive=NULL, 
					LPCTSTR szAgentName=NULL, CWnd* pWnd=NULL, 
					bool bIsShowErrorMessage=true, //是否显示错误提示信息
					bool bIsAnalyzeServerReply=false);//是否对服务器返回的数据进行分析

	//Http的GET连接
	CWebWrapperSession* m_pSession_GET;
	CHttpFile* m_pFile_GET;
	void ReleaseConnect_GET();
	bool InitSession_GET(LPCTSTR szAgentName=NULL, CWnd* pWnd=NULL);
    bool GetCommand(CString strHttpHeaders_Send, CString strHttpContent_Send, 
					CHttpDataPackage_Receive* pHttpData_Receive = NULL, 
					LPCTSTR szAgentName=NULL, CWnd* pWnd=NULL, 
					bool bIsShowErrorMessage=true, //是否显示错误提示信息
					bool bIsAnalyzeServerReply=false);//是否对服务器返回的数据进行分析

	//上传图片的3个连接
	CWebWrapperSession* m_pSession_Upload_Pic;
	CHttpConnection* m_pConnection_Upload_Pic;
	CHttpFile* m_pFile_Upload_Pic;
	void ReleaseConnect_Upload_Pic();
	bool InitSession_Upload_Pic(LPCTSTR szAgentName=NULL, CWnd* pWnd=NULL);
	BOOL PostFilePic(CString strHttpHeaders_Send, BYTE* p, int length, 
					CHttpDataPackage_Receive* pHttpData_Receive=NULL, 
					LPCTSTR szAgentName=NULL, CWnd* pWnd=NULL);

	//获取图片的2个连接
	CWebWrapperSession* m_pSession_Download_Pic;
	CHttpFile* m_pFile_Download_Pic;
	void ReleaseConnect_Download_Pic();
	bool InitSession_Download_Pic(LPCTSTR szAgentName=NULL, CWnd* pWnd=NULL);
    bool DownloadPic(CString strPic_URL, CString& strPic_Buffer,
					LPCTSTR szAgentName=NULL, CWnd* pWnd=NULL);
	//从网上获取图片数据
	BOOL	GetPicDataFromWeb(CWebWrapperSession* pSession, CHttpFile* pFile, 
								DWORD& dwHttpReply_StatusCode, 
								CString& strHttpHeaders_Receive, 
								CString& strHttpContent_Receive);

	bool	GetAndCheckServerReply(CWebWrapperSession* pSession, CHttpFile* pFile, 
									DWORD& dwHttpReply_StatusCode, 
									CString& strHttpHeaders_Receive, 
									CString& strHttpContent_Receive, 
									CHttpDataPackage_Receive* pHttpData_Receive);

	//读取服务器端返回的信息
	bool	GetServerReply(CWebWrapperSession* pSession, CHttpFile* pFile, 
						   DWORD& dwHttpReply_StatusCode, CString& strHttpHeaders_Receive, 
						   CString& strHttpContent_Receive);
	//检测服务器端返回时的 Http 错误
	bool	IsServerHttpError(CString& strServer_HttpError, DWORD dwHttp_StatusCode=0);
	
	//对收到的服务器端返回的信息进行有效性检查
	bool	IsServerReplyDataValid(CString strHttpHeaders_DataLength, 
									CString strHttpContent_Receive);
	//检测服务器返回的是否是出错信息
	bool	IsServerError(CHttpDataPackage_Receive* pHttpData_Receive, 
							CString& strHttpContent_Receive);

	void	Init();
	void	Release();//释放资源

	CString GetRawHeaders() { return m_rawHeaders; };
	CString GetLastErrorMessage() { return m_strLastErrorMessage; };
	DWORD	GetPageStatusCode() { return m_infoStatusCode; };
	double  GetRate() { return m_transferRate; };
	bool	GetForceReload() { return m_bForceReload; };
	void	SetForceReload (bool bForceReload) { m_bForceReload = bForceReload; };

// Overrides
public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebWrapper)
	//}}AFX_VIRTUAL
	// Generated message map functions
	//{{AFX_MSG(CWebWrapper))
	//}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WebWrapper_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_)
