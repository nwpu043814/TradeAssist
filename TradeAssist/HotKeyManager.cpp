#include "StdAfx.h"
#include "HotKeyManager.h"
#include "Constant.h"

CHotKeyManager::CHotKeyManager(HWND hWnd):m_hWnd(hWnd)
{
}

CHotKeyManager::~CHotKeyManager(void)
{
}


void CHotKeyManager::InstallHotKey(void)
{
	::RegisterHotKey(m_hWnd, HOT_KEY_CODE_LOW, MOD_WIN, VK_Z);
	::RegisterHotKey(m_hWnd, HOT_KEY_CODE_HIGH, MOD_WIN, VK_X);
	::RegisterHotKey(m_hWnd, HOT_KEY_CODE_COUNT, MOD_WIN, VK_C);
	::RegisterHotKey(m_hWnd, HOT_KEY_FLASH_COMPLETE, MOD_WIN, VK_A);
	::RegisterHotKey(m_hWnd, HOT_KEY_DECREASE_PRICE, MOD_SHIFT, VK_Z);
	::RegisterHotKey(m_hWnd, HOT_KEY_INCREASE_PRICE, MOD_SHIFT, VK_X);
	::RegisterHotKey(m_hWnd, HOT_KEY_TEST_SERVER, MOD_SHIFT, VK_T);
	::RegisterHotKey(m_hWnd, HOT_KEY_DIRECT_DUO, MOD_SHIFT, VK_D);
	::RegisterHotKey(m_hWnd, HOT_KEY_ALT_D, MOD_ALT, VK_D);
	::RegisterHotKey(m_hWnd, HOT_KEY_DIRECT_KONG, MOD_SHIFT, VK_K);
	::RegisterHotKey(m_hWnd, HOT_KEY_INCREASE_THRESHOLD, MOD_SHIFT, VK_S);
	::RegisterHotKey(m_hWnd, HOT_KEY_DECREASE_THRESHOLD, MOD_SHIFT, VK_A);
}

void  CHotKeyManager::UninstallHotkey(void)
{
	::UnregisterHotKey(m_hWnd,HOT_KEY_CODE_LOW);
	::UnregisterHotKey(m_hWnd,HOT_KEY_CODE_HIGH);
	::UnregisterHotKey(m_hWnd,HOT_KEY_CODE_COUNT);
	::UnregisterHotKey(m_hWnd,HOT_KEY_FLASH_COMPLETE);
	::UnregisterHotKey(m_hWnd,HOT_KEY_DECREASE_PRICE);
	::UnregisterHotKey(m_hWnd,HOT_KEY_INCREASE_PRICE);
	::UnregisterHotKey(m_hWnd,HOT_KEY_TEST_SERVER);
	::UnregisterHotKey(m_hWnd,HOT_KEY_DIRECT_DUO);
	::UnregisterHotKey(m_hWnd,HOT_KEY_DIRECT_KONG);
	::UnregisterHotKey(m_hWnd,HOT_KEY_INCREASE_THRESHOLD);
	::UnregisterHotKey(m_hWnd,HOT_KEY_DECREASE_THRESHOLD);
	::UnregisterHotKey(m_hWnd,HOT_KEY_ALT_D);

}
int CHotKeyManager::ParseHotKey(UINT mode, UINT virKey)
{
	if (mode == MOD_WIN)
	{
		if (virKey == VK_Z)
		{
			return HOT_KEY_CODE_LOW;
		}
		else if (virKey == VK_X)
		{
			return HOT_KEY_CODE_HIGH;
		}
		else if (virKey == VK_C)
		{
			return HOT_KEY_CODE_COUNT;
		}
		else if (virKey == VK_A)
		{
			return HOT_KEY_FLASH_COMPLETE;
		}
	}
	else if (mode == MOD_SHIFT)
	{
		if (virKey == VK_Z)
		{
			return HOT_KEY_DECREASE_PRICE;
		}
		else if (virKey == VK_X)
		{
			return HOT_KEY_INCREASE_PRICE;
		}
		else if (virKey == VK_T)
		{
			return HOT_KEY_TEST_SERVER;
		}
		else if (virKey == VK_D)
		{
			return HOT_KEY_DIRECT_DUO;
		}
		else if (virKey == VK_K)
		{
			return HOT_KEY_DIRECT_KONG;
		}
		else if (virKey == VK_S)
		{
			return HOT_KEY_INCREASE_THRESHOLD;
		}
		else if (virKey == VK_A)
		{
			return HOT_KEY_DECREASE_THRESHOLD;
		}
	}
	else if (mode == MOD_ALT)
	{
		if (virKey == VK_D)
		{
			return HOT_KEY_ALT_D;
		}

	}

	return VK_INVALID;
}
