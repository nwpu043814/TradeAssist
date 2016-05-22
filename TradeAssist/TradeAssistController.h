#pragma once
#include "Resource.h"
#include "stdafx.h"
#include "HotKeyManager.h"
#include "ActionManager.h"
#include "DataManager.h"

class CTradeAssistController : public CWnd
{
private:
	CHotKeyManagerP mHotkeyManager;
	CDataManagerP mDataManager;
	CActionManagerP mActionManager;
	CWnd*			mMainWnd;
	CDataKP mDataK;

public:
	CTradeAssistController(CHotKeyManagerP hotkey, CDataManagerP data, CActionManagerP action, CWnd* wnd
		, CDataKP dataK);
	virtual ~CTradeAssistController(void);
	void EnableWindowMostTop(BOOL isTop);
	void SetMainWindow(CWnd* wnd);
};

typedef CTradeAssistController * CTradeAssistControllerP;