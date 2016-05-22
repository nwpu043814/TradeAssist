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


#define DECLARE_POINT_FUNCTION_WITH_DIRECT(fun_name)  \
	public:const CPoint & fun_name ## (int direct);\
	private: CPoint mLow ## fun_name;\
	private: CPoint mHigh ## fun_name;

#define DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(fun_name)  public:const CPoint & fun_name ## ();\
	private: CPoint m ## fun_name;

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
	DECLARE_POINT_FUNCTION_WITH_DIRECT(getPrice2Count)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(getDirection2Price)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(getEntrust2Direction)

private:
	CPoint mHighPrice2Count;
	CPoint mLowPrice2Count;
	CPoint mHighDirection2Price;
	CPoint mCount2Confirm;
	CPoint mHighEntrust2Direction;
	CPoint mOrigin2Remove;
public:
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(getOrigin2Entrust)
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(GetOriginal2DoLow)
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(GetOriginal2DoHigh)
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(getCount2Confirm)
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(getOrigin2Remove)
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(getRemove2Confirm)
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(GetOrigin2Count)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(GetCount2OrderButton)
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
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(GetTradeCount)
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(GetOrigin2DropListButton)
	DECLARE_POINT_FUNCTION_WITHOUT_DIRECT(GetOrderTypeButton)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(GetDirectionButton)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(GetPriceAdjustButton)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(GetEnableStopButton)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(GetInitialStopPriceButton)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(GetAdjustStopPriceButton)
	DECLARE_POINT_FUNCTION_WITH_DIRECT(GetConfirmButton)
public:
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
private:
	CPoint mHFPriceRange2PriceHigh;
	CPoint mHFPriceRange2PriceLow;
public:
	const CPoint& GetPrice2StopCheckbox(void);
private:
	CPoint mPrice2StopCheckbox;
};
