#if !defined(AFX_IMAGELISTCTRL_H__E61420F2_8F8A_4E77_8E45_25CA774639DC__INCLUDED_)
#define AFX_IMAGELISTCTRL_H__E61420F2_8F8A_4E77_8E45_25CA774639DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SortHeaderCtrl.h"
// ImageListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageListCtrl window

class _COMMON_API CImageListCtrl : public CListCtrl
{
// Construction
public:
	CImageListCtrl();

// Attributes
public:
	bool m_bIsSortAscending;//是否升序排序
	int m_nSortedColumn;//当前排序的列

// Operations
public:
	void InitData();
	//void SetSortArrow();//设置排序的按钮
	bool ClearupColumnAndItem();//清除所有内容
	bool SetColumns(CStringArray* pstrArray_Columns);
	bool AddOneItem(CStringArray* pstrArray_Item);
	bool AddSubItem(int nItem, int nSubItem, CString strSubItemText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	int CreateAndSetImageList(UINT nIDResource);
	void SelectAllItems();
	void ClearupColumns();
	int GetCurSelectedItem();
	virtual ~CImageListCtrl();

	// Generated message map functions
protected:
	CSortHeaderCtrl m_clsSortHeader;
	CBitmap m_bitmap;
	CImageList m_imageList;
	//{{AFX_MSG(CImageListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGELISTCTRL_H__E61420F2_8F8A_4E77_8E45_25CA774639DC__INCLUDED_)
