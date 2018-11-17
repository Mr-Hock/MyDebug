#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"


// MyImportTabDlg �Ի���

class MyImportTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyImportTabDlg)

public:
	MyImportTabDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyImportTabDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORTTAB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	ClistCtrlEx m_List_Import_Dll;
	ClistCtrlEx m_List_Import_Function;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
