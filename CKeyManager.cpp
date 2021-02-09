#include "CKeyManager.h"

CKeyManager* g_pKeyManager = nullptr;

CKeyManager::CKeyManager(HWND hWnd)
{
	//m_bKeyState.reset();
	m_hWnd = hWnd;
	m_pOrigWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
	m_ullLastTick = GetTickCount64();
}

CKeyManager::~CKeyManager()
{
	SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_pOrigWndProc);
}

void CKeyManager::ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	{
		ProcessKeybdMessage(msg, wParam, lParam);
		return;
	}
	default:
		return;
	}
}
void CKeyManager::ProcessKeybdMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int iKey = (int)wParam;
	auto state = KEYSTATE_NONE;

	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		state = KEYSTATE_HOLDING;
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		state = KEYSTATE_NONE;
		break;
	default:
		return;
	}
	if (state == KEYSTATE_NONE && m_bKeyState[iKey] == KEYSTATE_HOLDING)
	{
		state = KEYSTATE_PRESSED;
	}
	m_bKeyState[iKey] = state;
	if (GetTickCount64() - m_ullLastTick <= 50ull)
	{
		return;
	}
	m_ullLastTick = GetTickCount64();
	std::thread([iKey, state]() {
		BitStream bs;
		bs.Write((uint8_t)ID_KEYSYNC_UPDATE);
		bs.Write((uint8_t)iKey);
		bs.Write((uint8_t)state);
		pSAMP->sendPacket(&bs);
		}).detach();
}

ULONGLONG CKeyManager::GetKeyBits() const
{
	return m_bKeyState.to_ullong();
}

WNDPROC CKeyManager::GetWndProc() const
{
	return m_pOrigWndProc;
}

LRESULT __stdcall CKeyManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_pKeyManager->ProcessMessage(msg, wParam, lParam);

	return CallWindowProc(g_pKeyManager->GetWndProc(), hWnd, msg, wParam, lParam);
}
