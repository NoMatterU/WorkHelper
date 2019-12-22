#pragma once
#include "stdafx.h"

/**/
#define ID_EXIT   150
#define ID_EDIT	  180
#define ID_TEXT   200

class KDialog
{
public:

	KDialog(HINSTANCE hinst, HWND hwndOwner);

	~KDialog();

	virtual INT_PTR DoModal();

protected:

	// API��ע�����Ϣ�������������ǳ�Ա��������Ϊ��Ա������thisָ��
	static BOOL CALLBACK DialogProc(HWND hWnd,
		UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	HWND  m_hOwnerWnd;

	HINSTANCE m_hInst;

};


