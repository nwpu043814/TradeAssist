#include "StdAfx.h"
#include "DataK.h"
#include "Util.h"

CDataK::CDataK(void)
: mOpen(0)
, mClose(0)
, mHigh(0)
, mLow(0)
, mOpenTime(_T(""))
, mCloseTime(_T(""))
, mIntCurrent2ExtremeDiff(0)
{
}

CDataK::~CDataK(void)
{
}

void CDataK::SetClose(double close, CString time)
{
	mClose = close;
	InitialHighLow(close);
	UpdateRange(close);

	if (mOpenTime.GetLength() == 0)
	{
		mOpenTime = time;
	}

	mCloseTime = time;
}

double CDataK::GetHigh(void)
{
	return mHigh;
}

double CDataK::GetLow(void)
{
	return mLow;
}

CString CDataK::GetOpenTime(void)
{
	return mOpenTime;
}

void CDataK::SetCloseTime(CString closeTime)
{
	//initialize the open time
	if (mOpenTime.GetLength() == 0)
	{
		mOpenTime = closeTime;
	}

	mCloseTime = closeTime;
}

CString CDataK::GetCloseTime(void)
{
	return mCloseTime;
}

// maintain  the high price and the low price.
void CDataK::UpdateRange(double latestPrice)
{
	//update the highest price
	if (latestPrice > mHigh)
	{
		mHigh = latestPrice;
	}

	if (latestPrice < mLow)
	{
		mLow = latestPrice;
	}
}

BOOL CDataK::IsPositive(void)
{
	return GetAmplitude() > 0;
}

BOOL CDataK::IsNegtive(void)
{
	return GetAmplitude() <  0;
}

double CDataK::GetAmplitude(void)
{
	return mClose - mOpen;
}

void CDataK::InitialHighLow(double price)
{
	if (mOpen == 0)
	{
		mOpen = price;
	}

	if (mHigh == 0)
	{
		mHigh = price;
	}

	if (mLow == 0)
	{
		mLow = price;
	}
}

double CDataK::GetAmplitudeRate(void)
{
	return (GetAmplitude() + 0.000F)/mOpen;
}

double CDataK::GetClose(void)
{
	return mClose;
}

double CDataK::GetOpen(void)
{
	return mOpen;
}

int CDataK::GetCurrent2ExtremeDiff(void)
{
	return mIntCurrent2ExtremeDiff;
}

// 设置回调幅度
void CDataK::SetCurrent2ExtremeDiff(int diff)
{
	mIntCurrent2ExtremeDiff = diff;
}

void CDataK::SetMillionSecond(long ms)
{
	mMillionSecond = ms;
}

LONG CDataK::GetMillionSecond(void)
{
	return mMillionSecond;
}

float CDataK::GetQueryPriceUseTime(void)
{
	return mQueryPriceUseTime;
}

void CDataK::SetQueryPriceUseTime(float time)
{
	mQueryPriceUseTime = time;
}

float CDataK::GetCapturePriceUseTime(void)
{
	return mCapturePriceUseTime;
}

void CDataK::SetCapturePriceUseTime(float time)
{
	mCapturePriceUseTime = time;
}
