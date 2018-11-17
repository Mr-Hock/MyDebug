
// MyTaskDlg.cpp : ʵ���ļ�
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


//��ӡ�����б�
void PrintTaskList(ClistCtrlEx &m_List_Task,TASKLIST &nTaskList, DWORD nTaskNum)
{
	HIMAGELIST nHiml = { 0 };//ͼ���б�
	SHFILEINFO nPsfi = { 0 };//�ļ���Ϣ
	ImageList_Destroy(nHiml);//���ͼ���б�
	nHiml = ImageList_Create(16, 16, ILC_COLOR32, 0, 0);//����ͼ����
	ImageList_SetBkColor(nHiml, m_List_Task.GetBkColor());//����ͼ���б��ɫ
	m_List_Task.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//���ó����б���ʾͼ��


	for (DWORD i = 0; i < nTaskNum; i++)
	{
		DWORD nIndex = m_List_Task.GetItemCount();

		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//��ȡ�ļ���Ϣ
		if (!SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			 SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON| SHGFI_USEFILEATTRIBUTES| SHGFI_SMALLICON);
		DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//����ͼ�귵��ͼ���±�

		m_List_Task.InsertItem(nIndex, nTaskList.szExeFile[nIndex],nIco);
		m_List_Task.SetItemText(nIndex, 1, nTaskList.th32ProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 2, nTaskList.szPriority[nIndex]);
		m_List_Task.SetItemText(nIndex, 3, nTaskList.WorkingSetSize[nIndex]);
		m_List_Task.SetItemText(nIndex, 4, nTaskList.cntThreads[nIndex]);
		m_List_Task.SetItemText(nIndex, 5, nTaskList.th32ParentProcessID[nIndex]);
		m_List_Task.SetItemText(nIndex, 6, nTaskList.szExeFilePath[nIndex]);
	}

}
//ˢ�½����б�
void RefershTaskList(ClistCtrlEx &m_List_Task, TASKLIST &nTaskList, DWORD nTaskNum)
{
	static HIMAGELIST nHiml = { 0 };//ͼ���б�
	static SHFILEINFO nPsfi = { 0 };//�ļ���Ϣ
	//ImageList_Destroy(nHiml);//���ͼ���б�
	static BOOL nOn = FALSE;
	if (!nOn)
	{
		nOn = !nOn;
		nHiml = ImageList_Create(16, 16, ILC_COLOR32, 0, 0);//����ͼ����
		ImageList_SetBkColor(nHiml, m_List_Task.GetBkColor());//����ͼ���б��ɫ
		m_List_Task.SendMessage(LVM_SETIMAGELIST, 1, (LPARAM)nHiml);//���ó����б���ʾͼ��
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
		DWORD nFileAttributes = SHGFI_SYSICONINDEX | SHGFI_SMALLICON;//��ȡ�ļ���Ϣ
		if (!SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON))
			SHGetFileInfo(nTaskList.szExeFilePath[nIndex], nFileAttributes, &nPsfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON);
		//DWORD nIco = ImageList_AddIcon(nHiml, nPsfi.hIcon);//����ͼ�귵��ͼ���±�
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
		//ˢ�½���
		nDlg->Fun();

		//ˢ��״̬��
		TASKLIST nTaskList = {};
		DWORD nNum = EnumTaskList(nTaskList);
		ClearTaskList(nTaskList);
		Sleep(1000);
	}
}


BOOL CMyTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_List_Task.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Task.InsertColumn(0, TEXT("��������"),0,150);
	m_List_Task.InsertColumn(1, TEXT("����PID"), 0, 60);
	m_List_Task.InsertColumn(2, TEXT("���ȼ�"), 0, 60);
	m_List_Task.InsertColumn(3, TEXT("�ڴ�ʹ��"), 0, 80);
	m_List_Task.InsertColumn(4, TEXT("�߳�����"), 0, 60);
	m_List_Task.InsertColumn(5, TEXT("������PID"), 0, 80);
	m_List_Task.InsertColumn(6, TEXT("����·��"), 0, 390);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	MyRtlAdjustPrivilege();//����Ȩ��

	TASKLIST nTaskList = {};
	DWORD nNum = EnumTaskList(nTaskList);//ö�ٽ����б�
	PrintTaskList(m_List_Task, nTaskList, nNum);//��ӡ���̵��б�
	ClearTaskList(nTaskList);

	
	CreateThread(NULL, NULL, RefershThread, (LPVOID)this, NULL, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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


