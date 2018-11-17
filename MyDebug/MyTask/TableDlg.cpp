// TableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "TableDlg.h"
#include "afxdialogex.h"
#include "DebugDlg.h"

extern DWORD g_Pid;
extern DebugDlg* g_pDlg;
extern DWORD nReadAddressFirst;
// TableDlg 对话框

IMPLEMENT_DYNAMIC(TableDlg, CDialogEx)

TableDlg::TableDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TABLE_DIALOG, pParent)
{
	//g_pDlg->m_Table.swap(vector<TABLE>());
	Create(IDD_TABLE_DIALOG);
}

TableDlg::~TableDlg()
{
}

void TableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODULE, m_List_Module);
	DDX_Control(pDX, IDC_LIST_TABLE, m_List_Table);
}


BEGIN_MESSAGE_MAP(TableDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MODULE, &TableDlg::OnClickListModule)
END_MESSAGE_MAP()


// TableDlg 消息处理程序
DWORD WINAPI RunThread(LPVOID lpThreadParameter)
{
	TableDlg *nDlg = (TableDlg*)lpThreadParameter;

	MODULELIST nModuleList = {};
	DWORD nNum = EnumModuleList(nModuleList, g_Pid);//枚举进程列表
	nDlg->PrintModuleList(nModuleList, nNum);//打印进程到列表
	ClearModuleList(nModuleList);

	return 0;
}

BOOL TableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List_Module.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Module.InsertColumn(0, TEXT("模块名称"), 0, 100);
	m_List_Module.InsertColumn(1, TEXT("模块地址"), 0, 70);

	m_List_Table.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List_Table.InsertColumn(0, TEXT("所属模块"), 0, 100);
	m_List_Table.InsertColumn(1, TEXT("函数名字"), 0, 150);
	m_List_Table.InsertColumn(2, TEXT("函数地址"), 0, 80);
	m_List_Table.InsertColumn(3, TEXT("IAT"), 0, 80);
	m_List_Table.InsertColumn(4, TEXT("类型"), 0, 80);



	if (g_pDlg->m_Table.size()==0)
	{
		CreateThread(NULL, NULL, RunThread, (LPVOID)this, NULL, NULL);
	}
	else
	{
		for (DWORD i = 0; i < g_pDlg->m_Table.size(); i++)
		{
			CString str;
			m_List_Module.InsertItem(i, g_pDlg->m_Table[i].ModuleName);
			str.Format(L"%08X", g_pDlg->m_Table[i].ModuleAddress);
			m_List_Module.SetItemText(i, 1, str);

		}
	}

	return TRUE;
}

void TableDlg::PrintModuleList(MODULELIST &nModuleList, DWORD nTaskNum)
{
	g_pDlg->m_Progress_Table.SetStep(nTaskNum);

	for (DWORD i = 0; i < nTaskNum; i++)
	{

		CString str;
		DWORD nIndex = m_List_Module.GetItemCount();
		m_List_Module.InsertItem(nIndex, nModuleList.szModule[nIndex]);
		str.Format(L"%08X", nModuleList.modBaseAddr[nIndex]);
		m_List_Module.SetItemText(nIndex, 1, str);

		g_pDlg->m_Static_Table.SetWindowText(TEXT("正在分析符号:") + nModuleList.szModule[nIndex]);

		GetTable(nModuleList.szModule[nIndex],nModuleList.modBaseAddr[nIndex], GetImageBassSize(nModuleList.modBaseAddr[nIndex]));


		g_pDlg->m_Progress_Table.StepIt();
	}
	g_pDlg->m_Progress_Table.ShowWindow(FALSE);
	g_pDlg->m_Static_Table.ShowWindow(FALSE);
	g_pDlg->InitAsm(nReadAddressFirst);
}

VOID TableDlg::GetTable(CString nModuleName, DWORD nImageBass, DWORD nSize)
{
	CString ExportModuleName;
	vector<DWORD>ExportFunctionAddress;
	vector<CString>ExportFunctionAddress_str;
	vector<CString>ExportFunctionName;

	vector<CString>ImportModuleName;
	vector<CString>ImportFunctionName;
	vector<DWORD>ImportFunctionAddress;
	vector<CString>ImportFunctionAddress_str;
	vector<DWORD>IAT;
	vector<CString>IAT_str;


	BYTE  nData[4096]{};

	g_pDlg->ReadMemoryBytes(nImageBass, nData, _countof(nData));

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS32 nNtHead = (PIMAGE_NT_HEADERS32)((LONG)nData + nDosHeade->e_lfanew);
	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));


	DWORD nImportHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD nImportAddress = nImportHeadRva + nImageBass;
	DWORD nImportSize = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;

	DWORD nExportHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	DWORD nExportAddress = nExportHeadRva + nImageBass;
	DWORD nExportSize = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;


	ZeroMemory(nData,_countof(nData));
	g_pDlg->ReadMemoryBytes(nImportAddress, nData, _countof(nData));
	PIMAGE_IMPORT_DESCRIPTOR nImportHead = (PIMAGE_IMPORT_DESCRIPTOR)nData;

	while (nImportHead->Name != 0 && nImportHead->OriginalFirstThunk)
	{
		DWORD nIndex;

		DWORD ImportDllNameRva = nImportHead->Name + (DWORD)nImageBass;

		BYTE nDllName[256]{};
		g_pDlg->ReadMemoryBytes(ImportDllNameRva, nDllName, _countof(nDllName));
		

		DWORD nIntAddress = nImportHead->OriginalFirstThunk + nImageBass;
		DWORD nIatAddress = nImportHead->FirstThunk + nImageBass;


		IMAGE_THUNK_DATA32 nThunkDataName{};
		IMAGE_THUNK_DATA32 nThunkDataAddress{};

		g_pDlg->ReadMemoryBytes(nIntAddress, (LPBYTE)&nThunkDataName, sizeof(IMAGE_THUNK_DATA32));
		g_pDlg->ReadMemoryBytes(nIatAddress, (LPBYTE)&nThunkDataAddress, sizeof(IMAGE_THUNK_DATA32));


		while (nThunkDataName.u1.AddressOfData != 0)
		{
			nIndex = m_List_Table.GetItemCount();
			//m_List_Table.InsertItem(nIndex, CString(nDllName));
			ImportModuleName.push_back(CString(nDllName));

			//导入的函数名字
			if (!IMAGE_SNAP_BY_ORDINAL(nThunkDataName.u1.Function))
			{
				DWORD nImportNameAddress = nThunkDataName.u1.Function + nImageBass;

				LPBYTE nImportName = (LPBYTE)malloc(sizeof(IMAGE_IMPORT_BY_NAME) + 256);
				ZeroMemory(nImportName, sizeof(IMAGE_IMPORT_BY_NAME) + 256);
				g_pDlg->ReadMemoryBytes(nImportNameAddress, nImportName, sizeof(IMAGE_IMPORT_BY_NAME) + 256);

				CString nIntName(nImportName + 2);
				//m_List_Table.SetItemText(nIndex, 1, nIntName);
				ImportFunctionName.push_back(nIntName);


				free(nImportName);
			}
			else
			{
				CString nIntOrdinal;
				nIntOrdinal.Format(TEXT("%d"), nThunkDataName.u1.Ordinal & 0xffff);
				//m_List_Table.SetItemText(nIndex,1, TEXT("序号：") + nIntOrdinal);
				ImportFunctionName.push_back(TEXT("序号：") + nIntOrdinal);

			}


			////导入的函数地址
			//CString nImportAddress;
			//nImportAddress.Format(TEXT("%08X"), nThunkDataAddress.u1.Function);
			//m_List_Table.SetItemText(nIndex, 2, nImportAddress);
			CString nTemp;
			ImportFunctionAddress.push_back(nThunkDataAddress.u1.Function);
			nTemp.Format(TEXT("%08X"), nThunkDataAddress.u1.Function);
			ImportFunctionAddress_str.push_back(nTemp);
			//CString nIatTableAddress;
			//nIatTableAddress.Format(TEXT("%08X"), nIatAddress);
			//m_List_Table.SetItemText(nIndex, 3, nIatTableAddress);
			m_List_Table.SetItemText(nIndex, 4, TEXT("导入"));
			nTemp.Format(TEXT("%08X"), nIatAddress);
			IAT.push_back(nIatAddress);
			IAT_str.push_back(nTemp);

			nIntAddress = nIntAddress + sizeof(IMAGE_THUNK_DATA32);
			nIatAddress = nIatAddress + sizeof(IMAGE_THUNK_DATA32);

			g_pDlg->ReadMemoryBytes(nIntAddress, (LPBYTE)&nThunkDataName, sizeof(IMAGE_THUNK_DATA32));
			g_pDlg->ReadMemoryBytes(nIatAddress, (LPBYTE)&nThunkDataAddress, sizeof(IMAGE_THUNK_DATA32));
			if (nThunkDataName.u1.AddressOfData == 0)break;

		}


		nImportHead++;
	}



	ZeroMemory(nData, _countof(nData));
	g_pDlg->ReadMemoryBytes(nExportAddress, nData, _countof(nData));
	PIMAGE_EXPORT_DIRECTORY nExportHead = (PIMAGE_EXPORT_DIRECTORY)nData;

	if (nExportAddress == nImageBass)
	{
		g_pDlg->m_Table.push_back({ nModuleName,nImageBass,nSize, ExportModuleName ,ExportFunctionAddress ,ExportFunctionAddress_str,ExportFunctionName
			,ImportModuleName,ImportFunctionName,ImportFunctionAddress,ImportFunctionAddress_str,IAT,IAT_str });
		return ;
	}

	PDWORD nExportFunctionAddress = (PDWORD)(nExportHead->AddressOfFunctions + nImageBass);
	PDWORD nExportFunctionNameAddress = (PDWORD)(nExportHead->AddressOfNames + nImageBass);
	PWORD nExportFunctionOrdinalsAddress = (PWORD)(nExportHead->AddressOfNameOrdinals + nImageBass);
	PDWORD nExportModuleNameAddress = (PDWORD)(nExportHead->Name + nImageBass);


	BYTE nExportModuleName[256]{};
	g_pDlg->ReadMemoryBytes((DWORD)nExportModuleNameAddress, nExportModuleName, _countof(nExportModuleName));

	ExportModuleName = nExportModuleName;


	DWORD * ExportAddress = new DWORD[nExportHead->AddressOfFunctions]{};

	for (DWORD i = 0; i < nExportHead->AddressOfFunctions; i++)
	{
		DWORD nAddress;
		g_pDlg->ReadMemoryBytes((DWORD)nExportFunctionAddress, (LPBYTE)&nAddress, sizeof(DWORD));
		nAddress = nAddress + nImageBass;

		ExportAddress[i]=(nAddress);

		nExportFunctionAddress++;

	}

	for (DWORD i = 0; i < nExportHead->NumberOfNames; i++)
	{
		DWORD nIndex; 
		nIndex = m_List_Table.GetItemCount();

		BYTE nExportFunctionName[256]{};
		DWORD nAddress;
		WORD nOrdinals;
		g_pDlg->ReadMemoryBytes((DWORD)nExportFunctionNameAddress, (LPBYTE)&nAddress, sizeof(DWORD));
		nAddress = nAddress + nImageBass;
		g_pDlg->ReadMemoryBytes(nAddress, nExportFunctionName, _countof(nExportFunctionName));

		g_pDlg->ReadMemoryBytes((DWORD)nExportFunctionOrdinalsAddress, (LPBYTE)&nOrdinals, sizeof(WORD));

		//m_List_Table.InsertItem(nIndex, CString(nExportModuleName));
		//m_List_Table.SetItemText(nIndex, 1, CString(nExportFunctionName));

		CString nTemp;
		ExportFunctionAddress.push_back(ExportAddress[nOrdinals]);
		nTemp.Format(TEXT("%08X"), ExportAddress[nOrdinals]);
		ExportFunctionAddress_str.push_back(nTemp);
		ExportFunctionName.push_back(CString(nExportFunctionName));

		//CString szAddress;
		//szAddress.Format(TEXT("%08X"), ExportAddress[nOrdinals]);
		//m_List_Table.SetItemText(nIndex, 2, szAddress);
		//m_List_Table.SetItemText(nIndex, 3, szAddress);
		//m_List_Table.SetItemText(nIndex, 4, TEXT("导出"));


		//ExportName.push_back(CString(nExportFunctionName));
		//ExportOrdinals.push_back(nOrdinals);

		nExportFunctionNameAddress++;
		nExportFunctionOrdinalsAddress++;
	}

	delete[]ExportAddress;

	g_pDlg->m_Table.push_back({ nModuleName,nImageBass,nSize,ExportModuleName ,ExportFunctionAddress ,ExportFunctionAddress_str,ExportFunctionName
		,ImportModuleName,ImportFunctionName,ImportFunctionAddress,ImportFunctionAddress_str,IAT,IAT_str });

	return ;
}

DWORD TableDlg::GetImageBassSize(DWORD nImageBass)
{
	BYTE  nData[4096]{};

	g_pDlg->ReadMemoryBytes(nImageBass, nData, _countof(nData));

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS32 nNtHead = (PIMAGE_NT_HEADERS32)((LONG)nData + nDosHeade->e_lfanew);
	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	return nOptionalHead->SizeOfImage;
}

void TableDlg::OnClickListModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)return;

	DWORD nIndex = pNMItemActivate->iItem;
	DWORD nCount;

	m_List_Table.DeleteAllItems();


	for (DWORD i = 0; i < g_pDlg->m_Table[nIndex].ExportFunctionName.size(); i++)
	{
		nCount = m_List_Table.GetItemCount();
		m_List_Table.InsertItem(nCount, g_pDlg->m_Table[nIndex].ExportModuleName);
		m_List_Table.SetItemText(nCount, 1, g_pDlg->m_Table[nIndex].ExportFunctionName[i]);
		CString nTemp;
		nTemp.Format(TEXT("%08X"), g_pDlg->m_Table[nIndex].ExportFunctionAddress[i]);
		m_List_Table.SetItemText(nCount, 2, nTemp);
		m_List_Table.SetItemText(nCount, 3, nTemp);
		m_List_Table.SetItemText(nCount, 4, TEXT("导出"));
	}


	for (DWORD i = 0; i < g_pDlg->m_Table[nIndex].ImportModuleName.size(); i++)
	{
		nCount = m_List_Table.GetItemCount();
		m_List_Table.InsertItem(nCount, g_pDlg->m_Table[nIndex].ImportModuleName[i]);
		m_List_Table.SetItemText(nCount, 1, g_pDlg->m_Table[nIndex].ImportFunctionName[i]);
		CString nTemp;
		nTemp.Format(TEXT("%08X"), g_pDlg->m_Table[nIndex].ImportFunctionAddress[i]);
		m_List_Table.SetItemText(nCount, 2, nTemp);
		nTemp.Format(TEXT("%08X"), g_pDlg->m_Table[nIndex].IAT[i]);
		m_List_Table.SetItemText(nCount, 3, nTemp);
		m_List_Table.SetItemText(nCount, 4, TEXT("导入"));
	}

}
