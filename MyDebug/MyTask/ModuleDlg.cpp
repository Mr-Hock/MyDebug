// ModuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "ModuleDlg.h"
#include "afxdialogex.h"
#include "DebugDlg.h"

extern DWORD g_Pid;
extern DWORD g_RecvAddress_Module;
extern DebugDlg* g_pDlg;

// ModuleDlg 对话框

IMPLEMENT_DYNAMIC(ModuleDlg, CDialogEx)

ModuleDlg::ModuleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MODULE_DIALOG, pParent)
{

}

ModuleDlg::~ModuleDlg()
{
}

void ModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HEAP, m_List_Module);
}


BEGIN_MESSAGE_MAP(ModuleDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HEAP, &ModuleDlg::OnDblclkListHeap)
END_MESSAGE_MAP()


// ModuleDlg 消息处理程序


BOOL ModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_RecvAddress_Module = 0;

	m_List_Module.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List_Module.InsertColumn(0, TEXT("模块名"), 0, 100);
	m_List_Module.InsertColumn(1, TEXT("模块路径"), 0, 330);
	m_List_Module.InsertColumn(2, TEXT("模块地址"), 0, 80);
	m_List_Module.InsertColumn(3, TEXT("模块大小"), 0, 80);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);


	MODULELIST nModuleList = {};
	DWORD nNum = EnumModuleList(nModuleList, g_Pid);//枚举进程列表
	PrintModuleList( nModuleList, nNum);//打印进程到列表
	ClearModuleList(nModuleList);

	return TRUE;
}
void ModuleDlg::PrintModuleList( MODULELIST &nModuleList, DWORD nTaskNum)
{

	for (DWORD i = 0; i < nTaskNum; i++)
	{
		DWORD nIndex = m_List_Module.GetItemCount();
		m_List_Module.InsertItem(nIndex, nModuleList.szModule[nIndex]);
		m_List_Module.SetItemText(nIndex, 1, nModuleList.szExePath[nIndex]);
		CString str;
		str.Format(L"%08X", nModuleList.modBaseAddr[nIndex]);
		m_List_Module.SetItemText(nIndex, 2, str);
		str.Format(L"%08X", GetImageBassSize(nModuleList.modBaseAddr[nIndex]));
		m_List_Module.SetItemText(nIndex, 3, str);
	}
}

void ModuleDlg::OnDblclkListHeap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	DWORD nItem = pNMItemActivate->iItem;
	DWORD nSubItem = pNMItemActivate->iSubItem;

	if (nItem == -1)return;

	CString nAddress;
	nAddress = m_List_Module.GetItemText(nItem, 2);
	_stscanf_s(nAddress, TEXT("%X"), &g_RecvAddress_Module);

	EndDialog(0);
}

DWORD ModuleDlg::GetImageBassSize(DWORD nImageBass)
{
	BYTE  nData[4096]{};

	g_pDlg->ReadMemoryBytes(nImageBass, nData, _countof(nData));

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS32 nNtHead = (PIMAGE_NT_HEADERS32)((LONG)nData + nDosHeade->e_lfanew);
	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	return nOptionalHead->SizeOfImage;
}
