#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"


// MyImportTabDlg 对话框

class MyImportTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyImportTabDlg)

public:
	MyImportTabDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyImportTabDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORTTAB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ClistCtrlEx m_List_Import_Dll;
	ClistCtrlEx m_List_Import_Function;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
