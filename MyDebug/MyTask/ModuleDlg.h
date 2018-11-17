#pragma once
#include "afxcmn.h"
#include "ClistCtrlEx.h"
#include "MyHead.h"


// ModuleDlg �Ի���

class ModuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ModuleDlg)

public:
	ModuleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ModuleDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODULE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_Pid;
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Module;
	VOID PrintModuleList(MODULELIST &nModuleList, DWORD nTaskNum);
	afx_msg void OnDblclkListHeap(NMHDR *pNMHDR, LRESULT *pResult);
	DWORD GetImageBassSize(DWORD nImageBass);
};
