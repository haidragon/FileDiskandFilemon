/////////////////////////////////////////////////////////////////////////////
// WebWrapper.cpp : implementation file
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

#include "stdafx.h"
#include "WebWrapper.h"
#include "Server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebWrapper

CWebWrapper::CWebWrapper()
{
	Init();
}

CWebWrapper::~CWebWrapper()
{
    Release();
}

void CWebWrapper::Init()
{
	m_strServerAddress = BLOG_CONNECT_SERVER_ADDRESS;
	m_nServerPort = BLOG_CONNECT_SERVER_PORT;
	m_strServerPath = BLOG_CONNECT_SERVER_PATH;

	m_infoStatusCode=0;
	//这个标志是为了避免前后两次提交同一个请求时，不用再到服务器里去数据
	//而是到缓冲里去取, 如果设置为true, 则每次都会重新提交
	m_bForceReload = true;

	m_bIsShowErrorMessage = true;//是否显示错误提示信息
	m_bIsAnalyzeServerReply = false;//是否对服务器返回的数据进行分析

	m_pSession_Upload_Pic = NULL;
	m_pConnection_Upload_Pic = NULL;
	m_pFile_Upload_Pic = NULL;

	m_pSession_Download_Pic = NULL;
	m_pFile_Download_Pic = NULL;

	m_pSession_GET = NULL;
	m_pFile_GET = NULL;

	m_pFile_POST = NULL;
	m_pConnection_POST = NULL;
	m_pSession_POST = NULL;
}

void CWebWrapper::Release()
{
	ReleaseConnect_Download_Pic();
	ReleaseConnect_Upload_Pic();
	ReleaseConnect_GET();
	ReleaseConnect_POST();
}

//应用于上传图片
BOOL CWebWrapper::PostFilePic(CString strHttpHeaders_Send, BYTE* p, int length,
							  CHttpDataPackage_Receive* pHttpData_Receive, 
							  LPCTSTR szAgentName, CWnd* pWnd)
{
	BOOL bRet = true;
	DWORD dwHttpReply_StatusCode = 0;
	CString strHttpHeaders_Receive = _T("");
	CString strHttpContent_Receive = _T("");
	pHttpData_Receive->SetContent_DataAll(_T(""));

    if(!m_pSession_Upload_Pic && !InitSession_Upload_Pic(szAgentName, pWnd) &&
		!pHttpData_Receive)
 	{
		bRet = false;
		//AfxMessageBox(ERROR_WEB_INVALID_WEBCONNECTION, MB_ICONSTOP);
		goto PostPic_RETURN;
	}
    if (pWnd)
        m_pSession_Upload_Pic->SetStatusWnd(pWnd);
    try
    {
		CString strURL = BLOG_CONNECT_SERVER_URL;
	     m_pConnection_Upload_Pic = m_pSession_Upload_Pic->GetHttpConnection(m_strServerAddress, (INTERNET_PORT)m_nServerPort);
		 if(m_pConnection_Upload_Pic == NULL)
		 {
			 bRet = false;
			 //AfxMessageBox(_T("connect failed when uploading picture!"));
			 goto PostPic_RETURN;
		 }
		 strHttpHeaders_Send +=_T("\r\nContent-Type: application/x-www-form-urlencoded");
		 m_pFile_Upload_Pic = m_pConnection_Upload_Pic->OpenRequest(CHttpConnection::HTTP_VERB_POST, m_strServerPath);//szURL); 
		 bRet = m_pFile_Upload_Pic->SendRequest(strHttpHeaders_Send, (LPVOID)p, length);
	}
    catch (CInternetException* e)
    {
        TCHAR szCause[255];
        e->GetErrorMessage(szCause, 255);
        e->Delete();
		bRet = false;
		m_strLastErrorMessage = ERROR_WEB_FAILED_WEBCONNECTION;
		//AfxMessageBox(m_strLastErrorMessage, MB_ICONSTOP);
		goto PostPic_RETURN;
    }//*/
	bRet = GetAndCheckServerReply(m_pSession_Upload_Pic, 
								m_pFile_Upload_Pic, 
								dwHttpReply_StatusCode, strHttpHeaders_Receive, 
								strHttpContent_Receive, 
								pHttpData_Receive);

PostPic_RETURN:
	//操作完成，必须释放资源
	ReleaseConnect_Upload_Pic();
	return bRet;//*/
}//*/

bool CWebWrapper::InitSession_Upload_Pic(LPCTSTR szAgentName, CWnd* pWnd)
{
    ReleaseConnect_Upload_Pic();
	m_infoStatusCode = 0;
    m_pSession_Upload_Pic = new CWebWrapperSession(szAgentName, pWnd);
	return (m_pSession_Upload_Pic != NULL);
}

//释放上传图片所使用的3个连接资源
void CWebWrapper::ReleaseConnect_Upload_Pic()
{
	if(m_pFile_Upload_Pic != NULL)
	{
		m_pFile_Upload_Pic->Close();
		delete m_pFile_Upload_Pic;
		m_pFile_Upload_Pic = NULL;
	}
	if(m_pConnection_Upload_Pic != NULL)
	{
		m_pConnection_Upload_Pic->Close();
		delete m_pConnection_Upload_Pic;
		m_pConnection_Upload_Pic = NULL;
	}
    if(m_pSession_Upload_Pic != NULL)
    {
        m_pSession_Upload_Pic->Close();
        delete m_pSession_Upload_Pic;
		m_pSession_Upload_Pic = NULL;
    }
}//*/

bool CWebWrapper::DownloadPic(CString strPic_URL, CString& strPic_Buffer, 
							  LPCTSTR szAgentName, CWnd* pWnd)
{
	bool bRet = TRUE;
	DWORD dwHttpReply_StatusCode = 0;
	CString strHttpHeaders_Receive = _T("");
	CString strHttpContent_Receive = _T("");

    if (!m_pSession_Download_Pic && !InitSession_Download_Pic(szAgentName, pWnd))
	{
		bRet = FALSE;
		AfxMessageBox(ERROR_WEB_INVALID_WEBCONNECTION, MB_ICONSTOP);
		goto DOWNLOAD_PIC_RETURN;
	}
    if (pWnd)
        m_pSession_Download_Pic->SetStatusWnd(pWnd);
    try
    {
        DWORD dwFlags = INTERNET_FLAG_TRANSFER_ASCII;
        if (m_bForceReload)
		{
            dwFlags |= INTERNET_FLAG_RELOAD;
        }
        m_pFile_Download_Pic = (CHttpFile*)m_pSession_Download_Pic->OpenURL(strPic_URL, 
											1, dwFlags);
	}
    catch (CInternetException* e)
    {
        TCHAR szCause[255];
        e->GetErrorMessage(szCause, 255);
        e->Delete();
		m_strLastErrorMessage = ERROR_WEB_FAILED_WEBCONNECTION;
		//AfxMessageBox(m_strLastErrorMessage, MB_ICONSTOP);
		bRet = false;
		goto DOWNLOAD_PIC_RETURN;
    }
	if(GetPicDataFromWeb(m_pSession_Download_Pic, m_pFile_Download_Pic, 
					dwHttpReply_StatusCode, strHttpHeaders_Receive, strHttpContent_Receive))
	{//strHttpContent_Receive 中包含有完整的服务器端返回的字符串，而且没有被分析过
		//检测服务器端返回时的 Http 错误
		if(dwHttpReply_StatusCode!=200)
		{
			bRet = false;
			goto DOWNLOAD_PIC_RETURN;
		}
		else
		{
			strPic_Buffer = strHttpContent_Receive;
			bRet = true;
		}
	}
	else
	{
		bRet = false;
	}
DOWNLOAD_PIC_RETURN:
	//操作完成，必须释放资源
	ReleaseConnect_Download_Pic();
	return bRet;//*/
}

bool CWebWrapper::InitSession_Download_Pic(LPCTSTR szAgentName, CWnd* pWnd)
{
    ReleaseConnect_Download_Pic();
	m_infoStatusCode = 0;
    m_pSession_Download_Pic = new CWebWrapperSession(szAgentName, pWnd);
	return (m_pSession_Download_Pic != NULL);
}

//释放下载图片所使用的2个连接资源
void CWebWrapper::ReleaseConnect_Download_Pic()
{
	if(m_pFile_Download_Pic != NULL)
	{
		m_pFile_Download_Pic->Close();
		delete m_pFile_Download_Pic;
		m_pFile_Download_Pic = NULL;
	}
    if(m_pSession_Download_Pic != NULL)
    {
        m_pSession_Download_Pic->Close();
        delete m_pSession_Download_Pic;
		m_pSession_Download_Pic = NULL;
    }
}//*/

BOOL CWebWrapper::GetPicDataFromWeb(CWebWrapperSession* pSession, CHttpFile* pFile, 
									DWORD& dwHttpReply_StatusCode, CString& strHttpHeaders_Receive, 
									CString& strHttpContent_Receive)
{
	BOOL bRet = TRUE;
	CTime startTime = CTime::GetCurrentTime();
	CString strBuffer = _T("");
    if((pSession == NULL) || (pFile == NULL))
    {
		CString strError = ERROR_WEB_INVALID_WEBCONNECTION;
		bRet = false;
		AfxMessageBox(strError, MB_ICONSTOP);
	}
	else
    {
        BYTE buffer[BUFFER_SIZE+1];
        try
		{
            UINT nRead = 0;
            DWORD dwBufferCount = 0;
			nRead = pFile->Read(buffer, BUFFER_SIZE);
            while(nRead > 0)
            {
                buffer[nRead] = 0;
                LPTSTR ptr = strBuffer.GetBufferSetLength(dwBufferCount + nRead + 1);
                memcpy(ptr+dwBufferCount, buffer, nRead);
                dwBufferCount += nRead;
                strBuffer.ReleaseBuffer(dwBufferCount+1);
                CTimeSpan elapsed = CTime::GetCurrentTime() - startTime;
                double dSecs = (double)elapsed.GetTotalSeconds();
                if (dSecs > 0.0)
				{
                    m_transferRate = (double)dwBufferCount / 1024.0 / dSecs;
					pSession->SetStatus(_T("Read %d bytes (%0.1f Kb/s)"), 
                                         dwBufferCount, m_transferRate );
				}
				else
				{
                    pSession->SetStatus(_T("Read %d bytes"), dwBufferCount);
					m_transferRate = dwBufferCount;
				}
				nRead = pFile->Read(buffer, BUFFER_SIZE);
            }
           //*/
        }
        catch (CFileException *e)
        {
            TCHAR   szCause[255];
            e->GetErrorMessage(szCause, 255);
            e->Delete();
			m_strLastErrorMessage = ERROR_WEB_FAILED_WEBCONNECTION;
			AfxMessageBox(m_strLastErrorMessage, MB_ICONSTOP);
			bRet = FALSE;
			return bRet;
        }
		if(bRet != false)
		{
			strHttpHeaders_Receive.Empty();
			pFile->QueryInfoStatusCode(dwHttpReply_StatusCode);
			pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strHttpHeaders_Receive);
			strHttpContent_Receive = strBuffer;
		}
	}
	return bRet;
}

bool CWebWrapper::GetCommand(CString strHttpHeaders_Send, CString strHttpContent_Send,
							 CHttpDataPackage_Receive* pHttpData_Receive, 
							 LPCTSTR szAgentName, CWnd* pWnd, 
							 bool bIsShowErrorMessage, //是否显示错误提示信息
							 bool bIsAnalyzeServerReply)//是否对服务器返回的数据进行分析
{
	bool bRet = true;
	DWORD dwHttpReply_StatusCode = 0;
	CString strHttpHeaders_Receive = _T("");
	CString strHttpContent_Receive = _T("");

	m_bIsShowErrorMessage = bIsShowErrorMessage;
	m_bIsAnalyzeServerReply = bIsAnalyzeServerReply;

	pHttpData_Receive->SetContent_DataAll(_T(""));
    if (!m_pSession_GET && 
		!InitSession_GET(szAgentName, pWnd) && 
		!pHttpData_Receive)
	{
		bRet = false;
		m_strLastErrorMessage = ERROR_WEB_INVALID_WEBCONNECTION;
		goto LOGIN_RETURN;
	}
    if (pWnd)
        m_pSession_GET->SetStatusWnd(pWnd);
    try
    {
        DWORD dwFlags = INTERNET_FLAG_TRANSFER_ASCII;
        if (m_bForceReload)
		{
            dwFlags |= INTERNET_FLAG_RELOAD;
        }
		CString strURL = BLOG_CONNECT_SERVER_URL;
		strURL += strHttpContent_Send;

        m_pFile_GET = 
			(CHttpFile*)m_pSession_GET->OpenURL(strURL, 1, 
			dwFlags, strHttpHeaders_Send, -1L);//*/
	}
    catch (CInternetException* e)
    {
        TCHAR szCause[255];
        e->GetErrorMessage(szCause, 255);
        e->Delete();
		bRet = false;
		m_strLastErrorMessage = ERROR_WEB_FAILED_WEBCONNECTION;
		goto LOGIN_RETURN;
    }//*/
	bRet = GetAndCheckServerReply(m_pSession_GET, 
								m_pFile_GET, 
								dwHttpReply_StatusCode, strHttpHeaders_Receive, 
								strHttpContent_Receive, 
								pHttpData_Receive);

LOGIN_RETURN:
	//操作完成，必须释放资源
	ReleaseConnect_GET();
	if((bRet == false) && (m_bIsShowErrorMessage == true))
	{
		AfxMessageBox(m_strLastErrorMessage, MB_ICONSTOP);
	}
	return bRet;//*/
}

bool CWebWrapper::InitSession_GET(LPCTSTR szAgentName, CWnd* pWnd)
{
    ReleaseConnect_GET();
	m_infoStatusCode = 0;
    m_pSession_GET = new CWebWrapperSession(szAgentName, pWnd);
	return (m_pSession_GET != NULL);
}

void CWebWrapper::ReleaseConnect_GET()
{
	if(m_pFile_GET != NULL)
	{
		m_pFile_GET->Close();
		delete m_pFile_GET;
		m_pFile_GET = NULL;
	}
    if(m_pSession_GET != NULL)
    {
        m_pSession_GET->Close();
        delete m_pSession_GET;
		m_pSession_GET = NULL;
    }
}//*/

bool CWebWrapper::GetAndCheckServerReply(CWebWrapperSession* pSession, 
										 CHttpFile* pFile, 
										 DWORD& dwHttpReply_StatusCode, 
										 CString& strHttpHeaders_Receive, 
										 CString& strHttpContent_Receive, 
										 CHttpDataPackage_Receive* pHttpData_Receive)
{
	bool bRet = true;

	if(GetServerReply(pSession, pFile, 
					dwHttpReply_StatusCode, strHttpHeaders_Receive, strHttpContent_Receive))
	{//strHttpContent_Receive 中包含有完整的服务器端返回的字符串，而且没有被分析过
		//检测服务器端返回时的 Http 错误
		if(IsServerHttpError(strHttpContent_Receive, dwHttpReply_StatusCode))
		{
			bRet = false;
			return bRet;
		}
		//分析、拆分Http头信息，并填写数据到 pHttpData_Receive 中的相应部分中去
		if(!pHttpData_Receive->SetHttp_ReceiveHeaders(strHttpHeaders_Receive))
		{
			bRet = false;
			return bRet;
		}
		//对收到的服务器端返回的信息进行有效性检查
		/*if(!IsServerReplyDataValid(pHttpData_Receive->GetHeader_DataLength(), 
									strHttpContent_Receive))
		{
			bRet = false;
			return bRet;
		}//*/
		//strHttpContent_Receive 在执行下面功能函数前包含有完整的服务器端返回的字符串，
		//而且没有被分析过；而在执行完以下函数后，则只包含相关的数据部分或者出错信息
		if(!pHttpData_Receive->SetHttp_ReceiveContent(strHttpContent_Receive, m_bIsAnalyzeServerReply))
		{
			bRet = false;
			m_strLastErrorMessage = strHttpContent_Receive;
			return bRet;
		}
		//检测是否是服务器端返回的 E 字母开头的错误代码信息
		/*if(IsServerError(pHttpData_Receive, strHttpContent_Receive))
		{
			bRet = false;
			return bRet;
		}//*/
		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool CWebWrapper::GetServerReply(CWebWrapperSession* pSession, CHttpFile* pFile, 
								 DWORD& dwHttpReply_StatusCode, 
								 CString& strHttpHeaders_Receive, 
								 CString& strHttpContent_Receive)
{
	bool bRet = true;
	CTime startTime = CTime::GetCurrentTime();
    if((pSession == NULL) || (pFile == NULL))
    {
		bRet = false;
		m_strLastErrorMessage = ERROR_WEB_INVALID_WEBCONNECTION;
	}
	else
	{
        BYTE buffer[BUFFER_SIZE+1];
        try
		{
            UINT nRead = 0;
			UINT nIndex = 0;
            DWORD dwBufferCount = 0;
			CString strTemp = _T("");
            do
            {
                nRead = pFile->Read(buffer, BUFFER_SIZE);
                if (nRead > 0)
                {
                    buffer[nRead] = 0;
					for(nIndex=0; nIndex<nRead; nIndex++)
					{
						strTemp.Empty();
						strTemp.Format(_T("%c"), (_TCHAR)buffer[nIndex]);
						strHttpContent_Receive += strTemp;
					}
					dwBufferCount += nRead;
                    CTimeSpan elapsed = CTime::GetCurrentTime() - startTime;
                    double dSecs = (double)elapsed.GetTotalSeconds();
                    if (dSecs > 0.0)
					{
                        m_transferRate = (double)dwBufferCount / 1024.0 / dSecs;
						/*pSession->SetStatus("\nRead %d bytes (%0.1f Kb/s)", 
                                             dwBufferCount, m_transferRate );//*/
					}
					else
					{
                        //pSession->SetStatus("\nRead %d bytes", dwBufferCount);
						m_transferRate = dwBufferCount;
					}
                }
            }
            while (nRead > 0);//*/
        }
        catch (CFileException *e)
        {
            TCHAR   szCause[255];
            e->GetErrorMessage(szCause, 255);
            e->Delete();
			bRet = false;
			m_strLastErrorMessage = ERROR_WEB_FAILED_WEBCONNECTION;
        }
		if(bRet != false)
		{
			strHttpHeaders_Receive.Empty();
			pFile->QueryInfoStatusCode(dwHttpReply_StatusCode);
			pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strHttpHeaders_Receive);
			if(!strHttpContent_Receive.IsEmpty())
			{
				bRet = true;
			}
			else
			{
				bRet = false;
			}
		}
    }
	return bRet;
}

//对收到的服务器端返回的信息进行有效性检查
bool CWebWrapper::IsServerReplyDataValid(CString strHttpHeaders_DataLength, 
										 CString strHttpContent_Receive)
{
	bool bRet = true;
	CString strError = _T("");

	if(!strHttpHeaders_DataLength.IsEmpty() && !strHttpContent_Receive.IsEmpty())
	{
		//数据总长度的检测
		int nData_Length = atoi(strHttpHeaders_DataLength);
		int nHttpContent_Length = strHttpContent_Receive.GetLength();
		if(nHttpContent_Length != nData_Length)
		{
			bRet = false;
			m_strLastErrorMessage = ERROR_WEB_RECEIVE_INVALID_DATA;
		}
	}
	else
	{
		if(strHttpHeaders_DataLength.IsEmpty())
		{
			m_strLastErrorMessage = ERROR_WEB_RECEIVE_EMPTY_HEADERS;
		}
		if(strHttpContent_Receive.IsEmpty())
		{
			m_strLastErrorMessage = ERROR_WEB_RECEIVE_EMPTY_CONTENT;
		}
		bRet = true;
	}

	return bRet;
}

//检测服务器端返回时的 Http 错误
bool CWebWrapper::IsServerHttpError(CString& strServer_HttpError, DWORD dwHttp_StatusCode)
{
	bool bRet = false;

	if(dwHttp_StatusCode != 200)//有错误发生
	{
		//下面检测 HTTP 连接出错代码
		switch(dwHttp_StatusCode)
		{
		case 400://Unintelligble request
			m_strLastErrorMessage = ERROR_WEB_HTTP400;
			break;

		case 404://Requested URL not found
			m_strLastErrorMessage = ERROR_WEB_HTTP404;
			break;

		case 405://Server does not support requested method
			m_strLastErrorMessage = ERROR_WEB_HTTP405;
			break;

		case 500://Unknown server error
			m_strLastErrorMessage = ERROR_WEB_HTTP500;
			break;

		case 503://Server capacity reached
			m_strLastErrorMessage = ERROR_WEB_HTTP503;
			break;

		default:
			m_strLastErrorMessage = ERROR_WEB_HTTPUNKNOWN;
			break;
		}//*/
		bRet = true;
	}
	else
	{//dwHttp_StatusCode == 200 , 该次操作成功
		bRet = false;
	}
	return bRet;
}

//检测服务器返回的是否是出错信息
//返回 false 表示不是出错信息
bool CWebWrapper::IsServerError(CHttpDataPackage_Receive* pHttpData_Receive, 
								CString& strHttpContent_Receive)
{
	bool bRet = false;
	CString strError = _T("");

	if(pHttpData_Receive != NULL)
	{
		CString strTransferType = pHttpData_Receive->GetContent_TransferType();
		if(!strTransferType.IsEmpty())
		{
			if(strTransferType == "E")
			{//是服务器返回的出错信息				
				bRet = true;
				m_strLastErrorMessage = pHttpData_Receive->GetContent_DataAll();
			}
		}
		else
		{
			bRet = true;
			m_strLastErrorMessage = ERROR_WEB_RECEIVE_TRANSFERTYPE_UNKOWN;
		}
	}
	else
	{
		bRet = true;
		m_strLastErrorMessage = ERROR_WEB_NO_RECEIVE_HTTPDATAPACKAGE;
	}
	return bRet;
}

//用post方式发送数据
BOOL CWebWrapper::PostCommand(CString strHttpHeaders_Send, CString strHttpContent_Send, 
							  CHttpDataPackage_Receive* pHttpData_Receive, 
							  LPCTSTR szAgentName, CWnd* pWnd, 
							  bool bIsShowErrorMessage, //是否显示错误提示信息
							  bool bIsAnalyzeServerReply)//是否对服务器返回的数据进行分析
{
	BOOL bRet = true;
	DWORD dwHttpReply_StatusCode = 0;
	CString strHttpHeaders_Receive = _T("");
	CString strHttpContent_Receive = _T("");
	pHttpData_Receive->SetContent_DataAll(_T(""));

	m_bIsShowErrorMessage = bIsShowErrorMessage;
	m_bIsAnalyzeServerReply = bIsAnalyzeServerReply;

    if(!m_pSession_POST && !InitSession_POST(szAgentName, pWnd) &&
		!pHttpData_Receive)
 	{
		bRet = false;
		m_strLastErrorMessage = ERROR_WEB_INVALID_WEBCONNECTION;
		goto PostCommand_RETURN;
	}
    if (pWnd)
        m_pSession_POST->SetStatusWnd(pWnd);
    try
    {
		CString strURL = BLOG_CONNECT_SERVER_URL;
	     m_pConnection_POST = m_pSession_POST->GetHttpConnection(m_strServerAddress, (INTERNET_PORT)m_nServerPort);
		 if(m_pConnection_POST == NULL)
		 {
			 bRet = false;
			 goto PostCommand_RETURN;
		 }
		 strHttpHeaders_Send +=_T("\r\nContent-Type: application/x-www-form-urlencoded");
		 m_pFile_POST = m_pConnection_POST->OpenRequest(CHttpConnection::HTTP_VERB_POST, m_strServerPath);//szURL); 
		 bRet = m_pFile_POST->SendRequest(strHttpHeaders_Send, (LPVOID)(LPCTSTR)strHttpContent_Send, strHttpContent_Send.GetLength());
		 //m_pSession_POST->

	}
    catch (CInternetException* e)
    {
	//	MessageBox(NULL,e->GetRuntimeClass()->m_lpszClassName,"Help",MB_OK);
        TCHAR szCause[255];
        e->GetErrorMessage(szCause, 255);
		//MessageBox(NULL,szCause,"Help",MB_OK);
        e->Delete();
		bRet = false;
		m_strLastErrorMessage = ERROR_WEB_FAILED_WEBCONNECTION;
		goto PostCommand_RETURN;
    }

	bRet = GetAndCheckServerReply(m_pSession_POST, 
								m_pFile_POST, 
								dwHttpReply_StatusCode, strHttpHeaders_Receive, 
								strHttpContent_Receive, 
								pHttpData_Receive);

PostCommand_RETURN:
	//操作完成，必须释放资源
	ReleaseConnect_POST();
	if((bRet == false) && (m_bIsShowErrorMessage == true))
	{
		AfxMessageBox(m_strLastErrorMessage, MB_ICONSTOP);
	}//*/

	return bRet;
}

bool CWebWrapper::InitSession_POST(LPCTSTR szAgentName, CWnd* pWnd)
{
    ReleaseConnect_POST();
	m_infoStatusCode = 0;
    m_pSession_POST = new CWebWrapperSession(szAgentName, pWnd);
	return (m_pSession_POST != NULL);
}

//释放上传所使用的3个连接资源
void CWebWrapper::ReleaseConnect_POST()
{
	if(m_pFile_POST != NULL)
	{
		m_pFile_POST->Close();
		delete m_pFile_POST;
		m_pFile_POST = NULL;
	}
	if(m_pConnection_POST != NULL)
	{
		m_pConnection_POST->Close();
		delete m_pConnection_POST;
		m_pConnection_POST = NULL;
	}
    if(m_pSession_POST != NULL)
    {
        m_pSession_POST->Close();
        delete m_pSession_POST;
		m_pSession_POST = NULL;
    }
}//*/
