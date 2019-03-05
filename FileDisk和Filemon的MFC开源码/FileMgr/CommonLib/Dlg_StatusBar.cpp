// Dlg_StatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_StatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_StatusBar dialog


CDlg_StatusBar::CDlg_StatusBar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_StatusBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_StatusBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_StatusBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_StatusBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_StatusBar, CDialog)
	//{{AFX_MSG_MAP(CDlg_StatusBar)
		// NOTE: the ClassWizard will add message map macros here
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_StatusBar message handlers

HBRUSH CDlg_StatusBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor==CTLCOLOR_DLG)
	{
		//pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(160,180,220));
		HBRUSH b=CreateSolidBrush(RGB(160,180,220));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(160,180,220));
		HBRUSH b=CreateSolidBrush(RGB(160,180,220));
		return b;
	}
	return hbr;
}
