
// MyTaskDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"
#include "StatusBarEx.h"
#include "afxwin.h"
#include "DebugDlg.h"


// CMyTaskDlg 对话框
class CMyTaskDlg : public CDialogEx
{
// 构造
public:
	CMyTaskDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYTASK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HMODULE hModule;
	ClistCtrlEx m_List_Task;
	void Fun();
	void LoadSafeSkin();
	afx_msg void OnDblclkListTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnClose();
};
