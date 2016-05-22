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
, mIntDayUpDrop(0)
, mIsDirectionAgree(false)
, mIntCurrent2ExtremeDiff(0)
{
}

CDataK::~CDataK(void)
{
}

//
void CDataK::SetClose(UINT close, CString time)
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

UINT CDataK::GetHigh(void)
{
	return mHigh;
}

UINT CDataK::GetLow(void)
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
	return GetAmplitude() > 0;
}

BOOL CDataK::IsNegtive(void)
{
	return GetAmplitude() <  0;
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

UINT CDataK::GetOpen(void)
{
	return mOpen;
}

// �����ȡ����ȫ���ǵ�ֵ
int CDataK::GetDayUpDrop(void)
{
	return mIntDayUpDrop;
}

// ����ȫ���ǵ�ֵ
void CDataK::SetDayUpDrop(int updrop)
{
	mIntDayUpDrop = updrop;
}

// ���ͳ���ǵ����·��ǵ������Ƿ�ƥ�䡣
bool CDataK::IsDirectionAgree(void)
{
	return mIsDirectionAgree;
}

// ���ͳ���ǵ����·��ǵ������Ƿ�ƥ�䡣
void CDataK::SetDirectionAgree(bool isAgree)
{
	mIsDirectionAgree = isAgree;
}

int CDataK::GetCurrent2ExtremeDiff(void)
{
	return mIntCurrent2ExtremeDiff;
}

// ���ûص�����
void CDataK::SetCurrent2ExtremeDiff(int diff)
{
	mIntCurrent2ExtremeDiff = diff;
}
