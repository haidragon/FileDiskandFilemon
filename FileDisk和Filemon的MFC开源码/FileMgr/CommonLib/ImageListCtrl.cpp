// ImageListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ImageListCtrl.h"
#include ".\imagelistctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageListCtrl

CImageListCtrl::CImageListCtrl()
{
	InitData();
}

CImageListCtrl::~CImageListCtrl()
{
}

void CImageListCtrl::InitData()
{
	m_bIsSortAscending = true;//是否升序排序
	m_nSortedColumn = -1;//当前排序的列
}

BEGIN_MESSAGE_MAP(CImageListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CImageListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageListCtrl message handlers

int CImageListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	//CreateAndSetImageList(IDB_LISTCTRL);

	return 0;
}

int CImageListCtrl::GetCurSelectedItem()
{
	int nCurSelectedItem = -1;//没有任何一行被选中
	POSITION pos = GetFirstSelectedItemPosition();
	if(pos != NULL)
	{
		nCurSelectedItem = GetNextSelectedItem(pos);
	}
	else
	   TRACE0("No items were selected!\n");

	return nCurSelectedItem;
}

void CImageListCtrl::ClearupColumns()
{
	int nColumn = GetHeaderCtrl()->GetItemCount();
	for(int i=0; i<nColumn; i++)
	{
		DeleteColumn(0);
	}
}

bool CImageListCtrl::ClearupColumnAndItem()//清除所有内容
{
	bool bRet = true;
	//消除以前的显示内容
	DeleteAllItems();
	ClearupColumns();
	return bRet;
}

bool CImageListCtrl::SetColumns(CStringArray* pstrArray_Columns)
{
	bool bRet = true;
	//检测参数的合法性
	if(pstrArray_Columns == NULL)
	{
		AfxMessageBox("当前没有 ListCtrl 控件要显示的头部分字符串!\n\nAt CImageListCtrl::SetColumns()");
		bRet = false;
		return bRet;
	}
	//获取 ListCtrl 的显示区域大小
	CRect rtArea;
	GetClientRect(rtArea);
	//平均分配 ListCtrl 每个列的显示区域大小
	int nHeaderCount = (int) pstrArray_Columns->GetSize();
	if(nHeaderCount > 0)
	{
		int nWidth = 0;
		nWidth = rtArea.Width()/nHeaderCount;
		//设置 ListCtrl 头的新内容
		for(int i=0; i<nHeaderCount; i++)
		{
			InsertColumn(i, _T(pstrArray_Columns->GetAt(i)), LVCFMT_LEFT, nWidth);
		}
		if (m_clsSortHeader.GetSafeHwnd() != NULL)
			m_clsSortHeader.UnsubclassWindow();
		VERIFY(m_clsSortHeader.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd()));
	}
	bRet = true;
	return bRet;
}

bool CImageListCtrl::AddOneItem(CStringArray* pstrArray_Item)
{
	bool bRet = true;
	//检测参数的合法性
	if(pstrArray_Item == NULL)
	{
		AfxMessageBox("当前没有 ListCtrl 控件要添加显示的1条记录字符串!\n\nAt CImageListCtrl::AddOneItem()");
		bRet = false;
		return bRet;
	}
	//当前控件里有几列
	int nHeaderCount = GetHeaderCtrl()->GetItemCount();
	//要添加入控件里的字符串有几个(或者说几列)
	int nCurrentItem_TextCount = (int) pstrArray_Item->GetSize();
	if(nHeaderCount != nCurrentItem_TextCount)
	{
		CString strError= "";
		strError.Format("当前 ListCtrl 控件有%d列,而要添加显示的1条记录有%d个字符串!\n2者不相等而出错!", 
						nHeaderCount, nCurrentItem_TextCount);
		strError += "\n\nAt CImageListCtrl::AddOneItem()!";
		AfxMessageBox(strError);
		bRet = false;
		return bRet;
	}
	//获得当前有几条内容(或者说几条记录)
	int nItemCount = GetItemCount();
	//在控件内容的末尾添加新的内容(或者说记录)
	InsertItem(nItemCount, _T(pstrArray_Item->GetAt(0)), 1);
	for(int i=1; i<nHeaderCount; i++)
	{
		SetItemText (nItemCount, i, _T(pstrArray_Item->GetAt(i)));
	}
	bRet = true;
	return bRet;
}

void CImageListCtrl::SelectAllItems()
{
	int nItemCount = GetItemCount();
	for(int i=0; i<nItemCount; i++)
	{
		SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

int CImageListCtrl::CreateAndSetImageList(UINT nIDResource)
{
	// load the tree images bitmap and add it to the image list.
	if(m_bitmap.LoadBitmap( nIDResource ) != 0)
	{
		if ( !m_imageList.Create( 16, 16, ILC_COLOR24 | ILC_MASK, 1, 1 ) )
		{
			TRACE0("Failed to create image list.\n");
			return -1;
		}
		m_imageList.Add( &m_bitmap, RGB( 0xff,0x00,0xff ) );
		SetImageList( &m_imageList, LVSIL_SMALL );
	}

	return 0;
}

bool CImageListCtrl::AddSubItem(int nItem, int nSubItem, CString strSubItemText)
{
	bool bRet = true;
	//----------- 检测参数的合法性 开始 ---------------------
	if((nItem < 0) || (nSubItem < 0))
	{
		CString strError= "";
		strError.Format("ListCtrl 控件里的记录数不可能负数!\nItem:%d  Sub Item:%d", 
						nItem, nSubItem);
		strError += "\n\nAt CImageListCtrl::AddSubItem()!";
		AfxMessageBox(strError);
		bRet = false;
		return bRet;
	}
	//当前控件里有几列
	int nHeaderCount = GetHeaderCtrl()->GetItemCount();
	if(nSubItem >= nHeaderCount)//要加入的子项大于现有的列数
	{
		CString strError= "";
		strError.Format("当前 ListCtrl 控件有%d列,而您要添加显示的子项为第%d个!\n后者大于前者而出错!", 
						nHeaderCount, nSubItem+1);
		strError += "\n\nAt CImageListCtrl::AddSubItem()!";
		AfxMessageBox(strError);
		bRet = false;
		return bRet;
	}
	//----------- 检测参数的合法性 结束 ---------------------
	if(nSubItem == 0)
	{
		//获得当前有几条内容(或者说几条记录)
		int nItemCount = GetItemCount();
		if(nItem < nItemCount)
		{//记录已经存在，重新刷新内容
			SetItemText(nItem, nSubItem, strSubItemText);
		}
		else
		{//新记录
			//在控件内容的末尾添加新的内容(或者说记录)
			InsertItem(nItemCount, strSubItemText, 1);
		}
	}
	else
	{
		SetItemText(nItem, nSubItem, strSubItemText);
	}
	return bRet;
}
