#pragma once
#include "stdafx.h"

class CListenKey {
public:
	static CListenKey &getInstance() {
		static CListenKey l;
		return l;
	}

	bool Init();

	DWORD GetIntervalTime(DWORD nowTime);

	//add ���־�����ʾ
	void TextOutStatic(const char *text1, const char *text2 = NULL, const char *text3 = NULL);

	void ExitHook();

	bool CheckTime(DWORD curTime);

	bool IsBufFull();

	bool SaveMsg2File();

	void PushMsgBuf(USHORT nowTime, UINT message, WPARAM wParam, LPARAM lParam);

	void KeyStatInfo(DWORD vkCode, char *outstr, bool iKeyUp);

private:
	CListenKey() {};

	bool m_iFirst{ true };
	MyMSG m_msgBuf[MAX_BUF_SIZE]{ 0 };
	int m_msgIndex{ 0 };
	DWORD m_startTime{ 0 }, m_lastTime{ 0 };
	ofstream m_saveFile;
};