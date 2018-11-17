#pragma once
#include "afxwin.h"


// ConditionBreakDlg 对话框

class ConditionBreakDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConditionBreakDlg)

public:
	ConditionBreakDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ConditionBreakDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONDITIONBREAK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CButton m_Check_Eax;
	CButton m_Check_Ebx;
	CButton m_Check_Ecx;
	CButton m_Check_Edx;
	CButton m_Check_Esi;
	CButton m_Check_Edi;
	CButton m_Check_Ebp;
	CButton m_Check_Esp;
	CEdit m_Edit_Eax;
	CEdit m_Edit_Ebx;
	CEdit m_Edit_Ecx;
	CEdit m_Edit_Edx;
	CEdit m_Edit_Esi;
	CEdit m_Edit_Edi;
	CEdit m_Edit_Ebp;
	CEdit m_Edit_Esp;
};
