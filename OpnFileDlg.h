#pragma once
#include "stdafx.h"
#include "resource.h"
#include "ModelDialog.h"


class OpnFileDlg : ModelDialog
{
public:

	OpnFileDlg() : ModelDialog() {
	};

	OpnFileDlg(HWND hParent)  : ModelDialog(hParent)
	{
	//	ModelDialog::ResizeWindow(Height, Width);
	}

	~OpnFileDlg() {};

protected:

	virtual bool CreateWindows(LPWSTR pWndName, int cx, int cy, int height, int width);

private:

//	static WCHAR m_FileName[MAXFILENAME];

//	HWND  m_hWnd;

	HWND m_hChild[4]{ 0 };

public:
	virtual INT_PTR DoModal();

	const PWCH GetFileName();
};
