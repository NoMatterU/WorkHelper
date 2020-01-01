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
/* 声明回调函数 */
//LRESULT CALLBACK KDialog::WndProc(HWND, UINT, WPARAM, LPARAM);


/* 回调函数 */
LRESULT CALLBACK KDialog::WndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc = NULL;
	PAINTSTRUCT	ps{ 0 };
	RECT rect{ 0 };

	static int cxChar, cyChar;

	switch (umsg)
	{
	case WM_CREATE:            //处理窗口创建成功后发来的消息
							   /* 创建窗口控件 */

//		cxChar = LOWORD(GetDialogBaseUnits());    //获得窗口中内定字体字元宽度（低字组）
//		cyChar = HIWORD(GetDialogBaseUnits());    //或得窗口中内定字体字元高度（高字组）
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

	case WM_PAINT:             //处理窗口产生无效区域时发来的消息
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
//		TextOut(hdc, rect.left + cxChar * 12, 95, TEXT("UserName:"), lstrlen(TEXT("UserName:")));
//		TextOut(hdc, rect.left + cxChar * 12, 145, TEXT("Password:"), lstrlen(TEXT("Password:")));
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:           //处理窗口关闭时发来的消息
		::PostQuitMessage(0);
		EnableWindow(m_hParent, true);
		return 0;

	case WM_NCACTIVATE:
		if (wParam == 0) iLoseForce = true;
		break;
	}

	return DefWindowProc(hwnd, umsg, wParam, lParam);
}


/* 创建控件 */
bool KDialog::CreateWindows()
{
	RECT rect{ 0 };
	GetWindowRect(m_hParent, &rect);

	/* 创建主窗口 */
	m_hWnd = CreateWindow(m_szAppName, TEXT("保存消息文件"),
		WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		rect.left + 100, rect.top + 20,
		m_cWidth, m_cHenght,
		m_hParent, NULL, NULL, NULL);
	if (!m_hWnd) return false;

	/* 创建文本框 */
	m_hChild[0] = CreateWindow(TEXT("EDIT"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		DEFAULTWIDTH / 2 - 200 / 3, DEFAULTHEIGHT / 4 - 30 / 2, 200, 30,
		m_hWnd, (HMENU)ID_EDIT, NULL, NULL);

	/* 创建确定按钮 */
	m_hChild[1] = CreateWindow(TEXT("BUTTON"), TEXT("确定"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		DEFAULTWIDTH / 4 - 60 / 2, (DEFAULTHEIGHT * 2) / 3 - 30 / 2, 60, 30,
		m_hWnd, (HMENU)ID_OKBTN, NULL, NULL);

	/* 创建取消按钮 */
	m_hChild[2] = CreateWindow(TEXT("BUTTON"), TEXT("取消"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER,
		(DEFAULTWIDTH * 3) / 4 - 60 / 2, (DEFAULTHEIGHT * 2) / 3 - 30 / 2, 60, 30,
		m_hWnd, (HMENU)ID_CANCELBTN, NULL, NULL);

	//创建静态文本
	m_hChild[3] = CreateWindow(TEXT("STATIC"), TEXT("文件名："),
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		DEFAULTWIDTH / 4 - 80 / 2, DEFAULTHEIGHT / 4 - 30 / 2, 80, 30,
		m_hWnd, (HMENU)ID_STATIC, NULL, NULL);

	if (!(m_hChild[0] && m_hChild[1] && m_hChild[2] && m_hChild[3])) {
		DestroyWindow(m_hWnd);
		return false;
	}

	/* 显示与更新窗口 */
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	return true;
}

INT_PTR KDialog::DoModal() {
	MSG msg;

	if(!CreateWindows()) return -1;

	/* 从消息队列中获取消息 */
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

		TranslateMessage(&msg);     //将虚拟键消息转换为字符消息
		DispatchMessage(&msg);      //将消息传递给回调函数处理
	}

	SetForegroundWindow(m_hParent);
	return retModel;
}