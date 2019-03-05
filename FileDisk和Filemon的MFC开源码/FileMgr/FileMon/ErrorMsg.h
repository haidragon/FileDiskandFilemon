#pragma once

#define ERRMSG_WINVER_SUCCESS						"======== 操作系统是Windows2000及以上，程序可以运行！========"
#define ERRMSG_WINVER_FAILURE						"======== 操作系统不是Windows2000及以上，程序无法运行！========"

#define ERRMSG_GET_WINPATH_FAILURE					"获取Windows目录失败！"

#define ERRMSG_SYSFILEFILTER_COPY_FAILURE			"将文件 FileFilter.sys 复制到系统目录失败！"
#define ERRMSG_SYSFILEFILTER_LOAD_FAILURE			"加载驱动文件 FileFilter.sys 失败！"
#define ERRMSG_SYSFILEFILTER_WRONGVERSION			"加载的驱动文件 FileFilter.sys 版本号不正确！如果您安装了一个新的版本，您必须在使用新版本前重新启动您的计算机！"
#define ERRMSG_SYSFILEFILTER_ACCESS_FAILURE			"无法访问设备驱动 FileFilter.sys ！"
#define ERRMSG_SYSFILEFILTER_OTHER_REQUEST			"FileFilter.sys 设备驱动无法卸载！因为当前还有未完成的请求！"
#define ERRMSG_SYSFILEFILTER_UNLOAD_FAILURE			"FileFilter.sys 设备驱动卸载失败！"
#define ERRMSG_SYSFILEFILTER_DEFAULTHOOK_FAILURE	"FileFilter.sys 设备驱动对指定盘进行文件系统过滤失败！"

#define ERRMSG_FILEDISKSERVICE_STOPREMOVE_FAILURE	"停止并移除服务 FileDisk 失败！"

#define ERRMSG_SYSFILEDISK_DEFAULTMOUNT_FAILURE		"FileDisk.sys 驱动加载失败！"
