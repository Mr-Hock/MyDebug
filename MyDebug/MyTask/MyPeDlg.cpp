// MyPeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyTask.h"
#include "MyPeDlg.h"
#include "MyExportTabDlg.h"
#include "MyImportTabDlg.h"
#include "MyResourceDlg.h"
#include "MyRelTabDlg.h"
#include "MyTlsTabDlg.h"
#include "MyDelayDlg.h"

#include "afxdialogex.h"
#include "PE.h"

PE MyPe;

// MyPeDlg 对话框

IMPLEMENT_DYNAMIC(MyPeDlg, CDialogEx)

MyPeDlg::MyPeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PEINFO_DIALOG, pParent)
{
	Create(IDD_PEINFO_DIALOG);

	MyPe.AddEditData(m_Edit_Info, CString("================================================================"));
	MyPe.AddEditData(m_Edit_Info, CString("提示：请把要查看的文件拖拽到该窗口中!"));
	MyPe.AddEditData(m_Edit_Info, CString("================================================================"));

}

MyPeDlg::~MyPeDlg()
{
}

void MyPeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Info);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_Export_Addr);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_Export_Size);
	DDX_Control(pDX, IDC_EDIT4, m_Edit_Import_Addr);
	DDX_Control(pDX, IDC_EDIT5, m_Edit_Import_Size);
	DDX_Control(pDX, IDC_EDIT6, m_Edit_Resource_Addr);
	DDX_Control(pDX, IDC_EDIT7, m_Edit_Resource_Size);
	DDX_Control(pDX, IDC_EDIT8, m_Edit_Exception_Addr);
	DDX_Control(pDX, IDC_EDIT9, m_Edit_Exception_Size);
	DDX_Control(pDX, IDC_EDIT10, m_Edit_Security_Addr);
	DDX_Control(pDX, IDC_EDIT11, m_Edit_Security_Size);
	DDX_Control(pDX, IDC_EDIT12, m_Edit_Relocation_Addr);
	DDX_Control(pDX, IDC_EDIT13, m_Edit_Relocation_Size);
	DDX_Control(pDX, IDC_EDIT14, m_Edit_Debug_Addr);
	DDX_Control(pDX, IDC_EDIT15, m_Edit_Debug_Size);
	DDX_Control(pDX, IDC_EDIT16, m_Edit_Architecture_Addr);
	DDX_Control(pDX, IDC_EDIT17, m_Edit_Architecture_Size);
	DDX_Control(pDX, IDC_EDIT18, m_Edit_Globalptr_Addr);
	DDX_Control(pDX, IDC_EDIT19, m_Edit_Globalptr_Size);
	DDX_Control(pDX, IDC_EDIT20, m_Edit_TLS_Addr);
	DDX_Control(pDX, IDC_EDIT21, m_Edit_TLS_Size);
	DDX_Control(pDX, IDC_EDIT22, m_Edit_Load_Addr);
	DDX_Control(pDX, IDC_EDIT23, m_Edit_Load_Size);
	DDX_Control(pDX, IDC_EDIT24, m_Edit_Bound_Addr);
	DDX_Control(pDX, IDC_EDIT25, m_Edit_Bound_Size);
	DDX_Control(pDX, IDC_EDIT26, m_Edit_IAT_Addr);
	DDX_Control(pDX, IDC_EDIT27, m_Edit_IAT_Size);
	DDX_Control(pDX, IDC_EDIT28, m_Edit_Delay_Addr);
	DDX_Control(pDX, IDC_EDIT29, m_Edit_Delay_Size);
	DDX_Control(pDX, IDC_EDIT30, m_Edit_COM_Addr);
	DDX_Control(pDX, IDC_EDIT31, m_Edit_COM_Size);
	DDX_Control(pDX, IDC_EDIT32, m_Edit_Save_Addr);
	DDX_Control(pDX, IDC_EDIT33, m_Edit_Save_Size);
}


BEGIN_MESSAGE_MAP(MyPeDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &MyPeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MyPeDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &MyPeDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &MyPeDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &MyPeDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &MyPeDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// MyPeDlg 消息处理程序


void MyPeDlg::PrintPeTable_32()
{
	CString nTemp;
	PIMAGE_OPTIONAL_HEADER32 nOptionalHead = nullptr;
	nOptionalHead = MyPe.GetOptionalHead_32();

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	m_Edit_Export_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);
	m_Edit_Export_Size.SetWindowText(nTemp);


	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	m_Edit_Import_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
	m_Edit_Import_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	m_Edit_Resource_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);
	m_Edit_Resource_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);
	m_Edit_Exception_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size);
	m_Edit_Exception_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);
	m_Edit_Security_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
	m_Edit_Security_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	m_Edit_Relocation_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
	m_Edit_Relocation_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
	m_Edit_Debug_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);
	m_Edit_Debug_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress);
	m_Edit_Architecture_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size);
	m_Edit_Architecture_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress);
	m_Edit_Globalptr_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size);
	m_Edit_Globalptr_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
	m_Edit_TLS_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size);
	m_Edit_TLS_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
	m_Edit_Load_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size);
	m_Edit_Load_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress);
	m_Edit_Bound_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);
	m_Edit_Bound_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	m_Edit_IAT_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);
	m_Edit_IAT_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
	m_Edit_Delay_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size);
	m_Edit_Delay_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress);
	m_Edit_COM_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size);
	m_Edit_COM_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[15].VirtualAddress);
	m_Edit_Save_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[15].Size);
	m_Edit_Save_Size.SetWindowText(nTemp);
}
void MyPeDlg::PrintPeTable_64()
{
	CString nTemp;
	PIMAGE_OPTIONAL_HEADER64 nOptionalHead = nullptr;
	nOptionalHead = MyPe.GetOptionalHead_64();

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	m_Edit_Export_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);
	m_Edit_Export_Size.SetWindowText(nTemp);


	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	m_Edit_Import_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
	m_Edit_Import_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	m_Edit_Resource_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);
	m_Edit_Resource_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress);
	m_Edit_Exception_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size);
	m_Edit_Exception_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress);
	m_Edit_Security_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
	m_Edit_Security_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	m_Edit_Relocation_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
	m_Edit_Relocation_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
	m_Edit_Debug_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);
	m_Edit_Debug_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress);
	m_Edit_Architecture_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size);
	m_Edit_Architecture_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress);
	m_Edit_Globalptr_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size);
	m_Edit_Globalptr_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
	m_Edit_TLS_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size);
	m_Edit_TLS_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
	m_Edit_Load_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size);
	m_Edit_Load_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress);
	m_Edit_Bound_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);
	m_Edit_Bound_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	m_Edit_IAT_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);
	m_Edit_IAT_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress);
	m_Edit_Delay_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size);
	m_Edit_Delay_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress);
	m_Edit_COM_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size);
	m_Edit_COM_Size.SetWindowText(nTemp);

	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[15].VirtualAddress);
	m_Edit_Save_Addr.SetWindowText(nTemp);
	nTemp.Format(TEXT("%08X"), nOptionalHead->DataDirectory[15].Size);
	m_Edit_Save_Size.SetWindowText(nTemp);
}

void MyPeDlg::OnDropFiles(HDROP hDropInfo)
{
	CDialogEx::OnDropFiles(hDropInfo);
	CString buff;
	buff.GetBufferSetLength(MAX_PATH);
	DragQueryFile(hDropInfo, 0, (TCHAR*)(const TCHAR*)buff, MAX_PATH);
	DragFinish(hDropInfo);

	if (MyPe.nData != nullptr)delete[]MyPe.nData;
	MyPe.nData = nullptr;

	m_Edit_Info.SetWindowText(TEXT(""));

	if (!MyPe.SetPeData(buff))
	{
		MyPe.AddEditData(m_Edit_Info, CString("这不是一个PE文件！"));
		return;
	}

	MyPe.AddEditData(m_Edit_Info, buff);
	MyPe.AddEditData(m_Edit_Info, CString("\n"));

	if (MyPe.Is32Pe())
	{
		MyPe.GetPeInfo_32(m_Edit_Info);
		PrintPeTable_32();
	}
	else 
	{ 
		MyPe.GetPeInfo_64(m_Edit_Info); 
		PrintPeTable_64();
	}

	

}


void MyPeDlg::OnBnClickedButton1()
{
	CString nTemp;
	m_Edit_Export_Size.GetWindowText(nTemp);
	if (nTemp == TEXT("00000000")) 
	{ 
		MessageBox(TEXT("不存在导出表！"), TEXT("提示"), MB_ICONWARNING);
		return; 
	}
	MyExportTabDlg nMyExportTabDlg;
	nMyExportTabDlg.DoModal();
}


void MyPeDlg::OnBnClickedButton2()
{
	CString nTemp;
	m_Edit_Import_Size.GetWindowText(nTemp);
	if (nTemp == TEXT("00000000"))
	{
		MessageBox(TEXT("不存在导入表！"), TEXT("提示"), MB_ICONWARNING);
		return;
	}
	MyImportTabDlg nMyImportTabDlg;
	nMyImportTabDlg.DoModal();
}


void MyPeDlg::OnBnClickedButton3()
{
	CString nTemp;
	m_Edit_Resource_Size.GetWindowText(nTemp);
	if (nTemp == TEXT("00000000"))
	{
		MessageBox(TEXT("不存在资源表！"), TEXT("提示"), MB_ICONWARNING);
		return;
	}
	MyResourceDlg nMyResourceDlg;
	nMyResourceDlg.DoModal();
}


void MyPeDlg::OnBnClickedButton4()
{
	CString nTemp;
	m_Edit_Relocation_Size.GetWindowText(nTemp);
	if (nTemp == TEXT("00000000"))
	{
		MessageBox(TEXT("不存在重定位表！"), TEXT("提示"), MB_ICONWARNING);
		return;
	}
	MyRelTabDlg nMyRelTabDlg;
	nMyRelTabDlg.DoModal();
}


void MyPeDlg::OnBnClickedButton5()
{
	CString nTemp;
	m_Edit_TLS_Size.GetWindowText(nTemp);
	if (nTemp == TEXT("00000000"))
	{
		MessageBox(TEXT("不存在TLS表！"), TEXT("提示"), MB_ICONWARNING);
		return;
	}
	MyTlsTabDlg nMyTlsTabDlg;
	nMyTlsTabDlg.DoModal();
}


void MyPeDlg::OnBnClickedButton6()
{
	CString nTemp;
	m_Edit_Delay_Size.GetWindowText(nTemp);
	if (nTemp == TEXT("00000000"))
	{
		MessageBox(TEXT("不存在延迟加载表！"), TEXT("提示"), MB_ICONWARNING);
		return;
	}
	MyDelayDlg nMyDelayDlg;
	nMyDelayDlg.DoModal();

}
