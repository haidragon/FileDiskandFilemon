//----------------------------------------------------------------------------------------------------
//  中国博客网，版权所有，２００５
//
//  文件内容：
//      实现 CWebDA 累
//
//  修改历史：
//      薛永海		２００５年１１月２１日	新建文件
//----------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include ".\WebDA.h"

CWebDA::CWebDA(void)
{
	m_strUserName	= "";
	m_strPasswordMD5 = "";
	m_strBlogName = "0";
}

CWebDA::~CWebDA(void)
{
}

/*====================================================================================
//  函数功能
//      设置用户名 m_strUserName;
//
//  参数说明
//      strUserName ：  用户名
//===================================================================================*/
void CWebDA::SetUserName(CString strUserName)
{
    m_strUserName = strUserName;
}

/*====================================================================================
//  函数功能
//      设置用户MD5密码    m_strPasswordMD5;
//
//  参数说明
//      strPasswordMD5 ：  用户密码
//===================================================================================*/
void CWebDA::SetPasswordMD5(CString strPasswordMD5)
{
    m_strPasswordMD5 = strPasswordMD5;
}

/*====================================================================================
//  函数功能
//      设置博客名    m_strBlogName;
//
//  参数说明
//      strBlogName ：  用户博客名
//===================================================================================*/
void CWebDA::SetBlogName(CString strBlogName)
{
    m_strBlogName = strBlogName;
}

/*====================================================================================
//  函数功能
//      调用系统函数
//
//  参数说明
//      funcode                 ：  要被调用的系统函数号
//      pHttpData_Receive       ：  存放服务器返回的信息
//      pparam                  ：  HTTP中body中的数据
//      username                ：  用户名
//      password                ：  用户密码
//      rand                    ：  MD5算法产生的随机数
//      blogname                ：  用户的博客名
//      pWnd                    ：  调用该函数的窗口句柄
//      bIsShowErrorMessage     ：  是否显示错误提示信息
//      bIsAnalyzeServerReply   ：  是否对服务器返回的数据进行分析
//===================================================================================*/
HRESULT CWebDA::CallServerFun(
                        CString funcode,
                        CHttpDataPackage_Receive* pHttpData_Receive,
                        CStringArray* pparam,
                        LPCSTR username,
                        LPCSTR password,
                        LPCSTR rand,
                        LPCSTR blogname,
                        CWnd* pWnd, 
                        bool bIsShowErrorMessage,
                        bool bIsAnalyzeServerReply
                        )
{
    HRESULT hr = S_OK;

    CString strAgentName = _T("WebGrab");
	CHttpDataPackage_Send HttpData_Send;
	CHttpDataPackage_Receive HttpData_Receive;
	CMd5Encrypt md5;
	
	if(!m_strUserName.IsEmpty())
		md5.encrypt(m_strUserName, m_strPasswordMD5);

	// Http header
	if(blogname == NULL)
		blogname = m_strBlogName;

	if(username == NULL)
		username = m_strUserName;

	if(rand == NULL)
		rand = md5.GetRand_Function();

	if(password == NULL)
		password = md5.GetCryptograph_Fuction();

	HttpData_Send.SetHeader_BlogName(blogname);
	HttpData_Send.SetHeader_Cryptograph(password);
	HttpData_Send.SetHeader_Random(rand);
	HttpData_Send.SetHeader_UserName(username);
	HttpData_Send.SetHeader_Function(funcode);
	HttpData_Send.SetContent_TransferType("S");		
    //HttpData_Send.SetHeader_Version("1.0.3");

	// Call param
	if(pparam != NULL)
		HttpData_Send.SetContent_DataItem(pparam);
	if(pHttpData_Receive == NULL)
		pHttpData_Receive = &HttpData_Receive;

	//Call server function
	if (m_clsWebWrapper.PostCommand(
                                        HttpData_Send.GetHeaders(),
                                        HttpData_Send.GetContent_DataAll(),
                                        pHttpData_Receive,
                                        strAgentName,
                                        pWnd,
                                        bIsShowErrorMessage,
                                        bIsAnalyzeServerReply
                                        ))
    {
        hr = S_OK;
    }
    else
    {
        hr = S_FALSE;
    }

    return hr;
}

//====================================================================================
//  函数功能：
//      为用户注册 Lava-Lava GID
//  
//  参数说明
//      strUserName     ：  BlogCN用户名
//      strPasswordMD5  ：  用户密码
//      strLavaGID      ：  存放Lava服务器返回的GID
//===================================================================================//
HRESULT CWebDA::RegisterLavaUser(CString strUserName, CString strPasswordMD5, CString & strLavaGID)
{
    HRESULT hr = S_OK;

    //**********************************************************************************
    //开始去 Lava-Lava 服务器注册GID
    CInternetSession * interSession = new CInternetSession("Rabo", 1, INTERNET_OPEN_TYPE_DIRECT, NULL);
    CHttpConnection * phttpConnection = NULL;
    CHttpFile       * phttpFile = NULL;
    CString regURL = "";

    regURL = "lavaOEM/regOEMuser.php?oemid=507";
    regURL += "&nickname=";
    regURL += strUserName;
	regURL += "&oemaccount=";
	regURL += strUserName;
    regURL += "&password=";
    regURL += strPasswordMD5;

    phttpConnection = interSession->GetHttpConnection("www.lava-lava.com", (INTERNET_PORT)80);
    phttpFile = phttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, regURL);

    phttpFile->SendRequest();

    CString strContent = "";
	CString strResultTmp = "";

    DWORD nFileSize = (DWORD)phttpFile->GetLength();
    LPSTR lpstrBuf = strContent.GetBuffer(nFileSize);
    UINT uBytesRead = phttpFile->Read(lpstrBuf, nFileSize);
    lpstrBuf[nFileSize] = '\0';
    strResultTmp = lpstrBuf;
	strContent.ReleaseBuffer();
	
	strResultTmp.Trim(" ");

	int nIndex = strResultTmp.Find(',');
	if (strResultTmp.Left(nIndex) == REGISTER_LAVAGID_SUCCESS)
	{
		//18表示成功
		strLavaGID = strResultTmp.Mid(nIndex + 1);
	}
	else if (strResultTmp.Left(nIndex) == REGISTER_LAVAGID_FAILED)
	{
		//应该是在 17 的位置找到，表示失败了
		CString strError = strResultTmp.Mid(nIndex + 1);
		int nErrorNum = atoi(strError);
		switch (nErrorNum)
		{
		case 1:
			hr = E_ERROR_OEMID;
			break;
		case 2:
			hr = E_ERROR_COMMOND;
			break;
		case 3:
			hr = E_ERROR_OTHER;
			break;
		default:
			hr = S_FALSE;
		}
	}
	else
	{
		hr = E_ERROR_OTHER;
	}

    if(phttpFile != NULL)
    {
        phttpFile->Close();
        delete phttpFile;
        phttpFile = NULL;
    }
    if(phttpConnection != NULL)
    {
        phttpConnection->Close();
        delete phttpConnection;
        phttpConnection = NULL;
    }
    if(interSession != NULL)
    {
        interSession->Close();
        delete interSession;
        interSession = NULL;
    }

    return hr;
}
