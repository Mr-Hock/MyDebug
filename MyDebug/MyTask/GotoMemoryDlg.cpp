// GotoMemoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyHead.h"
#include "GotoMemoryDlg.h"
#include "afxdialogex.h"
#include "DebugDlg.h"

extern DWORD g_SendAddress_Memory;
extern DWORD g_RecvAddress_Memory;
extern BOOL g_Ok_Memory;
extern DebugDlg* g_pDlg;
// GotoMemoryDlg 对话框

IMPLEMENT_DYNAMIC(GotoMemoryDlg, CDialogEx)

GotoMemoryDlg::GotoMemoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GOTOMEMORY_DIALOG, pParent)
{

}

GotoMemoryDlg::~GotoMemoryDlg()
{
}

void GotoMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Address);
}


BEGIN_MESSAGE_MAP(GotoMemoryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &GotoMemoryDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


BOOL GotoMemoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_RecvAddress_Memory = 0;
	CString nTemp;
	nTemp.Format(TEXT("%08X"), g_SendAddress_Memory);
	m_Edit_Address.SetWindowText(nTemp);


	return TRUE;
}


void GotoMemoryDlg::OnBnClickedButton1()
{
	CString szAddress;
	CString nTemp;
	m_Edit_Address.GetWindowText(szAddress);

	if (szAddress == "peb" || szAddress == "PEB")
	{
		g_RecvAddress_Memory = GetPebAddress(g_pDlg->m_ProcessHandle);
	}
	else if (szAddress == "eax")
	{
		g_pDlg->m_Edit_Eax.GetWindowText(nTemp);
		_stscanf_s(nTemp, TEXT("%X"), &g_RecvAddress_Memory);
	}
	else if (szAddress == "ebx")
	{
		g_pDlg->m_Edit_Ebx.GetWindowText(nTemp);
		_stscanf_s(nTemp, TEXT("%X"), &g_RecvAddress_Memory);
	}
	else if (szAddress == "ecx")
	{
		g_pDlg->m_Edit_Ecx.GetWindowText(nTemp);
		_stscanf_s(nTemp, TEXT("%X"), &g_RecvAddress_Memory);
	}
	else if (szAddress == "edx")
	{
		g_pDlg->m_Edit_Edx.GetWindowText(nTemp);
		_stscanf_s(nTemp, TEXT("%X"), &g_RecvAddress_Memory);
	}
	else if (szAddress == "esi")
	{
		g_pDlg->m_Edit_Esi.GetWindowText(nTemp);
		_stscanf_s(nTemp, TEXT("%X"), &g_RecvAddress_Memory);
	}
	else if (szAddress == "edi")
	{
		g_pDlg->m_Edit_Edi.GetWindowText(nTemp);
		_stscanf_s(nTemp, TEXT("%X"), &g_RecvAddress_Memory);
	}
	else if (szAddress == "ebp")
	{
		g_pDlg->m_Edit_Ebp.GetWindowText(nTemp);
		_stscanf_s(nTemp, TEXT("%X"), &g_RecvAddress_Memory);
	}
	else if (szAddress == "esp")
	{
		g_pDlg->m_Edit_Esp.GetWindowText(nTemp);
		_stscanf_s(nTemp, TEXT("%X"), &g_RecvAddress_Memory);
	}
	else
	{
		_stscanf_s(szAddress, TEXT("%X"), &g_RecvAddress_Memory);
	}

	if (!g_RecvAddress_Memory)return;

	g_Ok_Memory = TRUE;
	EndDialog(0);
	
}

BOOL GotoMemoryDlg::PreTranslateMessage(MSG* pMsg)
{


	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == m_Edit_Address.m_hWnd)OnBnClickedButton1();
		return FALSE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
