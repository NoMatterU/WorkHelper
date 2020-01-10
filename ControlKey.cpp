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
				//消息文件已经读完了
				if (count != MAX_MSG_BUFFER) {
					iFinish = true;
					break;
				}
			}
			rdyLoad = false;
		}
		//1500个消息预计2s能发现
		Sleep(2000);
	}
#ifdef _DEBUG
	MessageBoxA(CWorkHelperDlg::hMain, "资源守护线程结束!", "提示", MB_OK | MB_ICONINFORMATION);
#endif
	return 0;
}

UINT __cdecl CControlKey::varControlKey(LPVOID lparam)
{
	if (!getInstance()->IsFileOpen()) return 0;

	MsgBuf *ptr = &data1;
	spaDat = &data2;
	unsigned int index = 0;

	//没有完成控键后的判断 Done
	while (iCtlKey) {
		//发送消息给目标窗口
		::SendMessage(CWorkHelperDlg::hTarget, 
			ptr->data[index].message, ptr->data[index].wParam, NULL);
		Sleep(ptr->data[index].Interval);

		//到达一半的数据，读入消息文件
		if (iCheck) if (index > MAX_MSG_BUFFER / 2) {
			rdyLoad = true;
			iCheck = false;
		}

		//双缓冲区数据切换
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
