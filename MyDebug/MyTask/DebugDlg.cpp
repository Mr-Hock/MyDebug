// DebugDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyHead.h"
#include "DebugDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <DbgHelp.h>
#pragma  comment (lib,"DbgHelp.lib")

extern DWORD g_Pid;

DWORD g_SendAddress_Asm = 0;
CString g_SendAsm_Asm;
DWORD g_SendBytesNum_Asm = 0;
BYTE g_RecvBytes_Asm[256]{};
DWORD g_RecvBytesNum_Asm = 0;
BOOL g_Ok_Asm = FALSE;

DWORD g_SendAddress_Memory = 0;
CString g_SendBytes_Memory;
DWORD g_SendBytesNum_Memory = 0;
DWORD g_RecvAddress_Memory = 0;
DWORD g_RecvBytes_Memory = 0;
DWORD g_RecvBytesNum_Memory = 0;
BOOL g_Ok_Memory = FALSE;

DWORD g_RecvAddress_Module = 0;

BOOL g_OK_ConditionBreakPoint = FALSE;
CONDITION g_RecvCondition = {};

#define SETUP_STOP 0
#define SETUP_IN 1
#define SETUP_OUT 2
#define SETUP_RUN 3

#define BREAK_SOFT 0
#define BREAK_HARD 1
#define BREAK_MEMORY_EXCU 2
#define BREAK_MEMORY_RW 3


LONG g_AsmListProc;
LONG g_MemoryListProc;
LONG g_EditProc;


DWORD nReadAddressFirst;
DWORD nReadAddressSecond;
DWORD nReadAddressEnd;
DebugDlg* g_pDlg = NULL;


DWORD g_SingDebug = SETUP_RUN;
BOOL g_BreakType = BREAK_SOFT;
BOOL g_BreakIng = FALSE;

CONTEXT g_Context{};

// DebugDlg �Ի���


IMPLEMENT_DYNAMIC(DebugDlg, CDialogEx)

DebugDlg::DebugDlg(CString nProName, DWORD nPid)
	: CDialogEx(IDD_DEBUG_DIALOG, NULL),
	m_ProName(nProName),
	m_Pid(nPid),
	m_WinText(nProName + TEXT(" - "))
{
	Create(IDD_DEBUG_DIALOG);
	SetWindowText(m_WinText + TEXT("���Խ���"));
}
DebugDlg::DebugDlg(CString nFilePath, CString nProName)
	: CDialogEx(IDD_DEBUG_DIALOG, NULL),
	m_ProName(nProName), 
	m_FilePath(nFilePath),
	m_WinText(nProName + TEXT(" - "))
{
	Create(IDD_DEBUG_DIALOG);
	SetWindowText(m_WinText + TEXT("���Խ���"));
}

DebugDlg::~DebugDlg()
{
}

void DebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ASM, m_List_Asm);
	DDX_Control(pDX, IDC_LIST_HEAP, m_List_Heap);
	DDX_Control(pDX, IDC_EDIT12, m_Edit_Output);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Eax);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_Ebx);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_Ecx);
	DDX_Control(pDX, IDC_EDIT4, m_Edit_Edx);
	DDX_Control(pDX, IDC_EDIT5, m_Edit_Esi);
	DDX_Control(pDX, IDC_EDIT34, m_Edit_Edi);
	DDX_Control(pDX, IDC_EDIT35, m_Edit_Ebp);
	DDX_Control(pDX, IDC_EDIT6, m_Edit_Esp);
	DDX_Control(pDX, IDC_EDIT11, m_Edit_Eip);
	DDX_Control(pDX, IDC_EDIT15, m_Edit_Dr0);
	DDX_Control(pDX, IDC_EDIT7, m_Edit_Dr1);
	DDX_Control(pDX, IDC_EDIT8, m_Edit_Dr2);
	DDX_Control(pDX, IDC_EDIT9, m_Edit_Dr3);
	DDX_Control(pDX, IDC_EDIT17, m_Edit_Dr6);
	DDX_Control(pDX, IDC_EDIT18, m_Edit_Dr7);
	DDX_Control(pDX, IDC_COMBO3, m_Combo_Address);
	DDX_Control(pDX, IDC_EDIT22, m_Edit_Es);
	DDX_Control(pDX, IDC_EDIT19, m_Edit_Cs);
	DDX_Control(pDX, IDC_EDIT20, m_Edit_Ss);
	DDX_Control(pDX, IDC_EDIT21, m_Edit_Ds);
	DDX_Control(pDX, IDC_EDIT23, m_Edit_Fs);
	DDX_Control(pDX, IDC_EDIT24, m_Edit_Gs);
	DDX_Control(pDX, IDC_LIST_MEMORY, m_List_Memory);
	DDX_Control(pDX, IDC_EDIT28, m_Edit_Cf);
	DDX_Control(pDX, IDC_EDIT25, m_Edit_Pf);
	DDX_Control(pDX, IDC_EDIT26, m_Edit_Af);
	DDX_Control(pDX, IDC_EDIT27, m_Edit_Zf);
	DDX_Control(pDX, IDC_EDIT29, m_Edit_Sf);
	DDX_Control(pDX, IDC_EDIT30, m_Edit_Tf);
	DDX_Control(pDX, IDC_EDIT31, m_Edit_Df);
	DDX_Control(pDX, IDC_EDIT32, m_Edit_Of);
	DDX_Control(pDX, IDC_EDIT10, m_Edit_Print);
	DDX_Control(pDX, IDC_PROGRESS_TABLE, m_Progress_Table);
	DDX_Control(pDX, IDC_STATIC_TABLE, m_Static_Table);
	DDX_Control(pDX, IDC_STATIC_COMMAND, m_Static_Command);
}


BEGIN_MESSAGE_MAP(DebugDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HEAP, &DebugDlg::OnDblclkListHeap)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MEMORY, &DebugDlg::OnCustomdrawListMemory)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_ASM, &DebugDlg::OnCustomdrawListAsm)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_HEAP, &DebugDlg::OnCustomdrawListHeap)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ASM, &DebugDlg::OnDblclkListAsm)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MEMORY, &DebugDlg::OnDblclkListMemory)
	ON_COMMAND(ID_GotoAddress, &DebugDlg::OnGotoaddress)
	ON_COMMAND(ID_ChangeAsm, &DebugDlg::OnChangeasm)
	ON_COMMAND(ID_SoftBreak, &DebugDlg::OnSoftbreak)
	ON_COMMAND(ID_HardBreak, &DebugDlg::OnHardbreak)
	ON_COMMAND(ID_In, &DebugDlg::OnIn)
	ON_COMMAND(ID_Jump, &DebugDlg::OnJump)
	ON_COMMAND(ID_Run, &DebugDlg::OnRun)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ASM, &DebugDlg::OnRclickListAsm)
	ON_COMMAND(ID_InAddress, &DebugDlg::OnInaddress)
	ON_COMMAND(ID_Module, &DebugDlg::OnModule)
	ON_COMMAND(ID_BreakList, &DebugDlg::OnBreaklist)
	ON_COMMAND(ID_EditMemory, &DebugDlg::OnEditmemory)
	ON_COMMAND(ID_GotoMemory, &DebugDlg::OnGotomemory)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MEMORY, &DebugDlg::OnRclickListMemory)
	ON_COMMAND(ID_HardWrite1, &DebugDlg::OnHardwrite1)
	ON_COMMAND(ID_HardWrite2, &DebugDlg::OnHardwrite2)
	ON_COMMAND(ID_HardWrite4, &DebugDlg::OnHardwrite4)
	ON_COMMAND(ID_HardRead1, &DebugDlg::OnHardread1)
	ON_COMMAND(ID_HardRead2, &DebugDlg::OnHardread2)
	ON_COMMAND(ID_HardRead4, &DebugDlg::OnHardread4)
	ON_COMMAND(ID_ConditionSoftBreak, &DebugDlg::OnConditionsoftbreak)
	ON_COMMAND(ID_ConditionHardBreak, &DebugDlg::OnConditionhardbreak)
	ON_COMMAND(ID_MemoryBreak, &DebugDlg::OnMemoryBreak)
	ON_COMMAND(ID_MemoryWrite, &DebugDlg::OnMemoryWrite)
	ON_COMMAND(ID_MemoryRead, &DebugDlg::OnMemoryRead)
	ON_COMMAND(ID_MemoryReadWrite, &DebugDlg::OnMemoryReadWrite)
	ON_COMMAND(ID_Table, &DebugDlg::OnTable)
	ON_COMMAND(ID_SaveDump, &DebugDlg::OnSavedump)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ASM, &DebugDlg::OnClickListAsm)
	ON_WM_TIMER()
	ON_COMMAND(ID_SOURCEDEBUG, &DebugDlg::OnSourcedebug)
END_MESSAGE_MAP()


LRESULT CALLBACK ListAsmProc(HWND hwnd,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOUSEWHEEL)
	{
		SHORT zDelta = (SHORT)HIWORD(wParam);

		//OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("%d"), zDelta);

		if (zDelta > 0)
		{
			g_pDlg->PrintOnceAsm(g_pDlg->GetBeforeAddress(nReadAddressFirst));

			if (g_pDlg->m_List_Asm.GetSelectionItem() <= g_pDlg->m_Num_Asm - 2)
			{
				int nIndex = g_pDlg->m_List_Asm.GetSelectionItem();
				g_pDlg->m_List_Asm.SetSelectionEx(nIndex + 1);
			}
		}
		else 
		{ 
			g_pDlg->PrintOnceAsm(nReadAddressSecond); 
			if (g_pDlg->m_List_Asm.GetSelectionItem() != 0)
			{
				int nIndex = g_pDlg->m_List_Asm.GetSelectionItem();
				g_pDlg->m_List_Asm.SetSelectionEx(nIndex - 1);
			}

		}
	}
	else if(uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_SPACE)
		{
			g_pDlg->OnInaddress();
			
		}
		else if(wParam == VK_OEM_MINUS)
		{
			g_pDlg->BackSelectRecord();
		}
		else if (wParam == VK_OEM_PLUS)
		{
			g_pDlg->ComeSelectRecord();
		}

		else if (wParam == VK_RETURN)
		{
			g_pDlg->OnChangeasm();
		}
		else if (wParam == VK_F2)
		{
			g_pDlg->OnSoftbreak();
		}
		else if (wParam == VK_F3)
		{
			g_pDlg->OnConditionsoftbreak();
		}
		else if (wParam == VK_F4)
		{
			g_pDlg->OnHardbreak();
		}
		else if (wParam == VK_F5)
		{
			g_pDlg->OnConditionhardbreak();
		}
		else if (wParam == VK_F6)
		{
			g_pDlg->OnMemoryBreak();
		}
		else if (wParam == VK_F7)
		{
			g_pDlg->OnIn();
		}
		else if (wParam == VK_F8)
		{
			g_pDlg->OnJump();
		}
		else if (wParam == VK_F9)
		{
			g_pDlg->OnRun();
		}

	}
	else if (uMsg == WM_CHAR)
	{
		if (wParam == 12)
		{
			g_pDlg->OnBreaklist();
		}
		else if (wParam == 13)
		{
			g_pDlg->OnModule();
		}
		else if (wParam == 20)
		{
			g_pDlg->OnTable();
		}

	}

	return CallWindowProc((WNDPROC)g_AsmListProc, hwnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK ListMemoryProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOUSEWHEEL)
	{
		SHORT zDelta = (SHORT)HIWORD(wParam);

		CString nTemp;
		DWORD nAddressFirst;
		DWORD nAddressSecond;
		DWORD nSeg;
		if (zDelta > 0)
		{
			nTemp = g_pDlg->m_List_Memory.GetItemText(0, g_pDlg->m_Flag_MemoryAddress);
			_stscanf_s(nTemp, TEXT("%x"), &nAddressFirst);
			nTemp = g_pDlg->m_List_Memory.GetItemText(1, g_pDlg->m_Flag_MemoryAddress);
			_stscanf_s(nTemp, TEXT("%x"), &nAddressSecond);

			nSeg = nAddressSecond - nAddressFirst;
			g_pDlg->PrintMemory(nAddressFirst - nSeg);


			if (g_pDlg->m_List_Memory.GetSelectionItem() <= g_pDlg->m_Num_Memory - 2)
			{
				int nIndex = g_pDlg->m_List_Memory.GetSelectionItem();
				g_pDlg->m_List_Memory.SetSelectionEx(nIndex + 1);
			}

		}
		else
		{
			nTemp = g_pDlg->m_List_Memory.GetItemText(0, g_pDlg->m_Flag_MemoryAddress);
			_stscanf_s(nTemp, TEXT("%x"), &nAddressFirst);
			nTemp = g_pDlg->m_List_Memory.GetItemText(1, g_pDlg->m_Flag_MemoryAddress);
			_stscanf_s(nTemp, TEXT("%x"), &nAddressSecond);

			nSeg = nAddressSecond - nAddressFirst;
			g_pDlg->PrintMemory(nAddressFirst + nSeg);


			if (g_pDlg->m_List_Memory.GetSelectionItem() != 0)
			{
				int nIndex = g_pDlg->m_List_Memory.GetSelectionItem();
				g_pDlg->m_List_Memory.SetSelectionEx(nIndex - 1);
			}

		}
	}
	else if (uMsg == WM_CHAR)
	{
		if (wParam == 7)
		{
			g_pDlg->OnGotomemory();
		}

	}
	else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
	{
		g_pDlg->nTempItem = g_pDlg->m_List_Memory.GetSelectionItem();
		g_pDlg->nTempSubItem = g_pDlg->m_List_Memory.GetSelectionSubItem();
		g_pDlg->SetFocus();
		g_pDlg->m_List_Memory.SetFocus();
	}
	return CallWindowProc((WNDPROC)g_MemoryListProc, hwnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_LBUTTONDBLCLK && g_BreakIng)
	{
		SendMessage(hwnd, EM_SETREADONLY, FALSE, 0L);
	}
	else if (uMsg == WM_KEYDOWN && wParam == VK_RETURN)
	{
		SendMessage(hwnd, EM_SETREADONLY, TRUE, 0L);

		if (g_BreakIng)
		{
			CString szValue;

			ZeroMemory(&g_Context, sizeof(g_Context));
			g_Context.ContextFlags = CONTEXT_FULL;
			GetThreadContext(g_pDlg->m_ThreadHandle, &g_Context);

			if (hwnd == g_pDlg->m_Edit_Eax.m_hWnd)
			{
				g_pDlg->m_Edit_Eax.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Eax);
				szValue.Format(TEXT("%08X"), g_Context.Eax);
				g_pDlg->m_Edit_Eax.SetWindowText(szValue);
			}
			else if (hwnd == g_pDlg->m_Edit_Ebx.m_hWnd)
			{
				g_pDlg->m_Edit_Ebx.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Ebx);
			}
			else if (hwnd == g_pDlg->m_Edit_Ecx.m_hWnd)
			{
				g_pDlg->m_Edit_Ecx.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Ecx);
			}
			else if (hwnd == g_pDlg->m_Edit_Edx.m_hWnd)
			{
				g_pDlg->m_Edit_Edx.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Edx);
			}
			else if (hwnd == g_pDlg->m_Edit_Esi.m_hWnd)
			{
				g_pDlg->m_Edit_Esi.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Esi);
			}
			else if (hwnd == g_pDlg->m_Edit_Edi.m_hWnd)
			{
				g_pDlg->m_Edit_Edi.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Edi);
			}
			else if (hwnd == g_pDlg->m_Edit_Ebp.m_hWnd)
			{
				g_pDlg->m_Edit_Ebp.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Ebp);
			}
			else if (hwnd == g_pDlg->m_Edit_Esp.m_hWnd)
			{
				g_pDlg->m_Edit_Esp.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Esp);
			}
			else if (hwnd == g_pDlg->m_Edit_Eip.m_hWnd)
			{
				g_pDlg->m_Edit_Eip.GetWindowText(szValue);
				_stscanf_s(szValue, TEXT("%X"), &g_Context.Eip);
			}

			SetThreadContext(g_pDlg->m_ThreadHandle, &g_Context);

		}

	}
	return CallWindowProc((WNDPROC)g_EditProc, hwnd, uMsg, wParam, lParam);
}

DWORD WINAPI DebugThread(LPVOID lpThreadParameter)
{
	DebugDlg *nDlg = (DebugDlg*)lpThreadParameter;

	DEBUG_EVENT nDebugEvent{};
	BOOL nCondotionSoftBreak = FALSE;
	BOOL nCondotionHardBreak = FALSE;
	BOOL nFindMemoryBreakAddress = FALSE;
	BOOL nIsMemoryBreakAddress = FALSE;
	BOOL nJumpMemoryBreakAddress = FALSE;
	//�жϴ�������
	if (nDlg->m_FilePath.IsEmpty() == FALSE)
	{
		if (!nDlg->CreateDebugProcess())return 0;
	}

	//����Ǹ��ӽ���
	else
	{
		if (!DebugActiveProcess(nDlg->m_Pid))MessageBox(NULL, TEXT("���Խ���ʧ�ܣ�"), TEXT("��ʾ"), MB_ICONERROR);
	}


	//�����������������������������������������������������쳣�����������������������������������������������������������

	while (WaitForDebugEvent(&nDebugEvent, INFINITE))
	{
		//************************************
		// �����쳣�¼�
		//************************************
		if (nDebugEvent.dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
		{
			static BOOL nFirstBreak = TRUE;
			if (nFirstBreak && nDlg->m_FilePath != "")
			{
				//���öϵ��¼���ԭ��һ������ϵ�
				nDlg->OpenAllBreakPoint();
				nFirstBreak = FALSE;
			}

			g_BreakIng = TRUE;

			//************************************
			// �ϵ��쳣�¼�
			//************************************
			if (nDebugEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
			{
				DWORD nBreakPoint;
				DWORD nTempBreakPoint;
				BOOL nIsCondition;
				BOOL nIsConditionSucess;

				ZeroMemory(&g_Context, sizeof(g_Context));
				g_Context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
				GetThreadContext(nDlg->m_ThreadHandle, &g_Context);
				g_Context.Eip = g_Context.Eip - 1;
				
				//////////Դ�����//////////
				//nDlg->ShowSymbol(g_Context.Eip + 1);
				nDlg->ShowSource(g_Context.Eip + 1);
				


				nBreakPoint = nDlg->IsBreakPoint(g_Context.Eip);
				nTempBreakPoint = nDlg->IsTempBreakPoint(g_Context.Eip);
				nIsCondition = nDlg->IsConditionBreakPoint(nBreakPoint);
				nIsConditionSucess = nDlg->IsConditionSucessBreakPoint(nBreakPoint, g_Context);

				//����ϵ��ַ���ҵĶϵ��б���
				if (nBreakPoint != -1 || nTempBreakPoint != -1)
				{
					if (nTempBreakPoint != -1)nDlg->DelTempBreakPoint(g_Context.Eip);


					//��������ϵ����
					if ((nIsCondition && nIsConditionSucess) || nIsCondition == FALSE)
					{
						nDlg->PrintContext(g_Context);
						OutputDebugEdit(nDlg->m_Edit_Output, TEXT("�жϣ�%08X"), g_Context.Eip);


						g_pDlg->SetWindowText(g_pDlg->m_WinText + TEXT("����ͣ�߳�..."));
						SwitchToThisWindow(g_pDlg->m_hWnd, TRUE);//������ǰ
						g_pDlg->m_List_Asm.SetFocus();//�б���ȡ����
						g_pDlg->SetFocus();//���ڻ�ȡ����

						g_pDlg->CloseAllBreakPoint();

						nDlg->ShowSingleAsm(g_Context.Eip);
						nDlg->PrintHeap(g_Context);
						//g_pDlg->m_List_Asm.SetSelectionEx(0);


						g_Context.EFlags = g_Context.EFlags + 256;
						SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
						SuspendThread(nDlg->m_ThreadHandle);

						OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("����ϵ㴥��"));
						g_BreakType = BREAK_SOFT;
					}
					//��������ϵ㲻����
					else
					{
						OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("����ϵ㴥������������"));

						g_pDlg->CloseAllBreakPoint();
						g_Context.EFlags = g_Context.EFlags + 256;
						SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
						nCondotionSoftBreak = TRUE;
					}

				}

				//����ϵ��ַ�����ҵĶϵ��б���
				else
				{
					//ContinueDebugEvent(nDebugEvent.dwProcessId, nDebugEvent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
				}
			}

			//************************************
			// �ڴ����ҳ�쳣�¼�
			//************************************
			else if (nDebugEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
			{
	
				ZeroMemory(&g_Context, sizeof(g_Context));
				g_Context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
				GetThreadContext(nDlg->m_ThreadHandle, &g_Context);
				
				//nExceptType��8=ִ���쳣��0=��ȡ�쳣��1=д���쳣
				DWORD nExceptType = nDebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[0];
				//nRwAddress���ڴ��ҳ�쳣�ĵ�ַ
				DWORD nRwAddress = nDebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[1];

				//�����ִ���쳣
				if (nExceptType == 8 && nExceptType == g_pDlg->GetMemoryBreakPointType())
				{
					//������еĲ����ڴ�ϵ�
					if (!nDlg->IsMemoryBreakPoint(g_Context.Eip))
					{
						DWORD nOldProtect;
						VirtualProtectEx(g_pDlg->m_ProcessHandle, (LPVOID)g_Context.Eip, 1, g_pDlg->m_MemoryBreakPoint.nOldPageProtect, &nOldProtect);

						g_Context.EFlags = g_Context.EFlags + 256;
						SetThreadContext(nDlg->m_ThreadHandle, &g_Context);

						nIsMemoryBreakAddress = FALSE;
						nFindMemoryBreakAddress = TRUE;
						g_BreakType = BREAK_MEMORY_EXCU;

					}

					//������е����ڴ�ϵ�
					else
					{
						//////////Դ�����//////////
						nDlg->ShowSource(g_Context.Eip + 1);

						g_Context.EFlags = g_Context.EFlags + 256;
						SetThreadContext(nDlg->m_ThreadHandle, &g_Context);

						OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("1�ڴ�����쳣%08X��%08X��%08X��%08X"),
							nDebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[0],
							nDebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[1],
							g_Context.Eip,
							nDebugEvent.u.Exception.ExceptionRecord.ExceptionAddress);

						DWORD nOldProtect;

						VirtualProtectEx(g_pDlg->m_ProcessHandle, (LPVOID)g_Context.Eip,
							1, g_pDlg->m_MemoryBreakPoint.nOldPageProtect, &nOldProtect);

						nIsMemoryBreakAddress = TRUE;
						nFindMemoryBreakAddress = FALSE;
						g_BreakType = BREAK_MEMORY_EXCU;

					}
				}

				//����Ƕ�ȡ,��д�쳣
				else if (nExceptType == 0 || nExceptType == 1)
				{

					DWORD nOldProtect;
					VirtualProtectEx(g_pDlg->m_ProcessHandle, (LPVOID)g_pDlg->m_MemoryBreakPoint.nAddress, 1, 
						g_pDlg->m_MemoryBreakPoint.nOldPageProtect, &nOldProtect);

					g_Context.EFlags = g_Context.EFlags + 256;
					SetThreadContext(nDlg->m_ThreadHandle, &g_Context);

					//��������ڴ�ϵ�
					if (nDlg->IsMemoryBreakPoint(nRwAddress))
					{
						DWORD nReadWriteAddress = 0;
						nReadWriteAddress = (DWORD)nDebugEvent.u.Exception.ExceptionRecord.ExceptionAddress;

						//����Ƕ�д�쳣
						if (g_pDlg->GetMemoryBreakPointType() == 100)
						{
							g_pDlg->AddBreakReadWrite(nReadWriteAddress, g_pDlg->GetOnceAsm(nReadWriteAddress));
						}

						//�����0=��ȡ�쳣��1=д���쳣
						else if (nExceptType == g_pDlg->GetMemoryBreakPointType())
						{
							g_pDlg->AddBreakReadWrite(nReadWriteAddress, g_pDlg->GetOnceAsm(nReadWriteAddress));
						}

						//OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("%s�����жϵ�:%08X��ִ�е�ַ:%08X"),
						//	nExceptType == 0 ? TEXT("��ȡ�쳣") : TEXT("��д�쳣"), nRwAddress, nReadWriteAddress);

					}

					//���û�������ڴ�ϵ�
					else
					{
						//OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("�쳣��ַ:%08X��Info1:%08X��Info2:%08X"), 
						//	 nDebugEvent.u.Exception.ExceptionRecord.ExceptionAddress,nInfo1, nInfo2);

					}

					nJumpMemoryBreakAddress = TRUE;
					g_BreakType = BREAK_MEMORY_RW;

				}

			}

			//************************************
			// �����쳣�¼�
			//************************************
			else if (nDebugEvent.u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_SINGLE_STEP)
			{

				DWORD nHardBreak;
				DWORD nHardBreakEx;
				DWORD nTempHardBreak;
				BOOL nIsCondition;
				BOOL nIsConditionSucess;

				ZeroMemory(&g_Context, sizeof(g_Context));
				g_Context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
				GetThreadContext(nDlg->m_ThreadHandle, &g_Context);

				nHardBreak = nDlg->IsHardBreakPoint(g_Context.Eip);
				nTempHardBreak = nDlg->IsTempHardBreakPoint(g_Context.Eip);
				nHardBreakEx = nDlg->IsHardBreakPointEx(g_Context.Eip);
				nIsCondition = nDlg->IsConditionHardBreakPoint(nHardBreak);
				nIsConditionSucess = nDlg->IsConditionSucessHardBreakPoint(nHardBreak, g_Context);

				//����Ƕ�д�����쳣
				if (nHardBreakEx != -1)
				{
					DWORD nReadWriteAddress;
					nReadWriteAddress = g_pDlg->GetBeforeAddress(g_Context.Eip);
					
					g_pDlg->AddBreakReadWrite(nReadWriteAddress, g_pDlg->GetOnceAsm(nReadWriteAddress));
					OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Ӳ����д�ϵ�%08X - %s"), nReadWriteAddress, g_pDlg->GetOnceAsm(nReadWriteAddress));

				}

				//����ǻ�ԭ�ϵ��쳣
				else if (nCondotionSoftBreak)
				{
					nCondotionSoftBreak = FALSE;
					g_pDlg->OpenAllBreakPoint();
					OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("����������������ԭ�ϵ�"));
				}

				//�����ִ�жϵ��쳣
				else
				{

					//************************************
					// Ӳ���ϵ㴥��
					//************************************
					if (nHardBreak != -1 || nTempHardBreak != -1 || g_BreakType == BREAK_HARD)
					{
						//////////Դ�����//////////
						nDlg->ShowSource(g_Context.Eip + 1);

						//������״�ִ�жϵ㴥��
						if (nHardBreak != -1 || nTempHardBreak != -1)
						{
							//��������ϵ����
							if ((nIsCondition && nIsConditionSucess) || nIsCondition == FALSE)
							{
								nDlg->PrintContext(g_Context);
								nDlg->PrintHeap(g_Context);
								nDlg->ShowSingleAsm(g_Context.Eip);

								if (nHardBreak != -1)nDlg->CloseAllHardBreakPoint();
								else if (nTempHardBreak != -1)nDlg->DelTempHardBreakPoint(g_Context.Eip);

								g_pDlg->SetWindowText(g_pDlg->m_WinText + TEXT("����ͣ�߳�..."));
								SwitchToThisWindow(g_pDlg->m_hWnd, TRUE);//������ǰ
								g_pDlg->m_List_Asm.SetFocus();//�б���ȡ����
								g_pDlg->SetFocus();//���ڻ�ȡ����

								g_Context.EFlags = g_Context.EFlags + 256;
								SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
								SuspendThread(nDlg->m_ThreadHandle);

								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Ӳ���ϵ㵥������"));
								g_BreakType = BREAK_HARD;
							}

							//��������ϵ㲻����
							else
							{
								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Ӳ���ϵ㴥������������"));
								g_Context.Eip = g_Context.Eip + 1;
								SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
							}

						}

						//����ǵ�������
						else if (g_SingDebug == SETUP_IN)
						{
							nDlg->PrintContext(g_Context);
							nDlg->PrintHeap(g_Context);
							nDlg->ShowSingleAsm(g_Context.Eip);

							g_Context.EFlags = g_Context.EFlags + 256;
							SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
							SuspendThread(nDlg->m_ThreadHandle);

							OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Ӳ���ϵ㵥������ - ��������"));
						}

						//����ǵ�������
						else if (g_SingDebug == SETUP_OUT)
						{
							nDlg->PrintContext(g_Context);
							nDlg->PrintHeap(g_Context);
							nDlg->ShowSingleAsm(g_Context.Eip);

							DWORD nNextAddress;

							//������㲽������
							if (nDlg->IsCallRepAsm(g_Context.Eip))
							{
								nNextAddress = nDlg->GetNextAddress(g_Context.Eip);

								nDlg->SetTempHardBreakPoint(nNextAddress);

								SuspendThread(nDlg->m_ThreadHandle);

								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Ӳ���ϵ����㲽����������ǰ��ַ��%08X"), g_Context.Eip);
								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Ӳ���ϵ���һ����ַ��%08X"), nNextAddress);

							}

							//��������㲽��Ҫ��
							else
							{
								g_Context.EFlags = g_Context.EFlags + 256;
								SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
								SuspendThread(nDlg->m_ThreadHandle);
								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Ӳ���ϵ㲻���㲽����������ǰ��ַ��%08X"), g_Context.Eip);
							}

						}

						//�����ִ��
						else if (g_SingDebug == SETUP_RUN)
						{
							g_pDlg->SetWindowText(g_pDlg->m_WinText + TEXT("���Խ���"));
							g_pDlg->OpenAllHardBreakPoint();

							OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Ӳ���ϵ㵥������ - ִ��"));
							g_BreakIng = FALSE;

						}

					}

					//************************************
					// ����ϵ㴥��
					//************************************
					else if (g_BreakType == BREAK_SOFT)
					{
						//////////Դ�����//////////
						nDlg->ShowSource(g_Context.Eip + 1);


						//����ǵ�������
						if (g_SingDebug == SETUP_IN)
						{
							nDlg->PrintContext(g_Context);
							nDlg->PrintHeap(g_Context);
							nDlg->ShowSingleAsm(g_Context.Eip);

							g_Context.EFlags = g_Context.EFlags + 256;
							SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
							SuspendThread(nDlg->m_ThreadHandle);

							OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("����ϵ㵥������ - ��������"));

						}

						//����ǵ�������
						else if (g_SingDebug == SETUP_OUT)
						{
							nDlg->PrintContext(g_Context);
							nDlg->PrintHeap(g_Context);
							nDlg->ShowSingleAsm(g_Context.Eip);

							DWORD nNextAddress;

							//������㲽������
							if (nDlg->IsCallRepAsm(g_Context.Eip))
							{
								nNextAddress = nDlg->GetNextAddress(g_Context.Eip);

								nDlg->SetTempBreakPoint(nNextAddress);

								SuspendThread(nDlg->m_ThreadHandle);

								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("����ϵ����㲽����������ǰ��ַ��%08X"), g_Context.Eip);
								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("����ϵ���һ����ַ��%08X"), nNextAddress);

							}
							
							//��������㲽��Ҫ��
							else
							{
								g_Context.EFlags = g_Context.EFlags + 256;
								SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
								SuspendThread(nDlg->m_ThreadHandle);
								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("����ϵ㲻���㲽����������ǰ��ַ��%08X"), g_Context.Eip);
							}

						}

						//�����ִ��
						else if (g_SingDebug == SETUP_RUN)
						{
							g_pDlg->SetWindowText(g_pDlg->m_WinText + TEXT("���Խ���"));
							g_pDlg->OpenAllBreakPoint();

							OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("����ϵ㵥������ - ִ��"));
							g_BreakIng = FALSE;

						}
					}

					//************************************
					// �ڴ�ִ�жϵ㴥��
					//************************************
					else if (g_BreakType == BREAK_MEMORY_EXCU)
					{

						//�����ҪѰ���ڴ�ϵ�׼ȷλ��
						if (nFindMemoryBreakAddress)
						{
							ZeroMemory(&g_Context, sizeof(g_Context));
							g_Context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
							GetThreadContext(nDlg->m_ThreadHandle, &g_Context);

							//����ҵ����ڴ�ϵ�λ��
							if (nDlg->IsMemoryBreakPoint(g_Context.Eip))
							{
								nDlg->PrintContext(g_Context);
								nDlg->PrintHeap(g_Context);
								nDlg->ShowSingleAsm(g_Context.Eip);

								g_pDlg->SetWindowText(g_pDlg->m_WinText + TEXT("����ͣ�߳�..."));
								SwitchToThisWindow(g_pDlg->m_hWnd, TRUE);//������ǰ
								g_pDlg->m_List_Asm.SetFocus();//�б���ȡ����
								g_pDlg->SetFocus();//���ڻ�ȡ����

								nFindMemoryBreakAddress = FALSE;

								nDlg->PrintContext(g_Context);
								nDlg->PrintHeap(g_Context);
								nDlg->ShowSingleAsm(g_Context.Eip);

								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("2�ڴ�����쳣%08X��%08X��%08X��%08X"),
									nDebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[0],
									nDebugEvent.u.Exception.ExceptionRecord.ExceptionInformation[1],
									g_Context.Eip,
									nDebugEvent.u.Exception.ExceptionRecord.ExceptionAddress);
								SuspendThread(nDlg->m_ThreadHandle);

								g_Context.EFlags = g_Context.EFlags + 256;
								SetThreadContext(nDlg->m_ThreadHandle, &g_Context);

								ContinueDebugEvent(nDebugEvent.dwProcessId, nDebugEvent.dwThreadId, DBG_CONTINUE);
								continue;


							}

							//���û���ҵ��ڴ�ϵ�λ��
							else
							{
								//DWORD nOldProtect;
								//VirtualProtectEx(g_pDlg->m_ProcessHandle, (LPVOID)g_Context.Eip, 1, g_OldProtect, &nOldProtect);

								nFindMemoryBreakAddress = TRUE;
								g_Context.EFlags = g_Context.EFlags + 256;
								SetThreadContext(nDlg->m_ThreadHandle, &g_Context);

								ContinueDebugEvent(nDebugEvent.dwProcessId, nDebugEvent.dwThreadId, DBG_CONTINUE);
								continue;

							}
						}

						//�������ҪѰ���ڴ�ϵ�׼ȷλ��
						else
						{

							ZeroMemory(&g_Context, sizeof(g_Context));
							g_Context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
							GetThreadContext(nDlg->m_ThreadHandle, &g_Context);

							//////////Դ�����//////////
							nDlg->ShowSource(g_Context.Eip + 1);

							//����ǵ�������
							if (g_SingDebug == SETUP_IN)
							{
								nDlg->PrintContext(g_Context);
								nDlg->PrintHeap(g_Context);
								nDlg->ShowSingleAsm(g_Context.Eip);

								g_Context.EFlags = g_Context.EFlags + 256;
								SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
								SuspendThread(nDlg->m_ThreadHandle);

								ContinueDebugEvent(nDebugEvent.dwProcessId, nDebugEvent.dwThreadId, DBG_CONTINUE);
								continue;
							}

							//����ǵ�������
							else if (g_SingDebug == SETUP_OUT)
							{
								nDlg->PrintContext(g_Context);
								nDlg->PrintHeap(g_Context);
								nDlg->ShowSingleAsm(g_Context.Eip);

								DWORD nNextAddress;

								//������㲽������
								if (nDlg->IsCallRepAsm(g_Context.Eip))
								{
									nNextAddress = nDlg->GetNextAddress(g_Context.Eip);

									nDlg->SetTempBreakPoint(nNextAddress);

									SuspendThread(nDlg->m_ThreadHandle);

									OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("�ڴ�ϵ����㲽����������ǰ��ַ��%08X"), g_Context.Eip);
									OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("�ڴ�ϵ���һ����ַ��%08X"), nNextAddress);

								}

								//��������㲽��Ҫ��
								else
								{
									g_Context.EFlags = g_Context.EFlags + 256;
									SetThreadContext(nDlg->m_ThreadHandle, &g_Context);
									SuspendThread(nDlg->m_ThreadHandle);
									OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("�ڴ�ϵ㲻���㲽����������ǰ��ַ��%08X"), g_Context.Eip);
								}


							}

							//�����ִ��
							else if(g_SingDebug == SETUP_RUN)
							{
								g_pDlg->SetWindowText(g_pDlg->m_WinText + TEXT("���Խ���"));
								
								DWORD nOldProtect;
								VirtualProtectEx(g_pDlg->m_ProcessHandle, (LPVOID)g_pDlg->m_MemoryBreakPoint.nAddress,
									1, PAGE_NOACCESS, &nOldProtect);


								DWORD nMemoryBreakBassAddress;
								DWORD nEipBassAddress;

								MEMORY_BASIC_INFORMATION nMbi{};
								VirtualQueryEx(g_pDlg->m_ProcessHandle, (LPVOID)g_pDlg->m_MemoryBreakPoint.nAddress, &nMbi, sizeof(MEMORY_BASIC_INFORMATION));
								nMemoryBreakBassAddress = (DWORD)nMbi.BaseAddress;
								VirtualQueryEx(g_pDlg->m_ProcessHandle, (LPVOID)g_Context.Eip, &nMbi, sizeof(MEMORY_BASIC_INFORMATION));
								nEipBassAddress = (DWORD)nMbi.BaseAddress;
								
								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("m_MemoryBreakPoint - BaseAddress:%08X"), nMemoryBreakBassAddress);
								OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("Eip - BaseAddress:%08X"), nEipBassAddress);

								//if (nMemoryBreakBassAddress = nEipBassAddress)g_pDlg->DelMemoryBreakPoint(g_pDlg->m_MemoryBreakPoint.nAddress);

								g_BreakIng = FALSE;
								ContinueDebugEvent(nDebugEvent.dwProcessId, nDebugEvent.dwThreadId, DBG_CONTINUE);
								continue;
							}

						}
					}

					//************************************
					// �ڴ��д�ϵ㴥��
					//************************************
					else if (g_BreakType == BREAK_MEMORY_RW)
					{
						if (nJumpMemoryBreakAddress)
						{
							DWORD nOldProtect;
							VirtualProtectEx(g_pDlg->m_ProcessHandle, (LPVOID)g_pDlg->m_MemoryBreakPoint.nAddress, 1,
								PAGE_NOACCESS, &nOldProtect);

						}
					}
				}

			}
		}

		//************************************
		// ���̴����¼�
		//************************************
		else if (nDebugEvent.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT)
		{
			//������̵ķ��ű�
			nDlg->LoadSymbol(&nDebugEvent.u.CreateProcessInfo);

			nDlg->SetTimer(nDlg->TABLECLOCK, 1000, NULL);
			//���ò��CreateProcessEvent�ӿ�
			nDlg->m_LoadPlugin.CallCreateProcessEvent((DWORD)nDebugEvent.dwProcessId, (DWORD)nDebugEvent.dwThreadId);

			//�������ק��������
			if (nDlg->m_FilePath != "")
			{
				//��ʼ����࣬�ڴ�
				nDlg->InitInformationEx((DWORD)nDebugEvent.u.CreateProcessInfo.lpBaseOfImage);

				//��ԭ��һ��Ӳ������ϵ��б�,
				nDlg->OpenAllBreakPoint();
				nDlg->OpenAllHardBreakPointEx();

				//�����ԭ��Ķϵ��б���û��oep������һ��oep�ϵ�
				if (nDlg->IsBreakPoint(nDlg->GetOepAddressEx((DWORD)nDebugEvent.u.CreateProcessInfo.lpBaseOfImage)) == -1)
				{
					nDlg->SetBreakPoint(nDlg->GetOepAddressEx((DWORD)nDebugEvent.u.CreateProcessInfo.lpBaseOfImage), FALSE, {});
				}
			}

			//����Ǹ��ӽ���
			else
			{
				//��ԭ��һ��Ӳ��������ϵ��б�
				nDlg->RecoveryAllBreakInfo();
			}
		}

		//************************************
		// �̴߳����¼�
		//************************************
		else if (nDebugEvent.dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT)
		{
			//���ò��CreateThreadEvent�ӿ�
			nDlg->m_LoadPlugin.CallCreateThreadEvent((DWORD)nDebugEvent.u.CreateThread.hThread, (DWORD)nDebugEvent.u.CreateThread.lpThreadLocalBase);
		}

		ContinueDebugEvent(nDebugEvent.dwProcessId, nDebugEvent.dwThreadId, DBG_CONTINUE);
	}

	//�����������������������������������������������������쳣�����������������������������������������������������������

	return 0;
}

BOOL DebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_pDlg = this;
	m_SourceDlg = new SourceDlg;

	m_Menu.LoadMenu(IDR_MENU1);
	SetMenu(&m_Menu);

	SetPluginMenu();
	SetListType();
	SetAllWindowsLong();

	ReadAllBreakInfo();

	if (m_FilePath.IsEmpty())	InitInformation();

	CreateThread(NULL, NULL, DebugThread, (LPVOID)this, NULL, NULL);


	return TRUE;
}
void DebugDlg::OnClose()
{
	TerminateProcess(m_ProcessHandle, 0);

	DebugActiveProcessStop(m_Pid);
	CloseHandle(m_ProcessHandle);
	CloseHandle(m_ThreadHandle);

	CDialogEx::OnClose();
	DestroyWindow();
}
void DebugDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TABLECLOCK)
	{
		SetTimer(nIDEvent, -1, NULL);
		TableDlg *nTableDlg = new TableDlg;
		//nTableDlg->ShowWindow(TRUE);
	}

	CDialogEx::OnTimer(nIDEvent);
}
BOOL DebugDlg::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		COMBOBOXINFO info = { 0 };
		info.cbSize = sizeof(COMBOBOXINFO);
		m_Combo_Address.GetComboBoxInfo(&info);

		if (pMsg->hwnd == info.hwndItem)
		{
			DWORD nReadAddress;
			CString nTempAddress;
			m_Combo_Address.GetWindowText(nTempAddress);
			
			if (!CheckCommand(nTempAddress))
			{
				nReadAddress = GetApiAddress(nTempAddress);

				AddSelectRecord();

				if (nReadAddress)InitAsm(nReadAddress);
				else
				{
					if (nTempAddress == "oep")nReadAddress = GetOepAddressEx(GetImageBassAddress());
					else _stscanf_s(nTempAddress, TEXT("%x"), &nReadAddress);

					InitAsm(nReadAddress);
				}
				m_Static_Command.SetWindowText(TEXT("��ת�ɹ�"));
			}

			m_Combo_Address.InsertString(m_Combo_Address.GetCount(), nTempAddress);
			m_List_Asm.SetFocus();
			m_List_Asm.SetSelectionEx(0);

			m_Combo_Address.SetWindowText(TEXT(""));
		}


		return FALSE;
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}
HBRUSH DebugDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT://����ؼ����ı��������Ϣ���������һ��switch  
		switch (pWnd->GetDlgCtrlID())//��ĳһ���ض��ؼ������ж�  
		{
		case IDC_EDIT1:
			pDC->SetTextColor(m_EaxColor);
			break;
		case IDC_EDIT2:
			pDC->SetTextColor(m_EbxColor);
			break;
		case IDC_EDIT3:
			pDC->SetTextColor(m_EcxColor);
			break;
		case IDC_EDIT4:
			pDC->SetTextColor(m_EdxColor);
			break;
		case IDC_EDIT5:
			pDC->SetTextColor(m_EsiColor);
			break;
		case IDC_EDIT34:
			pDC->SetTextColor(m_EdiColor);
			break;
		case IDC_EDIT35:
			pDC->SetTextColor(m_EbpColor);
			break;
		case IDC_EDIT6:
			pDC->SetTextColor(m_EspColor);
			break;
		case IDC_EDIT11:
			pDC->SetTextColor(m_EipColor);
			break;
		case IDC_EDIT15:
			pDC->SetTextColor(m_Dr0Color);
			break;
		case IDC_EDIT7:
			pDC->SetTextColor(m_Dr1Color);
			break;
		case IDC_EDIT8:
			pDC->SetTextColor(m_Dr2Color);
			break;
		case IDC_EDIT9:
			pDC->SetTextColor(m_Dr3Color);
			break;
		case IDC_EDIT17:
			pDC->SetTextColor(m_Dr6Color);
			break;
		case IDC_EDIT18:
			pDC->SetTextColor(m_Dr7Color);
			break;
		case IDC_EDIT22:
			pDC->SetTextColor(m_EsColor);
			break;
		case IDC_EDIT19:
			pDC->SetTextColor(m_CsColor);
			break;
		case IDC_EDIT20:
			pDC->SetTextColor(m_SsColor);
			break;
		case IDC_EDIT21:
			pDC->SetTextColor(m_DsColor);
			break;
		case IDC_EDIT23:
			pDC->SetTextColor(m_FsColor);
			break;
		case IDC_EDIT24:
			pDC->SetTextColor(m_GsColor);
			break;
		case IDC_EDIT28:
			pDC->SetTextColor(m_CfColor);
			break;
		case IDC_EDIT25:
			pDC->SetTextColor(m_PfColor);
			break;
		case IDC_EDIT26:
			pDC->SetTextColor(m_AfColor);
			break;
		case IDC_EDIT27:
			pDC->SetTextColor(m_AfColor);
			break;
		case IDC_EDIT29:
			pDC->SetTextColor(m_SfColor);
			break;
		case IDC_EDIT30:
			pDC->SetTextColor(m_TfColor);
			break;
		case IDC_EDIT31:
			pDC->SetTextColor(m_TfColor);
			break;
		case IDC_EDIT32:
			pDC->SetTextColor(m_OfColor);
			break;
		}
		break;
	}
	return hbr;
}
//*********************************************************************************
//���ÿؼ�����
//*********************************************************************************
VOID DebugDlg::SetAllWindowsLong()
{
	g_AsmListProc = SetWindowLong(m_List_Asm.m_hWnd, GWL_WNDPROC, (LONG)ListAsmProc);
	g_MemoryListProc = SetWindowLong(m_List_Memory.m_hWnd, GWL_WNDPROC, (LONG)ListMemoryProc);
	g_EditProc = SetWindowLong(m_Edit_Eax.m_hWnd, GWL_WNDPROC, (LONG)EditProc);
	SetWindowLong(m_Edit_Ebx.m_hWnd, GWL_WNDPROC, (LONG)EditProc);
	SetWindowLong(m_Edit_Ecx.m_hWnd, GWL_WNDPROC, (LONG)EditProc);
	SetWindowLong(m_Edit_Edx.m_hWnd, GWL_WNDPROC, (LONG)EditProc);
	SetWindowLong(m_Edit_Esi.m_hWnd, GWL_WNDPROC, (LONG)EditProc);
	SetWindowLong(m_Edit_Edi.m_hWnd, GWL_WNDPROC, (LONG)EditProc);
	SetWindowLong(m_Edit_Ebp.m_hWnd, GWL_WNDPROC, (LONG)EditProc);
	SetWindowLong(m_Edit_Esp.m_hWnd, GWL_WNDPROC, (LONG)EditProc);
	SetWindowLong(m_Edit_Eip.m_hWnd, GWL_WNDPROC, (LONG)EditProc);

}
//*********************************************************************************
//����List�ؼ�
//*********************************************************************************
VOID DebugDlg::SetListType()
{
	m_List_Asm.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Asm.InsertColumn(m_Flag_Break, TEXT(""), 0, 18);
	m_List_Asm.InsertColumn(m_Flag_Address, TEXT("��ַ"), 0, 0);
	m_List_Asm.InsertColumn(m_Flag_Address_Show, TEXT("��ַ"), 0, 150);
	m_List_Asm.InsertColumn(m_Flag_Opcode, TEXT("�ֽ�"), 0, 150);
	m_List_Asm.InsertColumn(m_Flag_Asm, TEXT("���"), 0, 0);
	m_List_Asm.InsertColumn(m_Flag_Asm_Show, TEXT("���"), 0, 250);
	m_List_Asm.InsertColumn(m_Flag_Mark, TEXT("��ע"), 0, 200);

	m_List_Heap.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Heap.InsertColumn(m_Flag_HeapAddress, TEXT("��ַ"), 0, 100);
	m_List_Heap.InsertColumn(m_Flag_HeapData, TEXT("����"), 0, 100);

	m_List_Memory.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Memory.InsertColumn(m_Flag_MemoryAddress, TEXT("��ַ"), 0, 70);
	for (DWORD i = 0; i < 16; i++)
	{
		CString nTemp;
		nTemp.Format(TEXT("%02X"), i);
		m_List_Memory.InsertColumn(i + 1, nTemp, 0, 33);
	}
	m_List_Memory.InsertColumn(m_Flag_MemoryStr, TEXT("�ı�"), 0, 200);


	m_Combo_Address.InsertString(m_Combo_Address.GetCount(), TEXT("MessageBoxA"));
	m_Combo_Address.InsertString(m_Combo_Address.GetCount(), TEXT("MessageBoxW"));
	m_Combo_Address.InsertString(m_Combo_Address.GetCount(), TEXT("OpenProcess"));
	m_Combo_Address.InsertString(m_Combo_Address.GetCount(), TEXT("TerminateProcess"));
	m_Combo_Address.InsertString(m_Combo_Address.GetCount(), TEXT("ReadProcessMemory"));
	m_Combo_Address.InsertString(m_Combo_Address.GetCount(), TEXT("WriteProcessMemory"));
	m_Combo_Address.InsertString(m_Combo_Address.GetCount(), TEXT("NtQueryInformationProcess"));
	m_Combo_Address.SetWindowText(TEXT("MessageBoxA"));
	for (DWORD i = 0; i < m_Num_Asm; i++) m_List_Asm.InsertItem(0, TEXT(""));
	for (DWORD i = 0; i < m_Num_Heap; i++) m_List_Heap.InsertItem(0, TEXT(""));
	for (DWORD i = 0; i < m_Num_Memory; i++) m_List_Memory.InsertItem(0, TEXT(""));

}
//*********************************************************************************
//���ò���˵�����
//*********************************************************************************
VOID DebugDlg::SetPluginMenu()
{
	CMenu *nMenu = m_Menu.GetSubMenu(4);
	for (DWORD i = 0; i < m_LoadPlugin.m_PluginName.size(); i++)
	{
		nMenu->AppendMenuW(MF_STRING, 0x1000 + i, m_LoadPlugin.m_PluginName[i]);
	}

}
//*********************************************************************************
//��ʼ���Ĵ������ڴ���Ϣ
//*********************************************************************************
VOID DebugDlg::InitInformation()
{
	m_ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Pid);
	if (m_ProcessHandle == nullptr)MessageBox(TEXT("�򿪽���ʧ�ܣ�"), TEXT("��ʾ"), MB_ICONERROR);

	m_Tid = GetProcessThreadId(m_Pid);
	m_ThreadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, m_Tid);
	if (m_ProcessHandle == nullptr)MessageBox(TEXT("���߳�ʧ�ܣ�"), TEXT("��ʾ"), MB_ICONERROR);

	InitAsm(GetOepAddress());
	PrintMemory(GetImageBassAddress());
	m_List_Asm.SetSelectionEx(0);
}
//*********************************************************************************
//��ʼ���Ĵ������ڴ���Ϣ�����ػ�ַ
//*********************************************************************************
VOID DebugDlg::InitInformationEx(DWORD nImageBase)
{
	m_ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Pid);
	if (m_ProcessHandle == nullptr)MessageBox(TEXT("�򿪽���ʧ�ܣ�"), TEXT("��ʾ"), MB_ICONERROR);

	m_Tid = GetProcessThreadId(m_Pid);
	m_ThreadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, m_Tid);
	if (m_ProcessHandle == nullptr)MessageBox(TEXT("���߳�ʧ�ܣ�"), TEXT("��ʾ"), MB_ICONERROR);

	InitAsm(GetOepAddressEx(nImageBase));
	PrintMemory(nImageBase);
	m_List_Asm.SetSelectionEx(0);
}
//*********************************************************************************
//��ʼ�������ָ���ַ
//*********************************************************************************
VOID DebugDlg::InitAsm(DWORD nReadAddress)
{

	BYTE nReadBuff[1024]{};
	CString nAddr;
	DISASM disAsm = { 0 };
	COLORREF nTextColor;
	COLORREF nBkColor;

	ReadMemoryBytes(nReadAddress, nReadBuff, 1024);

	disAsm.EIP = (UIntPtr)nReadBuff;
	disAsm.VirtualAddr = nReadAddress;
	disAsm.Archi = 0;
	disAsm.Options = 0x000;
	DWORD nOpcodeSize = 1024;
	INT nLen = 0;

	RepairAsm(nReadAddress, (DWORD)disAsm.VirtualAddr, nReadBuff);

	ClearItemColor();
	for (DWORD i = 0; i < m_Num_Asm; i++)
	{
		nLen = Disasm(&disAsm);
		if (nLen == -1)
		{
			disAsm.EIP += 1;
			disAsm.VirtualAddr += 1;
			continue;
		}

		PrintBreakFlag((DWORD)disAsm.VirtualAddr,i);
		nAddr.Format(TEXT("%08X"), disAsm.VirtualAddr);
		m_List_Asm.SetItemText(i, m_Flag_Address, nAddr);
		m_List_Asm.SetItemText(i, m_Flag_Address_Show, ShowExportName(nAddr));
		PrintOpcode((BYTE*)disAsm.EIP, nLen, i);
		m_List_Asm.SetItemText(i, m_Flag_Asm, disAsm.CompleteInstr);
		m_List_Asm.SetItemText(i, m_Flag_Asm_Show, ShowAsmName(nAddr,CString(disAsm.CompleteInstr)));

		disAsm.EIP += nLen;
		disAsm.VirtualAddr += nLen;

		RepairAsm(nReadAddress, (DWORD)disAsm.VirtualAddr, nReadBuff);

		if (IsColorWord(CString(disAsm.CompleteInstr), &nTextColor, &nBkColor))
		{
			SetItemColor(i, m_Flag_Asm, nTextColor, nBkColor);
			SetItemColor(i, m_Flag_Asm_Show, nTextColor, nBkColor);
		}
	}
	
	_stscanf_s(m_List_Asm.GetItemText(0, m_Flag_Address), TEXT("%x"), &nReadAddressFirst);
	_stscanf_s(m_List_Asm.GetItemText(1, m_Flag_Address), TEXT("%x"), &nReadAddressSecond);
	_stscanf_s(m_List_Asm.GetItemText(m_Num_Asm - 1, m_Flag_Address), TEXT("%x"), &nReadAddressEnd);

}
//*********************************************************************************
//��ӡ�����ָ���ַ
//*********************************************************************************
VOID DebugDlg::PrintOnceAsm(DWORD nReadAddress)
{
	BYTE nReadBuff[1024]{};
	CString nAddr;
	DISASM disAsm = { 0 };
	COLORREF nTextColor;
	COLORREF nBkColor;

	ReadMemoryBytes(nReadAddress, nReadBuff, 1024);

	disAsm.EIP = (UIntPtr)nReadBuff;
	disAsm.VirtualAddr = nReadAddress;
	disAsm.Archi = 0;
	disAsm.Options = 0x000;
	DWORD nOpcodeSize = 1024;

	INT nLen = 0;

	RepairAsm(nReadAddress, (DWORD)disAsm.VirtualAddr, nReadBuff);

	ClearItemColor();
	for (DWORD i = 0; i < m_Num_Asm; i++)
	{
		nLen = Disasm(&disAsm);
		if (nLen == -1)
		{
			disAsm.EIP += 1;
			disAsm.VirtualAddr += 1;
			continue;
		}
		
		PrintBreakFlag((DWORD)disAsm.VirtualAddr, i);
		nAddr.Format(TEXT("%08X"), disAsm.VirtualAddr);
		m_List_Asm.SetItemText(i, m_Flag_Address, nAddr);
		m_List_Asm.SetItemText(i, m_Flag_Address_Show, ShowExportName(nAddr));
		PrintOpcode((BYTE*)disAsm.EIP, nLen, i);
		m_List_Asm.SetItemText(i, m_Flag_Asm, disAsm.CompleteInstr);
		m_List_Asm.SetItemText(i, m_Flag_Asm_Show, ShowAsmName(nAddr, CString(disAsm.CompleteInstr)));

		disAsm.EIP += nLen;
		disAsm.VirtualAddr += nLen;

		RepairAsm(nReadAddress, (DWORD)disAsm.VirtualAddr, nReadBuff);

		if (IsColorWord(CString(disAsm.CompleteInstr), &nTextColor, &nBkColor))
		{
			SetItemColor(i, m_Flag_Asm, nTextColor, nBkColor);
			SetItemColor(i, m_Flag_Asm_Show, nTextColor, nBkColor);
		}
	}

	_stscanf_s(m_List_Asm.GetItemText(0, m_Flag_Address), TEXT("%x"), &nReadAddressFirst);
	_stscanf_s(m_List_Asm.GetItemText(1, m_Flag_Address), TEXT("%x"), &nReadAddressSecond);
	_stscanf_s(m_List_Asm.GetItemText(m_Num_Asm - 1, m_Flag_Address), TEXT("%x"), &nReadAddressEnd);

}
//*********************************************************************************
//������ʾ��ַ�еķ��ţ���ַ
//*********************************************************************************
CString DebugDlg::ShowExportName(CString szAddress)
{
	DWORD nAddress = 0;
	DWORD nMinAddress = 0;
	DWORD nMaxAddress = 0;

	_stscanf_s(szAddress, TEXT("%x"), &nAddress);

	//ѭ����������ģ���
	for (DWORD i = 0; i < m_Table.size(); i++)
	{
		//�����ģ�����ʼ��ַ�ͽ���
		nMinAddress = m_Table[i].ModuleAddress;
		nMaxAddress = nMinAddress + m_Table[i].ModuleSize;

		//�жϴ���ĵ�ַ�Ƿ����ڸ�ģ��
		if (nAddress >= nMinAddress && nAddress <= nMaxAddress)
		{
			//ȷ��ģ��󣬱�����ģ������е���������ַ
			for (DWORD x = 0; x < m_Table[i].ExportFunctionAddress.size(); x++)
			{
				//������������ĵ�ַ�봫��ĵ�ַ���
				if (nAddress == m_Table[i].ExportFunctionAddress[x])
				{
					//���ظõ��������ĵ�ַ����Ӧ��ģ�����͵�����������
					return CString(m_Table[i].ExportModuleName + CString(".") + m_Table[i].ExportFunctionName[x]);
				}
			}

			break;
		}


	}
	return szAddress;
}
//*********************************************************************************
//������ʾAsm�����еķ��ţ�Asm�������ڵ�ַ��Asm����
//*********************************************************************************
CString DebugDlg::ShowAsmName(CString szAddress,CString szAsm)
{

	if (szAsm.GetLength() < 12)return szAsm;


	DWORD nAddress = 0;
	DWORD nMinAddress = 0;
	DWORD nMaxAddress = 0;

	_stscanf_s(szAddress, TEXT("%x"), &nAddress);

	for (DWORD i = 0; i < m_Table.size(); i++)
	{
		nMinAddress = m_Table[i].ModuleAddress;
		nMaxAddress = nMinAddress + m_Table[i].ModuleSize;

		if (nAddress >= nMinAddress && nAddress <= nMaxAddress)
		{

			//����������
			for (DWORD x = 0; x < m_Table[i].ExportFunctionAddress.size(); x++)
			{
				
				INT nIndex = szAsm.Find(m_Table[i].ExportFunctionAddress_str[x]);

				if (nIndex!=-1)
				{
					szAsm.Replace(m_Table[i].ExportFunctionAddress_str[x] + TEXT("h"), m_Table[i].ExportFunctionName[x]);
					return szAsm;
				}
			}


			//���������������Ϣ
			for (DWORD x = 0; x < m_Table[i].ImportFunctionAddress.size(); x++)
			{
				//�ڻ�����������Ƿ��е��뺯���ĵ�ַ
				INT nIndex = szAsm.Find(m_Table[i].ImportFunctionAddress_str[x]);
				//���ҵ��˵��뺯���ĵ�ַ
				if (nIndex != -1)
				{	
					//ֱ�ӽ��������ڵĵ�ַ�滻Ϊ����ģ�����뵼�뺯����
					szAsm.Replace(m_Table[i].ImportFunctionAddress_str[x] + TEXT("h"), m_Table[i].ImportModuleName[x] + CString(".") + m_Table[i].ImportFunctionName[x]);
					return szAsm;
				}

				nIndex = szAsm.Find(m_Table[i].IAT_str[x]);

				if (nIndex != -1)
				{
					szAsm.Replace(m_Table[i].IAT_str[x] + TEXT("h"), m_Table[i].ImportModuleName[x] + CString(".") + m_Table[i].ImportFunctionName[x]);
					return szAsm;
				}


			}


			break;
		}
	}

	return szAsm;

}
//*********************************************************************************
//���ִ���������
//*********************************************************************************
BOOL DebugDlg::CheckCommand(CString nCommand)
{
	DWORD nReadAddress;
	CString nTempAddress;

	if (nCommand.Left(3)=="bp ")
	{
		nTempAddress = nCommand.Right(nCommand.GetLength() - 3);

		nReadAddress = GetApiAddress(nTempAddress);
		if (nReadAddress)SetBreakPoint(nReadAddress, FALSE, {});
		else
		{
			_stscanf_s(nTempAddress, TEXT("%x"), &nReadAddress);
			SetBreakPoint(nReadAddress, FALSE, {});
		}
		m_Static_Command.SetWindowText(TEXT("��������ϵ�ɹ�"));

		return TRUE;
	}
	else if (nCommand.Left(3) == "bp ")
	{
		nTempAddress = nCommand.Right(nCommand.GetLength() - 3);

		nReadAddress = GetApiAddress(nTempAddress);
		if (nReadAddress)SetBreakPoint(nReadAddress, FALSE, {});
		else
		{
			_stscanf_s(nTempAddress, TEXT("%x"), &nReadAddress);
			SetHardBreakPoint(nReadAddress, FALSE, {});
		}
		m_Static_Command.SetWindowText(TEXT("����Ӳ���ϵ�ɹ�"));

		return TRUE;
	}

	return FALSE;
}
//*********************************************************************************
//��ʾһ��Asm������ʾ�ĵ�ַ
//*********************************************************************************
VOID DebugDlg::ShowSingleAsm(DWORD nShowAddress)
{

	if (m_List_Asm.GetSelectionItem() == m_List_Asm.GetItemCount() - 2)
	{
		g_pDlg->PrintOnceAsm(nReadAddressSecond);

		INT nIndex = FindAddress(nShowAddress);
		if (nIndex != -1)m_List_Asm.SetSelectionEx(nIndex);
		else
		{
			PrintOnceAsm(nShowAddress);
			m_List_Asm.SetSelectionEx(0);
		}
	}
	else
	{

		INT nIndex = FindAddress(nShowAddress);
		if (nIndex != -1)m_List_Asm.SetSelectionEx(nIndex);
		else
		{
			PrintOnceAsm(nShowAddress);
			m_List_Asm.SetSelectionEx(0);
		}
	}
}
//*********************************************************************************
//�޸�int3�ϵ��opcode��Ҫ�޸��ĵ�ַ�������������ַ�����޸�opcode
//*********************************************************************************
VOID DebugDlg::RepairAsm(DWORD nReadAddress, DWORD nVirtualAddress, BYTE *nRepairBytes)
{
	DWORD nIsBreakPoint;
	nIsBreakPoint = IsBreakPoint(nVirtualAddress);
	if (nIsBreakPoint != -1)
	{
		nRepairBytes[nVirtualAddress - nReadAddress] = m_BreakPoint[nIsBreakPoint].nData;
	}

	DWORD nIsTempBreakPoint;
	nIsTempBreakPoint = IsTempBreakPoint(nVirtualAddress);
	if (nIsTempBreakPoint != -1)
	{
		nRepairBytes[nVirtualAddress - nReadAddress] = m_TempBreakPoint[nIsTempBreakPoint].nData;
	}
}
//*********************************************************************************
// ��ӡopcode��opcode��ַ����ӡ���ȣ��ڼ���
//*********************************************************************************
VOID DebugDlg::PrintOpcode(BYTE* pOpcode, DWORD nSize, DWORD nItem)
{
	CString nTemp;
	CString nOpcode;
	for (DWORD i = 0; i < nSize; ++i)
	{
		nTemp.Format(TEXT("%02X "), pOpcode[i]);
		nOpcode += nTemp;
	}

	m_List_Asm.SetItemText(nItem, g_pDlg->m_Flag_Opcode, nOpcode);

}
//*********************************************************************************
// ˢ����ʾ�ϵ�ĸ�����־���ϵ��ַ���ڼ���
//*********************************************************************************
VOID DebugDlg::PrintBreakFlag(DWORD nBreakAddress, DWORD nItem)
{
	CString nAddr;
	if (IsBreakPoint(nBreakAddress) != -1)
	{
		nAddr = TEXT("��");
		SetItemColor(nItem, m_Flag_Break, COLOR_WHITE, COLOR_RED);
		SetItemColor(nItem, m_Flag_Address, COLOR_WHITE, COLOR_RED);
		SetItemColor(nItem, m_Flag_Address_Show, COLOR_WHITE, COLOR_RED);
	}
	else if (IsHardBreakPoint(nBreakAddress) != -1) 
	{ 
		nAddr = TEXT("��"); 
		SetItemColor(nItem, m_Flag_Break, COLOR_WHITE, COLOR_BLACK);
		SetItemColor(nItem, m_Flag_Address, COLOR_WHITE, COLOR_BLACK);
		SetItemColor(nItem, m_Flag_Address_Show, COLOR_WHITE, COLOR_BLACK);
	}
	else if (IsMemoryBreakPoint(nBreakAddress))
	{
		nAddr = TEXT("��");
		SetItemColor(nItem, m_Flag_Break, COLOR_WHITE, COLOR_ORANGE);
		SetItemColor(nItem, m_Flag_Address, COLOR_WHITE, COLOR_ORANGE);
		SetItemColor(nItem, m_Flag_Address_Show, COLOR_WHITE, COLOR_ORANGE);
	}
	else nAddr = TEXT("");
	m_List_Asm.SetItemText(nItem, m_Flag_Break, nAddr);
}
//*********************************************************************************
// ��ӡ�Ĵ���ֵ���߳�������
//*********************************************************************************
VOID DebugDlg::PrintContext(CONTEXT &nCONTEXT)
{
	CString nTemp;
	CString nOldData;
	PEFLAGS m_EFlags = nullptr;
	m_EFlags = (PEFLAGS)&nCONTEXT.EFlags;

	nTemp.Format(TEXT("%08X"), nCONTEXT.Eax);
	m_Edit_Eax.GetWindowText(nOldData);
	m_Edit_Eax.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EaxColor = RGB(255, 0, 0);
	}
	else m_EaxColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Ebx);
	m_Edit_Ebx.GetWindowText(nOldData);
	m_Edit_Ebx.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EbxColor = RGB(255, 0, 0);
	}
	else m_EbxColor = RGB(0, 0, 0);



	nTemp.Format(TEXT("%08X"), nCONTEXT.Ecx);
	m_Edit_Ecx.GetWindowText(nOldData);
	m_Edit_Ecx.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EcxColor = RGB(255, 0, 0);
	}
	else m_EcxColor = RGB(0, 0, 0);



	nTemp.Format(TEXT("%08X"), nCONTEXT.Edx);
	m_Edit_Edx.GetWindowText(nOldData);
	m_Edit_Edx.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EdxColor = RGB(255, 0, 0);
	}
	else m_EdxColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Esi);
	m_Edit_Esi.GetWindowText(nOldData);
	m_Edit_Esi.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EsiColor = RGB(255, 0, 0);
	}
	else m_EsiColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Edi);
	m_Edit_Edi.GetWindowText(nOldData);
	m_Edit_Edi.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EdiColor = RGB(255, 0, 0);
	}
	else m_EdiColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Ebp);
	m_Edit_Ebp.GetWindowText(nOldData);
	m_Edit_Ebp.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EbpColor = RGB(255, 0, 0);
	}
	else m_EbpColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Esp);
	m_Edit_Esp.GetWindowText(nOldData);
	m_Edit_Esp.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EspColor = RGB(255, 0, 0);
	}
	else m_EspColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Eip);
	m_Edit_Eip.GetWindowText(nOldData);
	m_Edit_Eip.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EipColor = RGB(255, 0, 0);
	}
	else m_EipColor = RGB(0, 0, 0);



	nTemp.Format(TEXT("%08X"), nCONTEXT.Dr0);
	m_Edit_Dr0.GetWindowText(nOldData);
	m_Edit_Dr0.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_Dr0Color = RGB(255, 0, 0);
	}
	else m_Dr0Color = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Dr1);
	m_Edit_Dr1.GetWindowText(nOldData);
	m_Edit_Dr1.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_Dr1Color = RGB(255, 0, 0);
	}
	else m_Dr1Color = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Dr2);
	m_Edit_Dr2.GetWindowText(nOldData);
	m_Edit_Dr2.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_Dr2Color = RGB(255, 0, 0);
	}
	else m_Dr2Color = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Dr3);
	m_Edit_Dr3.GetWindowText(nOldData);
	m_Edit_Dr3.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_Dr3Color = RGB(255, 0, 0);
	}
	else m_Dr3Color = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Dr6);
	m_Edit_Dr6.GetWindowText(nOldData);
	m_Edit_Dr6.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_Dr6Color = RGB(255, 0, 0);
	}
	else m_Dr6Color = RGB(0, 0, 0);


	nTemp.Format(TEXT("%08X"), nCONTEXT.Dr7);
	m_Edit_Dr7.GetWindowText(nOldData);
	m_Edit_Dr7.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_Dr7Color = RGB(255, 0, 0);
	}
	else m_Dr7Color = RGB(0, 0, 0);


	nTemp.Format(TEXT("%04X"), nCONTEXT.SegEs);
	m_Edit_Es.GetWindowText(nOldData);
	m_Edit_Es.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_EsColor = RGB(255, 0, 0);
	}
	else m_EsColor = RGB(0, 0, 0);

	nTemp.Format(TEXT("%04X"), nCONTEXT.SegCs);
	m_Edit_Cs.GetWindowText(nOldData);
	m_Edit_Cs.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_CsColor = RGB(255, 0, 0);
	}
	else m_CsColor = RGB(0, 0, 0);

	nTemp.Format(TEXT("%04X"), nCONTEXT.SegSs);
	m_Edit_Ss.GetWindowText(nOldData);
	m_Edit_Ss.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_SsColor = RGB(255, 0, 0);
	}
	else m_SsColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%04X"), nCONTEXT.SegDs);
	m_Edit_Ds.GetWindowText(nOldData);
	m_Edit_Ds.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_DsColor = RGB(255, 0, 0);
	}
	else m_DsColor = RGB(0, 0, 0);

	nTemp.Format(TEXT("%04X"), nCONTEXT.SegFs);
	m_Edit_Fs.GetWindowText(nOldData);
	m_Edit_Fs.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_FsColor = RGB(255, 0, 0);
	}
	else m_FsColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%04X"), nCONTEXT.SegGs);
	m_Edit_Gs.GetWindowText(nOldData);
	m_Edit_Gs.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_GsColor = RGB(255, 0, 0);
	}
	else m_GsColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%X"), m_EFlags->CF);
	m_Edit_Cf.GetWindowText(nOldData);
	m_Edit_Cf.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_CfColor = RGB(255, 0, 0);
	}
	else m_CfColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%X"), m_EFlags->PF);
	m_Edit_Pf.GetWindowText(nOldData);
	m_Edit_Pf.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_PfColor = RGB(255, 0, 0);
	}
	else m_PfColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%X"), m_EFlags->AF);
	m_Edit_Af.GetWindowText(nOldData);
	m_Edit_Af.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_AfColor = RGB(255, 0, 0);
	}
	else m_AfColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%X"), m_EFlags->ZF);
	m_Edit_Zf.GetWindowText(nOldData);
	m_Edit_Zf.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_ZfColor = RGB(255, 0, 0);
	}
	else m_ZfColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%X"), m_EFlags->SF);
	m_Edit_Sf.GetWindowText(nOldData);
	m_Edit_Sf.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_SfColor = RGB(255, 0, 0);
	}
	else m_SfColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%X"), m_EFlags->TF);
	m_Edit_Tf.GetWindowText(nOldData);
	m_Edit_Tf.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_TfColor = RGB(255, 0, 0);
	}
	else m_TfColor = RGB(0, 0, 0);


	nTemp.Format(TEXT("%X"), m_EFlags->DF);
	m_Edit_Df.GetWindowText(nOldData);
	m_Edit_Df.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_DfColor = RGB(255, 0, 0);
	}
	else m_DfColor = RGB(0, 0, 0);

	nTemp.Format(TEXT("%X"), m_EFlags->OF);
	m_Edit_Of.GetWindowText(nOldData);
	m_Edit_Of.SetWindowText(nTemp);
	if (nTemp != nOldData)
	{
		m_OfColor = RGB(255, 0, 0);
	}
	else m_OfColor = RGB(0, 0, 0);

}
//*********************************************************************************
// ��ӡջ�����ݣ��߳�������
//*********************************************************************************
VOID DebugDlg::PrintHeap(CONTEXT &nCONTEXT)
{

	DWORD nReadData;
	CString nShowData;
	for (DWORD i = 0; i < m_Num_Heap; i++)
	{
		nShowData.Format(TEXT("%08X+%X"), nCONTEXT.Esp, (i * 4));
		m_List_Heap.SetItemText(i, m_Flag_HeapAddress, nShowData);
		ReadMemoryDword(nCONTEXT.Esp + (i * 4), &nReadData);
		nShowData.Format(TEXT("%08X"), nReadData);
		m_List_Heap.SetItemText(i, m_Flag_HeapData, nShowData);
	}

}
//*********************************************************************************
// ��ӡ�ڴ����ݣ��ڴ��ַ
//*********************************************************************************
VOID DebugDlg::PrintMemory(DWORD nAddress)
{
	BYTE nValue[16]{};
	BYTE nStrValue[17]{};

	CString nTemp;

	for (DWORD i = 0; i < m_Num_Memory; i++)
	{
		nTemp.Format(TEXT("%08X"), nAddress + (i * _countof(nValue)));
		m_List_Memory.SetItemText(i, m_Flag_MemoryAddress, nTemp);

		ReadMemoryBytes(nAddress + (i * _countof(nValue)), nValue, _countof(nValue));
		for (DWORD x = 0; x < _countof(nValue); x++)
		{
				nTemp.Format(TEXT("%02X "), nValue[x]);
				m_List_Memory.SetItemText(i, x + 1, nTemp);
		}


		for (DWORD x = 0; x < _countof(nValue); x++)
		{
			if (!nValue[x])nValue[x] = '.';
		}
		memcpy_s(nStrValue, _countof(nStrValue), nValue, _countof(nValue));
		m_List_Memory.SetItemText(i, m_Flag_MemoryStr, CString(nStrValue));
		ZeroMemory(nValue, _countof(nValue));
		ZeroMemory(nStrValue, _countof(nStrValue));
	}
}






//*********************************************************************************
// ����Ӳ���ϵ����
BOOL DebugDlg::SetHardBreakPoint(DWORD nBreakAddress, BOOL nIsCondotion, CONDITION nCondition)
{
	CONTEXT nContext{};
	PDR7 nDr7 = nullptr;
	nContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(m_ThreadHandle, &nContext);
	nDr7 = (PDR7)&nContext.Dr7;

	if (nContext.Dr0 == 0)
	{
		m_HardBreakPoint[0].nAddress = nBreakAddress;
		m_HardBreakPoint[0].nType = 0;
		m_HardBreakPoint[0].nSize = 0;
		m_HardBreakPoint[0].nIsCondition = nIsCondotion;
		m_HardBreakPoint[0].nCondition = nCondition;

		nContext.Dr0 = nBreakAddress;
		nDr7->Dr0_L = 1;
		nDr7->Dr0_LEN = 0;
		nDr7->Dr0_RWE = 0;
		SetThreadContext(m_ThreadHandle, &nContext);
		return TRUE;
	}
	else if (nContext.Dr1 == 0)
	{
		m_HardBreakPoint[1].nAddress = nBreakAddress;
		m_HardBreakPoint[1].nType = 0;
		m_HardBreakPoint[1].nSize = 0;
		m_HardBreakPoint[1].nIsCondition = nIsCondotion;
		m_HardBreakPoint[1].nCondition = nCondition;

		nContext.Dr1 = nBreakAddress;
		nDr7->Dr1_L = 1;
		nDr7->Dr1_LEN = 0;
		nDr7->Dr1_RWE = 0;
		SetThreadContext(m_ThreadHandle, &nContext);
		return TRUE;
	}
	else if (nContext.Dr2 == 0)
	{
		m_HardBreakPoint[2].nAddress = nBreakAddress;
		m_HardBreakPoint[2].nType = 0;
		m_HardBreakPoint[2].nSize = 0;
		m_HardBreakPoint[2].nIsCondition = nIsCondotion;
		m_HardBreakPoint[2].nCondition = nCondition;

		nContext.Dr2 = nBreakAddress;
		nDr7->Dr2_L = 1;
		nDr7->Dr2_LEN = 0;
		nDr7->Dr2_RWE = 0;
		SetThreadContext(m_ThreadHandle, &nContext);
		return TRUE;
	}
	else if (nContext.Dr3 == 0)
	{
		m_HardBreakPoint[3].nAddress = nBreakAddress;
		m_HardBreakPoint[3].nType = 0;
		m_HardBreakPoint[3].nSize = 0;
		m_HardBreakPoint[3].nIsCondition = nIsCondotion;
		m_HardBreakPoint[3].nCondition = nCondition;

		nContext.Dr3 = nBreakAddress;
		nDr7->Dr3_L = 1;
		nDr7->Dr3_LEN = 0;
		nDr7->Dr3_RWE = 0;
		SetThreadContext(m_ThreadHandle, &nContext);
		return TRUE;
	}
	return FALSE;
}
BOOL DebugDlg::SetHardBreakPointEx(DWORD nBreakAddress,DWORD nType,DWORD nSize)
{
	CONTEXT nContext{};
	PDR7 nDr7 = nullptr;
	nContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(m_ThreadHandle, &nContext);
	nDr7 = (PDR7)&nContext.Dr7;

	if (nContext.Dr0 == 0)
	{
		m_HardBreakPoint[0].nAddress = nBreakAddress;
		m_HardBreakPoint[0].nType = nType;
		m_HardBreakPoint[0].nSize = nSize;
		nContext.Dr0 = nBreakAddress;
		nDr7->Dr0_L = 1;
		nDr7->Dr0_LEN = nSize;
		nDr7->Dr0_RWE = nType;
		SetThreadContext(m_ThreadHandle, &nContext);
		return TRUE;
	}
	else if (nContext.Dr1 == 0)
	{
		m_HardBreakPoint[1].nAddress = nBreakAddress;
		m_HardBreakPoint[1].nType = nType;
		m_HardBreakPoint[1].nSize = nSize;
		nContext.Dr1 = nBreakAddress;
		nDr7->Dr1_L = 1;
		nDr7->Dr1_LEN = nSize;
		nDr7->Dr1_RWE = nType;
		SetThreadContext(m_ThreadHandle, &nContext);
		return TRUE;
	}
	else if (nContext.Dr2 == 0)
	{
		m_HardBreakPoint[2].nAddress = nBreakAddress;
		m_HardBreakPoint[2].nType = nType;
		m_HardBreakPoint[2].nSize = nSize;
		nContext.Dr2 = nBreakAddress;
		nDr7->Dr2_L = 1;
		nDr7->Dr2_LEN = nSize;
		nDr7->Dr2_RWE = nType;
		SetThreadContext(m_ThreadHandle, &nContext);
		return TRUE;
	}
	else if (nContext.Dr3 == 0)
	{
		m_HardBreakPoint[3].nAddress = nBreakAddress;
		m_HardBreakPoint[3].nType = nType;
		m_HardBreakPoint[3].nSize = nSize;
		nContext.Dr3 = nBreakAddress;
		nDr7->Dr3_L = 1;
		nDr7->Dr3_LEN = nSize;
		nDr7->Dr3_RWE = nType;
		SetThreadContext(m_ThreadHandle, &nContext);
		return TRUE;
	}
	return FALSE;
}
VOID DebugDlg::AddBreakReadWrite(DWORD nBreakAddress, CString nAsm)
{
	BOOL nHave = FALSE;
	if (m_HardBreakReadWrite.size()==0)
	{
		m_HardBreakReadWrite.push_back({ nBreakAddress ,nAsm });
		return;
	}

	for (DWORD i = 0; i < m_HardBreakReadWrite.size(); i++)
	{
		if (m_HardBreakReadWrite[i].nAddress == nBreakAddress)
		{
			nHave = TRUE;
			break;
		}
	}

	if (!nHave)	m_HardBreakReadWrite.push_back({ nBreakAddress ,nAsm });

}
VOID DebugDlg::ClearBreakReadWrite()
{
	m_HardBreakReadWrite.clear();
	m_HardBreakReadWrite.swap(vector<BREAKREADWRITE>());
}
CString DebugDlg::GetOnceAsm(DWORD nReadAddress)
{
	BYTE nReadBuff[128]{};
	CString nAddr;
	DISASM disAsm = { 0 };

	ReadMemoryBytes(nReadAddress, nReadBuff, 128);

	disAsm.EIP = (UIntPtr)nReadBuff;
	disAsm.VirtualAddr = nReadAddress;
	disAsm.Archi = 0;
	disAsm.Options = 0x000;
	DWORD nOpcodeSize = 128;

	Disasm(&disAsm);

	return CString(disAsm.CompleteInstr);

}
VOID DebugDlg::SetTempHardBreakPoint(DWORD nBreakAddress)
{
	CONTEXT nContext{};
	PDR7 nDr7 = nullptr;
	nContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(m_ThreadHandle, &nContext);
	nDr7 = (PDR7)&nContext.Dr7;

	nContext.Dr0 = nBreakAddress;
	nDr7->Dr0_L = 1;
	nDr7->Dr0_LEN = 0;
	nDr7->Dr0_RWE = 0;
	SetThreadContext(m_ThreadHandle, &nContext);
	m_TempHardBreakPoint[0] = nBreakAddress;

}
VOID DebugDlg::DelHardBreakPoint(DWORD nBreakAddress)
{
	DWORD nIndex;
	CONTEXT nContext{};
	PDR7 nDr7 = nullptr;
	nContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(m_ThreadHandle, &nContext);
	nDr7 = (PDR7)&nContext.Dr7;

	nIndex = IsHardBreakPoint(nBreakAddress);
	if (nIndex == -1)return;

	if (nIndex == 0)
	{
		nContext.Dr0 = 0;
		nDr7->Dr0_L = 0;
		nDr7->Dr0_LEN = 0;
		nDr7->Dr0_RWE = 0;
		m_HardBreakPoint[0].nAddress = 0;
		m_HardBreakPoint[0].nType = 0;

	}
	else if (nIndex == 1)
	{
		nContext.Dr1 = 0;
		nDr7->Dr1_L = 0;
		nDr7->Dr1_LEN = 0;
		nDr7->Dr1_RWE = 0;
		m_HardBreakPoint[1].nAddress = 0;
		m_HardBreakPoint[1].nType = 0;
	}
	else if (nIndex == 2)
	{
		nContext.Dr2 = 0;
		nDr7->Dr2_L = 0;
		nDr7->Dr2_LEN = 0;
		nDr7->Dr2_RWE = 0;
		m_HardBreakPoint[2].nAddress = 0;
		m_HardBreakPoint[2].nType = 0;
	}
	else if (nIndex == 3)
	{
		nContext.Dr3 = 0;
		nDr7->Dr3_L = 0;
		nDr7->Dr3_LEN = 0;
		nDr7->Dr3_RWE = 0;
		m_HardBreakPoint[3].nAddress = 0;
		m_HardBreakPoint[3].nType = 0;
	}
	SetThreadContext(m_ThreadHandle, &nContext);
}
VOID DebugDlg::DelTempHardBreakPoint(DWORD nBreakAddress)
{
	DWORD nIndex;

	nIndex = IsTempHardBreakPoint(nBreakAddress);
	if (nIndex == -1)return;

	//CONTEXT nContext{};
	PDR7 nDr7 = nullptr;
	//nContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	//GetThreadContext(m_ThreadHandle, &nContext);
	nDr7 = (PDR7)&g_Context.Dr7;

	g_Context.Dr0 = 0;
	nDr7->Dr0_L = 0;
	nDr7->Dr0_LEN = 0;
	nDr7->Dr0_RWE = 0;
	SetThreadContext(m_ThreadHandle, &g_Context);

	m_TempHardBreakPoint[nIndex] = 0;
}
VOID DebugDlg::OpenAllHardBreakPoint()
{
	//CONTEXT nContext{};
	PDR7 nDr7 = nullptr;
	//nContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	//GetThreadContext(m_ThreadHandle, &nContext);
	nDr7 = (PDR7)&g_Context.Dr7;

	if (m_HardBreakPoint[0].nAddress != 0)
	{
		g_Context.Dr0 = m_HardBreakPoint[0].nAddress;
		nDr7->Dr0_L = 1;
		nDr7->Dr0_LEN = m_HardBreakPoint[0].nSize;
		nDr7->Dr0_RWE = m_HardBreakPoint[0].nType;
	}
	if (m_HardBreakPoint[1].nAddress != 0)
	{
		g_Context.Dr1 = m_HardBreakPoint[1].nAddress;
		nDr7->Dr1_L = 1;
		nDr7->Dr1_LEN = m_HardBreakPoint[1].nSize;
		nDr7->Dr1_RWE = m_HardBreakPoint[1].nType;
	}
	if (m_HardBreakPoint[2].nAddress != 0)
	{
		g_Context.Dr2 = m_HardBreakPoint[2].nAddress;
		nDr7->Dr2_L = 1;
		nDr7->Dr2_LEN = m_HardBreakPoint[2].nSize;
		nDr7->Dr2_RWE = m_HardBreakPoint[2].nType;
	}
	if (m_HardBreakPoint[3].nAddress != 0)
	{
		g_Context.Dr3 = m_HardBreakPoint[3].nAddress;
		nDr7->Dr3_L = 1;
		nDr7->Dr3_LEN = m_HardBreakPoint[3].nSize;
		nDr7->Dr3_RWE = m_HardBreakPoint[3].nType;
	}
	SetThreadContext(m_ThreadHandle, &g_Context);
}
VOID DebugDlg::OpenAllHardBreakPointEx()
{
	CONTEXT nContext{};
	PDR7 nDr7 = nullptr;
	nContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	GetThreadContext(m_ThreadHandle, &nContext);
	nDr7 = (PDR7)&nContext.Dr7;

	if (m_HardBreakPoint[0].nAddress != 0)
	{
		nContext.Dr0 = m_HardBreakPoint[0].nAddress;
		nDr7->Dr0_L = 1;
		nDr7->Dr0_LEN = m_HardBreakPoint[0].nSize;
		nDr7->Dr0_RWE = m_HardBreakPoint[0].nType;
	}
	if (m_HardBreakPoint[1].nAddress != 0)
	{
		nContext.Dr1 = m_HardBreakPoint[1].nAddress;
		nDr7->Dr1_L = 1;
		nDr7->Dr1_LEN = m_HardBreakPoint[1].nSize;
		nDr7->Dr1_RWE = m_HardBreakPoint[1].nType;
	}
	if (m_HardBreakPoint[2].nAddress != 0)
	{
		nContext.Dr2 = m_HardBreakPoint[2].nAddress;
		nDr7->Dr2_L = 1;
		nDr7->Dr2_LEN = m_HardBreakPoint[2].nSize;
		nDr7->Dr2_RWE = m_HardBreakPoint[2].nType;
	}
	if (m_HardBreakPoint[3].nAddress != 0)
	{
		nContext.Dr3 = m_HardBreakPoint[3].nAddress;
		nDr7->Dr3_L = 1;
		nDr7->Dr3_LEN = m_HardBreakPoint[3].nSize;
		nDr7->Dr3_RWE = m_HardBreakPoint[3].nType;
	}
	SetThreadContext(m_ThreadHandle, &nContext);
}
VOID DebugDlg::CloseAllHardBreakPoint()
{
	//CONTEXT nContext{};
	PDR7 nDr7 = nullptr;
	//nContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	//GetThreadContext(m_ThreadHandle, &nContext);
	nDr7 = (PDR7)&g_Context.Dr7;

	g_Context.Dr0 = 0;
	g_Context.Dr1 = 0;
	g_Context.Dr2 = 0;
	g_Context.Dr3 = 0;

	nDr7->Dr0_L = 0;
	nDr7->Dr0_LEN = 0;
	nDr7->Dr0_RWE = 0;
	nDr7->Dr1_L = 0;
	nDr7->Dr1_LEN = 0;
	nDr7->Dr1_RWE = 0;
	nDr7->Dr2_L = 0;
	nDr7->Dr2_LEN = 0;
	nDr7->Dr2_RWE = 0;
	nDr7->Dr3_L = 0;
	nDr7->Dr3_LEN = 0;
	nDr7->Dr3_RWE = 0;
	SetThreadContext(m_ThreadHandle, &g_Context);
}
DWORD DebugDlg::IsHardBreakPoint(DWORD nBreakAddress)
{
	if (m_HardBreakPoint[0].nAddress == nBreakAddress)return 0;
	else if (m_HardBreakPoint[1].nAddress == nBreakAddress)return 1;
	else if (m_HardBreakPoint[2].nAddress == nBreakAddress)return 2;
	else if (m_HardBreakPoint[3].nAddress == nBreakAddress)return 3;
	return -1;
}
DWORD DebugDlg::IsHardBreakPointEx(DWORD nBreakAddress)
{

	DR6 *pDr6 = nullptr;
	pDr6 = (DR6*)&g_Context.Dr6;

	if (pDr6->Dr0)
	{
		if (m_HardBreakPoint[0].nAddress == g_Context.Dr0 && m_HardBreakPoint[0].nType)return 0;
		else if (m_HardBreakPoint[1].nAddress == g_Context.Dr0 && m_HardBreakPoint[1].nType)return 1;
		else if (m_HardBreakPoint[2].nAddress == g_Context.Dr0 && m_HardBreakPoint[2].nType)return 2;
		else if (m_HardBreakPoint[3].nAddress == g_Context.Dr0 && m_HardBreakPoint[3].nType)return 3;
	}
	else if (pDr6->Dr1)
	{
	if (m_HardBreakPoint[0].nAddress == g_Context.Dr1 && m_HardBreakPoint[0].nType)return 0;
	else if (m_HardBreakPoint[1].nAddress == g_Context.Dr1 && m_HardBreakPoint[1].nType)return 1;
	else if (m_HardBreakPoint[2].nAddress == g_Context.Dr1 && m_HardBreakPoint[2].nType)return 2;
	else if (m_HardBreakPoint[3].nAddress == g_Context.Dr1 && m_HardBreakPoint[3].nType)return 3;

	}
	else if (pDr6->Dr2)
	{
	if (m_HardBreakPoint[0].nAddress == g_Context.Dr2 && m_HardBreakPoint[0].nType)return 0;
	else if (m_HardBreakPoint[1].nAddress == g_Context.Dr2 && m_HardBreakPoint[1].nType)return 1;
	else if (m_HardBreakPoint[2].nAddress == g_Context.Dr2 && m_HardBreakPoint[2].nType)return 2;
	else if (m_HardBreakPoint[3].nAddress == g_Context.Dr2 && m_HardBreakPoint[3].nType)return 3;

	}
	else if (pDr6->Dr3)
	{
	if (m_HardBreakPoint[0].nAddress == g_Context.Dr3 && m_HardBreakPoint[0].nType)return 0;
	else if (m_HardBreakPoint[1].nAddress == g_Context.Dr3 && m_HardBreakPoint[1].nType)return 1;
	else if (m_HardBreakPoint[2].nAddress == g_Context.Dr3 && m_HardBreakPoint[2].nType)return 2;
	else if (m_HardBreakPoint[3].nAddress == g_Context.Dr3 && m_HardBreakPoint[3].nType)return 3;
	}

	return -1;
}
DWORD DebugDlg::IsTempHardBreakPoint(DWORD nBreakAddress)
{
	for (DWORD i = 0; i < 4; i++)
	{
		if (m_TempHardBreakPoint[i] == nBreakAddress)
		{
			return i;
		}
	}
	return -1;
}
DWORD DebugDlg::IsConditionHardBreakPoint(DWORD nBreakPointIndex)
{
	if (nBreakPointIndex == -1)return FALSE;
	return m_HardBreakPoint[nBreakPointIndex].nIsCondition;
}
DWORD DebugDlg::IsConditionSucessHardBreakPoint(DWORD nBreakPointIndex, CONTEXT nContext)
{
	if (nBreakPointIndex == -1)return FALSE;

	if (m_HardBreakPoint[nBreakPointIndex].nCondition.OPEN_EAX)
	{
		if (m_HardBreakPoint[nBreakPointIndex].nCondition.EAX != nContext.Eax)return FALSE;
	}
	if (m_HardBreakPoint[nBreakPointIndex].nCondition.OPEN_EBX)
	{
		if (m_HardBreakPoint[nBreakPointIndex].nCondition.EBX != nContext.Ebx)return FALSE;
	}
	if (m_HardBreakPoint[nBreakPointIndex].nCondition.OPEN_ECX)
	{
		if (m_HardBreakPoint[nBreakPointIndex].nCondition.ECX != nContext.Ecx)return FALSE;
	}
	if (m_HardBreakPoint[nBreakPointIndex].nCondition.OPEN_EDX)
	{
		if (m_HardBreakPoint[nBreakPointIndex].nCondition.EDX != nContext.Edx)return FALSE;
	}
	if (m_HardBreakPoint[nBreakPointIndex].nCondition.OPEN_ESI)
	{
		if (m_HardBreakPoint[nBreakPointIndex].nCondition.ESI != nContext.Esi)return FALSE;
	}
	if (m_HardBreakPoint[nBreakPointIndex].nCondition.OPEN_EDI)
	{
		if (m_HardBreakPoint[nBreakPointIndex].nCondition.EDI != nContext.Edi)return FALSE;
	}
	if (m_HardBreakPoint[nBreakPointIndex].nCondition.OPEN_EBP)
	{
		if (m_HardBreakPoint[nBreakPointIndex].nCondition.EBP != nContext.Ebp)return FALSE;
	}
	if (m_HardBreakPoint[nBreakPointIndex].nCondition.OPEN_ESP)
	{
		if (m_HardBreakPoint[nBreakPointIndex].nCondition.ESP != nContext.Esp)return FALSE;
	}
	return TRUE;
}
//*********************************************************************************




//*********************************************************************************
// �����ڴ�ϵ����
VOID DebugDlg::SetMemoryBreakPoint(DWORD nBreakAddress, DWORD nType, BOOL nIsCondotion, CONDITION nCondotion)
{
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nBreakAddress, 1, PAGE_NOACCESS, &m_MemoryBreakPoint.nOldPageProtect);

	m_MemoryBreakPoint.nAddress = nBreakAddress;
	m_MemoryBreakPoint.nType = nType;
	m_MemoryBreakPoint.nIsCondition = nIsCondotion;
	m_MemoryBreakPoint.nCondition = nCondotion;
}
BOOL DebugDlg::IsMemoryBreakPoint(DWORD nBreakAddress)
{
	if (m_MemoryBreakPoint.nAddress == nBreakAddress) return TRUE;
	else return FALSE;
}
BOOL DebugDlg::HaveMemoryBreakPoint()
{
	if (m_MemoryBreakPoint.nAddress == 0) return FALSE;
	else return TRUE;
}
VOID DebugDlg::DelMemoryBreakPoint(DWORD nBreakAddress)
{
	DWORD nOldProtece;
	
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nBreakAddress, 1, m_MemoryBreakPoint.nOldPageProtect, &nOldProtece);

	ZeroMemory(&m_MemoryBreakPoint, sizeof(MEMORYBREAKPOINT));

}
DWORD DebugDlg::GetMemoryBreakPointType()
{
	if (!HaveMemoryBreakPoint())return -1;
	return m_MemoryBreakPoint.nType;
}
//*********************************************************************************



//*********************************************************************************
// ȡ����һ����ַ����ǰ��ַ
//*********************************************************************************
DWORD DebugDlg::GetBeforeAddress(DWORD nReadAddress)
{

	BYTE nReadBuff[128]{};
	CString nAddr;
	DISASM disAsm = { 0 };

	ReadMemoryBytes((nReadAddress - 30), nReadBuff, 128);

	disAsm.EIP = (UIntPtr)nReadBuff;
	disAsm.VirtualAddr = nReadAddress - 30;
	disAsm.Archi = 0;
	disAsm.Options = 0x000;
	DWORD nOpcodeSize = 128;

	INT nLen = 0;

	for (DWORD i = 0; i < 128; i++)
	{
		nLen = Disasm(&disAsm);
		if (nLen == -1)
		{
			disAsm.EIP += 1;
			disAsm.VirtualAddr += 1;
			continue;
		}
		disAsm.EIP += nLen;
		disAsm.VirtualAddr += nLen;

		if (disAsm.VirtualAddr == nReadAddress)
		{
			return nReadAddress -= nLen;
		}
	}

	return nReadAddress-1;
}
//*********************************************************************************
// ȡ����һ����ַ����ǰ��ַ
//*********************************************************************************
DWORD DebugDlg::GetNextAddress(DWORD nReadAddress)
{
	BYTE nReadBuff[128]{};
	CString nAddr;
	DISASM disAsm = { 0 };

	ReadMemoryBytes(nReadAddress, nReadBuff, 128);

	disAsm.EIP = (UIntPtr)nReadBuff;
	disAsm.VirtualAddr = nReadAddress;
	disAsm.Archi = 0;
	disAsm.Options = 0x000;
	DWORD nOpcodeSize = 128;

	INT nLen = 0;

	for (DWORD i = 0; i < 128; i++)
	{
		nLen = Disasm(&disAsm);
		if (nLen == -1)
		{
			disAsm.EIP += 1;
			disAsm.VirtualAddr += 1;
			continue;
		}

		disAsm.EIP += nLen;
		disAsm.VirtualAddr += nLen;

		return (DWORD)disAsm.VirtualAddr;
	}
	return nReadAddress + 1;
}
//*********************************************************************************
// ȡ��������OEP��ַ
//*********************************************************************************
DWORD DebugDlg::GetOepAddress()
{
	HMODULE nHmodule[255]{};
	DWORD nNeedSize;
	BYTE nProcessBuff[1024]{};

	EnumProcessModules(m_ProcessHandle, nHmodule, 255, &nNeedSize);

	ReadMemoryBytes((DWORD)nHmodule[0], nProcessBuff, 1024);

	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)nProcessBuff;
	PIMAGE_NT_HEADERS pNtHead = (PIMAGE_NT_HEADERS)(pDosHead->e_lfanew + nProcessBuff);

	return (DWORD)nHmodule[0] + pNtHead->OptionalHeader.AddressOfEntryPoint;
}
//*********************************************************************************
// ȡ��ָ��ģ���OEP��ַ��ģ����ػ�ַ
//*********************************************************************************
DWORD DebugDlg::GetOepAddressEx(DWORD nImageBase)
{
	BYTE nProcessBuff[1024]{};

	ReadMemoryBytes(nImageBase, nProcessBuff, 1024);

	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)nProcessBuff;
	PIMAGE_NT_HEADERS pNtHead = (PIMAGE_NT_HEADERS)(pDosHead->e_lfanew + nProcessBuff);

	return nImageBase + pNtHead->OptionalHeader.AddressOfEntryPoint;
}
//*********************************************************************************
// ȡ�����ݶε�ַ
//*********************************************************************************
DWORD DebugDlg::GetDataAddress()
{
	HMODULE nHmodule[255]{};
	DWORD nNeedSize;
	BYTE nProcessBuff[1024]{};

	EnumProcessModules(m_ProcessHandle, nHmodule, 255, &nNeedSize);

	ReadMemoryBytes((DWORD)nHmodule[0], nProcessBuff, 1024);

	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)nProcessBuff;
	PIMAGE_NT_HEADERS pNtHead = (PIMAGE_NT_HEADERS)(pDosHead->e_lfanew + nProcessBuff);

	return (DWORD)nHmodule[0] + pNtHead->OptionalHeader.BaseOfData;
}
//*********************************************************************************
// ȡ�������̵ļ��ص�ַ
//*********************************************************************************
DWORD DebugDlg::GetImageBassAddress()
{
	HMODULE nHmodule[255]{};
	DWORD nNeedSize;
	BYTE nProcessBuff[1024]{};

	EnumProcessModules(m_ProcessHandle, nHmodule, 255, &nNeedSize);

	return (DWORD)nHmodule[0];
}
//*********************************************************************************
// ȡ��ָ��Api��ַ��Api����
//*********************************************************************************
DWORD DebugDlg::GetApiAddress(CString nApi)
{
	HMODULE nHandle = nullptr;
	DWORD nAddress = 0;

	nHandle = GetModuleHandle(TEXT("kernel32.dll"));
	nAddress = (DWORD)GetProcAddress(nHandle, CStringA(nApi));
	if (nAddress)return nAddress;
	FreeLibrary(nHandle);

	nHandle = GetModuleHandle(TEXT("user32.dll"));
	nAddress = (DWORD)GetProcAddress(nHandle, CStringA(nApi));
	if (nAddress)return nAddress;
	FreeLibrary(nHandle);

	nHandle = GetModuleHandle(TEXT("gdi32.dll"));
	nAddress = (DWORD)GetProcAddress(nHandle, CStringA(nApi));
	if (nAddress)return nAddress;
	FreeLibrary(nHandle);

	nHandle = GetModuleHandle(TEXT("ntdll.dll"));
	nAddress = (DWORD)GetProcAddress(nHandle, CStringA(nApi));
	if (nAddress)return nAddress;
	FreeLibrary(nHandle);

	return 0;
}



//*********************************************************************************
// ��������ϵ����
VOID DebugDlg::SetBreakPoint(DWORD nBreakAddress,BOOL nIsCondotion,CONDITION nCondition)
{
	BYTE nReadByte;
	BYTE nWriteByte = 0xcc;

	ReadMemoryByte(nBreakAddress, &nReadByte);
	WriteMemoryByte(nBreakAddress, nWriteByte);

	if (nIsCondotion)	m_BreakPoint.push_back({ nBreakAddress ,nReadByte,TRUE,nCondition });
	else 	m_BreakPoint.push_back({ nBreakAddress ,nReadByte,FALSE,NULL });

}
VOID DebugDlg::SetBreakPointEx(DWORD nBreakAddress)
{
	BYTE nWriteByte = 0xcc;

	WriteMemoryByte(nBreakAddress, nWriteByte);
}
VOID DebugDlg::SetTempBreakPoint(DWORD nBreakAddress)
{
	BYTE nReadByte;
	BYTE nWriteByte = 0xcc;

	ReadMemoryByte(nBreakAddress, &nReadByte);
	WriteMemoryByte(nBreakAddress, nWriteByte);

	m_TempBreakPoint.push_back({ nBreakAddress ,nReadByte });
}
VOID DebugDlg::DelBreakPoint(DWORD nBreakAddress)
{
	BYTE nWriteByte;
	DWORD nIndex;

	nIndex = IsBreakPoint(nBreakAddress);
	if (nIndex == -1)return;

	nWriteByte = m_BreakPoint[nIndex].nData;
	WriteMemoryByte(nBreakAddress, nWriteByte);
	m_BreakPoint.erase(m_BreakPoint.begin() + nIndex);
}
VOID DebugDlg::DelTempBreakPoint(DWORD nBreakAddress)
{
	BYTE nWriteByte;
	DWORD nIndex;

	nIndex = IsTempBreakPoint(nBreakAddress);
	if (nIndex == -1)return;

	nWriteByte = m_TempBreakPoint[nIndex].nData;
	WriteMemoryByte(nBreakAddress, nWriteByte);
	m_TempBreakPoint.erase(m_TempBreakPoint.begin() + nIndex);
}
VOID DebugDlg::OpenAllBreakPoint()
{
	BYTE nWriteByte = 0xcc;

	for (DWORD i = 0; i < m_BreakPoint.size(); i++)
	{
		WriteMemoryByte(m_BreakPoint[i].nAddress, nWriteByte);
	}
}
VOID DebugDlg::CloseAllBreakPoint()
{
	BYTE nWriteByte;

	for (DWORD i = 0; i < m_BreakPoint.size(); i++)
	{
		nWriteByte = m_BreakPoint[i].nData;
		WriteMemoryByte(m_BreakPoint[i].nAddress, nWriteByte);
	}
}
DWORD DebugDlg::IsBreakPoint(DWORD nBreakAddress)
{
	for (DWORD i = 0; i < m_BreakPoint.size(); i++)
	{
		if (m_BreakPoint[i].nAddress == nBreakAddress)
		{
			return i;
		}
	}
	return -1;
}
DWORD DebugDlg::IsTempBreakPoint(DWORD nBreakAddress)
{
	for (DWORD i = 0; i < m_TempBreakPoint.size(); i++)
	{
		if (m_TempBreakPoint[i].nAddress == nBreakAddress)
		{
			return i;
		}
	}
	return -1;
}
DWORD DebugDlg::IsConditionBreakPoint(DWORD nBreakPointIndex)
{
	if (nBreakPointIndex == -1)return FALSE;
	return m_BreakPoint[nBreakPointIndex].nIsCondition;
}
DWORD DebugDlg::IsConditionSucessBreakPoint(DWORD nBreakPointIndex,CONTEXT nContext)
{
	if (nBreakPointIndex == -1)return FALSE;

	if (m_BreakPoint[nBreakPointIndex].nCondition.OPEN_EAX)
	{
		if (m_BreakPoint[nBreakPointIndex].nCondition.EAX != nContext.Eax)return FALSE;
	}
	if (m_BreakPoint[nBreakPointIndex].nCondition.OPEN_EBX)
	{
		if (m_BreakPoint[nBreakPointIndex].nCondition.EBX != nContext.Ebx)return FALSE;
	}
	if (m_BreakPoint[nBreakPointIndex].nCondition.OPEN_ECX)
	{
		if (m_BreakPoint[nBreakPointIndex].nCondition.ECX != nContext.Ecx)return FALSE;
	}
	if (m_BreakPoint[nBreakPointIndex].nCondition.OPEN_EDX)
	{
		if (m_BreakPoint[nBreakPointIndex].nCondition.EDX != nContext.Edx)return FALSE;
	}
	if (m_BreakPoint[nBreakPointIndex].nCondition.OPEN_ESI)
	{
		if (m_BreakPoint[nBreakPointIndex].nCondition.ESI != nContext.Esi)return FALSE;
	}
	if (m_BreakPoint[nBreakPointIndex].nCondition.OPEN_EDI)
	{
		if (m_BreakPoint[nBreakPointIndex].nCondition.EDI != nContext.Edi)return FALSE;
	}
	if (m_BreakPoint[nBreakPointIndex].nCondition.OPEN_EBP)
	{
		if (m_BreakPoint[nBreakPointIndex].nCondition.EBP != nContext.Ebp)return FALSE;
	}
	if (m_BreakPoint[nBreakPointIndex].nCondition.OPEN_ESP)
	{
		if (m_BreakPoint[nBreakPointIndex].nCondition.ESP != nContext.Esp)return FALSE;
	}
	return TRUE;
}
//*********************************************************************************



//*********************************************************************************
// �Ƿ�ΪCallRep���룺�ڴ��ַ
//*********************************************************************************
BOOL DebugDlg::IsCallRepAsm(DWORD nAddress)
{
	BYTE nReadBuff[64]{};
	CString nAddr;
	DISASM disAsm = { 0 };

	ReadMemoryBytes(nAddress, nReadBuff, 64);

	disAsm.EIP = (UIntPtr)nReadBuff;
	disAsm.VirtualAddr = nAddress;
	disAsm.Archi = 0;
	disAsm.Options = 0x000;
	DWORD nOpcodeSize = 64;

	INT nLen = 0;

	for (DWORD i = 0; i < 64; i++)
	{
		nLen = Disasm(&disAsm);
		if (nLen == -1)
		{
			disAsm.EIP += 1;
			disAsm.VirtualAddr += 1;
			continue;
		}
		break;
	}

	CString nTempAsm(disAsm.CompleteInstr);
	if (nTempAsm.Left(4) == TEXT("call"))return TRUE;
	else if (nTempAsm.Left(3) == TEXT("rep"))return TRUE;
	return FALSE;
}
//*********************************************************************************
// ���ҵ�ַ�����б���λ�ã���ַ
//*********************************************************************************
DWORD DebugDlg::FindAddress(DWORD nShowAddress)
{
	DWORD nShow;
	CString nFind;
	for (INT i = 0; i < m_List_Asm.GetItemCount(); i++)
	{
		nFind = m_List_Asm.GetItemText(i, m_Flag_Address);
		_stscanf_s(nFind, TEXT("%x"), &nShow);
		if (nShowAddress == nShow)return i;
	}
	return -1;
}





//*********************************************************************************
// ��д�ڴ��װ
VOID DebugDlg::ReadMemoryDword(DWORD nAddress, LPDWORD nValue)
{
	DWORD nReadSize;
	ReadProcessMemory(m_ProcessHandle, (LPCVOID)nAddress, nValue, 4, &nReadSize);
}
VOID DebugDlg::WriteMemoryDword(DWORD nAddress, DWORD nValue)
{
	DWORD nOldProtect;
	DWORD nWriteSize;
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nAddress, 1, PAGE_EXECUTE_READWRITE, &nOldProtect);
	WriteProcessMemory(m_ProcessHandle, (LPVOID)nAddress, &nValue, 4, &nWriteSize);
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nAddress, 1, nOldProtect, &nOldProtect);
}
VOID DebugDlg::ReadMemoryByte(DWORD nAddress, LPBYTE nValue)
{
	DWORD nReadSize;
	ReadProcessMemory(m_ProcessHandle, (LPCVOID)nAddress, nValue, 1, &nReadSize);
}
VOID DebugDlg::WriteMemoryByte(DWORD nAddress, BYTE nValue)
{
	DWORD nOldProtect;
	DWORD nWriteSize;
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nAddress, 1, PAGE_EXECUTE_READWRITE, &nOldProtect);
	WriteProcessMemory(m_ProcessHandle, (LPVOID)nAddress, &nValue, 1, &nWriteSize);
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nAddress, 1, nOldProtect, &nOldProtect);
}
VOID DebugDlg::ReadMemoryBytes(DWORD nAddress, LPBYTE nValue, DWORD nLen)
{
	DWORD nReadSize;
	ReadProcessMemory(m_ProcessHandle, (LPCVOID)nAddress, nValue, nLen, &nReadSize);
}
VOID DebugDlg::WriteMemoryBytes(DWORD nAddress, LPBYTE nValue, DWORD nLen)
{
	DWORD nOldProtect;
	DWORD nWriteSize;
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nAddress, nLen, PAGE_EXECUTE_READWRITE, &nOldProtect);
	WriteProcessMemory(m_ProcessHandle, (LPVOID)nAddress, nValue, nLen, &nWriteSize);
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nAddress, nLen, nOldProtect, &nOldProtect);
}
VOID DebugDlg::ReadMemoryWord(DWORD nAddress, LPWORD nValue)
{
	DWORD nReadSize;
	ReadProcessMemory(m_ProcessHandle, (LPCVOID)nAddress, nValue, 2, &nReadSize);
}
VOID DebugDlg::WriteMemoryWord(DWORD nAddress, WORD nValue)
{
	DWORD nOldProtect;
	DWORD nWriteSize;
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nAddress, 1, PAGE_EXECUTE_READWRITE, &nOldProtect);
	WriteProcessMemory(m_ProcessHandle, (LPVOID)nAddress, &nValue, 2, &nWriteSize);
	VirtualProtectEx(m_ProcessHandle, (LPVOID)nAddress, 1, nOldProtect, &nOldProtect);
}
//*********************************************************************************




//************************************
// ���򵥻����
//************************************
void DebugDlg::OnClickListAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	if (pNMItemActivate->iItem == -1)return;

	CString nAddress;
	CString nOpcode;
	CString nAsm;

	nAddress = m_List_Asm.GetItemText(pNMItemActivate->iItem, m_Flag_Address);
	nOpcode = m_List_Asm.GetItemText(pNMItemActivate->iItem, m_Flag_Opcode);
	nAsm = m_List_Asm.GetItemText(pNMItemActivate->iItem, m_Flag_Asm);

	m_Edit_Print.SetWindowText(nAddress + TEXT("              -              ") + nOpcode + TEXT("              -              ") + nAsm);
}
//************************************
// ����˫�����
//************************************
void DebugDlg::OnDblclkListAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	if (pNMItemActivate->iItem == -1 ||
			(pNMItemActivate->iSubItem != m_Flag_Asm && pNMItemActivate->iSubItem != m_Flag_Asm_Show))return;

	OnChangeasm();


}
//************************************
// ���򵥻��Ҽ�
//************************************
void DebugDlg::OnRclickListAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	DWORD nItem = pNMItemActivate->iItem;
	DWORD nSubItem = pNMItemActivate->iSubItem;

	if (nItem == -1)return;

	CMenu *nMenu = m_Menu.GetSubMenu(0);
	POINT pos;
	GetCursorPos(&pos);
	nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);

}
//************************************
// ��ջ��˫�����
//************************************
void DebugDlg::OnDblclkListHeap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	DWORD nItem = pNMItemActivate->iItem;
	DWORD nSubItem = pNMItemActivate->iSubItem;

	if (nItem == -1)return;

	CString nTemp;
	DWORD nReadAddress;
	DWORD nReadOffset;


	if (nSubItem == m_Flag_HeapAddress)
	{
		_GotoMemory:
		nTemp = m_List_Heap.GetItemText(nItem, m_Flag_HeapAddress);
		_stscanf_s(nTemp, TEXT("%X+%X"), &nReadAddress,&nReadOffset);
		PrintMemory(nReadAddress + nReadOffset);
		m_List_Memory.SetFocus();
		m_List_Memory.SetSelectionEx(0);
	}
	else if (nSubItem == m_Flag_HeapData)
	{
		nTemp = m_List_Heap.GetItemText(nItem, m_Flag_HeapData);
		if (nTemp.IsEmpty())return;

		_stscanf_s(nTemp, TEXT("%x"), &nReadAddress);
		if (nReadAddress < 65536)goto _GotoMemory;

		InitAsm(nReadAddress);
		m_List_Asm.SetFocus();
		m_List_Asm.SetSelectionEx(0);
	}

}
//************************************
// �ڴ��˫�����
//************************************
void DebugDlg::OnDblclkListMemory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	DWORD nItem = pNMItemActivate->iItem;
	DWORD nSubItem = pNMItemActivate->iSubItem;

	if (nItem == -1 || nSubItem == m_Flag_MemoryAddress || nSubItem == m_Flag_MemoryStr)return;

	EditMemoryProc(nItem, nSubItem);
}
//************************************
// �ڴ�򵥻��Ҽ�
//************************************
void DebugDlg::OnRclickListMemory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
	DWORD nItem = pNMItemActivate->iItem;
	DWORD nSubItem = pNMItemActivate->iSubItem;

	if (nItem == -1)return;

	CMenu *nMenu = m_Menu.GetSubMenu(1);
	POINT pos;
	GetCursorPos(&pos);


	if (nSubItem == m_Flag_MemoryAddress || nSubItem == m_Flag_MemoryStr)
	{
		nMenu->EnableMenuItem(ID_EditMemory, TRUE);
		nMenu->EnableMenuItem(ID_HardWrite1, TRUE);
		nMenu->EnableMenuItem(ID_HardWrite2, TRUE);
		nMenu->EnableMenuItem(ID_HardWrite4, TRUE);
		nMenu->EnableMenuItem(ID_HardRead1, TRUE);
		nMenu->EnableMenuItem(ID_HardRead2, TRUE);
		nMenu->EnableMenuItem(ID_HardRead4, TRUE);

		
	}
	else
	{
		nMenu->EnableMenuItem(ID_EditMemory, FALSE);
		nMenu->EnableMenuItem(ID_HardWrite1, FALSE);
		nMenu->EnableMenuItem(ID_HardWrite2, FALSE);
		nMenu->EnableMenuItem(ID_HardWrite4, FALSE);
		nMenu->EnableMenuItem(ID_HardRead1, FALSE);
		nMenu->EnableMenuItem(ID_HardRead2, FALSE);
		nMenu->EnableMenuItem(ID_HardRead4, FALSE);
	}

	nTempItem = nItem;
	nTempSubItem = nSubItem;
	nMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, this);

}
//************************************
// ����������
//************************************
void DebugDlg::OnCustomdrawListAsm(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	LPNMCUSTOMDRAW nmCustomDraw = &pNMCD->nmcd;

	switch (nmCustomDraw->dwDrawStage)
	{
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
	{

		if (m_List_Asm.IsSelectionItem(nmCustomDraw->dwItemSpec))
		{
			pNMCD->clrText = COLOR_WHITE;
			pNMCD->clrTextBk = COLOR_BLUE;
			nmCustomDraw->uItemState = 0;
			*pResult = CDRF_NEWFONT;
			return;
		}


		for (auto &value : m_AsmColor)
		{
			if (value.Item == nmCustomDraw->dwItemSpec && value.SubItem == pNMCD->iSubItem)
			{
				pNMCD->clrText = value.TextColor;
				pNMCD->clrTextBk = value.BkColor;
				break;
			}
			else
			{
				pNMCD->clrText = COLOR_BLACK;
				pNMCD->clrTextBk = COLOR_WHITE;
			}
		}
		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;		//�����У���Ȼ��û��Ч��
	*pResult |= CDRF_NOTIFYITEMDRAW;		//�����У���Ȼ��û��Ч��}
}
//************************************
// �����ջ�����
//************************************
void DebugDlg::OnCustomdrawListHeap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	LPNMCUSTOMDRAW nmCustomDraw = &pNMCD->nmcd;

	switch (nmCustomDraw->dwDrawStage)
	{
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
	{

		if (m_List_Heap.IsSelectionItem(nmCustomDraw->dwItemSpec))
		{
			pNMCD->clrText = COLOR_WHITE;
			pNMCD->clrTextBk = COLOR_BLUE;
			nmCustomDraw->uItemState = 0;
			*pResult = CDRF_NEWFONT;
			return;
		}
		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;		//�����У���Ȼ��û��Ч��
	*pResult |= CDRF_NOTIFYITEMDRAW;		//�����У���Ȼ��û��Ч��}}
}
//************************************
// �����ڴ�����
//************************************
void DebugDlg::OnCustomdrawListMemory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	LPNMCUSTOMDRAW nmCustomDraw = &pNMCD->nmcd;

	switch (nmCustomDraw->dwDrawStage)
	{
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
	{
		
		if (m_List_Memory.IsSelectionItem(nmCustomDraw->dwItemSpec))
		{
			if (pNMCD->iSubItem == nTempSubItem)
			{
				pNMCD->clrText = COLOR_WHITE;
				pNMCD->clrTextBk = COLOR_BLUE;
				nmCustomDraw->uItemState = 0;
				*pResult = CDRF_NEWFONT;
				return;
			}

			pNMCD->clrText = COLOR_BLACK;
			pNMCD->clrTextBk = COLOR_WHITE;
			nmCustomDraw->uItemState = 0;
			*pResult = CDRF_NEWFONT;
			return;
		}

		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;		//�����У���Ȼ��û��Ч��
	*pResult |= CDRF_NOTIFYITEMDRAW;		//�����У���Ȼ��û��Ч��}

}



//*********************************************************************************
// ���ø�����ɫ���У��У�������ɫ��������ɫ
//*********************************************************************************
VOID DebugDlg::SetItemColor(DWORD Item, DWORD SubItem, COLORREF TextColor, COLORREF BkColor)
{
	BOOL nHava = FALSE;
	for (auto &value : m_AsmColor)
	{
		if (value.Item == Item && value.SubItem== SubItem)
		{
			value.TextColor = TextColor;
			value.BkColor = BkColor;
			nHava = TRUE;
			break;
		}
	}

	if (!nHava)m_AsmColor.push_back({ Item ,SubItem ,TextColor ,BkColor });
	
}
//*********************************************************************************
// ������и�����ɫ
//*********************************************************************************
VOID DebugDlg::ClearItemColor()
{
	m_AsmColor.clear();
	m_AsmColor.swap(vector<LISTCOLOR>());
}
//*********************************************************************************
// �Ƿ�Ϊ������ָ�ָ�����������ɫ�����ر�����ɫ
//*********************************************************************************
BOOL DebugDlg::IsColorWord(CString nStr, LPDWORD nTextColor, LPDWORD nBkColor)
{
	if (nStr.Left(4) == "call")
	{
		*nTextColor = COLOR_RED;
		*nBkColor = COLOR_WHITE;
		return TRUE;
	}
	else if (nStr.Left(1) == "j")
	{
		*nTextColor = COLOR_BLUE;
		*nBkColor = COLOR_WHITE;
		return TRUE;
	}
	else if (nStr.Left(3) == "ret")
	{
		*nTextColor = COLOR_GREEN;
		*nBkColor = COLOR_WHITE;
		return TRUE;
	}
	return FALSE;
}
//*********************************************************************************
// �Ƿ�Ϊ��תָ�ָ���������Ŀ�ĵص�ַ
//*********************************************************************************
BOOL DebugDlg::IsJumpWord(CString nStr, LPDWORD nAddress)
{
	if (nStr.Left(4) == CString("call") || nStr.Left(1) == CString("j"))
	{
		nStr.Replace(TEXT("h"), TEXT(""));

		if (nStr.Find(TEXT("["))==-1)
		{
			nStr = nStr.Right(8);
			_stscanf_s(nStr, TEXT("%x"), nAddress);
		}
		else
		{
			nStr.Replace(TEXT("["), TEXT(""));
			nStr.Replace(TEXT("]"), TEXT(""));
			nStr = nStr.Right(8);
			DWORD nTempAddress;
			_stscanf_s(nStr, TEXT("%x"), &nTempAddress);
			ReadMemoryDword(nTempAddress, nAddress);
		}
		return TRUE;
	}

	return FALSE;
}
//*********************************************************************************
// Asmת��λopcode��Asm���ڵ�ַ��Asmָ�����opcode������opcode����
//*********************************************************************************
BOOL DebugDlg::AsmToOpcaode(DWORD nAddress, CHAR *nAsm, LPBYTE nBytes, DWORD &nByteNum)
{
	XEDPARSE nXedparse = { 0 };

	nXedparse.cip = (ULONGLONG)nAddress;
	strcpy_s(nXedparse.instr, XEDPARSE_MAXBUFSIZE, nAsm);

	if (XEDParseAssemble(&nXedparse) != XEDPARSE_OK)return FALSE;

	nByteNum = nXedparse.dest_size;

	memcpy_s(nBytes, nXedparse.dest_size, nXedparse.dest, nXedparse.dest_size);

	return TRUE;
}



//*********************************************************************************
// �޸��ڴ����ݹ��̣��У���
//*********************************************************************************
VOID DebugDlg::EditMemoryProc(DWORD nItem, DWORD nSubItem)
{

	CString szAddress;
	DWORD dwAddress;
	DWORD dwByte;
	szAddress = m_List_Memory.GetItemText(nItem, m_Flag_MemoryAddress);
	_stscanf_s(szAddress, TEXT("%X"), &g_SendAddress_Memory);
	g_SendAddress_Memory = g_SendAddress_Memory + nSubItem - 1;
	g_SendBytes_Memory = m_List_Memory.GetItemText(nItem, nSubItem);
	g_SendBytes_Memory.Replace(TEXT(" "), TEXT(""));

	g_Ok_Memory = FALSE;
	EditMemoryDlg nEditMemoryDlg;
	nEditMemoryDlg.DoModal();
	if (!g_Ok_Memory)return;

	_stscanf_s(g_SendBytes_Memory, TEXT("%02X"), &dwByte);
	if (dwByte == g_RecvBytes_Memory)return;

	WriteMemoryByte(g_SendAddress_Memory, (BYTE)g_RecvBytes_Memory);

	szAddress = m_List_Memory.GetItemText(0, m_Flag_MemoryAddress);
	_stscanf_s(szAddress, TEXT("%X"), &dwAddress);
	PrintMemory(dwAddress);

}
//*********************************************************************************
// ����ָ��Asm��ַ
//*********************************************************************************
void DebugDlg::OnGotoaddress()
{

}
//*********************************************************************************
// ����ָ��Asm��ַ
//*********************************************************************************
void DebugDlg::OnInaddress()
{
	CString nTempAddress;
	CString nRecordAddress;
	DWORD nReadAddress;
	nRecordAddress = m_List_Asm.GetItemText(m_List_Asm.GetSelectionItem(), m_Flag_Address);
	nTempAddress = m_List_Asm.GetItemText(m_List_Asm.GetSelectionItem(), m_Flag_Asm);

	if (IsJumpWord(nTempAddress, &nReadAddress))
	{
		AddSelectRecord();
		InitAsm(nReadAddress);
		m_List_Asm.SetSelectionEx(0);
	}

}
//*********************************************************************************
// �޸�Asm����
//*********************************************************************************
void DebugDlg::OnChangeasm()
{
	DWORD nItem = m_List_Asm.GetSelectionItem();
	CString nAddress;
	CString nOpcode;
	nOpcode = m_List_Asm.GetItemText(nItem, m_Flag_Opcode);
	nAddress = m_List_Asm.GetItemText(nItem, m_Flag_Address);
	_stscanf_s(nAddress, TEXT("%X"), &g_SendAddress_Asm);

	g_SendAsm_Asm = m_List_Asm.GetItemText(nItem, m_Flag_Asm);
	g_SendBytesNum_Asm = nOpcode.Replace(TEXT(" "), TEXT(" "));

	g_Ok_Asm = FALSE;
	EditAsmDlg nEditAsmDlg;
	nEditAsmDlg.DoModal();
	if (!g_Ok_Asm)return;


	if (g_RecvBytesNum_Asm)
	{
		CString nTemp;
		CString nTempBytes;

		for (DWORD i = 0; i < g_RecvBytesNum_Asm; i++)
		{
			nTemp.Format(TEXT("%02X "), g_RecvBytes_Asm[i]);
			nTempBytes = nTempBytes + nTemp;
		}

		m_List_Asm.SetItemText(nItem, m_Flag_Opcode, nTempBytes);
		m_List_Asm.SetItemText(nItem, m_Flag_Asm, g_SendAsm_Asm);

		WriteMemoryBytes(g_SendAddress_Asm, g_RecvBytes_Asm, g_RecvBytesNum_Asm);

		InitAsm(nReadAddressFirst);
		OutputDebugEdit(m_Edit_Output, TEXT("%08X - %s "), g_SendAddress_Asm, nTempBytes);

	}
}
//*********************************************************************************
// ��������ϵ�
//*********************************************************************************
void DebugDlg::OnSoftbreak()
{
	DWORD nBreakAddress;
	CString nTempAddress;
	DWORD nIndex;
	DWORD nSelect;

	nSelect = m_List_Asm.GetSelectionItem();
	nTempAddress = m_List_Asm.GetItemText(nSelect, m_Flag_Address);
	_stscanf_s(nTempAddress, TEXT("%x"), &nBreakAddress);

	nIndex = IsBreakPoint(nBreakAddress);
	if (nIndex == -1)
	{
		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT("��"));
		SetBreakPoint(nBreakAddress, FALSE, {});
		SetItemColor(nSelect, m_Flag_Break,COLOR_WHITE, COLOR_RED);
		SetItemColor(nSelect, m_Flag_Address, COLOR_WHITE, COLOR_RED);
		SetItemColor(nSelect, m_Flag_Address_Show, COLOR_WHITE, COLOR_RED);
	}
	else
	{
		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT(""));
		DelBreakPoint(nBreakAddress);
		SetItemColor(nSelect, m_Flag_Break, COLOR_BLACK, COLOR_WHITE);
		SetItemColor(nSelect, m_Flag_Address, COLOR_BLACK, COLOR_WHITE);
		SetItemColor(nSelect, m_Flag_Address_Show, COLOR_BLACK, COLOR_WHITE);
	}

	WriteAllBreakInfo();
	OutputDebugEdit(m_Edit_Output, TEXT("%s����ϵ㣺%08X"), nIndex == -1 ? TEXT("����") : TEXT("ɾ��"), nBreakAddress);

}
//*********************************************************************************
// ������������ϵ�
//*********************************************************************************
void DebugDlg::OnConditionsoftbreak()
{
	DWORD nBreakAddress;
	CString nTempAddress;
	DWORD nIndex;
	DWORD nSelect;

	nSelect = m_List_Asm.GetSelectionItem();
	nTempAddress = m_List_Asm.GetItemText(nSelect, m_Flag_Address);
	_stscanf_s(nTempAddress, TEXT("%x"), &nBreakAddress);

	nIndex = IsBreakPoint(nBreakAddress);
	if (nIndex == -1)
	{
		ZeroMemory(&g_RecvCondition, sizeof(CONDITION));
		ConditionBreakDlg nConditionBreakDlg;
		nConditionBreakDlg.DoModal();

		if (!g_OK_ConditionBreakPoint)return;

		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT("��"));
		SetBreakPoint(nBreakAddress, TRUE, g_RecvCondition);
		SetItemColor(nSelect, m_Flag_Break, COLOR_WHITE, COLOR_RED);
		SetItemColor(nSelect, m_Flag_Address, COLOR_WHITE, COLOR_RED);
	}
	else
	{
		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT(""));
		DelBreakPoint(nBreakAddress);
		SetItemColor(nSelect, m_Flag_Break, COLOR_BLACK, COLOR_WHITE);
		SetItemColor(nSelect, m_Flag_Address, COLOR_BLACK, COLOR_WHITE);
	}

	WriteAllBreakInfo();
	OutputDebugEdit(m_Edit_Output, TEXT("%s��������ϵ㣺%08X"), nIndex == -1 ? TEXT("����") : TEXT("ɾ��"), nBreakAddress);
}
//*********************************************************************************
// ����Ӳ���ϵ�
//*********************************************************************************
void DebugDlg::OnHardbreak()
{
	DWORD nBreakAddress;
	CString nTempAddress;
	DWORD nIndex;
	DWORD nSelect;
	BOOL nRet;

	nSelect = m_List_Asm.GetSelectionItem();
	nTempAddress = m_List_Asm.GetItemText(nSelect, m_Flag_Address);
	_stscanf_s(nTempAddress, TEXT("%x"), &nBreakAddress);

	nIndex = IsHardBreakPoint(nBreakAddress);
	if (nIndex == -1)
	{
		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT("��"));
		nRet = SetHardBreakPoint(nBreakAddress, FALSE, {});
		if (!nRet)
		{
			MessageBox(TEXT("Ӳ���ϵ�ʧ�ܣ�"), TEXT("ʧ�ܣ�"), MB_ICONERROR);
			return;
		}
		SetItemColor(nSelect, m_Flag_Break, COLOR_WHITE, COLOR_BLACK);
		SetItemColor(nSelect, m_Flag_Address, COLOR_WHITE, COLOR_BLACK);
		SetItemColor(nSelect, m_Flag_Address_Show, COLOR_WHITE, COLOR_BLACK);
	}
	else
	{
		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT(""));
		DelHardBreakPoint(nBreakAddress);
		SetItemColor(nSelect, m_Flag_Break, COLOR_BLACK, COLOR_WHITE);
		SetItemColor(nSelect, m_Flag_Address, COLOR_BLACK, COLOR_WHITE);
		SetItemColor(nSelect, m_Flag_Address_Show, COLOR_BLACK, COLOR_WHITE);
	}

	WriteAllBreakInfo();
	OutputDebugEdit(m_Edit_Output, TEXT("%sӲ���ϵ㣺%08X"), nIndex == -1 ? TEXT("����") : TEXT("ɾ��"), nBreakAddress);
}
//*********************************************************************************
// ����Ӳ�������ϵ�
//*********************************************************************************
void DebugDlg::OnConditionhardbreak()
{
	DWORD nBreakAddress;
	CString nTempAddress;
	DWORD nIndex;
	DWORD nSelect;
	BOOL nRet;

	nSelect = m_List_Asm.GetSelectionItem();
	nTempAddress = m_List_Asm.GetItemText(nSelect, m_Flag_Address);
	_stscanf_s(nTempAddress, TEXT("%x"), &nBreakAddress);

	nIndex = IsHardBreakPoint(nBreakAddress);
	if (nIndex == -1)
	{

		ZeroMemory(&g_RecvCondition, sizeof(CONDITION));
		ConditionBreakDlg nConditionBreakDlg;
		nConditionBreakDlg.DoModal();

		if (!g_OK_ConditionBreakPoint)return;

		nRet = SetHardBreakPoint(nBreakAddress, TRUE, g_RecvCondition);
		if (!nRet)
		{
			MessageBox(TEXT("Ӳ���ϵ�ʧ�ܣ�"), TEXT("ʧ�ܣ�"), MB_ICONERROR);
			return;
		}
		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT("��"));
		SetItemColor(nSelect, m_Flag_Break, COLOR_WHITE, COLOR_BLACK);
		SetItemColor(nSelect, m_Flag_Address, COLOR_WHITE, COLOR_BLACK);
	}
	else
	{
		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT(""));
		DelHardBreakPoint(nBreakAddress);
		SetItemColor(nSelect, m_Flag_Break, COLOR_BLACK, COLOR_WHITE);
		SetItemColor(nSelect, m_Flag_Address, COLOR_BLACK, COLOR_WHITE);
	}

	WriteAllBreakInfo();
	OutputDebugEdit(m_Edit_Output, TEXT("%sӲ���ϵ㣺%08X"), nIndex == -1 ? TEXT("����") : TEXT("ɾ��"), nBreakAddress);
}
//*********************************************************************************
// �����ڴ�ִ�жϵ�
//*********************************************************************************
void DebugDlg::OnMemoryBreak()
{
	DWORD nBreakAddress;
	CString nTempAddress;
	BOOL nIndex;
	DWORD nSelect;
	nSelect = m_List_Asm.GetSelectionItem();
	nTempAddress = m_List_Asm.GetItemText(nSelect, m_Flag_Address);
	_stscanf_s(nTempAddress, TEXT("%x"), &nBreakAddress);
	


	nIndex = IsMemoryBreakPoint(nBreakAddress);


	if (HaveMemoryBreakPoint() && nIndex == FALSE)
	{
		MessageBox(TEXT("�Ѵ����ڴ�ϵ㣡�ϵ�ʧ�ܣ�"), TEXT("ʧ�ܣ�"), MB_ICONERROR);
		return;
	}

	if (!HaveMemoryBreakPoint())
	{
		SetMemoryBreakPoint(nBreakAddress, 8, FALSE, {});

		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT("��"));
		SetItemColor(nSelect, m_Flag_Break, COLOR_WHITE, COLOR_ORANGE);
		SetItemColor(nSelect, m_Flag_Address, COLOR_WHITE, COLOR_ORANGE);
		SetItemColor(nSelect, m_Flag_Address_Show, COLOR_WHITE, COLOR_ORANGE);

	}
	else
	{
		DelMemoryBreakPoint(nBreakAddress);

		m_List_Asm.SetItemText(nSelect, m_Flag_Break, TEXT(""));
		SetItemColor(nSelect, m_Flag_Break, COLOR_BLACK, COLOR_WHITE);
		SetItemColor(nSelect, m_Flag_Address, COLOR_BLACK, COLOR_WHITE);
		SetItemColor(nSelect, m_Flag_Address_Show, COLOR_BLACK, COLOR_WHITE);

	}

	OutputDebugEdit(m_Edit_Output, TEXT("%s�ڴ�ϵ㣺%08X"), nIndex ? TEXT("ɾ��") : TEXT("����"), nBreakAddress);
}
//*********************************************************************************
// ����
//*********************************************************************************
void DebugDlg::OnIn()
{
	g_SingDebug = SETUP_IN;
	ResumeThread(g_pDlg->m_ThreadHandle);
	//OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("F7��������"));
}
//*********************************************************************************
// ����
//*********************************************************************************
void DebugDlg::OnJump()
{
	g_SingDebug = SETUP_OUT;
	ResumeThread(g_pDlg->m_ThreadHandle);
	//OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("F8��������"));
}
//*********************************************************************************
// ִ��
//*********************************************************************************
void DebugDlg::OnRun()
{
	g_SingDebug = SETUP_RUN;
	ResumeThread(g_pDlg->m_ThreadHandle);
	//OutputDebugEdit(g_pDlg->m_Edit_Output, TEXT("F9����"));
}
//*********************************************************************************
// ����Dump�ļ�
//*********************************************************************************
void DebugDlg::OnSavedump()
{
	DumpFile();
}
//*********************************************************************************
// �鿴ģ��
//*********************************************************************************
void DebugDlg::OnModule()
{
	ModuleDlg nModuleDlg;
	nModuleDlg.DoModal();

	if (!g_RecvAddress_Module)return;

	InitAsm(g_RecvAddress_Module);
	m_List_Asm.SetSelectionEx(0);
}
//*********************************************************************************
// �������뵼����
//*********************************************************************************
void DebugDlg::OnTable()
{
	TableDlg *nTableDlg=new TableDlg;
	nTableDlg->ShowWindow(TRUE);
}
//*********************************************************************************
// �鿴�ϵ��б�
//*********************************************************************************
void DebugDlg::OnBreaklist()
{
	BreakListDlg nBreakListDlg;
	nBreakListDlg.DoModal();
}
//*********************************************************************************
// �༭�ڴ�����
//*********************************************************************************
void DebugDlg::OnEditmemory()
{
	EditMemoryProc(nTempItem, nTempSubItem);
}
//*********************************************************************************
// ����ָ���ڴ��ַ
//*********************************************************************************
void DebugDlg::OnGotomemory()
{
	CString szAddress;
	szAddress = m_List_Memory.GetItemText(nTempItem, m_Flag_MemoryAddress);
	_stscanf_s(szAddress, TEXT("%X"), &g_SendAddress_Memory);
	g_SendAddress_Memory = g_SendAddress_Memory + nTempSubItem - 1;

	g_Ok_Memory = FALSE;
	GotoMemoryDlg nGotoMemoryDlg;
	nGotoMemoryDlg.DoModal();
	if (!g_Ok_Memory)return;

	if (g_SendAddress_Memory == g_RecvAddress_Memory)return;

	PrintMemory(g_RecvAddress_Memory);
	m_List_Memory.SetFocus();
	m_List_Memory.SetSelectionEx(0);

}



//*********************************************************************************
// ����Ӳ����д�ϵ���̣��ϵ����ͣ��ϵ㳤��
//*********************************************************************************
VOID DebugDlg::SetHardReadWriteProc(DWORD nType,DWORD nSize)
{
	DWORD nBreakAddress;
	CString nTempAddress;
	DWORD nIndex;
	BOOL nRet;

	nTempAddress = m_List_Memory.GetItemText(nTempItem, m_Flag_MemoryAddress);
	_stscanf_s(nTempAddress, TEXT("%x"), &nBreakAddress);

	nBreakAddress = nBreakAddress + nTempSubItem - 1;


	nIndex = IsHardBreakPoint(nBreakAddress);
	if (nIndex == -1)
	{
		nRet = SetHardBreakPointEx(nBreakAddress, nType, nSize);
		if (!nRet)
		{
			MessageBox(TEXT("Ӳ���ϵ�ʧ�ܣ�"), TEXT("ʧ�ܣ�"), MB_ICONERROR);
			return;
		}
		ClearBreakReadWrite();
		ReadWriteDlg * pReadWriteDlg = new ReadWriteDlg;
	}
	else
	{
		DelHardBreakPoint(nBreakAddress);
	}

	OutputDebugEdit(m_Edit_Output, TEXT("%sӲ��д��ϵ�%d��%08X"), nIndex == -1 ? TEXT("����") : TEXT("ɾ��"), nSize, nBreakAddress);

}
//*********************************************************************************
// �����ڴ��д�ϵ���̣��ϵ�����
//*********************************************************************************
VOID DebugDlg::SetMemoryReadWriteProc(DWORD nType)
{
	DWORD nBreakAddress;
	CString nTempAddress;
	DWORD nIndex;

	nTempAddress = m_List_Memory.GetItemText(nTempItem, m_Flag_MemoryAddress);
	_stscanf_s(nTempAddress, TEXT("%x"), &nBreakAddress);

	nBreakAddress = nBreakAddress + nTempSubItem - 1;


	nIndex = IsMemoryBreakPoint(nBreakAddress);


	if (HaveMemoryBreakPoint() && nIndex == FALSE)
	{
		MessageBox(TEXT("�Ѵ����ڴ�ϵ㣡�ϵ�ʧ�ܣ�"), TEXT("ʧ�ܣ�"), MB_ICONERROR);
		return;
	}

	if (!HaveMemoryBreakPoint())
	{
		SetMemoryBreakPoint(nBreakAddress, nType, FALSE, {});

		ClearBreakReadWrite();
		ReadWriteDlg * pReadWriteDlg = new ReadWriteDlg;
	}
	else
	{
		DelMemoryBreakPoint(nBreakAddress);
	}

	OutputDebugEdit(m_Edit_Output, TEXT("%s�ڴ�ϵ㣺%08X"), nIndex ? TEXT("ɾ��") : TEXT("����"), nBreakAddress);
}
//*********************************************************************************
// ����Ӳ��д��ϵ�1
//*********************************************************************************
void DebugDlg::OnHardwrite1()
{
	SetHardReadWriteProc(1, 1);
}
//*********************************************************************************
// ����Ӳ��д��ϵ�2
//*********************************************************************************
void DebugDlg::OnHardwrite2()
{
	SetHardReadWriteProc(1, 2);


}
//*********************************************************************************
// ����Ӳ��д��ϵ�4
//*********************************************************************************
void DebugDlg::OnHardwrite4()
{
	SetHardReadWriteProc(1, 3);


}
//*********************************************************************************
// ����Ӳ����ȡ�ϵ�1
//*********************************************************************************
void DebugDlg::OnHardread1()
{
	SetHardReadWriteProc(3, 1);

}
//*********************************************************************************
// ����Ӳ����ȡ�ϵ�2
//*********************************************************************************
void DebugDlg::OnHardread2()
{
	SetHardReadWriteProc(3, 2);

}
//*********************************************************************************
// ����Ӳ����ȡ�ϵ�4
//*********************************************************************************
void DebugDlg::OnHardread4()
{
	SetHardReadWriteProc(3, 3);

}
//*********************************************************************************
// �����ڴ�д��ϵ�
//*********************************************************************************
void DebugDlg::OnMemoryWrite()
{
	SetMemoryReadWriteProc(1);
}
//*********************************************************************************
// �����ڴ��ȡ�ϵ�
//*********************************************************************************
void DebugDlg::OnMemoryRead()
{
	SetMemoryReadWriteProc(0);
}
//*********************************************************************************
// �����ڴ��д�ϵ�
//*********************************************************************************
void DebugDlg::OnMemoryReadWrite()
{
	SetMemoryReadWriteProc(100);
}
//*********************************************************************************
// Դ�����
//*********************************************************************************
void DebugDlg::OnSourcedebug()
{
	if (IsSourceDebug())m_SourceDlg->ShowWindow(FALSE);
	else m_SourceDlg->ShowWindow(TRUE);	
}


//*********************************************************************************
// ��������
VOID DebugDlg::AntiPeb()
{
	DWORD nPebAddress = GetPebAddress(m_ProcessHandle);
	WriteMemoryByte(nPebAddress + 2, '\0');//BeingDebugged

	WriteMemoryByte(nPebAddress + 0x68, '\0');//Global

}
VOID DebugDlg::AntiNtQueryInformationProcess()
{

	DWORD nExplorerPID;
	DWORD nApiAddress;
	DWORD nHookCodeAddress;
	BYTE nJmpCode[5] = { 0xe9,0x0,0x0 ,0x0 ,0x0 };
	BYTE nHookCode[] = { 
		139,68,36,8,131,248,7,117,13,139,68,36,12,199,0,0,0,0,0,194,20,0,131,
		248,30,117,13,139,68,36,12,199,0,0,0,0,0,194,20,0,131,248,31,117,13,139,
		68,36,12,199,0,1,0,0,0,194,20,0,131,248,35,117,13,139,68,36,12,199,0,0,0,0,0,
		194,20,0,131,248,0,117,16,139,68,36,12,131,192,20,199,0,136,136,136,136,194,20,
		0,184,22,0,0,0,104,237,250,163,119,195 };
	nApiAddress = GetApiAddress(TEXT("NtQueryInformationProcess"));
	nHookCodeAddress = (DWORD)VirtualAllocEx(m_ProcessHandle, NULL, 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	GetWindowThreadProcessId(::FindWindow(L"Progman", NULL), &nExplorerPID);

	*(DWORD*)(nJmpCode + 1) = nHookCodeAddress - nApiAddress - 5;
	*(DWORD*)(nHookCode + _countof(nHookCode) - 5) = nApiAddress + 5;
	*(DWORD*)(nHookCode + 90) = nExplorerPID;

	WriteMemoryBytes(nHookCodeAddress, nHookCode, _countof(nHookCode));

	WriteMemoryBytes(nApiAddress, nJmpCode, _countof(nJmpCode));


		//000A0000 - 8B 44 24 08 - mov eax, [esp + 08]
		//000A0004 - 83 F8 07 - cmp eax, 07
		//000A0007 - 75 0D - jne 000A0016
		//000A0009 - 8B 44 24 0C - mov eax, [esp + 0C]
		//000A000D - C7 00 00000000 - mov[eax], 00000000
		//000A0013 - C2 1400 - ret 0014
		//000A0016 - 83 F8 1E - cmp eax, 1E
		//000A0019 - 75 0D - jne 000A0028
		//000A001B - 8B 44 24 0C - mov eax, [esp + 0C]
		//000A001F - C7 00 00000000 - mov[eax], 00000000
		//000A0025 - C2 1400 - ret 0014
		//000A0028 - 83 F8 1F - cmp eax, 1F
		//000A002B - 75 0D - jne 000A003A
		//000A002D - 8B 44 24 0C - mov eax, [esp + 0C]
		//000A0031 - C7 00 01000000 - mov[eax], 00000001
		//000A0037 - C2 1400 - ret 0014
		//000A003A - 83 F8 23 - cmp eax, 23
		//000A003D - 75 0D - jne 000A004C
		//000A003F - 8B 44 24 0C - mov eax, [esp + 0C]
		//000A0043 - C7 00 00000000 - mov[eax], 00000000
		//000A0049 - C2 1400 - ret 0014
		//000A004C - 83 F8 00 - cmp eax, 00
		//000A004F - 75 10 - jne 000A0061
		//000A0051 - 8B 44 24 0C - mov eax, [esp + 0C]
		//000A0055 - 83 C0 14 - add eax, 14
		//000A0058 - C7 00 88888888 - mov[eax], 88888888
		//000A005E - C2 1400 - ret 0014
		//000A0061 - B8 16000000 - mov eax, 00000016
		//000A0066 - 68 EDFAA377 - push ntdll.ZwQueryInformationProcess + 5
		//000A006B - C3 - ret

}
VOID DebugDlg::AntiZwSetInformationThread()
{
	DWORD nApiAddress;
	nApiAddress = GetApiAddress(TEXT("ZwSetInformationThread"));
	BYTE nHookCode[] = { 0xC2,0x10,0x00 };
	WriteMemoryBytes(nApiAddress, nHookCode, _countof(nHookCode));

	//ntdll.ZwSetInformationThread - B8 0A000000 - mov eax, 0000000A
	//ntdll.ZwSetInformationThread + 5 - 33 C9 - xor ecx, ecx
	//ntdll.ZwSetInformationThread + 7 - 8D 54 24 04 - lea edx, [esp + 04]
	//ntdll.ZwSetInformationThread + B - 64 FF 15 C0000000 - call fs : [000000C0]
	//ntdll.ZwSetInformationThread + 12 - 83 C4 04 - add esp, 04
	//ntdll.ZwSetInformationThread + 15 - C2 1000 - ret 0010

}
//*********************************************************************************



//*********************************************************************************
// Dump�ļ�����
//*********************************************************************************
VOID DebugDlg::DumpFile()
{
	//�����������ϵ�
	CloseAllBreakPoint();
	if (HaveMemoryBreakPoint())
	{
		MessageBox(TEXT("��ɾ���ڴ�ϵ�����ԣ�") , TEXT("��ʾ"), MB_ICONERROR);
		return;
	}

	DWORD nImageBassAddress = GetImageBassAddress();

	DWORD nPeSize = 0;				//PEͷ
	DWORD nImageSize = 0;			//�ڴ��д�С
	DWORD nFileSize = 0;			//�ļ���С
	DWORD nSectionNum = 0;			//��������
	PBYTE nPeHeadData = nullptr;	//PE����
	PBYTE nImageBuf = nullptr;		//�ļ�����
	FILE *pFile = nullptr;			//д���ļ�ָ��
	CString nFilePath;				//�����ļ�·��

	nPeHeadData = new BYTE[4096]{};

	//��ȡ�ļ�ͷ��Ϣ
	ReadMemoryBytes(nImageBassAddress, nPeHeadData, 4096);

	//��ȡPE��Ϣ
	PIMAGE_DOS_HEADER nDosHead = (PIMAGE_DOS_HEADER)nPeHeadData;
	PIMAGE_NT_HEADERS nNtHead = (PIMAGE_NT_HEADERS)(nPeHeadData + nDosHead->e_lfanew);
	PIMAGE_SECTION_HEADER nSecetionHead = IMAGE_FIRST_SECTION(nNtHead);

	//PEͷ��С
	nPeSize = nNtHead->OptionalHeader.SizeOfHeaders;
	//�ļ��ĳߴ�
	nImageSize = nNtHead->OptionalHeader.SizeOfImage;
	//��������	
	nSectionNum = nNtHead->FileHeader.NumberOfSections;


	//����exe����Ķѿռ�
	nImageBuf = new BYTE[nImageSize]{};

	//��ȡPE����
	ReadMemoryBytes(nImageBassAddress, nImageBuf, nPeSize);

	nFileSize += nPeSize;
	//��ȡÿ�����ε�����
	for (DWORD i = 0; i < nSectionNum; i++)
	{
		ReadMemoryBytes(nImageBassAddress + nSecetionHead[i].VirtualAddress, nImageBuf + nSecetionHead[i].PointerToRawData, nSecetionHead[i].SizeOfRawData);
		nFileSize += nSecetionHead[i].SizeOfRawData;
	}

	//�޸��ļ�����
	nDosHead = (PIMAGE_DOS_HEADER)nImageBuf;
	nNtHead = (PIMAGE_NT_HEADERS)((DWORD)nImageBuf + nDosHead->e_lfanew);
	nNtHead->OptionalHeader.FileAlignment = nNtHead->OptionalHeader.SectionAlignment;

	//���Ļ�ַ
	//nNtHead->OptionalHeader.ImageBase = 0x400000;

	nFilePath = GetRoute(TEXT("save"), TEXT("Dump_") + m_ProName);

	fopen_s(&pFile, nFilePath+"", "wb");
	fwrite(nImageBuf, nFileSize, 1, pFile);
	fclose(pFile);

	delete[] nPeHeadData;
	delete[] nImageBuf;

	MessageBox(TEXT("�ɹ������ļ���\n") + nFilePath, TEXT("�ɹ�"), MB_ICONINFORMATION);
}
//*********************************************************************************
// �������Խ���
//*********************************************************************************
BOOL DebugDlg::CreateDebugProcess()
{
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi = { 0 };

	//1. �������ԻỰ
	BOOL bRet = CreateProcess(m_FilePath, 0, 0, 0, 0, DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE, 0, 0, &si, &pi);

	if (!bRet)
	{
		MessageBox( TEXT("�������Խ���ʧ�ܣ�"), TEXT("��ʾ"), MB_ICONERROR);
		return FALSE;
	}
	g_Pid = pi.dwProcessId;
	m_Pid = pi.dwProcessId;
	m_Tid = pi.dwThreadId;
	m_ProcessHandle = pi.hProcess;
	m_ThreadHandle = pi.hThread;

	return TRUE;
}




//*********************************************************************************
// ��ȡ�ϵ��б��ļ�
//*********************************************************************************
VOID DebugDlg::ReadAllBreakInfo()
{
	HANDLE  pFile = nullptr;

	pFile = CreateFile(GetProgramDir() + m_ProName + TEXT("_HardBreakInfo.dat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile != (HANDLE)-1)
	{
		DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
		DWORD nReadSize = 0;
		ReadFile(pFile, (LPVOID)m_HardBreakPoint, nSize, &nReadSize, NULL);
	}
	CloseHandle(pFile);


	pFile = CreateFile(GetProgramDir() + m_ProName + TEXT("_SoftBreakInfo.dat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile != (HANDLE)-1)
	{
		DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
		DWORD nCount = nSize / sizeof(BREAKPOINT);
		DWORD nReadSize = 0;
		BREAKPOINT *nBreakPoint = new BREAKPOINT[nCount]{};
		ReadFile(pFile, nBreakPoint, nSize, &nReadSize, NULL);

		for (DWORD i=0;i<nCount;i++)
		{
			m_BreakPoint.push_back(nBreakPoint[i]);
		}
		delete[]nBreakPoint;
	}
	CloseHandle(pFile);
}
//*********************************************************************************
// д���ϵ��б��ļ�
//*********************************************************************************
VOID DebugDlg::WriteAllBreakInfo()
{
	FILE *pFile = nullptr;
	fopen_s(&pFile, GetProgramDir() + m_ProName + "_HardBreakInfo.dat", "wb");
	fwrite(m_HardBreakPoint, sizeof(HARDBREAKPOINT), 4, pFile);
	fclose(pFile);


	DWORD nCount = m_BreakPoint.size();
	BREAKPOINT *nBreakPoint = new BREAKPOINT[nCount]{};
	for (DWORD i = 0; i < nCount; i++)
	{
		memcpy_s(&nBreakPoint[i], sizeof(BREAKPOINT), &m_BreakPoint[i], sizeof(BREAKPOINT));
	}

	fopen_s(&pFile, GetProgramDir() + m_ProName + "_SoftBreakInfo.dat", "wb");
	fwrite(nBreakPoint, sizeof(BREAKPOINT), nCount, pFile);
	fclose(pFile);
	delete[]nBreakPoint;
}
//*********************************************************************************
// ��ԭ���жϵ�
//*********************************************************************************
VOID DebugDlg::RecoveryAllBreakInfo()
{
	OpenAllBreakPoint();
	OpenAllHardBreakPointEx();
}


//*********************************************************************************
// ������ת��ַ��¼
//*********************************************************************************
VOID DebugDlg::AddSelectRecord()
{
	if (m_RecordAddress.size() == m_SelectNum)
	{
		m_SelectNum = m_SelectNum + 1;
		m_RecordAddress.push_back({ nReadAddressFirst ,m_List_Asm.GetSelectionItem() });
	}
	else
	{
		m_RecordAddress[m_SelectNum] = { nReadAddressFirst ,m_List_Asm.GetSelectionItem() };
	}
}
//*********************************************************************************
// �ص���һ�ε�ַ��¼
//*********************************************************************************
VOID DebugDlg::BackSelectRecord()
{
	if (m_SelectNum == 0)return;

	m_SelectNum = m_SelectNum - 1;

	InitAsm(m_RecordAddress[m_SelectNum].nAddress);
	m_List_Asm.SetSelectionEx(m_RecordAddress[m_SelectNum].nSelect);

}
//*********************************************************************************
// ǰ��һ�ε�ַ��¼
//*********************************************************************************
VOID DebugDlg::ComeSelectRecord()
{
	if (m_RecordAddress.size() <= m_SelectNum + 1)return;

	m_SelectNum = m_SelectNum + 1;

	InitAsm(m_RecordAddress[m_SelectNum].nAddress);
	m_List_Asm.SetSelectionEx(m_RecordAddress[m_SelectNum].nSelect);
}



//*********************************************************************************
// ��������ļ����������̵��Խṹ��
//*********************************************************************************
BOOL DebugDlg::LoadSymbol(CREATE_PROCESS_DEBUG_INFO* pInfo) {

	//�򿪽��̻�ý��̾��
	m_SymHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_Pid);

	//��ʼ�����Ŵ�����
	SymInitialize(m_SymHandle,NULL,FALSE);
	
	//��������ļ�
	SymLoadModule64(m_SymHandle,pInfo->hFile,NULL,NULL,(DWORD64)pInfo->lpBaseOfImage,0);

	IMAGEHLP_MODULE64 nIMAGEHLP_MODULE64{};
	nIMAGEHLP_MODULE64.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
	SymGetModuleInfo64(m_SymHandle, (DWORD64)pInfo->lpBaseOfImage, &nIMAGEHLP_MODULE64);

	return TRUE;
}
//*********************************************************************************
// ��ʾԴ�룺�쳣��ַ
//*********************************************************************************
VOID DebugDlg::ShowSource(DWORD nAddress)
{
	if (IsSourceDebug() == FALSE)return;

	DWORD nDisplacement = 0;
	IMAGEHLP_LINE64 nSourceInfo{};						//Դ����Ϣ�ṹ��
	nSourceInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);	//��ʼ���ṹ��
	//ȡ��Դ����Ϣ(���̾���������쳣�ĵ�ַ����0�����Դ�������Ϣ�ṹ��)
	SymGetLineFromAddr64(m_SymHandle, nAddress,&nDisplacement,&nSourceInfo);

	OutputDebugEdit(m_Edit_Output, L"returned error : %d", GetLastError());

	if (GetLastError() != 0)
	{
		m_SourceDlg->m_List_Source.DeleteAllItems();
		for (DWORD i = 0;i <= 30; i++)m_SourceDlg->m_List_Source.InsertItem(i, TEXT(""));
		m_SourceDlg->m_List_Source.SetItemText(0, 3, TEXT("��ǰģ��û�з��ű�"));
		return;
	}

	//��ȡ�ļ�
	FILE *nFile = nullptr;
	CHAR nCode[1024]{};
	fopen_s(&nFile, CStringA(nSourceInfo.FileName), "r");

	if (nFile ==NULL)
	{
		m_SourceDlg->m_List_Source.DeleteAllItems();
		for (DWORD i = 0; i <= 30; i++)m_SourceDlg->m_List_Source.InsertItem(i, TEXT(""));
		m_SourceDlg->m_List_Source.SetItemText(0, 3, TEXT("��ǰģ��û�з��ű�"));
		return;
	}


	//��ȡǰN��
	DWORD nLineNumber = 0;
	while (nSourceInfo.LineNumber - 10 > nLineNumber)
	{
		fgets(nCode, 1024, nFile);
		++nLineNumber;
	}


	for (DWORD x = 0; x <= 30; x++)m_SourceDlg->m_List_Source.SetItemText(x, 2, TEXT(""));

	BOOL nHaveSelect = FALSE;
	for (int i = 0; i <= 30; i++)
	{
		LONG displacement;
		IMAGEHLP_LINE64 lineInfo = { 0 };
		lineInfo.SizeOfStruct = sizeof(lineInfo);

		SymGetLineFromName64(m_SymHandle,NULL,nSourceInfo.FileName,nLineNumber + i + 1,&displacement,&lineInfo);

		fgets(nCode, 1024, nFile);


		CString nThisAddress;
		nThisAddress.Format(TEXT("%08X"), lineInfo.Address);

		BOOL nHaveAddress = FALSE;
		for (DWORD x = 0; x <= 30; x++)
		{
			CString nTempAddress = m_SourceDlg->m_List_Source.GetItemText(x, 2);
			if (nTempAddress == nThisAddress)
			{
				nHaveAddress = TRUE;
				break;
			}
		}

		if (nHaveAddress)m_SourceDlg->m_List_Source.SetItemText(i, 2, TEXT(""));
		else m_SourceDlg->m_List_Source.SetItemText(i, 2, nThisAddress);


		if (lineInfo.Address == nAddress - 1 && nHaveSelect == FALSE)
		{
			nHaveSelect = TRUE;
			for (DWORD x = 0; x <= 30; x++)	m_SourceDlg->m_List_Source.SetItemText(x, 0, TEXT(""));

			m_SourceDlg->m_List_Source.SetItemText(i, 0, TEXT("��"));
			m_SourceDlg->m_List_Source.SetSelectionEx(i);
		}
		
		m_SourceDlg->m_List_Source.SetItemText(i, 1, nLineNumber + i + 1);
		m_SourceDlg->m_List_Source.SetItemText(i, 3, CString(nCode));

	}

	fclose(nFile);
}
VOID DebugDlg::ShowSymbol(DWORD nAddress)
{
	if (IsSourceDebug() == FALSE)return;

	DWORD64 nDisplacement = 0;
	PSYMBOL_INFO nSymbol_Info = (PSYMBOL_INFO)malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME);
	ZeroMemory(nSymbol_Info, sizeof(IMAGEHLP_LINE64) + MAX_SYM_NAME);
	nSymbol_Info->SizeOfStruct = sizeof(IMAGEHLP_LINE64) + MAX_SYM_NAME;
	nSymbol_Info->MaxNameLen =  MAX_SYM_NAME;

	SymFromAddr(m_SymHandle, nAddress, &nDisplacement, nSymbol_Info);

	OutputDebugEdit(m_Edit_Output, L"returned error : %d", GetLastError());

	if (GetLastError() != 0)
	{
		m_SourceDlg->m_List_Source.DeleteAllItems();
		for (DWORD i = 0; i <= 30; i++)m_SourceDlg->m_List_Source.InsertItem(i, TEXT(""));
		m_SourceDlg->m_List_Source.SetItemText(0, 3, TEXT("��ǰģ��û�з��ű�"));
		return;
	}


}
//*********************************************************************************
// �Ƿ�ΪԴ�����
//*********************************************************************************
BOOL DebugDlg::IsSourceDebug()
{
	if (m_SourceDlg == NULL)return FALSE;
	return 	m_SourceDlg->IsWindowVisible();
}