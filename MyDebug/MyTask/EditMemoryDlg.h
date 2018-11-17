#pragma once
#include "afxwin.h"


// EditMemoryDlg 对话框

class EditMemoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditMemoryDlg)

public:
	EditMemoryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EditMemoryDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITMEMORY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CEdit m_Edit_Address;
	CEdit m_Edit_Data;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
