#pragma once


#define OWN_SERVER_SPLITER _T(":")

#define KEY_VALUE_DISTANCE 3
#define VALUE_TAIL _T("\"")

struct CDataPacket
{
	//�仯�ٷֱ�
	CString	mPercent;
	CString mPriceTime;//�۸�ʱ��
	CString mUpdateTime;//�۸����ʱ��
	CString mNickName;//��Ʒ����
	double	mPrice;//�۸�
	INT		mUpDrop;//�ǵ�
	INT		mChaseDirect;//׷������
	BOOL	mIsGood;//�����Ƿ���Ч
	LONG	mMillionSecond;
	float	mQueryPriceUseTime;//http�����ʱ
	float	mCapturePriceUseTime;//����ץͼ��ʱ


public:
	CDataPacket():mPriceTime(""),mUpdateTime(""),mNickName(""),mPrice(0.0),mUpDrop(0)
		,mChaseDirect(0),mIsGood(FALSE),mMillionSecond(0),mQueryPriceUseTime(0.0F),mCapturePriceUseTime(0.0)
	{
	}
};

typedef CDataPacket * CDataPacketP;