// MyExportTabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyExportTabDlg.h"
#include "afxdialogex.h"
#include "PE.h"

extern PE MyPe;

// MyExportTabDlg 对话框

IMPLEMENT_DYNAMIC(MyExportTabDlg, CDialogEx)

MyExportTabDlg::MyExportTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPORTTAB_DIALOG, pParent)
{

}

MyExportTabDlg::~MyExportTabDlg()
{
}

void MyExportTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List_Emport);
}


BEGIN_MESSAGE_MAP(MyExportTabDlg, CDialogEx)
END_MESSAGE_MAP()


// MyExportTabDlg 消息处理程序


BOOL MyExportTabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_Emport.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_List_Emport.InsertColumn(0, TEXT("序号"), 0, 60);
	m_List_Emport.InsertColumn(1, TEXT("RVA"), 0, 100);
	m_List_Emport.InsertColumn(2, TEXT("FOA"), 0, 100);
	m_List_Emport.InsertColumn(3, TEXT("函数名称"), 0, 200);

	if (MyPe.Is32Pe())
	{
		MyPe.PrintExport_32(m_List_Emport);
	}
	else
	{
		MyPe.PrintExport_64(m_List_Emport);
	}

	return TRUE;
}
