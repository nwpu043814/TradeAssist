#pragma once
#include "baseguadan.h"

class CZhonXinGuaDan :
	public CBaseGuaDan
{
public:
	CZhonXinGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner);
	virtual ~CZhonXinGuaDan(void);
	virtual CPoint GetGuaDanDialogPos(void);
	virtual int DoSingleSideAction(int diff, int direct, int count, int windowDelay);
};
