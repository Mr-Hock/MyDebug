// ReadWriteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "ReadWriteDlg.h"
#include "afxdialogex.h"
#include "DebugDlg.h"

extern DebugDlg* g_pDlg;

// ReadWriteDlg 对话框

IMPLEMENT_DYNAMIC(ReadWriteDlg, CDialogEx)

ReadWriteDlg::ReadWriteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_READWRITE_DIALOG, pParent)
{
	Create(IDD_READWRITE_DIALOG);
}

ReadWriteDlg::~ReadWriteDlg()
{
}

void ReadWriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ReadWrite, m_List_ReadWrite);
}


BEGIN_MESSAGE_MAP(ReadWriteDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ReadWrite, &ReadWriteDlg::OnDblclkListReadwrite)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ReadWriteDlg 消息处理程序


BOOL ReadWriteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_List_ReadWrite.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_ReadWrite.InsertColumn(0, TEXT("地址"), 0, 100);
	m_List_ReadWrite.InsertColumn(1, TEXT("汇编代码"), 0, 300);

	SetTimer(0x1000, 500, NULL);
	return TRUE;
}


void ReadWriteDlg::OnDblclkListReadwrite(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	DWORD nItem = pNMItemActivate->iItem;
	DWORD nSubItem = pNMItemActivate->iSubItem;

	if (nItem == -1)return;

	CString nTemp;
	DWORD nReadAddress;
	nTemp = m_List_ReadWrite.GetItemText(nItem, 0);
	_stscanf_s(nTemp, TEXT("%X"), &nReadAddress);

	g_pDlg->InitAsm(nReadAddress);
	g_pDlg->m_List_Asm.SetFocus();
	g_pDlg->m_List_Asm.SetSelectionEx(0);

}


void ReadWriteDlg::OnClose()
{
	CDialogEx::OnClose();

	DestroyWindow();
}


void ReadWriteDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (0x1000 == nIDEvent)
	{
		CString nAddress;

		DWORD nPath;
		if ((INT)g_pDlg->m_HardBreakReadWrite.size()>m_List_ReadWrite.GetItemCount())
		{
			nPath = g_pDlg->m_HardBreakReadWrite.size() - m_List_ReadWrite.GetItemCount();


			for (DWORD i = 0; i < nPath; i++)
			{
				m_List_ReadWrite.InsertItem(m_List_ReadWrite.GetItemCount(), TEXT(""));
			}
		}
		else if ((INT)g_pDlg->m_HardBreakReadWrite.size() < m_List_ReadWrite.GetItemCount())
		{
			nPath = m_List_ReadWrite.GetItemCount() - g_pDlg->m_HardBreakReadWrite.size();

			for (DWORD i = 0; i < nPath; i++)
			{
				m_List_ReadWrite.DeleteItem(m_List_ReadWrite.GetItemCount() - 1);
			}

		}

		for (DWORD i = 0; i < g_pDlg->m_HardBreakReadWrite.size(); i++)
		{
			nAddress.Format(TEXT("%08X"), g_pDlg->m_HardBreakReadWrite[i].nAddress);
			m_List_ReadWrite.SetItemText(i, 0, nAddress);
			m_List_ReadWrite.SetItemText(i, 1, g_pDlg->m_HardBreakReadWrite[i].nAsm);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
