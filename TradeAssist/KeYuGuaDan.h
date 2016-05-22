#pragma once
#include "baseguadan.h"

class CKeYuGuaDan :
	public CBaseGuaDan
{
public:
	CKeYuGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner);
	virtual ~CKeYuGuaDan(void);
	virtual CPoint GetGuaDanDialogPos(void);
	virtual int DoSingleSideAction(int diff, int direct, int count, int windowDelay);
};
