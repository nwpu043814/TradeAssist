#pragma once

class CHotKeyManager
{
private:
	HWND m_hWnd;  
public:
	CHotKeyManager(HWND m_hWnd);
	virtual ~CHotKeyManager(void);

	void InstallHotKey(void);

	void UninstallHotkey(void);
	int ParseHotKey(UINT mode, UINT virKey);
};
