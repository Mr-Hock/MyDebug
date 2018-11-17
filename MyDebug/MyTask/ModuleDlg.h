#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"
#include "MyHead.h"


// ModuleDlg 对话框

class ModuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ModuleDlg)

public:
	ModuleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ModuleDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODULE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_Pid;
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Module;
	VOID PrintModuleList(MODULELIST &nModuleList, DWORD nTaskNum);
	afx_msg void OnDblclkListHeap(NMHDR *pNMHDR, LRESULT *pResult);
	DWORD GetImageBassSize(DWORD nImageBass);
};
