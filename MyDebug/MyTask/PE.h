#pragma once
#include "ClistCtrlEx.h"

class PE
{
public:
	LPBYTE *nData = nullptr;

	PE();
	~PE();

	void AddEditData(CEdit &m_Edit_Info, CString &nData);
	void AddEditData(CEdit &m_Edit_Info, CString &nData, DWORD nDword);

	BOOL SetPeData(CString nPath);

	DWORD RvaToFoa_32(DWORD nRva);
	DWORD PE::RvaToFoa_64(DWORD nRva);

	CString GetSeltion_32(DWORD nRva);
	CString PE::GetSeltion_64(DWORD nRva);

	VOID GetPeInfo_32(CEdit &m_Edit_Info);
	VOID PE::GetPeInfo_64(CEdit &m_Edit_Info);

	PIMAGE_OPTIONAL_HEADER GetOptionalHead_32();
	PIMAGE_OPTIONAL_HEADER64 GetOptionalHead_64();

	VOID PrintExport_32(ClistCtrlEx &m_List_Emport);
	VOID PE::PrintExport_64(ClistCtrlEx &m_List_Emport);

	VOID PrintImportDll_32(ClistCtrlEx &m_List_Import_Dll);
	VOID PE::PrintImportDll_64(ClistCtrlEx &m_List_Import_Dll);

	VOID PrintImportFunction_32(ClistCtrlEx &m_List_Import_Function, DWORD nIndex);
	VOID PE::PrintImportFunction_64(ClistCtrlEx &m_List_Import_Function, DWORD nIndex);

	VOID PrintResource_32(ClistCtrlEx &m_List_Resource, LPBYTE pResDirRoot, IMAGE_RESOURCE_DIRECTORY* pDir, int nLevel=1);
	VOID PE::PrintResource_64(ClistCtrlEx &m_List_Resource, LPBYTE pResDirRoot, IMAGE_RESOURCE_DIRECTORY* pDir, int nLevel=1);

	VOID PE::PrintRelPage_32(ClistCtrlEx &m_List_RelTab);
	VOID PE::PrintRelPage_64(ClistCtrlEx &m_List_RelTab);

	VOID PE::PrintRelRva_32(ClistCtrlEx &m_List_RelRva, DWORD nIndex);
	VOID PE::PrintRelRva_64(ClistCtrlEx &m_List_RelRva, DWORD nIndex);

	VOID PE::GetTlsInfo_32(CEdit &m_Edit_StartAddress, CEdit &m_Edit_EndAddress, CEdit &m_Edit_AddressIndex, CEdit &m_Edit_AddressCallBack, CEdit &m_Edit_SizeOfZero, CEdit &m_Edit_Characteristics);
	VOID PE::GetTlsInfo_64(CEdit &m_Edit_StartAddress, CEdit &m_Edit_EndAddress, CEdit &m_Edit_AddressIndex, CEdit &m_Edit_AddressCallBack, CEdit &m_Edit_SizeOfZero, CEdit &m_Edit_Characteristics);
	
	VOID PrintDelayDll_32(ClistCtrlEx &m_List_Delay_Dll);
	VOID PE::PrintDelayDll_64(ClistCtrlEx &m_List_Delay_Dll);

	VOID PrintDelayFunction_32(ClistCtrlEx &m_List_Delay_Function, DWORD nIndex);
	VOID PE::PrintDelayFunction_64(ClistCtrlEx &m_List_Delay_Function, DWORD nIndex);

	BOOL Is32Pe();



	// 1. 得到这个重定位块的分页下的所有页内偏移
	typedef struct TypeOffset {
		WORD Offset : 12;  // (1) 大小为12Bit的重定位偏移
		WORD Type : 4;    // (2) 大小为4Bit的重定位信息类型值
	}TypeOffset;	    // 这个结构体是A1Pass总结的
};

