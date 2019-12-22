#include "stdafx.h"
#include "OpnFileDlg.h"
/*
BOOL COpnFileDlg::OnInitDialog()
{
	this->MoveWindow(200, 200, 400, 600);
//	m_pFilePath->Create(L"ASB", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(50, 80, 150, 150), this);
	return TRUE;
}

*/


BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

LPWORD lpwAlign(LPWORD lpIn)
{
	ULONG ul;

	ul = (ULONG)lpIn;
	ul += 3;
	ul >>= 2;
	ul <<= 2;
	return (LPWORD)ul;
}

KDialog::KDialog(HINSTANCE hinst, HWND hwndOwner)
{
	m_hInst = hinst;
	m_hOwnerWnd = hwndOwner;
}

KDialog::~KDialog()
{

}

WCHAR nEditTwo[128] = L"从这儿输入字符串.";

BOOL CALLBACK KDialog::DialogProc(HWND hDlg,
	UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, ID_EDIT, nEditTwo);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EDIT:
			GetDlgItemText(hDlg, ID_EDIT, nEditTwo, 127);
			return TRUE;
		case ID_HELP:
			MessageBox(NULL, nEditTwo, L"输入数据", MB_OK | MB_SYSTEMMODAL | MB_NOFOCUS);
			return TRUE;
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}
/*
INT_PTR KDialog::DoModal()
{
	HGLOBAL hgbl;
	LPDLGTEMPLATE lpdt;
	LPDLGITEMTEMPLATE lpdit;
	LPWORD lpw;
	LPWSTR lpwsz;
	LRESULT ret;
	int nchar;

	hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
	if (!hgbl)
		return -1;

	lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);

	// Define a dialog box.

	lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU
		| DS_MODALFRAME | WS_CAPTION;
	lpdt->cdit = 3;  // number of controls
	lpdt->x = 10;
	lpdt->y = 10;
	lpdt->cx = 100;
	lpdt->cy = 70;

	lpw = (LPWORD)(lpdt + 1);
	*lpw++ = 0;   // no menu
	*lpw++ = 0;   // predefined dialog box class (by default)

	lpwsz = (LPWSTR)lpw;
	nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "内存对话框", -1,
		lpwsz, 50);
	lpw += nchar;

	//-----------------------
	// Define an OK button.
	//-----------------------
	lpw = lpwAlign(lpw);

	lpdit = (LPDLGITEMTEMPLATE)lpw;
	lpdit->x = 10;
	lpdit->y = 50;
	lpdit->cx = 80;
	lpdit->cy = 15;
	lpdit->id = IDOK;  // OK button identifier
	lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0080;    // button class

	lpwsz = (LPWSTR)lpw;
	nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "退出", -1, lpwsz, 50);
	lpw += nchar;
	*lpw++ = 0;              // no creation data


							 //-----------------------
							 // Define a Help button.
							 //-----------------------
	lpw = lpwAlign(lpw);

	lpdit = (LPDLGITEMTEMPLATE)lpw;
	lpdit->x = 10;
	lpdit->y = 30;
	lpdit->cx = 80;
	lpdit->cy = 15;
	lpdit->id = ID_HELP;    // Help button identifier
	lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0080;                 // button class atom

	lpwsz = (LPWSTR)lpw;
	nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "显示输入", -1, lpwsz, 50);
	lpw += nchar;
	*lpw++ = 0;                      // no creation data


									 //-----------------------
									 // Define a EDIT.
									 //-----------------------
	lpw = lpwAlign(lpw);

	lpdit = (LPDLGITEMTEMPLATE)lpw;
	lpdit->x = 10;
	lpdit->y = 10;
	lpdit->cx = 80;
	lpdit->cy = 12;
	lpdit->id = ID_EDIT;    // Help button identifier
	lpdit->style = ES_LEFT | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE;

	lpw = (LPWORD)(lpdit + 1);
	*lpw++ = 0xFFFF;
	*lpw++ = 0x0081;                 // edit class atom
	*lpw++ = 0;                      // no creation data

	GlobalUnlock(hgbl);

	ret = DialogBoxIndirect(m_hInst,
		(LPDLGTEMPLATE)hgbl,
		m_hOwnerWnd,
		(DLGPROC)DialogProc);

	GlobalFree(hgbl);
	return ret;
}
*/
int nCopyAnsiToWideChar(LPWORD lpWCStr, LPSTR lpAnsiIn)
{
	int cchAnsi = lstrlenA(lpAnsiIn);

	return MultiByteToWideChar(GetACP(), MB_PRECOMPOSED, lpAnsiIn, cchAnsi, (LPWSTR)lpWCStr, cchAnsi) + 1;
}

INT_PTR KDialog::DoModal() {
	WORD  *p, *pdlgtemplate;
	int   nchar;
	DWORD lStyle;


	/* allocate some memory to play with  */
	pdlgtemplate = p = (PWORD)LocalAlloc(LPTR, 1000);


	/* start to fill in the dlgtemplate information.  addressing by WORDs */
	lStyle = DS_MODALFRAME | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;

	*p++ = LOWORD(lStyle);
	*p++ = HIWORD(lStyle);
	*p++ = 0;          // LOWORD (lExtendedStyle)
	*p++ = 0;          // HIWORD (lExtendedStyle)
	*p++ = 1;          // NumberOfItems
	*p++ = 10;         // x
	*p++ = 10;         // y
	*p++ = 100;        // cx
	*p++ = 100;        // cy
	*p++ = 0;          // Menu
	*p++ = 0;          // Class

					   /* copy the title of the dialog */
	if (PRIMARYLANGID(GetUserDefaultLangID()) == LANG_JAPANESE)
		nchar = nCopyAnsiToWideChar(p, "Application 1");
	else
		nchar = nCopyAnsiToWideChar(p, "Title 1");
	p += nchar;

	/* add in the wPointSize and szFontName here iff the DS_SETFONT bit on */

	/* make sure the first item starts on a DWORD boundary */
	p = lpwAlign(p);


	/* now start with the first item */
	lStyle = BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD;

	*p++ = LOWORD(lStyle);
	*p++ = HIWORD(lStyle);
	*p++ = 0;          // LOWORD (lExtendedStyle)
	*p++ = 0;          // HIWORD (lExtendedStyle)
	*p++ = 10;         // x
	*p++ = 70;         // y
	*p++ = 80;         // cx
	*p++ = 20;         // cy
	*p++ = IDOK;       // ID

					   /* fill in class i.d. Button in this case */
	*p++ = (WORD)0xffff;
	*p++ = (WORD)0x0080;

	/* copy the text of the first item */
	nchar = nCopyAnsiToWideChar(p, "OK");
	p += nchar;

	*p++ = 0;  // advance pointer over nExtraStuff WORD

	CreateDialogIndirect(m_hInst, (LPDLGTEMPLATE)pdlgtemplate, m_hOwnerWnd, (DLGPROC)DialogProc);

	LocalFree(LocalHandle(pdlgtemplate));

	return 0;
}