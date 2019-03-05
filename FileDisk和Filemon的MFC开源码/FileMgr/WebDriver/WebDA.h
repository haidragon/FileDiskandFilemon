//----------------------------------------------------------------------------------------------------
//  中国博客网，版权所有，２００５
//
//  文件内容：
//      定义 CWebDA 累
//
//  修改历史：
//      薛永海		２００５年１１月２１日	新建文件
//----------------------------------------------------------------------------------------------------
#pragma once

class _WEBDRIVER_API CWebDA
{
public:
	CWebDA(void);
    ~CWebDA(void);

    /*====================================================================================
    //  函数功能
    //      设置用户名 m_strUserName;
    //
    //  参数说明
    //      strUserName ：  用户名
    //===================================================================================*/
    void SetUserName(CString strUserName);

    /*====================================================================================
    //  函数功能
    //      设置用户MD5密码    m_strPasswordMD5;
    //
    //  参数说明
    //      strPasswordMD5 ：  用户密码
    //===================================================================================*/
    void SetPasswordMD5(CString strPasswordMD5);

    /*====================================================================================
    //  函数功能
    //      设置博客名    m_strBlogName;
    //
    //  参数说明
    //      strBlogName ：  用户博客名
    //===================================================================================*/
    void SetBlogName(CString strBlogName);

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
    HRESULT CallServerFun(
                            CString funcode,
                            CHttpDataPackage_Receive* pHttpData_Receive = NULL,
                            CStringArray* pparam = NULL,
                            LPCSTR username = NULL,
                            LPCSTR password = NULL,
                            LPCSTR rand = NULL,
                            LPCSTR blogname = NULL,
                            CWnd* pWnd = NULL, 
                            bool bIsShowErrorMessage = true,
                            bool bIsAnalyzeServerReply = false
                            );

    //====================================================================================
    //  函数功能：
    //      为用户注册 Lava-Lava GID
    //  
    //  参数说明
    //      strUserName     ：  BlogCN用户名
    //      strPasswordMD5  ：  用户密码
    //      strLavaGID      ：  存放Lava服务器返回的GID
    //===================================================================================//
    HRESULT RegisterLavaUser(CString strUserName, CString strPasswordMD5, CString & strLavaGID);

private:
    CWebWrapper m_clsWebWrapper;
    CString     m_strUserName;
    CString     m_strPasswordMD5;
    CString     m_strBlogName;
};
