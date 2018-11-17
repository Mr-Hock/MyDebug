// EditMemoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "EditMemoryDlg.h"
#include "afxdialogex.h"




extern DWORD g_SendAddress_Memory ;
extern CString g_SendBytes_Memory;
extern DWORD g_SendBytesNum_Memory ;
extern DWORD g_RecvBytes_Memory;
extern DWORD g_RecvBytesNum_Memory;
extern BOOL g_Ok_Memory;


IMPLEMENT_DYNAMIC(EditMemoryDlg, CDialogEx)

EditMemoryDlg::EditMemoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EDITMEMORY_DIALOG, pParent)
{

}

EditMemoryDlg::~EditMemoryDlg()
{
}

void EditMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Address);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_Data);
}


BEGIN_MESSAGE_MAP(EditMemoryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &EditMemoryDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// EditMemoryDlg 消息处理程序


BOOL EditMemoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString nAddress;
	nAddress.Format(TEXT("%08X"), g_SendAddress_Memory);
	m_Edit_Address.SetWindowText(nAddress);
	m_Edit_Data.SetWindowText(g_SendBytes_Memory);

	return TRUE;
}


void EditMemoryDlg::OnBnClickedButton1()
{
	CString nBytes;
	m_Edit_Data.GetWindowText(nBytes);

	_stscanf_s(nBytes, TEXT("%xc"), &g_RecvBytes_Memory);
	g_Ok_Memory = TRUE;
	EndDialog(0);

}


BOOL EditMemoryDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == m_Edit_Data.m_hWnd)OnBnClickedButton1();
		return FALSE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
