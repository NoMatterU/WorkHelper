#pragma once
#include "stdafx.h"

#define MAX_MSG_BUFFER 3000

#ifndef WIN_MSG
#define WIN_MSG

struct MyMSG {
	USHORT Interval;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};
#endif

#ifndef MSG_BUFFER
#define MSG_BUFFER

struct MsgBuf {
	MyMSG data[MAX_MSG_BUFFER];
	unsigned int index;
};
#endif // !MSG_BUFFER

class CControlKey {
public:
	static CControlKey *getInstance() {
		static CControlKey ck;
		return &ck;
	}
public:
	BOOL InitFile(const WCHAR *filename){
		if (m_isOpen) {
			msgFile.Close();
			memset(&data1, 0, sizeof(MsgBuf));
			memset(&data2, 0, sizeof(MsgBuf));
			spaDat = &data1;
			iFinish = false;
			iCtlKey = true;
		}
		m_isOpen = msgFile.Open(filename, CFile::modeRead | CFile::typeBinary);
		if (!m_isOpen) return false;
//		m_FileLen = msgFile.GetLength();
		int ByteNum = msgFile.Read(data1.data, MAX_MSG_BUFFER * sizeof(MyMSG));
		if (ByteNum <= 0) return false;
		else if (ByteNum < MAX_MSG_BUFFER * sizeof(MyMSG)) {
			data1.index = ByteNum / sizeof(MyMSG);
			iFinish = true;
		}
		return true;
	}

	static UINT __cdecl LoadMsgBuffer(LPVOID lparam);
	static UINT __cdecl varControlKey(LPVOID lparam);

	BOOL IsFileOpen() { return m_isOpen; }

	VOID StopCtlKey() { iCtlKey = false; }

	UINT ReadFile() { 
		const int count = msgFile.Read(spaDat->data, MAX_MSG_BUFFER * sizeof(MyMSG));
		return  count / sizeof(MyMSG);
	}
private:
	CControlKey() {
		iCtlKey = true;
	}

	~CControlKey() {
		if(m_isOpen) msgFile.Close();
	}

	CFile msgFile;
	BOOL m_isOpen = false;

	static BOOL iCtlKey;
	//标志是否结束
	static BOOL iFinish;

	//指向空闲的数据buf
	static MsgBuf *spaDat;
	//双数据buf
	static MsgBuf data1;
	static MsgBuf data2;
};