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
	// ȫ���ǵ�ֵ
	int mIntDayUpDrop;
public:
	// �����ȡ����ȫ���ǵ�ֵ
	int GetDayUpDrop(void);
	// ����ȫ���ǵ�ֵ
	void SetDayUpDrop(int updrop);
private:
	// ���ͳ���ǵ����·��ǵ������Ƿ�ƥ�䡣
	bool mIsDirectionAgree;
public:
	// ���ͳ���ǵ����·��ǵ������Ƿ�ƥ�䡣
	bool IsDirectionAgree(void);
	// ���ͳ���ǵ����·��ǵ������Ƿ�ƥ�䡣
	void SetDirectionAgree(bool isAgree);
private:
	// �ص�����
	int mIntCurrent2ExtremeDiff;
public:
	int GetCurrent2ExtremeDiff(void);
	// ���ûص�����
	void SetCurrent2ExtremeDiff(int diff);
	void SetMillionSecond(long ms);
	LONG GetMillionSecond(void);
};


typedef CDataK * CDataKP;