#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"


// SourceDlg �Ի���

class SourceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SourceDlg)

public:
	SourceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SourceDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOURCE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	ClistCtrlEx m_List_Source;
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListAsm(NMHDR *pNMHDR, LRESULT *pResult);
};
