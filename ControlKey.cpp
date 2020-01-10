#include "stdafx.h"
#include "ControlKey.h"
#include "WorkHelperDlg.h"


MsgBuf CControlKey::data1{ 0 };
MsgBuf CControlKey::data2{ 0 };
MsgBuf *CControlKey::spaDat = NULL;
BOOL CControlKey::iFinish = false;
BOOL CControlKey::iCtlKey;
BOOL iCheck = true, rdyLoad = false;


UINT __cdecl CControlKey::LoadMsgBuffer(LPVOID lparam)
{
	while (iCtlKey) {
		if (rdyLoad) {
			int count = getInstance()->ReadFile();
			if (count < 0) {
				spaDat->index = 0;
				iFinish = true;
			}
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
		//1500����ϢԤ��2s�ܷ���
		Sleep(2000);
	}
#ifdef _DEBUG
	MessageBoxA(CWorkHelperDlg::hMain, "��Դ�ػ��߳̽���!", "��ʾ", MB_OK | MB_ICONINFORMATION);
#endif
	return 0;
}

UINT __cdecl CControlKey::varControlKey(LPVOID lparam)
{
	if (!getInstance()->IsFileOpen()) return 0;

	MsgBuf *ptr = &data1;
	spaDat = &data2;
	unsigned int index = 0;

	//û����ɿؼ�����ж� Done
	while (iCtlKey) {
		//������Ϣ��Ŀ�괰��
		::SendMessage(CWorkHelperDlg::hTarget, 
			ptr->data[index].message, ptr->data[index].wParam, NULL);
		Sleep(ptr->data[index].Interval);

		//����һ������ݣ�������Ϣ�ļ�
		if (iCheck) if (index > MAX_MSG_BUFFER / 2) {
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
	::SendMessage(CWorkHelperDlg::hMain, WM_CTLEND, NULL, NULL);
	iCtlKey = false;
	iCheck = true;
	rdyLoad = false;
	return 0;
}
