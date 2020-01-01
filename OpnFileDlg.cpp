#include "stdafx.h"
#include "OpnFileDlg.h"

#define	ID_EDIT	1000
#define	ID_OKBTN	1001
#define	ID_CANCELBTN	1002
#define	ID_STATIC	1003

HWND KDialog::m_hWnd = NULL;
//HINSTANCE KDialog::m_hInst = NULL;
HWND KDialog::m_hParent = NULL;
WCHAR KDialog::m_FileName[]{ 0 };
bool iLoseForce = false;
INT_PTR retModel = NULL;
/* �����ص����� */
//LRESULT CALLBACK KDialog::WndProc(HWND, UINT, WPARAM, LPARAM);


/* �ص����� */
LRESULT CALLBACK KDialog::WndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc = NULL;
	PAINTSTRUCT	ps{ 0 };
	RECT rect{ 0 };

	static int cxChar, cyChar;

	switch (umsg)
	{
	case WM_CREATE:            //�����ڴ����ɹ���������Ϣ
							   /* �������ڿؼ� */

//		cxChar = LOWORD(GetDialogBaseUnits());    //��ô������ڶ�������Ԫ��ȣ������飩
//		cyChar = HIWORD(GetDialogBaseUnits());    //��ô������ڶ�������Ԫ�߶ȣ������飩
		EnableWindow(m_hParent, false);
		return 0;

	case WM_COMMAND:
		if (ID_OKBTN == LOWORD(wParam)) {

			if (HIWORD(wParam) == BN_CLICKED) {
				HWND hEdit = GetDlgItem(m_hWnd, ID_EDIT);
				if (MAXFILENAME > GetWindowTextLength(hEdit)) {
					GetWindowText(hEdit, m_FileName, MAXFILENAME-1);
					DestroyWindow(m_hWnd);
					retModel = IDOK;
				}
			}
			return 0;
		}
		else if(ID_CANCELBTN == LOWORD(wParam)) {
			if (HIWORD(wParam) == BN_CLICKED) {
				DestroyWindow(m_hWnd);
				retModel = IDCANCEL;
			}
			return 0;
		}
		break;

	case WM_PAINT:             //�����ڲ�����Ч����ʱ��������Ϣ
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
//		TextOut(hdc, rect.left + cxChar * 12, 95, TEXT("UserName:"), lstrlen(TEXT("UserName:")));
//		TextOut(hdc, rect.left + cxChar * 12, 145, TEXT("Password:"), lstrlen(TEXT("Password:")));
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:           //�����ڹر�ʱ��������Ϣ
		::PostQuitMessage(0);
		EnableWindow(m_hParent, true);
		return 0;

	case WM_NCACTIVATE:
		if (wParam == 0) iLoseForce = true;
		break;
	}

	return DefWindowProc(hwnd, umsg, wParam, lParam);
}


/* �����ؼ� */
bool KDialog::CreateWindows()
{
	RECT rect{ 0 };
	GetWindowRect(m_hParent, &rect);

	/* ���������� */
	m_hWnd = CreateWindow(m_szAppName, TEXT("������Ϣ�ļ�"),
		WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		rect.left + 100, rect.top + 20,
		m_cWidth, m_cHenght,
		m_hParent, NULL, NULL, NULL);
	if (!m_hWnd) return false;

	/* �����ı��� */
	m_hChild[0] = CreateWindow(TEXT("EDIT"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		DEFAULTWIDTH / 2 - 200 / 3, DEFAULTHEIGHT / 4 - 30 / 2, 200, 30,
		m_hWnd, (HMENU)ID_EDIT, NULL, NULL);

	/* ����ȷ����ť */
	m_hChild[1] = CreateWindow(TEXT("BUTTON"), TEXT("ȷ��"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		DEFAULTWIDTH / 4 - 60 / 2, (DEFAULTHEIGHT * 2) / 3 - 30 / 2, 60, 30,
		m_hWnd, (HMENU)ID_OKBTN, NULL, NULL);

	/* ����ȡ����ť */
	m_hChild[2] = CreateWindow(TEXT("BUTTON"), TEXT("ȡ��"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER,
		(DEFAULTWIDTH * 3) / 4 - 60 / 2, (DEFAULTHEIGHT * 2) / 3 - 30 / 2, 60, 30,
		m_hWnd, (HMENU)ID_CANCELBTN, NULL, NULL);

	//������̬�ı�
	m_hChild[3] = CreateWindow(TEXT("STATIC"), TEXT("�ļ�����"),
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		DEFAULTWIDTH / 4 - 80 / 2, DEFAULTHEIGHT / 4 - 30 / 2, 80, 30,
		m_hWnd, (HMENU)ID_STATIC, NULL, NULL);

	if (!(m_hChild[0] && m_hChild[1] && m_hChild[2] && m_hChild[3])) {
		DestroyWindow(m_hWnd);
		return false;
	}

	/* ��ʾ����´��� */
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	return true;
}

INT_PTR KDialog::DoModal() {
	MSG msg;

	if(!CreateWindows()) return -1;

	/* ����Ϣ�����л�ȡ��Ϣ */
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		if (iLoseForce) {
			if (msg.hwnd == m_hParent) {
				if (GetForegroundWindow() == m_hParent) {
					FLASHWINFO fw;
					fw.cbSize = sizeof(FLASHWINFO);
					fw.dwFlags = FLASHW_CAPTION;
					fw.hwnd = m_hWnd;
					fw.uCount = 3;
					fw.dwTimeout = 300;
					FlashWindowEx(&fw);
					SetFocus(m_hWnd);
				}
			}
			iLoseForce = false;
		}

		TranslateMessage(&msg);     //���������Ϣת��Ϊ�ַ���Ϣ
		DispatchMessage(&msg);      //����Ϣ���ݸ��ص���������
	}

	SetForegroundWindow(m_hParent);
	return retModel;
}