#pragma once
#include "SimulateAction.h"
#include "Constant.h"


#define  WM_DO_HUIFENG_GUADAN	WM_USER + 250

class CActionManager :
	public CWinThread
{
public:
	CActionManager();
	~CActionManager(void);
	CString GetContentFromClipboard(void)  const;
	BOOL SetClipboardContent(CString source)  const;
	CString GetEditText(UINT checkLen = PRICE_LENGTH);
	WORD GetMilliseconds(void);
	BOOL CheckEditPasteResult(const CString & mLastClipboardContent);
	int DoTrade(const POINT &dialogPos,const POINT &	direction2Price,const CPoint & start2Tab,const POINT & tab2Direction, const POINT & price2CountVector,const CPoint & count2Confirm, double hightDiff, double lowDiff, CString & mLastClipboardContent, BOOL direction,
		int mIntOrderCount, BOOL mIsAutoSubmits);
	int UpdatePrice(bool isAdd, float diff);
	CSimulateAction* GetAction();
private:
	CSimulateActionP mAction;
	HWND mWndNewOwner;
	CLuaEngine mLuaEngine;
	
	int DoHFDoubleSide(int lowDiff, int highDiff,int count,int windowDelay, int direct);
	// 1 for low 2 for high
	int DoHFSingleSide(int diff,int direct,int count,int windowDelay);
	int DoHFSingleSideAction(int diff,int direct, int count,int windowDelay);
	const CPoint & GetHFDialogPos(void) const;
	void DoHop(int x, int y)  const;
	const CPoint& GetHFConfirmDialogPos(void) const;
	void CloseHFConfirmDialog(int top, int left);
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CActionManager)
protected:
	afx_msg void OnDoHuifengGuadan(WPARAM wParam,LPARAM lParam);
public:
	void SetWindowOwner(HWND owner);
	virtual BOOL InitInstance();
	BOOL MakeOrder(const CPoint & start2Button,	const CPoint & start2Count,const CPoint & count2Button,	UINT count, BOOL isDirectly);
	POINT GetSunAwtDialogPos(void);
};

typedef CActionManager * CActionManagerP;