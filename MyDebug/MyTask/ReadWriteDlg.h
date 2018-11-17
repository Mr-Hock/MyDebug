#pragma once
#include "afxcmn.h"


// ReadWriteDlg 对话框

class ReadWriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ReadWriteDlg)

public:
	ReadWriteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ReadWriteDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_READWRITE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_ReadWrite;
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListReadwrite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
