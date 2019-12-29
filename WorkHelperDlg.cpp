
// WorkHelperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorkHelper.h"
#include "WorkHelperDlg.h"
#include "afxdialogex.h"
#include "ListenKey.h"
#include "OpnFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWorkHelperDlg dialog
//HWND CWorkHelperDlg::m_hWnd = NULL;
HDC CWorkHelperDlg::hDC = NULL;
HHOOK CWorkHelperDlg::hHook = NULL;

//bool g_bCapsLock{false}, g_bShift{false};
/*
bool iFirst{ true };
MyMSG msgBuf[MAX_BUF_SIZE]{ 0 };
int msgIndex{ 0 };
DWORD startTime{ 0 }, lastTime{ 0 };
*/
//ofstream SaveFile("");
/*
//add ���־�����ʾ
void TextOutStatic(const char *text1, const char *text2 = NULL, const char *text3 = NULL) {
	char line1[31]{ 0 }, line2[31]{ 0 }, line3[31]{ 0 };
//	memset(line1, 0x20, 30);
//	memset(line2, 0x20, 30);
//	memset(line3, 0x20, 30);
	if (text1 != NULL) {
		strcpy_s(line1, text1);
		TextOutA(CWorkHelperDlg::hDC, 60, 25, line1, 30);
	}
	if (text2 != NULL) {
		strcpy_s(line2, text2);
		TextOutA(CWorkHelperDlg::hDC, 60, 55, line2, 30);
	}
	if (text3 != NULL) {
		strcpy_s(line3, text3);
		TextOutA(CWorkHelperDlg::hDC, 60, 85, line3, 30);
	}
}

void ExitHook() {
	memset(msgBuf, 0, sizeof(MyMSG) * MAX_BUF_SIZE);
	iFirst = true;
	msgIndex = 0;
	startTime = 0;
	lastTime = 0;
	UnhookWindowsHookEx(CWorkHelperDlg::hHook);
}

bool CheckTime(DWORD curTime) {
	DWORD interTime = curTime - lastTime, totalTime = curTime - startTime;
	if (interTime < 65536 && interTime >= 0)
		if (totalTime < 0xFF000000 && totalTime >= 0) return true;
	return false;
}

bool IsBufFull() {
	if (msgIndex >= MAX_BUF_SIZE) return true;
	return false;
}

bool SaveMsg2File() {
	if (SaveFile.is_open()) 
		if(msgIndex <= MAX_BUF_SIZE && msgIndex > 0)
			if(SaveFile.write((char *)msgBuf, sizeof(MyMSG)*msgIndex))
				return true;
	return false;
}

void PushMsgBuf(USHORT Interval, UINT message, WPARAM wParam, LPARAM lParam) {
	MyMSG msg{ Interval, message, wParam, lParam };
	if (msgIndex < MAX_BUF_SIZE) {
		msgBuf[msgIndex] = msg;
		msgIndex++;
	}
}

void KeyStatInfo(DWORD vkCode, char *outstr, bool iKeyUp) {
	if (vkCode >= 'A' && vkCode <= 'Z') {
		if(iKeyUp) sprintf_s(outstr, MAX_TEXT_SIZE, "   ��ǰ %c ������   ", vkCode);
		else sprintf_s(outstr, MAX_TEXT_SIZE, "   ��ǰ %c ������   ", vkCode);
	}
	else if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9) {
		if(iKeyUp) sprintf_s(outstr, MAX_TEXT_SIZE, "   ��ǰ %c ������   ", 48 + vkCode - 96);
		else sprintf_s(outstr, MAX_TEXT_SIZE, "   ��ǰ %c ������   ", 48 + vkCode - 96);
	}
	else if (vkCode == VK_LEFT) { 
		if(iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "������� ������");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "������� ������");
	}
	else if (vkCode == VK_UP) {
		if(iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "������� ������");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "������� ������");
	}
	else if (vkCode == VK_RIGHT) {
		if(iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "������� ������");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "������� ������");
	}
	else if(vkCode == VK_DOWN) {
		if(iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "������� ������");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "������� ������");
	}
}
*/
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
	m_hWnd = this->GetDlgItem(IDC_STATIC)->GetSafeHwnd();
	hDC = ::GetDC(m_hWnd);

	if (!hDC || !m_hWnd) {
		MessageBox(L"��ʼ������ʧ��", L"ERROR", MB_OK | MB_ICONERROR);
		exit(-1);
	}
	
//	SaveFile.open("Journal.txt", ios_base::out);
//	if (!SaveFile.is_open()) {
//		MessageBox(L"�ļ���ʧ��!", L"ERROR", MB_OK|MB_ICONERROR);
//		exit(-1);
//	}

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
	::ReleaseDC(m_hWnd, hDC);
//	SaveFile.close();
}

/****************************************************************
WH_KEYBOARD hook procedure
�I�P���Ӵ������
****************************************************************/
LRESULT CALLBACK CWorkHelperDlg::KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	char text[30]{ "�޷�ʶ�𰴼�" }, interval[30]{ 0 };
	CListenKey &listen = CListenKey::getInstance();

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
			listen.KeyStatInfo(keyNum->vkCode, text, false);
			sprintf_s(interval, "��Ϣ���ʱ�� : %d ms", listen.GetIntervalTime(keyNum->time));
			listen.PushMsgBuf(keyNum->time, WM_KEYDOWN, keyNum->vkCode, lParam);
			listen.TextOutStatic(NULL, text, interval);
		}
		else if (wParam == WM_KEYUP) {		//�м��ɿ�
			listen.KeyStatInfo(keyNum->vkCode, text, true);
			listen.PushMsgBuf(keyNum->time, WM_KEYUP, keyNum->vkCode, lParam);
			listen.TextOutStatic(NULL, text);
		}
		else if (wParam == WM_CHAR) {
			listen.PushMsgBuf(keyNum->time, WM_CHAR, keyNum->vkCode, lParam);
		}
		
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


/*			//������ĸ��Сд
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


void CWorkHelperDlg::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
/*	
	CFileDialog FileDlg(true, NULL, L"*", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK,
		TEXT("�����ļ�(*.*)|*.*|JPEG�ļ�(*.jpg)|*.jpg|PNG�ļ�(*.png)|*.png|BMP�ļ�(*.bmp)|*.bmp||"), this);

	if (IDOK == FileDlg.DoModal()) {

		hHook = SetWindowsHookEx(
			WH_KEYBOARD_LL,    // �������͡���꡿
			KeyBoardProc,  // ������
			theApp.m_hInstance,      // ��ǰʵ�����
			0               // �������ھ��(NULLΪȫ�ּ���)
		);
*/
//	KDialog dlg(theApp.m_hInstance, this->GetSafeHwnd());
//	CMyDlg dlg;
//	CString result;
//	dlg.CreateModeDlg(_T("123"), CRect(0, 0, 100, 100), TRUE, this);
	KDialog dlg(this->GetSafeHwnd(), theApp.m_hInstance);
	INT_PTR nResponse = NULL;
	if (dlg.OnInitDialog())
	 nResponse = dlg.DoModal();

	if (nResponse == IDOK) MessageBox(L"ASDA", L"rewrew", MB_OK);
	else if (nResponse == IDCANCEL) MessageBox(L"BDSFS", L"rewrew", MB_OK);
	else if (nResponse == -1) MessageBox(L"SHFD", L"rewrew", MB_OK);

	if (hHook == NULL)
	{
		WCHAR text[30]{ 0 };
		wsprintf(text, L"���̼���ʧ�ܣ������� : %d", GetLastError());
		MessageBoxW(text, TEXT("ERROE"), MB_OK | MB_ICONERROR);
		return;
	}
	//	TextOutA(hDC, 30, 10, , 20);
	CListenKey::getInstance().TextOutStatic("���ڼ���������Ϣ...");
	this->SetFocus();

//	}
}


void CWorkHelperDlg::OnBnClickedFinish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!::UnhookWindowsHookEx(hHook)) return;
	if (!CListenKey::getInstance().SaveMsg2File()) MessageBox(L"�����¼�ļ�ʧ��", L"ERROR", MB_OK | MB_ICONERROR);

	CListenKey::getInstance().ExitHook();

	CListenKey::getInstance().TextOutStatic("��ǰδ�ڼ���״̬", " ", " ");
	this->SetFocus();
}

LRESULT CWorkHelperDlg::OnEndHook(WPARAM wParam, LPARAM lParam) {
	CListenKey::getInstance().ExitHook();
	::UnhookWindowsHookEx(hHook);
	return LRESULT(NULL);
}
