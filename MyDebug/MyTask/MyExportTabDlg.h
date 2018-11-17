#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"


// MyExportTabDlg 对话框

class MyExportTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyExportTabDlg)

public:
	MyExportTabDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyExportTabDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORTTAB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Emport;
};
