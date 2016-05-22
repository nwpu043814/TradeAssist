#pragma once
#include "SimulateAction.h"
#include "Constant.h"

class CActionManager
{
public:
	CActionManager(__in HWND hWndNewOwner);
	~CActionManager(void);
	CString GetContentFromClipboard(void);
	BOOL SetClipboardContent(CString source);
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
public:
	BOOL MakeOrder(const CPoint & start2Button,
	const CPoint & start2Count,const CPoint & count2Button,
	UINT count, BOOL isDirectly);
	POINT GetSunAwtDialogPos(void);
};
