// FileMon.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "FileMon.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "FileMonDoc.h"
#include "FileMonView.h"
#include ".\filemon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileMonApp

BEGIN_MESSAGE_MAP(CFileMonApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

// CFileMonApp 构造
CFileMonApp::CFileMonApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CFileMonApp 对象
CFileMonApp theApp;

// 初始化函数
BOOL CFileMonApp::Init(void)
{
	BOOL bRet = TRUE;
	// get NT version
	DWORD NTVersion = GetVersion();
	//if( NTVersion >= 0x80000000 )	g_bIsNT = FALSE;
	//else							g_bIsNT = TRUE;
	if( NTVersion >= 0x80000000 )
	{
		g_dlgErrorMsg.SetErrorMsg(ERRMSG_WINVER_FAILURE);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}
	else
	{
		//g_dlgErrorMsg.SetErrorMsg(ERRMSG_WINVER_SUCCESS);
		//g_dlgErrorMsg.SetLastErrorMsg(ERRMSG_WINVER_SUCCESS);
		//g_dlgErrorMsg.DoModal();
		TRACE0(ERRMSG_WINVER_SUCCESS);
		TRACE0("\n");
	}

	GetApplicationPath(theApp.m_hInstance, g_strAppPath);

	// determine performance counter frequency
	QueryPerformanceFrequency( &g_PerfFrequency );

	//初始化变量的初始值
	g_cDriveLetter = 'a';
	g_hSys_FileFilter = INVALID_HANDLE_VALUE;
	g_dwCurDriveSet = 0;//GetLogicalDrives();

	return bRet;
}
// CFileMonApp 初始化
BOOL CFileMonApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	if (!Init())
	{
		return FALSE;
	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_FileMonTYPE,
		RUNTIME_CLASS(CFileMonDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CFileMonView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CFileMonApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
