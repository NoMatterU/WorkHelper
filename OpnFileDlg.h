#pragma once
#include "stdafx.h"

#define DEFAULTHEIGHT 260
#define DEFAULTWIDTH 400

class KDialog
{
public:

	KDialog() {
		RegisterWindow();
	};

	KDialog(HWND hWnd, PWSTR &&ClassName = NULL) {
		if(ClassName) StrCpyW(m_szAppName, ClassName);
		RegisterWindow();
		m_hParent = hWnd;
	}

	~KDialog() {};

	virtual INT_PTR DoModal();

protected:

	// API��ע�����Ϣ�������������ǳ�Ա��������Ϊ��Ա������thisָ��
	static LRESULT CALLBACK WndProc(HWND hWnd,
		UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL CreateWindows();

private:
	VOID RegisterWindow() {
		m_Wnd.style = CS_HREDRAW | CS_VREDRAW;                        //������ʽ
		m_Wnd.lpfnWndProc = WndProc;                                        //���ڵĻص�����
		m_Wnd.hInstance = NULL;                                    //����ʵ�����
		m_Wnd.cbClsExtra = 0;                                            //���ڽṹ����չ����
		m_Wnd.cbWndExtra = 0;                                            //����ʵ����չ����
		m_Wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //���ڱ���
		m_Wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //����ͼ��
		m_Wnd.hCursor = LoadCursor(NULL, IDC_ARROW);                //�����е������ʽ
		m_Wnd.lpszClassName = m_szAppName;                                    //���ڽṹ������
		m_Wnd.lpszMenuName = NULL;      //���˵����ƣ���
		RegisterClass(&m_Wnd);
	};

	WCHAR m_szAppName[20]{L"Demo"};

	HWND m_hChild[4]{ 0 };

	WNDCLASS m_Wnd{ 0 };

	int m_cHenght{ DEFAULTHEIGHT };
	int m_cWidth{ DEFAULTWIDTH };
public:
	static HWND  m_hWnd;
	static HWND m_hParent;
};


