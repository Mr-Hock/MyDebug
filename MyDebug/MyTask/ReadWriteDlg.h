#pragma once
#include "afxcmn.h"


// ReadWriteDlg �Ի���

class ReadWriteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ReadWriteDlg)

public:
	ReadWriteDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ReadWriteDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_READWRITE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List_ReadWrite;
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListReadwrite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
