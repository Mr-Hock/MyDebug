
// MyTaskDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyTaskDlg.h"
#include "afxdialogex.h"
#include "MyHead.h"
#include <windows.h>

DWORD g_Pid = 0;
BOOL g_OpenTraffic = FALSE;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMyTaskDlg::CMyTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYTASK_DIALOG, pParent)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASK, m_List_Task);
}

BEGIN_MESSAGE_MAP(CMyTaskDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TASK, &CMyTaskDlg::OnDblclkListTask)
	ON_WM_DROPFILES()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


//打印进程列表
void PrintTaskList(ClistCtrlEx &m_List_Task,TASKLIST &nTaskList, DWORD nTaskNum)
{
	HIMAGELIST nHiml = { 0 };//图标列表
	SHFILEINFO nPsfi = { 0 };//文件信息
	ImageList_Destroy(nHiml);//清除图标列表
	nHiml = ImageList_Create(16, 16, ILC_COLOR32, 0, 0);//订制图标框架
	ImageList_SetBkColor(nHiml, m_List_Task.GetBkColor());//设置图标列表底色
	m_List_Task.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//设置超级列表显示图标


	for (DWORD i = 0; i < nTaskNum; i++)
	{
		DWORD nIndex = m_List_Task.GetItemCount();

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//获取文件信息
		if (!SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			 SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON| SHGFI_USEFILEATTRIBUTES| SHGFI_SMALLICON);
		DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//置入图标返回图标下标

		m_List_Task.InsertItem(nIndex, nTaskList.szExeFile[nIndex],nIco);
		m_List_Task.SetItemText(nIndex, 1, nTaskList.th32ProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 2, nTaskList.szPriority[nIndex]);
		m_List_Task.SetItemText(nIndex, 3, nTaskList.WorkingSetSize[nIndex]);
		m_List_Task.SetItemText(nIndex, 4, nTaskList.cntThreads[nIndex]);
		m_List_Task.SetItemText(nIndex, 5, nTaskList.th32ParentProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 6, nTaskList.szExeFilePath[nIndex]);
	}

}
//刷新进程列表
void RefershTaskList(ClistCtrlEx &m_List_Task, TASKLIST &nTaskList, DWORD nTaskNum)
{
	static HIMAGELIST nHiml = { 0 };//图标列表
	static SHFILEINFO nPsfi = { 0 };//文件信息
	//ImageList_Destroy(nHiml);//清除图标列表
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(16, 16, ILC_COLOR32, 0, 0);//订制图标框架
		ImageList_SetBkColor(nHiml, m_List_Task.GetBkColor());//设置图标列表底色
		m_List_Task.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//设置超级列表显示图标
	}

	DWORD nListNum = m_List_Task.GetItemCount();

	if (nListNum > nTaskNum)
	{
		DWORD nSub = nListNum - nTaskNum;

		for (DWORD i = 0; i < nSub; i++)
		{
			m_List_Task.DeleteItem(m_List_Task.GetItemCount()-1);
		}
	}
	else if(nTaskNum > nListNum)
	{
		DWORD nAdd = nTaskNum - nListNum;
		for (DWORD i = 0; i < nAdd; i++)
		{
			m_List_Task.InsertItem(m_List_Task.GetItemCount(),L"");
		}
	}

	ImageList_SetImageCount(nHiml, nListNum);

	DWORD nIndex = 0;

	for (DWORD i = 0; i < nTaskNum; i++)
	{
		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//获取文件信息
		if (!SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		//DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//置入图标返回图标下标
		ImageList_ReplaceIcon(nHiml,i, nPsfi.hIcon);
		DestroyIcon(nPsfi.hIcon);

		m_List_Task.SetItem(nIndex, 0,2,TEXT(""),i,0,0,0);
		m_List_Task.SetItemText(nIndex, 0, nTaskList.szExeFile[nIndex]);
		m_List_Task.SetItemText(nIndex, 1, nTaskList.th32ProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 2, nTaskList.szPriority[nIndex]);
		m_List_Task.SetItemText(nIndex, 3, nTaskList.WorkingSetSize[nIndex]);
		m_List_Task.SetItemText(nIndex, 4, nTaskList.cntThreads[nIndex]);
		m_List_Task.SetItemText(nIndex, 5, nTaskList.th32ParentProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 6, nTaskList.szExeFilePath[nIndex]);

		nIndex++;
	}

}
void CMyTaskDlg::Fun()
{
	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);
	RefershTaskList(m_List_Task, nTaskList, nNum);
	ClearTaskList(nTaskList);

}
DWORD WINAPI RefershThread(LPVOID lpThreadParameter)
{
	CMyTaskDlg *nDlg = (CMyTaskDlg*)lpThreadParameter;
	while (true)
	{
		//刷新进程
		nDlg->Fun();

		//刷新状态栏
		TASKLIST nTaskList = {};
		DWORD nNum = EnumTaskList(nTaskList);
		ClearTaskList(nTaskList);
		Sleep(1000);
	}
}


BOOL CMyTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_List_Task.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Task.InsertColumn(0, TEXT("进程名字"),0,150);
	m_List_Task.InsertColumn(1, TEXT("进程PID"), 0, 60);
	m_List_Task.InsertColumn(2, TEXT("优先级"), 0, 60);
	m_List_Task.InsertColumn(3, TEXT("内存使用"), 0, 80);
	m_List_Task.InsertColumn(4, TEXT("线程数量"), 0, 60);
	m_List_Task.InsertColumn(5, TEXT("父进程PID"), 0, 80);
	m_List_Task.InsertColumn(6, TEXT("进程路径"), 0, 390);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	MyRtlAdjustPrivilege();//提升权限

	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);//枚举进程列表
	PrintTaskList(m_List_Task, nTaskList, nNum);//打印进程到列表
	ClearTaskList(nTaskList);

	
	CreateThread(NULL, NULL, RefershThread, (LPVOID)this, NULL, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CMyTaskDlg::OnClose()
{
	TerminateProcess((HANDLE)-1, 0);

	ExitProcess(0);
	CDialogEx::OnClose();
}
void CMyTaskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
HCURSOR CMyTaskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyTaskDlg::OnDblclkListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)return;

	DWORD nNow = pNMItemActivate->iItem;
	CString nProName = m_List_Task.GetItemText(nNow, 0);
	CString nPidTemp = m_List_Task.GetItemText(nNow, 1);

	_stscanf_s(nPidTemp, TEXT("%d"), &g_Pid);
	DebugDlg *pDebugDlg = new DebugDlg(nProName, g_Pid);
}


void CMyTaskDlg::OnDropFiles(HDROP hDropInfo)
{
	CDialogEx::OnDropFiles(hDropInfo);

	TCHAR buff[MAX_PATH]{};
	DragQueryFile(hDropInfo, 0, buff, MAX_PATH);
	DragFinish(hDropInfo);

	CString nProName = GetFileFullName(buff);
	DebugDlg *pDebugDlg = new DebugDlg(CString(buff), nProName);

}


