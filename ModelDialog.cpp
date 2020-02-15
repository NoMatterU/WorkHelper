#pragma once
#include "stdafx.h"
#include "ModelDialog.h"

HWND ModelDialog::hParent = NULL;
bool ModelDialog::iLoseForce = false;

void defCommand(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) { return; }
funptr pOnCommand = defCommand;

INT_PTR ModelDialog::DoModal() {
	MSG msg;

	if (!CreateWindows(L"打开消息文件")) return -1;

	/* 从消息队列中获取消息 */
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		if (iLoseForce) {
			if (msg.hwnd == hParent) {
				if (GetForegroundWindow() == hParent) {
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

	//清空操作
	m_hWnd = NULL;
	Sleep(90);
	SetForegroundWindow(hParent);
	return IDOK;
}

bool ModelDialog::CreateWindows(LPWSTR pWndName, int cx, int cy, int height, int width) {
	RECT rect{ 0 };
	//hParent NULL 时桌面大小
	if (hParent) GetWindowRect(hParent, &rect);

	if (cx < 0 || cx > rect.right)
		cx = rect.left + ((rect.right - rect.left) - DEFAULTWIDTH) / 2;
	if (cy < 0 || cy > rect.bottom)
		cy = rect.top + ((rect.bottom - rect.top) - DEFAULTHEIGHT) / 2;

	if (pWndName == NULL) pWndName = L"Default";

	if ((height <= 0) || (height > rect.right - rect.left)) height = m_height;
	if((width <= 0) || (width > rect.bottom - rect.top)) width = m_width;


	/* 创建主窗口 */
	m_hWnd = CreateWindowW(m_className, pWndName,
		WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		cx, cy,
		width, height,
		hParent, NULL, NULL, NULL);
	if (!m_hWnd) return false;

	// 显示与更新窗口
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	return true;
}


void ModelDialog::RegisterWindow() {
	m_Wndc.style = CS_HREDRAW | CS_VREDRAW;                //窗口样式
	m_Wndc.lpfnWndProc = CallWndProc;              //窗口的回调函数
	m_Wndc.hInstance = NULL;                       //窗口实例句柄
	m_Wndc.cbClsExtra = 0;                              //窗口结构体扩展：无
	m_Wndc.cbWndExtra = 0;                          //窗口实例扩展：无
	m_Wndc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //窗口背景
	m_Wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);       //窗口图标
	m_Wndc.hCursor = LoadCursor(NULL, IDC_ARROW);      //窗口中的鼠标样式
	m_Wndc.lpszClassName = m_className;                  //窗口结构体名称
	m_Wndc.lpszMenuName = NULL;      //主菜单名称：无
	RegisterClass(&m_Wndc);
};


LRESULT CALLBACK ModelDialog::CallWndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc = NULL;
	HBRUSH hBrush = NULL;
	PAINTSTRUCT	ps{ 0 };
	RECT rect{ 0 };

	switch (umsg)
	{
	case WM_CREATE:     //处理窗口创建成功后发来的消息
		SendMessageW(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)));//添加窗口小图标
		EnableWindow(hParent, false);
		return 0;

	case WM_COMMAND:
		if(pOnCommand)
			(pOnCommand)(hwnd, umsg, wParam, lParam);
		return 0;

	case WM_PAINT:             //处理窗口产生无效区域时发来的消息
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(240, 240, 240));
		FillRect(hdc, &rect, hBrush);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:           //处理窗口关闭时发来的消息
		::PostQuitMessage(0);
		EnableWindow(hParent, true);
		return 0;

	case WM_NCACTIVATE:
		if (wParam == 0) iLoseForce = true;
		break;

	}
	return DefWindowProc(hwnd, umsg, wParam, lParam);
}
