#pragma once
#include "LuaEngine.h"
#include "SimulateAction.h"
#include "Constant.h"

class CBaseGuaDan
{
protected:
	CLuaEngineP mLuaEngine;
	CSimulateActionP mAction;
		HWND mWndNewOwner;
public:
	CBaseGuaDan(CLuaEngineP lua, CSimulateActionP action, HWND mWndNewOwner);
	virtual ~CBaseGuaDan(void);
	virtual void CBaseGuaDan::DoHop(int x, int y)  const;
	virtual CPoint GetGuaDanDialogPos(void);
	virtual int DoSingleSideAction(int diff, int direct, int count, int windowDelay);
	int DoDoubleSide(int lowDiff, int highDiff, int count, int windowDelay, int direct);
	int DoSingleSide(int diff, int direct, int count, int windowDelay);
	virtual CString GetContentFromClipboard(void) const;
	virtual BOOL SetClipboardContent(CString source) const;
	BOOL CheckEditPasteResult(const CString & mLastClipboardContent);
	CString GetEditText(UINT checkLen = PRICE_LENGTH);
	CPoint GetDialogPosByTitle(CString title , UINT retryTime = FIND_SUN_DIALOG_MAX_RETRY_TIMES);
};
