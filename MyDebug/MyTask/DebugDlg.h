#pragma once
#include "ClistCtrlEx.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
#include <Psapi.h>
#include "EditAsmDlg.h"
#include "EditMemoryDlg.h"
#include "ModuleDlg.h"
#include "BreakListDlg.h"
#include "GotoMemoryDlg.h"
#include "ReadWriteDlg.h"
#include "LoadPlugin.h"
#include "ConditionBreakDlg.h"
#include "TableDlg.h"
#include "SourceDlg.h"

using std::vector;

//���������ͷ�ļ�����̬��
#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "BeaEngine_4.1/Win32/headers/BeaEngine.h"
#pragma comment (lib , "BeaEngine_4.1/Win32/Win32/Lib/BeaEngine.lib")
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")



// DebugDlg �Ի���

class DebugDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DebugDlg)

public:
	DebugDlg(CString nProName, DWORD nPid);   // ��׼���캯��
	DebugDlg(CString nFilePath, CString nProName);
	virtual ~DebugDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEBUG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_Num_Asm = 27;
	DWORD m_Num_Heap = 128;
	DWORD m_Num_Memory = 9;


	typedef struct _BREAKPOINT
	{
		DWORD nAddress;
		BYTE nData;
		BOOL nIsCondition;
		CONDITION nCondition;
	}BREAKPOINT,*PBREAKPOINT;

	typedef struct _HARDBREAKPOINT
	{
		DWORD nAddress;
		DWORD nType;
		DWORD nSize;
		BOOL nIsCondition;
		CONDITION nCondition;
	}HARDBREAKPOINT, *PHARDBREAKPOINT;

	typedef struct _MEMORYBREAKPOINT
	{
		DWORD nAddress;
		DWORD nOldPageProtect;
		DWORD nType;
		BOOL nIsCondition;
		CONDITION nCondition;
	}MEMORYBREAKPOINT, *PMEMORYBREAKPOINT;

	typedef struct _EFLAGS
	{
		BYTE CF : 1;
		BYTE NULL1 : 1;
		BYTE PF : 1;
		BYTE NULL2 : 1;
		BYTE AF : 1;
		BYTE NULL3 : 1;
		BYTE ZF : 1;
		BYTE SF : 1;
		BYTE TF : 1;
		BYTE IF : 1;
		BYTE DF : 1;
		BYTE OF : 1;
		BYTE NULL4 : 4;
	}EFLAGS ,*PEFLAGS;
	typedef struct _DR6 {
		/*
		//     �ϵ����б�־λ�����λ��DR0~3��ĳ���ϵ㱻���У�������쳣����ǰ����Ӧ
		// ��B0~3�ͻᱻ��Ϊ1��
		*/
		unsigned Dr0 : 1;  // Dr0�ϵ㴥����λ
		unsigned Dr1 : 1;  // Dr1�ϵ㴥����λ
		unsigned Dr2 : 1;  // Dr2�ϵ㴥����λ
		unsigned Dr3 : 1;  // Dr3�ϵ㴥����λ
						  /*
						  // �����ֶ�
						  */
		unsigned Reserve1 : 9;
		/*
		// ����״̬�ֶ�
		*/
		unsigned BD : 1;  // ���ƼĴ����������ϵ�󣬴�λ����Ϊ1
		unsigned BS : 1;  // �����쳣����������Ҫ��Ĵ���EFLAGS��TF����ʹ��
		unsigned BT : 1;  // ��λ��TSS��T��־����ʹ�ã����ڽ���CPU�����л��쳣
						  /*
						  // �����ֶ�
						  */
		unsigned Reserve2 : 16;
	}DR6, *PDR6;
	typedef struct _DR7
	{
		unsigned Dr0_L : 1;		//1
		unsigned Dr0_G : 1;
		unsigned Dr1_L : 1;
		unsigned Dr1_G : 1;
		unsigned Dr2_L : 1;
		unsigned Dr2_G : 1;
		unsigned Dr3_L : 1;
		unsigned Dr3_G : 1;

		unsigned NULL1 : 3;		//001
		unsigned GD : 1;		//����
		unsigned NULL2 : 2;		//00

		unsigned Dr0_RWE : 2;	//00:ִֻ�� 01:д�����ݶϵ� 10:I/O�˿ڶϵ� 11:����д���ݶϵ�
		unsigned Dr0_LEN : 2;	//00:1�ֽ� 01:2�ֽ� 10:���� 11:4�ֽ�
		unsigned Dr1_RWE : 2;
		unsigned Dr1_LEN : 2;
		unsigned Dr2_RWE : 2;
		unsigned Dr2_LEN : 2;
		unsigned Dr3_RWE : 2;
		unsigned Dr3_LEN : 2;

	}DR7,*PDR7;

	typedef struct _BREAKREADWRITE
	{
		DWORD nAddress;
		CString nAsm;
	}BREAKREADWRITE, *PBREAKREADWRITE;

	vector<BREAKPOINT>m_BreakPoint;
	vector<BREAKPOINT>m_TempBreakPoint;

	HARDBREAKPOINT m_HardBreakPoint[4]{};
	DWORD m_TempHardBreakPoint[4]{};
	vector<BREAKREADWRITE>m_HardBreakReadWrite;

	MEMORYBREAKPOINT m_MemoryBreakPoint = {};

	CString m_WinText;
	CString m_FilePath;
	CString m_ProName;
	DWORD m_Pid = 0;
	DWORD m_Tid = 0;
	LoadPlugin m_LoadPlugin;

	HANDLE m_ProcessHandle = nullptr;
	HANDLE m_ThreadHandle = nullptr;

	DWORD m_Flag_Break = 0;
	DWORD m_Flag_Address = 1;
	DWORD m_Flag_Address_Show = 2;
	DWORD m_Flag_Opcode = 3;
	DWORD m_Flag_Asm = 4;
	DWORD m_Flag_Asm_Show = 5;
	DWORD m_Flag_Mark = 6;
	DWORD m_Flag_HeapAddress = 0;
	DWORD m_Flag_HeapData = 1;
	DWORD m_Flag_MemoryAddress = 0;
	//DWORD m_Flag_MemoryData = 1;
	DWORD m_Flag_MemoryStr = 17;

	COLORREF m_EaxColor = 0;
	COLORREF m_EbxColor = 0;
	COLORREF m_EcxColor = 0;
	COLORREF m_EdxColor = 0;
	COLORREF m_EsiColor = 0;
	COLORREF m_EdiColor = 0;
	COLORREF m_EbpColor = 0;
	COLORREF m_EspColor = 0;
	COLORREF m_EipColor = 0;
	COLORREF m_Dr0Color = 0;
	COLORREF m_Dr1Color = 0;
	COLORREF m_Dr2Color = 0;
	COLORREF m_Dr3Color = 0;
	COLORREF m_Dr6Color = 0;
	COLORREF m_Dr7Color = 0;
	COLORREF m_EsColor = 0;
	COLORREF m_CsColor = 0;
	COLORREF m_SsColor = 0;
	COLORREF m_DsColor = 0;
	COLORREF m_FsColor = 0;
	COLORREF m_GsColor = 0;
	COLORREF m_CfColor = 0;
	COLORREF m_PfColor = 0;
	COLORREF m_AfColor = 0;
	COLORREF m_ZfColor = 0;
	COLORREF m_SfColor = 0;
	COLORREF m_TfColor = 0;
	COLORREF m_DfColor = 0;
	COLORREF m_OfColor = 0;


	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDblclkListHeap(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawListMemory(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawListAsm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawListHeap(NMHDR *pNMHDR, LRESULT *pResult);
	VOID SetAllWindowsLong();
	VOID SetListType();
	VOID SetPluginMenu();
	VOID InitInformation();
	VOID InitInformationEx(DWORD nImageBase);

	CMenu m_Menu;
	ClistCtrlEx m_List_Asm;
	ClistCtrlEx m_List_Heap;
	ClistCtrlEx m_List_Memory;
	CEdit m_Edit_Output;
	CEdit m_Edit_Print;
	CComboBox m_Combo_Address;
	CProgressCtrl m_Progress_Table;
	CStatic m_Static_Table;
	CStatic m_Static_Command;
	CEdit m_Edit_Eax;
	CEdit m_Edit_Ebx;
	CEdit m_Edit_Ecx;
	CEdit m_Edit_Edx;
	CEdit m_Edit_Esi;
	CEdit m_Edit_Edi;
	CEdit m_Edit_Ebp;
	CEdit m_Edit_Esp;
	CEdit m_Edit_Eip;
	CEdit m_Edit_Dr0;
	CEdit m_Edit_Dr1;
	CEdit m_Edit_Dr2;
	CEdit m_Edit_Dr3;
	CEdit m_Edit_Dr6;
	CEdit m_Edit_Dr7;
	CEdit m_Edit_Es;
	CEdit m_Edit_Cs;
	CEdit m_Edit_Ss;
	CEdit m_Edit_Ds;
	CEdit m_Edit_Fs;
	CEdit m_Edit_Gs;
	CEdit m_Edit_Cf;
	CEdit m_Edit_Pf;
	CEdit m_Edit_Af;
	CEdit m_Edit_Zf;
	CEdit m_Edit_Sf;
	CEdit m_Edit_Tf;
	CEdit m_Edit_Df;
	CEdit m_Edit_Of;


	VOID InitAsm(DWORD nReadAddress);
	VOID PrintOpcode(BYTE* pOpcode, DWORD nSize, DWORD nItem);
	VOID PrintBreakFlag(DWORD nBreakAddress, DWORD nItem);
	VOID PrintOnceAsm(DWORD nReadAddress);
	CString ShowExportName(CString szAddress);
	CString ShowAsmName(CString szAddress, CString szAsm);
	BOOL CheckCommand(CString nCommand);
	DWORD GetBeforeAddress(DWORD nReadAddress);
	DWORD GetNextAddress(DWORD nReadAddress);

	DWORD GetOepAddress();
	DWORD GetOepAddressEx(DWORD nImageBase);
	DWORD GetDataAddress();
	DWORD GetImageBassAddress();

	VOID SetBreakPoint(DWORD nBreakAddress, BOOL nIsCondotion, CONDITION nCondotion);
	VOID SetBreakPointEx(DWORD nBreakAddress);
	VOID SetTempBreakPoint(DWORD nBreakAddress);

	VOID OpenAllBreakPoint();
	VOID CloseAllBreakPoint();

	VOID DelBreakPoint(DWORD nBreakAddress);
	VOID DelTempBreakPoint(DWORD nBreakAddress);

	VOID ReadMemoryDword(DWORD nAddress, LPDWORD nValue);
	VOID WriteMemoryDword(DWORD nAddress,DWORD nValue);

	VOID ReadMemoryByte(DWORD nAddress, LPBYTE nValue);
	VOID WriteMemoryByte(DWORD nAddress, BYTE nValue);

	VOID ReadMemoryBytes(DWORD nAddress, LPBYTE nValue,DWORD nLen);
	VOID WriteMemoryBytes(DWORD nAddress, LPBYTE nValue, DWORD nLen);

	VOID ReadMemoryWord(DWORD nAddress, LPWORD nValue);
	VOID WriteMemoryWord(DWORD nAddress, WORD nValue);

	VOID PrintContext(CONTEXT &nCONTEXT);
	VOID PrintHeap(CONTEXT &nCONTEXT);
	VOID ShowSingleAsm(DWORD nShowAddress);
	VOID RepairAsm(DWORD nReadAddress, DWORD nVirtualAddress, BYTE *nRepairBytes);
	DWORD FindAddress(DWORD nShowAddress);
	DWORD IsBreakPoint(DWORD nBreakAddress);
	DWORD IsTempBreakPoint(DWORD nBreakAddress);
	DWORD IsConditionBreakPoint(DWORD nBreakAddress);
	DWORD IsConditionSucessBreakPoint(DWORD nBreakPointIndex,CONTEXT nContext);

	DWORD GetApiAddress(CString nApi);

	BOOL IsCallRepAsm(DWORD nAddress);

	VOID PrintMemory(DWORD nAddress);

	BOOL SetHardBreakPoint(DWORD nBreakAddress, BOOL nIsCondotion, CONDITION nCondition);
	BOOL SetHardBreakPointEx(DWORD nBreakAddress, DWORD nType, DWORD nSize);
	DWORD IsHardBreakPoint(DWORD nBreakAddress);
	DWORD IsHardBreakPointEx(DWORD nBreakAddress);
	VOID DelHardBreakPoint(DWORD nBreakAddress);
	VOID OpenAllHardBreakPoint();
	VOID OpenAllHardBreakPointEx();
	VOID CloseAllHardBreakPoint();
	VOID SetTempHardBreakPoint(DWORD nBreakAddress);
	VOID DelTempHardBreakPoint(DWORD nBreakAddress);
	DWORD IsTempHardBreakPoint(DWORD nBreakAddress);
	DWORD IsConditionHardBreakPoint(DWORD nBreakPointIndex);
	DWORD IsConditionSucessHardBreakPoint(DWORD nBreakPointIndex, CONTEXT nContext);


	VOID SetMemoryBreakPoint(DWORD nBreakAddress, DWORD nType, BOOL nIsCondotion, CONDITION nCondotion);
	BOOL IsMemoryBreakPoint(DWORD nBreakAddress);
	BOOL HaveMemoryBreakPoint();
	VOID DelMemoryBreakPoint(DWORD nBreakAddress);
	DWORD GetMemoryBreakPointType();


	typedef struct _LISTCOLOR
	{
		DWORD Item;
		DWORD SubItem;
		COLORREF TextColor;
		COLORREF BkColor;
	}LISTCOLOR,*PLISTCOLOR;

	vector<LISTCOLOR>m_AsmColor;

	COLORREF COLOR_WHITE = RGB(255, 255, 255);
	COLORREF COLOR_BLACK = RGB(0, 0, 0);
	COLORREF COLOR_RED = RGB(255, 0, 0);
	COLORREF COLOR_GREEN = RGB(0, 178, 102);
	COLORREF COLOR_BLUE = RGB(51, 153, 255);
	COLORREF COLOR_ORANGE = RGB(255, 153, 0);
	COLORREF COLOR_VIOLET = RGB(153, 0, 153);
	
	VOID SetItemColor(DWORD Item, DWORD SubItem, COLORREF TextColor, COLORREF BkColor);
	VOID ClearItemColor();
	BOOL IsColorWord(CString nStr, LPDWORD nTextColor, LPDWORD nBkColor);
	BOOL IsJumpWord(CString nStr,LPDWORD nAddress);
	BOOL AsmToOpcaode(DWORD nAddress, CHAR *nAsm, LPBYTE nBytes, DWORD &nByteNum);
	VOID EditMemoryProc(DWORD nItem, DWORD nSubItem);

	DWORD nTempItem = 0;
	DWORD nTempSubItem = 0;
	afx_msg void OnDblclkListAsm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListMemory(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListAsm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListMemory(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListAsm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGotoaddress();
	afx_msg void OnChangeasm();
	afx_msg void OnSoftbreak();
	afx_msg void OnConditionsoftbreak();
	afx_msg void OnHardbreak();
	afx_msg void OnConditionhardbreak();
	afx_msg void OnIn();
	afx_msg void OnJump();
	afx_msg void OnRun();
	afx_msg void OnInaddress();
	afx_msg void OnModule();
	afx_msg void OnBreaklist();
	afx_msg void OnEditmemory();
	afx_msg void OnGotomemory();
	afx_msg void OnHardwrite1();
	afx_msg void OnHardwrite2();
	afx_msg void OnHardwrite4();
	afx_msg void OnHardread1();
	afx_msg void OnHardread2();
	afx_msg void OnHardread4();
	afx_msg void OnMemoryBreak();
	afx_msg void OnMemoryWrite();
	afx_msg void OnMemoryRead();
	afx_msg void OnMemoryReadWrite();
	afx_msg void OnTable();
	afx_msg void OnSavedump();


	VOID AddBreakReadWrite(DWORD nBreakAddress, CString nAsm);
	VOID ClearBreakReadWrite();
	CString GetOnceAsm(DWORD nReadAddress);
	VOID SetHardReadWriteProc(DWORD nType, DWORD nSize);
	VOID SetMemoryReadWriteProc(DWORD nType);

	VOID AntiPeb();
	VOID AntiNtQueryInformationProcess();
	VOID AntiZwSetInformationThread();

	VOID DumpFile();
	BOOL CreateDebugProcess();

	VOID ReadAllBreakInfo();
	VOID WriteAllBreakInfo();
	VOID RecoveryAllBreakInfo();

	typedef struct _SELECTRECORD
	{
		DWORD nAddress;
		DWORD nSelect;

	}SELECTRECORD, *PSELECTRECORD;

	vector<TABLE> m_Table;
	DWORD TABLECLOCK = 0x999;
	vector<SELECTRECORD>m_RecordAddress;
	DWORD m_SelectNum = 0;

	VOID AddSelectRecord();
	VOID BackSelectRecord();
	VOID ComeSelectRecord();
	BOOL LoadSymbol(CREATE_PROCESS_DEBUG_INFO* pInfo);
	VOID ShowSource(DWORD nAddress);
	VOID ShowSymbol(DWORD nAddress);
	BOOL IsSourceDebug();
	HANDLE m_SymHandle = (HANDLE)0x9999;
	SourceDlg *m_SourceDlg = nullptr;

	afx_msg void OnSourcedebug();
};
