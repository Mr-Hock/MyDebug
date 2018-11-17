#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"
#include "MyHead.h"
#include "vector"

using std::vector;


// TableDlg �Ի���

class TableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TableDlg)

public:
	TableDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TableDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ClistCtrlEx m_List_Module;
	ClistCtrlEx m_List_Table;

	void PrintModuleList(MODULELIST &nModuleList, DWORD nTaskNum);
	VOID GetTable(CString nModuleName, DWORD nImageBass, DWORD nSize);
	DWORD GetImageBassSize(DWORD nImageBass);

	afx_msg void OnClickListModule(NMHDR *pNMHDR, LRESULT *pResult);
};
