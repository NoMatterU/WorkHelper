
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

HWND hStatic = NULL;
HCURSOR hCur[3] = { 0 };
HWND hEdit = NULL;
HHOOK hHook = NULL;
HWND hFstChild = NULL;
HWND CWorkHelperDlg::hMain = NULL;
HDC CWorkHelperDlg::hDC = NULL;
HWND CWorkHelperDlg::hTarget = NULL;
BOOL iFstFind = true;

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

	ON_BN_CLICKED(IDC_START, &CWorkHelperDlg::OnLisKeyStart)
	ON_BN_CLICKED(IDC_FINISH, &CWorkHelperDlg::OnLisKeyFinish)
	ON_MESSAGE(WM_ENDHOOK, &CWorkHelperDlg::OnShutHook)
	ON_MESSAGE(WM_CTLEND, &CWorkHelperDlg::OnCtlKeyEnd)

	ON_BN_CLICKED(IDC_START1, &CWorkHelperDlg::OnCtlKeyStart)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_FINISH1, &CWorkHelperDlg::OnCtlKeyFinish)

	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_SUSPEND, &CWorkHelperDlg::OnLisKeySuspend)
	ON_BN_CLICKED(IDC_SUSPEND1, &CWorkHelperDlg::OnCtlKeySuspend)
	ON_COMMAND(ID_SCAN, &CWorkHelperDlg::OnScanFile)
	ON_COMMAND(ID_POINT, &CWorkHelperDlg::OnPointToHwnd)
	ON_COMMAND(ID_HELP, &CWorkHelperDlg::OnHelp)
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
	// �������ȫ�ּ���
	hStatic = this->GetDlgItem(IDC_STATIC)->GetSafeHwnd();
	hMain = this->GetSafeHwnd();
	hDC = ::GetDC(hStatic);

	if (!(hDC && m_hWnd)) {
		MessageBox(L"��ʼ������ʧ��", L"ERROR", MB_OK | MB_ICONERROR);
		exit(-1);
	}
	
	hEdit = GetDlgItem(IDC_EDIT)->GetSafeHwnd();
//	m_hArrow = LoadCursorFromFile(L"C:\\Windows\\Cursor\\aero_arrow_l.cur");
//	SaveFile.open("Journal.txt", ios_base::out);
//	if (!SaveFile.is_open()) {
//		MessageBox(L"�ļ���ʧ��!", L"ERROR", MB_OK|MB_ICONERROR);
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
�I�P���Ӵ������
****************************************************************/
LRESULT CALLBACK CWorkHelperDlg::KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char text[30]{ "�޷�ʶ�𰴼�" }, interval[30]{ 0 };
	CListenKey &listen = CListenKey::getInstance();

	//����һ�����˰������� Done
	if (HC_ACTION == nCode) {

		KBDLLHOOKSTRUCT *  keyNum = (KBDLLHOOKSTRUCT *)lParam;

		if (!listen.CheckTime(keyNum->time)) {
			MessageBoxA(theApp.GetMainWnd()->GetSafeHwnd(), "��¼��ʱ���˴�ʧЧ!", "ERROR", MB_OK | MB_ICONERROR);
//			SaveFile.clear();
			::SendMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_ENDHOOK, 0, 0);
			goto end;
		}

		if (listen.IsBufFull()) if (!listen.SaveMsg2File()) {
			MessageBoxA(theApp.GetMainWnd()->GetSafeHwnd(), "�����ļ�ʧ��", "ERROR", MB_OK | MB_ICONERROR);
			::SendMessage(theApp.GetMainWnd()->GetSafeHwnd(), WM_ENDHOOK, 0, 0);
			goto end;
		}

		if ((wParam == WM_KEYDOWN)) {		//�м�����
			DWORD Code = keyNum->vkCode;
			//������������˼���Ϣ
			if (listen.KeyStatInfo(keyNum->vkCode, text, false)) {
				sprintf_s(interval, "��Ϣ���ʱ�� : %d ms", listen.GetIntervalTime(keyNum->time));
				listen.PushMsgBuf(keyNum->time, WM_KEYDOWN, Code, lParam);
				//�����WM_CHAR��Ϣ
				if ((keyNum->vkCode >= 'A' && keyNum->vkCode <= 'Z') ||
						(keyNum->vkCode >= '0' && keyNum->vkCode <= '9') ||
							keyNum->vkCode == VK_SPACE
					) {
					if((keyNum->vkCode >= 'A' && keyNum->vkCode <= 'Z')) 
						if (!IsHCase()) Code = tolower(keyNum->vkCode);
					listen.PushMsgBuf(keyNum->time + 20, WM_CHAR, Code, lParam);
				}
			}
			listen.TextOutStatic(NULL, text, interval);
		}
		else if (wParam == WM_KEYUP) {		//�м��ɿ�
			if (listen.KeyStatInfo(keyNum->vkCode, text, true)) {
				listen.PushMsgBuf(keyNum->time, WM_KEYUP, keyNum->vkCode, lParam);
				listen.TextOutStatic(NULL, text);
			}
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
		if (wParam == WM_MOUSEMOVE)       info = "����ƶ���������";
		else if (wParam == WM_LBUTTONDOWN)   info = "��꡾���������";
		else if (wParam == WM_LBUTTONUP)     info = "��꡾�����̧��";
		else if (wParam == WM_LBUTTONDBLCLK) info = "��꡾�����˫��";
		else if (wParam == WM_RBUTTONDOWN)   info = "��꡾�Ҽ�������";
		else if (wParam == WM_RBUTTONUP)     info = "��꡾�Ҽ���̧��";
		else if (wParam == WM_RBUTTONDBLCLK) info = "��꡾�Ҽ���˫��";
		else if (wParam == WM_MBUTTONDOWN)   info = "��꡾���֡�����";
		else if (wParam == WM_MBUTTONUP)     info = "��꡾���֡�̧��";
		else if (wParam == WM_MBUTTONDBLCLK) info = "��꡾���֡�˫��";
		else if (wParam == WM_MOUSEWHEEL)    info = "��꡾���֡�����";

		ZeroMemory(text, sizeof(text));
		ZeroMemory(pData, sizeof(pData));
		ZeroMemory(mData, sizeof(mData));

		wsprintf(text, L"��ǰ״̬�� %10s   ", info);
		wsprintf(pData, L"0x%x - X: [%04d], Y: [%04d]  ", wParam, pt.x, pt.y);
		wsprintf(mData, L"�������ݣ� %16u   ", mouseData);

//		hdc = GetDC(hgWnd);
		TextOut(hDC, 10, 10, text, lstrlenW(text));
		TextOut(hDC, 10, 30, pData, lstrlenW(pData));
		TextOut(hDC, 10, 50, mData, lstrlenW(mData));
//		ReleaseDC(hgWnd, hdc);
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
*/


/*			
//������ĸ��Сд
if ((keyNum->vkCode == VK_CAPITAL) || (keyNum->vkCode == VK_LSHIFT) || (keyNum->vkCode == VK_RETURN)
	|| (keyNum->vkCode >= 65 && keyNum->vkCode <= 90)) {

	if (!GetKeyState(VK_CAPITAL)) {	//�����д������δ������
		g_bCapsLock = FALSE;
	}
	else {
		g_bCapsLock = TRUE;
	}

	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) { //���shift������ס
		g_bShift = TRUE;
	}
	else {
		g_bShift = FALSE;
	}

	if (keyNum->vkCode >= 65 && keyNum->vkCode <= 90) {
		BOOL flag = g_bCapsLock^g_bShift;//ͬ������
		if (flag) {
			c = keyNum->vkCode;
		}
		else {
			c = keyNum->vkCode + 32;
		}
		//					SaveFile << (int)c[0] << " : " << c;// << endl;
	}
	sprintf_s(text, "��ǰ %c ������", keyNum->vkCode);
}
//��������С����
else if (keyNum->vkCode == VK_NUMLOCK || (keyNum->vkCode >= VK_NUMPAD0 && keyNum->vkCode <= VK_NUMPAD9)) { //144��ʾ����С��������
	if (GetKeyState(VK_NUMLOCK)) {		//�������С��������������
		c = c;
		sprintf_s(text, "��ǰ %c ������", 48 + keyNum->vkCode - 96);
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
//		strTemp.Format(_T(".\\*.*"), strPath);//����ָ��Ŀ¼�µ�ֱ�ӵ������ļ���Ŀ¼

		HANDLE hFile = FindFirstFile(strTemp, &findData);
		while (hFile != INVALID_HANDLE_VALUE)
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))//�������Ŀ¼
			{
		//		if (findData.cFileName[0] != _T('.'))//�ų�.��..�ļ���
		//			pCombo->AddString(findData.cFileName);
				CString temp = findData.cFileName;
				temp = temp.Right(temp.GetLength() - temp.ReverseFind('.') - 1);
				if (!temp.CompareNoCase(L"msg"))
					pCombo->AddString(findData.cFileName);
			}

			if (!FindNextFile(hFile, &findData)) break;

		}
		FindClose(hFile);
	}
}


BOOL UnSetSelectMouse()
{
	//�����ԭ���ʧ�� �뵽�����������ѡ���ﻹԭ
	BOOL iRet = SetSystemCursor(hCur[0], OCR_NORMAL);
	iRet &= SetSystemCursor(hCur[1], OCR_HAND);
	iRet &= SetSystemCursor(hCur[2], OCR_IBEAM);
	if (!iRet) {
		::MessageBoxW(NULL, L"��껹ԭʧ�ܣ�", L"����", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

BOOL SetSelectMouse()
{
	hCur[0] = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	hCur[1] = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
	hCur[2] = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));

	BOOL iRet = SetSystemCursor(CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS))), OCR_NORMAL);
	iRet &= SetSystemCursor(CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS))), OCR_HAND);
	iRet &= SetSystemCursor(CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS))), OCR_IBEAM);
	if (!iRet) {
		::MessageBoxW(NULL, L"�ؼ�ʧ��!", L"����", MB_OK | MB_ICONERROR);
		UnSetSelectMouse();
		return false;
	}
	return true;
}

void CWorkHelperDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//	CDialogEx::OnOK();
}

void CWorkHelperDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListenKey::getInstance().ExitHook();
	CDialogEx::OnCancel();
}


void CWorkHelperDlg::OnLisKeyStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ((m_Stat == HelperStat::STAT_LISTENING) || (m_Stat == HelperStat::STAT_LISSUSPEND)) {
		MessageBox(L"��ǰ���ڼ���״̬��...", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if ((m_Stat == HelperStat::STAT_CONTROLING) || (m_Stat == HelperStat::STAT_CTLSUSPEND)) {
		MessageBox(L"��ǰ���ڿؼ�״̬��...", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}

	OpnFileDlg dlg(this->GetSafeHwnd());

//	INT_PTR nResponse = dlg.DoModal();

	if (IDOK == dlg.DoModal()) {

		hHook = SetWindowsHookEx(
			WH_KEYBOARD_LL,    // �������͡����̡�
			KeyBoardProc,  // ������
			theApp.m_hInstance,      // ��ǰʵ�����
			0               // �������ھ��(NULLΪȫ�ּ���)
		);
	}
	else return;

	if (hHook == NULL)
	{
		WCHAR text[30]{ 0 };
		wsprintf(text, L"���̼���ʧ�ܣ������� : %d", GetLastError());
		MessageBoxW(text, TEXT("ERROE"), MB_OK | MB_ICONERROR);
		m_Stat = HelperStat::STAT_SPACE;
		return;
	}

	m_Stat = HelperStat::STAT_LISTENING;

	this->SetFocus();
	CListenKey::getInstance().TextOutStatic("���ڼ���������Ϣ...", " ", " ");
}


void CWorkHelperDlg::OnLisKeyFinish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ((m_Stat == HelperStat::STAT_LISTENING) 
		|| (m_Stat == HelperStat::STAT_LISSUSPEND)) {

		if(m_Stat == HelperStat::STAT_LISTENING) 
			if(!::UnhookWindowsHookEx(hHook)) return;

		m_Stat = HelperStat::STAT_SPACE;
		if (!CListenKey::getInstance().SaveMsg2File()) MessageBox(L"�����¼�ļ�ʧ��", L"ERROR", MB_OK | MB_ICONERROR);

		CListenKey::getInstance().ExitHook();

		CListenKey::getInstance().TextOutStatic("��ǰ�ѽ�������״̬", " ", " ");
		this->SetFocus();
		return;
	}
#ifdef _DEBUG
		MessageBox(L"CNMB", L"SB", MB_OK);
#endif
}

//ʧ����Ϣ�������˳�����
LRESULT CWorkHelperDlg::OnShutHook(WPARAM wParam, LPARAM lParam) {
	CListenKey::getInstance().ExitHook();
	::UnhookWindowsHookEx(hHook);
	CListenKey::getInstance().TextOutStatic(" ", " ", " ");
	return LRESULT(NULL);
}

LRESULT CWorkHelperDlg::OnCtlKeyEnd(WPARAM wParam, LPARAM lParam)
{
	m_Stat = HelperStat::STAT_SPACE;
	CListenKey::getInstance().TextOutStatic("��ǰ�ѽ����ؼ�״̬");
	MessageBoxA(this->GetSafeHwnd(), "�ؼ�����!", "��ʾ", MB_OK | MB_ICONINFORMATION);
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
			wsprintfW(arr, L"����: %s\t���: 0x%X", title, target);
			SetWindowText(hEdit, arr);
			CWorkHelperDlg::hTarget = target;
			Sleep(200);
			UnSetSelectMouse();
//			::SendMessage(CWorkHelperDlg::hMain, WM_CTLKEY, NULL, NULL);
			break;
		}
	}
}

void CWorkHelperDlg::OnCtlKeyStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ((m_Stat == HelperStat::STAT_LISTENING) || (m_Stat == HelperStat::STAT_LISSUSPEND)) {
		MessageBox(L"��ǰ���ڼ���״̬��...", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if ((m_Stat == HelperStat::STAT_CONTROLING) || (m_Stat == HelperStat::STAT_CTLSUSPEND)) {
		MessageBox(L"��ǰ���ڿؼ�״̬��...", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CString msgfile;
	CComboBox *pComb = (CComboBox *)GetDlgItem(IDC_COMBO);

	pComb->GetWindowTextW(msgfile);
	if ((msgfile.IsEmpty()) || (hTarget == NULL)) {
		MessageBox(L"�ؼ�������Դ����!", L"SB", MB_OK);
		return;
	}
	m_Stat = HelperStat::STAT_CONTROLING;
//	MessageBox(msgfile, L"OnControlKey", MB_OK);

	CControlKey *pck = CControlKey::getInstance();

	if (!pck->InitFile(L"./Journal.txt")) {
		MessageBox(L"CNMB", L"SB", MB_OK);
		return;
	}

	CListenKey::getInstance().TextOutStatic("�ؼ�����Ŀ�괰����...", "", "");
	::SetForegroundWindow(hTarget);
	//��οؼ���ô��
	//	if(m_pthread1 == NULL)
	m_pthread1 = AfxBeginThread(pck->LoadMsgBuffer, this, THREAD_PRIORITY_NORMAL, 0, 0);

	//	if (m_pthread2 == NULL)
	m_pthread2 = AfxBeginThread(pck->varControlKey, this, THREAD_PRIORITY_NORMAL, 0, 0);
}


BOOL CWorkHelperDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
//  TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HELP)));
//	return true;
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CWorkHelperDlg::OnCtlKeyFinish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ((m_Stat == HelperStat::STAT_CONTROLING) || (m_Stat == HelperStat::STAT_CONTROLING)) {
		CControlKey::getInstance()->StopCtlKey();
		return;
	}

#ifdef _DEBUG
		MessageBox(L"CNMB", L"SB", MB_OK);
#endif
//���Բ������ÿ���״̬
//	m_Stat = HelperStat::STAT_SPACE;
}


void CWorkHelperDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	WCHAR arr[60]{ 0 }, title[30]{ 0 };
	if (hTarget) {
		if (nSBCode == SB_LINEUP) {
			HWND hTemp = ::GetParent(hTarget);
			if (hTemp) {
				hTarget = hTemp;
				::GetWindowText(hTarget, title, 29);
				wsprintfW(arr, L"����: %s\t���: 0x%X", title, hTarget);
				::SetWindowText(hEdit, arr);
				iFstFind = true;
//				MessageBox(L"You are Click Up", L"Attion", MB_OK);
			}
		}
		else if (nSBCode == SB_LINEDOWN) {
			if (iFstFind) {
				hFstChild = ::GetWindow(hTarget, GW_CHILD);
				if (hFstChild == NULL) return;
				hTarget = hFstChild;
				iFstFind = false;
			}
			else {
				HWND hTemp = ::GetNextWindow(hTarget, GW_HWNDNEXT);
				if (hTemp == NULL) {
					hTarget = hFstChild;
					MessageBox(L"�Ѿ������һ���Ӵ���!", L"��ʾ", MB_OK | MB_ICONINFORMATION);
					return;
				}
				else hTarget = hTemp;
			}
			::GetWindowText(hTarget, title, 29);
			wsprintfW(arr, L"����: %s\t���: 0x%X", title, hTarget);
			::SetWindowText(hEdit, arr);
//			MessageBox(L"You are Click Down", L"Attion", MB_OK);
		}
	}
//	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CWorkHelperDlg::OnScanFile()
{
	// TODO: �ڴ���������������
//	MessageBox(L"You are Scaning File", L"Attion", MB_OK);
	CComboBox *pComb = (CComboBox *)GetDlgItem(IDC_COMBO);
	int num = pComb->GetCount();
	for (int i = 0; i < num; i++) pComb->DeleteString(0);
	FindMsgFile();
}


void CWorkHelperDlg::OnPointToHwnd()
{
	// TODO: �ڴ���������������
//	MessageBox(L"You are Pointing Hwnd", L"Attion", MB_OK);

	SetSelectMouse();

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)fun, 0, 0, 0);
}


void CWorkHelperDlg::OnLisKeySuspend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Stat == HelperStat::STAT_LISTENING) {
		if (!::UnhookWindowsHookEx(hHook)) {
			MessageBoxW(TEXT("��ͣ����ʧ��!"), TEXT("ERROE"), MB_OK | MB_ICONERROR);
//			m_Stat = HelperStat::STAT_SPACE;
			return;
		}

		CListenKey::getInstance().ExitHook();
		CListenKey::getInstance().TextOutStatic("Ŀǰ������ͣ����״̬");
		m_Stat = HelperStat::STAT_LISSUSPEND;
	}
	else if (m_Stat == HelperStat::STAT_LISSUSPEND) {
		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardProc, theApp.m_hInstance, 0);
		if (hHook == NULL)
		{
			MessageBoxW(TEXT("���¿�ʼ����ʧ��!"), TEXT("ERROE"), MB_OK | MB_ICONERROR);
//			m_Stat = HelperStat::STAT_SPACE;
			return;
		}

		CListenKey::getInstance().TextOutStatic("���ڼ���������Ϣ...");
		m_Stat = HelperStat::STAT_LISTENING;
	}
}


void CWorkHelperDlg::OnCtlKeySuspend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_Stat == HelperStat::STAT_CONTROLING) {
		SuspendThread(m_pthread2->m_hThread); //�����̡߳�����ͣ��
		SuspendThread(m_pthread1->m_hThread); //�����̡߳�����ͣ��

		CListenKey::getInstance().TextOutStatic("Ŀǰ������ͣ�ؼ�״̬");
		m_Stat = HelperStat::STAT_CTLSUSPEND;
	}
	else if (m_Stat == HelperStat::STAT_CTLSUSPEND) {
		ResumeThread(m_pthread2->m_hThread); //�ͷ��̡߳������š�
		ResumeThread(m_pthread1->m_hThread); //�����̡߳�����ͣ��

		CListenKey::getInstance().TextOutStatic("���ڿؼ�Ŀ�괰����...");
		m_Stat = HelperStat::STAT_CONTROLING;
	}

}


void CWorkHelperDlg::OnHelp()
{
	// TODO: �ڴ���������������
	WinExec("notepad.exe ./README.md", SW_NORMAL);
}
