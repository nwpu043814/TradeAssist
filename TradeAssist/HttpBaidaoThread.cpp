#include "StdAfx.h"
#include "HttpBaidaoThread.h"
#include "Constant.h"
#include "DataPacket.h"
#include "Util.h"
#define  TIMER_CYCLE_TIMES 10

BEGIN_MESSAGE_MAP(CHttpBaidaoThread, CWinThread)
	//{{AFX_MSG_MAP(CCalculateThread)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_DO_HTTP_GET_BAIDAO_DATA,OnDoHttpGetTiantongPrice)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CHttpBaidaoThread, CWinThread)

CHttpBaidaoThread::CHttpBaidaoThread(void):mPriceData()
, mChaseHighThreshold(4)
, mChaseLowThreshold(4)
, mHttpRequestCounter(0)
, mRequestTimer(0)
, mLastQueryPriceUseTime(0)
,mHttpReturnString("")
{
	mHttpWorker = new CHttpWorker();
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
}

CHttpBaidaoThread::~CHttpBaidaoThread(void)
{
	delete mHttpWorker;
}

BOOL CHttpBaidaoThread::InitInstance()
{
	return TRUE;
}

int CHttpBaidaoThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


void CHttpBaidaoThread::OnDoHttpGetTiantongPrice(WPARAM wParam,LPARAM lParam)
{

	mHttpWorker->DoGet(TEXT(HTTP_URL_TIANTONG_HOST), 80, TEXT(HTTP_URL_TIANTONG_URI), mHttpReturnString);
	CDataPacketP packet = new CDataPacket();
	CUtil::ParseBaidaoApiDataString(mHttpReturnString,*packet);
	mPriceData.SetClose(packet->mPrice, packet->mPriceTime);

	if (0 == mRequestTimer)
	{
		//首次计时初始化
		mRequestTimer =  GetTickCount();
		++mHttpRequestCounter;
	}
	
	if (++mHttpRequestCounter == TIMER_CYCLE_TIMES)
	{
		mHttpRequestCounter = 0;
		packet->mQueryPriceUseTime = ( GetTickCount() - mRequestTimer)/TIMER_CYCLE_TIMES;
		mRequestTimer = GetTickCount();
		//程序段开始前取得系统运行时间(ms)
		mLastQueryPriceUseTime = packet->mQueryPriceUseTime;
	}
	else
	{
		packet->mQueryPriceUseTime = mLastQueryPriceUseTime;
	}

	if(mPriceData.GetClose() >= mPriceData.GetOpen() + mChaseHighThreshold)
	{
		//追多
		packet->mChaseDirect = DO_HIGH;
	}
	else if(mPriceData.GetClose() <= mPriceData.GetOpen() - mChaseLowThreshold)
	{
		//追空
		packet->mChaseDirect = DO_LOW;
	}
	else
	{
		//Sleep(SLEEP_TIME);
		this->PostThreadMessage(WM_DO_HTTP_GET_BAIDAO_DATA, NULL, NULL);
	}
	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_HTTP_GET_BAIDAO_PRICE_SUCCESS,(WPARAM)packet,NULL);
}

void CHttpBaidaoThread::SetChaseHighThreshold(int value)
{
	mChaseHighThreshold = value;
}

void CHttpBaidaoThread::SetChaseLowThreshold(int value)
{
	mChaseLowThreshold = value;
}
