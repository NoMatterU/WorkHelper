
// WorkHelper.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWorkHelperApp:
// See WorkHelper.cpp for the implementation of this class
//

class CWorkHelperApp : public CWinApp
{
public:
	CWorkHelperApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation

	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hMutex;
};

extern CWorkHelperApp theApp;