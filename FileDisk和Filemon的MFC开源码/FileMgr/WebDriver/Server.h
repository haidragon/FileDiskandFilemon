#if !defined(AFX_SERVER_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_)
#define AFX_SERVER_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Server.h : header file
//

// 通讯服务器 URL 外网正式
#define BLOG_CONNECT_SERVER_URL			_T("http://192.168.5.202:8080/server/server?data=")
#define BLOG_CONNECT_SERVER_ADDRESS		_T("92.168.5.202")
#define BLOG_CONNECT_SERVER_PORT		8080
#define BLOG_CONNECT_SERVER_PATH        _T("server/server")
//*/

// 通讯服务器2 URL 外网测试
/*#define BLOG_CONNECT_SERVER_URL			"http://mo.blogcn.com:8821/RaboTest/test?data="
#define BLOG_CONNECT_SERVER_ADDRESS		_T("mo.blogcn.com")
#define BLOG_CONNECT_SERVER_PORT		8821
#define BLOG_CONNECT_SERVER_PATH		"RaboTest/test"
//*/

// 局域网测试服务器 URL 内网测试
/*#define BLOG_CONNECT_SERVER_URL			"http://10.0.0.251:8821/Rabo/test?data="
#define BLOG_CONNECT_SERVER_ADDRESS		"10.0.0.251"
#define BLOG_CONNECT_SERVER_PORT		8821
#define BLOG_CONNECT_SERVER_PATH		"Rabo/test"
//*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVER_H__26FBD501_DC64_4FA6_8DEC_00B55CFF94C0__INCLUDED_)
