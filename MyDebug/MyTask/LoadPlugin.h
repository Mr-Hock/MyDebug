#pragma once
#include "MyHead.h"
#include "vector"

using std::vector;

class LoadPlugin
{
public:
	LoadPlugin();
	~LoadPlugin();

	vector <CString>m_PluginName;
	vector <DWORD>m_Handle;
	vector <DWORD>m_InitPlugin_Address;
	vector <DWORD>m_CreateProcessEvent_Address;
	vector <DWORD>m_CreateThreadEvent_Address;

	typedef CHAR*(WINAPI *pInitPlugin)(VOID);
	typedef VOID(WINAPI *pCreateProcessEvent)(DWORD, DWORD);
	typedef VOID(WINAPI *pCreateThreadEvent)(DWORD, DWORD);
	pInitPlugin InitPlugin;
	pCreateProcessEvent CreateProcessEvent;
	pCreateThreadEvent CreateThreadEvent;

	VOID LoadPluginFile();
	VOID CallCreateProcessEvent(DWORD nPid, DWORD nTid);
	VOID CallCreateThreadEvent(DWORD nHandle, DWORD nThreadLocalBase);

};

