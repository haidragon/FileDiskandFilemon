// HttpDataPackage.cpp : implementation file
//

#include "stdafx.h"
#include "HttpDataPackage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHttpDataPackage_Send

CHttpDataPackage_Send::CHttpDataPackage_Send()
{
	InitAllData();
}

CHttpDataPackage_Send::~CHttpDataPackage_Send()
{
	ReleaseAllData();
}

void CHttpDataPackage_Send::ReleaseAllData()
{
	m_strArrayContent_DataInfo_Item.FreeExtra();
	m_strArrayContent_DataInfo_Item.RemoveAll();
	m_strArrayContent_DataItem.FreeExtra();
	m_strArrayContent_DataItem.RemoveAll();
}

void CHttpDataPackage_Send::InitAllData()
{
	//------ Http头 -------------------------
	m_strHeader_DataOrigin = _T("1");//数据来源，1默认为Rabo
	m_strHeader_DataLength = _T("");//数据总长度
	//安全校验信息
	m_strHeader_UserName = _T("");//用户名
	m_strHeader_Random = _T("");//随机数
	m_strHeader_Cryptograph = _T("");//密文
	//版本校验信息
	//m_strHeader_Version = _T("002");//Rabo版本号，默认
	m_strHeader_Version = _T("Lava001");//和Lava合作时用的版本号
	//收费校验信息
	m_strHeader_BlogName = _T("");//博客名
	m_strHeader_Function = _T("");//功能代码
	//组合好的完整的Http头
	m_strHeaders = _T("");;
	//------ Http头 -------------------------

	//------ Http内容 -------------------------
	m_strContent_TransferType = _T("");//数据传输类型，比如 S 表示是发送， P 表示是服务器返回
	m_strContent_DataCRC = _T("");//有效性校验码
	m_strContent_DataInfo_ItemCount = _T("0000");//数据信息项数：4位，为数据信息项数的总和（ = n）

	ReleaseAllData();
	//数据内容
	m_strContent_DataAll = _T("");
	//------ Http内容 -------------------------
}

//按 nFormat_BaseUnit 指定的位数对长度进行字符化，比如100，将被转化为"0100"
bool CHttpDataPackage_Send::SetHttp_Length_Format(CString& strLength, int nLength, 
												  int nFormat_BaseUnit)
{
	bool bRet = true;

	if(nLength <= 0)
	{
		strLength.Empty();
		for(int i=0; i<nFormat_BaseUnit; i++)
		{
			strLength += "0";
		}
	}
	else
	{
		strLength.Format(_T("%d"), nLength);
		switch(strLength.GetLength() % nFormat_BaseUnit)
		{
		case 0:
			break;

		case 1:
			strLength = _T("000") + strLength;
			break;
			
		case 2:
			strLength = _T("00") + strLength;
			break;
			
		case 3:
			strLength = _T("0") + strLength;
			break;

		default:
			{
				AfxMessageBox(ERROR_WEB_LENGTH_FORMAT, MB_ICONSTOP);
				bRet = false;
			}
			break;
		}
	}
	
	return bRet;
}

//将传入的字符串进行长度的格式化，比如"Title"，将被转化为"0005"
//如果字符串长度很长，超过了4位，则以4位为基础进行扩充，比如"0005 0010"
bool CHttpDataPackage_Send::SetHttp_StringLength_Format(CString strOldString, 
														   CString& strNewString)
{
	bool bRet = true;
	strNewString = _T("0000");

	int nOldString_Length =0;
	nOldString_Length = strOldString.GetLength();
	SetHttp_Length_Format(strNewString, nOldString_Length, DATA_INFO_BASEUNIT);

	return bRet;
}

bool CHttpDataPackage_Send::SetHttp_Compute_DataInfo_ItemCount(CStringArray* pstrArrayItem,
															   CString& strItemCount)
{
	bool bRet = true;
	strItemCount = _T("0000");
	int nItemCount = 0;

	nItemCount = (int)pstrArrayItem->GetSize();
	if(nItemCount > 0)
	{
		//检查最后一项
		int nLastItem_Length = pstrArrayItem->GetAt(nItemCount-1).GetLength();
		if(nLastItem_Length > DATA_INFO_BASEUNIT)
		{//最后一项超过4位，表示是扩充过的数据长度，比如"0004 1002"
			nLastItem_Length = nLastItem_Length/DATA_INFO_BASEUNIT;
			nItemCount += nLastItem_Length-1;
		}
		if(nItemCount <= DATA_BASEUNIT_LENGTH_MAX)
		{
			SetHttp_Length_Format(strItemCount, nItemCount, DATA_INFO_BASEUNIT);
			bRet = true;
		}
		else
		{
			CString strError = ERROR_WEB_SEND_CONTENT_EXCEED;
			AfxMessageBox(strError);
			bRet = false;
		}
	}
	else
		bRet = true;

	return bRet;
}

//将本类中的有效数据组合成要发送的数据包头 m_strHeaders
bool CHttpDataPackage_Send::SetHttp_SendHeaders()
{
	bool bRet = true;
	m_strHeaders.Empty();
	//检查参数合法性
	if(m_strHeader_DataOrigin.IsEmpty() || m_strHeader_DataLength.IsEmpty() || 
		m_strHeader_UserName.IsEmpty() || m_strHeader_Random.IsEmpty() || 
		m_strHeader_Cryptograph.IsEmpty() || m_strHeader_Version.IsEmpty() || 
		m_strHeader_BlogName.IsEmpty() || m_strHeader_Function.IsEmpty())
	{
		CString strError = ERROR_WEB_SEND_INVALID_HEADERS;
		/*strError += _T("Origin:")		+ m_strHeader_DataOrigin;
		strError += _T("Length:")		+ m_strHeader_DataLength;
		strError += _T("UserName:")		+ m_strHeader_UserName;
		strError += _T("Random:")		+ m_strHeader_Random;
		strError += _T("Cryptograph:")	+ m_strHeader_Cryptograph;
		strError += _T("Version:")		+ m_strHeader_Version;
		strError += _T("BlogName:")		+ m_strHeader_BlogName;
		strError += _T("Function:")		+ m_strHeader_Function;//*/
		AfxMessageBox(strError);
		bRet = false;
	}
	else
	{
		//头信息格式如下： Name:Value\r\n ；即 名字:值 再加回车换行
		m_strHeaders = 
		HTTPDATAPACKAGE_SEND_HEADERSNAME_DATAORIGIN  + m_strHeader_DataOrigin + _T("\r\n") + 
		HTTPDATAPACKAGE_SEND_HEADERSNAME_DATALENGTH  + m_strHeader_DataLength + _T("\r\n") +
		HTTPDATAPACKAGE_SEND_HEADERSNAME_USERNAME    + m_strHeader_UserName + _T("\r\n") + 
		HTTPDATAPACKAGE_SEND_HEADERSNAME_RANDOM		 + m_strHeader_Random + _T("\r\n") +
		HTTPDATAPACKAGE_SEND_HEADERSNAME_CRYPTOGRAPH + m_strHeader_Cryptograph + _T("\r\n") + 
		HTTPDATAPACKAGE_SEND_HEADERSNAME_VERSION	 + m_strHeader_Version + _T("\r\n") + 
		HTTPDATAPACKAGE_SEND_HEADERSNAME_BLOGNAME    + m_strHeader_BlogName + _T("\r\n") + 
		HTTPDATAPACKAGE_SEND_HEADERSNAME_FUNCTION    + m_strHeader_Function;// + "\r\n";
		bRet = true;
	}
	return bRet;
}

//由 SetHttp_Combine_SendContent() 调用，将本类中的 GetContent_DataInfo_ItemCount 
//GetContent_DataInfo_Item GetContent_DataItem 数据组合成要发送的
//数据包内容 m_strContent_DataAll
bool CHttpDataPackage_Send::SetContent_DataAll()
{
	bool bRet = true;
	m_strContent_DataAll = m_strContent_TransferType 
						 + m_strContent_DataCRC;

	m_strContent_DataAll += m_strContent_DataInfo_ItemCount;
	int nDataCount = (int)m_strArrayContent_DataInfo_Item.GetSize();
	if(nDataCount > 0)
	{
		for(int i=0; i<nDataCount; i++)
		{
			m_strContent_DataAll += m_strArrayContent_DataInfo_Item.GetAt(i);
		}
		for(i=0; i<nDataCount; i++)
		{
			m_strContent_DataAll += m_strArrayContent_DataItem.GetAt(i);
		}
		bRet = true;
	}
	return bRet;
}//*/

bool CHttpDataPackage_Send::SetHttp_Combine_SendContent()
{
	bool bRet = true;
	m_strContent_DataAll.Empty();
	m_strArrayContent_DataInfo_Item.RemoveAll();
	//检查参数合法性
	int nDataCount = (int)m_strArrayContent_DataItem.GetSize();
	CString strDataItem = _T("");
	CString strDataItem_Length = _T("");
	for(int i=0; i<nDataCount; i++)
	{
		strDataItem = m_strArrayContent_DataItem.GetAt(i);
		if(!SetHttp_StringLength_Format(strDataItem, strDataItem_Length))
		{
			bRet = false;
			return bRet;
		}
		m_strArrayContent_DataInfo_Item.Add(strDataItem_Length);
	}
	if(!SetHttp_Compute_DataInfo_ItemCount(&m_strArrayContent_DataInfo_Item, 
		m_strContent_DataInfo_ItemCount))
	{
		bRet = false;
		return bRet;
	}
	if(!SetContent_DataAll())
	{
		bRet = false;
		return bRet;
	}
	strDataItem_Length.Empty();
	strDataItem_Length.Format(_T("%d"), m_strContent_DataAll.GetLength());
	SetHeader_DataLength(strDataItem_Length);
	bRet = true;
	return bRet;
}//*/

/////////////////////////////////////////////////////////////////////////////
// CHttpDataPackage_Send message handlers

//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// CHttpDataPackage_Receive

CHttpDataPackage_Receive::CHttpDataPackage_Receive()
{
	InitAllData();
}

CHttpDataPackage_Receive::~CHttpDataPackage_Receive()
{
	ReleaseAllData();
}

void CHttpDataPackage_Receive::ReleaseAllData()
{
	m_strArrayContent_DataInfo_Item.FreeExtra();
	m_strArrayContent_DataInfo_Item.RemoveAll();
	m_strArrayContent_DataItem.FreeExtra();
	m_strArrayContent_DataItem.RemoveAll();
}

void CHttpDataPackage_Receive::InitAllData()
{
	//------ Http头 -------------------------
	//m_strHeader_DataOrigin = _T("");//数据来源
	m_strHeader_DataLength = _T("");//数据总长度
	m_strHeader_Function = _T("");//功能代码
	//------ Http头 -------------------------

	//------ Http内容 -------------------------
	m_strContent_TransferType = _T("");//数据传输类型，比如 S 表示是发送， P 表示是服务器返回
	m_strContent_DataCRC = _T("");//有效性校验码
	//数据信息
	m_strContent_DataInfo_ItemCount = _T("0000");//数据信息项数：4位，为数据信息项数的总和（ = n）

	ReleaseAllData();
	//数据内容
	m_strContent_DataAll = _T("");
	//------ Http内容 -------------------------
}

/////////////////////////////////////////////////////////////////////////////
// CHttpDataPackage_Receive message handlers

//对strHttpContent_Receive 数据进行 CRC码=strHttpContent_CRC 的有效性检验
//返回 true 表示检验的数据有效
bool CHttpDataPackage_Receive::CRC_Http_ReceiveContent(CString strHttpContent_CRC, 
											CString& strHttpContent_Receive)
{
	bool bRet = true;
	return bRet;
}

//在 strHttpHeaders_Receive 中根据 strHttp_Receive_HeaderName 指定的头名字查找
//该名字对应的头的值，并作为返回值
CString CHttpDataPackage_Receive::GetHttp_ReceiveHeader(CString strHttpHeaders_Receive, 
							  CString strHttp_Receive_HeaderName)
{
	CString strReceive_HeaderValue = _T("");
	int nHeaderName_Length = strHttp_Receive_HeaderName.GetLength();
	int nHeaderName_Pos = strHttpHeaders_Receive.Find(strHttp_Receive_HeaderName);
	strHttpHeaders_Receive = strHttpHeaders_Receive.Mid(nHeaderName_Pos);
	nHeaderName_Pos = strHttpHeaders_Receive.Find(_T("\r\n"));
	strReceive_HeaderValue = strHttpHeaders_Receive.Mid(nHeaderName_Length+1, 
										nHeaderName_Pos-nHeaderName_Length-1);

	return strReceive_HeaderValue;
}

//将 strHttpHeaders_Receive 中的有效数据分析、拆分，
//并填写到接收的数据包头 pHttpData_Receive 中去
bool CHttpDataPackage_Receive::SetHttp_ReceiveHeaders(CString strHttpHeaders_Receive)
{
	bool bRet = false;
	if(!strHttpHeaders_Receive.IsEmpty())
	{
		CString strHeader_Value = GetHttp_ReceiveHeader(strHttpHeaders_Receive, 
									HTTPDATAPACKAGE_RECEIVE_HEADERSNAME_DATALENGTH);
		SetHeader_DataLength(strHeader_Value);

		strHeader_Value.Empty();
		strHeader_Value = GetHttp_ReceiveHeader(strHttpHeaders_Receive, 
			HTTPDATAPACKAGE_RECEIVE_HEADERSNAME_FUNCTION);
		SetHeader_Function(strHeader_Value);

		bRet = true;
	}
	else
	{
		CString strError = ERROR_WEB_RECEIVE_EMPTY_HEADERS;
		AfxMessageBox(strError);
		bRet = false;
	}
	return bRet;
}

//将 strHttpContent_Receive 中的有效数据分析并拆分填写到接收的
//数据包 m_strArrayContent_DataInfo_Item 和 m_strArrayContent_DataItem 中去
//bIsAnalyzeServerReply=true 表示将数据分析并填写到 m_strArrayContent_DataItem 中去
bool CHttpDataPackage_Receive::SetHttp_ReceiveContent(CString& strHttpContent_Receive, 
													   bool bIsAnalyzeServerReply)
{
	bool bRet = false;
	int nData_Header_BaseUnit = DATA_HEADER_BASEUNIT;//数据是以 3 位为基准扩充的模式
	int nData_Info_BaseUnit = DATA_INFO_BASEUNIT;//数据是以 4 位为基准扩充的模式
	CString strContent_NotAnalysed = _T("");//还没有被分析的字符串
	CString strTemp = _T("");//临时字符串

	if(!strHttpContent_Receive.IsEmpty())
	{
		SetContent_DataAll(strHttpContent_Receive);
		bRet = true;
		/*//这里的分析请参照文档 "Rabo标准通讯规则.doc"
		//第1位表示传输类型，比如服务器返回成功的话是 P，返回错误的是 E
		SetContent_TransferType(strHttpContent_Receive.GetAt(0));
		strContent_NotAnalysed = strHttpContent_Receive.Mid(1);//第1位已经分析完毕，去掉
		//有效性校验码
		strTemp = _T("");//strContent_NotAnalysed.Mid(0, nData_Header_BaseUnit);
		SetContent_DataCRC(strTemp);
		//有效性校验
		if(CRC_Http_ReceiveContent(strTemp, strHttpContent_Receive))
		{
			//CRC校验完毕，去掉表示 CRC 的 nData_Header_BaseUnit 那么多个位
			//strContent_NotAnalysed = strContent_NotAnalysed.Mid(nData_Header_BaseUnit);
			//数据信息项数
			strTemp = strContent_NotAnalysed.Mid(0, nData_Info_BaseUnit);
			SetContent_DataInfo_ItemCount(strTemp);
			int nData_Info_ItemCount = atoi(strTemp);
			//数据信息项数已经分析完毕，去掉
			strContent_NotAnalysed = strContent_NotAnalysed.Mid(nData_Info_BaseUnit);
			if(nData_Info_ItemCount > 0)
			{//数据信息 有子项
				for(int i=0; i< nData_Info_ItemCount; i++)
				{
					(GetContent_DataInfo_Item())->Add(
						strContent_NotAnalysed.Mid(0, nData_Info_BaseUnit));
					//已经分析完毕一个子项，去掉该子项
					strContent_NotAnalysed = strContent_NotAnalysed.Mid(nData_Info_BaseUnit);
				}
				//最后剩下的就是完整的数据内容信息
				strHttpContent_Receive = strContent_NotAnalysed;
				SetContent_DataAll(strContent_NotAnalysed);
				if(bIsAnalyzeServerReply == true)
				{
					int nLength = 0;
					for(i=0; i<nData_Info_ItemCount; i++)
					{
						nLength = atoi(m_strArrayContent_DataInfo_Item.GetAt(i));
						m_strArrayContent_DataItem.Add(
							strContent_NotAnalysed.Mid(0, nLength));
						strContent_NotAnalysed = strContent_NotAnalysed.Mid(nLength);
					}
				}
			}
			else
			{//允许没有任何 数据信息 项数
			}
			bRet = true;
		}
		else
		{//数据校验失败，所以无效
			strHttpContent_Receive = ERROR_WEB_RECEIVE_CRC_FAILED;
			bRet = false;
		}//*/
	}
	else
	{
		strHttpContent_Receive = ERROR_WEB_RECEIVE_EMPTY_CONTENT;
		bRet = false;
	}
	return bRet;
}
