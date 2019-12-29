#include "stdafx.h"
#include "OpnFileDlg.h"

#define         ID_UserName         1000
#define         ID_Password         1001
#define         ID_btnLogin         1002

HWND KDialog::m_hWnd = NULL;
HWND KDialog::m_hParent = NULL;
bool iLoseForce = false;
/* �����ص����� */
//LRESULT CALLBACK KDialog::WndProc(HWND, UINT, WPARAM, LPARAM);

/* �����������ڿؼ� */
int CreateChildWindow(HWND, HWND *, LPARAM);


/* �ص����� */
LRESULT CALLBACK KDialog::WndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
//	static HWND     hChild[2];
	HDC             hdc;
	PAINTSTRUCT     ps;
	RECT            rect;

	static int      cxChar, cyChar;
//	if (hwnd == m_hParent) return 0;
	switch (umsg)
	{
	case WM_CREATE:            //�����ڴ����ɹ���������Ϣ
							   /* �������ڿؼ� */

//		cxChar = LOWORD(GetDialogBaseUnits());    //��ô������ڶ�������Ԫ��ȣ������飩
//		cyChar = HIWORD(GetDialogBaseUnits());    //��ô������ڶ�������Ԫ�߶ȣ������飩
		EnableWindow(m_hParent, false);
		return 0;

	case WM_SIZE:              //�����ڳߴ緢���ı��������Ϣ
//		GetClientRect(hwnd, &rect);
//		MoveWindow(hChild[0], rect.left + cxChar * 12, 110, cxChar * 24, cyChar * 5 / 4, TRUE);
//		MoveWindow(hChild[1], rect.left + cxChar * 12, 160, cxChar * 24, cyChar * 5 / 4, TRUE);
//		MoveWindow(hChild[2], rect.left + cxChar * 12, 210, cxChar * 12, cyChar * 7 / 4, TRUE);
		return 0;

	case WM_PAINT:             //�����ڲ�����Ч����ʱ��������Ϣ
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, rect.left + cxChar * 12, 95, TEXT("UserName:"), lstrlen(TEXT("UserName:")));
		TextOut(hdc, rect.left + cxChar * 12, 145, TEXT("Password:"), lstrlen(TEXT("Password:")));
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:           //�����ڹر�ʱ��������Ϣ
		::PostQuitMessage(0);
		EnableWindow(m_hParent, true);
		return 0;

	case WM_NCACTIVATE:
		if (wParam == 0) {
			iLoseForce = true;
		}
		break;
	}

	return DefWindowProc(hwnd, umsg, wParam, lParam);
}

BOOL KDialog::OnInitDialog()
{
	if (!RegisterClass(&m_Wnd))
	{
		MessageBox(NULL, TEXT("�޷�ע�ᴰ�ڽṹ�壡"), TEXT("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	/* ����Login���� */
	m_hWnd = CreateWindow(m_szAppName, TEXT("��¼"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		400, 300,
		m_hParent, NULL, m_hInst, NULL);
	if (m_hWnd == NULL) return false;

	CreateChildWindow();
//	ShowWindow(m_hParent, SW_HIDE);
	return true;
}

/* �����ؼ� */
void KDialog::CreateChildWindow()
{
	/* ����UserName�ı��� */
	m_hChild[0] = CreateWindow(TEXT("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		200, 80, 100, 20,
		m_hWnd, (HMENU)ID_UserName, m_hInst, NULL);

	/* ����Password�ı��� */
	m_hChild[1] = CreateWindow(TEXT("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		200, 100, 100, 20,
		m_hWnd, (HMENU)ID_Password, m_hInst, NULL);

	/* ����Login��ť */
	m_hChild[2] = CreateWindow(TEXT("button"), TEXT("Login"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER,
		200, 120, 40, 40,
		m_hWnd, (HMENU)ID_btnLogin, m_hInst, NULL);
}

INT_PTR KDialog::DoModal() {
	MSG msg;

	/* ��ʾ����´��� */
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	/* ����Ϣ�����л�ȡ��Ϣ */
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		if (iLoseForce) if (msg.hwnd == m_hParent) {
			if (GetForegroundWindow() == m_hParent) {
				FLASHWINFO fw;
				fw.cbSize = sizeof(FLASHWINFO);
				fw.dwFlags = FLASHW_ALL;
				fw.hwnd = m_hWnd;
				fw.uCount = 3;
				fw.dwTimeout = 300;
				FlashWindowEx(&fw);
				SetForegroundWindow(m_hWnd);
				SetFocus(m_hWnd);
			}
			iLoseForce = false;
		}

		TranslateMessage(&msg);     //���������Ϣת��Ϊ�ַ���Ϣ
		DispatchMessage(&msg);      //����Ϣ���ݸ��ص���������

	}

	return msg.wParam;
}