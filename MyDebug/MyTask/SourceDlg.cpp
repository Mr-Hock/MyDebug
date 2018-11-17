// SourceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "SourceDlg.h"
#include "afxdialogex.h"
#include "DebugDlg.h"


extern DebugDlg* g_pDlg;

// SourceDlg 对话框

IMPLEMENT_DYNAMIC(SourceDlg, CDialogEx)

SourceDlg::SourceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SOURCE_DIALOG, pParent)
{
	Create(IDD_SOURCE_DIALOG);
}

SourceDlg::~SourceDlg()
{
}

void SourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ASM, m_List_Source);
}


BEGIN_MESSAGE_MAP(SourceDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ASM, &SourceDlg::OnDblclkListAsm)
END_MESSAGE_MAP()


// SourceDlg 消息处理程序


void SourceDlg::OnClose()
{
	ShowWindow(FALSE);
	return;

	CDialogEx::OnClose();
}


BOOL SourceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_List_Source.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Source.InsertColumn(0, TEXT(""), 0, 30);
	m_List_Source.InsertColumn(1, TEXT("行号"), 0, 50);
	m_List_Source.InsertColumn(2, TEXT("地址"), 0, 100);
	m_List_Source.InsertColumn(3, TEXT("源码"), 0, 500);

	for (DWORD i = 0; i <= 30; i++)m_List_Source.InsertItem(i, TEXT(""));


	return TRUE;
}


void SourceDlg::OnDblclkListAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;


	if (pNMItemActivate->iItem == -1)return;

	CString nSourceAddress;
	DWORD dwSourceAddress;
	CString nDebugAddress;

	nSourceAddress = m_List_Source.GetItemText(pNMItemActivate->iItem, 2);

	if (nSourceAddress == "")return;

	BOOL nSelectSuccess = FALSE;
	for (INT i = 0; i < g_pDlg->m_List_Asm.GetItemCount(); i++)
	{
		nDebugAddress = g_pDlg->m_List_Asm.GetItemText(i, g_pDlg->m_Flag_Address);
		if (nSourceAddress == nDebugAddress)
		{
			nSelectSuccess = TRUE;
			g_pDlg->SetFocus();
			g_pDlg->m_List_Asm.SetSelectionEx(i);
			g_pDlg->m_List_Asm.SetFocus();
			break;
		}
	}

	if (nSelectSuccess==FALSE)
	{
		_stscanf_s(nSourceAddress,TEXT("%x"),&dwSourceAddress);
		g_pDlg->InitAsm(dwSourceAddress);
		g_pDlg->SetFocus();
		g_pDlg->m_List_Asm.SetSelectionEx(0);
		g_pDlg->m_List_Asm.SetFocus();

	}


}
