#pragma once
#include "SimulateAction.h"
#include "Constant.h"


#define  WM_DO_HUIFENG_GUADAN	WM_USER + 250
#define  WM_DO_TIANTONG_GUADAN	WM_USER + 251
#define  WM_DO_HAIJIAO_GUADAN	WM_USER + 252
#define  WM_DO_KUNJIAO_GUADAN	WM_USER + 253
#define  WM_DO_LUOGE_GUADAN	WM_USER + 254
#define  WM_DO_ZHONGXIN_GUADAN	WM_USER + 255
#define  WM_DO_INPUT_PRICE	WM_USER + 256

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
	int DoTrade(double hightDiff, double lowDiff, BOOL direction,
		int mIntOrderCount);
	int UpdatePrice(bool isAdd, float diff);
	CSimulateAction* GetAction();
private:
	CSimulateActionP mAction;
	HWND mWndNewOwner;
	CLuaEngine mLuaEngine;
	
	int DoHFDoubleSide(int lowDiff, int highDiff,int count,int windowDelay, int direct, int softwareType);
	// 1 for low 2 for high
	int DoHFSingleSide(int diff,int direct,int count,int windowDelay, int softwareType);

	const CPoint & GetDialogPosByTitle(CString title , UINT retryTime = FIND_SUN_DIALOG_MAX_RETRY_TIMES) const;
	void DoHop(int x, int y)  const;
	const CPoint& GetHFConfirmDialogPos(void) const;
	void CloseHFConfirmDialog(int top, int left);
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CActionManager)
protected:
	afx_msg void OnDoHuiFengGuadan(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoTianTongGuadan(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoKunJiaoGuadan(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoLuoGeGuadan(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoZhongXinGuadan(WPARAM wParam,LPARAM lParam);
	afx_msg void OnDoInputPrice(WPARAM wParam,LPARAM lParam);
public:
	void SetWindowOwner(HWND owner);
	virtual BOOL InitInstance();
	BOOL MakeOrder(const CPoint & start2Button,	const CPoint & start2Count,const CPoint & count2Button,	UINT count, BOOL isDirectly);
	POINT GetSunAwtDialogPos(CString title);
	int DoHuiFengSingleSideAction(int diff, int direct, int count, int windowDelay);
	CPoint GetKunJiaoDialogPos(void);
	int DoKunJiaoSingleSideAction(int diff, int direct, int count, int windowDelay);
	int DoZhongXinSingleSideAction(int diff, int direct, int count, int windowDelay);
};

typedef CActionManager * CActionManagerP;