
// WorkHelperDlg.h : header file
//

#pragma once
#include "stdafx.h"

// CWorkHelperDlg dialog
class CWorkHelperDlg : public CDialogEx
{
// Construction
public:
	CWorkHelperDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORKHELPER_DIALOG };
#endif

	static LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void EndDialog(int nResult);
	DECLARE_MESSAGE_MAP()

	//Myself Defined functions
	void FindMsgFile();

	//判断按下的键大小写
	static BOOL IsHCase() {
		BOOL cFlag = GetKeyState(VK_CAPITAL) & 0x0001;
		BOOL sFlag = GetKeyState(VK_SHIFT) & 0x8000;
		return cFlag ^ sFlag;
	};
	BOOL SetSelectMouse();
	BOOL UnSetSelectMouse();
private:
	CWinThread *m_pthread1 = NULL;
	CWinThread *m_pthread2 = NULL; 
public:
	static HWND hTarget;
	static HDC hDC;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedFinish();
	afx_msg LRESULT OnEndHook(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnControlKey(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedStart1();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedFinish1();
};
