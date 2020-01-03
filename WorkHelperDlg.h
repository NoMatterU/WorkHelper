
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

private:
	HWND m_hWnd;
	HCURSOR m_hArrow;//{ m_hArrow = GetCursor() };
public:
	static HWND hwnd;
	static HDC hDC;
	static HHOOK hHook;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnEndHook(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedFinish();

	afx_msg void OnBnClickedStart2();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBnClickedFinish2();
};
