#if !defined(AFX_HTTPDATAPACKAGE_H__5391DECF_8292_4D1C_AA45_65A647A90723__INCLUDED_)
#define AFX_HTTPDATAPACKAGE_H__5391DECF_8292_4D1C_AA45_65A647A90723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HttpDataPackage.h : header file
//

#define BUFFER_SIZE 4095

//Send Data Package
#define HTTPDATAPACKAGE_SEND_HEADERSNAME_DATAORIGIN		_T("DON:")
#define HTTPDATAPACKAGE_SEND_HEADERSNAME_DATALENGTH		_T("DLH:")
#define HTTPDATAPACKAGE_SEND_HEADERSNAME_USERNAME		_T("UNE:")
#define HTTPDATAPACKAGE_SEND_HEADERSNAME_RANDOM			_T("RDM:")
#define HTTPDATAPACKAGE_SEND_HEADERSNAME_CRYPTOGRAPH	_T("CGH:")
#define HTTPDATAPACKAGE_SEND_HEADERSNAME_VERSION		_T("VSN:")
#define HTTPDATAPACKAGE_SEND_HEADERSNAME_BLOGNAME		_T("BNE:")
#define HTTPDATAPACKAGE_SEND_HEADERSNAME_FUNCTION		_T("FCN:")

//Receive Data Package
#define HTTPDATAPACKAGE_RECEIVE_HEADERSNAME_DATALENGTH	_T("DLH:")
#define HTTPDATAPACKAGE_RECEIVE_HEADERSNAME_FUNCTION	_T("FCN:")

//Data Base Unit
#define DATA_HEADER_BASEUNIT	3
#define DATA_INFO_BASEUNIT		4

#define DATA_BASEUNIT_LENGTH_MAX 9999
/////////////////////////////////////////////////////////////////////////////
// CHttpDataPackage_Send

class _WEBDRIVER_API CHttpDataPackage_Send : public CObject
{
// Construction
public:
	CHttpDataPackage_Send();
	virtual ~CHttpDataPackage_Send();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpDataPackage_Send)
	//}}AFX_VIRTUAL

private:
	//------ Http头 -------------------------
	CString m_strHeader_DataOrigin;//数据来源
	CString m_strHeader_DataLength;//数据总长度，即 S001001002000...等的总长度
	//安全校验信息
	CString m_strHeader_UserName;//用户名
	CString m_strHeader_Random;//随机数
	CString m_strHeader_Cryptograph;//密文
	//版本校验信息
	CString m_strHeader_Version;//Rabo版本号
	//收费校验信息
	CString m_strHeader_BlogName;//博客名
	CString m_strHeader_Function;//功能代码
	//组合好的完整的Http头
	CString m_strHeaders;
	//------ Http头 -------------------------

	//------ Http内容 -------------------------
	CString m_strContent_TransferType;//数据传输类型，比如 S 表示是发送， P 表示是服务器返回
	CString m_strContent_DataCRC;//有效性校验码
	//数据信息
	CString m_strContent_DataInfo_ItemCount;//数据信息项数：4位，为数据信息项数的总和（ = n）
	//数据项长度, m_strArrayContent_DataInfo_Item.GetAt(0) 表示第1个数据项的长度
	//m_strArrayContent_DataInfo_Item.GetAt(1) 表示第2个数据项的长度，依次类推
	CStringArray m_strArrayContent_DataInfo_Item;
	//数据内容，与上面的数据项长度一一对应
	CStringArray m_strArrayContent_DataItem;
	//包括数据头信息和数据内容的所有数据
	CString m_strContent_DataAll;//所有的数据内容
	//------ Http内容 -------------------------

// Implementation
public:
	//将传入的字符串进行长度的格式化，比如"Title"，将被转化为"0005"
	bool	SetHttp_StringLength_Format(CString strOldString, CString& strNewString);
	bool	SetHttp_Length_Format(CString& strLength, int nLength=0, 
									int nFormat_BaseUnit=DATA_INFO_BASEUNIT); 
private:
	//计算strArrayItem有多少个项，其中最后一项可以是N的4位的组合，而前面都只允许1个4位
	//比如最后一项可以是"0004 0013"
	bool	 SetHttp_Compute_DataInfo_ItemCount(CStringArray* pstrArrayItem, 
												CString& strItemCount);
	//将本类中的有效数据组合成要发送的数据包头 m_strHeaders
	bool	SetHttp_SendHeaders();
public:
	CString GetHeaders()
			{
				if(!SetHttp_SendHeaders())
				{
					m_strHeaders = _T("");
				}
				return m_strHeaders;
			};
	void	SetHeaders(CString strHeaders)
			{ m_strHeaders = strHeaders; };//*/

	/*CString GetHeader_DataOrigin() { return m_strHeader_DataOrigin; };
	void	SetHeader_DataOrigin(CString strHeader_DataOrigin)
			{ m_strHeader_DataOrigin = strHeader_DataOrigin; };//*/

	//CString GetHeader_DataLength() { return m_strHeader_DataLength; };
	void	SetHeader_DataLength(CString strHeader_DataLength)
			{ m_strHeader_DataLength = strHeader_DataLength; };//*/

	//CString GetHeader_UserName() { return m_strHeader_UserName; };
	void	SetHeader_UserName(CString strHeader_UserName)
			{ m_strHeader_UserName = strHeader_UserName; };

	//CString GetHeader_Random() { return m_strHeader_Random; };
	void	SetHeader_Random(CString strHeader_Random)
			{ m_strHeader_Random = strHeader_Random; };

	//CString GetHeader_Cryptograph() { return m_strHeader_Cryptograph; };
	void	SetHeader_Cryptograph(CString strHeader_Cryptograph)
			{ m_strHeader_Cryptograph = strHeader_Cryptograph; };

	CString GetHeader_Version() { return m_strHeader_Version; };
	void	SetHeader_Version(CString strHeader_Version)
			{ m_strHeader_Version = strHeader_Version; };

	//CString GetHeader_BlogName() { return m_strHeader_BlogName; };
	void	SetHeader_BlogName(CString strHeader_BlogName)
			{ m_strHeader_BlogName = strHeader_BlogName; };

	//CString GetHeader_Function() { return m_strHeader_Function; };
	void	SetHeader_Function(CString strHeader_Function)
			{ m_strHeader_Function = strHeader_Function; };

	//CString GetContent_TransferType() { return m_strContent_TransferType; };
	void	SetContent_TransferType(CString strContent_TransferType)
			{ m_strContent_TransferType = strContent_TransferType; };

	CString GetContent_DataCRC() { return m_strContent_DataCRC; };
	void	SetContent_DataCRC(CString strContent_DataCRC)
			{ m_strContent_DataCRC = strContent_DataCRC; };//*/

	CStringArray* GetContent_DataItem()
			{ return &m_strArrayContent_DataItem; };
	void	SetContent_DataItem(CStringArray* pstrArrayContent_DataItem)
			{
				m_strArrayContent_DataItem.RemoveAll();
				int nCount = (int)pstrArrayContent_DataItem->GetSize();
				if(nCount > 0)
				{
					for(int i=0; i<nCount; i++)
					{
						m_strArrayContent_DataItem.Add(
							pstrArrayContent_DataItem->GetAt(i));
					}
				}//*/
			};

private:
	//由 SetHttp_Combine_SendContent() 调用，将本类中的 GetContent_DataInfo_ItemCount 
	//GetContent_DataInfo_Item GetContent_DataItem 数据组合成要发送的
	//数据包内容 m_strContent_DataAll
	bool	SetContent_DataAll();//*/
	//将本类中的有效数据组合成要发送的数据包内容 m_strContent_DataAll
	//并设置Http头中的数据总长度 SetHeader_DataLength()
	bool	SetHttp_Combine_SendContent();//*/

public:
	void ReleaseAllData();
	void InitAllData();
	CString GetContent_DataAll()
			{
				if(!SetHttp_Combine_SendContent())
				{
					m_strContent_DataAll = _T("");
				}
				return m_strContent_DataAll;
			};
	/*void	SetContent_DataAll(CString strContent_DataAll)
			{ m_strContent_DataAll = strContent_DataAll; };//*/

	// Generated message map functions
protected:
	//{{AFX_MSG(CHttpDataPackage_Send)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};

//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// CHttpDataPackage_Receive

class _WEBDRIVER_API CHttpDataPackage_Receive : public CObject
{
// Construction
public:
	CHttpDataPackage_Receive();
	virtual ~CHttpDataPackage_Receive();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpDataPackage_Receive)
	//}}AFX_VIRTUAL

private:
	//------ Http头 -------------------------
	CString m_strHeader_DataLength;//数据总长度，即 P001001002000...等的总长度
	CString m_strHeader_Function;//功能代码
	//------ Http头 -------------------------

	//------ Http内容 -------------------------
	//数据传输类型，比如 S 表示是发送， P 表示是服务器返回的成功代码， 
	//E 表示是服务器返回的出错代码
	CString m_strContent_TransferType;
	CString m_strContent_DataCRC;//有效性校验码
	//数据信息
	CString m_strContent_DataInfo_ItemCount;//数据信息项数：4位，为数据信息项数的总和（ = n）
	//数据项长度, m_strArrayContent_DataInfo_Item.GetAt(0) 表示第1个数据项的长度
	//m_strArrayContent_DataInfo_Item.GetAt(1) 表示第2个数据项的长度，依次类推
	CStringArray m_strArrayContent_DataInfo_Item;
	//数据内容，与上面的数据项长度一一对应
	CStringArray m_strArrayContent_DataItem;
	//数据内容--//所有的数据内容；当服务器有返回内容时，这里保存返回的内容；
	//如果出错时，这里是出错的信息
	CString m_strContent_DataAll;
	//------ Http内容 -------------------------

// Implementation
public:
	void ReleaseAllData();
	void InitAllData();
	//对strHttpContent_Receive 数据进行 CRC码=strHttpContent_CRC 的有效性检验
	//返回 true 表示检验的数据有效
	bool	CRC_Http_ReceiveContent(CString strHttpContent_CRC, 
									CString& strHttpContent_Receive);

	//在 strHttpHeaders_Receive 中根据 strHttp_Receive_HeaderName 指定的头名字查找
	//该名字对应的头的值，并作为返回值
	CString GetHttp_ReceiveHeader(CString strHttpHeaders_Receive, 
									CString strHttp_Receive_HeaderName);
	//将 strHttpContent_Receive 中的有效数据分析并拆分填写到接收的数据包头 strHttpHeaders_Receive 中去
	bool	SetHttp_ReceiveHeaders(CString strHttpHeaders_Receive);
	//将 strHttpContent_Receive 中的有效数据分析并拆分填写到接收的
	//数据包 m_strArrayContent_DataInfo_Item 和 m_strArrayContent_DataItem 中去
	//bIsAnalyzeServerReply=true 表示将数据分析并填写到 m_strArrayContent_DataItem 中去
	bool	SetHttp_ReceiveContent(CString& strHttpContent_Receive, 
									bool bIsAnalyzeServerReply=false);

	CString GetHeader_DataLength() { return m_strHeader_DataLength; };
	void	SetHeader_DataLength(CString strHeader_DataLength)
			{ m_strHeader_DataLength = strHeader_DataLength; };//*/

	CString GetHeader_Function() { return m_strHeader_Function; };
	void	SetHeader_Function(CString strHeader_Function)
			{ m_strHeader_Function = strHeader_Function; };//*/

	CString GetContent_TransferType() { return m_strContent_TransferType; };
	void	SetContent_TransferType(_TCHAR strContent_TransferType)
			{ m_strContent_TransferType = strContent_TransferType; };//*/

	CString GetContent_DataCRC() { return m_strContent_DataCRC; };
	void	SetContent_DataCRC(CString strContent_DataCRC)
			{ m_strContent_DataCRC = strContent_DataCRC; };//*/

	CString GetContent_DataInfo_ItemCount() { return m_strContent_DataInfo_ItemCount; };
	void	SetContent_DataInfo_ItemCount(CString strContent_DataInfo_ItemCount)
			{ m_strContent_DataInfo_ItemCount = strContent_DataInfo_ItemCount; };

	CStringArray* GetContent_DataInfo_Item()
			{ return &m_strArrayContent_DataInfo_Item; };
	void	SetContent_DataInfo_Item(CStringArray* pstrArrayContent_DataInfo_Item)
			{
				m_strArrayContent_DataInfo_Item.RemoveAll();
				int nCount = (int)pstrArrayContent_DataInfo_Item->GetSize();
				if(nCount <= 0)
					return;

				for(int i=0; i<nCount; i++)
				{
					m_strArrayContent_DataInfo_Item.Add(
						pstrArrayContent_DataInfo_Item->GetAt(i));
				}//*/
			};

	CStringArray* GetContent_DataItem()
			{ return &m_strArrayContent_DataItem; };
	void	SetContent_DataItem(CStringArray* pstrArrayContent_DataItem)
			{
				m_strArrayContent_DataItem.RemoveAll();
				int nCount = (int)pstrArrayContent_DataItem->GetSize();
				if(nCount > 0)
				{
					for(int i=0; i<nCount; i++)
					{
						m_strArrayContent_DataItem.Add(
							pstrArrayContent_DataItem->GetAt(i));
					}
				}
			};//*/

	CString GetContent_DataAll() { return m_strContent_DataAll; };
	void	SetContent_DataAll(CString strContent_DataAll)
			{ m_strContent_DataAll = strContent_DataAll; };

	//void	SetHttpDataPackage(CHttpDataPackage_Receive* pHttpDataPackage);

	// Generated message map functions
protected:
	//{{AFX_MSG(CHttpDataPackage_Receive)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPDATAPACKAGE_H__5391DECF_8292_4D1C_AA45_65A647A90723__INCLUDED_)
