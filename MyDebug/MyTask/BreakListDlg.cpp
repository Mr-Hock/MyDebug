// BreakListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "BreakListDlg.h"
#include "afxdialogex.h"
#include "DebugDlg.h"
extern DebugDlg* g_pDlg;

// BreakListDlg 对话框

IMPLEMENT_DYNAMIC(BreakListDlg, CDialogEx)

BreakListDlg::BreakListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BREAKLIST_DIALOG, pParent)
{

}

BreakListDlg::~BreakListDlg()
{
}

void BreakListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BREAKLIST, m_List_Break);
}


BEGIN_MESSAGE_MAP(BreakListDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BREAKLIST, &BreakListDlg::OnDblclkListBreaklist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BREAKLIST, &BreakListDlg::OnRclickListBreaklist)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_BREAKLIST, &BreakListDlg::OnKeydownListBreaklist)
END_MESSAGE_MAP()


// BreakListDlg 消息处理程序


BOOL BreakListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_Break.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Break.InsertColumn(0, TEXT("断点地址"), 0, 100);
	m_List_Break.InsertColumn(1, TEXT("断点类型"), 0, 150);

	CString nTemp;
	for (DWORD i = 0;i<g_pDlg->m_BreakPoint.size();i++)
	{
		nTemp.Format(TEXT("%08X"), g_pDlg->m_BreakPoint[i].nAddress);
		m_List_Break.InsertItem(i,nTemp);
		m_List_Break.SetItemText(i,1, TEXT("软件断点"));
	}
	
	for (DWORD i = 0; i < _countof(g_pDlg->m_HardBreakPoint); i++)
	{
		if (g_pDlg->m_HardBreakPoint[i].nAddress)
		{
			nTemp.Format(TEXT("%08X"), g_pDlg->m_HardBreakPoint[i].nAddress);
			m_List_Break.InsertItem(i, nTemp);
			m_List_Break.SetItemText(i, 1, TEXT("硬件断点"));
		}
	}

	if (g_pDlg->HaveMemoryBreakPoint())
	{
		nTemp.Format(TEXT("%08X"), g_pDlg->m_MemoryBreakPoint.nAddress);
		m_List_Break.InsertItem(m_List_Break.GetItemCount(), nTemp);
		m_List_Break.SetItemText(m_List_Break.GetItemCount() - 1, 1, TEXT("内存断点"));
	}

	return TRUE;
}

void BreakListDlg::OnDblclkListBreaklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;



	DWORD nItem = pNMItemActivate->iItem;
	DWORD nSubItem = pNMItemActivate->iSubItem;

	if (nItem == -1)return;

	CString szBreakAddress;
	DWORD dwBreakAddress;
	szBreakAddress = m_List_Break.GetItemText(nItem, 0);

	_stscanf_s(szBreakAddress, TEXT("%X"), &dwBreakAddress);

	g_pDlg->InitAsm(dwBreakAddress);
	g_pDlg->m_List_Asm.SetFocus();
	g_pDlg->m_List_Asm.SetSelectionEx(0);

	EndDialog(0);
}


void BreakListDlg::OnRclickListBreaklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	DWORD nItem = pNMItemActivate->iItem;
	DWORD nSubItem = pNMItemActivate->iSubItem;

	if (nItem == -1)return;

	CString nBreakType;
	CString szBreakAddress;
	DWORD dwBreakAddress;
	szBreakAddress = m_List_Break.GetItemText(nItem, 0);

	_stscanf_s(szBreakAddress, TEXT("%X"), &dwBreakAddress);

	nBreakType = m_List_Break.GetItemText(nItem, 1);

	if (nBreakType == CString("软件断点"))
	{
		g_pDlg->DelBreakPoint(dwBreakAddress);
	}
	else if (nBreakType == CString("硬件断点"))
	{
		g_pDlg->DelHardBreakPoint(dwBreakAddress);
	}
	else if (nBreakType == CString("内存断点"))
	{
		g_pDlg->DelMemoryBreakPoint(dwBreakAddress);
	}

	m_List_Break.DeleteItem(nItem);

	g_pDlg->WriteAllBreakInfo();

}


void BreakListDlg::OnKeydownListBreaklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	*pResult = 0;
}
