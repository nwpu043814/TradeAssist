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

	// �Ƿ��Զ��ύ��
	BOOL mIsAutoSubmits;
	WORD mLastTime;
	CString mStrHighPriceDiff;
	UINT mIntOrderCount;
	// ��Ϣ��ʱ ��λ����
	UINT mIntMsgDelayMilliSeconds;
	UINT mIntHour;
	UINT mIntMinute;
	CString mIntSecond;
	// �Զ��µ����������������λΪ��
	CString mAutoCompleteInterval;

	// �Ƿ��������Զ�ƽ��ʱ��
	BOOL mEnableCheckAutoCloseDepot;
	CString mStrLowPriceDiff;
	double mDataKClose;
	double mDataKOpen;
	CString mDataKCloseTime;
	CString mDataKOpenTime;
	double mDataKHighPrice;
	double mDataKLowPrice;
	// �Զ�ƽ����ֵ
	UINT mUintAutoCloseThreshold;
	double mDataKStatisticsUpdrop;
	int mDataKDayUpdrop;
	// ��ǰ�ص�����
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
