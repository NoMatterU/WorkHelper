#pragma once
#include "stdafx.h"
#include "resource.h"

//窗口大小
#define DEFAULTHEIGHT 240
#define DEFAULTWIDTH 400
//文件 窗口字串最大长度
#define MAXFILENAME 30 + 1
#define MAX_CLASSNAME_LEN 50

class ModelDialog {
public:
	ModelDialog() {
		RegisterWindow();
	}

	~ModelDialog() {
		//hInstance NULL 能成功?
		UnregisterClass(m_className, NULL);
	}
	virtual INT_PTR DoModal() {
	}

	virtual bool CreateWindows(LPWSTR pWndName, int cx, int cy, int height, int width) {
		RECT rect{ 0 };
		//hParent NULL 时桌面大小
		if(hParent) GetWindowRect(hParent, &rect);

		int cx = rect.left + ((rect.right - rect.left) - DEFAULTWIDTH) / 2;
		int cy = rect.top + ((rect.bottom - rect.top) - DEFAULTHEIGHT) / 2;

		if (pWndName == NULL) pWndName = L"Default";
		/* 创建主窗口 */
		m_hWnd = CreateWindow(m_className, pWndName,
			WS_EX_LAYERED | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
			cx, cy,
			m_width, m_height,
			hParent, NULL, NULL, NULL);
		if (!m_hWnd) return false;
		return true;
	}

	void ResizeWindow(int height, int width);

	virtual void OnCommand(WPARAM wParam, LPARAM lParam);
protected:
	static HWND hParent;
	static bool m_iLoseForce;

	void RegisterWindow() {
		m_Wndc.style = CS_HREDRAW | CS_VREDRAW;                        //窗口样式
		m_Wndc.lpfnWndProc = CallWndProc;                                        //窗口的回调函数
		m_Wndc.hInstance = NULL;                                    //窗口实例句柄
		m_Wndc.cbClsExtra = 0;                                            //窗口结构体扩展：无
		m_Wndc.cbWndExtra = 0;                                            //窗口实例扩展：无
		m_Wndc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //窗口背景
		m_Wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //窗口图标
		m_Wndc.hCursor = LoadCursor(NULL, IDC_ARROW);                //窗口中的鼠标样式
		m_Wndc.lpszClassName = m_className;                                    //窗口结构体名称
		m_Wndc.lpszMenuName = NULL;      //主菜单名称：无
		RegisterClass(&m_Wndc);
	};
private:
	/* 回调函数 */
	static LRESULT CALLBACK CallWndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
	{
		HDC	hdc = NULL;
		HBRUSH hBrush = NULL;
		PAINTSTRUCT	ps{ 0 };
		RECT rect{ 0 };

		switch (umsg)
		{
		case WM_CREATE:     //处理窗口创建成功后发来的消息
			SendMessage(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)));//添加窗口小图标
			EnableWindow(hParent, false);
			return 0;

		case WM_COMMAND:
			OnCommand(wParam, lParam);
			break;

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
			if (wParam == 0) m_iLoseForce = true;
			break;

		}
		return DefWindowProc(hwnd, umsg, wParam, lParam);
	}

	HWND m_hWnd = NULL;
	const WCHAR m_className[MAX_CLASSNAME_LEN] = { L"YDDDIALOG" };
	WNDCLASS m_Wndc = { 0 };
	int m_height = DEFAULTHEIGHT;
	int m_width = DEFAULTWIDTH;
};

class OpnFileDlg
{
public:

	OpnFileDlg() {
		RegisterWindow();
	};

	OpnFileDlg(HWND hWnd, int Height = DEFAULTHEIGHT, int Width = DEFAULTWIDTH) : m_cHenght(Height), m_cWidth(Width)
	{
		RegisterWindow();
		m_hParent = hWnd;
	}

	~OpnFileDlg() {
		UnregisterClassW(m_szAppName, NULL);
	};

protected:

	// API中注册的消息处理函数，不能是成员函数，因为成员函数有this指针
	static LRESULT CALLBACK WndProc(HWND hWnd,
		UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool CreateWindows();

private:

	void RegisterWindow() {
		m_Wndc.style = CS_HREDRAW | CS_VREDRAW;                        //窗口样式
		m_Wndc.lpfnWndProc = WndProc;                                        //窗口的回调函数
		m_Wndc.hInstance = NULL;                                    //窗口实例句柄
		m_Wndc.cbClsExtra = 0;                                            //窗口结构体扩展：无
		m_Wndc.cbWndExtra = 0;                                            //窗口实例扩展：无
		m_Wndc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //窗口背景
		m_Wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //窗口图标
		m_Wndc.hCursor = LoadCursor(NULL, IDC_ARROW);                //窗口中的鼠标样式
		m_Wndc.lpszClassName = m_szAppName;                                    //窗口结构体名称
		m_Wndc.lpszMenuName = NULL;      //主菜单名称：无
		RegisterClass(&m_Wndc);
	};

//	WCHAR m_szAppName[MAXCLASSNAME]{L"OpnFileDlg"};
	static WCHAR m_FileName[MAXFILENAME];
	static HWND m_hParent;
	static HWND  m_hWnd;

	HWND m_hChild[4]{ 0 };

	WNDCLASS m_Wndc{ 0 };

	int m_cHenght{ DEFAULTHEIGHT };
	int m_cWidth{ DEFAULTWIDTH };

public:
	virtual INT_PTR DoModal();

	void SetClassName(PWCH &&className) {
		if (className) StrCpyW(m_szAppName, className);
	}

	const PWCH GetFileName() {
		return m_FileName;
	}
};


