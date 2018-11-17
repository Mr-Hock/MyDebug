
// MyTaskDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"
#include "StatusBarEx.h"
#include "afxwin.h"
#include "DebugDlg.h"


// CMyTaskDlg �Ի���
class CMyTaskDlg : public CDialogEx
{
// ����
public:
	CMyTaskDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYTASK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
