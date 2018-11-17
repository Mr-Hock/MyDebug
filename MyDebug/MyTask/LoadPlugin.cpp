#include "stdafx.h"
#include "LoadPlugin.h"


LoadPlugin::LoadPlugin()
{
	//��ʼ��������LoadPluginFile����Ŀ¼�µ������ļ�����������
	LoadPluginFile();
}


LoadPlugin::~LoadPlugin()
{
	//��������
	//�˳�֮ǰ������������Ĳ��ж�أ��ͷ���Դ
	for (DWORD i =0;i<m_Handle.size();i++)
	{
		FreeLibrary((HMODULE)m_Handle[i]);
	}
	//������б����ڶ�̬��������ĺ���ָ��
	m_PluginName.swap(vector <CString>());
	m_Handle.swap(vector <DWORD>());
	m_InitPlugin_Address.swap(vector <DWORD>());
	m_CreateProcessEvent_Address.swap(vector <DWORD>());
	m_CreateThreadEvent_Address.swap(vector <DWORD>());


}

VOID LoadPlugin::LoadPluginFile()
{
	//����Ŀ¼���ڳ���Ŀ¼�µ�Plugin�ļ���
	CString nPluginPath(GetProgramDir() + TEXT("Plugin"));
	WIN32_FIND_DATA fData = { 0 };
	HANDLE hFind = INVALID_HANDLE_VALUE;
	HMODULE nHandle = nullptr;
	DWORD nAddress = 0;


	//��ʼ����Ŀ¼�µ������ļ�
	hFind = FindFirstFile(nPluginPath + L"\\*", &fData);
	if (hFind == INVALID_HANDLE_VALUE)return;

	do {
		//��������һ���ļ��У�������
		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {}

		//�������һ���ļ��������
		else
		{
			//��ȡ���ļ��ĺ�׺�����������dll������
			if (GetFileLastName(fData.cFileName) != TEXT("dll"))continue;

			//����LoadLibrary�����ļ�
			nHandle = LoadLibrary(nPluginPath + TEXT("\\") + fData.cFileName);

			//����GetProcAddressȡ��ָ���Ĳ����ʼ��������ַ
			nAddress = (DWORD)GetProcAddress(nHandle, CStringA("InitPlugin"));

			//���������ַ��Ϊ0������ڴ˺���
			if (nAddress)
			{
				//���ò���ļ��ؾ�����浽��̬������
				m_Handle.push_back((DWORD)nHandle);
				//����ʼ��������ַ�����ڶ�̬������
				m_InitPlugin_Address.push_back((DWORD)nAddress);
				//����ʼ��������ַǿ��ת��λ����ָ��
				InitPlugin = (pInitPlugin)nAddress;
				//ͨ������ָ����ó�ʼ�����������ѵ��õķ��ؽ�����浽��̬�����ڣ����ص��ǲ��������
				m_PluginName.push_back(CString(InitPlugin()));

				//ȡ��ָ���ӿ�CreateProcessEvent�ĺ�����ַ
				nAddress = (DWORD)GetProcAddress(nHandle, CStringA("CreateProcessEvent"));
				//���������ַ��Ϊ0������ڴ˺�������������ַ���浽��̬������
				if (nAddress)m_CreateProcessEvent_Address.push_back((DWORD)nAddress);

				//ȡ��ָ���ӿ�CreateThreadEvent�ĺ�����ַ
				nAddress = (DWORD)GetProcAddress(nHandle, CStringA("CreateThreadEvent"));
				//���������ַ��Ϊ0������ڴ˺�������������ַ���浽��̬������
				if (nAddress)m_CreateThreadEvent_Address.push_back((DWORD)nAddress);
			}

			//���������ַΪ0�����ͷŸ��ļ�
			else
			{
				FreeLibrary(nHandle);
				nHandle = nullptr;
			}

		}
		//����������һ���ļ�
	} while (FindNextFile(hFind, &fData));
}



//************************************
// ����ȫ������ĺ���
//************************************
VOID LoadPlugin::CallCreateProcessEvent(DWORD m_Pid, DWORD m_Tid)
{
	//��������CreateProcessEvent�����ĵ�ַ
	for (DWORD i = 0; i < m_CreateProcessEvent_Address.size(); i++)
	{
		//ǿ��ת��Ϊ����ָ��
		CreateProcessEvent = (pCreateProcessEvent)m_CreateProcessEvent_Address[i];
		//ͨ������ָ����øú���
		CreateProcessEvent(m_Pid, m_Tid);
	}
}

VOID LoadPlugin::CallCreateThreadEvent(DWORD nHandle, DWORD nThreadLocalBase)
{
	//��������CreateThreadEvent�����ĵ�ַ
	for (DWORD i = 0; i < m_CreateThreadEvent_Address.size(); i++)
	{
		//ǿ��ת��Ϊ����ָ��
		CreateThreadEvent = (pCreateThreadEvent)m_CreateThreadEvent_Address[i];
		//ͨ������ָ����øú���
		CreateThreadEvent(nHandle, nThreadLocalBase);
	}
}
