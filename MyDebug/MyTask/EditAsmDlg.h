#pragma once
#include "afxwin.h"

//�������ͷ�ļ�����̬��
#include "XEDParse/XEDParse.h"
#pragma comment (lib,"XEDParse/x86/XEDParse_x86.lib")


// EditAsmDlg �Ի���

class EditAsmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditAsmDlg)

public:
	EditAsmDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EditAsmDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit_Asm;
	afx_msg void OnBnClickedButton1();
	BOOL AsmToOpcaode(DWORD nAddress, CHAR *nAsm, LPBYTE nBytes, DWORD &nByteNum);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
