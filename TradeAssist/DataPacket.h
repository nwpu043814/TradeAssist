#pragma once

#define KEY_NAME_UPDROP _T("updrop")
#define KEY_NAME_PERCENT _T("percent")
#define KEY_NAME_PRICETIME _T("pricetime")
#define KEY_NAME_UPDATETIME _T("updatetime")
#define KEY_NAME_NICKNAME _T("nickname")
#define KEY_NAME_NOW _T("now")

#define OWN_SERVER_SPLITER _T(":")

#define KEY_VALUE_DISTANCE 3
#define VALUE_TAIL _T("\"")

struct CDataPacket
{
	//变化百分比
	CString	mPercent;
	CString mPriceTime;
	CString mUpdateTime;
	CString mNickName;
	double	mPrice;
	INT		mUpDrop;
	INT		mChaseDirect;
	BOOL	mIsGood;
	LONG	mMillionSecond;
	float	mQueryPriceUseTime;
	float	mCapturePriceUseTime;
};

typedef CDataPacket * CDataPacketP;