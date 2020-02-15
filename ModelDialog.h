#pragma once
#include "stdafx.h"
#include "resource.h"

//窗口大小
#define DEFAULTHEIGHT 240
#define DEFAULTWIDTH 400
//文件 窗口字串最大长度
#define MAXFILENAME 30 + 1
#define MAX_CLASSNAME_LEN 50

#define DEFAULT_VAlUE -1
//class ModelDialog;
//typedef void(_cdecl ModelDialog::*funptr)(WPARAM, LPARAM);
#define ON_MYCOMMAND
typedef void(*funptr)(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);

#define MSG_COMMAND(x) \
void _setcommand(){\
if(x) pOnCommand = x;\
}

extern funptr pOnCommand;
void _setcommand();

class ModelDialog {
public:
	ModelDialog() {};

	ModelDialog(HWND hParent) {
		RegisterWindow();
		_setcommand();
//		pOnCommand = onCommand;
		this->hParent = hParent;
	}

 	virtual ~ModelDialog() {
		//hInstance NULL 能成功?
		UnregisterClassW(m_className, NULL);
	}

	virtual INT_PTR DoModal();

	virtual bool CreateWindows(LPWSTR pWndName, int cx = DEFAULT_VAlUE, int cy = DEFAULT_VAlUE, int height = DEFAULT_VAlUE, int width = DEFAULT_VAlUE);

	//	static void SetOnComandMsg(funptr fun) {
	//		pOnCommand = fun;
	//	}

	void ResizeWindow(int height, int width) {
		m_height = height;
		m_width = width;
		if(m_hWnd) MoveWindow(m_hWnd, 0, 0, height, width, true);
	}

	const HWND getSafehWnd() { return m_hWnd; }

//	virtual void OnCommand(WPARAM wParam, LPARAM lParam) {};
protected:
	static HWND hParent;
	static bool iLoseForce;

	void RegisterWindow();
private:
	/* 回调函数 */
	static LRESULT CALLBACK CallWndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd = NULL;
//	static funptr pOnCommand;
	const WCHAR m_className[MAX_CLASSNAME_LEN] = { L"YDDDIALOG" };
	WNDCLASS m_Wndc = { 0 };
	int m_height = DEFAULTHEIGHT;
	int m_width = DEFAULTWIDTH;
};