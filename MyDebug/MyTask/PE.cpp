#include "stdafx.h"
#include "PE.h"


PE::PE()
{
}


PE::~PE()
{
}

void PE::AddEditData(CEdit &m_Edit_Info, CString &nData)
{
	m_Edit_Info.SetSel(-1);
	m_Edit_Info.ReplaceSel(nData);
	m_Edit_Info.SetSel(-1);
	m_Edit_Info.ReplaceSel(TEXT("\n"));

}
void PE::AddEditData(CEdit &m_Edit_Info, CString &nData, DWORD nDword)
{
	CString nTemp;
	nTemp.Format(TEXT("%08X"), nDword);

	m_Edit_Info.SetSel(-1);
	m_Edit_Info.ReplaceSel(nData + nTemp);
	m_Edit_Info.SetSel(-1);
	m_Edit_Info.ReplaceSel(TEXT("\n"));
}

//读取PE文件到内存
BOOL PE::SetPeData(CString nPath)
{
	HANDLE  pFile = nullptr;

	pFile = CreateFile(nPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == (HANDLE)-1)
	{
		MessageBox(NULL,TEXT("打开文件失败！"), TEXT("失败"), MB_ICONWARNING);
		return FALSE;
	}


	DWORD nSize = GetFileSize(pFile, 0);
	DWORD nReadSize = 0;
	nData = new LPBYTE[nSize]{};

	ReadFile(pFile, nData, nSize, &nReadSize, NULL);

	CloseHandle(pFile);


	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;

	if (nDosHeade->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}


	PIMAGE_NT_HEADERS nNtHead = (PIMAGE_NT_HEADERS)((LONG)nData + nDosHeade->e_lfanew);

	if (nNtHead->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	return TRUE;
}

//转换
DWORD PE::RvaToFoa_32(DWORD nRva)
{
	PIMAGE_DOS_HEADER nDosHead = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS32 nNtHead = (PIMAGE_NT_HEADERS32)((LONG)nData + nDosHead->e_lfanew);
	DWORD nSectionNum = nNtHead->FileHeader.NumberOfSections;
	CString nRvaValue;
	CString nFoaValue;
	DWORD nFoa = 0;

	PIMAGE_SECTION_HEADER nSeltionHead = (PIMAGE_SECTION_HEADER)((LONG)nData + nDosHead->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + nNtHead->FileHeader.SizeOfOptionalHeader);
	for (DWORD i = 0; i < nSectionNum; i++)
	{
		if (nRva >= nSeltionHead->VirtualAddress && nRva <= nSeltionHead->VirtualAddress + nSeltionHead->Misc.VirtualSize)
		{
			nFoa = nRva - nSeltionHead->VirtualAddress + nSeltionHead->PointerToRawData;
			return nFoa;
		}
		nSeltionHead++;
	}

	return -1;
}
DWORD PE::RvaToFoa_64(DWORD nRva)
{
	PIMAGE_DOS_HEADER nDosHead = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS64 nNtHead = (PIMAGE_NT_HEADERS64)((LONG)nData + nDosHead->e_lfanew);
	DWORD nSectionNum = nNtHead->FileHeader.NumberOfSections;
	CString nRvaValue;
	CString nFoaValue;
	DWORD nFoa = 0;

	PIMAGE_SECTION_HEADER nSeltionHead = (PIMAGE_SECTION_HEADER)((LONG)nData + nDosHead->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + nNtHead->FileHeader.SizeOfOptionalHeader);
	for (DWORD i = 0; i < nSectionNum; i++)
	{
		if (nRva >= nSeltionHead->VirtualAddress && nRva <= nSeltionHead->VirtualAddress + nSeltionHead->Misc.VirtualSize)
		{
			nFoa = nRva - nSeltionHead->VirtualAddress + nSeltionHead->PointerToRawData;
			return nFoa;
		}
		nSeltionHead++;
	}

	return -1;
}

//取区段名字
CString PE::GetSeltion_32(DWORD nRva)
{
	PIMAGE_DOS_HEADER nDosHead = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS32 nNtHead = (PIMAGE_NT_HEADERS32)((LONG)nData + nDosHead->e_lfanew);
	DWORD nSectionNum = nNtHead->FileHeader.NumberOfSections;
	CString nRvaValue;
	CString nFoaValue;
	DWORD nFoa = 0;

	PIMAGE_SECTION_HEADER nSeltionHead = (PIMAGE_SECTION_HEADER)((LONG)nData + nDosHead->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + nNtHead->FileHeader.SizeOfOptionalHeader);
	for (DWORD i = 0; i < nSectionNum; i++)
	{
		if (nRva >= nSeltionHead->VirtualAddress && nRva <= nSeltionHead->VirtualAddress + nSeltionHead->Misc.VirtualSize)
		{
			nFoa = nRva - nSeltionHead->VirtualAddress + nSeltionHead->PointerToRawData;

			CHAR nStr[9]{};
			memcpy_s(nStr, _countof(nStr), nSeltionHead->Name, 8);
			CString nTemp(nStr);
			return nTemp;
		}
		nSeltionHead++;
	}

	return CString("NULL");
}
CString PE::GetSeltion_64(DWORD nRva)
{
	PIMAGE_DOS_HEADER nDosHead = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS64 nNtHead = (PIMAGE_NT_HEADERS64)((LONG)nData + nDosHead->e_lfanew);
	DWORD nSectionNum = nNtHead->FileHeader.NumberOfSections;
	CString nRvaValue;
	CString nFoaValue;
	DWORD nFoa = 0;

	PIMAGE_SECTION_HEADER nSeltionHead = (PIMAGE_SECTION_HEADER)((LONG)nData + nDosHead->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + nNtHead->FileHeader.SizeOfOptionalHeader);
	for (DWORD i = 0; i < nSectionNum; i++)
	{
		if (nRva >= nSeltionHead->VirtualAddress && nRva <= nSeltionHead->VirtualAddress + nSeltionHead->Misc.VirtualSize)
		{
			nFoa = nRva - nSeltionHead->VirtualAddress + nSeltionHead->PointerToRawData;

			CHAR nStr[9]{};
			memcpy_s(nStr, _countof(nStr), nSeltionHead->Name, 8);
			CString nTemp(nStr);
			return nTemp;
		}
		nSeltionHead++;
	}

	return CString("NULL");
}

//取出PE的文件头，扩展头信息
VOID PE::GetPeInfo_32(CEdit &m_Edit_Info)
{

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS32 nNtHead = (PIMAGE_NT_HEADERS32)((LONG)nData + nDosHeade->e_lfanew);

	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER nSectionHead = (PIMAGE_SECTION_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + nFileHead->SizeOfOptionalHeader);
	//PIMAGE_SECTION_HEADER nSectionHead = IMAGE_FIRST_SECTION(nNtHead);

	tm *nFileTime = new tm{ 0 };
	//gmtime_s(nFileTime,(time_t *)&nFileHead->TimeDateStamp);
	localtime_s(nFileTime, (time_t *)&nFileHead->TimeDateStamp);

	CString nFileTimeFomat;
	nFileTimeFomat.Format(TEXT("%d年%d月%d日 %d时%d分%d秒"),
		nFileTime->tm_year + 1900, nFileTime->tm_mon + 1, nFileTime->tm_mday,
		nFileTime->tm_hour, nFileTime->tm_min, nFileTime->tm_sec);

	CString nPeData;
	CString nTempData;

	AddEditData(m_Edit_Info, CString("文件头======================================="));
	AddEditData(m_Edit_Info, CString("文件运行平台："), nFileHead->Machine);

	AddEditData(m_Edit_Info, CString("区段数量："), nFileHead->NumberOfSections);
	AddEditData(m_Edit_Info, CString("文件创建时间：") + nFileTimeFomat);
	AddEditData(m_Edit_Info, CString("扩展头大小："), nFileHead->SizeOfOptionalHeader);
	AddEditData(m_Edit_Info, CString("PE文件属性："), nFileHead->Characteristics);

	AddEditData(m_Edit_Info, CString("\n"));
	AddEditData(m_Edit_Info, CString("可选文件头======================================="));

	AddEditData(m_Edit_Info, CString("PE文件类型：") + CString(nOptionalHead->Magic == 0x010b ? CString("32位") : CString("64位")));
	AddEditData(m_Edit_Info, CString("代码区段总大小："), nOptionalHead->SizeOfCode);
	AddEditData(m_Edit_Info, CString("已初始化数据总大小："), nOptionalHead->SizeOfInitializedData);
	AddEditData(m_Edit_Info, CString("未初始化数据总大小："), nOptionalHead->SizeOfUninitializedData);
	AddEditData(m_Edit_Info, CString("OEP："), nOptionalHead->AddressOfEntryPoint);
	AddEditData(m_Edit_Info, CString("起始代码相对虚拟地址RVA："), nOptionalHead->BaseOfCode);
	AddEditData(m_Edit_Info, CString("起始数据相对虚拟地址RVA："), nOptionalHead->BaseOfData);
	AddEditData(m_Edit_Info, CString("默认加载基址："), nOptionalHead->ImageBase);
	AddEditData(m_Edit_Info, CString("内存对齐数："), nOptionalHead->SectionAlignment);
	AddEditData(m_Edit_Info, CString("文件对齐数："), nOptionalHead->FileAlignment);
	AddEditData(m_Edit_Info, CString("加载所需内存(内存对齐后)："), nOptionalHead->SizeOfImage);
	AddEditData(m_Edit_Info, CString("所有头部大小："), nOptionalHead->SizeOfHeaders);
	AddEditData(m_Edit_Info, CString("指使DLL标志："), nOptionalHead->DllCharacteristics);

	AddEditData(m_Edit_Info, CString("数据目录个数："), nOptionalHead->NumberOfRvaAndSizes);



	AddEditData(m_Edit_Info, CString("\n"));
	AddEditData(m_Edit_Info, CString("区段表======================================="));
	for (INT i = 0; i < nFileHead->NumberOfSections; i++)
	{
		AddEditData(m_Edit_Info, CString("区段表：") + CString(nSectionHead->Name));
		AddEditData(m_Edit_Info, CString("区段大小："), nSectionHead->Misc.VirtualSize);
		AddEditData(m_Edit_Info, CString("区段起始地址RVA："), nSectionHead->VirtualAddress);
		AddEditData(m_Edit_Info, CString("区段在文件中大小："), nSectionHead->SizeOfRawData);
		AddEditData(m_Edit_Info, CString("区段文件偏移："), nSectionHead->PointerToRawData);
		AddEditData(m_Edit_Info, CString("区段属性："), nSectionHead->Characteristics);
		AddEditData(m_Edit_Info, CString("\n"));

		nSectionHead++;
	}
}
VOID PE::GetPeInfo_64(CEdit &m_Edit_Info)
{

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS64 nNtHead = (PIMAGE_NT_HEADERS64)((LONG)nData + nDosHeade->e_lfanew);

	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER nSectionHead = (PIMAGE_SECTION_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + nFileHead->SizeOfOptionalHeader);
	//PIMAGE_SECTION_HEADER nSectionHead = IMAGE_FIRST_SECTION(nNtHead);

	tm *nFileTime = new tm{ 0 };
	//gmtime_s(nFileTime,(time_t *)&nFileHead->TimeDateStamp);
	localtime_s(nFileTime, (time_t *)&nFileHead->TimeDateStamp);

	CString nFileTimeFomat;
	nFileTimeFomat.Format(TEXT("%d年%d月%d日 %d时%d分%d秒"),
		nFileTime->tm_year + 1900, nFileTime->tm_mon + 1, nFileTime->tm_mday,
		nFileTime->tm_hour, nFileTime->tm_min, nFileTime->tm_sec);

	CString nPeData;
	CString nTempData;

	AddEditData(m_Edit_Info, CString("文件头======================================="));
	AddEditData(m_Edit_Info, CString("文件运行平台："), nFileHead->Machine);

	AddEditData(m_Edit_Info, CString("区段数量："), nFileHead->NumberOfSections);
	AddEditData(m_Edit_Info, CString("文件创建时间：") + nFileTimeFomat);
	AddEditData(m_Edit_Info, CString("扩展头大小："), nFileHead->SizeOfOptionalHeader);
	AddEditData(m_Edit_Info, CString("PE文件属性："), nFileHead->Characteristics);

	AddEditData(m_Edit_Info, CString("\n"));
	AddEditData(m_Edit_Info, CString("可选文件头======================================="));

	AddEditData(m_Edit_Info, CString("PE文件类型：") + CString(nOptionalHead->Magic == 0x010b ? CString("32位") : CString("64位")));
	AddEditData(m_Edit_Info, CString("代码区段总大小："), nOptionalHead->SizeOfCode);
	AddEditData(m_Edit_Info, CString("已初始化数据总大小："), nOptionalHead->SizeOfInitializedData);
	AddEditData(m_Edit_Info, CString("未初始化数据总大小："), nOptionalHead->SizeOfUninitializedData);
	AddEditData(m_Edit_Info, CString("OEP："), nOptionalHead->AddressOfEntryPoint);
	AddEditData(m_Edit_Info, CString("起始代码相对虚拟地址RVA："), nOptionalHead->BaseOfCode);
	AddEditData(m_Edit_Info, CString("起始数据相对虚拟地址RVA："), (DWORD)nOptionalHead->ImageBase);
	AddEditData(m_Edit_Info, CString("默认加载基址："), (DWORD)nOptionalHead->ImageBase);
	AddEditData(m_Edit_Info, CString("内存对齐数："), nOptionalHead->SectionAlignment);
	AddEditData(m_Edit_Info, CString("文件对齐数："), nOptionalHead->FileAlignment);
	AddEditData(m_Edit_Info, CString("加载所需内存(内存对齐后)："), nOptionalHead->SizeOfImage);
	AddEditData(m_Edit_Info, CString("所有头部大小："), nOptionalHead->SizeOfHeaders);
	AddEditData(m_Edit_Info, CString("指使DLL标志："), nOptionalHead->DllCharacteristics);

	AddEditData(m_Edit_Info, CString("数据目录个数："), nOptionalHead->NumberOfRvaAndSizes);



	AddEditData(m_Edit_Info, CString("\n"));
	AddEditData(m_Edit_Info, CString("区段表======================================="));
	for (INT i = 0; i < nFileHead->NumberOfSections; i++)
	{
		AddEditData(m_Edit_Info, CString("区段表：") + CString(nSectionHead->Name));
		AddEditData(m_Edit_Info, CString("区段大小："), nSectionHead->Misc.VirtualSize);
		AddEditData(m_Edit_Info, CString("区段起始地址RVA："), nSectionHead->VirtualAddress);
		AddEditData(m_Edit_Info, CString("区段在文件中大小："), nSectionHead->SizeOfRawData);
		AddEditData(m_Edit_Info, CString("区段文件偏移："), nSectionHead->PointerToRawData);
		AddEditData(m_Edit_Info, CString("区段属性："), nSectionHead->Characteristics);
		AddEditData(m_Edit_Info, CString("\n"));

		nSectionHead++;
	}
}

//取出扩展头指针
PIMAGE_OPTIONAL_HEADER32 PE::GetOptionalHead_32()
{
	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS32 nNtHead = (PIMAGE_NT_HEADERS32)((LONG)nData + nDosHeade->e_lfanew);

	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	return nOptionalHead;
}
PIMAGE_OPTIONAL_HEADER64 PE::GetOptionalHead_64()
{
	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS64 nNtHead = (PIMAGE_NT_HEADERS64)((LONG)nData + nDosHeade->e_lfanew);

	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	return nOptionalHead;
}

//遍历导出表
VOID PE::PrintExport_32(ClistCtrlEx &m_List_Emport)
{

	m_List_Emport.DeleteAllItems();


	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nExportHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	if (nExportHeadRva == 0)return;


	DWORD nExportHeadFoa = RvaToFoa_32((DWORD)nExportHeadRva) + (DWORD)nData;

	PIMAGE_EXPORT_DIRECTORY nExportHead = (PIMAGE_EXPORT_DIRECTORY)nExportHeadFoa;



	PDWORD nExportFunctionRva = (PDWORD)nExportHead->AddressOfFunctions;
	nExportFunctionRva = (PDWORD)(RvaToFoa_32((DWORD)nExportFunctionRva) + (DWORD)nData);


	PDWORD nExportNameRva = (PDWORD)nExportHead->AddressOfNames;
	nExportNameRva = (PDWORD)(RvaToFoa_32((DWORD)nExportNameRva) + (DWORD)nData);


	PWORD nExportOrdinalRva = (PWORD)nExportHead->AddressOfNameOrdinals;
	nExportOrdinalRva = (PWORD)(RvaToFoa_32((DWORD)nExportOrdinalRva) + (DWORD)nData);



	for (DWORD i = 0; i < nExportHead->NumberOfNames; i++)
	{
		PDWORD nExportNameFoa = (PDWORD)(RvaToFoa_32(*nExportNameRva) + (DWORD)nData);

		PCHAR nName = (PCHAR)nExportNameFoa;

		CString nRvaTemp, nFoaTemp;
		nRvaTemp.Format(TEXT("%08X"), *nExportFunctionRva);
		nFoaTemp.Format(TEXT("%08X"), RvaToFoa_32(*nExportFunctionRva));

		m_List_Emport.InsertItem(i, *nExportOrdinalRva);
		m_List_Emport.SetItemText(i, 1, nRvaTemp);
		m_List_Emport.SetItemText(i, 2, nFoaTemp);
		m_List_Emport.SetItemText(i, 3, CString(nName));

		nExportFunctionRva++;
		nExportNameRva++;
		nExportOrdinalRva++;
	}
}
VOID PE::PrintExport_64(ClistCtrlEx &m_List_Emport)
{

	m_List_Emport.DeleteAllItems();


	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nExportHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	if (nExportHeadRva == 0)return;


	DWORD nExportHeadFoa = RvaToFoa_64((DWORD)nExportHeadRva) + (DWORD)nData;

	PIMAGE_EXPORT_DIRECTORY nExportHead = (PIMAGE_EXPORT_DIRECTORY)nExportHeadFoa;



	PDWORD nExportFunctionRva = (PDWORD)nExportHead->AddressOfFunctions;
	nExportFunctionRva = (PDWORD)(RvaToFoa_64((DWORD)nExportFunctionRva) + (DWORD)nData);


	PDWORD nExportNameRva = (PDWORD)nExportHead->AddressOfNames;
	nExportNameRva = (PDWORD)(RvaToFoa_64((DWORD)nExportNameRva) + (DWORD)nData);


	PWORD nExportOrdinalRva = (PWORD)nExportHead->AddressOfNameOrdinals;
	nExportOrdinalRva = (PWORD)(RvaToFoa_64((DWORD)nExportOrdinalRva) + (DWORD)nData);



	for (DWORD i = 0; i < nExportHead->NumberOfNames; i++)
	{
		PDWORD nExportNameFoa = (PDWORD)(RvaToFoa_64(*nExportNameRva) + (DWORD)nData);

		PCHAR nName = (PCHAR)nExportNameFoa;

		CString nRvaTemp, nFoaTemp;
		nRvaTemp.Format(TEXT("%08X"), *nExportFunctionRva);
		nFoaTemp.Format(TEXT("%08X"), RvaToFoa_64(*nExportFunctionRva));

		m_List_Emport.InsertItem(i, *nExportOrdinalRva);
		m_List_Emport.SetItemText(i, 1, nRvaTemp);
		m_List_Emport.SetItemText(i, 2, nFoaTemp);
		m_List_Emport.SetItemText(i, 3, CString(nName));

		nExportFunctionRva++;
		nExportNameRva++;
		nExportOrdinalRva++;
	}
}

//遍历导入表
VOID PE::PrintImportDll_32(ClistCtrlEx &m_List_Import_Dll)
{
	m_List_Import_Dll.DeleteAllItems();

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nImportHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD nImportHeadFoa = RvaToFoa_32(nImportHeadRva) + (DWORD)nData;

	PIMAGE_IMPORT_DESCRIPTOR nImportHead = (PIMAGE_IMPORT_DESCRIPTOR)nImportHeadFoa;


	while (nImportHead->Name != 0)
	{
		DWORD nIndexDll = m_List_Import_Dll.GetItemCount();
		//DWORD nIndexFun = m_List_Import_Function.GetItemCount();
		DWORD ImportDllNameRva = nImportHead->Name;
		DWORD ImportDllNameFoa = RvaToFoa_32(ImportDllNameRva) + (DWORD)nData;
		CString nDllName((CHAR*)ImportDllNameFoa);

		CString nTemp;

		m_List_Import_Dll.InsertItem(nIndexDll, nDllName);

		nTemp.Format(TEXT("%08X"), nImportHead->OriginalFirstThunk);
		m_List_Import_Dll.SetItemText(nIndexDll, 1, nTemp);

		nTemp.Format(TEXT("%08X"), nImportHead->FirstThunk);
		m_List_Import_Dll.SetItemText(nIndexDll, 2, nTemp);

		nImportHead++;
	}
}
VOID PE::PrintImportDll_64(ClistCtrlEx &m_List_Import_Dll)
{
	m_List_Import_Dll.DeleteAllItems();

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nImportHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD nImportHeadFoa = RvaToFoa_64(nImportHeadRva) + (DWORD)nData;

	PIMAGE_IMPORT_DESCRIPTOR nImportHead = (PIMAGE_IMPORT_DESCRIPTOR)nImportHeadFoa;


	while (nImportHead->Name != 0)
	{
		DWORD nIndexDll = m_List_Import_Dll.GetItemCount();
		//DWORD nIndexFun = m_List_Import_Function.GetItemCount();
		DWORD ImportDllNameRva = nImportHead->Name;
		DWORD ImportDllNameFoa = RvaToFoa_64(ImportDllNameRva) + (DWORD)nData;
		CString nDllName((CHAR*)ImportDllNameFoa);

		CString nTemp;

		m_List_Import_Dll.InsertItem(nIndexDll, nDllName);

		nTemp.Format(TEXT("%08X"), nImportHead->OriginalFirstThunk);
		m_List_Import_Dll.SetItemText(nIndexDll, 1, nTemp);

		nTemp.Format(TEXT("%08X"), nImportHead->FirstThunk);
		m_List_Import_Dll.SetItemText(nIndexDll, 2, nTemp);

		nImportHead++;
	}
}
//导入表函数
VOID PE::PrintImportFunction_32(ClistCtrlEx &m_List_Import_Function,DWORD nIndex)
{

	m_List_Import_Function.DeleteAllItems();
	if (nIndex == -1)return;

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	DWORD nImportHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD nImportHeadFoa = RvaToFoa_32(nImportHeadRva) + (DWORD)nData;

	PIMAGE_IMPORT_DESCRIPTOR nImportHead = (PIMAGE_IMPORT_DESCRIPTOR)nImportHeadFoa;

	nImportHead += nIndex;

	DWORD ImportIntRva = nImportHead->OriginalFirstThunk;
	DWORD ImportIntFoa = RvaToFoa_32(ImportIntRva) + (DWORD)nData;

	PIMAGE_THUNK_DATA32 nIntHead = (PIMAGE_THUNK_DATA32)ImportIntFoa;



	while (nIntHead->u1.AddressOfData != 0)
	{
		DWORD nIndexFun = m_List_Import_Function.GetItemCount();

		PDWORD nThunk = (PDWORD)nIntHead->u1.Function;
		CString nTemp;

		if (!IMAGE_SNAP_BY_ORDINAL(nIntHead->u1.Function))
		{
			PDWORD nIntHeadNameFoa = (PDWORD)(RvaToFoa_32((DWORD)nThunk) + (DWORD)nData);

			IMAGE_IMPORT_BY_NAME *nIntNameHead = (IMAGE_IMPORT_BY_NAME*)nIntHeadNameFoa;

			CString nIntName(nIntNameHead->Name);
			m_List_Import_Function.InsertItem(nIndexFun, nIntName);

			nTemp.Format(TEXT("%08X"), nIntHead->u1.AddressOfData);
			m_List_Import_Function.SetItemText(nIndexFun, 1, nTemp);

			nTemp.Format(TEXT("%04X"), nIntNameHead->Hint);
			m_List_Import_Function.SetItemText(nIndexFun, 2, nTemp);

		}
		else
		{
			CString nIntOrdinal;
			nIntOrdinal.Format(TEXT("%d"), nIntHead->u1.Ordinal & 0xffff);
			m_List_Import_Function.InsertItem(nIndexFun, TEXT("序号：") + nIntOrdinal);

			nTemp.Format(TEXT("%08X"), nIntHead->u1.AddressOfData);
			m_List_Import_Function.SetItemText(nIndexFun, 1, nTemp);

			m_List_Import_Function.SetItemText(nIndexFun, 2, TEXT("-"));

		}


		nIntHead++;

	}

}
VOID PE::PrintImportFunction_64(ClistCtrlEx &m_List_Import_Function, DWORD nIndex)
{

	m_List_Import_Function.DeleteAllItems();
	if (nIndex == -1)return;

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	DWORD nImportHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD nImportHeadFoa = RvaToFoa_64(nImportHeadRva) + (DWORD)nData;

	PIMAGE_IMPORT_DESCRIPTOR nImportHead = (PIMAGE_IMPORT_DESCRIPTOR)nImportHeadFoa;

	nImportHead += nIndex;

	DWORD ImportIntRva = nImportHead->OriginalFirstThunk;
	DWORD ImportIntFoa = RvaToFoa_64(ImportIntRva) + (DWORD)nData;

	PIMAGE_THUNK_DATA64 nIntHead = (PIMAGE_THUNK_DATA64)ImportIntFoa;



	while (nIntHead->u1.AddressOfData != 0)
	{
		DWORD nIndexFun = m_List_Import_Function.GetItemCount();

		PDWORD nThunk = (PDWORD)nIntHead->u1.Function;
		CString nTemp;

		if (!IMAGE_SNAP_BY_ORDINAL(nIntHead->u1.Function))
		{
			PDWORD nIntHeadNameFoa = (PDWORD)(RvaToFoa_64((DWORD)nThunk) + (DWORD)nData);

			IMAGE_IMPORT_BY_NAME *nIntNameHead = (IMAGE_IMPORT_BY_NAME*)nIntHeadNameFoa;

			CString nIntName(nIntNameHead->Name);
			m_List_Import_Function.InsertItem(nIndexFun, nIntName);

			nTemp.Format(TEXT("%08X"), nIntHead->u1.AddressOfData);
			m_List_Import_Function.SetItemText(nIndexFun, 1, nTemp);

			nTemp.Format(TEXT("%04X"), nIntNameHead->Hint);
			m_List_Import_Function.SetItemText(nIndexFun, 2, nTemp);

		}
		else
		{
			CString nIntOrdinal;
			nIntOrdinal.Format(TEXT("%d"), nIntHead->u1.Ordinal & 0xffff);
			m_List_Import_Function.InsertItem(nIndexFun, TEXT("序号：") + nIntOrdinal);

			nTemp.Format(TEXT("%08X"), nIntHead->u1.AddressOfData);
			m_List_Import_Function.SetItemText(nIndexFun, 1, nTemp);

			m_List_Import_Function.SetItemText(nIndexFun, 2, TEXT("-"));

		}


		nIntHead++;

	}

}

//遍历资源表
VOID PE::PrintResource_32(ClistCtrlEx &m_List_Resource,LPBYTE pResDirRoot, IMAGE_RESOURCE_DIRECTORY* pDir, int nLevel)
{

	CString nTemp;
	// 找到第2层目录的首地址, 目录的位置是保存在第一层目录入口
	// 中的偏移信息里面的.
	// 因此先得到第一层的目录入口

	// 得到这一层目录的目录入口的总个数
	DWORD dwEntryCount = pDir->NumberOfIdEntries + pDir->NumberOfNamedEntries;

	// 获取到这一层目录的目录入口数组的首地址
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry = NULL;
	pEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir + 1);

	if (nLevel == 1) {
		for (DWORD i = 0; i < dwEntryCount; ++i) {

			// 第一层的目录入口中保存了两个信息:
			// 1. id(资源类型的id),有两类,一是整形id,而是字符串id
			// 解析资源的类型
			if (pEntry[i].NameIsString) {/*id是一个字符串*/
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResDirRoot);
				// 打印资源类型的名字
				//wprintf(L"[%s]\n", pIdStr->NameString);

				WCHAR szStr[MAX_PATH] = { 0 };
				memcpy_s(szStr, MAX_PATH, pIdStr->NameString, pIdStr->Length * sizeof(WCHAR));
				nTemp.Format(TEXT("[种类:%s]"), szStr);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);

			}
			else { /*id是一个整形值.*/
				char* pIdName[] =
				{
					"",
					"鼠标指针（Cursor）",
					"位图（Bitmap）",
					"图标（Icon）",
					"菜单（Menu）",
					"对话框（Dialog）",
					"字符串列表（String Table）",
					"字体目录（Font Directory）",
					"字体（Font）",
					"快捷键（Accelerators）",
					"非格式化资源（Unformatted）",
					"消息列表（Message Table）",
					"鼠标指针组（Group Cursor）",
					"",
					"图标组（Group Icon）",
					"",
					"版本信息（Version Information）"
				};
				if (pEntry[i].Id >= _countof(pIdName)) {
					//printf("[%d]\n", pEntry[i].Id);
					nTemp.Format(TEXT("[未知种类:%d]"), pEntry[i].Id);
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
				}
				else {
					CString nUincode(pIdName[pEntry[i].Id]);
					nTemp.Format(TEXT("[种类:%s]"), nUincode);
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
					//printf("[%s]\n", pIdName[pEntry[i].Id]);
				}
			}

			// 2. 偏移(以资源根目录首地址为偏移)
			IMAGE_RESOURCE_DIRECTORY* p2Dir = NULL;
			p2Dir = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResDirRoot);
			PrintResource_32(m_List_Resource,pResDirRoot, p2Dir, 2);
		}
	}
	else if (nLevel == 2) {
		for (DWORD i = 0; i < dwEntryCount; i++)
		{
			// 解析第二层的目录入口;
			// 1. id(资源的id)(有字符串,有整形)
			if (pEntry[i].NameIsString) {/*id是一个字符串*/
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResDirRoot);
				// 打印资源类型的名字
				//wprintf(L"\t[%s]\n", pIdStr->NameString);

				WCHAR szStr[MAX_PATH] = { 0 };
				memcpy_s(szStr, MAX_PATH, pIdStr->NameString, pIdStr->Length * sizeof(WCHAR));
				nTemp.Format(TEXT("资源ID:%s"), szStr);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
			}
			else {
				nTemp.Format(TEXT("资源ID:%04d"), (DWORD)pEntry[i].Id);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
				//printf("\t%04d\n", (DWORD)pEntry[i].Id);
			}


			// 2. 偏移(以资源根目录偏移, 偏移到第三层)
			IMAGE_RESOURCE_DIRECTORY* p3Dir = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResDirRoot);
			// 找到第3层目录的首地址
			PrintResource_32(m_List_Resource,pResDirRoot, p3Dir, 3);
		}

	}
	else if (nLevel == 3) {
		for (DWORD i = 0; i < dwEntryCount; ++i) {
			// 解析第三层
			// 1. id(没有什么用)
			// 2. 偏移(以资源根目录为偏移, 偏移到数据入口)
			IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)
				(pEntry[i].OffsetToData + pResDirRoot);
			//m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);

			nTemp.Format(TEXT("大小:%08X"), pDataEntry->Size);
			m_List_Resource.SetItemText(m_List_Resource.GetItemCount() - 1, 1, nTemp);

			nTemp.Format(TEXT("位置:%08X"),pDataEntry->OffsetToData);
			m_List_Resource.SetItemText(m_List_Resource.GetItemCount() - 1, 2, nTemp);

			//printf("\t\t大小:%08X 位置:%08X\n",pDataEntry->Size, pDataEntry->OffsetToData);
		}
	}

}
VOID PE::PrintResource_64(ClistCtrlEx &m_List_Resource, LPBYTE pResDirRoot, IMAGE_RESOURCE_DIRECTORY* pDir, int nLevel)
{

	CString nTemp;
	// 找到第2层目录的首地址, 目录的位置是保存在第一层目录入口
	// 中的偏移信息里面的.
	// 因此先得到第一层的目录入口

	// 得到这一层目录的目录入口的总个数
	DWORD dwEntryCount = pDir->NumberOfIdEntries + pDir->NumberOfNamedEntries;

	// 获取到这一层目录的目录入口数组的首地址
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry = NULL;
	pEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir + 1);

	if (nLevel == 1) {
		for (DWORD i = 0; i < dwEntryCount; ++i) {

			// 第一层的目录入口中保存了两个信息:
			// 1. id(资源类型的id),有两类,一是整形id,而是字符串id
			// 解析资源的类型
			if (pEntry[i].NameIsString) {/*id是一个字符串*/
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResDirRoot);
				// 打印资源类型的名字
				//wprintf(L"[%s]\n", pIdStr->NameString);

				WCHAR szStr[MAX_PATH] = { 0 };
				memcpy_s(szStr, MAX_PATH, pIdStr->NameString, pIdStr->Length * sizeof(WCHAR));
				nTemp.Format(TEXT("[种类:%s]"), szStr);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);

			}
			else { /*id是一个整形值.*/
				char* pIdName[] =
				{
					"",
					"鼠标指针（Cursor）",
					"位图（Bitmap）",
					"图标（Icon）",
					"菜单（Menu）",
					"对话框（Dialog）",
					"字符串列表（String Table）",
					"字体目录（Font Directory）",
					"字体（Font）",
					"快捷键（Accelerators）",
					"非格式化资源（Unformatted）",
					"消息列表（Message Table）",
					"鼠标指针组（Group Cursor）",
					"",
					"图标组（Group Icon）",
					"",
					"版本信息（Version Information）"
				};
				if (pEntry[i].Id >= _countof(pIdName)) {
					//printf("[%d]\n", pEntry[i].Id);
					nTemp.Format(TEXT("[未知种类:%d]"), pEntry[i].Id);
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
				}
				else {
					CString nUincode(pIdName[pEntry[i].Id]);
					nTemp.Format(TEXT("[种类:%s]"), nUincode);
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
					//printf("[%s]\n", pIdName[pEntry[i].Id]);
				}
			}

			// 2. 偏移(以资源根目录首地址为偏移)
			IMAGE_RESOURCE_DIRECTORY* p2Dir = NULL;
			p2Dir = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResDirRoot);
			PrintResource_64(m_List_Resource, pResDirRoot, p2Dir, 2);
		}
	}
	else if (nLevel == 2) {
		for (DWORD i = 0; i < dwEntryCount; i++)
		{
			// 解析第二层的目录入口;
			// 1. id(资源的id)(有字符串,有整形)
			if (pEntry[i].NameIsString) {/*id是一个字符串*/
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResDirRoot);
				// 打印资源类型的名字
				//wprintf(L"\t[%s]\n", pIdStr->NameString);

				WCHAR szStr[MAX_PATH] = { 0 };
				memcpy_s(szStr, MAX_PATH, pIdStr->NameString, pIdStr->Length * sizeof(WCHAR));
				nTemp.Format(TEXT("资源ID:%s"), szStr);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
			}
			else {
				nTemp.Format(TEXT("资源ID:%04d"), (DWORD)pEntry[i].Id);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
				//printf("\t%04d\n", (DWORD)pEntry[i].Id);
			}


			// 2. 偏移(以资源根目录偏移, 偏移到第三层)
			IMAGE_RESOURCE_DIRECTORY* p3Dir = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResDirRoot);
			// 找到第3层目录的首地址
			PrintResource_64(m_List_Resource, pResDirRoot, p3Dir, 3);
		}

	}
	else if (nLevel == 3) {
		for (DWORD i = 0; i < dwEntryCount; ++i) {
			// 解析第三层
			// 1. id(没有什么用)
			// 2. 偏移(以资源根目录为偏移, 偏移到数据入口)
			IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)
				(pEntry[i].OffsetToData + pResDirRoot);
			//m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);

			nTemp.Format(TEXT("大小:%08X"), pDataEntry->Size);
			m_List_Resource.SetItemText(m_List_Resource.GetItemCount() - 1, 1, nTemp);

			nTemp.Format(TEXT("位置:%08X"), pDataEntry->OffsetToData);
			m_List_Resource.SetItemText(m_List_Resource.GetItemCount() - 1, 2, nTemp);

			//printf("\t\t大小:%08X 位置:%08X\n",pDataEntry->Size, pDataEntry->OffsetToData);
		}
	}

}

//遍历重定位表
VOID PE::PrintRelPage_32(ClistCtrlEx &m_List_RelTab )
{

	m_List_RelTab.DeleteAllItems();

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nRelTabRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	if (nRelTabRva == 0)return;

	DWORD dwRelTabFoa = RvaToFoa_32(nRelTabRva) + (LONG)nData;

	IMAGE_BASE_RELOCATION* pRelTab = (IMAGE_BASE_RELOCATION*)(dwRelTabFoa);

	CString nTemp;

	while (pRelTab->SizeOfBlock != 0)
	{
		TypeOffset* pTypeOffset = NULL;
		pTypeOffset = (TypeOffset*)(pRelTab + 1);
		// SizeOfBlock保存的是包含结构体和Typeoffset数组的总字节数
		// 要计算Typeoffset数组的个数时候,先减去结构体的大小
		// 得到Typeoffset数组的字节数, 除以2就得到个数
		DWORD dwCount = (pRelTab->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;

		//printf("[%08X]\n", pRelTab->VirtualAddress);

		nTemp.Format(TEXT("%d"), m_List_RelTab.GetItemCount() + 1);
		m_List_RelTab.InsertItem(m_List_RelTab.GetItemCount(), nTemp);

		nTemp.Format(TEXT("%s"), GetSeltion_32(pRelTab->VirtualAddress));
		m_List_RelTab.SetItemText(m_List_RelTab.GetItemCount() - 1, 1, nTemp);

		nTemp.Format(TEXT("%08X"), pRelTab->VirtualAddress);
		m_List_RelTab.SetItemText(m_List_RelTab.GetItemCount() - 1, 2, nTemp);

		nTemp.Format(TEXT("%08X"), pRelTab->SizeOfBlock);
		m_List_RelTab.SetItemText(m_List_RelTab.GetItemCount() - 1, 3, nTemp);

		nTemp.Format(TEXT("%d"), (pRelTab->SizeOfBlock - 8) / sizeof(WORD));
		m_List_RelTab.SetItemText(m_List_RelTab.GetItemCount() - 1, 4, nTemp);

		// 得到下一个重定位块的首地址
		pRelTab = (IMAGE_BASE_RELOCATION*)((LPBYTE)pRelTab + pRelTab->SizeOfBlock);
	}

}
VOID PE::PrintRelPage_64(ClistCtrlEx &m_List_RelTab)
{

	m_List_RelTab.DeleteAllItems();

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nRelTabRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	if (nRelTabRva == 0)return;

	DWORD dwRelTabFoa = RvaToFoa_32(nRelTabRva) + (LONG)nData;

	IMAGE_BASE_RELOCATION* pRelTab = (IMAGE_BASE_RELOCATION*)(dwRelTabFoa);

	CString nTemp;

	while (pRelTab->SizeOfBlock != 0)
	{
		TypeOffset* pTypeOffset = NULL;
		pTypeOffset = (TypeOffset*)(pRelTab + 1);
		// SizeOfBlock保存的是包含结构体和Typeoffset数组的总字节数
		// 要计算Typeoffset数组的个数时候,先减去结构体的大小
		// 得到Typeoffset数组的字节数, 除以2就得到个数
		DWORD dwCount = (pRelTab->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;

		//printf("[%08X]\n", pRelTab->VirtualAddress);

		nTemp.Format(TEXT("%d"), m_List_RelTab.GetItemCount() + 1);
		m_List_RelTab.InsertItem(m_List_RelTab.GetItemCount(), nTemp);

		nTemp.Format(TEXT("%s"), GetSeltion_32(pRelTab->VirtualAddress));
		m_List_RelTab.SetItemText(m_List_RelTab.GetItemCount() - 1, 1, nTemp);

		nTemp.Format(TEXT("%08X"), pRelTab->VirtualAddress);
		m_List_RelTab.SetItemText(m_List_RelTab.GetItemCount() - 1, 2, nTemp);

		nTemp.Format(TEXT("%08X"), pRelTab->SizeOfBlock);
		m_List_RelTab.SetItemText(m_List_RelTab.GetItemCount() - 1, 3, nTemp);

		nTemp.Format(TEXT("%d"), (pRelTab->SizeOfBlock - 8) / sizeof(WORD));
		m_List_RelTab.SetItemText(m_List_RelTab.GetItemCount() - 1, 4, nTemp);

		// 得到下一个重定位块的首地址
		pRelTab = (IMAGE_BASE_RELOCATION*)((LPBYTE)pRelTab + pRelTab->SizeOfBlock);
	}

}
//重定位偏移
VOID PE::PrintRelRva_32(ClistCtrlEx &m_List_RelRva,DWORD nIndex)
{
	m_List_RelRva.DeleteAllItems();

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nRelTabRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	if (nRelTabRva == 0)return;

	DWORD dwRelTabFoa = RvaToFoa_32(nRelTabRva) + (LONG)nData;

	IMAGE_BASE_RELOCATION* pRelTab = (IMAGE_BASE_RELOCATION*)(dwRelTabFoa);

	CString nTemp;

	for (DWORD i = 0; i < nIndex; i++)
	{
		pRelTab = (IMAGE_BASE_RELOCATION*)((LPBYTE)pRelTab + pRelTab->SizeOfBlock);
	}

	DWORD dwCount = (pRelTab->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
	TypeOffset* pTypeOffset = NULL;
	pTypeOffset = (TypeOffset*)(pRelTab + 1);

	for (DWORD i = 0; i < dwCount; ++i) {

		nTemp.Format(TEXT("%d"), m_List_RelRva.GetItemCount() + 1);
		m_List_RelRva.InsertItem(m_List_RelRva.GetItemCount(), nTemp);

		nTemp.Format(TEXT("%04X"), pTypeOffset[i].Offset);
		m_List_RelRva.SetItemText(m_List_RelRva.GetItemCount() - 1, 1, nTemp);

		nTemp.Format(TEXT("%d"), pTypeOffset[i].Type);
		m_List_RelRva.SetItemText(m_List_RelRva.GetItemCount() - 1, 2, nTemp);

		//printf("\t[%d][%04X]\n",pTypeOffset[i].Type, pTypeOffset[i].Offset);
	}
}
VOID PE::PrintRelRva_64(ClistCtrlEx &m_List_RelRva, DWORD nIndex)
{
	m_List_RelRva.DeleteAllItems();

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nRelTabRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	if (nRelTabRva == 0)return;

	DWORD dwRelTabFoa = RvaToFoa_32(nRelTabRva) + (LONG)nData;

	IMAGE_BASE_RELOCATION* pRelTab = (IMAGE_BASE_RELOCATION*)(dwRelTabFoa);

	CString nTemp;

	for (DWORD i = 0; i < nIndex; i++)
	{
		pRelTab = (IMAGE_BASE_RELOCATION*)((LPBYTE)pRelTab + pRelTab->SizeOfBlock);
	}

	DWORD dwCount = (pRelTab->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
	TypeOffset* pTypeOffset = NULL;
	pTypeOffset = (TypeOffset*)(pRelTab + 1);

	for (DWORD i = 0; i < dwCount; ++i) {

		nTemp.Format(TEXT("%d"), m_List_RelRva.GetItemCount() + 1);
		m_List_RelRva.InsertItem(m_List_RelRva.GetItemCount(), nTemp);

		nTemp.Format(TEXT("%04X"), pTypeOffset[i].Offset);
		m_List_RelRva.SetItemText(m_List_RelRva.GetItemCount() - 1, 1, nTemp);

		nTemp.Format(TEXT("%d"), pTypeOffset[i].Type);
		m_List_RelRva.SetItemText(m_List_RelRva.GetItemCount() - 1, 2, nTemp);

		//printf("\t[%d][%04X]\n",pTypeOffset[i].Type, pTypeOffset[i].Offset);
	}
}

//遍历tls表
VOID PE::GetTlsInfo_32(CEdit &m_Edit_StartAddress, CEdit &m_Edit_EndAddress, CEdit &m_Edit_AddressIndex, CEdit &m_Edit_AddressCallBack, CEdit &m_Edit_SizeOfZero, CEdit &m_Edit_Characteristics)
{
	CString nTemp;
	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nTlsHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress;
	DWORD nTlsHeadFoa = RvaToFoa_32(nTlsHeadRva) + (DWORD)nData;

	PIMAGE_TLS_DIRECTORY pTlsTab = (PIMAGE_TLS_DIRECTORY)nTlsHeadFoa;

	nTemp.Format(TEXT("%08X"), pTlsTab->StartAddressOfRawData);
	m_Edit_StartAddress.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->EndAddressOfRawData);
	m_Edit_EndAddress.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->AddressOfIndex);
	m_Edit_AddressIndex.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->AddressOfCallBacks);
	m_Edit_AddressCallBack.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->SizeOfZeroFill);
	m_Edit_SizeOfZero.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->Characteristics);
	m_Edit_Characteristics.SetWindowText(nTemp);

}
VOID PE::GetTlsInfo_64(CEdit &m_Edit_StartAddress, CEdit &m_Edit_EndAddress, CEdit &m_Edit_AddressIndex, CEdit &m_Edit_AddressCallBack, CEdit &m_Edit_SizeOfZero, CEdit &m_Edit_Characteristics)
{
	CString nTemp;
	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nTlsHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress;
	DWORD nTlsHeadFoa = RvaToFoa_64(nTlsHeadRva) + (DWORD)nData;

	PIMAGE_TLS_DIRECTORY pTlsTab = (PIMAGE_TLS_DIRECTORY)nTlsHeadFoa;

	nTemp.Format(TEXT("%08X"), pTlsTab->StartAddressOfRawData);
	m_Edit_StartAddress.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->EndAddressOfRawData);
	m_Edit_EndAddress.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->AddressOfIndex);
	m_Edit_AddressIndex.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->AddressOfCallBacks);
	m_Edit_AddressCallBack.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->SizeOfZeroFill);
	m_Edit_SizeOfZero.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), pTlsTab->Characteristics);
	m_Edit_Characteristics.SetWindowText(nTemp);

}

//遍历延迟加载
VOID PE::PrintDelayDll_32(ClistCtrlEx &m_List_Delay_Dll)
{
	m_List_Delay_Dll.DeleteAllItems();

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nDelayHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress;
	DWORD nDelayHeadFoa = RvaToFoa_32(nDelayHeadRva) + (DWORD)nData;

	PIMAGE_DELAYLOAD_DESCRIPTOR pDelayTab = (PIMAGE_DELAYLOAD_DESCRIPTOR)nDelayHeadFoa;


	while (pDelayTab->DllNameRVA != 0)
	{
		DWORD nIndexDll = m_List_Delay_Dll.GetItemCount();
		//DWORD nIndexFun = m_List_Import_Function.GetItemCount();
		DWORD ImportDllNameRva = pDelayTab->DllNameRVA;
		DWORD ImportDllNameFoa = RvaToFoa_32(ImportDllNameRva) + (DWORD)nData;
		CString nDllName((CHAR*)ImportDllNameFoa);

		CString nTemp;

		m_List_Delay_Dll.InsertItem(nIndexDll, nDllName);
		
		nTemp.Format(TEXT("%08X"), pDelayTab->ImportNameTableRVA);
		m_List_Delay_Dll.SetItemText(nIndexDll, 1, nTemp);

		nTemp.Format(TEXT("%08X"), pDelayTab->ImportAddressTableRVA);
		m_List_Delay_Dll.SetItemText(nIndexDll, 2, nTemp);

		pDelayTab++;
	}
}
VOID PE::PrintDelayDll_64(ClistCtrlEx &m_List_Delay_Dll)
{
	m_List_Delay_Dll.DeleteAllItems();

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));

	DWORD nDelayHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress;
	DWORD nDelayHeadFoa = RvaToFoa_64(nDelayHeadRva) + (DWORD)nData;

	PIMAGE_DELAYLOAD_DESCRIPTOR pDelayTab = (PIMAGE_DELAYLOAD_DESCRIPTOR)nDelayHeadFoa;


	while (pDelayTab->DllNameRVA != 0)
	{
		DWORD nIndexDll = m_List_Delay_Dll.GetItemCount();
		//DWORD nIndexFun = m_List_Import_Function.GetItemCount();
		DWORD ImportDllNameRva = pDelayTab->DllNameRVA;
		DWORD ImportDllNameFoa = RvaToFoa_64(ImportDllNameRva) + (DWORD)nData;
		CString nDllName((CHAR*)ImportDllNameFoa);

		CString nTemp;

		m_List_Delay_Dll.InsertItem(nIndexDll, nDllName);

		nTemp.Format(TEXT("%08X"), pDelayTab->ImportNameTableRVA);
		m_List_Delay_Dll.SetItemText(nIndexDll, 1, nTemp);

		nTemp.Format(TEXT("%08X"), pDelayTab->ImportAddressTableRVA);
		m_List_Delay_Dll.SetItemText(nIndexDll, 2, nTemp);

		pDelayTab++;
	}
}

//延迟加载函数
VOID PE::PrintDelayFunction_32(ClistCtrlEx &m_List_Delay_Function, DWORD nIndex)
{

	m_List_Delay_Function.DeleteAllItems();
	if (nIndex == -1)return;

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = (PIMAGE_OPTIONAL_HEADER32)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	DWORD nDelayHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress;
	DWORD nDelayHeadFoa = RvaToFoa_32(nDelayHeadRva) + (DWORD)nData;

	PIMAGE_DELAYLOAD_DESCRIPTOR nDelayHead = (PIMAGE_DELAYLOAD_DESCRIPTOR)nDelayHeadFoa;

	nDelayHead += nIndex;

	DWORD DelayIntRva = nDelayHead->ImportNameTableRVA;
	DWORD DelayIntFoa = RvaToFoa_32(DelayIntRva) + (DWORD)nData;

	PIMAGE_THUNK_DATA32 pIntTab = (PIMAGE_THUNK_DATA32)DelayIntFoa;



	while (pIntTab->u1.AddressOfData != 0)
	{
		DWORD nIndexFun = m_List_Delay_Function.GetItemCount();

		PDWORD nThunk = (PDWORD)pIntTab->u1.Function;
		CString nTemp;

		if (!IMAGE_SNAP_BY_ORDINAL(pIntTab->u1.Function))
		{
			PDWORD nIntHeadNameFoa = (PDWORD)(RvaToFoa_32((DWORD)nThunk) + (DWORD)nData);

			IMAGE_IMPORT_BY_NAME *nIntNameHead = (IMAGE_IMPORT_BY_NAME*)nIntHeadNameFoa;

			CString nIntName(nIntNameHead->Name);
			m_List_Delay_Function.InsertItem(nIndexFun, nIntName);

			nTemp.Format(TEXT("%08X"), pIntTab->u1.AddressOfData);
			m_List_Delay_Function.SetItemText(nIndexFun, 1, nTemp);

			nTemp.Format(TEXT("%04X"), nIntNameHead->Hint);
			m_List_Delay_Function.SetItemText(nIndexFun, 2, nTemp);

		}
		else
		{
			CString nIntOrdinal;
			nIntOrdinal.Format(TEXT("%d"), pIntTab->u1.Ordinal & 0xffff);
			m_List_Delay_Function.InsertItem(nIndexFun, TEXT("序号：") + nIntOrdinal);

			nTemp.Format(TEXT("%08X"), pIntTab->u1.AddressOfData);
			m_List_Delay_Function.SetItemText(nIndexFun, 1, nTemp);

			m_List_Delay_Function.SetItemText(nIndexFun, 2, TEXT("-"));

		}


		pIntTab++;

	}

}
VOID PE::PrintDelayFunction_64(ClistCtrlEx &m_List_Delay_Function, DWORD nIndex)
{

	m_List_Delay_Function.DeleteAllItems();
	if (nIndex == -1)return;

	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = (PIMAGE_OPTIONAL_HEADER64)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	DWORD nDelayHeadRva = nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress;
	DWORD nDelayHeadFoa = RvaToFoa_64(nDelayHeadRva) + (DWORD)nData;

	PIMAGE_DELAYLOAD_DESCRIPTOR nDelayHead = (PIMAGE_DELAYLOAD_DESCRIPTOR)nDelayHeadFoa;

	nDelayHead += nIndex;

	DWORD DelayIntRva = nDelayHead->ImportNameTableRVA;
	DWORD DelayIntFoa = RvaToFoa_64(DelayIntRva) + (DWORD)nData;

	PIMAGE_THUNK_DATA64 pIntTab = (PIMAGE_THUNK_DATA64)DelayIntFoa;



	while (pIntTab->u1.AddressOfData != 0)
	{
		DWORD nIndexFun = m_List_Delay_Function.GetItemCount();

		PDWORD nThunk = (PDWORD)pIntTab->u1.Function;
		CString nTemp;

		if (!IMAGE_SNAP_BY_ORDINAL(pIntTab->u1.Function))
		{
			PDWORD nIntHeadNameFoa = (PDWORD)(RvaToFoa_64((DWORD)nThunk) + (DWORD)nData);

			IMAGE_IMPORT_BY_NAME *nIntNameHead = (IMAGE_IMPORT_BY_NAME*)nIntHeadNameFoa;

			CString nIntName(nIntNameHead->Name);
			m_List_Delay_Function.InsertItem(nIndexFun, nIntName);

			nTemp.Format(TEXT("%08X"), pIntTab->u1.AddressOfData);
			m_List_Delay_Function.SetItemText(nIndexFun, 1, nTemp);

			nTemp.Format(TEXT("%04X"), nIntNameHead->Hint);
			m_List_Delay_Function.SetItemText(nIndexFun, 2, nTemp);

		}
		else
		{
			CString nIntOrdinal;
			nIntOrdinal.Format(TEXT("%d"), pIntTab->u1.Ordinal & 0xffff);
			m_List_Delay_Function.InsertItem(nIndexFun, TEXT("序号：") + nIntOrdinal);

			nTemp.Format(TEXT("%08X"), pIntTab->u1.AddressOfData);
			m_List_Delay_Function.SetItemText(nIndexFun, 1, nTemp);

			m_List_Delay_Function.SetItemText(nIndexFun, 2, TEXT("-"));

		}


		pIntTab++;

	}

}

//判断是否32位pe
BOOL PE::Is32Pe()
{
	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS nNtHead = (PIMAGE_NT_HEADERS)((LONG)nData + nDosHeade->e_lfanew);
	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	if (nFileHead->SizeOfOptionalHeader == 0xe0)return TRUE;
	return FALSE;
}
