#pragma once
#include "stdafx.h"
#include "resource.h"

//���ڴ�С
#define DEFAULTHEIGHT 240
#define DEFAULTWIDTH 400
//�ļ� �����ִ���󳤶�
#define MAXFILENAME 30 + 1
#define MAXCLASSNAME 20 + 1

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

	~OpnFileDlg() {};

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

	WCHAR m_szAppName[MAXCLASSNAME]{L"OpnFileDlg"};
	static WCHAR m_FileName[MAXFILENAME];
	static HWND  m_hWnd;
	static HWND m_hParent;

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


