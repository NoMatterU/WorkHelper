#pragma once
#include "stdafx.h"
#include "resource.h"

//���ڴ�С
#define DEFAULTHEIGHT 240
#define DEFAULTWIDTH 400
//�ļ� �����ִ���󳤶�
#define MAXFILENAME 30 + 1
#define MAX_CLASSNAME_LEN 50

class ModelDialog {
public:
	ModelDialog() {
		RegisterWindow();
	}

	~ModelDialog() {
		//hInstance NULL �ܳɹ�?
		UnregisterClass(m_className, NULL);
	}
	virtual INT_PTR DoModal() {
	}

	virtual bool CreateWindows(LPWSTR pWndName, int cx, int cy, int height, int width) {
		RECT rect{ 0 };
		//hParent NULL ʱ�����С
		if(hParent) GetWindowRect(hParent, &rect);

		int cx = rect.left + ((rect.right - rect.left) - DEFAULTWIDTH) / 2;
		int cy = rect.top + ((rect.bottom - rect.top) - DEFAULTHEIGHT) / 2;

		if (pWndName == NULL) pWndName = L"Default";
		/* ���������� */
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
		m_Wndc.style = CS_HREDRAW | CS_VREDRAW;                        //������ʽ
		m_Wndc.lpfnWndProc = CallWndProc;                                        //���ڵĻص�����
		m_Wndc.hInstance = NULL;                                    //����ʵ�����
		m_Wndc.cbClsExtra = 0;                                            //���ڽṹ����չ����
		m_Wndc.cbWndExtra = 0;                                            //����ʵ����չ����
		m_Wndc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //���ڱ���
		m_Wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //����ͼ��
		m_Wndc.hCursor = LoadCursor(NULL, IDC_ARROW);                //�����е������ʽ
		m_Wndc.lpszClassName = m_className;                                    //���ڽṹ������
		m_Wndc.lpszMenuName = NULL;      //���˵����ƣ���
		RegisterClass(&m_Wndc);
	};
private:
	/* �ص����� */
	static LRESULT CALLBACK CallWndProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
	{
		HDC	hdc = NULL;
		HBRUSH hBrush = NULL;
		PAINTSTRUCT	ps{ 0 };
		RECT rect{ 0 };

		switch (umsg)
		{
		case WM_CREATE:     //�����ڴ����ɹ���������Ϣ
			SendMessage(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)));//��Ӵ���Сͼ��
			EnableWindow(hParent, false);
			return 0;

		case WM_COMMAND:
			OnCommand(wParam, lParam);
			break;

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

	// API��ע�����Ϣ�������������ǳ�Ա��������Ϊ��Ա������thisָ��
	static LRESULT CALLBACK WndProc(HWND hWnd,
		UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool CreateWindows();

private:

	void RegisterWindow() {
		m_Wndc.style = CS_HREDRAW | CS_VREDRAW;                        //������ʽ
		m_Wndc.lpfnWndProc = WndProc;                                        //���ڵĻص�����
		m_Wndc.hInstance = NULL;                                    //����ʵ�����
		m_Wndc.cbClsExtra = 0;                                            //���ڽṹ����չ����
		m_Wndc.cbWndExtra = 0;                                            //����ʵ����չ����
		m_Wndc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //���ڱ���
		m_Wndc.hIcon = LoadIcon(NULL, IDI_APPLICATION);            //����ͼ��
		m_Wndc.hCursor = LoadCursor(NULL, IDC_ARROW);                //�����е������ʽ
		m_Wndc.lpszClassName = m_szAppName;                                    //���ڽṹ������
		m_Wndc.lpszMenuName = NULL;      //���˵����ƣ���
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


