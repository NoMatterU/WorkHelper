#include "stdafx.h"
#include "ControlKey.h"
#include "WorkHelperDlg.h"

VOID CControlKey::varControlKey()
{
	if (!isOpen) return;
	unsigned int i = 0;
	MyMSG *ptr = data1;
	while (true) {
		//������Ϣ��Ŀ�괰��
		::SendMessage(CWorkHelperDlg::hTarget, ptr[i].message, ptr[i].wParam, ptr[i].lParam);
		Sleep(ptr[i].Interval);

		//˫�����������л�
		if (i < MAX_MSG_BUFFER) i++;
		else {
			if (ptr == data1) ptr = data2;
			else ptr = data1;
			i = 0;
		}
	}
	return;
}
