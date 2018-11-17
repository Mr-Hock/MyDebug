#pragma once
#include "afxwin.h"

//汇编引擎头文件，静态库
#include "XEDParse/XEDParse.h"
#pragma comment (lib,"XEDParse/x86/XEDParse_x86.lib")


// EditAsmDlg 对话框

class EditAsmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditAsmDlg)

public:
	EditAsmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EditAsmDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit_Asm;
	afx_msg void OnBnClickedButton1();
	BOOL AsmToOpcaode(DWORD nAddress, CHAR *nAsm, LPBYTE nBytes, DWORD &nByteNum);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
