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
#define BROADCAST_MSGSTR	"�㲥 ����..."
#define MULTICAST_MSGSTR	"�鲥 ����..."
#define UNICAST_MSGSTR		"��Ϣ ����..."
#define UNIABSENCE_MSGSTR	"��Ϣ ���� (��ģʽ)..."
#define KEYOPEN_MSGSTR		"����"
#define FILEATTBTN_MSGSTR	"(�����ļ�)"
#define INTERCEPT_MSGSTR	"Reply"
#define FONTSET_MSGSTR		"��������"
#define SIZESET_MSGSTR		"���ڴ�С����"
#define SAVEPOS_MSGSTR		"�̶���С����"
#define SAVEFILE_MSGSTR		"�����ļ�"
#define TRANSSTOP_MSGSTR	"�Ƿ�Ҫ��ֹ����?"
#define PREPARETRANS_MSGSTR	"����׼�������ļ�����"
#define CREATEFAIL_MSGSTR	"�޷������ļ�!"
#define WRITEFAIL_MSGSTR	"����д���ļ�"
#define DECRYPTERR_MSGSTR	"\r\n ==== �Զ��ظ�(���ܴ���) ====\r\n�޷����¹�Կ\r\n�յ�������Ϣ\r\n�밴ˢ�°�ť\r\n =============================="
#define PATHTOOLONG_MSGSTR	"·����̫��"
#define NOTSAFEPATH_MSGSTR	"û�б���·��"

//MISCDLG
#define CANTEXEC_MSGSTR		"����ִ��"
#define EXECPROGRAM_MSGSTR	"%s �򿪳���"
#define OPENFILEPRG_MSGSTR	"�����ļ�(*.exe)\0*.exe\0�����ļ�(*.*)\0*.*\0\0"
#define CANTAUTH_MSGSTR		"������֤ʧ��!"
#define NOTSAMEPASS_MSGSTR	"��������֤ʧ��!"
#define ICQQURL_MSGSTR		"http://scm.atic.com.cn"

//LOGDLG
#define OPENFILESND_MSGSTR	"ѡ�������ļ�", "�����ļ�(*.wav)\0*.wav\0�����ļ�(*.*)\0*.*\0\0"
#define OPENFILEICON_MSGSTR	"ѡ��ͼ���ļ�", "ͼ���ļ�(*.ico,*.exe,*.dll)\0*.ico;*.exe;*.dll\0�����ļ�(*.*)\0*.*\0\0"
#define OPENFILELOG_MSGSTR	"ѡ���¼�ļ�", "��¼�ļ�(*.log)\0*.log\0�����ļ�(*.*)\0*.*\0\0"
#define LOGALERT_MSGSTR		"��һ�ֿ����ԣ��Դ�����־�ļ���Զ��������.\r\n(�������Ҫֹͣ���ֽ䱸״̬,������־�ļ���Ϊ����·��)"

//SENDDLG
#define FIRE_MSGSTR				"Send"
#define ABSENCEQUERY_MSGSTR		"��ģʽ. �ͷŲ�������?"
#define USERNUM_MSGSTR			"�����û�\r\n"
#define BROADCASTHEAD_MSGSTR	"�㲥"
#define RETRYSEND_MSGSTR		"���ʹ���. ����?"
#define BROADCASTSEND_MSGSTR	"�㲥�ɹ�!"
#define GROUPSELECT_MSGSTR		"ѡ������"
#define DEFAULTSET_MSGSTR		"ȷ��Ҫ��ʼ�����ȼ�������?"
#define BROADCASTMSG_MSGSTR		"�㲥��Ϣ"
#define BUSYMSG_MSGSTR			"ռ��...����һ��"
#define MOVETO_MSGSTR			"����"
#define SELECT_MSGSTR			"ѡ��"
#define DEFAULTDISP_MSGSTR		"Ĭ����ʾ "
#define USER_MSGSTR				"�û���"
#define DISPPRIORITY_MSGSTR		"���ȼ�Ϊ %d"
#define NODISP_MSGSTR			"����  "
#define MEMBERCOUNT_MSGSTR		"�û� %2d / %2d"
#define MEMBERCOUNTDEF_MSGSTR	"�û� %2d"
#define TMPNODISPDISP_MSGSTR	"��ʾ���ص��û�(��ʱ)"
#define RESETPRIORITY_MSGSTR	"ȡ�����е����ȼ�����"
#define SORTFILTER_MSGSTR		"������ʾ���ȼ�"
#define FINDDLG_MSGSTR			"�����û� Ctl+F"
#define FILEATTACHMENU_MSGSTR	"�����ļ�"
#define FOLDERATTACHMENU_MSGSTR	"�����ļ���"
#define SAVECOLUMN_MSGSTR		"�����б�˳��"
#define COMCTL_MSGSTR			"comctl32.dll�汾̫��\r\n�����ver4.71����߰汾\r\n��ο�readme.txt"
#define CANTGETORDER_MSGSTR		"���ܻ����˳��"
#define MEMBERDISP_MSGSTR		"�б���ʾ����..."
#define PRIORITY_MSGSTR			"���ȼ�"
#define ABSENCE_MSGSTR			"*"
#define GROUP_MSGSTR			"������"
#define HOST_MSGSTR				"������"
#define LOGON_MSGSTR			"��¼��"
#define IPADDR_MSGSTR			"IP��ַ"
#define ADDFILE_MSGSTR			"����ļ�"
#define OPENFILEALL_MSGSTR		"�����ļ�(*.*)\0*.*\0\0"
#define FOLDERATTACH_MSGSTR		"ѡ��׼�����͵��ļ�"

//LOGMNG
#define BROADCASTLOG_MSGSTR	"(�㲥)"
#define AUTORETLOG_MSGSTR	"(�Զ�)"
#define MULTICASTLOG_MSGSTR	"(�鲥)"
#define PASSWDLOG_MSGSTR	"(��װ)"
#define SECRETLOG_MSGSTR	"(�ܷ�)"
#define FILEATTACH_MSGSTR	"(�ļ�)"
#define ENCRYPT_MSGSTR		"(RSA)"
#define CANTDELLOG_MSGSTR	"�޷�ɾ�������ļ�"

//MAINWIN
#define USEWIN32_MSGSTR		"This Program is Win16 version.\r\nPlease Use <ICQQ for Win32>"
#define ICQQ_MSGSTR		"ICQQ"
#define PASSWDNOTSET_MSGSTR	"û����������\r\n������"
#define WIN32EDITION_MSGSTR	"Win32 ed. %s"
#define WIN16EDITION_MSGSTR	"Win16 ed. %s"
#define OPENFIN_MSGSTR		"�ŷ��Ѿ�����"
#define NOTABSENCE_MSGSTR	"��ģʽ"
#define FILEEXTERR_MSGSTR	"��Ч������������"
#define CANTOPENLOG_MSGSTR	"���ܴ���־�ļ�"
#define ABSENCEMENU_MSGSTR	"�뿪"////Leave
#define ABSENCELIFT_MSGSTR	"����"//On the Line
#define ABSENCESET_MSGSTR	"�Զ���..."//User-Defined
#define DOWNLOAD_MSGSTR		"�ļ����ͼ����"//FileTransfersMonitor

//MSGMNG
#define SETENVHOST_MSGSTR	"����������������������"
#define HOSTUNREACH_MSGSTR	"����(10065) ����\r\n����TCP����������Ĭ������"

//SETUPDLG
#define DEFABSENCE1_MSGSTR	"���²���\r\n"//\r\nabsence now.
#define DEFABSENCE2_MSGSTR	"�Է�ȥ��.\r\n"//having a meal now.
#define DEFABSENCE3_MSGSTR	"���ڿ���\r\n"
#define DEFABSENCE4_MSGSTR	"���ڻ��.\r\n"
#define DEFABSENCE5_MSGSTR	"�������.\r\n"
#define DEFABSENCE6_MSGSTR	"�ؼ�ȥ��.\r\n"
#define DEFABSENCE7_MSGSTR	"ȥ������!\r\n"
#define DEFABSENCE8_MSGSTR	"������.\r\n�������...\r\n"
#define DEFABSENCEHEAD1_MSGSTR	"��"//absence
#define DEFABSENCEHEAD2_MSGSTR	"��"//meal
#define DEFABSENCEHEAD3_MSGSTR	"��"//meeting
#define DEFABSENCEHEAD4_MSGSTR	"��"//visitor
#define DEFABSENCEHEAD5_MSGSTR	"��"//out
#define DEFABSENCEHEAD6_MSGSTR	"��"//home
#define DEFABSENCEHEAD7_MSGSTR	"��"//Edo
#define DEFABSENCEHEAD8_MSGSTR	"��"//priest
#define CANTRESOLVE_MSGSTR		"�޷����"

// SHAREDLG
#define FILENAME_MSGSTR		"�ļ���"
#define SIZE_MSGSTR			"��С"
#define LOCATION_MSGSTR		"λ��"
#define LUMPBUTTON_MSGSTR	"ȫ�� (%s / %d file)"
#define FILEINFO_MSGSTR		"No: %d / %d   �ļ���: \"%s\"   ��С: %s"
#define PREVBUTTON_MSGSTR	"ǰһ��"
#define NEXTBUTTON_MSGSTR	"��һ��"
#define OVERWRITE_MSGSTR	"�ļ��Ѿ�����!\r\n�Ƿ񸲸�?"
#define ATTENTION_MSGSTR	"��ʾ"
#define DIRSAVE_MSGSTR		"�ļ���(���ܸ���)"
#endif

#endif
