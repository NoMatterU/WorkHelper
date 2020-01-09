#pragma once
#include "stdafx.h"

class CListenKey {
public:
	static CListenKey &getInstance() {
		static CListenKey l;
		return l;
	}

	DWORD GetIntervalTime(DWORD nowTime);

	//add Œƒ◊÷æ”÷–œ‘ æ Dis
	void TextOutStatic(const char *text1, const char *text2 = NULL, const char *text3 = NULL);

	void ExitHook();

	bool CheckTime(DWORD curTime);

	void UpdateTime(DWORD curTime);

	bool IsBufFull();

	bool SaveMsg2File();

	void PushMsgBuf(DWORD nowTime, UINT message, WPARAM wParam, LPARAM lParam);

	BOOL KeyStatInfo(DWORD vkCode, char *outstr, bool iKeyUp);

private:
	CListenKey() {
		m_saveFile.open("Journal.txt", ios_base::in | ios_base::out);
	};

	~CListenKey() {
		m_saveFile.close();
	}

	bool m_iFirst{ true };
	MyMSG m_msgBuf[MAX_BUF_SIZE]{ 0 };
	int m_msgIndex{ 0 };
	DWORD m_startTime{ 0 }, m_lastTime{ 0 };
	ofstream m_saveFile;
};