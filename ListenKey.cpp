#include "stdafx.h"
#include "ListenKey.h"
#include "WorkHelperDlg.h"

DWORD CListenKey::GetIntervalTime(DWORD nowTime)
{
	return nowTime - m_lastTime;
}

bool CListenKey::Init() {
	m_saveFile.open("Journal.txt", ios_base::out);
	return true;
}

//add 文字居中显示
void CListenKey::TextOutStatic(const char *text1, const char *text2, const char *text3) {
	char line1[31]{ 0 }, line2[31]{ 0 }, line3[31]{ 0 };
	//	memset(line1, 0x20, 30);
	//	memset(line2, 0x20, 30);
	//	memset(line3, 0x20, 30);
	if (text1 != NULL) {
		strcpy_s(line1, text1);
		TextOutA(CWorkHelperDlg::hDC, 60, 25, line1, 30);
	}
	if (text2 != NULL) {
		strcpy_s(line2, text2);
		TextOutA(CWorkHelperDlg::hDC, 60, 55, line2, 30);
	}
	if (text3 != NULL) {
		strcpy_s(line3, text3);
		TextOutA(CWorkHelperDlg::hDC, 60, 85, line3, 30);
	}
}

void CListenKey::ExitHook() {
	memset(m_msgBuf, 0, sizeof(MyMSG) * MAX_BUF_SIZE);
	m_iFirst = true;
	m_msgIndex = 0;
	m_startTime = 0;
	m_lastTime = 0;
	UnhookWindowsHookEx(CWorkHelperDlg::hHook);
}

bool CListenKey::CheckTime(DWORD curTime) {
	if (m_iFirst) {
		m_startTime = curTime;
		m_lastTime = curTime;
		m_iFirst = false;
	}

	DWORD interTime = curTime - m_lastTime, totalTime = curTime - m_startTime;
	if (interTime < 65536 && interTime >= 0)
		if (totalTime < 0xFF000000 && totalTime >= 0) return true;
	return false;
}

bool CListenKey::IsBufFull() {
	if (m_msgIndex >= MAX_BUF_SIZE) return true;
	return false;
}

bool CListenKey::SaveMsg2File() {
	if (m_saveFile.is_open())
		if (m_msgIndex <= MAX_BUF_SIZE && m_msgIndex > 0)
			if (m_saveFile.write((char *)m_msgBuf, sizeof(MyMSG)*m_msgIndex)) {
//				memset(m_msgBuf, 0, sizeof(MyMSG) * MAX_BUF_SIZE);
				m_msgIndex = 0;
				return true;
			}
	return false;
}

void CListenKey::PushMsgBuf(USHORT nowTime, UINT message, WPARAM wParam, LPARAM lParam) {
	MyMSG msg{ nowTime - m_lastTime, message, wParam, lParam };
	if (m_msgIndex < MAX_BUF_SIZE) {
		m_msgBuf[m_msgIndex] = msg;
		m_msgIndex++;
	}

	m_lastTime = nowTime;
}

void CListenKey::KeyStatInfo(DWORD vkCode, char *outstr, bool iKeyUp) {
	if (vkCode >= 'A' && vkCode <= 'Z') {
		if (iKeyUp) sprintf_s(outstr, MAX_TEXT_SIZE, "   当前 %c 键弹起   ", vkCode);
		else sprintf_s(outstr, MAX_TEXT_SIZE, "   当前 %c 键按下   ", vkCode);
	}
	else if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9) {
		if (iKeyUp) sprintf_s(outstr, MAX_TEXT_SIZE, "   当前 %c 键弹起   ", 48 + vkCode - 96);
		else sprintf_s(outstr, MAX_TEXT_SIZE, "   当前 %c 键按下   ", 48 + vkCode - 96);
	}
	else if (vkCode == VK_LEFT) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "分向键← 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "分向键← 键按下");
	}
	else if (vkCode == VK_UP) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "分向键↑ 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "分向键↑ 键按下");
	}
	else if (vkCode == VK_RIGHT) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "分向键→ 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "分向键→ 键按下");
	}
	else if (vkCode == VK_DOWN) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "分向键↓ 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "分向键↓ 键按下");
	}
}
