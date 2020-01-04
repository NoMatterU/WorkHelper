#pragma once
#include "stdafx.h"

#define MAX_MSG_BUFFER 65536

#ifndef WIN_MSG
#define WIN_MSG
struct MyMSG {
	USHORT Interval;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};
#endif

class CControlKey {
public:
	CControlKey() {};
	CControlKey(const WCHAR *filename) {
		isOpen = msgFile.Open(filename, CFile::modeRead | CFile::typeBinary);
	};
	~CControlKey() {
		msgFile.Close();
	};
public:
	VOID varControlKey();
private:
	CFile msgFile;
	BOOL isOpen = false;
	MyMSG data1[MAX_MSG_BUFFER]{ 0 };
	MyMSG data2[MAX_MSG_BUFFER]{ 0 };
};