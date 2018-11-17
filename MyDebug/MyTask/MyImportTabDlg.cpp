// MyImportTabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyImportTabDlg.h"
#include "afxdialogex.h"
#include "PE.h"

extern PE MyPe;
// MyImportTabDlg 对话框

IMPLEMENT_DYNAMIC(MyImportTabDlg, CDialogEx)

MyImportTabDlg::MyImportTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMPORTTAB_DIALOG, pParent)
{

}

MyImportTabDlg::~MyImportTabDlg()
{
}

void MyImportTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List_Import_Dll);
	DDX_Control(pDX, IDC_LIST2, m_List_Import_Function);
}


BEGIN_MESSAGE_MAP(MyImportTabDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &MyImportTabDlg::OnClickList1)
END_MESSAGE_MAP()


// MyImportTabDlg 消息处理程序


BOOL MyImportTabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_Import_Dll.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Import_Function.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	m_List_Import_Dll.InsertColumn(0, TEXT("DLL名字"), 0, 200);
	m_List_Import_Dll.InsertColumn(1, TEXT("INT(RVA)"), 0, 120);
	m_List_Import_Dll.InsertColumn(2, TEXT("IAT(RVA)"), 0, 120);

	m_List_Import_Function.InsertColumn(0, TEXT("函数名字"), 0, 200);
	m_List_Import_Function.InsertColumn(1, TEXT("Thunk Value"), 0, 120);
	m_List_Import_Function.InsertColumn(2, TEXT("HINT"), 0, 120);
	//m_List_Import_Function.InsertColumn(3, TEXT("RVA"), 0, 120);


	if (MyPe.Is32Pe())
	{
		MyPe.PrintImportDll_32(m_List_Import_Dll);
	}
	else
	{
		MyPe.PrintImportDll_64(m_List_Import_Dll);
	}

	return TRUE;
}


void MyImportTabDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)return;
	if (MyPe.Is32Pe())
	{
		MyPe.PrintImportFunction_32(m_List_Import_Function, pNMItemActivate->iItem);
	}
	else
	{
		MyPe.PrintImportFunction_64(m_List_Import_Function, pNMItemActivate->iItem);
	}

}
