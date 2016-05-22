#pragma once
#include "baseguadan.h"
#include "atltypes.h"

class CTianTongGuaDan :
	public CBaseGuaDan
{
public:
	CTianTongGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner);
	void GetHFConfirmDialogPos(CPoint& pos, int retryTimes = FIND_SUN_DIALOG_MAX_RETRY_TIMES) const;
	virtual ~CTianTongGuaDan(void);
	virtual CPoint GetGuaDanDialogPos(void);
	virtual int DoSingleSideAction(int diff, int direct, int count, int windowDelay);
	void CloseHFConfirmDialog(int left, int top);
	void DoTianTongChaseAction(int direction, int count);

	//只点击方向和确定按钮，快速下单
	void DoTianTongFastChase(int direction);
private:
	CPoint mInitialCursorPos;
public:
	CPoint GetInitialCursorPos(void);
};
