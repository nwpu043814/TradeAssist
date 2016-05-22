#pragma once
#include "LuaEngine.h"
#include "EcnomicPair.h"
#include "HuifengGuadanParam.h"
class CDataManager
{
public:
	CDataManager(void);
	virtual ~CDataManager(void);
	void Load(void);
	void Save(void);

	// 是否自动提交。
	BOOL mIsAutoSubmits;
	WORD mLastTime;
	CString mStrHighPriceDiff;
	UINT mIntOrderCount;
	// 消息延时 单位毫秒
	UINT mIntMsgDelayMilliSeconds;
	UINT mIntHour;
	UINT mIntMinute;
	CString mIntSecond;
	// 自动下单的两个单间隔，单位为秒
	CString mAutoCompleteInterval;

	// 是否允许检测自动平仓时机
	BOOL mEnableCheckAutoCloseDepot;
	CString mStrLowPriceDiff;
	double mDataKClose;
	double mDataKOpen;
	CString mDataKCloseTime;
	CString mDataKOpenTime;
	double mDataKHighPrice;
	double mDataKLowPrice;
	// 自动平仓阈值
	UINT mUintAutoCloseThreshold;
	double mDataKStatisticsUpdrop;
	int mDataKDayUpdrop;
	// 当前回调幅度
	int mDataKCurrent2ExtremeDiff;
	int mOpenDirection;
	CString mServerIp;
	UINT mUintDoHttpInterval;
	PEcnomicData *mNonfarmerNumber;
	PEcnomicData *mJoblessRate;
	PEcnomicData *mLocalPrice;
	BOOL mBoolEnableAutoThreshold;
	bool mIsTimer4Tomorrow;
	CString mActualNonfarmerNumber;
	CString mActualJoblessRate;
	CString mNonfarmerNumberResult;
	CString mJoblessRateResult;
	CString mNonfarmerNumnerWeight;
	CString mJoblessRateWeight;
	CString mNonfarmerNumberCount;
	CString mJoblessRateCount;
	UINT mServerPort;
	CString mTotalConclution;
	unsigned int mPullPriceCount;
	float mQueryPriceUseTime;
	float mCapturePriceUseTime;
	long mUILastUpdateTime;
	BOOL mEnableWindowMostTop;
	void initialJoblessRateThread(CLuaEngine * p);
	void initialLocalPriceThread(void);
	void initialNonfarmerThread(CLuaEngine * p);
	void StartUpdateFeiNongData(void);
	void StartHttpThread(PEcnomicData data);
	CHuifengGuadanParamP GetGuaDanParam(int dirct);
};
