static char *senddlg_id = 
"@(#)Copyright (C) gaoguangtao 2008-2050   senddlg.cpp	Ver2.06";
/* ========================================================================
Project  Name			: IP Messenger for Win32
Module Name				: Send Dialog
Create					: 2008-02-20(Sat)
Update					: 2008-02-14(Wed)
Copyright				: gaoguangtao
Reference				: 
======================================================================== */
#include "tlib.h"
#include "resource.h"
#include "ICQQ.h"
#include "msgstr.h"
#include "blowfish.h"

#pragma warning(disable: 4355)
/*
SendDialog 
*/
TSendDlg::TSendDlg(MsgMng *_msgmng, ShareMng *_shareMng, THosts *_hosts, Cfg *_cfg, LogMng *_logmng, HWND _hRecvWnd, MsgBuf *_msg)
: TListDlg(SEND_DIALOG), editSub(_cfg, this), separateSub(this), hostListView(this)
{
	hRecvWnd		= _hRecvWnd;
	msgMng			= _msgmng;
	shareMng		= _shareMng;
	shareInfo		= NULL;
	shareStr		= NULL;
	hosts			= _hosts;
	hostArray		= NULL;
	cfg				= _cfg;
	logmng			= _logmng;
	memberCnt		= 0;
	sendEntry		= NULL;
	sendEntryNum	= 0;
	packetNo		= msgMng->MakePacketNo();
	retryCnt		= 0;
	timerID			= 0;
	hEditFont		= NULL;
	hListFont		= NULL;
	captureMode		= FALSE;
	listOperateCnt	= 0;
	hiddenDisp		= FALSE;
	*selectGroup	= 0;
	currentMidYdiff	= cfg->SendMidYdiff;
	memset(&orgFont, 0, sizeof(orgFont));
	maxItems = 0;
	lvStateEnable	= FALSE;
	sortItem = -1;
	sortRev = FALSE;
	findDlg = NULL;

	msg.Init(_msg);

	hAccel = ::LoadAccelerators(TApp::hI, (LPCSTR)ICQQ_ACCEL);
}

/*
SendDialog 
*/
TSendDlg::~TSendDlg()
{
	if (findDlg)
		delete findDlg;

	// ListView 
	SendDlgItemMessage(HOST_LIST, LVM_DELETEALLITEMS, 0, 0);

	if (hListFont)
		::DeleteObject(hListFont);
	if (hEditFont)
		::DeleteObject(hEditFont);

	delete [] sendEntry;
	delete [] shareStr;
	if (hostArray)
		free(hostArray);
}

/*
SendDialog 
*/
BOOL TSendDlg::EvCreate(LPARAM lParam)
{
	SetDlgIcon(hWnd);

	if (msg.hostSub.addr) {
		Host	*host = cfg->priorityHosts.GetHostByName(&msg.hostSub);
		if (host && host->priority <= 0)
			hiddenDisp = TRUE;
	}

	HMENU	hMenu = ::GetSystemMenu(hWnd, FALSE);
	::AppendMenu(hMenu, MF_SEPARATOR, NULL, NULL);
	SetMainMenu(hMenu);

	if (cfg->AbnormalButton)
		SetDlgItemText(IDOK, FIRE_MSGSTR);

	if (cfg->SecretCheck)
		SendDlgItemMessage(SECRET_CHECK, BM_SETCHECK, cfg->SecretCheck, 0);
	else
		::EnableWindow(GetDlgItem(PASSWORD_CHECK), FALSE);

	SetFont();
	SetSize();
	DisplayMemberCnt();

	if (IsNewShell() != TRUE)
	{
		ULONG	style;
		style = GetWindowLong(GWL_STYLE);
		style &= 0xffffff0f;
		style |= 0x00000080;
		SetWindowLong(GWL_STYLE, style);
		style = ::GetWindowLong(GetDlgItem(SEPARATE_STATIC), GWL_STYLE);
		style &= 0xffffff00;
		style |= 0x00000007;
		::SetWindowLong(GetDlgItem(SEPARATE_STATIC), GWL_STYLE, style);
	}
	SetForegroundWindow();

	PostMessage(WM_DELAYSETTEXT, 0, 0);

	editSub.CreateByWnd(GetDlgItem(SEND_EDIT));
	separateSub.CreateByWnd(GetDlgItem(SEPARATE_STATIC));
	hostListView.CreateByWnd(GetDlgItem(HOST_LIST));

#if 0
	SendDlgItemMessage(HOST_LIST, LVM_SETTEXTBKCOLOR, 0, 0x222222);
	SendDlgItemMessage(HOST_LIST, LVM_SETTEXTCOLOR, 0, 0xeeeeee);
	SendDlgItemMessage(HOST_LIST, LVM_SETBKCOLOR, 0, 0x222222);
#endif

	InitializeHeader();

	for (int cnt=0; cnt < hosts->HostCnt(); cnt++)
		AddHost(hosts->GetHost(cnt));
	::SetFocus(hostListView.hWnd);
	if (msg.hostSub.addr)
		SelectHost(&msg.hostSub);

	return	TRUE;
}

/*
Construct/Rebuild Column Header
*/
void TSendDlg::InitializeHeader(void)
{
	int		order[MAX_SENDWIDTH];
	int		revItems[MAX_SENDWIDTH];
	int		cnt;

	while (maxItems > 0)
		SendDlgItemMessage(HOST_LIST, LVM_DELETECOLUMN, --maxItems, 0);

	ColumnItems = cfg->ColumnItems & ~(1 << SW_ABSENCE);
	memcpy(FullOrder, cfg->SendOrder, sizeof(FullOrder));

	for (cnt=0; cnt < MAX_SENDWIDTH; cnt++) {
		if (GetItem(ColumnItems, cnt)) {
			items[maxItems] = cnt;
			revItems[cnt] = maxItems++;
		}
	}
	int		orderCnt = 0;
	for (cnt=0; cnt < MAX_SENDWIDTH; cnt++) {
		if (GetItem(ColumnItems, FullOrder[cnt]))
			order[orderCnt++] = revItems[FullOrder[cnt]];
	}

	DWORD dw = ::GetWindowLong(GetDlgItem(HOST_LIST), GWL_STYLE) | LVS_SHOWSELALWAYS;
	::SetWindowLong(GetDlgItem(HOST_LIST), GWL_STYLE, dw);

	DWORD style = SendDlgItemMessage(HOST_LIST, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0) | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
	if (cfg->GlidLineCheck)
		style |= LVS_EX_GRIDLINES;
	else
		style &= ~LVS_EX_GRIDLINES;
	SendDlgItemMessage(HOST_LIST, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, style);

	static char	*headerStr[MAX_SENDWIDTH];
	if (headerStr[SW_NICKNAME] == NULL) {
		headerStr[SW_NICKNAME]	= USER_MSGSTR;
		headerStr[SW_PRIORITY]	= PRIORITY_MSGSTR;
		headerStr[SW_ABSENCE]	= ABSENCE_MSGSTR;
		headerStr[SW_GROUP]		= GROUP_MSGSTR;
		headerStr[SW_HOST]		= HOST_MSGSTR;
		headerStr[SW_USER]		= LOGON_MSGSTR;
		headerStr[SW_IPADDR]	= IPADDR_MSGSTR;
	}

	LV_COLUMN	lvC;
	memset(&lvC, 0, sizeof(lvC));
	lvC.fmt = LVCFMT_LEFT;
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for (cnt = 0; cnt < maxItems; cnt++) {
		lvC.pszText = headerStr[items[cnt]];
		lvC.cx = cfg->SendWidth[items[cnt]];
		SendDlgItemMessage(HOST_LIST, LVM_INSERTCOLUMN, lvC.iSubItem=cnt, (LPARAM)&lvC);
	}
	SendDlgItemMessage(HOST_LIST, LVM_SETCOLUMNORDERARRAY, maxItems, (LPARAM)order);
}


/*
WM_COMMAND CallBack
*/
BOOL TSendDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)///////
{
	switch (wID)
	{
	case IDOK:
		if (IsSending())
			return	TRUE;
		if (findDlg && findDlg->hWnd)
			return	findDlg->SetForegroundWindow();
		if (cfg->AbsenceCheck && cfg->Debug < 2)
		{
			if (MessageBox(ABSENCEQUERY_MSGSTR, MSG_STR, MB_OKCANCEL) != IDOK)
				return	TRUE;
			::SendMessage(GetMainWnd(), WM_COMMAND, MENU_ABSENCE, 0);
		}
		SendMsg();
		if (shareInfo && shareInfo->fileCnt == 0)
			shareMng->DestroyShare(shareInfo), shareInfo = NULL;
		return	TRUE;

	case IDCANCEL:
		if (findDlg && findDlg->hWnd)
			return	findDlg->Destroy(), TRUE;
		if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) == 0)
		{
			if (shareInfo)
				shareMng->DestroyShare(shareInfo), shareInfo = NULL;
			::PostMessage(GetMainWnd(), WM_SENDDLG_EXIT, 0, (LPARAM)this);
			return	TRUE;
		}
		break;

	case REFRESH_BUTTON:
		::PostMessage(GetMainWnd(), WM_REFRESH_HOST, (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? TRUE : FALSE, 0);
		return	TRUE;

	case IDC_ChatRecord://�����¼
		{
			SHELLEXECUTEINFO	shellExecInfo;

			memset(&shellExecInfo, 0, sizeof(shellExecInfo));
			shellExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			shellExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			shellExecInfo.hwnd = NULL;
			shellExecInfo.lpFile = cfg->LogFile;
			shellExecInfo.nShow = SW_SHOWNORMAL;

			if (!ShellExecuteEx(&shellExecInfo) || (int)shellExecInfo.hInstApp <= WINEXEC_ERR_MAX)
			{
				switch ((int)shellExecInfo.hInstApp)
				{
				case SE_ERR_NOASSOC: case SE_ERR_ASSOCINCOMPLETE:
					MessageBox(FILEEXTERR_MSGSTR);
					break;
				default:
					MessageBox(CANTOPENLOG_MSGSTR);
					break;
				}
			}
		}
		return TRUE;

	case IDC_OK://�رհ�ť
		EndDialog(TRUE);
		break;

	case SECRET_CHECK:
		if (::IsDlgButtonChecked(hWnd, SECRET_CHECK) != 0)
			::EnableWindow(GetDlgItem(PASSWORD_CHECK), TRUE);
		else {
			SendDlgItemMessage(PASSWORD_CHECK, BM_SETCHECK, 0, 0);
			::EnableWindow(GetDlgItem(PASSWORD_CHECK), FALSE);
		}
		break;

	case MENU_FILEADD:
		{
			char	buf[MAX_PATH] = "";
			if (TShareDlg::FileAddDlg(this, shareMng, shareInfo ? shareInfo : (shareInfo = shareMng->CreateShare(packetNo)), cfg))
			{
				SetFileButton(this, FILE_BUTTON, shareInfo);
				EvSize(SIZE_RESTORED, 0, 0);
			}
		}
		break;

	case MENU_FOLDERADD:
		{
			for (int cnt=0; cnt < 5; cnt++)
			{
				if (*cfg->lastOpenDir && GetFileAttributes(cfg->lastOpenDir) == 0xffffffff)
					if (PathToDir(cfg->lastOpenDir, cfg->lastOpenDir) == FALSE)
						break;
			}
			if (BrowseDirDlg(this, FOLDERATTACH_MSGSTR, cfg->lastOpenDir, cfg->lastOpenDir))
			{
				shareMng->AddFileShare(shareInfo ? shareInfo : (shareInfo = shareMng->CreateShare(packetNo)), cfg->lastOpenDir);
				SetFileButton(this, FILE_BUTTON, shareInfo);
				EvSize(SIZE_RESTORED, 0, 0);
			}
		}
		break;

	case FILE_BUTTON:
		TShareDlg(shareMng, shareInfo ? shareInfo : (shareInfo = shareMng->CreateShare(packetNo)), cfg, this).Exec();
		SetFileButton(this, FILE_BUTTON, shareInfo);
		EvSize(SIZE_RESTORED, 0, 0);
		break;

	case MISC_ACCEL:
	case HIDE_ACCEL:
		::PostMessage(GetMainWnd(), WM_COMMAND, wID, 0);
		return	TRUE;

	case ALLSELECT_ACCEL:
		SendDlgItemMessage(SEND_EDIT, EM_SETSEL, 0, (LPARAM)-1);
		return	TRUE;

	case MENU_FINDDLG:
		if (findDlg == NULL)
			findDlg = new TFindDlg(cfg, this);
		if (findDlg->hWnd == NULL)
			findDlg->Create();
		return	TRUE;

	case SEPARATE_STATIC:
		return	TRUE;

	case MENU_SAVEPOS:
		if ((cfg->SendSavePos = !cfg->SendSavePos) != 0)
		{
			GetWindowRect(&rect);
			cfg->SendXpos = rect.left;
			cfg->SendYpos = rect.top;
		}
		cfg->WriteRegistry(CFG_WINSIZE);
		return	TRUE;

	case MENU_SAVESIZE:
		GetWindowRect(&rect);
		cfg->SendXdiff = (rect.right - rect.left) - (orgRect.right - orgRect.left);
		cfg->SendYdiff = (rect.bottom - rect.top) - (orgRect.bottom - orgRect.top);
		cfg->SendMidYdiff = currentMidYdiff;
		cfg->WriteRegistry(CFG_WINSIZE);
		return	TRUE;

	case MENU_SAVECOLUMN:
		{
			GetOrder();
			for (int cnt=0; cnt < maxItems; cnt++)
				cfg->SendWidth[items[cnt]] = SendDlgItemMessage(HOST_LIST, LVM_GETCOLUMNWIDTH, cnt, 0);
			cfg->WriteRegistry(CFG_WINSIZE);
		}
		return	TRUE;

	case MENU_EDITFONT: case MENU_LISTFONT:
		{
			CHOOSEFONT	cf;
			LOGFONT		tmpFont, *targetFont;

			targetFont = wID == MENU_EDITFONT ? &cfg->SendEditFont : &cfg->SendListFont;
			memset(&cf, 0, sizeof(cf));
			cf.lStructSize	= sizeof(cf);
			cf.hwndOwner	= hWnd;
			cf.lpLogFont	= &(tmpFont = *targetFont);
			cf.Flags		= CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_SHOWHELP;
			cf.nFontType	= SCREEN_FONTTYPE;
			if (::ChooseFont(&cf))
			{
				*targetFont = tmpFont;
				SetFont();
				::InvalidateRgn(hWnd, NULL, TRUE);
				cfg->WriteRegistry(CFG_FONT);
			}
		}
		return	TRUE;

	case MENU_DEFAULTFONT:
		cfg->SendEditFont = cfg->SendListFont = orgFont;
#ifdef JAPANESE
		strcpy(cfg->SendListFont.lfFaceName, PROPORTIONAL_FONT);
#endif
		SetFont();
		::InvalidateRgn(hWnd, NULL, TRUE);
		cfg->WriteRegistry(CFG_FONT);
		return	TRUE;

	case MENU_NORMALSIZE:
		GetWindowRect(&rect);
		currentMidYdiff = 0;
		MoveWindow(rect.left, rect.top, orgRect.right - orgRect.left, orgRect.bottom - orgRect.top, TRUE);
		return	TRUE;

	case MENU_MEMBERDISP:
		if (TSortDlg(cfg, this).Exec()) {
			GetOrder();
			cfg->WriteRegistry(CFG_WINSIZE);
			DelAllHost();
			InitializeHeader();
			for (int cnt=0; cnt < hosts->HostCnt(); cnt++)
				AddHost(hosts->GetHost(cnt));
		}
		return	TRUE;

	default:
		if (wID >= MENU_PRIORITY_RESET && wID < MENU_GROUP_START)
		{
			if (wID == MENU_PRIORITY_RESET)
			{
				if (MessageBox(DEFAULTSET_MSGSTR, MSG_STR, MB_OKCANCEL) != IDOK)
					return	TRUE;
				while (cfg->priorityHosts.HostCnt() > 0)
				{
					Host	*host = cfg->priorityHosts.GetHost(0);
					cfg->priorityHosts.DelHost(host);
					if (host->RefCnt() == 0)
						delete host;
				}
				for (int cnt=0; cnt < hosts->HostCnt(); cnt++)
					hosts->GetHost(cnt)->priority = DEFAULT_PRIORITY;
			}
			else if (wID == MENU_PRIORITY_HIDDEN)
			{
				hiddenDisp = !hiddenDisp;
			}
			else if (wID >= MENU_PRIORITY_START && wID < MENU_GROUP_START)
			{
				int	priority = wID - MENU_PRIORITY_START;

				for (int cnt=0; cnt < memberCnt; cnt++)
				{
					if (hostArray[cnt]->priority == priority || (SendDlgItemMessage(HOST_LIST, LVM_GETITEMSTATE, cnt, LVIS_SELECTED) & LVIS_SELECTED) == 0)
						continue;
					if (hostArray[cnt]->priority == DEFAULT_PRIORITY)
						cfg->priorityHosts.AddHost(hostArray[cnt]);
					else if (priority == DEFAULT_PRIORITY)
						cfg->priorityHosts.DelHost(hostArray[cnt]);
					hostArray[cnt]->priority = priority;
				}
			}
			DelAllHost();
			for (int cnt=0; cnt < hosts->HostCnt(); cnt++)
				AddHost(hosts->GetHost(cnt));
			if (wID != MENU_PRIORITY_HIDDEN)
				cfg->WriteRegistry(CFG_HOSTINFO|CFG_DELHOST|CFG_DELCHLDHOST);
		}
		else if (wID >= MENU_GROUP_START && wID < MENU_GROUP_START + (UINT)memberCnt)
		{
			BOOL	ctl_on = (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? TRUE : FALSE;
			BOOL	ensure = FALSE;
			LV_ITEM	lvi;
			memset(&lvi, 0, sizeof(lvi));
			lvi.mask = LVIF_STATE;

			for (lvi.iItem=0; lvi.iItem < memberCnt; lvi.iItem++)
			{
				if (strcmp(selectGroup, hostArray[lvi.iItem]->groupName) == 0 && IsSameHost(&hostArray[lvi.iItem]->hostSub, msgMng->GetLocalHost()) != TRUE) {
					lvi.stateMask = lvi.state = LVIS_FOCUSED|LVIS_SELECTED;
					SendDlgItemMessage(HOST_LIST, LVM_SETITEMSTATE, lvi.iItem, (LPARAM)&lvi);
					if (ensure == FALSE) {
						ensure = TRUE;
						SendDlgItemMessage(HOST_LIST, LVM_ENSUREVISIBLE, lvi.iItem, 0);
						SendDlgItemMessage(HOST_LIST, LVM_SETSELECTIONMARK, 0, lvi.iItem);
					}
				}
				else if (ctl_on == FALSE) {
					lvi.stateMask = LVIS_SELECTED;
					lvi.state = 0;
					SendDlgItemMessage(HOST_LIST, LVM_SETITEMSTATE, lvi.iItem, (LPARAM)&lvi);
				}
			}
		}
		return	TRUE;
	}

	return	FALSE;
}

void TSendDlg::GetOrder(void)
{
	int		order[MAX_SENDWIDTH], orderCnt=0;

	if (SendDlgItemMessage(HOST_LIST, LVM_GETCOLUMNORDERARRAY, maxItems, (LPARAM)order) == FALSE) {
		MessageBox(COMCTL_MSGSTR, CANTGETORDER_MSGSTR);
		return;
	}
	for (int cnt=0; cnt < MAX_SENDWIDTH; cnt++) {
		if (GetItem(ColumnItems, FullOrder[cnt]))
			FullOrder[cnt] = items[order[orderCnt++]];
	}
	memcpy(cfg->SendOrder, FullOrder, sizeof(FullOrder));
}

/*
WM_SYSCOMMAND CallBack
*/
BOOL TSendDlg::EvSysCommand(WPARAM uCmdType, POINTS pos)
{
	switch (uCmdType)
	{
	case MENU_SAVEPOS:
	case MENU_SAVESIZE:
	case MENU_SAVECOLUMN:
	case MENU_FINDDLG:
	case MENU_EDITFONT: case MENU_LISTFONT:
	case MENU_DEFAULTFONT:
	case MENU_NORMALSIZE:
	case MENU_MEMBERDISP:
	case MENU_FILEADD:
	case MENU_FOLDERADD:
		return	EvCommand(0, uCmdType, 0);
	}

	return	FALSE;
}

/*
MenuInit Event CallBack
*/
BOOL TSendDlg::EventInitMenu(UINT uMsg, HMENU hMenu, UINT uPos, BOOL fSystemMenu)
{
	switch (uMsg)
	{
	case WM_INITMENU:
		{
			::ModifyMenu(hMenu, MENU_SAVEPOS, MF_BYCOMMAND|(cfg->SendSavePos ? MF_CHECKED :  0), MENU_SAVEPOS, SAVEPOS_MSGSTR);
		}
		return	TRUE;
	}
	return	FALSE;
}

/*
Color Event CallBack
*/
BOOL TSendDlg::EventCtlColor(UINT uMsg, HDC hDcCtl, HWND hWndCtl, HBRUSH *result)
{
#if 0
	COLORREF	bkref	= 0x0000ff;
	COLORREF	foreref	= 0x00ff00;
	COLORREF	dlgref	= 0xff0000;
	COLORREF	statref	= 0xffff00;

	switch (uMsg) {
	case WM_CTLCOLORDLG:
		{ static HBRUSH hb; if (hb == NULL) hb = ::CreateSolidBrush(dlgref); *result = hb; }
		//		SetTextColor(hDcCtl, foreref);
		//		SetBkColor(hDcCtl, dlgref);
		break;
	case WM_CTLCOLOREDIT:
		{ static HBRUSH hb; if (hb == NULL) hb = ::CreateSolidBrush(bkref); *result = hb; }
		SetTextColor(hDcCtl, foreref);
		SetBkColor(hDcCtl, bkref);
		break;
	case WM_CTLCOLORSTATIC:
		if (GetDlgItem(SEPARATE_STATIC) == hWndCtl) { static HBRUSH hb; if (hb == NULL) hb = ::CreateSolidBrush(bkref); *result = hb; } else { static HBRUSH hb; if (hb == NULL) hb = ::CreateSolidBrush(dlgref); *result = hb; }
		SetTextColor(hDcCtl, statref);
		SetBkColor(hDcCtl, dlgref);
		break;
	}
	return	TRUE;
#else
	return	FALSE;
#endif
}

/*
MenuSelect Event CallBack
*/
BOOL TSendDlg::EvMenuSelect(UINT uItem, UINT fuFlag, HMENU hMenu)
{
	if (uItem >= MENU_GROUP_START && uItem < MENU_GROUP_START + (UINT)memberCnt)
		::GetMenuString(hMenu, uItem, selectGroup, sizeof(selectGroup), MF_BYCOMMAND);
	return	FALSE;
}

/*
DropFiles Event CallBack
*/
BOOL TSendDlg::EvDropFiles(HDROP hDrop)
{
	char	buf[MAX_BUF];
	int		max = ::DragQueryFile(hDrop, ~0UL, 0, 0), cnt;

	if (shareInfo == NULL)
		shareInfo = shareMng->CreateShare(packetNo);

	for (cnt=0; cnt < max; cnt++)
	{
		if (::DragQueryFile(hDrop, cnt, buf, sizeof(buf)) <= 0)
			break;
		shareMng->AddFileShare(shareInfo, buf);
	}
	::DragFinish(hDrop);

	if (shareInfo->fileCnt == 0)
		return	FALSE;

	SetFileButton(this, FILE_BUTTON, shareInfo);
	EvSize(SIZE_RESTORED, 0, 0);

	return	TRUE;
}

char *TSendDlg::GetListItemStr(Host *host, int item)
{
	switch (items[item]) {
	case SW_NICKNAME:
		return	*host->nickName ? host->nickName : host->hostSub.userName;
	case SW_USER:
		return	host->hostSub.userName;
	case SW_ABSENCE:
		return	(host->hostStatus & ICQQ_ABSENCEOPT) ? "*" : "";
	case SW_PRIORITY:
		static char buf[4];
		if (host->priority == DEFAULT_PRIORITY) buf[0] = '-', buf[1] = 0;
		else if (host->priority <= 0) buf[0] = 'X', buf[1] = 0;
		else wsprintf(buf, "%d", cfg->PriorityMax - (host->priority - DEFAULT_PRIORITY) / PRIORITY_OFFSET);
		return	buf;
	case SW_GROUP:
		return	host->groupName;
	case SW_HOST:
		return	host->hostSub.hostName;
	case SW_IPADDR:
		return	inet_ntoa(*(LPIN_ADDR)&host->hostSub.addr);
	}
	return	NULL;
}

/*
Notify Event CallBack
*/
BOOL TSendDlg::EvNotify(UINT ctlID, NMHDR *pNmHdr)
{
	if (pNmHdr->code == LVN_COLUMNCLICK) {
		NM_LISTVIEW	*nmLv = (NM_LISTVIEW *)pNmHdr;
		if (sortItem == items[nmLv->iSubItem]) {
			if ((sortRev = !sortRev) == FALSE && sortItem == 0)
				sortItem = -1;
		}
		else {
			sortItem = items[nmLv->iSubItem];
			sortRev = FALSE;
		}
		DelAllHost();
		for (int cnt=0; cnt < hosts->HostCnt(); cnt++)
			AddHost(hosts->GetHost(cnt));
		return	TRUE;
	}

	if (pNmHdr->code == LVN_GETDISPINFO) {
		LV_DISPINFO		*dispInfo = (LV_DISPINFO *)pNmHdr;
		Host	*host = (Host *)dispInfo->item.lParam;

		dispInfo->item.pszText = GetListItemStr(host, dispInfo->item.iSubItem);
		return	TRUE;
	}

	return	FALSE;
}


/*
WM_MOUSEMOVE CallBack
*/
BOOL TSendDlg::EvMouseMove(UINT fwKeys, POINTS pos)
{

	if ((fwKeys & MK_LBUTTON) && captureMode)
	{
		if (lastYPos == (int)pos.y)
			return	TRUE;
		lastYPos = (int)pos.y;

		RECT	tmpRect;
		int		min_y = (5 * (item[refresh_item].y + item[refresh_item].cy) - 3 * item[separate_item].y) / 2;

		if (pos.y < min_y)
			pos.y = min_y;

		currentMidYdiff += (int)(short)(pos.y - dividYPos);
		EvSize(SIZE_RESTORED, 0, 0);
		GetWindowRect(&tmpRect);
		MoveWindow(tmpRect.left, tmpRect.top, tmpRect.right - tmpRect.left, tmpRect.bottom - tmpRect.top, TRUE);
		dividYPos = (int)pos.y;
		return	TRUE;
	}
	return	FALSE;
}

BOOL TSendDlg::EventButton(UINT uMsg, int nHitTest, POINTS pos)
{
	switch (uMsg)
	{
	case WM_LBUTTONUP:
		if (captureMode)
		{
			captureMode = FALSE;
			::ReleaseCapture();
			return	TRUE;
		}
		break;
	}
	return	FALSE;
}

BOOL TSendDlg::EvSize(UINT fwSizeType, WORD nWidth, WORD nHeight)///////////
{
	if (fwSizeType != SIZE_RESTORED && fwSizeType != SIZE_MAXIMIZED)
		return	FALSE;

	GetWindowRect(&rect);
	int	xdiff = (rect.right - rect.left) - (orgRect.right - orgRect.left);
	int ydiff = (rect.bottom - rect.top) - (orgRect.bottom - orgRect.top);

	HDWP	hdwp = ::BeginDeferWindowPos(max_senditem);
	WINPOS	*wpos;
	BOOL	isFileBtn = shareInfo && shareInfo->fileCnt > 0 ? TRUE : FALSE;
	UINT	dwFlg = (IsNewShell() ? SWP_SHOWWINDOW : SWP_NOREDRAW) | SWP_NOZORDER;
	if (hdwp == NULL)
		return	FALSE;

	wpos = &item[host_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(HOST_LIST), NULL, wpos->x, wpos->y, wpos->cx + xdiff, wpos->cy + currentMidYdiff, dwFlg)) == NULL)
		return	FALSE;

	wpos = &item[member_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(MEMBERCNT_TEXT), NULL, wpos->x + xdiff, wpos->y + (currentMidYdiff >= 0 ? 0 : currentMidYdiff / 2), wpos->cx, wpos->cy, dwFlg)) == NULL)
		return	FALSE;

	wpos = &item[refresh_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(REFRESH_BUTTON), NULL, wpos->x + xdiff, wpos->y + (currentMidYdiff >= 0 ? 0 : currentMidYdiff * 2 / 3), wpos->cx, wpos->cy, dwFlg)) == NULL)
		return	FALSE;

	wpos = &item[file_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(FILE_BUTTON), NULL, wpos->x, wpos->y + currentMidYdiff, wpos->cx + xdiff, wpos->cy, isFileBtn ? dwFlg : (SWP_HIDEWINDOW|SWP_NOZORDER))) == NULL)
		return	FALSE;

	wpos = &item[edit_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(SEND_EDIT), NULL, wpos->x, (isFileBtn ? wpos->y : item[file_item].y) + currentMidYdiff, wpos->cx + xdiff, wpos->cy + ydiff - currentMidYdiff + (isFileBtn ? 0 : wpos->y - item[file_item].y), dwFlg)) == NULL)
		return	FALSE;

	wpos = &item[ok_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(IDOK), NULL, wpos->x + (xdiff >= 0 ? xdiff / 2 : xdiff * 6 / 7), wpos->y + ydiff, wpos->cx, wpos->cy, dwFlg)) == NULL)
		return	FALSE;

	wpos = &item[Ok];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(IDC_OK), NULL, wpos->x + (xdiff >= 0 ? xdiff / 2 : xdiff * 3 / 4), wpos->y + ydiff, wpos->cx + (xdiff >= 0 ? 0 : xdiff / 4), wpos->cy, dwFlg)) == NULL)
		return	FALSE;

	///////////////////////////////////
	wpos = &item[ChatRecord];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(IDC_ChatRecord), NULL, wpos->x + (xdiff >= 0 ? xdiff / 2 : xdiff * 3 / 4), wpos->y + ydiff, wpos->cx + (xdiff >= 0 ? 0 : xdiff / 4), wpos->cy, dwFlg)) == NULL)
		return	FALSE;

	wpos = &item[passwd_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(PASSWORD_CHECK), NULL, wpos->x + (xdiff >= 0 ? xdiff / 2 : xdiff), wpos->y + ydiff, wpos->cx, wpos->cy, dwFlg)) == NULL)
		return	FALSE;

	wpos = &item[secret_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(SECRET_CHECK), NULL, wpos->x + (xdiff >= 0 ? xdiff / 2 : xdiff), wpos->y + ydiff, wpos->cx, wpos->cy, dwFlg)) == NULL)
		return	FALSE;

	wpos = &item[separate_item];
	if ((hdwp = ::DeferWindowPos(hdwp, GetDlgItem(SEPARATE_STATIC), NULL, wpos->x, wpos->y + currentMidYdiff, wpos->cx + xdiff, wpos->cy, dwFlg)) == NULL)
		return	FALSE;

	EndDeferWindowPos(hdwp);

	if (!IsNewShell())
		::InvalidateRgn(hWnd, NULL, TRUE);
	else if (captureMode)
	{
		::InvalidateRgn(GetDlgItem(PASSWORD_CHECK), NULL, TRUE);
		::InvalidateRgn(GetDlgItem(SECRET_CHECK), NULL, TRUE);
		::InvalidateRgn(GetDlgItem(IDOK), NULL, TRUE);
		::InvalidateRgn(GetDlgItem(IDC_OK), NULL, TRUE);
		::InvalidateRgn(GetDlgItem(IDC_ChatRecord), NULL, TRUE);

	}

	return	TRUE;
}

BOOL TSendDlg::EvGetMinMaxInfo(MINMAXINFO *info)
{
	info->ptMinTrackSize.x = (orgRect.right - orgRect.left) * 2 / 3;
	info->ptMinTrackSize.y = (item[separate_item].y + item[separate_item].cy + currentMidYdiff) + (shareInfo && shareInfo->fileCnt ? 130 : 95);
	info->ptMaxTrackSize.y = 10000;

	return	TRUE;
}

/*
Context Menu event call back
*/
BOOL TSendDlg::EvContextMenu(HWND childWnd, POINTS pos)
{
	PopupContextMenu(pos);
	return	TRUE;
}

BOOL TSendDlg::EvMeasureItem(UINT ctlID, MEASUREITEMSTRUCT *lpMis)
{
	return	FALSE;
}

BOOL TSendDlg::EvDrawItem(UINT ctlID, DRAWITEMSTRUCT *lpDis)
{
	return	FALSE;
}

BOOL TSendDlg::EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DELAYSETTEXT:
		SetQuoteStr(msg.msgBuf, cfg->QuoteStr);
		return	TRUE;

	case WM_SENDDLG_RESIZE:
		if (captureMode != TRUE)
		{
			POINT	pt;
			captureMode = TRUE;
			::SetCapture(hWnd);
			::GetCursorPos(&pt);
			::ScreenToClient(hWnd, &pt);
			dividYPos = pt.y;
			lastYPos = 0;
		}
		return	TRUE;
	}
	return	FALSE;
}

/*
WM_TIMER event call back
*/
BOOL TSendDlg::EvTimer(WPARAM _timerID, TIMERPROC proc)
{
	if (IsSendFinish())
	{
		::KillTimer(hWnd, ICQQ_SEND_TIMER);
		if (timerID == ICQQ_DUMMY_TIMER)
			return	FALSE;
		timerID = ICQQ_DUMMY_TIMER;
		::PostMessage(GetMainWnd(), WM_SENDDLG_EXIT, 0, (LPARAM)this);
		return	TRUE;
	}
	if (retryCnt++ <= cfg->RetryMax)
	{
		SendMsgSub();
		return	TRUE;
	}

	::KillTimer(hWnd, ICQQ_SEND_TIMER);
	char *buf = new char [MAX_UDPBUF];
	*buf = 0;

	for (int cnt=0; cnt < sendEntryNum; cnt++)
	{
		if (sendEntry[cnt].Status() != ST_DONE)
		{
			MakeListString(cfg, sendEntry[cnt].Host(), buf + strlen(buf));
			strcat(buf, "\r\n");
		}
	}
	strcat(buf, RETRYSEND_MSGSTR);
	int ret = ::MessageBox(hWnd, buf, MSG_STR, MB_RETRYCANCEL|MB_ICONINFORMATION);
	delete [] buf;

	if (ret == IDRETRY && IsSendFinish() != TRUE)
	{
		retryCnt = 0;
		SendMsgSub();
		timerID = ICQQ_SEND_TIMER;
		if (::SetTimer(hWnd, ICQQ_SEND_TIMER, cfg->RetryMSec, NULL) == 0)
			::PostMessage(GetMainWnd(), WM_SENDDLG_EXIT, 0, (LPARAM)this);
	}
	else
		::PostMessage(GetMainWnd(), WM_SENDDLG_EXIT, 0, (LPARAM)this);

	return	TRUE;
}

void TSendDlg::Show(int mode)
{
	if (timerID == 0 && hWnd)
		TWin::Show(mode);
}

void TSendDlg::SetQuoteStr(LPSTR str, LPCSTR quoteStr)
{
	char	*buf = new char[MAX_UDPBUF];
	char	*tok = str, *tok2, *buf2;

	strcpy(buf, quoteStr);
	buf2 = buf + strlen(buf);

	while (1)
	{
		tok2 = strstr(tok, "\r\n");
		int len = tok2 ? tok2 - tok : strlen(tok);
		strncpy(buf2, tok, len);
		strcpy(buf2 + len, "\r\n");

		if (tok2 != NULL || len != 0)
			SendDlgItemMessage(SEND_EDIT, EM_REPLACESEL, 0, (LPARAM)buf);

		if (tok2 == NULL)
			break;

		tok = tok2 +2;
	}
	delete	[] buf;
}

/*
HostEntry
*/
void TSendDlg::AddHost(Host *host)
{
	if (IsSending() || host->priority <= 0 && hiddenDisp == FALSE)
		return;

#define MAX_ICON	5
	LV_ITEM	lvi;

	lvi.mask = LVIF_TEXT|LVIF_PARAM|(lvStateEnable ? LVIF_STATE : LVIF_IMAGE);
	lvi.iItem = GetInsertIndexPoint(host);
	lvi.iSubItem = 0;
	int	state = ((host->hostStatus & ICQQ_ABSENCEOPT) ? 0 : 1) | ((host->hostStatus & ICQQ_ENCRYPTOPT) ? 4 : ((host->hostStatus & ICQQ_FILEATTACHOPT) ? 2 : 0));
	lvi.state = INDEXTOSTATEIMAGEMASK((state + 1) % (MAX_ICON + 1));
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.pszText = GetListItemStr(host, 0);
	lvi.cchTextMax = 0;
	lvi.iImage = 0;
	lvi.lParam = (LPARAM)host;

	listOperateCnt++;
	int		index;
	if ((index = (int)SendDlgItemMessage(HOST_LIST, LVM_INSERTITEM, 0, (LPARAM)&lvi)) >= 0)
	{
#define BIG_ALLOC	100
		if ((memberCnt % BIG_ALLOC) == 0)
			hostArray = (Host **)realloc(hostArray, (memberCnt + BIG_ALLOC) * sizeof(Host *));
		memmove(hostArray + index +1, hostArray + index, (memberCnt++ - index) * sizeof(Host *));
		hostArray[index] = host;
		DisplayMemberCnt();
	}
	listOperateCnt--;
}

/*
HostEntry
*/
void TSendDlg::ModifyHost(Host *host)
{
	DelHost(host);
	AddHost(host);
}

/*
HostEntry
*/
void TSendDlg::DelHost(Host *host)
{
	if (IsSending())
		return;

	listOperateCnt++;

	int		index;

	for (index=0; index < memberCnt; index++)
		if (hostArray[index] == host)
			break;

	if (index < memberCnt && SendDlgItemMessage(HOST_LIST, LVM_DELETEITEM, index, 0) != LB_ERR)
	{
		memmove(hostArray + index, hostArray + index +1, (memberCnt - index -1) * sizeof(Host *));
		memberCnt--;
		DisplayMemberCnt();
	}
	listOperateCnt--;
}

void TSendDlg::DelAllHost(void)
{
	if (IsSending())
		return;
	SendDlgItemMessage(HOST_LIST, LVM_DELETEALLITEMS, 0, 0);
	free(hostArray);
	hostArray = NULL;
	memberCnt = 0;
	DisplayMemberCnt();
}

UINT TSendDlg::GetInsertIndexPoint(Host *host)
{
	int		index, min = 0, max = memberCnt -1, ret;

	while (min <= max)
	{
		index = (min + max) / 2;

		if ((ret = CompareHosts(host, hostArray[index])) > 0)
			min = index +1;
		else 
			if (ret < 0)
				max = index -1;
			else
			{
				min = index;
				break;
			}
	}

	return	min;
}

#ifdef JAPANESE
#define IS_KANJI(x) ((x) & 0x80)
#else
#define IS_KANJI(x) (0)
#endif

int TSendDlg::CompareHosts(Host *host1, Host *host2)
{
	int		ret = 0;

	if (host1->hostStatus & ICQQ_SERVEROPT)
		return	1;
	if (host2->hostStatus & ICQQ_SERVEROPT)
		return	-1;

	if (sortItem != -1) {
		switch (sortItem) {
		case SW_NICKNAME:
			ret = strcmp(*host1->nickName ? host1->nickName : host1->hostSub.userName, *host2->nickName ? host2->nickName : host2->hostSub.userName); break;
		case SW_ABSENCE:
			ret = (host1->hostStatus & ICQQ_ABSENCEOPT) > (host2->hostStatus & ICQQ_ABSENCEOPT) ? 1 : (host1->hostStatus & ICQQ_ABSENCEOPT) < (host2->hostStatus & ICQQ_ABSENCEOPT) ? -1 : 0; break;
		case SW_GROUP:
			ret = strcmp(*host1->groupName ? host1->groupName : "\xff", *host2->groupName ? host2->groupName : "\xff"); break;
		case SW_HOST:
			ret = strcmp(host1->hostSub.hostName, host2->hostSub.hostName); break;
		case SW_IPADDR:
			ret = ntohl(host1->hostSub.addr) > ntohl(host2->hostSub.addr) ? 1 : -1;
			break;
		case SW_USER:
			ret = strcmp(host1->hostSub.userName, host2->hostSub.userName); break;
		case SW_PRIORITY:
			ret = host1->priority > host2->priority ? 1 : host1->priority < host2->priority ? -1 : 0; break;
		}
		if (ret)
			return	sortRev ? -ret : ret;
	}

	if (host1->priority < host2->priority)
		ret = 1;
	else if (host1->priority > host2->priority)
		ret = -1;
	else if ((ret = GroupCompare(host1, host2)) == 0)
		ret = SubCompare(host1, host2);

	return	(cfg->Sort & ICQQ_ALLREVSORTOPT) || sortRev ? -ret : ret;
}

int TSendDlg::GroupCompare(Host *host1, Host *host2)
{
	int	ret = 0;

	if (!(cfg->Sort & ICQQ_NOGROUPSORTOPT) && *cfg->GroupNameStr)
	{
		if ((ret = strcmp(host1->groupName, host2->groupName)) != 0)
		{
			if (strcmp(host1->groupName, cfg->GroupNameStr) == 0)
				ret = -1;
			else if (strcmp(host2->groupName, cfg->GroupNameStr) == 0)
				ret = 1;
			else
			{
				if (*host1->groupName == 0)
					ret = 1;
				else if (*host2->groupName == 0)
					ret = -1;
				else if (!(cfg->Sort & ICQQ_NOKANJISORTOPT))
				{
					if (IS_KANJI(*host1->groupName) && !IS_KANJI(*host2->groupName))
						ret = -1;
					else if (!IS_KANJI(*host1->groupName) && IS_KANJI(*host2->groupName))
						ret = 1;
				}
				ret = (cfg->Sort & ICQQ_GROUPREVSORTOPT) ? -ret : ret;
			}
		}
	}
	return	ret;
}

int TSendDlg::SubCompare(Host *host1, Host *host2)
{
	int ret = 0, (*StrCmp)(const char*, const char*) = (cfg->Sort & ICQQ_ICMPSORTOPT) ? stricmp : strcmp;

	switch (GET_MODE(cfg->Sort))
	{
	case ICQQ_NAMESORT: default:
		char	*name1, *name2;

		name1 = *host1->nickName ? host1->nickName : host1->hostSub.userName;
		name2 = *host2->nickName ? host2->nickName : host2->hostSub.userName;
		if (!(cfg->Sort & ICQQ_NOKANJISORTOPT))
		{
			if (IS_KANJI(*name1) && !IS_KANJI(*name2))
				ret = -1;
			if (!IS_KANJI(*name1) && IS_KANJI(*name2))
				ret = 1;
		}
		if (ret == 0)
			if ((ret = StrCmp(name1, name2)) == 0)
				if ((ret = StrCmp(host1->hostSub.hostName, host2->hostSub.hostName)) == 0)
					ret = StrCmp(host1->hostSub.userName, host2->hostSub.userName);
		break;

	case ICQQ_HOSTSORT:
		if (!(cfg->Sort & ICQQ_NOKANJISORTOPT))
		{
			if (IS_KANJI(*host1->hostSub.hostName) && !IS_KANJI(*host2->hostSub.hostName))
				ret = 1;
			if (!IS_KANJI(*host1->hostSub.hostName) && IS_KANJI(*host2->hostSub.hostName))
				ret = -1;
		}
		if (ret == 0)
			ret = StrCmp(host1->hostSub.hostName, host2->hostSub.hostName);
		if (ret)
			break;

	case ICQQ_IPADDRSORT:
		if (ntohl(host1->hostSub.addr) > ntohl(host2->hostSub.addr))
			ret = 1;
		else
			ret = -1;
		break;
	}
	return	(cfg->Sort & ICQQ_SUBREVSORTOPT) ? -ret : ret;
}

void TSendDlg::SelectHost(HostSub *hostSub, BOOL force)
{
	int		index;
	LV_ITEM	lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_STATE;
	lvi.stateMask = LVIS_FOCUSED|LVIS_SELECTED;

	for (index=0; index < memberCnt; index++) {
		if (hostSub->addr == hostArray[index]->hostSub.addr && hostSub->portNo == hostArray[index]->hostSub.portNo) {
			lvi.state = LVIS_FOCUSED|LVIS_SELECTED;
			SendDlgItemMessage(HOST_LIST, LVM_SETITEMSTATE, index, (LPARAM)&lvi);
			SendDlgItemMessage(HOST_LIST, LVM_ENSUREVISIBLE, index, 0);
			SendDlgItemMessage(HOST_LIST, LVM_SETSELECTIONMARK, 0, index);
			hostListView.SetFocusIndex(index);
			if (force == FALSE)
				return;
		}
		else if (force) {
			lvi.state = 0;
			SendDlgItemMessage(HOST_LIST, LVM_SETITEMSTATE, index, (LPARAM)&lvi);
		}
	}
}

void TSendDlg::DisplayMemberCnt(void)
{
	static char	buf[MAX_LISTBUF] = USERNUM_MSGSTR;
	static char	*append_point;

	if (append_point == NULL)
		append_point = buf + strlen(buf);

	wsprintf(append_point, "%d", memberCnt);
	SetDlgItemText(MEMBERCNT_TEXT, buf);
}

BOOL TSendDlg::SendMsg(void)
{
	command = ICQQ_SENDMSG|ICQQ_SENDCHECKOPT;

	BOOL ctl_on = (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? TRUE : FALSE;
	BOOL shift_on = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? TRUE : FALSE;
	BOOL rbutton_on = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? TRUE : FALSE;

	if (ctl_on && shift_on)
		command = rbutton_on ? ICQQ_GETINFO : ICQQ_GETABSENCEINFO;
	else if (ctl_on || shift_on || rbutton_on)
		return	FALSE;

	if (listOperateCnt) {
		MessageBox(BUSYMSG_MSGSTR);
		return	FALSE;
	}

	if (::IsDlgButtonChecked(hWnd, SECRET_CHECK) != 0)
		command |= ICQQ_SECRETEXOPT;
	if (::IsDlgButtonChecked(hWnd, PASSWORD_CHECK) != 0)
		command |= ICQQ_PASSWORDOPT;
	if (shareInfo && shareInfo->fileCnt)
		command |= ICQQ_FILEATTACHOPT;

	if ((sendEntryNum = (int)SendDlgItemMessage(HOST_LIST, LVM_GETSELECTEDCOUNT, 0, 0)) <= 0 || (sendEntry = new SendEntry [sendEntryNum]) == NULL)
		return	FALSE;

	timerID = ICQQ_DUMMY_TIMER;

	GetDlgItemText(SEND_EDIT, msg.msgBuf, MAX_UDPBUF);

	logmng->WriteSendStart();

	if (sendEntryNum > 1)
		command |= ICQQ_MULTICASTOPT;

	int	storeCnt = 0, status = 0, cnt;
	int localStatus = sendEntryNum <= cfg->EncryptNum && (cfg->pubKey.Key() || cfg->smallPubKey.Key()) ? ICQQ_ENCRYPTOPT : 0;

	for (cnt=0; cnt < memberCnt && storeCnt < sendEntryNum; cnt++)
	{
		if ((SendDlgItemMessage(HOST_LIST, LVM_GETITEMSTATE, cnt, LVIS_SELECTED) & LVIS_SELECTED) == 0)
			continue;
		char		hostStr[MAX_LISTBUF];
		Host		*host = hostArray[cnt];
		SendEntry	*entry = &sendEntry[storeCnt++];

		status |= host->hostStatus & ICQQ_ENCRYPTOPT;
		MakeListString(cfg, host, hostStr);
		logmng->WriteSendHead(hostStr);

		entry->SetHost(host);
		entry->SetStatus((localStatus & host->hostStatus) ? host->pubKey.Key() == NULL ? ST_GETCRYPT : ST_MAKECRYPTMSG : ST_MAKEMSG);
		entry->SetCommand(command | (entry->Status() == ST_MAKEMSG ? 0 : ICQQ_ENCRYPTOPT));
	}

	msg.msgBuf[MAX_CRYPTLEN] = 0;
	if (status &= localStatus)
		command |= ICQQ_ENCRYPTOPT;

	logmng->WriteSendMsg(msg.msgBuf, command, shareInfo);

	if (shareInfo && shareInfo->fileCnt)
	{
		char	buf[MAX_UDPBUF / 2];
		EncodeShareMsg(shareInfo, buf, sizeof(buf));
		shareStr = new char [strlen(buf) + 1];
		strcpy(shareStr, buf);
		shareMng->AddHostShare(shareInfo, sendEntry, sendEntryNum);
	}

	SendMsgSub();

	timerID = ICQQ_SEND_TIMER;
	if (::SetTimer(hWnd, ICQQ_SEND_TIMER, cfg->RetryMSec, NULL) == 0)
		::PostMessage(GetMainWnd(), WM_SENDDLG_EXIT, 0, (LPARAM)this);
	::SendMessage(GetMainWnd(), WM_SENDDLG_HIDE, 0, (LPARAM)this);
	TWin::Show(SW_HIDE);
	return	TRUE;
}

BOOL TSendDlg::MakeEncryptPacket(SendEntry *entry)
{
	char	*tmpbuf = new char[MAX_UDPBUF];
	char	*buf = new char[MAX_UDPBUF];
	int		msgLen;

	if (MakeEncryptMsg(entry->Host(), msg.msgBuf, tmpbuf) == FALSE)
	{
		entry->SetCommand(entry->Command() & ~ICQQ_ENCRYPTOPT);
		strncpyz(tmpbuf, msg.msgBuf, MAX_UDPBUF);
	}
	msgMng->MakeMsg(buf, packetNo, entry->Command(), tmpbuf, shareStr, &msgLen);
	entry->SetMsg(buf, msgLen);
	entry->SetStatus(ST_SENDMSG);

	delete [] buf;
	delete [] tmpbuf;
	return	TRUE;
}

BOOL TSendDlg::MakeEncryptMsg(Host *host, char *msgstr, char *buf)
{
	HCRYPTKEY	hExKey = 0, hKey = 0;
	BYTE		skey[MAX_BUF], data[MAX_UDPBUF];
	int			len, capa = host->pubKey.Capa() & (cfg->pubKey.Capa()|cfg->smallPubKey.Capa());
	HCRYPTPROV	target_csp = (capa & ICQQ_RSA_1024) ? cfg->hCsp : cfg->hSmallCsp;
	DWORD 		msgLen;

	if ((capa & ICQQ_RSA_1024) && (capa & ICQQ_BLOWFISH_128))
		capa = ICQQ_RSA_1024 | ICQQ_BLOWFISH_128;
	else if ((capa & ICQQ_RSA_512) && (capa & ICQQ_RC2_40))
		capa = ICQQ_RSA_512 | ICQQ_RC2_40;
	else
		return	FALSE;

	// KeyBlob
	host->pubKey.KeyBlob(data, sizeof(data), &len);

	if (!pCryptImportKey(target_csp, data, len, 0, 0, &hExKey))
		return GetLastErrorMsg("CryptImportKey"), FALSE;

	if (capa & ICQQ_BLOWFISH_128)
	{	
		if (!pCryptGenRandom(target_csp, len = 128/8, data))
			return	GetLastErrorMsg("CryptGenRandom"), FALSE;

		CBlowFish	bl(data, len);

		if (!pCryptEncrypt(hExKey, 0, TRUE, 0, data, (DWORD *)&len, MAX_BUF))
			return GetLastErrorMsg("CryptEncrypt"), FALSE;
		bin2hexstr_bigendian(data, len, (char *)skey);

		MsgMng::LocalNewLineToUnix(msgstr, (char *)data, MAX_CRYPTLEN);

		msgLen = bl.Encrypt(data, data, strlen(msgstr)+1);
	}
	else 
	{
		if (!pCryptGenKey(target_csp, CALG_RC2, CRYPT_EXPORTABLE, &hKey))
			return	GetLastErrorMsg("CryptGenKey"), FALSE;

		pCryptExportKey(hKey, hExKey, SIMPLEBLOB, 0, NULL, (DWORD *)&len);
		if (!pCryptExportKey(hKey, hExKey, SIMPLEBLOB, 0, data, (DWORD *)&len))
			return GetLastErrorMsg("CryptExportKey"), FALSE;

		len -= SKEY_HEADER_SIZE;
		bin2hexstr_bigendian(data + SKEY_HEADER_SIZE, len, (char *)skey);

		if (capa & ICQQ_RC2_40)
			MsgMng::LocalNewLineToUnix(msgstr, (char *)data, MAX_CRYPTLEN);
		else
			strncpyz((char *)data, msgstr, MAX_CRYPTLEN);	// beta1-4 only
		msgLen = strlen((char *)data) + 1;

		if (!pCryptEncrypt(hKey, 0, TRUE, 0, data, &msgLen, MAX_UDPBUF))
			return GetLastErrorMsg("CryptEncrypt RC2"), FALSE;
		pCryptDestroyKey(hKey);
	}
	wsprintf(buf, "%X:%s:", capa, skey);
	bin2hexstr(data, (int)msgLen, buf + strlen(buf));
	pCryptDestroyKey(hExKey);

	return TRUE;
}


BOOL TSendDlg::SendMsgSub(void)
{
	BOOL	makeNomalMsg = TRUE;

	for (int cnt=0; cnt < sendEntryNum; cnt++)
	{
		if (sendEntry[cnt].Status() == ST_GETCRYPT) 
		{
			char	spec_str[MAX_BUF];
			int		spec = ICQQ_RSA_512 | ICQQ_RC2_40;

			if (cfg->pubKey.Key())
				spec |= ICQQ_RSA_1024 | ICQQ_BLOWFISH_128;
			wsprintf(spec_str, "%x", spec);
			msgMng->Send(&sendEntry[cnt].Host()->hostSub, ICQQ_GETPUBKEY, spec_str);
		}
		if (sendEntry[cnt].Status() == ST_MAKECRYPTMSG) 
		{
			MakeEncryptPacket(sendEntry + cnt);
		}
		if (sendEntry[cnt].Status() == ST_MAKEMSG)
		{
			sendEntry[cnt].SetStatus(ST_SENDMSG);
			if (makeNomalMsg)
				msgMng->MakeMsg(msgBuf, packetNo, command & ~ICQQ_ENCRYPTOPT, msg.msgBuf, shareStr, &packetLen), makeNomalMsg = FALSE;
		}
		if (sendEntry[cnt].Status() == ST_SENDMSG) 
		{
			const char	*str = sendEntry[cnt].Msg() ? sendEntry[cnt].Msg() : msgBuf;
			int		len = sendEntry[cnt].Msg() ? sendEntry[cnt].MsgLen() : packetLen;

			msgMng->UdpSend(sendEntry[cnt].Host()->hostSub.addr, sendEntry[cnt].Host()->hostSub.portNo, str, len);
		}
	}

	return	TRUE;
}

inline char *strtoupper(char *buf, const char *org)
{
	BOOL dbcsFlg = FALSE;

	for (int cnt=0; buf[cnt] = org[cnt]; cnt++) {
		if (dbcsFlg)
			dbcsFlg = FALSE;
		else if (IsDBCSLeadByte(org[cnt]))
			dbcsFlg = TRUE;
		else
			buf[cnt] = toupper(org[cnt]);
	}
	return	buf;
}

BOOL TSendDlg::FindHost(char *findStr, BOOL isAllfind)
{
	int		startNo = hostListView.GetFocusIndex() + 1;
	char	find_buf[MAX_NAMEBUF], buf[MAX_NAMEBUF];

	if (*findStr == '\0')
		return	FALSE;

	strtoupper(find_buf, findStr);

	for (int cnt=0; cnt < memberCnt; cnt++) {
		int		index = (cnt + startNo) % memberCnt;
		if (*hostArray[index]->nickName)
			strtoupper(buf, hostArray[index]->nickName);
		else
			strtoupper(buf, hostArray[index]->hostSub.userName);

		if (strstr(buf, find_buf) || isAllfind &&
			(strstr(strtoupper(buf, hostArray[index]->groupName), find_buf)
			|| strstr(strtoupper(buf, hostArray[index]->hostSub.hostName), find_buf)
			|| strstr(strtoupper(buf, hostArray[index]->hostSub.userName), find_buf))) {
				SelectHost(&hostArray[index]->hostSub, TRUE);
				return	TRUE;
			}
	}
	return	FALSE;
}

BOOL TSendDlg::SendFinishNotify(HostSub *hostSub, ULONG packet_no)
{
	for (int cnt=0; cnt < sendEntryNum; cnt++)
	{
		if (sendEntry[cnt].Status() == ST_SENDMSG && sendEntry[cnt].Host()->hostSub.addr == hostSub->addr && sendEntry[cnt].Host()->hostSub.portNo == hostSub->portNo && (packet_no == packetNo || packet_no == 0))
		{
			sendEntry[cnt].SetStatus(ST_DONE);

			if (IsSendFinish() && hWnd)
			{
				HWND	hMessageWnd = ::GetNextWindow(hWnd, GW_HWNDPREV);
				if (hMessageWnd && ::GetWindow(hMessageWnd, GW_OWNER) == hWnd)
					::PostMessage(hMessageWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
			}
			return	TRUE;
		}
	}
	return	FALSE;
}

BOOL TSendDlg::SendPubKeyNotify(HostSub *hostSub, BYTE *pubkey, int len, int e, int capa)
{
	for (int cnt=0; cnt < sendEntryNum; cnt++)
	{
		if (sendEntry[cnt].Status() == ST_GETCRYPT && sendEntry[cnt].Host()->hostSub.addr == hostSub->addr && sendEntry[cnt].Host()->hostSub.portNo == hostSub->portNo)
		{
			sendEntry[cnt].Host()->pubKey.Set(pubkey, len, e, capa);
			MakeEncryptPacket(sendEntry + cnt);
			msgMng->UdpSend(sendEntry[cnt].Host()->hostSub.addr, sendEntry[cnt].Host()->hostSub.portNo, sendEntry[cnt].Msg(), sendEntry[cnt].MsgLen());
			return	TRUE;
		}
	}
	return	FALSE;
}

BOOL TSendDlg::IsSending(void)
{
	return	timerID ? TRUE : FALSE;
}

BOOL TSendDlg::IsSendFinish(void)
{
	BOOL	finish = TRUE;

	for (int cnt=0; cnt < sendEntryNum; cnt++)
	{
		if (sendEntry[cnt].Status() != ST_DONE)
		{
			finish = FALSE;
			break;
		}
	}

	return	finish;
}

/*
Font ����
*/
void TSendDlg::SetFont(void)
{
	HFONT	hDlgFont;

	if ((hDlgFont = (HFONT)SendMessage(WM_GETFONT, 0, 0)) == NULL)
		return;
	if (::GetObject(hDlgFont, sizeof(LOGFONT), (LPSTR)&orgFont) == NULL)
		return;

	if (*cfg->SendEditFont.lfFaceName == 0)	
		cfg->SendEditFont = orgFont;
	if (*cfg->SendListFont.lfFaceName == 0) {
		cfg->SendListFont = orgFont;
#ifdef JAPANESE
		strcpy(cfg->SendListFont.lfFaceName, PROPORTIONAL_FONT);
#endif
	}

	if (*cfg->SendListFont.lfFaceName && (hDlgFont = ::CreateFontIndirect(&cfg->SendListFont)) != NULL)
	{
		SendDlgItemMessage(HOST_LIST, WM_SETFONT, (WPARAM)hDlgFont, 0);
		if (hListFont)
			::DeleteObject(hListFont);
		hListFont = hDlgFont;
	}

	if (*cfg->SendEditFont.lfFaceName && (hDlgFont = ::CreateFontIndirect(&cfg->SendEditFont)) != NULL)
	{
		SendDlgItemMessage(SEND_EDIT, WM_SETFONT, (WPARAM)hDlgFont, 0);
		if (hEditFont)
			::DeleteObject(hEditFont);
		hEditFont = hDlgFont;
	}

	static HIMAGELIST	himlState;
	static int			lfHeight;
	static HICON		hiconItem[MAX_ICON];

	if (himlState == NULL || lfHeight != cfg->SendListFont.lfHeight) {
		if (hiconItem[0] == NULL) {
			hiconItem[0] = ::LoadIcon(TApp::hI, (LPCSTR)ICQQV1ABS_ICON);
			hiconItem[1] = ::LoadIcon(TApp::hI, (LPCSTR)ICQQV1_ICON);
			hiconItem[2] = ::LoadIcon(TApp::hI, (LPCSTR)FILEABS_ICON);
			hiconItem[3] = ::LoadIcon(TApp::hI, (LPCSTR)FILE_ICON);
			hiconItem[4] = ::LoadIcon(TApp::hI, (LPCSTR)ABSENCE_ICON);
		}
		if (himlState)
			ImageList_Destroy(himlState);
		lfHeight = cfg->SendListFont.lfHeight;
		himlState = ImageList_Create(abs(cfg->SendListFont.lfHeight) / 4, abs(cfg->SendListFont.lfHeight), TRUE, 1, 1);
		for (int cnt=0; cnt < MAX_ICON; cnt++)
			ImageList_AddIcon(himlState, hiconItem[cnt]);
	}
	SendDlgItemMessage(HOST_LIST, LVM_SETIMAGELIST, LVSIL_STATE, (LPARAM)himlState);
	if (SendDlgItemMessage(HOST_LIST, LVM_GETIMAGELIST, LVSIL_STATE, 0))
		lvStateEnable = TRUE;
	else {
		SendDlgItemMessage(HOST_LIST, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)himlState);
		lvStateEnable = FALSE;
	}
}

void TSendDlg::SetSize(void)//////////////
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);

	::GetWindowPlacement(GetDlgItem(HOST_LIST), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[host_item]);

	::GetWindowPlacement(GetDlgItem(MEMBERCNT_TEXT), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[member_item]);

	::GetWindowPlacement(GetDlgItem(REFRESH_BUTTON), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[refresh_item]);

	::GetWindowPlacement(GetDlgItem(SEND_EDIT), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[edit_item]);

	::GetWindowPlacement(GetDlgItem(IDOK), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[ok_item]);

	::GetWindowPlacement(GetDlgItem(IDC_OK), &wp);/////////////
	RectToWinPos(&wp.rcNormalPosition, &item[Ok]);///////////

	::GetWindowPlacement(GetDlgItem(IDC_ChatRecord), &wp);/////////////
	RectToWinPos(&wp.rcNormalPosition, &item[ChatRecord]);///////////

	::GetWindowPlacement(GetDlgItem(FILE_BUTTON), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[file_item]);

	::GetWindowPlacement(GetDlgItem(SECRET_CHECK), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[secret_item]);

	::GetWindowPlacement(GetDlgItem(PASSWORD_CHECK), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[passwd_item]);

	::GetWindowPlacement(GetDlgItem(SEPARATE_STATIC), &wp);
	RectToWinPos(&wp.rcNormalPosition, &item[separate_item]);

	GetWindowRect(&rect);
	orgRect = rect;

	int	cx = ::GetSystemMetrics(SM_CXFULLSCREEN), cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
	int	xsize = rect.right - rect.left + cfg->SendXdiff, ysize = rect.bottom - rect.top + cfg->SendYdiff;
	int	x = cfg->SendXpos, y = cfg->SendYpos;

	if (cfg->SendSavePos == 0)
	{
		x = (cx - xsize)/2 + (rand() % (cx/4)) - cx/8;
		y = (cy - ysize)/2 + (rand() % (cy/4)) - cy/8;
	}
	if (x + xsize > cx)
		x = cx - xsize;
	if (y + ysize > cy)
		y = cy - ysize;

	EvSize(SIZE_RESTORED, 0, 0);
	MoveWindow((x < 0) ? 0 : x, (y < 0) ? 0 : y, xsize, ysize, TRUE);
}

void TSendDlg::PopupContextMenu(POINTS pos)
{
	HMENU	hMenu = ::CreatePopupMenu();
	HMENU	hPriorityMenu = ::CreateMenu();
	HMENU	hGroupMenu = ::CreateMenu();
	int		cnt;
	char	buf[MAX_BUF];
	int		selectNum = (int)SendDlgItemMessage(HOST_LIST, LVM_GETSELECTEDCOUNT, 0, 0);
	//	char	*appendStr = selectNum > 0 ? MOVETO_MSGSTR : SELECT_MSGSTR;
	char	*appendStr = selectNum > 0 ? MOVETO_MSGSTR : MOVETO_MSGSTR;
	u_int	flag = selectNum <= 0 ? MF_GRAYED : 0;

	if (hMenu == NULL || hPriorityMenu == NULL || hGroupMenu == NULL)
		return;

	// priority menu
	for (cnt=cfg->PriorityMax; cnt >= 0; cnt--)
	{
		char	*ptr = buf;
#ifndef JAPANESE
		ptr += wsprintf(ptr, "%s ", appendStr);
#endif
		if (cnt == 0)
			ptr += wsprintf(ptr, NODISP_MSGSTR);
		else if (cnt == 1)
			ptr += wsprintf(ptr, DEFAULTDISP_MSGSTR);
		else
			ptr += wsprintf(ptr, DISPPRIORITY_MSGSTR, cfg->PriorityMax - cnt + 1);
#ifdef JAPANESE
		ptr += wsprintf(ptr, " %s ", appendStr);
#endif
		wsprintf(ptr, cnt == 1 ? MEMBERCOUNTDEF_MSGSTR : MEMBERCOUNT_MSGSTR, hosts->PriorityHostCnt(cnt * PRIORITY_OFFSET, PRIORITY_OFFSET), cfg->priorityHosts.PriorityHostCnt(cnt * PRIORITY_OFFSET, PRIORITY_OFFSET));
		::AppendMenu(hPriorityMenu, MF_STRING|flag, MENU_PRIORITY_START + cnt * PRIORITY_OFFSET, buf);
	}

	::AppendMenu(hPriorityMenu, MF_SEPARATOR, 0, 0);
	::AppendMenu(hPriorityMenu, MF_STRING|(hiddenDisp ? MF_CHECKED : 0), MENU_PRIORITY_HIDDEN, TMPNODISPDISP_MSGSTR);
	::AppendMenu(hPriorityMenu, MF_STRING, MENU_PRIORITY_RESET, RESETPRIORITY_MSGSTR);
	::AppendMenu(hMenu, MF_POPUP, (UINT)hPriorityMenu, SORTFILTER_MSGSTR);
	//	::AppendMenu(hMenu, MF_SEPARATOR, 0, 0);

	// group select
	int	rowMax = ::GetSystemMetrics(SM_CYSCREEN) / ::GetSystemMetrics(SM_CYMENU) -1;

	for (cnt=0; cnt < memberCnt; cnt++)
	{
		int		menuMax = ::GetMenuItemCount(hGroupMenu), cnt2;

		for (cnt2=0; cnt2 < menuMax; cnt2++)
		{
			::GetMenuString(hGroupMenu, cnt2, buf, sizeof(buf), MF_BYPOSITION);
			if (strcmp(buf, hostArray[cnt]->groupName) == 0)
				break;
		}
		if (cnt2 == menuMax && *hostArray[cnt]->groupName)
			::AppendMenu(hGroupMenu, MF_STRING|((menuMax % rowMax || menuMax == 0) ? 0 : MF_MENUBREAK), MENU_GROUP_START + menuMax, hostArray[cnt]->groupName);
	}
	::AppendMenu(hMenu, MF_POPUP|(::GetMenuItemCount(hGroupMenu) ? 0 : MF_GRAYED), (UINT)hGroupMenu, GROUPSELECT_MSGSTR);
	//	::AppendMenu(hMenu, MF_SEPARATOR, 0, 0);

	SetMainMenu(hMenu);
	::TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pos.x, pos.y, 0, hWnd, NULL);
	::DestroyMenu(hMenu);
}

void TSendDlg::SetMainMenu(HMENU hMenu)
{
	::AppendMenu(hMenu, MF_STRING, MENU_FINDDLG, FINDDLG_MSGSTR);
	::AppendMenu(hMenu, MF_STRING, MENU_FILEADD, FILEATTACHMENU_MSGSTR);
	::AppendMenu(hMenu, MF_STRING, MENU_FOLDERADD, FOLDERATTACHMENU_MSGSTR);
	::AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	::AppendMenu(hMenu, MF_STRING, MENU_SAVECOLUMN, SAVECOLUMN_MSGSTR);

	::AppendMenu(hMenu, MF_POPUP, (UINT)::LoadMenu(TApp::hI, (LPCSTR)SENDFONT_MENU), FONTSET_MSGSTR);
	::AppendMenu(hMenu, MF_POPUP, (UINT)::LoadMenu(TApp::hI, (LPCSTR)SIZE_MENU), SIZESET_MSGSTR);
	::AppendMenu(hMenu, MF_STRING, MENU_SAVEPOS, SAVEPOS_MSGSTR);
	::AppendMenu(hMenu, MF_STRING, MENU_MEMBERDISP, MEMBERDISP_MSGSTR);
}

BOOL SetFileButton(TDlg *dlg, int buttonID, ShareInfo *info)
{
	char	buf[MAX_BUF] = "", fname[MAX_PATH];
	int		offset = 0;
	for (int cnt=0; cnt < info->fileCnt; cnt++)
	{
		if (dlg->ResId() == SEND_DIALOG)
			ForcePathToFname(info->fileInfo[cnt]->Fname(), fname);
		else
			strncpyz(fname, info->fileInfo[cnt]->Fname(), MAX_PATH);
		offset += wsprintf(buf + offset, "%s ", fname);
		if (offset + MAX_PATH >= sizeof(buf))
			break;
	}
	dlg->SetDlgItemText(buttonID, buf);
	::ShowWindow(dlg->GetDlgItem(buttonID), info->fileCnt ? SW_SHOW : SW_HIDE);
	::EnableWindow(dlg->GetDlgItem(buttonID), info->fileCnt ? TRUE : FALSE);
	return	TRUE;
}

