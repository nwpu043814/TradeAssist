#pragma once

#define KEY_NAME_UPDROP _T("updrop")
#define KEY_NAME_PERCENT _T("percent")
#define KEY_NAME_PRICETIME _T("pricetime")
#define KEY_NAME_UPDATETIME _T("updatetime")
#define KEY_NAME_NICKNAME _T("nickname")
#define KEY_NAME_NOW _T("now")

#define KEY_VALUE_DISTANCE 3
#define VALUE_TAIL _T("\"")

struct CDataPacket
{
	//�仯�ٷֱ�
	CString	mPercent;
	CString mPriceTime;
	CString mUpdateTime;
	CString mNickName;
	INT		mPrice;
	INT		mUpDrop;
	BOOL	mIsGood;
};

typedef CDataPacket * CDataPacketP;