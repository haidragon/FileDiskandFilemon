// FileMonView.h : CFileMonView 类的接口
//


#pragma once


class CFileMonView : public CView
{
protected: // 仅从序列化创建
	CFileMonView();
	DECLARE_DYNCREATE(CFileMonView)

// 属性
public:
	CFileMonDoc* GetDocument() const;

	int m_nX;//显示文本信息的起点 X 坐标
	int m_nY;//显示文本信息的起点 Y 坐标
	int m_nCount;//当前共有几个文本信息
	CString m_strMsg;

private:
	BOOL m_bIsFileFilterMsg;//是否有文件过滤器发送来的信息

// 操作
public:
	BOOL GetIsFileFilterMsg() { return m_bIsFileFilterMsg; } ;
	void SetIsFileFilterMsg(BOOL bIsFileFilterMsg) { m_bIsFileFilterMsg = bIsFileFilterMsg; } ;

	void DisplayFileFilterSystemMsg(CDC* pDC);//显示文件过滤驱动监测到的文件系统信息

	BOOL Init();//初始化
	void Release();//释放资源

// 重写
	public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CFileMonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // FileMonView.cpp 的调试版本
inline CFileMonDoc* CFileMonView::GetDocument() const
   { return reinterpret_cast<CFileMonDoc*>(m_pDocument); }
#endif

