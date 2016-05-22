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
	CPoint mOrigin2Entrust;
	CPoint mLowEntrust2Direction;
	CPoint mLowDirection2Price;
public:


private:
	CPoint mHighPrice2Count;
	CPoint mLowPrice2Count;
	CPoint mHighDirection2Price;
	CPoint mCount2Confirm;
	CPoint mHighEntrust2Direction;
	CPoint mOrigin2Remove;
private:
	CPoint mRemove2Confirm;
	CPoint mOrigin2Count;
	CPoint mCount2OrderButtonLow;
	CPoint mCount2OrderButtonHigh;
private:
	CString mNonfarmerTitle;
	CString mJoblessTitle;
	CString mDailyFxUrl;
	double mStartPrice;
public:
	CString GetDailyFxUrl(void);
	CString GetNonfarmerTitle(void);
	CString GetJoblessTitle(void);
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
	int GetDoubleSideType(void);
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
	int mDoubleSideType;
	CPoint mHFTradeCount;
	CPoint mHFScaleListItem;
public:
	const CPoint& GetScaleListItem(void);
	const CPoint& GetPriceRange2Price(int direct);
public:
	const CPoint& GetPrice2StopCheckbox(void);
private:
	CPoint mPrice2StopCheckbox;
public:
	int getStopGainDiff(int direct);
	const CPoint& getGainCheckbox2GainPriceEdit(void);
private:
	CPoint mGainCheckbox2GainPriceEdit;
	CPoint mHFConfirDialogOK;
public:
	const CPoint& GetHFConfirDialogOK(void);
	const CPoint& GetOrigin2DropListButton(void);
	const CPoint& GetOrderTypeButton(void);
	const CPoint& GetTradeCount(void);
	const CPoint& GetDirectionButton(int direct);
	const CPoint& GetPriceAdjustButton(int direct);
	const CPoint & GetEnableStopButton(int direct);
	const CPoint & GetInitialStopPriceButton(int direct);
	const CPoint & GetAdjustStopPriceButton(int direct);
	const CPoint& GetConfirmButton(int direct);
	const CPoint & getOrigin2Entrust(void);
	const CPoint & getDirection2Price(int direct);
	const CPoint& getCount2Confirm(void);
	const CPoint& getPrice2Count(int direct);
	const CPoint& getOrigin2Remove(void);
	const CPoint& GetOriginal2DoLow(void);
	const CPoint& getEntrust2Direction(int direct);
	const CPoint& getRemove2Confirm(void);
	const CPoint& GetOrigin2Count(void);
	const CPoint& GetCount2OrderButton(int direct);
	const CPoint& GetOriginal2DoHigh(void);
	const CPoint& GetGainCheckbox2GainPriceEdit(void);
private:
	CPoint mPriceRange2PriceLow;
	CPoint mPriceRange2PriceHigh;
public:
	// ÷π”Ø◊Ó–°æ‡¿Î
	int GetStopGainThreshold(void);
	int GetStopLoseDiff(int direct);
	int GetDebugSleepInterval(void);
	const CPoint GetHuiFengDirectionButton(int direct);
	const CPoint& GetHuiFengInitialStopPriceButton(int direct);
	const CPoint& GetHuiFengPriceRange2Price(int direct);
	const CPoint& GetHuiFengPrice2StopCheckbox(void);
	int GetHuiFengStopGainDiff(int direct);
	int GetHuiFengStopThreshold(void);
	 int GetHuiFengStopLoseThreshold(int direct);
	 int GetTianTongRetryTimes();
	 
	const CPoint& GetHuiFengConfirmButton(int direct);
	CPoint GetLuoGeOrigin2Entrust(void);
	CPoint GetLuoGeEntrust2Direct(int direct);
	CPoint GetLuoGeDirection2Price(int direct);
	CPoint GetLuoGePrice2Count(int direct);
	int GetLuoGeStopLoseDiff(void);
	int GetLuoGeStopGainDiff(void);
	CPoint GetZhongXinOrigin2Entrust(int direct);
	CPoint GetZhongXinEntrust2Direction(int direct);
	CPoint GetZhongXinDirection2Price(int direct);
	CPoint GetZhongXinPrice2Count(int direct);
};

typedef class CLuaEngine * CLuaEngineP;