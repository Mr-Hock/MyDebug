#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"


// MyExportTabDlg �Ի���

class MyExportTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyExportTabDlg)

public:
	MyExportTabDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyExportTabDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORTTAB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Emport;
};
