#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"


// SourceDlg 对话框

class SourceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SourceDlg)

public:
	SourceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SourceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOURCE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	ClistCtrlEx m_List_Source;
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListAsm(NMHDR *pNMHDR, LRESULT *pResult);
};
