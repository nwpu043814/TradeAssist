#pragma once
#include "baseguadan.h"

class CHaiJiaoGuaDan :
	public CBaseGuaDan
{
public:
	CHaiJiaoGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner);
	virtual ~CHaiJiaoGuaDan(void);
	virtual CPoint GetGuaDanDialogPos(void);
	virtual int DoSingleSideAction(int diff, int direct, int count, int windowDelay);
};
