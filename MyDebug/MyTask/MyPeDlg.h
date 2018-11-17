#pragma once
#include "afxwin.h"

// MyPeDlg 对话框

class MyPeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyPeDlg)

public:
	MyPeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyPeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEINFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	void PrintPeTable_32();
	void MyPeDlg::PrintPeTable_64();

	afx_msg void OnDropFiles(HDROP hDropInfo);
	CEdit m_Edit_Info;
	CEdit m_Edit_Export_Addr;
	CEdit m_Edit_Export_Size;
	CEdit m_Edit_Import_Addr;
	CEdit m_Edit_Import_Size;
	CEdit m_Edit_Resource_Addr;
	CEdit m_Edit_Resource_Size;
	CEdit m_Edit_Exception_Addr;
	CEdit m_Edit_Exception_Size;
	CEdit m_Edit_Security_Addr;
	CEdit m_Edit_Security_Size;
	CEdit m_Edit_Relocation_Addr;
	CEdit m_Edit_Relocation_Size;
	CEdit m_Edit_Debug_Addr;
	CEdit m_Edit_Debug_Size;
	CEdit m_Edit_Architecture_Addr;
	CEdit m_Edit_Architecture_Size;
	CEdit m_Edit_Globalptr_Addr;
	CEdit m_Edit_Globalptr_Size;
	CEdit m_Edit_TLS_Addr;
	CEdit m_Edit_TLS_Size;
	CEdit m_Edit_Load_Addr;
	CEdit m_Edit_Load_Size;
	CEdit m_Edit_Bound_Addr;
	CEdit m_Edit_Bound_Size;
	CEdit m_Edit_IAT_Addr;
	CEdit m_Edit_IAT_Size;
	CEdit m_Edit_Delay_Addr;
	CEdit m_Edit_Delay_Size;
	CEdit m_Edit_COM_Addr;
	CEdit m_Edit_COM_Size;
	CEdit m_Edit_Save_Addr;
	CEdit m_Edit_Save_Size;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
