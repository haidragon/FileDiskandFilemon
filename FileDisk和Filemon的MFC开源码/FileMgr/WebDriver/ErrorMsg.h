#pragma once

//_ZHANGMQ_EDIT_BEGIN_
//////////////////代码编辑开始//////////////////////
//创建者    ZhangMQ
//创建日期  2005/07/18
//创建目的  中文信息头文件
//--------------新代码开始-----------------------------------------------
#ifndef _ZHANGMQ_CHINESE_
#define	_ZHANGMQ_CHINESE_		TRUE
#endif

#ifdef _ZHANGMQ_CHINESE_

//--------------网络部分显示信息-----------------------------------------
#define ERROR_WEB_SEND_CONTENT_EXCEED			_T("发送数据中，发送的字符串长度超过目前设计的总长度！")
#define ERROR_WEB_STRINGLENGTH_FORMAT_EXCEED	_T("字符串长度超过 9999！")
#define ERROR_WEB_LENGTH_FORMAT					_T("对长度进行字符化时出错！")

#define	ERROR_WEB_NO_SEND_HTTPDATAPACKAGE		_T("您的发送数据为空！")
#define	ERROR_WEB_NO_RECEIVE_HTTPDATAPACKAGE	_T("您的接收数据容器为空！")
//Http头
#define	ERROR_WEB_SEND_INVALID_HEADERS			_T("发送数据中，Http头数据不完整！")
//设置发送数据头信息
#define	ERROR_WEB_SEND_INVALID_DATAHEADER		_T("发送数据中，数据头 项数的总和 与 实际跟在后面的数据项个数之和 不同！")
//设置发送数据内容信息
#define	ERROR_WEB_SEND_INVALID_DATAINFO			_T("发送数据中，数据信息 项数的总和 与 实际跟在后面的数据项个数之和 不同！")
#define	ERROR_WEB_SEND_DATAITEM_NULL			_T("发送数据中，数据内容为空（即没有任何数据项）！")

//获取服务器返回信息
#define	ERROR_WEB_INVALID_WEBCONNECTION	        _T("没有有效的网络连接！请重试或检查网络！")

//Http连接自动抛出的错误全部统一为以下提示信息
#define	ERROR_WEB_FAILED_WEBCONNECTION	        _T("网络连接失败！请重试或检查网络！")

//检测服务器返回信息
#define	ERROR_WEB_RECEIVE_INVALID_DATA	        _T("服务器返回的数据无效！请重试！")
#define	ERROR_WEB_RECEIVE_EMPTY_HEADERS	        _T("服务器返回的数据头为空！请重试！")
#define	ERROR_WEB_RECEIVE_EMPTY_CONTENT	        _T("服务器返回的数据内容为空！请重试！")

//服务器 Http 连接出错信息
#define	ERROR_WEB_HTTP400		                _T("无效请求！服务器处理失败，请重试！")
#define	ERROR_WEB_HTTP404		                _T("请求的服务没有找到，请重试！")
#define	ERROR_WEB_HTTP405		                _T("服务器不支持这种请求方式！")
#define	ERROR_WEB_HTTP500		                _T("服务器处理失败，请重试！")
#define	ERROR_WEB_HTTP503		                _T("服务器已满，请稍后重试！")
#define	ERROR_WEB_HTTPUNKNOWN	                _T("未知HTTP连接错误或服务器错误，请重试！")

//设置服务器返回数据包的头信息
#define	ERROR_WEB_RECEIVE_FUNCTION_UNKOWN		_T("未知的Rabo功能！")

//设置服务器返回数据包的内容信息
#define	ERROR_WEB_RECEIVE_CRC_FAILED			_T("您接收到的数据CRC检验失败！")
#define	ERROR_WEB_RECEIVE_TRANSFERTYPE_UNKOWN	_T("未知的Rabo传输类型功能！")

#endif
//--------------新代码结束-----------------------------------------------
//////////////////代码编辑结束//////////////////////
//_ZHANGMQ_EDIT_END_