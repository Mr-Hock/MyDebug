#pragma once
#include "afxwin.h"


// GotoMemoryDlg �Ի���

class GotoMemoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GotoMemoryDlg)

public:
	GotoMemoryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GotoMemoryDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOTOMEMORY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit_Address;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
