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

//��ȡPE�ļ����ڴ�
BOOL PE::SetPeData(CString nPath)
{
	HANDLE  pFile = nullptr;

	pFile = CreateFile(nPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (pFile == (HANDLE)-1)
	{
		MessageBox(NULL,TEXT("���ļ�ʧ�ܣ�"), TEXT("ʧ��"), MB_ICONWARNING);
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

//ת��
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

//ȡ��������
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

//ȡ��PE���ļ�ͷ����չͷ��Ϣ
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
	nFileTimeFomat.Format(TEXT("%d��%d��%d�� %dʱ%d��%d��"),
		nFileTime->tm_year + 1900, nFileTime->tm_mon + 1, nFileTime->tm_mday,
		nFileTime->tm_hour, nFileTime->tm_min, nFileTime->tm_sec);

	CString nPeData;
	CString nTempData;

	AddEditData(m_Edit_Info, CString("�ļ�ͷ======================================="));
	AddEditData(m_Edit_Info, CString("�ļ�����ƽ̨��"), nFileHead->Machine);

	AddEditData(m_Edit_Info, CString("����������"), nFileHead->NumberOfSections);
	AddEditData(m_Edit_Info, CString("�ļ�����ʱ�䣺") + nFileTimeFomat);
	AddEditData(m_Edit_Info, CString("��չͷ��С��"), nFileHead->SizeOfOptionalHeader);
	AddEditData(m_Edit_Info, CString("PE�ļ����ԣ�"), nFileHead->Characteristics);

	AddEditData(m_Edit_Info, CString("\n"));
	AddEditData(m_Edit_Info, CString("��ѡ�ļ�ͷ======================================="));

	AddEditData(m_Edit_Info, CString("PE�ļ����ͣ�") + CString(nOptionalHead->Magic == 0x010b ? CString("32λ") : CString("64λ")));
	AddEditData(m_Edit_Info, CString("���������ܴ�С��"), nOptionalHead->SizeOfCode);
	AddEditData(m_Edit_Info, CString("�ѳ�ʼ�������ܴ�С��"), nOptionalHead->SizeOfInitializedData);
	AddEditData(m_Edit_Info, CString("δ��ʼ�������ܴ�С��"), nOptionalHead->SizeOfUninitializedData);
	AddEditData(m_Edit_Info, CString("OEP��"), nOptionalHead->AddressOfEntryPoint);
	AddEditData(m_Edit_Info, CString("��ʼ������������ַRVA��"), nOptionalHead->BaseOfCode);
	AddEditData(m_Edit_Info, CString("��ʼ������������ַRVA��"), nOptionalHead->BaseOfData);
	AddEditData(m_Edit_Info, CString("Ĭ�ϼ��ػ�ַ��"), nOptionalHead->ImageBase);
	AddEditData(m_Edit_Info, CString("�ڴ��������"), nOptionalHead->SectionAlignment);
	AddEditData(m_Edit_Info, CString("�ļ���������"), nOptionalHead->FileAlignment);
	AddEditData(m_Edit_Info, CString("���������ڴ�(�ڴ�����)��"), nOptionalHead->SizeOfImage);
	AddEditData(m_Edit_Info, CString("����ͷ����С��"), nOptionalHead->SizeOfHeaders);
	AddEditData(m_Edit_Info, CString("ָʹDLL��־��"), nOptionalHead->DllCharacteristics);

	AddEditData(m_Edit_Info, CString("����Ŀ¼������"), nOptionalHead->NumberOfRvaAndSizes);



	AddEditData(m_Edit_Info, CString("\n"));
	AddEditData(m_Edit_Info, CString("���α�======================================="));
	for (INT i = 0; i < nFileHead->NumberOfSections; i++)
	{
		AddEditData(m_Edit_Info, CString("���α�") + CString(nSectionHead->Name));
		AddEditData(m_Edit_Info, CString("���δ�С��"), nSectionHead->Misc.VirtualSize);
		AddEditData(m_Edit_Info, CString("������ʼ��ַRVA��"), nSectionHead->VirtualAddress);
		AddEditData(m_Edit_Info, CString("�������ļ��д�С��"), nSectionHead->SizeOfRawData);
		AddEditData(m_Edit_Info, CString("�����ļ�ƫ�ƣ�"), nSectionHead->PointerToRawData);
		AddEditData(m_Edit_Info, CString("�������ԣ�"), nSectionHead->Characteristics);
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
	nFileTimeFomat.Format(TEXT("%d��%d��%d�� %dʱ%d��%d��"),
		nFileTime->tm_year + 1900, nFileTime->tm_mon + 1, nFileTime->tm_mday,
		nFileTime->tm_hour, nFileTime->tm_min, nFileTime->tm_sec);

	CString nPeData;
	CString nTempData;

	AddEditData(m_Edit_Info, CString("�ļ�ͷ======================================="));
	AddEditData(m_Edit_Info, CString("�ļ�����ƽ̨��"), nFileHead->Machine);

	AddEditData(m_Edit_Info, CString("����������"), nFileHead->NumberOfSections);
	AddEditData(m_Edit_Info, CString("�ļ�����ʱ�䣺") + nFileTimeFomat);
	AddEditData(m_Edit_Info, CString("��չͷ��С��"), nFileHead->SizeOfOptionalHeader);
	AddEditData(m_Edit_Info, CString("PE�ļ����ԣ�"), nFileHead->Characteristics);

	AddEditData(m_Edit_Info, CString("\n"));
	AddEditData(m_Edit_Info, CString("��ѡ�ļ�ͷ======================================="));

	AddEditData(m_Edit_Info, CString("PE�ļ����ͣ�") + CString(nOptionalHead->Magic == 0x010b ? CString("32λ") : CString("64λ")));
	AddEditData(m_Edit_Info, CString("���������ܴ�С��"), nOptionalHead->SizeOfCode);
	AddEditData(m_Edit_Info, CString("�ѳ�ʼ�������ܴ�С��"), nOptionalHead->SizeOfInitializedData);
	AddEditData(m_Edit_Info, CString("δ��ʼ�������ܴ�С��"), nOptionalHead->SizeOfUninitializedData);
	AddEditData(m_Edit_Info, CString("OEP��"), nOptionalHead->AddressOfEntryPoint);
	AddEditData(m_Edit_Info, CString("��ʼ������������ַRVA��"), nOptionalHead->BaseOfCode);
	AddEditData(m_Edit_Info, CString("��ʼ������������ַRVA��"), (DWORD)nOptionalHead->ImageBase);
	AddEditData(m_Edit_Info, CString("Ĭ�ϼ��ػ�ַ��"), (DWORD)nOptionalHead->ImageBase);
	AddEditData(m_Edit_Info, CString("�ڴ��������"), nOptionalHead->SectionAlignment);
	AddEditData(m_Edit_Info, CString("�ļ���������"), nOptionalHead->FileAlignment);
	AddEditData(m_Edit_Info, CString("���������ڴ�(�ڴ�����)��"), nOptionalHead->SizeOfImage);
	AddEditData(m_Edit_Info, CString("����ͷ����С��"), nOptionalHead->SizeOfHeaders);
	AddEditData(m_Edit_Info, CString("ָʹDLL��־��"), nOptionalHead->DllCharacteristics);

	AddEditData(m_Edit_Info, CString("����Ŀ¼������"), nOptionalHead->NumberOfRvaAndSizes);



	AddEditData(m_Edit_Info, CString("\n"));
	AddEditData(m_Edit_Info, CString("���α�======================================="));
	for (INT i = 0; i < nFileHead->NumberOfSections; i++)
	{
		AddEditData(m_Edit_Info, CString("���α�") + CString(nSectionHead->Name));
		AddEditData(m_Edit_Info, CString("���δ�С��"), nSectionHead->Misc.VirtualSize);
		AddEditData(m_Edit_Info, CString("������ʼ��ַRVA��"), nSectionHead->VirtualAddress);
		AddEditData(m_Edit_Info, CString("�������ļ��д�С��"), nSectionHead->SizeOfRawData);
		AddEditData(m_Edit_Info, CString("�����ļ�ƫ�ƣ�"), nSectionHead->PointerToRawData);
		AddEditData(m_Edit_Info, CString("�������ԣ�"), nSectionHead->Characteristics);
		AddEditData(m_Edit_Info, CString("\n"));

		nSectionHead++;
	}
}

//ȡ����չͷָ��
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

//����������
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

//���������
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
//�������
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
			m_List_Import_Function.InsertItem(nIndexFun, TEXT("��ţ�") + nIntOrdinal);

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
			m_List_Import_Function.InsertItem(nIndexFun, TEXT("��ţ�") + nIntOrdinal);

			nTemp.Format(TEXT("%08X"), nIntHead->u1.AddressOfData);
			m_List_Import_Function.SetItemText(nIndexFun, 1, nTemp);

			m_List_Import_Function.SetItemText(nIndexFun, 2, TEXT("-"));

		}


		nIntHead++;

	}

}

//������Դ��
VOID PE::PrintResource_32(ClistCtrlEx &m_List_Resource,LPBYTE pResDirRoot, IMAGE_RESOURCE_DIRECTORY* pDir, int nLevel)
{

	CString nTemp;
	// �ҵ���2��Ŀ¼���׵�ַ, Ŀ¼��λ���Ǳ����ڵ�һ��Ŀ¼���
	// �е�ƫ����Ϣ�����.
	// ����ȵõ���һ���Ŀ¼���

	// �õ���һ��Ŀ¼��Ŀ¼��ڵ��ܸ���
	DWORD dwEntryCount = pDir->NumberOfIdEntries + pDir->NumberOfNamedEntries;

	// ��ȡ����һ��Ŀ¼��Ŀ¼���������׵�ַ
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry = NULL;
	pEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir + 1);

	if (nLevel == 1) {
		for (DWORD i = 0; i < dwEntryCount; ++i) {

			// ��һ���Ŀ¼����б�����������Ϣ:
			// 1. id(��Դ���͵�id),������,һ������id,�����ַ���id
			// ������Դ������
			if (pEntry[i].NameIsString) {/*id��һ���ַ���*/
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResDirRoot);
				// ��ӡ��Դ���͵�����
				//wprintf(L"[%s]\n", pIdStr->NameString);

				WCHAR szStr[MAX_PATH] = { 0 };
				memcpy_s(szStr, MAX_PATH, pIdStr->NameString, pIdStr->Length * sizeof(WCHAR));
				nTemp.Format(TEXT("[����:%s]"), szStr);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);

			}
			else { /*id��һ������ֵ.*/
				char* pIdName[] =
				{
					"",
					"���ָ�루Cursor��",
					"λͼ��Bitmap��",
					"ͼ�꣨Icon��",
					"�˵���Menu��",
					"�Ի���Dialog��",
					"�ַ����б�String Table��",
					"����Ŀ¼��Font Directory��",
					"���壨Font��",
					"��ݼ���Accelerators��",
					"�Ǹ�ʽ����Դ��Unformatted��",
					"��Ϣ�б�Message Table��",
					"���ָ���飨Group Cursor��",
					"",
					"ͼ���飨Group Icon��",
					"",
					"�汾��Ϣ��Version Information��"
				};
				if (pEntry[i].Id >= _countof(pIdName)) {
					//printf("[%d]\n", pEntry[i].Id);
					nTemp.Format(TEXT("[δ֪����:%d]"), pEntry[i].Id);
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
				}
				else {
					CString nUincode(pIdName[pEntry[i].Id]);
					nTemp.Format(TEXT("[����:%s]"), nUincode);
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
					//printf("[%s]\n", pIdName[pEntry[i].Id]);
				}
			}

			// 2. ƫ��(����Դ��Ŀ¼�׵�ַΪƫ��)
			IMAGE_RESOURCE_DIRECTORY* p2Dir = NULL;
			p2Dir = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResDirRoot);
			PrintResource_32(m_List_Resource,pResDirRoot, p2Dir, 2);
		}
	}
	else if (nLevel == 2) {
		for (DWORD i = 0; i < dwEntryCount; i++)
		{
			// �����ڶ����Ŀ¼���;
			// 1. id(��Դ��id)(���ַ���,������)
			if (pEntry[i].NameIsString) {/*id��һ���ַ���*/
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResDirRoot);
				// ��ӡ��Դ���͵�����
				//wprintf(L"\t[%s]\n", pIdStr->NameString);

				WCHAR szStr[MAX_PATH] = { 0 };
				memcpy_s(szStr, MAX_PATH, pIdStr->NameString, pIdStr->Length * sizeof(WCHAR));
				nTemp.Format(TEXT("��ԴID:%s"), szStr);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
			}
			else {
				nTemp.Format(TEXT("��ԴID:%04d"), (DWORD)pEntry[i].Id);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
				//printf("\t%04d\n", (DWORD)pEntry[i].Id);
			}


			// 2. ƫ��(����Դ��Ŀ¼ƫ��, ƫ�Ƶ�������)
			IMAGE_RESOURCE_DIRECTORY* p3Dir = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResDirRoot);
			// �ҵ���3��Ŀ¼���׵�ַ
			PrintResource_32(m_List_Resource,pResDirRoot, p3Dir, 3);
		}

	}
	else if (nLevel == 3) {
		for (DWORD i = 0; i < dwEntryCount; ++i) {
			// ����������
			// 1. id(û��ʲô��)
			// 2. ƫ��(����Դ��Ŀ¼Ϊƫ��, ƫ�Ƶ��������)
			IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)
				(pEntry[i].OffsetToData + pResDirRoot);
			//m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);

			nTemp.Format(TEXT("��С:%08X"), pDataEntry->Size);
			m_List_Resource.SetItemText(m_List_Resource.GetItemCount() - 1, 1, nTemp);

			nTemp.Format(TEXT("λ��:%08X"),pDataEntry->OffsetToData);
			m_List_Resource.SetItemText(m_List_Resource.GetItemCount() - 1, 2, nTemp);

			//printf("\t\t��С:%08X λ��:%08X\n",pDataEntry->Size, pDataEntry->OffsetToData);
		}
	}

}
VOID PE::PrintResource_64(ClistCtrlEx &m_List_Resource, LPBYTE pResDirRoot, IMAGE_RESOURCE_DIRECTORY* pDir, int nLevel)
{

	CString nTemp;
	// �ҵ���2��Ŀ¼���׵�ַ, Ŀ¼��λ���Ǳ����ڵ�һ��Ŀ¼���
	// �е�ƫ����Ϣ�����.
	// ����ȵõ���һ���Ŀ¼���

	// �õ���һ��Ŀ¼��Ŀ¼��ڵ��ܸ���
	DWORD dwEntryCount = pDir->NumberOfIdEntries + pDir->NumberOfNamedEntries;

	// ��ȡ����һ��Ŀ¼��Ŀ¼���������׵�ַ
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry = NULL;
	pEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir + 1);

	if (nLevel == 1) {
		for (DWORD i = 0; i < dwEntryCount; ++i) {

			// ��һ���Ŀ¼����б�����������Ϣ:
			// 1. id(��Դ���͵�id),������,һ������id,�����ַ���id
			// ������Դ������
			if (pEntry[i].NameIsString) {/*id��һ���ַ���*/
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResDirRoot);
				// ��ӡ��Դ���͵�����
				//wprintf(L"[%s]\n", pIdStr->NameString);

				WCHAR szStr[MAX_PATH] = { 0 };
				memcpy_s(szStr, MAX_PATH, pIdStr->NameString, pIdStr->Length * sizeof(WCHAR));
				nTemp.Format(TEXT("[����:%s]"), szStr);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);

			}
			else { /*id��һ������ֵ.*/
				char* pIdName[] =
				{
					"",
					"���ָ�루Cursor��",
					"λͼ��Bitmap��",
					"ͼ�꣨Icon��",
					"�˵���Menu��",
					"�Ի���Dialog��",
					"�ַ����б�String Table��",
					"����Ŀ¼��Font Directory��",
					"���壨Font��",
					"��ݼ���Accelerators��",
					"�Ǹ�ʽ����Դ��Unformatted��",
					"��Ϣ�б�Message Table��",
					"���ָ���飨Group Cursor��",
					"",
					"ͼ���飨Group Icon��",
					"",
					"�汾��Ϣ��Version Information��"
				};
				if (pEntry[i].Id >= _countof(pIdName)) {
					//printf("[%d]\n", pEntry[i].Id);
					nTemp.Format(TEXT("[δ֪����:%d]"), pEntry[i].Id);
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
				}
				else {
					CString nUincode(pIdName[pEntry[i].Id]);
					nTemp.Format(TEXT("[����:%s]"), nUincode);
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), TEXT(""));
					m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
					//printf("[%s]\n", pIdName[pEntry[i].Id]);
				}
			}

			// 2. ƫ��(����Դ��Ŀ¼�׵�ַΪƫ��)
			IMAGE_RESOURCE_DIRECTORY* p2Dir = NULL;
			p2Dir = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResDirRoot);
			PrintResource_64(m_List_Resource, pResDirRoot, p2Dir, 2);
		}
	}
	else if (nLevel == 2) {
		for (DWORD i = 0; i < dwEntryCount; i++)
		{
			// �����ڶ����Ŀ¼���;
			// 1. id(��Դ��id)(���ַ���,������)
			if (pEntry[i].NameIsString) {/*id��һ���ַ���*/
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResDirRoot);
				// ��ӡ��Դ���͵�����
				//wprintf(L"\t[%s]\n", pIdStr->NameString);

				WCHAR szStr[MAX_PATH] = { 0 };
				memcpy_s(szStr, MAX_PATH, pIdStr->NameString, pIdStr->Length * sizeof(WCHAR));
				nTemp.Format(TEXT("��ԴID:%s"), szStr);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
			}
			else {
				nTemp.Format(TEXT("��ԴID:%04d"), (DWORD)pEntry[i].Id);
				m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);
				//printf("\t%04d\n", (DWORD)pEntry[i].Id);
			}


			// 2. ƫ��(����Դ��Ŀ¼ƫ��, ƫ�Ƶ�������)
			IMAGE_RESOURCE_DIRECTORY* p3Dir = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResDirRoot);
			// �ҵ���3��Ŀ¼���׵�ַ
			PrintResource_64(m_List_Resource, pResDirRoot, p3Dir, 3);
		}

	}
	else if (nLevel == 3) {
		for (DWORD i = 0; i < dwEntryCount; ++i) {
			// ����������
			// 1. id(û��ʲô��)
			// 2. ƫ��(����Դ��Ŀ¼Ϊƫ��, ƫ�Ƶ��������)
			IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)
				(pEntry[i].OffsetToData + pResDirRoot);
			//m_List_Resource.InsertItem(m_List_Resource.GetItemCount(), nTemp);

			nTemp.Format(TEXT("��С:%08X"), pDataEntry->Size);
			m_List_Resource.SetItemText(m_List_Resource.GetItemCount() - 1, 1, nTemp);

			nTemp.Format(TEXT("λ��:%08X"), pDataEntry->OffsetToData);
			m_List_Resource.SetItemText(m_List_Resource.GetItemCount() - 1, 2, nTemp);

			//printf("\t\t��С:%08X λ��:%08X\n",pDataEntry->Size, pDataEntry->OffsetToData);
		}
	}

}

//�����ض�λ��
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
		// SizeOfBlock������ǰ����ṹ���Typeoffset��������ֽ���
		// Ҫ����Typeoffset����ĸ���ʱ��,�ȼ�ȥ�ṹ��Ĵ�С
		// �õ�Typeoffset������ֽ���, ����2�͵õ�����
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

		// �õ���һ���ض�λ����׵�ַ
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
		// SizeOfBlock������ǰ����ṹ���Typeoffset��������ֽ���
		// Ҫ����Typeoffset����ĸ���ʱ��,�ȼ�ȥ�ṹ��Ĵ�С
		// �õ�Typeoffset������ֽ���, ����2�͵õ�����
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

		// �õ���һ���ض�λ����׵�ַ
		pRelTab = (IMAGE_BASE_RELOCATION*)((LPBYTE)pRelTab + pRelTab->SizeOfBlock);
	}

}
//�ض�λƫ��
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

//����tls��
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

//�����ӳټ���
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

//�ӳټ��غ���
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
			m_List_Delay_Function.InsertItem(nIndexFun, TEXT("��ţ�") + nIntOrdinal);

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
			m_List_Delay_Function.InsertItem(nIndexFun, TEXT("��ţ�") + nIntOrdinal);

			nTemp.Format(TEXT("%08X"), pIntTab->u1.AddressOfData);
			m_List_Delay_Function.SetItemText(nIndexFun, 1, nTemp);

			m_List_Delay_Function.SetItemText(nIndexFun, 2, TEXT("-"));

		}


		pIntTab++;

	}

}

//�ж��Ƿ�32λpe
BOOL PE::Is32Pe()
{
	PIMAGE_DOS_HEADER nDosHeade = (PIMAGE_DOS_HEADER)nData;
	PIMAGE_NT_HEADERS nNtHead = (PIMAGE_NT_HEADERS)((LONG)nData + nDosHeade->e_lfanew);
	PIMAGE_FILE_HEADER nFileHead = (PIMAGE_FILE_HEADER)((LONG)nData + nDosHeade->e_lfanew + sizeof(DWORD));
	if (nFileHead->SizeOfOptionalHeader == 0xe0)return TRUE;
	return FALSE;
}
