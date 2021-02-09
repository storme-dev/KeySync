#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

#include "CKeyManager.h"
#include "ASIFramework/main.h"

int Init()
{
	while (*(DWORD*)0xC8D4C0 != 9)
	{
		Sleep(100);
	}
	IDirect3DDevice9* pDevice = *(IDirect3DDevice9 * *)0xC97C28;
	D3DDEVICE_CREATION_PARAMETERS params;

	if (FAILED(pDevice->GetCreationParameters(&params)))
	{
		return 0;
	}
#ifdef _DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	if (pSAMP)
	{
		while (!pSAMP->tryInit())
			Sleep(100);
	}

	g_pKeyManager = new CKeyManager(params.hFocusWindow);

	return 0;
}

void DeInit()
{
	if (g_pKeyManager)
	{
		delete g_pKeyManager;
		g_pKeyManager = nullptr;
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason) 
	{
	case DLL_PROCESS_ATTACH:
		pSAMP = new SAMPFramework(GetModuleHandle("samp.dll"));
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Init, NULL, NULL, NULL);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DeInit();
		break;
	}
	return TRUE;
}