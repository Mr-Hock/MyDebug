#pragma once
#include "afxwin.h"


// EditMemoryDlg �Ի���

class EditMemoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditMemoryDlg)

public:
	EditMemoryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EditMemoryDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITMEMORY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CEdit m_Edit_Address;
	CEdit m_Edit_Data;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
