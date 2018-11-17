#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"
#include <vector>
using std::vector;


// BreakListDlg 对话框

class BreakListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BreakListDlg)

public:
	BreakListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BreakListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BREAKLIST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List_Break;
	afx_msg void OnDblclkListBreaklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListBreaklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeydownListBreaklist(NMHDR *pNMHDR, LRESULT *pResult);
};
