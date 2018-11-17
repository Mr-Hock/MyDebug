#pragma once
#include <winternl.h>
#pragma comment(lib,"ntdll.lib")

#include <vector>

using std::vector;

//一个模块的导入导出函数表记录结构
typedef struct _TABLE
{
	CString ModuleName;							//模块的名字
	DWORD ModuleAddress;						//模块的加载地址
	DWORD ModuleSize;							//模块的尺寸

	CString ExportModuleName;					//导出表模块的名字
	vector<DWORD>ExportFunctionAddress;			//导出函数地址_十进制
	vector<CString>ExportFunctionAddress_str;	//导出函数地址_十六进制
	vector<CString>ExportFunctionName;			//导出的函数名字

	vector<CString>ImportModuleName;			//导入的模块名字
	vector<CString>ImportFunctionName;			//导入的函数名字
	vector<DWORD>ImportFunctionAddress;			//导入的函数地址_十进制
	vector<CString>ImportFunctionAddress_str;	//导入的函数地址_十六进制
	vector<DWORD>IAT;							//导入函数所IAT地址_十进制
	vector<CString>IAT_str;						//导入函数所IAT地址_十六进制

}TABLE, *PTABLE;

typedef struct _CONDITION
{
	BOOL OPEN_EAX;
	DWORD EAX;

	BOOL OPEN_EBX;
	DWORD EBX;

	BOOL OPEN_ECX;
	DWORD ECX;

	BOOL OPEN_EDX;
	DWORD EDX;

	BOOL OPEN_ESI;
	DWORD ESI;

	BOOL OPEN_EDI;
	DWORD EDI;

	BOOL OPEN_EBP;
	DWORD EBP;

	BOOL OPEN_ESP;
	DWORD ESP;

}CONDITION, *PCONDITION;


typedef struct _TASKLIST
{
	CString *szExeFile;
	DWORD *th32ProcessID;
	DWORD *cntThreads;
	DWORD *th32ParentProcessID;
	CString *szExeFilePath;
	CString *WorkingSetSize;
	CString *szPriority;

}TASKLIST, *PTASKLIST;
typedef struct _MODULELIST
{
	CString *szModule;
	CString *szExePath;
	DWORD *modBaseAddr;
	DWORD *dwSize;
}MODULELIST, *PMOUDLELIST;
typedef struct _THREADLIST
{
	DWORD *th32ThreadID;
	DWORD *th32OwnerProcessID;
	DWORD *tpBasePri;
	CString *szExePath;
	CString *szThreadState;

}THREADLIST, *PTHREADLIST;
typedef struct _HEAPLIST
{
	DWORD *th32HeapID;
	DWORD *th32ProcessID;
	CString *szExeFile;

}HEAPLIST, *PHEAPLIST;
typedef struct _HEAPENTYR
{
	DWORD *dwAddress;//堆块的线性起始地址
	DWORD *dwBlockSize;//堆块的大小

}HEAPENTYR, *PHEAPENTYR;

typedef struct _VIRUSINFO
{
	CHAR FileName[MAX_PATH]{};
	CHAR FileMd5[MAX_PATH]{};
}VIRUSINFO, *PVIRUSINFO;

typedef union _NTOHS_UNION
{
	struct _uu
	{
		BYTE A;
		BYTE B;
	}uu;
	WORD nPort;

}NTOHS_UNION, *PNTOHS_UNION;


typedef struct _UNICODE_STR
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR pBuffer;
} UNICODE_STR, *PUNICODE_STR;
typedef struct _PEB_FREE_BLOCK // 2 elements, 0x8 bytes
{
	struct _PEB_FREE_BLOCK * pNext;
	DWORD dwSize;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;
typedef struct _MYPEB // 65 elements, 0x210 bytes
{
	BYTE bInheritedAddressSpace;
	BYTE bReadImageFileExecOptions;
	BYTE bBeingDebugged;
	BYTE bSpareBool;
	LPVOID lpMutant;
	LPVOID lpImageBaseAddress;
	PPEB_LDR_DATA pLdr;
	LPVOID lpProcessParameters;
	LPVOID lpSubSystemData;
	LPVOID lpProcessHeap;
	PRTL_CRITICAL_SECTION pFastPebLock;
	LPVOID lpFastPebLockRoutine;
	LPVOID lpFastPebUnlockRoutine;
	DWORD dwEnvironmentUpdateCount;
	LPVOID lpKernelCallbackTable;
	DWORD dwSystemReserved;
	DWORD dwAtlThunkSListPtr32;
	PPEB_FREE_BLOCK pFreeList;
	DWORD dwTlsExpansionCounter;
	LPVOID lpTlsBitmap;
	DWORD dwTlsBitmapBits[2];
	LPVOID lpReadOnlySharedMemoryBase;
	LPVOID lpReadOnlySharedMemoryHeap;
	LPVOID lpReadOnlyStaticServerData;
	LPVOID lpAnsiCodePageData;
	LPVOID lpOemCodePageData;
	LPVOID lpUnicodeCaseTableData;
	DWORD dwNumberOfProcessors;
	DWORD dwNtGlobalFlag;
	LARGE_INTEGER liCriticalSectionTimeout;
	DWORD dwHeapSegmentReserve;
	DWORD dwHeapSegmentCommit;
	DWORD dwHeapDeCommitTotalFreeThreshold;
	DWORD dwHeapDeCommitFreeBlockThreshold;
	DWORD dwNumberOfHeaps;
	DWORD dwMaximumNumberOfHeaps;
	LPVOID lpProcessHeaps;
	LPVOID lpGdiSharedHandleTable;
	LPVOID lpProcessStarterHelper;
	DWORD dwGdiDCAttributeList;
	LPVOID lpLoaderLock;
	DWORD dwOSMajorVersion;
	DWORD dwOSMinorVersion;
	WORD wOSBuildNumber;
	WORD wOSCSDVersion;
	DWORD dwOSPlatformId;
	DWORD dwImageSubsystem;
	DWORD dwImageSubsystemMajorVersion;
	DWORD dwImageSubsystemMinorVersion;
	DWORD dwImageProcessAffinityMask;
	DWORD dwGdiHandleBuffer[34];
	LPVOID lpPostProcessInitRoutine;
	LPVOID lpTlsExpansionBitmap;
	DWORD dwTlsExpansionBitmapBits[32];
	DWORD dwSessionId;
	ULARGE_INTEGER liAppCompatFlags;
	ULARGE_INTEGER liAppCompatFlagsUser;
	LPVOID lppShimData;
	LPVOID lpAppCompatInfo;
	UNICODE_STR usCSDVersion;
	LPVOID lpActivationContextData;
	LPVOID lpProcessAssemblyStorageMap;
	LPVOID lpSystemDefaultActivationContextData;
	LPVOID lpSystemAssemblyStorageMap;
	DWORD dwMinimumStackCommit;
} MYPEB, *PMYPEB;
typedef struct _PROCESS_BASIC_INFORMATION32 {
	NTSTATUS ExitStatus;
	UINT32 PebBaseAddress;
	UINT32 AffinityMask;
	UINT32 BasePriority;
	UINT32 UniqueProcessId;
	UINT32 InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION32;

void OutputDebugStringEx(const TCHAR* format, ...);
void OutputDebugEdit(CEdit &m_Edit_Info, const TCHAR* format, ...);

void MyRtlAdjustPrivilege();
DWORD EnumTaskList(TASKLIST &nTaskList);
DWORD EnumModuleList(MODULELIST &nModuleList, DWORD nPid);
DWORD EnumThreadList(THREADLIST &nThreadList, DWORD nPid);
DWORD EnumHeapList(HEAPLIST &nHeapList);
DWORD EnumHeapEntyr(HEAPENTYR &nHeapEntry, DWORD nHeapId, DWORD nPid);
void ClearTaskList(TASKLIST &nTaskList);
void ClearModuleList(MODULELIST &nModuleList);
void ClearThreadList(THREADLIST &nThreadList);
void ClearHeapList(HEAPLIST &nHeapList);
void ClearHeapEntyr(HEAPENTYR &nHeapEntyr);
void EndProcess(DWORD nPid);
void EndThread(DWORD nThreadid);
void GetProcessFilePath(HANDLE hProcess, CString& sFilePath);
void SetProcessState(DWORD nPid, BOOL nSuspend);
void SetThreadState(DWORD nThreadid, BOOL nSuspend);
DWORD GetMemState();
void OpenFile(CString &nFileNamePath);
void GetCpuState(CString& nStr);
bool GetKey(int vk_code);
void GetProcessName(CString &nProcessName, DWORD nPid);
void GetThreadState(CString &nState, DWORD nThreadid);
void GetProcessPriority(CString &nState, HANDLE nHandle);
void OpenFileInfo(HWND nHwnd, CString &nFileRoute);
DWORD GetProcessPid(CString nProcessName);

CString GetProgramDir();
CString GetPathEx(CString nPath);
void ClearMemory();
CString GetPathFrom();
vector<CString> GetDriverList();
CString GetPathShortFileName(CString nPath);
DWORD SegmentText(vector<CString>&nText, CString &nSrc, CString nSeg);
WORD NtohsEx(WORD ntemp);
VOID SetTransparentWin(HWND m_hWnd, BYTE nVal);
VOID SetCircularWin(HWND m_hWnd, BYTE nVal);
CString IntToIp(in_addr &nAddr);
DWORD IpToInt(CString nIp);
DWORD GetProcessThreadId(DWORD nPid);
DWORD GetPebAddress(HANDLE nHandle);
CString GetFileFullName(CString csFilePath);
CString GetFileLastName(CString nPath);
CString GetRoute(CString type, CString nFileName);
