#pragma once
#include "stdafx.h"


class KDialog
{
public:

	KDialog() {
	};

	KDialog(HWND hWnd, HINSTANCE hInst) {
		StrCpyW(m_szAppName, L"Demo");

		m_Wnd.style =  CS_HREDRAW | CS_VREDRAW;                        //窗口样式
		m_Wnd.lpfnWndProc = WndProc;                                        //窗口的回调函数
		m_Wnd.hInstance = NULL;                                    //窗口实例句柄
		m_Wnd.cbClsExtra = 0;                                            //窗口结构体扩展：无
		m_Wnd.cbWndExtra = 0;                                            //窗口实例扩展：无
		m_Wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //窗口背景
		m_Wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //窗口图标
		m_Wnd.hCursor = LoadCursor(NULL, IDC_ARROW);                //窗口中的鼠标样式
		m_Wnd.lpszClassName = m_szAppName;                                    //窗口结构体名称
		m_Wnd.lpszMenuName = NULL;      //主菜单名称：无

		m_hParent = hWnd;
		m_hInst = hInst;
	}

	~KDialog() {};

	virtual INT_PTR DoModal();
	BOOL OnInitDialog();

protected:

	// API中注册的消息处理函数，不能是成员函数，因为成员函数有this指针
	static LRESULT CALLBACK WndProc(HWND hWnd,
		UINT uMsg, WPARAM wParam, LPARAM lParam);

	void CreateChildWindow();
private:
	WCHAR m_szAppName[20];

	HWND m_hChild[3]{ 0 };

	HINSTANCE m_hInst = NULL;

	WNDCLASS m_Wnd{ 0 };
public:
	static HWND  m_hWnd;
	static HWND m_hParent;
};


