#if !defined(AFX_DLG_STATUSBAR_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_)
#define AFX_DLG_STATUSBAR_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_StatusBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_StatusBar dialog
#include "resource.h"
class _COMMON_API CDlg_StatusBar : public CDialog
{
// Construction
public:
	CDlg_StatusBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_StatusBar)
	enum { IDD = IDD_STATUS_BAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_StatusBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg_StatusBar)
		// NOTE: the ClassWizard will add member functions here
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_STATUSBAR_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_)
