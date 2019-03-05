// FileMonDoc.cpp :  CFileMonDoc 类的实现
//

#include "stdafx.h"
#include "FileMon.h"

#include "FileMonDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileMonDoc

IMPLEMENT_DYNCREATE(CFileMonDoc, CDocument)

BEGIN_MESSAGE_MAP(CFileMonDoc, CDocument)
END_MESSAGE_MAP()


// CFileMonDoc 构造/析构

CFileMonDoc::CFileMonDoc()
{
	// TODO: 在此添加一次性构造代码

}

CFileMonDoc::~CFileMonDoc()
{
}

BOOL CFileMonDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CFileMonDoc 序列化

void CFileMonDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CFileMonDoc 诊断

#ifdef _DEBUG
void CFileMonDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFileMonDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFileMonDoc 命令
