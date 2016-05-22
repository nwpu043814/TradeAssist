#pragma once
#include "baseguadan.h"

class CTianTongGuaDan :
	public CBaseGuaDan
{
public:
	CTianTongGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner);
	const CPoint& GetHFConfirmDialogPos(void) const;
	virtual ~CTianTongGuaDan(void);
	virtual CPoint GetGuaDanDialogPos(void);
	virtual int DoSingleSideAction(int diff, int direct, int count, int windowDelay);
	void CloseHFConfirmDialog(int left, int top);
};
