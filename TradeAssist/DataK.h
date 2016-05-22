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
	CString mOpenTime;
	CString mCloseTime;
	LONG mMillionSecond;
public:
	void SetClose(UINT close, CString time);
	UINT GetHigh(void);
	UINT GetLow(void);
	CString GetOpenTime(void);
	void SetCloseTime(CString closeTime);
	CString GetCloseTime(void);
	// maintain  the high price and the low price.
	void UpdateRange(UINT latestPrice);
	BOOL IsPositive(void);
	BOOL IsNegtive(void);
	INT GetAmplitude(void);
	void InitialHighLow(UINT price);
	float GetAmplitudeRate(void);
	UINT GetClose(void);
	UINT GetOpen(void);
private:
	// 全天涨跌值
	int mIntDayUpDrop;
public:
	// 获得拉取到的全天涨跌值
	int GetDayUpDrop(void);
	// 设置全天涨跌值
	void SetDayUpDrop(int updrop);
private:
	// 标记统计涨跌和下发涨跌方向是否匹配。
	bool mIsDirectionAgree;
public:
	// 标记统计涨跌和下发涨跌方向是否匹配。
	bool IsDirectionAgree(void);
	// 标记统计涨跌和下发涨跌方向是否匹配。
	void SetDirectionAgree(bool isAgree);
private:
	// 回调幅度
	int mIntCurrent2ExtremeDiff;
public:
	int GetCurrent2ExtremeDiff(void);
	// 设置回调幅度
	void SetCurrent2ExtremeDiff(int diff);
	void SetMillionSecond(long ms);
	LONG GetMillionSecond(void);
};


typedef CDataK * CDataKP;