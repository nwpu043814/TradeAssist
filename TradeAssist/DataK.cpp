#include "StdAfx.h"
#include "DataK.h"

CDataK::CDataK(void)
: mOpen(0)
, mClose(0)
, mHigh(0)
, mLow(0)
, mOpenTime(0)
, mCloseTime(0)
{
}

CDataK::~CDataK(void)
{
}

//
void CDataK::SetClose(UINT close, ULONGLONG time)
{
	mClose = close;
	InitialHighLow(close);
	UpdateRange(close);

	if (mOpenTime == 0)
	{
		mOpenTime = time;
	}

	mCloseTime = time;
}

UINT CDataK::GetHigh(void)
{
	return mHigh;
}

UINT CDataK::GetLow(void)
{
	return mLow;
}

ULONGLONG CDataK::GetOpenTime(void)
{
	return mOpenTime;
}

void CDataK::SetCloseTime(ULONGLONG closeTime)
{
	//initialize the open time
	if (mOpenTime == 0)
	{
		mOpenTime = closeTime;
	}

	mCloseTime = closeTime;
}

ULONGLONG CDataK::GetCloseTime(void)
{
	return mCloseTime;
}

// maintain  the high price and the low price.
void CDataK::UpdateRange(UINT latestPrice)
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
	return GetAmplitude();
}

BOOL CDataK::IsNegtive(void)
{
	return !IsPositive();
}

INT CDataK::GetAmplitude(void)
{
	return mClose - mOpen;
}

void CDataK::InitialHighLow(UINT price)
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

float CDataK::GetAmplitudeRate(void)
{
	return (GetAmplitude() + 0.000F)/mOpen;
}

UINT CDataK::GetClose(void)
{
	return mClose;
}
