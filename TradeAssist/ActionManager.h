#pragma once
#include "SimulateAction.h"
#include "Constant.h"

class CActionManager
{
public:
	CActionManager(__in HWND hWndNewOwner, CLuaEngine &  lua);
	~CActionManager(void);
	CString GetContentFromClipboard(void)  const;
	BOOL SetClipboardContent(CString source)  const;
	CString GetEditText(UINT checkLen = PRICE_LENGTH);
	WORD GetMilliseconds(void);
	BOOL CheckEditPasteResult(const CString & mLastClipboardContent);
	int DoTrade(const POINT &dialogPos,const POINT &	direction2Price,const CPoint & start2Tab,const POINT & tab2Direction, const POINT & price2CountVector,const CPoint & count2Confirm, double hightDiff, double lowDiff, CString & mLastClipboardContent, BOOL direction,
		int mIntOrderCount, BOOL mIsAutoSubmits);
	LRESULT OnDoTradeMsg(UINT w , UINT l);
	int UpdatePrice(bool isAdd, float diff);
	CSimulateAction* GetAction();
	LRESULT SemicAutoTrade(int direct);
private:
	CSimulateActionP mAction;
	HWND mWndNewOwner;
	CLuaEngine &  mLuaEngine;
public:
	BOOL MakeOrder(const CPoint & start2Button,
	const CPoint & start2Count,const CPoint & count2Button,
	UINT count, BOOL isDirectly);
	POINT GetSunAwtDialogPos(void);
	int DoHFDoubleSide(int lowDiff, int highDiff,int count,int windowDelay) const;
	// 1 for low 2 for high
	int DoHFSingleSide(int diff,int direct,int count,int windowDelay) const;
	int DoHFSingleSideAction(int diff,int direct, int count,int windowDelay) const;
	const CPoint & GetHFDialogPos(void) const;
	void DoHop(int x, int y)  const;
	const CPoint& GetHFConfirmDialogPos(void) const;
	void CloseHFConfirmDialog(int top, int left) const;
};
