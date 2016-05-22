#pragma once

class CDataK
{
public:
	CDataK(void);
	virtual ~CDataK(void);
private:
	// open price
	double mOpen;
	// close price
	double mClose;
	// highest price
	double mHigh;
	// lowest price
	double mLow;
	CString mOpenTime;
	CString mCloseTime;
	LONG mMillionSecond;
public:
	void SetClose(double close, CString time);
	double GetHigh(void);
	double GetLow(void);
	CString GetOpenTime(void);
	void SetCloseTime(CString closeTime);
	CString GetCloseTime(void);
	// maintain  the high price and the low price.
	void UpdateRange(double latestPrice);
	BOOL IsPositive(void);
	BOOL IsNegtive(void);
	double GetAmplitude(void);
	void InitialHighLow(double price);
	double GetAmplitudeRate(void);
	double GetClose(void);
	double GetOpen(void);
private:
	// 回调幅度
	int mIntCurrent2ExtremeDiff;

private:
	float	mQueryPriceUseTime;
	float	mCapturePriceUseTime;
public:
	int GetCurrent2ExtremeDiff(void);
	// 设置回调幅度
	void SetCurrent2ExtremeDiff(int diff);
	void SetMillionSecond(long ms);
	LONG GetMillionSecond(void);
	float GetQueryPriceUseTime(void);
	void SetQueryPriceUseTime(float time);
	float GetCapturePriceUseTime(void);
	void SetCapturePriceUseTime(float time);
};


typedef CDataK * CDataKP;