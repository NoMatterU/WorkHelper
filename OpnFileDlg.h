#pragma once
#include "stdafx.h"


class KDialog
{
public:

	KDialog() {
	};

	KDialog(HWND hWnd, HINSTANCE hInst) {
		StrCpyW(m_szAppName, L"Demo");

		m_Wnd.style =  CS_HREDRAW | CS_VREDRAW;                        //������ʽ
		m_Wnd.lpfnWndProc = WndProc;                                        //���ڵĻص�����
		m_Wnd.hInstance = NULL;                                    //����ʵ�����
		m_Wnd.cbClsExtra = 0;                                            //���ڽṹ����չ����
		m_Wnd.cbWndExtra = 0;                                            //����ʵ����չ����
		m_Wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //���ڱ���
		m_Wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //����ͼ��
		m_Wnd.hCursor = LoadCursor(NULL, IDC_ARROW);                //�����е������ʽ
		m_Wnd.lpszClassName = m_szAppName;                                    //���ڽṹ������
		m_Wnd.lpszMenuName = NULL;      //���˵����ƣ���

		m_hParent = hWnd;
		m_hInst = hInst;
	}

	~KDialog() {};

	virtual INT_PTR DoModal();
	BOOL OnInitDialog();

protected:

	// API��ע�����Ϣ�������������ǳ�Ա��������Ϊ��Ա������thisָ��
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


