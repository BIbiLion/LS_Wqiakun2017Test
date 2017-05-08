/*	@(#)Copyright (C) gaoguangtao 2008-2050   msgstr.h	Ver2.06 */
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Message Strings
	Create					: 2008-02-20(Sat)
    Update					: 2008-02-14(Wed)
	Copyright				: gaoguangtao
	Reference				: 
	======================================================================== */

#ifndef	MSGSTR_H
#define	MSGSTR_H

#ifdef	ENGLISH


#else

// RECVDLG
#define BROADCAST_MSGSTR	"广播 来自..."
#define MULTICAST_MSGSTR	"组播 来自..."
#define UNICAST_MSGSTR		"消息 来自..."
#define UNIABSENCE_MSGSTR	"消息 来自 (无模式)..."
#define KEYOPEN_MSGSTR		"上锁"
#define FILEATTBTN_MSGSTR	"(跟随文件)"
#define INTERCEPT_MSGSTR	"Reply"
#define FONTSET_MSGSTR		"字体设置"
#define SIZESET_MSGSTR		"窗口大小设置"
#define SAVEPOS_MSGSTR		"固定大小设置"
#define SAVEFILE_MSGSTR		"保存文件"
#define TRANSSTOP_MSGSTR	"是否要终止传送?"
#define PREPARETRANS_MSGSTR	"正在准备发送文件……"
#define CREATEFAIL_MSGSTR	"无法建立文件!"
#define WRITEFAIL_MSGSTR	"不能写入文件"
#define DECRYPTERR_MSGSTR	"\r\n ==== 自动回复(解密错误) ====\r\n无法更新公钥\r\n收到您的消息\r\n请按刷新按钮\r\n =============================="
#define PATHTOOLONG_MSGSTR	"路径名太长"
#define NOTSAFEPATH_MSGSTR	"没有保存路径"

//MISCDLG
#define CANTEXEC_MSGSTR		"不能执行"
#define EXECPROGRAM_MSGSTR	"%s 打开程序"
#define OPENFILEPRG_MSGSTR	"程序文件(*.exe)\0*.exe\0所有文件(*.*)\0*.*\0\0"
#define CANTAUTH_MSGSTR		"密码验证失败!"
#define NOTSAMEPASS_MSGSTR	"新密码验证失败!"
#define ICQQURL_MSGSTR		"http://scm.atic.com.cn"

//LOGDLG
#define OPENFILESND_MSGSTR	"选择声音文件", "声音文件(*.wav)\0*.wav\0所有文件(*.*)\0*.*\0\0"
#define OPENFILEICON_MSGSTR	"选择图标文件", "图标文件(*.ico,*.exe,*.dll)\0*.ico;*.exe;*.dll\0所有文件(*.*)\0*.*\0\0"
#define OPENFILELOG_MSGSTR	"选择记录文件", "记录文件(*.log)\0*.log\0所有文件(*.*)\0*.*\0\0"
#define LOGALERT_MSGSTR		"有一种可能性，以创建日志文件在远程驱动器.\r\n(如果你想要停止这种戒备状态,描述日志文件作为完整路径)"

//SENDDLG
#define FIRE_MSGSTR				"Send"
#define ABSENCEQUERY_MSGSTR		"无模式. 释放并发送吗?"
#define USERNUM_MSGSTR			"在线用户\r\n"
#define BROADCASTHEAD_MSGSTR	"广播"
#define RETRYSEND_MSGSTR		"发送错误. 重试?"
#define BROADCASTSEND_MSGSTR	"广播成功!"
#define GROUPSELECT_MSGSTR		"选择工作组"
#define DEFAULTSET_MSGSTR		"确定要初始化优先级设置吗?"
#define BROADCASTMSG_MSGSTR		"广播消息"
#define BUSYMSG_MSGSTR			"占线...再试一次"
#define MOVETO_MSGSTR			"设置"
#define SELECT_MSGSTR			"选择"
#define DEFAULTDISP_MSGSTR		"默认显示 "
#define USER_MSGSTR				"用户名"
#define DISPPRIORITY_MSGSTR		"优先级为 %d"
#define NODISP_MSGSTR			"隐藏  "
#define MEMBERCOUNT_MSGSTR		"用户 %2d / %2d"
#define MEMBERCOUNTDEF_MSGSTR	"用户 %2d"
#define TMPNODISPDISP_MSGSTR	"显示隐藏的用户(临时)"
#define RESETPRIORITY_MSGSTR	"取消所有的优先级设置"
#define SORTFILTER_MSGSTR		"设置显示优先级"
#define FINDDLG_MSGSTR			"搜索用户 Ctl+F"
#define FILEATTACHMENU_MSGSTR	"传送文件"
#define FOLDERATTACHMENU_MSGSTR	"传送文件夹"
#define SAVECOLUMN_MSGSTR		"保存列表顺序"
#define COMCTL_MSGSTR			"comctl32.dll版本太低\r\n请加载ver4.71或更高版本\r\n请参考readme.txt"
#define CANTGETORDER_MSGSTR		"不能获得行顺序"
#define MEMBERDISP_MSGSTR		"列表显示设置..."
#define PRIORITY_MSGSTR			"优先级"
#define ABSENCE_MSGSTR			"*"
#define GROUP_MSGSTR			"工作组"
#define HOST_MSGSTR				"主机名"
#define LOGON_MSGSTR			"登录名"
#define IPADDR_MSGSTR			"IP地址"
#define ADDFILE_MSGSTR			"添加文件"
#define OPENFILEALL_MSGSTR		"所有文件(*.*)\0*.*\0\0"
#define FOLDERATTACH_MSGSTR		"选择准备传送的文件"

//LOGMNG
#define BROADCASTLOG_MSGSTR	"(广播)"
#define AUTORETLOG_MSGSTR	"(自动)"
#define MULTICASTLOG_MSGSTR	"(组播)"
#define PASSWDLOG_MSGSTR	"(封装)"
#define SECRETLOG_MSGSTR	"(密封)"
#define FILEATTACH_MSGSTR	"(文件)"
#define ENCRYPT_MSGSTR		"(RSA)"
#define CANTDELLOG_MSGSTR	"无法删除加密文件"

//MAINWIN
#define USEWIN32_MSGSTR		"This Program is Win16 version.\r\nPlease Use <ICQQ for Win32>"
#define ICQQ_MSGSTR		"ICQQ"
#define PASSWDNOTSET_MSGSTR	"没有密码设置\r\n请设置"
#define WIN32EDITION_MSGSTR	"Win32 ed. %s"
#define WIN16EDITION_MSGSTR	"Win16 ed. %s"
#define OPENFIN_MSGSTR		"信封已经被打开"
#define NOTABSENCE_MSGSTR	"无模式"
#define FILEEXTERR_MSGSTR	"无效档案延伸连接"
#define CANTOPENLOG_MSGSTR	"不能打开日志文件"
#define ABSENCEMENU_MSGSTR	"离开"////Leave
#define ABSENCELIFT_MSGSTR	"上线"//On the Line
#define ABSENCESET_MSGSTR	"自定义..."//User-Defined
#define DOWNLOAD_MSGSTR		"文件传送监控器"//FileTransfersMonitor

//MSGMNG
#define SETENVHOST_MSGSTR	"请设置您的主机环境变量"
#define HOSTUNREACH_MSGSTR	"发送(10065) 错误\r\n请在TCP设置里设置默认网关"

//SETUPDLG
#define DEFABSENCE1_MSGSTR	"有事不在\r\n"//\r\nabsence now.
#define DEFABSENCE2_MSGSTR	"吃饭去了.\r\n"//having a meal now.
#define DEFABSENCE3_MSGSTR	"正在开会\r\n"
#define DEFABSENCE4_MSGSTR	"正在会客.\r\n"
#define DEFABSENCE5_MSGSTR	"外出办事.\r\n"
#define DEFABSENCE6_MSGSTR	"回家去了.\r\n"
#define DEFABSENCE7_MSGSTR	"去厕所了!\r\n"
#define DEFABSENCE8_MSGSTR	"我累了.\r\n请勿打扰...\r\n"
#define DEFABSENCEHEAD1_MSGSTR	"事"//absence
#define DEFABSENCEHEAD2_MSGSTR	"餐"//meal
#define DEFABSENCEHEAD3_MSGSTR	"会"//meeting
#define DEFABSENCEHEAD4_MSGSTR	"客"//visitor
#define DEFABSENCEHEAD5_MSGSTR	"外"//out
#define DEFABSENCEHEAD6_MSGSTR	"家"//home
#define DEFABSENCEHEAD7_MSGSTR	"嘘"//Edo
#define DEFABSENCEHEAD8_MSGSTR	"静"//priest
#define CANTRESOLVE_MSGSTR		"无法解决"

// SHAREDLG
#define FILENAME_MSGSTR		"文件名"
#define SIZE_MSGSTR			"大小"
#define LOCATION_MSGSTR		"位置"
#define LUMPBUTTON_MSGSTR	"全部 (%s / %d file)"
#define FILEINFO_MSGSTR		"No: %d / %d   文件名: \"%s\"   大小: %s"
#define PREVBUTTON_MSGSTR	"前一个"
#define NEXTBUTTON_MSGSTR	"后一个"
#define OVERWRITE_MSGSTR	"文件已经存在!\r\n是否覆盖?"
#define ATTENTION_MSGSTR	"提示"
#define DIRSAVE_MSGSTR		"文件夹(不能覆盖)"
#endif

#endif
