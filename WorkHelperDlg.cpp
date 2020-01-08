
// WorkHelperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorkHelper.h"
#include "WorkHelperDlg.h"
#include "afxdialogex.h"
#include "ListenKey.h"
#include "OpnFileDlg.h"
#include "ControlKey.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWorkHelperDlg dialog
//HWND CWorkHelperDlg::m_hWnd = NULL;
//HCURSOR CWorkHelperDlg::m_hArrow = NULL;
//HHOOK CWorkHelperDlg::hHook = NULL;
HWND hStatic = NULL;
HCURSOR hCur[3] = { 0 };
HWND hEdit = NULL;
HHOOK hHook = NULL;
HWND hMain = NULL;
HDC CWorkHelperDlg::hDC = NULL;
HWND CWorkHelperDlg::hTarget = NULL;

CWorkHelperDlg::CWorkHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WORKHELPER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWorkHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWorkHelperDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CWorkHelperDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWorkHelperDlg::OnBnClickedCancel)

	ON_BN_CLICKED(IDC_START, &CWorkHelperDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_FINISH, &CWorkHelperDlg::OnBnClickedFinish)
	ON_MESSAGE(WM_ENDHOOK, &CWorkHelperDlg::OnEndHook)
	ON_MESSAGE(WM_CTLKEY, &CWorkHelperDlg::OnControlKey)

	ON_BN_CLICKED(IDC_START1, &CWorkHelperDlg::OnBnClickedStart1)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_FINISH1, &CWorkHelperDlg::OnBnClickedFinish1)
END_MESSAGE_MAP()


// CWorkHelperDlg message handlers

BOOL CWorkHelperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// 设置鼠标全局监听
	hStatic = this->GetDlgItem(IDC_STATIC)->GetSafeHwnd();
	hMain = this->GetSafeHwnd();
	hDC = ::GetDC(hStatic);

	if (!hDC || !m_hWnd) {
		MessageBox(L"初始化程序失败", L"ERROR", MB_OK | MB_ICONERROR);
		exit(-1);
	}
	
	hEdit = GetDlgItem(IDC_EDIT)->GetSafeHwnd();
//	m_hArrow = LoadCursorFromFile(L"C:\\Windows\\Cursor\\aero_arrow_l.cur");
//	SaveFile.open("Journal.txt", ios_base::out);
//	if (!SaveFile.is_open()) {
//		MessageBox(L"文件打开失败!", L"ERROR", MB_OK|MB_ICONERROR);
//		exit(-1);
//	}

	FindMsgFile();
	return FALSE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWorkHelperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWorkHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWorkHelperDlg::EndDialog(int nResult) {
	::ReleaseDC(hStatic, hDC);
}

/****************************************************************
WH_KEYBOARD hook procedure
鍵盤钩子处理过程
****************************************************************/
LRESULT CALLBACK CWorkHelperDlg::KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char text[30]{ "无法识别按键" }, interval[30]{ 0 };
	CListenKey &listen = CListenKey::getInstance();

	//增加一个过滤按键功能
	if (HC_ACTION == nCode) {

		KBDLLHOOKSTRUCT *  keyNum = (KBDLLHOOKSTRUCT *)lParam;
		
		if (!listen.CheckTime(keyNum->time)) {
			MessageBoxA(theApp.GetMainWnd()->GetSafeHwnd(), "记录超时，此次失效!", "ERROR", MB_OK | MB_ICONERROR);
//			SaveFile.clear();
			::SendMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_ENDHOOK, 0, 0);
			goto end;
		}

		if (listen.IsBufFull()) if (!listen.SaveMsg2File()) {
			MessageBoxA(theApp.GetMainWnd()->GetSafeHwnd(), "保存文件失败", "ERROR", MB_OK | MB_ICONERROR);
			::SendMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_ENDHOOK, 0, 0);
			goto end;
		}

		if ((wParam == WM_KEYDOWN)) {		//有键按下
			DWORD Code = keyNum->vkCode;
			listen.KeyStatInfo(keyNum->vkCode, text, false);
			sprintf_s(interval, "消息间隔时间 : %d ms", listen.GetIntervalTime(keyNum->time));
			listen.PushMsgBuf(keyNum->time, WM_KEYDOWN, Code, lParam);
			if ((keyNum->vkCode >= 'A' && keyNum->vkCode <= 'Z') ||
					(keyNum->vkCode >= '0' && keyNum->vkCode <= '9')
				) {
				if (!IsHCase()) Code = tolower(keyNum->vkCode);
				listen.PushMsgBuf(keyNum->time + 20, WM_CHAR, Code, lParam);
			}
			listen.TextOutStatic(NULL, text, interval);
		}
		else if (wParam == WM_KEYUP) {		//有键松开
			listen.KeyStatInfo(keyNum->vkCode, text, true);
			listen.PushMsgBuf(keyNum->time, WM_KEYUP, keyNum->vkCode, lParam);
			listen.TextOutStatic(NULL, text);
		}
//		else if (wParam == WM_CHAR) {
//			MessageBoxA(NULL, "CNMB", "SB", MB_OK);
//		}
		
		listen.UpdateTime(keyNum->time);
	}
end:
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

/*
	POINT   pt = p->pt;
	DWORD   mouseData = p->mouseData;
	const char *info = NULL;
	WCHAR text[60], pData[50], mData[50];

	if (nCode >= 0)
	{
		if (wParam == WM_MOUSEMOVE)       info = "鼠标移动　　　　";
		else if (wParam == WM_LBUTTONDOWN)   info = "鼠标【左键】按下";
		else if (wParam == WM_LBUTTONUP)     info = "鼠标【左键】抬起";
		else if (wParam == WM_LBUTTONDBLCLK) info = "鼠标【左键】双击";
		else if (wParam == WM_RBUTTONDOWN)   info = "鼠标【右键】按下";
		else if (wParam == WM_RBUTTONUP)     info = "鼠标【右键】抬起";
		else if (wParam == WM_RBUTTONDBLCLK) info = "鼠标【右键】双击";
		else if (wParam == WM_MBUTTONDOWN)   info = "鼠标【滚轮】按下";
		else if (wParam == WM_MBUTTONUP)     info = "鼠标【滚轮】抬起";
		else if (wParam == WM_MBUTTONDBLCLK) info = "鼠标【滚轮】双击";
		else if (wParam == WM_MOUSEWHEEL)    info = "鼠标【滚轮】滚动";

		ZeroMemory(text, sizeof(text));
		ZeroMemory(pData, sizeof(pData));
		ZeroMemory(mData, sizeof(mData));

		wsprintf(text, L"当前状态： %10s   ", info);
		wsprintf(pData, L"0x%x - X: [%04d], Y: [%04d]  ", wParam, pt.x, pt.y);
		wsprintf(mData, L"附带数据： %16u   ", mouseData);

//		hdc = GetDC(hgWnd);
		TextOut(hDC, 10, 10, text, lstrlenW(text));
		TextOut(hDC, 10, 30, pData, lstrlenW(pData));
		TextOut(hDC, 10, 50, mData, lstrlenW(mData));
//		ReleaseDC(hgWnd, hdc);
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
*/


/*			
//处理字母大小写
if ((keyNum->vkCode == VK_CAPITAL) || (keyNum->vkCode == VK_LSHIFT) || (keyNum->vkCode == VK_RETURN)
	|| (keyNum->vkCode >= 65 && keyNum->vkCode <= 90)) {

	if (!GetKeyState(VK_CAPITAL)) {	//如果大写锁定键未被按下
		g_bCapsLock = FALSE;
	}
	else {
		g_bCapsLock = TRUE;
	}

	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) { //如果shift键被按住
		g_bShift = TRUE;
	}
	else {
		g_bShift = FALSE;
	}

	if (keyNum->vkCode >= 65 && keyNum->vkCode <= 90) {
		BOOL flag = g_bCapsLock^g_bShift;//同假异真
		if (flag) {
			c = keyNum->vkCode;
		}
		else {
			c = keyNum->vkCode + 32;
		}
		//					SaveFile << (int)c[0] << " : " << c;// << endl;
	}
	sprintf_s(text, "当前 %c 键按下", keyNum->vkCode);
}
//处理数字小键盘
else if (keyNum->vkCode == VK_NUMLOCK || (keyNum->vkCode >= VK_NUMPAD0 && keyNum->vkCode <= VK_NUMPAD9)) { //144表示数字小键盘锁键
	if (GetKeyState(VK_NUMLOCK)) {		//如果数字小键盘锁键被按下
		c = c;
		sprintf_s(text, "当前 %c 键按下", 48 + keyNum->vkCode - 96);
		int mapKey = keyNum->vkCode - 48;
		SaveFile << keyNum->vkCode << " : " << char(mapKey);// << endl;
	}
	else {
	}
}

SaveFile << '\t' << time << endl;
}
*/

void CWorkHelperDlg::FindMsgFile()
{
	WIN32_FIND_DATA findData;

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO);

	if (pCombo) {
		CString strTemp = L".\\*.*";
//		strTemp.Format(_T(".\\*.*"), strPath);//查找指定目录下的直接的所有文件和目录

		HANDLE hFile = FindFirstFile(strTemp, &findData);
		while (hFile != INVALID_HANDLE_VALUE)
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//如果是目录
			{
				if (findData.cFileName[0] != _T('.'))//排除.和..文件夹
					pCombo->AddString(findData.cFileName);
			}

			if (!FindNextFile(hFile, &findData)) break;

		}
	}
}

BOOL CWorkHelperDlg::SetSelectMouse()
{
	hCur[0] = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	hCur[1] = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
	hCur[2] = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));

	BOOL iRet = SetSystemCursor(CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS))), OCR_NORMAL);
	iRet &= SetSystemCursor(CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS))), OCR_HAND);
	iRet &= SetSystemCursor(CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS))), OCR_IBEAM);
	if (!iRet) {
		MessageBox(L"控键失败!", L"错误", MB_OK | MB_ICONERROR);
		UnSetSelectMouse();
		return false;
	}
	return true;
}

BOOL CWorkHelperDlg::UnSetSelectMouse()
{
	//如果还原光标失败 请到控制面板的鼠标选项里还原
	BOOL iRet = SetSystemCursor(hCur[0], OCR_NORMAL);
	iRet &= SetSystemCursor(hCur[1], OCR_HAND);
	iRet &= SetSystemCursor(hCur[2], OCR_IBEAM);
	if (!iRet) {
		MessageBoxA(NULL, "光标还原失败！", "错误", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}


void CWorkHelperDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//	CDialogEx::OnOK();
}

void CWorkHelperDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CListenKey::getInstance().ExitHook();
	CDialogEx::OnCancel();
}


void CWorkHelperDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	OpnFileDlg dlg(this->GetSafeHwnd());

//	INT_PTR nResponse = dlg.DoModal();

	if (IDOK == dlg.DoModal()) {

		hHook = SetWindowsHookEx(
			WH_KEYBOARD_LL,    // 监听类型【键盘】
			KeyBoardProc,  // 处理函数
			theApp.m_hInstance,      // 当前实例句柄
			0               // 监听窗口句柄(NULL为全局监听)
		);
	}
	else return;
//	KDialog dlg(theApp.m_hInstance, this->GetSafeHwnd());
//	CMyDlg dlg;
//	CString result;
//	dlg.CreateModeDlg(_T("123"), CRect(0, 0, 100, 100), TRUE, this);
//	MessageBox(dlg.GetFileName(), L"CNMB", MB_OK);
//	if (nResponse == IDOK) MessageBox(L"ASDA", L"rewrew", MB_OK);
//	else if (nResponse == IDCANCEL) MessageBox(L"BDSFS", L"rewrew", MB_OK);
//	else if (nResponse == -1) MessageBox(L"SHFD", L"rewrew", MB_OK);

	if (hHook == NULL)
	{
		WCHAR text[30]{ 0 };
		wsprintf(text, L"键盘监听失败！错误码 : %d", GetLastError());
		MessageBoxW(text, TEXT("ERROE"), MB_OK | MB_ICONERROR);
		return;
	}

	CListenKey::getInstance().TextOutStatic("正在监听键盘消息...");
	this->SetFocus();
}


void CWorkHelperDlg::OnBnClickedFinish()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!::UnhookWindowsHookEx(hHook)) return;
	if (!CListenKey::getInstance().SaveMsg2File()) MessageBox(L"保存记录文件失败", L"ERROR", MB_OK | MB_ICONERROR);

	CListenKey::getInstance().ExitHook();

	CListenKey::getInstance().TextOutStatic("当前未在监听状态", " ", " ");
	this->SetFocus();
}

//失败消息引发的退出程序
LRESULT CWorkHelperDlg::OnEndHook(WPARAM wParam, LPARAM lParam) {
	CListenKey::getInstance().ExitHook();
	::UnhookWindowsHookEx(hHook);
	CListenKey::getInstance().TextOutStatic(" ", " ", " ");
	return LRESULT(NULL);
}

LRESULT CWorkHelperDlg::OnControlKey(WPARAM wParam, LPARAM lParam)
{
	CString msgfile;
	CComboBox *pComb = (CComboBox *)GetDlgItem(IDC_COMBO);

	UnSetSelectMouse();
	pComb->GetWindowTextW(msgfile);
	MessageBox(msgfile, L"OnControlKey", MB_OK);

	CControlKey *pck = CControlKey::getInstance();

	if (!pck->InitFile(L"./Journal.txt")) {
		MessageBox(L"CNMB", L"SB", MB_OK);
		return LRESULT(NULL);
	}

	::SetForegroundWindow(hTarget);
//	::SendMessage(hTarget, WM_KEYDOWN, 'A', NULL);
//	::SendMessage(hTarget, WM_CHAR, 'a', NULL);
//	::SendMessage(hTarget, WM_KEYUP, 'A', NULL);
	//多次控键怎么办
//	if(m_pthread1 == NULL)
//		m_pthread1 = AfxBeginThread(pck->LoadMsgBuffer, this, THREAD_PRIORITY_NORMAL, 0, 0);

//	if (m_pthread2 == NULL)
		m_pthread2 = AfxBeginThread(pck->varControlKey, this, THREAD_PRIORITY_NORMAL, 0, 0);
/*
	SuspendThread(m_pthread2->m_hThread); //挂起第二个线程。“暂停”
	ResumeThread(m_pthread2->m_hThread); //释放第二个线程。”播放“
*/
	return LRESULT(NULL);
}

void fun(void *) {
	while (true) {
		if (GetKeyState(VK_LBUTTON) < 0) {
			POINT point{ 0 };
			WCHAR arr[60]{ 0 }, title[30]{ 0 };
			GetCursorPos(&point);
			HWND target = WindowFromPoint(point);
			GetWindowText(target, title, 29);
			wsprintfW(arr, L"标题: %s\t句柄: 0x%X", title, target);
			SetWindowText(hEdit, arr);
			CWorkHelperDlg::hTarget = target;
			Sleep(200);
			::SendMessage(hMain, WM_CTLKEY, NULL, NULL);
			break;
		}
	}
}

void CWorkHelperDlg::OnBnClickedStart1()
{
	// TODO: 在此添加控件通知处理程序代码
//	HCURSOR hCur = CopyCursor((HCURSOR)LoadImage(NULL, TEXT("C:\\Windows\\Cursors\\aero_ew_l.cur"), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE));
//	SetCursor(hCur);
//	HCURSOR hCur = GetCursor();
	SetSelectMouse();

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)fun, 0, 0, 0);

	//	::SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)hCur);
}


BOOL CWorkHelperDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
//  TODO: 在此添加消息处理程序代码和/或调用默认值
//	::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HELP)));
//	return true;
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CWorkHelperDlg::OnBnClickedFinish1()
{
	// TODO: 在此添加控件通知处理程序代码
//	std::thread t(fun);
//	t.join();
//	t.join();
}
