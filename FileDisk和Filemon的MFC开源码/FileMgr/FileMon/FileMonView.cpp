// FileMonView.cpp : CFileMonView 类的实现
//

#include "stdafx.h"
#include "FileMon.h"

#include "FileMonDoc.h"
#include "FileMonView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_MSG_COUNT		10000	//显示最大的信息个数
#define MAX_X_POSITION		50000	//如果显示区域太大时，可以加上这个最大坐标限定
#define MAX_Y_POSITION		50000
#define ROW_DISTANCE		20		//每行文字的间隔距离

// CFileMonView

IMPLEMENT_DYNCREATE(CFileMonView, CView)

BEGIN_MESSAGE_MAP(CFileMonView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFileMonView 构造/析构

CFileMonView::CFileMonView()
{
	// TODO: 在此处添加构造代码
	Init();
}

CFileMonView::~CFileMonView()
{
	Release();
}

BOOL CFileMonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

BOOL CFileMonView::Init()//初始化
{
	BOOL bRet = FALSE;

	m_nX = 0;
	m_nY = 0;
	m_nCount = 0;
	m_strMsg = "";
	m_bIsFileFilterMsg = FALSE;

	return bRet;
}

void CFileMonView::Release()//释放资源
{
}

//显示文件过滤驱动监测到的文件系统信息
void CFileMonView::DisplayFileFilterSystemMsg(CDC* pDC)
{
	/*if ((m_nCount >= MAX_MSG_COUNT) || (m_nY >= MAX_Y_POSITION) )
	{
		Init();
	}
	m_nX = 0;
	m_nY = m_nCount*ROW_DISTANCE;
	pDC->TextOut(m_nX, m_nY, "显示区域");
	m_nCount ++;//*/

	BOOL		bIsDisplay = FALSE;
	PENTRY		ptr;
	CString		strMsg = "";
	char		*items[NUMCOLUMNS];
	int			itemcnt = 0;
	CRect		rtText;
	GetClientRect(&rtText);
	// display all items from Stats[] data
	for ( ptr = (PENTRY)g_strStats; (char *)ptr < min(g_strStats+g_dwStatsLen, g_strStats + sizeof (g_strStats)); )
	{
		//strMsg.Format("有数据：%s\n", ptr->text);
		//m_strMsg += strMsg;
		//pDC->TextOut(m_nX, m_nY, m_strMsg);

		size_t len = strlen(ptr->text);
		// Split line into columns
		itemcnt = g_clsFileFilter_Driver.Split( ptr->text, '\t', items );
		if ( itemcnt <= 0 )
			return;
		// Determine Its a new request??
		if ( *items[0] )
		{
			// Its a new request.  Put at end.
			strMsg.Format(_T("Sequence number: %d -- "), ptr->seq );
			m_strMsg += strMsg;
			//strMsg.Format(_T("datatime: %d / perftime: %d / "), ptr->datetime, ptr->perftime );
			//m_strMsg += strMsg;
			// Sequence number if a new item
			if (itemcnt>0 && *items[0] )
			{
				strMsg.Format(_T("Process name: %s -- "), items[0] );
				m_strMsg += strMsg;
			}
			if (itemcnt>1 && *items[1])
			{
				strMsg.Format(_T("Request type: %s -- "), items[1] );
				m_strMsg += strMsg;
			}
			if (itemcnt>2 && *items[2])
			{
				strMsg.Format(_T("Path: %s -- "), items[2] );
				m_strMsg += strMsg;
			}
			if (itemcnt>3 && *items[3])
			{
				strMsg.Format(_T("Additional: %s -- "), items[3] );
				m_strMsg += strMsg;
			}
		}
		else
		{
			if (itemcnt>4 && *items[4])
			{
				strMsg.Format(_T("Result: %s"), items[4] );
				m_strMsg += strMsg;
			}
			m_strMsg += "\n";
		}
		len += 4; len &= 0xFFFFFFFC; // +1 for null-terminator +3 for 32bit alignment
		ptr = (PENTRY)(ptr->text + len);
		bIsDisplay = TRUE;
	}
	pDC->DrawText(m_strMsg, rtText, DT_LEFT | DT_TOP);
	// Empty the buffer
	if (bIsDisplay)
	{
		g_dwStatsLen = 0;
	}
	m_bIsFileFilterMsg = FALSE;
}

// CFileMonView 绘制
void CFileMonView::OnDraw(CDC* pDC)
{
	CFileMonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (m_bIsFileFilterMsg)
	{
		DisplayFileFilterSystemMsg(pDC);
	}
	else
	{
		CRect		rtText;
		GetClientRect(&rtText);
		pDC->DrawText(m_strMsg, rtText, DT_LEFT | DT_TOP | DT_WORDBREAK);
	}
}


// CFileMonView 打印

BOOL CFileMonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFileMonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CFileMonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CFileMonView 诊断

#ifdef _DEBUG
void CFileMonView::AssertValid() const
{
	CView::AssertValid();
}

void CFileMonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFileMonDoc* CFileMonView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileMonDoc)));
	return (CFileMonDoc*)m_pDocument;
}
#endif //_DEBUG


// CFileMonView 消息处理程序
