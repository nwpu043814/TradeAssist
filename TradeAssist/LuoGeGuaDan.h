#pragma once
#include "baseguadan.h"

class CLuoGeGuaDan :
	public CBaseGuaDan
{
public:
	CLuoGeGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner);
	virtual ~CLuoGeGuaDan(void);
	virtual CPoint GetGuaDanDialogPos(void);
	virtual int DoSingleSideAction(int diff, int direct, int count, int windowDelay);
};
