#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"
#include "MyHead.h"
#include "vector"

using std::vector;


// TableDlg 对话框

class TableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TableDlg)

public:
	TableDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TableDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Module;
	ClistCtrlEx m_List_Table;

	void PrintModuleList(MODULELIST &nModuleList, DWORD nTaskNum);
	VOID GetTable(CString nModuleName, DWORD nImageBass, DWORD nSize);
	DWORD GetImageBassSize(DWORD nImageBass);

	afx_msg void OnClickListModule(NMHDR *pNMHDR, LRESULT *pResult);
};
