#include "stdafx.h"
#include "ControlKey.h"
#include "WorkHelperDlg.h"


MsgBuf CControlKey::data1{ 0 };
MsgBuf CControlKey::data2{ 0 };
MsgBuf *CControlKey::spaDat = NULL;
BOOL CControlKey::iFinish = false;
BOOL iCheck = true, rdyLoad = false;
//HWND CWorkHelperDlg::hMain;

UINT __cdecl CControlKey::LoadMsgBuffer(LPVOID lparam)
{
	while (true) {
		if (rdyLoad) {
			int count = getInstance()->ReadFile();
			if (count < 0) spaDat->index = 0;
			else {
				spaDat->index = count;
				//��Ϣ�ļ��Ѿ�������
				if (count != MAX_MSG_BUFFER) {
					iFinish = true;
					break;
				}
			}
			rdyLoad = false;
		}
		Sleep(2000);
	}
	return 0;
}

UINT __cdecl CControlKey::varControlKey(LPVOID lparam)
{
	if (!getInstance()->IsFileOpen()) return 0;

	MsgBuf *ptr = &data1;
	spaDat = &data2;
	unsigned int index = 0;

	//û����ɿؼ�����ж� Done
	while (true) {
		//������Ϣ��Ŀ�괰��
		::SendMessage(CWorkHelperDlg::hTarget, 
			ptr->data[index].message, ptr->data[index].wParam, NULL);
		Sleep(ptr->data[index].Interval);

		//����һ������ݣ�������Ϣ�ļ�
		if (iCheck) if (index > ptr->index / 2) {
			rdyLoad = true;
			iCheck = false;
		}

		//˫�����������л�
		if (index < ptr->index) index++;
		else {
			if (iFinish) goto end;
			if (ptr == &data1) {
				ptr = &data2;
				spaDat = &data1;
			}
			else {
				ptr = &data1;
				spaDat = &data2;
			}
			index = 0;
			iCheck = true;
		}
	}
end:
	MessageBoxA(CWorkHelperDlg::hMain, "�ؼ�����!", "��ʾ", MB_OK | MB_ICONINFORMATION);
	return 0;
}
