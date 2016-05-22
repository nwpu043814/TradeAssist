#pragma once
#include "afxmt.h"
#include <map>
#include "EcnomicCompareResult.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
} 

#include "atltypes.h"
class CLuaEngine
{
public:

	CLuaEngine(void);
	~CLuaEngine(void);
	lua_State * mLua;
	lua_State * InitLuaState(void);
	int GetDynamicThreshold(int ,BOOL enableAuto, int back, CString diff, UINT threshold);
	CString GetJoblessUrl(int );
	CString GetNonfarmerWorkerUrl(int);
	lua_State* GetLuaState(int id);
	CString GetExpectValue(CString name);
private:
	CString mExpectNonfarmerNumber;
	CString mNonfarmerWorkerUrl;
	CString mJoblessUrl;
	CString mExpectJoblessRate;
	CPoint mOriginal2DoHigh;
	CPoint mOriginal2DoLow;
public:
	CPoint GetOriginal2DoHigh(void);
	CPoint GetOriginal2DoLow(void);
	const CPoint & getOrigin2Entrust(void);
private:
	CPoint mOrigin2Entrust;
public:
	const CPoint& getEntrust2Direction(int direct);
private:
	CPoint mLowEntrust2Direction;
public:
	const CPoint & getDirection2Price(int direct);
private:
	CPoint mLowDirection2Price;
public:
	const CPoint& getPrice2Count(int direct);
private:
	CPoint mHighPrice2Count;
	CPoint mLowPrice2Count;
	CPoint mHighDirection2Price;
	CPoint mHighEntrust2Direction;
public:
	const CPoint& getCount2Confirm(void);
private:
	CPoint mCount2Confirm;
public:
	const CPoint& getOrigin2Remove(void);
private:
	CPoint mOrigin2Remove;
public:
	const CPoint& getRemove2Confirm(void);
private:
	CPoint mRemove2Confirm;
	CPoint mOrigin2Count;
	CPoint mCount2OrderButtonLow;
	CPoint mCount2OrderButtonHigh;
public:
	const CPoint& GetOrigin2Count(void);
	const CPoint& GetCount2OrderButton(UINT direct);
private:
	CString mNonfarmerTitle;
	CString mJoblessTitle;
	CString mDailyFxUrl;
	double mStartPrice;
public:
	CString GetDailyFxUrl(void);
	CString GetNonfarmerTitle(void);
	CString GetJoblessTitle(void);
	double GetStartPrice(void);
	void SetStartPrice(double startPrice);
	INT GetChasePriceThreshold(void);
private:
	int mChasePriceThreshold;
	bool mHasChased;
public:
	bool GetHasChased(void);
	void SetHasChased(bool chase);
private:
	CTime  mStartTime;
public:
	CTime & GetStartTime(void);
private:
	INT mChaseMaxTime;
public:
	INT GetChaseMaxTime(void);
private:
	int mChasePriceMax;
public:
	int GetChasePriceMax(void);
	const CPoint& getOrigin2DropListButton(void);
	const CPoint& getOrderTypeButton(void);
	const CPoint& getDirectionButton(int direct);
	const CPoint& getPriceAdjustButton(int direct);
	const CPoint & getEnableStopButton(int direct);
	const CPoint & getInitialStopPriceButton(int direct);
	const CPoint & getAdjustStopPriceButton(int direct);
	const CPoint& getConfirmButton(int direct);
private:
	CPoint mHFOrigin2DropListButton;
	CPoint mHFOrderTypeButton;
	CPoint mHFDirectionButtonLow;
	CPoint mHFDirectionButtonHigh;
	CPoint mHFPriceAdjustButtonLow;
	CPoint mHFPriceAdjustButtonHigh;
	CPoint mHFEnableStopButtonLow;
	CPoint mHFEnableStopButtonHigh;
	CPoint mHFInitialStopPriceButtonLow;
	CPoint mHFInitialStopPriceButtonHigh;
	CPoint mHFAdjustStopPriceButtonLow;
	CPoint mHFAdjustStopPriceButtonHigh;
	CPoint mHFConfirmButtonLow;
	CPoint mHFConfirmButtonHigh;
};
