// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "FileMon.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#include "FileMonDoc.h"
#include "FileMonView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	InitVariables();
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::InitVariables()//初始化变量值
{
}

// 初始化
BOOL CMainFrame::Init(void)
{
	BOOL bRet = TRUE;
	CString strErrorPos = "CMainFrame::Init()";//出错位置描述
	CString strErrorMsg = "";

	CWebWrapper clsWebWrapper;
	CHttpDataPackage_Receive HttpData_Receive;
	//Call server function
	if (clsWebWrapper.PostCommand( _T(""), _T("abcd我你他"), &HttpData_Receive, 
									_T("WebGrab"), this, FALSE, FALSE))
	//if (clsWebWrapper.GetCommand( _T(""), _T("abcd我你他"), &HttpData_Receive, 
	//								_T("WebGrab"), this, FALSE, FALSE))
	{
		AfxMessageBox(HttpData_Receive.GetContent_DataAll());
	}
	else
	{
		AfxMessageBox("网络通讯失败");
		return FALSE;
	}

	InitVariables();

	if (!g_clsFileFilter_Driver.AutoLoadAndStart())
	{
		bRet = FALSE;
		return bRet;
	}

	if (g_clsFileDisk_Driver.DefaultMount() < 0)
	{
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEDISK_DEFAULTMOUNT_FAILURE);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}

	if (!g_clsFileFilter_Driver.DefaultHookDriver())
	{
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_DEFAULTHOOK_FAILURE);
		g_dlgErrorMsg.DoModal();
		bRet = FALSE;
		return bRet;
	}
	// Start up timer to periodically update screen
	//m_nTimer = SetTimer( 1, 500/*ms*/, NULL );
	return bRet;
}
//释放资源
void CMainFrame::Release(void)
{
	CString strErrorPos = "CMainFrame::Release()";//出错位置描述
	CString strErrorMsg = "";
//#if _DEBUG
	ULONG	ulIRPCount = 0;
//#endif

	//KillTimer(m_nTimer);

	if(g_clsFileDisk_Driver.DefaultUnMount() >= 0)
	{
		if(g_clsFileDisk_Driver.StopAndRemoveService(FILEDISK_NAME) < 0)
		{
			g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_FILEDISKSERVICE_STOPREMOVE_FAILURE);
			g_dlgErrorMsg.DoModal();
		}
	}
//#if _DEBUG
	// see if the driver can unload
	if ( ! DeviceIoControl(	g_hSys_FileFilter, IOCTL_FILEMON_UNLOADQUERY,
							NULL, 0, NULL, 0, &ulIRPCount, NULL ) )
	{
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_ACCESS_FAILURE);
		g_dlgErrorMsg.DoModal();
	}
	if( ulIRPCount )
	{
		TRACE0(ERRMSG_SYSFILEFILTER_OTHER_REQUEST);
		g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_OTHER_REQUEST);
		g_dlgErrorMsg.DoModal();
	}
	else
	{
		if ( ! g_clsFileFilter_Driver.UnloadDeviceDriver( FILEFILTER_NAME ) )
		{
			g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_UNLOAD_FAILURE);
			g_dlgErrorMsg.DoModal();
		}
		CloseHandle( g_hSys_FileFilter );
	}
//#endif
	//CloseHandle( g_hSys_FileFilter );
	free(g_pOpenFileInformation);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!Init())
	{
		Release();
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	Release();

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		CString strErrorPos = "CMainFrame::OnTimer()";
		//CString strMsg = "";
		//PENTRY		ptr;
		//DWORD dwStartTime = 0;
		// don't process for more than a second without pausing
		//dwStartTime = GetTickCount();
		for ( ; ; )
		{
			// Have driver fill Stats buffer with information
			if ( ! DeviceIoControl(	g_hSys_FileFilter, IOCTL_FILEMON_GETSTATS,
									NULL, 0, &g_strStats, sizeof g_strStats,
									&g_dwStatsLen, NULL ) )
			{
				g_dlgErrorMsg.SetLastErrorMsg(strErrorPos, ERRMSG_SYSFILEFILTER_ACCESS_FAILURE);
				g_dlgErrorMsg.DoModal();
				break;
			}
			if ( g_dwStatsLen == 0 )
			{
				TRACE0("没有数据\n");
				break;
			}
			else
			{
				TRACE0("============= 有数据 ===============\n");
				// display all items from Stats[] data
				/*for ( ptr = (PENTRY)g_strStats; (char *)ptr < min(g_strStats+g_dwStatsLen, g_strStats + sizeof (g_strStats)); )
				{
					strMsg.Format(_T("有数据：%s\n"), ptr->text);
					TRACE0(strMsg);
					size_t len = strlen(ptr->text);
					len += 4; len &= 0xFFFFFFFC; // +1 for null-terminator +3 for 32bit alignment
					ptr = (PENTRY)(ptr->text + len);
				}//*/
				// 这里添加显示文件过滤器发送来的信息的代码
				CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
				// Get the active MDI child window.
				CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
				// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
				// Get the active view attached to the active MDI child window.
				CFileMonView* pViewActive = (CFileMonView *) pChild->GetActiveView();
				if(pViewActive != NULL)
				{
					pViewActive->SetIsFileFilterMsg(TRUE);
					pViewActive->Invalidate();
					pViewActive->UpdateWindow(); //SendMessage(WM_SETREDRAW);
				}
				return;
			}
			//if( GetTickCount() - dwStartTime > 1000 ) break;
		}
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}
