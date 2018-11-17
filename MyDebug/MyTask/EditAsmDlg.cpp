// EditAsmDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "EditAsmDlg.h"
#include "afxdialogex.h"

extern DWORD g_SendAddress_Asm;
extern BYTE g_RecvBytes_Asm[256];
extern DWORD g_RecvBytesNum_Asm;
extern CString g_SendAsm_Asm;
extern DWORD g_SendBytesNum_Asm;
extern BOOL g_Ok_Asm;


// EditAsmDlg 对话框

IMPLEMENT_DYNAMIC(EditAsmDlg, CDialogEx)

EditAsmDlg::EditAsmDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EDITASM_DIALOG, pParent)
{

}

EditAsmDlg::~EditAsmDlg()
{
}

void EditAsmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Asm);
}

BOOL EditAsmDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_RecvBytesNum_Asm = 0;
	ZeroMemory(g_RecvBytes_Asm,_countof(g_RecvBytes_Asm));

	g_SendAsm_Asm.Replace(TEXT("h"), TEXT(""));
	m_Edit_Asm.SetWindowText(g_SendAsm_Asm);
	return TRUE;
}

BEGIN_MESSAGE_MAP(EditAsmDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &EditAsmDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// EditAsmDlg 消息处理程序


void EditAsmDlg::OnBnClickedButton1()
{
	CHAR nAsm[250]{};
	CString nTemp1;
	CStringA nTemp2;
	m_Edit_Asm.GetWindowText(nTemp1);
	nTemp2 = nTemp1;

	strcpy_s(nAsm, 250, nTemp2);
	
	if (!AsmToOpcaode(g_SendAddress_Asm, nAsm, g_RecvBytes_Asm, g_RecvBytesNum_Asm))
	{
		MessageBox(TEXT("指令错误！"),TEXT("提示"),MB_ICONERROR);
		return;
	}

	if (g_RecvBytesNum_Asm < g_SendBytesNum_Asm)
	{
		//DWORD nPathNum = g_SendBytesNum - g_RecvBytesNum;
		for (DWORD i = g_RecvBytesNum_Asm; i <= g_SendBytesNum_Asm; i++)
		{
			g_RecvBytes_Asm[i] = 144;
		}
		g_RecvBytesNum_Asm = g_SendBytesNum_Asm;
	}

	g_SendAsm_Asm = nTemp1;

	g_Ok_Asm = TRUE;
	EndDialog(0);

}

BOOL EditAsmDlg::AsmToOpcaode(DWORD nAddress, CHAR *nAsm, LPBYTE nBytes, DWORD &nByteNum)
{
	XEDPARSE nXedparse = { 0 };

	nXedparse.cip = (ULONGLONG)nAddress;
	strcpy_s(nXedparse.instr, XEDPARSE_MAXBUFSIZE, nAsm);

	if (XEDParseAssemble(&nXedparse) != XEDPARSE_OK)return FALSE;

	nByteNum = nXedparse.dest_size;

	memcpy_s(nBytes, nXedparse.dest_size, nXedparse.dest, nXedparse.dest_size);

	return TRUE;
}


BOOL EditAsmDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == m_Edit_Asm.m_hWnd)OnBnClickedButton1();
		return FALSE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
