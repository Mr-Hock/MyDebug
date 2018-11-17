#pragma once
#include "afxwin.h"


// GotoMemoryDlg 对话框

class GotoMemoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GotoMemoryDlg)

public:
	GotoMemoryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GotoMemoryDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOTOMEMORY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit_Address;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
