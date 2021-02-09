#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <bitset>
#include <thread>

#include "ASIFramework/main.h"

enum eKeyState : uint8_t
{
	KEYSTATE_NONE,
	KEYSTATE_HOLDING,
	KEYSTATE_PRESSED
};

#define MAX_KEYS	256

class CKeyManager
{
	std::bitset<MAX_KEYS> m_bKeyState;
	WNDPROC m_pOrigWndProc;
	HWND m_hWnd;
	ULONGLONG m_ullLastTick;
public:
	CKeyManager(HWND hWnd);
	~CKeyManager();

	void ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void ProcessKeybdMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	ULONGLONG GetKeyBits() const;

	WNDPROC GetWndProc() const;
	static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

extern CKeyManager* g_pKeyManager;
