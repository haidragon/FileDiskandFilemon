// ErrorMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ErrorMsgDlg.h"
#include ".\errormsgdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorMsgDlg dialog

CErrorMsgDlg::CErrorMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorMsgDlg::IDD, pParent)
	, m_strErrorMsg(_T(""))
{
	//{{AFX_DATA_INIT(CErrorMsgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	Init();
}

void CErrorMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorMsgDlg)
	DDX_Control(pDX, IDC_EDIT_ERRORMSG, m_editErrorMsg);
	DDX_Text(pDX, IDC_EDIT_ERRORMSG, m_strErrorMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CErrorMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CErrorMsgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CErrorMsgDlg::Init(void)
{
	BOOL bRet = TRUE;

	SetIsSendError(FALSE);
	SetErrorMsg("");
	SetBugManagerURL("");

	return bRet;
}

//strErrorPos 错误发生位置；strPrefix 错误描述前缀
void CErrorMsgDlg::SetLastErrorMsg(CString strErrorPos, CString strPrefix)
{
	CString strMsgBuf = "";
	CString strErrorMsg = "";

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), 0, (LPSTR) &strMsgBuf, 0, NULL);

	strErrorMsg.Format("在[%s]处发生以下错误：\r\n%s\r\n\r\n错误详情和原因如下：\r\n%s", strErrorPos, strPrefix, strMsgBuf);

	SetErrorMsg(strErrorMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CErrorMsgDlg message handlers

BOOL CErrorMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if (m_bIsSendError == TRUE)
		GetDlgItem(IDC_SEND)->EnableWindow(true);
	else
		GetDlgItem(IDC_SEND)->EnableWindow(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CErrorMsgDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	
}
