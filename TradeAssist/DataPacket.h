#pragma once


#define OWN_SERVER_SPLITER _T(":")

#define KEY_VALUE_DISTANCE 3
#define VALUE_TAIL _T("\"")

struct CDataPacket
{
	//变化百分比
	CString	mPercent;
	CString mPriceTime;//价格时刻
	CString mUpdateTime;//价格更新时刻
	CString mNickName;//商品名称
	double	mPrice;//价格
	INT		mUpDrop;//涨跌
	INT		mChaseDirect;//追单方向
	BOOL	mIsGood;//数据是否有效
	LONG	mMillionSecond;
	float	mQueryPriceUseTime;//http请求耗时
	float	mCapturePriceUseTime;//本地抓图耗时


public:
	CDataPacket():mPriceTime(""),mUpdateTime(""),mNickName(""),mPrice(0.0),mUpDrop(0)
		,mChaseDirect(0),mIsGood(FALSE),mMillionSecond(0),mQueryPriceUseTime(0.0F),mCapturePriceUseTime(0.0)
	{
	}
};

typedef CDataPacket * CDataPacketP;