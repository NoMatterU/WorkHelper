#pragma once
#include "stdafx.h"
#include "WorkHelperDlg.h"

class CListenKey {
public:
	static CListenKey &getInstance() {
		static CListenKey l;
		return l;
	}

	bool LoadFile(const WCHAR *filepath) {
		//每次文件打开清空文件内容
		if (m_isOpen) {
			m_saveFile.Close();
			m_isOpen = false;
		}
		if (filepath == L"" || filepath == NULL || lstrlenW(filepath) > 256) return false;
		m_isOpen = m_saveFile.Open(filepath, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate);
		if (!m_isOpen) return false;
//		m_saveFile.Flush();
		return true;
	}

	DWORD GetIntervalTime(DWORD nowTime);

	//add 文字居中显示 Dis
	void TextOutStatic(const char *text1, const char *text2 = NULL, const char *text3 = NULL);

	void ExitHook();

	bool CheckTime(DWORD curTime);

	void UpdateTime(DWORD curTime);

	bool IsBufFull();

	bool SaveMsg2File();

	void PushMsgBuf(DWORD nowTime, UINT message, WPARAM wParam, LPARAM lParam);

	BOOL KeyStatInfo(DWORD vkCode, char *outstr, bool iKeyUp);

	void CleanTextOut() {
		RECT rect{ 0 };
		GetWindowRect(m_hStatic, &rect);
		InvalidateRect(m_hStatic, CRect(0, 0, rect.right - rect.left, rect.bottom - rect.top), false);
		UpdateWindow(m_hStatic);
	}
private:
	CListenKey() {
		m_hStatic = FindWindowEx(CWorkHelperDlg::hMain, NULL, L"STATIC", NULL);
	};

	~CListenKey() {
		if(m_isOpen) m_saveFile.Close();
	}

	HWND m_hStatic;
	bool m_iFirst{ true };
	BOOL m_isOpen = false;
	MyMSG m_msgBuf[MAX_BUF_SIZE]{ 0 };
	int m_msgIndex{ 0 };
	DWORD m_startTime{ 0 }, m_lastTime{ 0 };
	CFile m_saveFile;
};