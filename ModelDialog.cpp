#pragma once
#include "stdafx.h"
#include "ModelDialog.h"

HWND ModelDialog::hParent = NULL;
bool ModelDialog::iLoseForce = false;

void defCommand(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam) { return; }
funptr pOnCommand = defCommand;

INT_PTR ModelDialog::DoModal() {
	MSG msg;

	if (!CreateWindows(L"����Ϣ�ļ�")) return -1;

	/* ����Ϣ�����л�ȡ��Ϣ */
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

		TranslateMessage(&msg);     //���������Ϣת��Ϊ�ַ���Ϣ
		DispatchMessage(&msg);      //����Ϣ���ݸ��ص���������
	}

	//��ղ���
	m_hWnd = NULL;
	Sleep(90);
	SetForegroundWindow(hParent);
	return IDOK;
}

bool ModelDialog::CreateWindows(LPWSTR pWndName, int cx, int cy, int height, int width) {
	RECT rect{ 0 };
	//hParent NULL ʱ�����С
	if (hParent) GetWindowRect(hParent, &rect);

	if (cx < 0 || cx > rect.right)
		cx = rect.left + ((rect.right - rect.left) - DEFAULTWIDTH) / 2;
	if (cy < 0 || cy > rect.bottom)
		cy = rect.top + ((rect.bottom - rect.top) - DEFAULTHEIGHT) / 2;

	if (pWndName == NULL) pWndName = L"Default";

	if ((height <= 0) || (height > rect.right - rect.left)) height = m_height;
	if((width <= 0) || (width > rect.bottom - rect.top)) width = m_width;


	/* ���������� */
	m_hWnd = CreateWindowW(m_className, pWndName,
		WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		cx, cy,
		width, height,
		hParent, NULL, NULL, NULL);
	if (!m_hWnd) return false;

	// ��ʾ����´���
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	return true;
}


void ModelDialog::RegisterWindow() {
	m_Wndc.style = CS_HREDRAW | CS_VREDRAW;                //������ʽ
	m_Wndc.lpfnWndProc = CallWndProc;              //���ڵĻص�����
	m_Wndc.hInstance = NULL;                       //����ʵ�����
	m_Wndc.cbClsExtra = 0;                              //���ڽṹ����չ����
	m_Wndc.cbWndExtra = 0;                          //����ʵ����չ����
	m_Wndc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //���ڱ���
	m_Wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);       //����ͼ��
	m_Wndc.hCursor = LoadCursor(NULL, IDC_ARROW);      //�����е������ʽ
	m_Wndc.lpszClassName = m_className;                  //���ڽṹ������
	m_Wndc.lpszMenuName = NULL;      //���˵����ƣ���
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
	case WM_CREATE:     //�����ڴ����ɹ���������Ϣ
		SendMessageW(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)));//��Ӵ���Сͼ��
		EnableWindow(hParent, false);
		return 0;

	case WM_COMMAND:
		if(pOnCommand)
			(pOnCommand)(hwnd, umsg, wParam, lParam);
		return 0;

	case WM_PAINT:             //�����ڲ�����Ч����ʱ��������Ϣ
		GetClientRect(hwnd, &rect);
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(240, 240, 240));
		FillRect(hdc, &rect, hBrush);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:           //�����ڹر�ʱ��������Ϣ
		::PostQuitMessage(0);
		EnableWindow(hParent, true);
		return 0;

	case WM_NCACTIVATE:
		if (wParam == 0) iLoseForce = true;
		break;

	}
	return DefWindowProc(hwnd, umsg, wParam, lParam);
}
