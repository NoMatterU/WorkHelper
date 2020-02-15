#include "stdafx.h"
#include "ListenKey.h"

DWORD CListenKey::GetIntervalTime(DWORD nowTime)
{
	return nowTime - m_lastTime;
}

void CListenKey::UpdateTime(DWORD curTime) {
	m_lastTime = curTime;
}

//add 文字居中显示
void CListenKey::TextOutStatic(const char *text1, const char *text2, const char *text3) {
	char line1[60]{ 0 }, line2[60]{ 0 }, line3[60]{ 0 };
	memset(line1, 0x20, 59);
	memset(line2, 0x20, 59);
	memset(line3, 0x20, 59);

	if (text1 != NULL) {
		int offset = (60 - strlen(text1)) / 2;
		if(offset >= 0 && offset < 29)
			memcpy(line1 + offset, text1, strlen(text1));

		TextOutA(CWorkHelperDlg::hDC, 60, 10, line1, 60);
	}
	if (text2 != NULL) {
		int offset = (60 - strlen(text2)) / 2;
		if (offset >= 0 && offset < 29)
			memcpy(line2 + offset, text2, strlen(text2));

		TextOutA(CWorkHelperDlg::hDC, 60, 40, line2, 60);
	}
	if (text3 != NULL) {
		int offset = (60 - strlen(text3)) / 2;
		if (offset >= 0 && offset < 29)
			memcpy(line3 + offset, text3, strlen(text3));

		TextOutA(CWorkHelperDlg::hDC, 60, 70, line3, 60);
	}
}

void CListenKey::ExitHook() {
//	memset(m_msgBuf, 0, sizeof(MyMSG) * MAX_BUF_SIZE);
	m_iFirst = true;
	m_msgIndex = 0;
	m_startTime = 0;
	m_lastTime = 0;
//	return UnhookWindowsHookEx(CWorkHelperDlg::hHook);
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
	if (m_isOpen)
		if (m_msgIndex == 0) return true;
		else if (m_msgIndex <= MAX_BUF_SIZE && m_msgIndex > 0) {
			m_saveFile.Write(m_msgBuf, sizeof(MyMSG)*m_msgIndex);
//			memset(m_msgBuf, 0, sizeof(MyMSG) * MAX_BUF_SIZE);
			m_msgIndex = 0;
			return true;
		}
	return false;
}

void CListenKey::PushMsgBuf(DWORD nowTime, UINT message, WPARAM wParam, LPARAM lParam) {
	MyMSG msg{ nowTime - m_lastTime, message, wParam, lParam };
	if (m_msgIndex < MAX_BUF_SIZE) {
		m_msgBuf[m_msgIndex] = msg;
		m_msgIndex++;
	}
//	m_lastTime = nowTime;
}

BOOL CListenKey::KeyStatInfo(DWORD vkCode, char *outstr, bool iKeyUp) {
	//增加一个空格
	if ((vkCode >= 'A' && vkCode <= 'Z') || (vkCode >= '0' && vkCode <= '9')) {
		if (iKeyUp) sprintf_s(outstr, MAX_TEXT_SIZE, "   当前 %c 键弹起   ", vkCode);
		else sprintf_s(outstr, MAX_TEXT_SIZE, "   当前 %c 键按下   ", vkCode);
		return true;
	}
	else if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9) {
		if (iKeyUp) sprintf_s(outstr, MAX_TEXT_SIZE, "   当前 %c 键弹起   ", 48 + vkCode - 96);
		else sprintf_s(outstr, MAX_TEXT_SIZE, "   当前 %c 键按下   ", 48 + vkCode - 96);
		return true;
	}
	else if (vkCode == VK_SPACE) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "当前 空格键 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "当前 空格键 键按下");
		return true;
	}
	else if (vkCode == VK_LEFT) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "分向键← 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "分向键← 键按下");
		return true;
	}
	else if (vkCode == VK_UP) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "分向键↑ 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "分向键↑ 键按下");
		return true;
	}
	else if (vkCode == VK_RIGHT) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "分向键→ 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "分向键→ 键按下");
		return true;
	}
	else if (vkCode == VK_DOWN) {
		if (iKeyUp) strcpy_s(outstr, MAX_TEXT_SIZE, "分向键↓ 键弹起");
		else strcpy_s(outstr, MAX_TEXT_SIZE, "分向键↓ 键按下");
		return true;
	}
	return false;
}
