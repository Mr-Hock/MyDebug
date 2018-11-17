// ConditionBreakDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "ConditionBreakDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"

extern BOOL g_OK_ConditionBreakPoint;
extern CONDITION g_RecvCondition;

// ConditionBreakDlg 对话框

IMPLEMENT_DYNAMIC(ConditionBreakDlg, CDialogEx)

ConditionBreakDlg::ConditionBreakDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONDITIONBREAK_DIALOG, pParent)
{

}

ConditionBreakDlg::~ConditionBreakDlg()
{
}

void ConditionBreakDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_Check_Eax);
	DDX_Control(pDX, IDC_CHECK2, m_Check_Ebx);
	DDX_Control(pDX, IDC_CHECK3, m_Check_Ecx);
	DDX_Control(pDX, IDC_CHECK4, m_Check_Edx);
	DDX_Control(pDX, IDC_CHECK5, m_Check_Esi);
	DDX_Control(pDX, IDC_CHECK6, m_Check_Edi);
	DDX_Control(pDX, IDC_CHECK7, m_Check_Ebp);
	DDX_Control(pDX, IDC_CHECK8, m_Check_Esp);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Eax);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_Ebx);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_Ecx);
	DDX_Control(pDX, IDC_EDIT4, m_Edit_Edx);
	DDX_Control(pDX, IDC_EDIT5, m_Edit_Esi);
	DDX_Control(pDX, IDC_EDIT34, m_Edit_Edi);
	DDX_Control(pDX, IDC_EDIT35, m_Edit_Ebp);
	DDX_Control(pDX, IDC_EDIT6, m_Edit_Esp);
}


BEGIN_MESSAGE_MAP(ConditionBreakDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ConditionBreakDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ConditionBreakDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// ConditionBreakDlg 消息处理程序


BOOL ConditionBreakDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;
}


BOOL ConditionBreakDlg::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnBnClickedButton1();
		return FALSE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void ConditionBreakDlg::OnBnClickedButton1()
{
	CString szValue;
	DWORD dwValue;

	if (m_Check_Eax.GetCheck())
	{
		m_Edit_Eax.GetWindowText(szValue);
		if (szValue!="")
		{
			_stscanf_s(szValue, TEXT("%x"), &dwValue);
			g_RecvCondition.OPEN_EAX = TRUE;
			g_RecvCondition.EAX = dwValue;
		}
	}
	if (m_Check_Ebx.GetCheck())
	{
		m_Edit_Ebx.GetWindowText(szValue);
		if (szValue != "")
		{
			_stscanf_s(szValue, TEXT("%x"), &dwValue);
			g_RecvCondition.OPEN_EBX = TRUE;
			g_RecvCondition.EBX = dwValue;
		}
	}
	if (m_Check_Ecx.GetCheck())
	{
		m_Edit_Ecx.GetWindowText(szValue);
		if (szValue != "")
		{
			_stscanf_s(szValue, TEXT("%x"), &dwValue);
			g_RecvCondition.OPEN_ECX = TRUE;
			g_RecvCondition.ECX = dwValue;
		}
	}
	if (m_Check_Edx.GetCheck())
	{
		m_Edit_Edx.GetWindowText(szValue);
		if (szValue != "")
		{
			_stscanf_s(szValue, TEXT("%x"), &dwValue);
			g_RecvCondition.OPEN_EDX = TRUE;
			g_RecvCondition.EDX = dwValue;
		}
	}
	if (m_Check_Esi.GetCheck())
	{
		m_Edit_Esi.GetWindowText(szValue);
		if (szValue != "")
		{
			_stscanf_s(szValue, TEXT("%x"), &dwValue);
			g_RecvCondition.OPEN_ESI = TRUE;
			g_RecvCondition.ESI = dwValue;
		}
	}
	if (m_Check_Edi.GetCheck())
	{
		m_Edit_Edi.GetWindowText(szValue);
		if (szValue != "")
		{
			_stscanf_s(szValue, TEXT("%x"), &dwValue);
			g_RecvCondition.OPEN_EDI = TRUE;
			g_RecvCondition.EDI = dwValue;
		}
	}
	if (m_Check_Ebp.GetCheck())
	{
		m_Edit_Ebp.GetWindowText(szValue);
		if (szValue != "")
		{
			_stscanf_s(szValue, TEXT("%x"), &dwValue);
			g_RecvCondition.OPEN_EBP = TRUE;
			g_RecvCondition.EBP = dwValue;
		}
	}
	if (m_Check_Esp.GetCheck())
	{
		m_Edit_Esp.GetWindowText(szValue);
		if (szValue != "")
		{
			_stscanf_s(szValue, TEXT("%x"), &dwValue);
			g_RecvCondition.OPEN_ESP = TRUE;
			g_RecvCondition.ESP = dwValue;
		}
	}

	g_OK_ConditionBreakPoint = TRUE;
	EndDialog(0);
}


void ConditionBreakDlg::OnBnClickedButton2()
{
	g_OK_ConditionBreakPoint = FALSE;
	EndDialog(0);
}
