#include "stdafx.h"
#include "LoadPlugin.h"


LoadPlugin::LoadPlugin()
{
	//初始化，调用LoadPluginFile遍历目录下的所有文件，并载入插件
	LoadPluginFile();
}


LoadPlugin::~LoadPlugin()
{
	//析构函数
	//退出之前，将所有载入的插件卸载，释放资源
	for (DWORD i =0;i<m_Handle.size();i++)
	{
		FreeLibrary((HMODULE)m_Handle[i]);
	}
	//清除所有保存在动态数组里面的函数指针
	m_PluginName.swap(vector <CString>());
	m_Handle.swap(vector <DWORD>());
	m_InitPlugin_Address.swap(vector <DWORD>());
	m_CreateProcessEvent_Address.swap(vector <DWORD>());
	m_CreateThreadEvent_Address.swap(vector <DWORD>());


}

VOID LoadPlugin::LoadPluginFile()
{
	//设置目录，在程序目录下的Plugin文件夹
	CString nPluginPath(GetProgramDir() + TEXT("Plugin"));
	WIN32_FIND_DATA fData = { 0 };
	HANDLE hFind = INVALID_HANDLE_VALUE;
	HMODULE nHandle = nullptr;
	DWORD nAddress = 0;


	//开始遍历目录下的所有文件
	hFind = FindFirstFile(nPluginPath + L"\\*", &fData);
	if (hFind == INVALID_HANDLE_VALUE)return;

	do {
		//如果这个是一个文件夹，则跳过
		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {}

		//如果这是一个文件，则进入
		else
		{
			//获取该文件的后缀名，如果不是dll则跳过
			if (GetFileLastName(fData.cFileName) != TEXT("dll"))continue;

			//调用LoadLibrary载入文件
			nHandle = LoadLibrary(nPluginPath + TEXT("\\") + fData.cFileName);

			//调用GetProcAddress取出指定的插件初始化函数地址
			nAddress = (DWORD)GetProcAddress(nHandle, CStringA("InitPlugin"));

			//如果函数地址不为0，则存在此函数
			if (nAddress)
			{
				//将该插件的加载句柄保存到动态数组内
				m_Handle.push_back((DWORD)nHandle);
				//将初始化函数地址保存在动态数组内
				m_InitPlugin_Address.push_back((DWORD)nAddress);
				//将初始化函数地址强制转换位函数指针
				InitPlugin = (pInitPlugin)nAddress;
				//通过函数指针调用初始化函数，并把调用的返回结果保存到动态数组内，返回的是插件的名字
				m_PluginName.push_back(CString(InitPlugin()));

				//取出指定接口CreateProcessEvent的函数地址
				nAddress = (DWORD)GetProcAddress(nHandle, CStringA("CreateProcessEvent"));
				//如果函数地址不为0，则存在此函数，将函数地址保存到动态数组内
				if (nAddress)m_CreateProcessEvent_Address.push_back((DWORD)nAddress);

				//取出指定接口CreateThreadEvent的函数地址
				nAddress = (DWORD)GetProcAddress(nHandle, CStringA("CreateThreadEvent"));
				//如果函数地址不为0，则存在此函数，将函数地址保存到动态数组内
				if (nAddress)m_CreateThreadEvent_Address.push_back((DWORD)nAddress);
			}

			//如果函数地址为0，则释放该文件
			else
			{
				FreeLibrary(nHandle);
				nHandle = nullptr;
			}

		}
		//继续遍历下一个文件
	} while (FindNextFile(hFind, &fData));
}



//************************************
// 调试全部插件的函数
//************************************
VOID LoadPlugin::CallCreateProcessEvent(DWORD m_Pid, DWORD m_Tid)
{
	//遍历所有CreateProcessEvent函数的地址
	for (DWORD i = 0; i < m_CreateProcessEvent_Address.size(); i++)
	{
		//强制转换为函数指针
		CreateProcessEvent = (pCreateProcessEvent)m_CreateProcessEvent_Address[i];
		//通过函数指针调用该函数
		CreateProcessEvent(m_Pid, m_Tid);
	}
}

VOID LoadPlugin::CallCreateThreadEvent(DWORD nHandle, DWORD nThreadLocalBase)
{
	//遍历所有CreateThreadEvent函数的地址
	for (DWORD i = 0; i < m_CreateThreadEvent_Address.size(); i++)
	{
		//强制转换为函数指针
		CreateThreadEvent = (pCreateThreadEvent)m_CreateThreadEvent_Address[i];
		//通过函数指针调用该函数
		CreateThreadEvent(nHandle, nThreadLocalBase);
	}
}
