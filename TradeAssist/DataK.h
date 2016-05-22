#pragma once

class CDataK
{
public:
	CDataK(void);
	virtual ~CDataK(void);
private:
	// open price
	UINT mOpen;
	// close price
	UINT mClose;
	// highest price
	UINT mHigh;
	// lowest price
	UINT mLow;
	ULONGLONG mOpenTime;
	ULONGLONG mCloseTime;
public:
	void SetClose(UINT close, ULONGLONG time);
	UINT GetHigh(void);
	UINT GetLow(void);
	ULONGLONG GetOpenTime(void);
	void SetCloseTime(ULONGLONG closeTime);
	ULONGLONG GetCloseTime(void);
	// maintain  the high price and the low price.
	void UpdateRange(UINT latestPrice);
	BOOL IsPositive(void);
	BOOL IsNegtive(void);
	INT GetAmplitude(void);
	void InitialHighLow(UINT price);
	float GetAmplitudeRate(void);
};


typedef CDataK * CDataKP;