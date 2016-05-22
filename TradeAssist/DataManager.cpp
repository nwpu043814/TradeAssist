#include "StdAfx.h"
#include "DataManager.h"
#include "Constant.h"
#include "TradeAssist.h "
#include "HttpBaidaoThread.h"
#include <process.h>
extern CTradeAssistApp theApp;
UINT	__stdcall HttpProcess(void * param);
CDataManager::CDataManager(void):mIsAutoSubmits(FALSE)
, mAutoCompleteInterval(_T(""))
, mStrHighPriceDiff(_T(""))
, mIntOrderCount(1)
, mIntMsgDelayMilliSeconds(1000)
, mIntHour(0)
, mIntMinute(0)
, mIntSecond(_T(""))
, mEnableCheckAutoCloseDepot(FALSE)
, mStrLowPriceDiff(_T(""))
, mDataKClose(0.0F)
, mDataKOpen(0.0F)
, mDataKCloseTime(_T(""))
, mDataKOpenTime(_T(""))
, mDataKHighPrice(0.0F)
, mDataKLowPrice(0.0F)
, mUintAutoCloseThreshold(0)
, mDataKStatisticsUpdrop(0)
, mDataKDayUpdrop(0)
, mDataKCurrent2ExtremeDiff(0)
, mOpenDirection(-1)
, mServerIp(_T("127.0.0.1"))
, mServerPort(80)
, mUintDoHttpInterval(0)
, mBoolEnableAutoThreshold(FALSE)
, mIsTimer4Tomorrow(false)
, mActualNonfarmerNumber(_T(""))
, mNonfarmerNumberResult(_T(""))
, mJoblessRateResult(_T(""))
, mNonfarmerNumnerWeight(_T(""))
, mJoblessRateWeight(_T(""))
, mNonfarmerNumberCount(_T(""))
, mJoblessRateCount(_T(""))
, mTotalConclution(_T(""))
, mPullPriceCount(0)
, mQueryPriceUseTime(0.0F)
, mCapturePriceUseTime(0)
, mUILastUpdateTime(0)
, mEnableWindowMostTop(0)
,mBaidaoHttpThread(NULL)
{

}

CDataManager::~CDataManager(void)
{
	StopBaidaoUpdateThread();
}

void CDataManager::Load(void)
{
	mStrHighPriceDiff.Format(_T("%d"), theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_DIFF, 10));
	mStrLowPriceDiff.Format(_T("%d"), theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_DIFF, 10));
	mIntOrderCount = theApp.GetProfileInt(STRING_SETTING, STRING_KEY_COUNT, 6);

	mIsAutoSubmits = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, FALSE);
	mAutoCompleteInterval = theApp.GetProfileString(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, NULL);
	mIntMsgDelayMilliSeconds = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_MSG_DELAY_TIME, 100);
	mIntHour = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_HOUR, 0);
	mIntMinute = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_MINUTE, 0);
	mIntSecond = theApp.GetProfileString(STRING_SETTING, STRING_EDIT_SECOND, NULL);
	mEnableCheckAutoCloseDepot = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_BOX_ENABLE_CHECK_AUTO_CLOSE_DEPOT, 0);
	mUintAutoCloseThreshold = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_AUTO_CLOSE_THRESHOLD, 20);

	if (mUintAutoCloseThreshold <THRESHOLD_MIN || mUintAutoCloseThreshold > THRESHOLD_MAX)
	{
		mUintAutoCloseThreshold = 10;
	}

	mServerIp = theApp.GetProfileString(STRING_SETTING, STRING_EDIT_SERVER_HOST, NULL);
	mServerPort = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_SERVER_PORT, 80);
	mUintDoHttpInterval = theApp.GetProfileInt(STRING_SETTING,STRING_EDIT_DO_HTTP_INTERVAL, 0) ;
	mBoolEnableAutoThreshold = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_ENABLE_AUTO_THRESHOLD, 1);
	mEnableWindowMostTop =  theApp.GetProfileInt(  STRING_SETTING, STRING_CHECK_ENABLE_MOST_TOP, 1);
}

void CDataManager::Save(void)
{
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_PRICE_DIFF , atoi(mStrHighPriceDiff));
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_PRICE_DIFF , atoi(mStrLowPriceDiff));
	theApp.WriteProfileInt(STRING_SETTING, STRING_KEY_COUNT , mIntOrderCount);

	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, mIsAutoSubmits);
	theApp.WriteProfileString(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, mAutoCompleteInterval);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_MSG_DELAY_TIME, mIntMsgDelayMilliSeconds);

	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_HOUR, mIntHour);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_MINUTE, mIntMinute);
	theApp.WriteProfileString(STRING_SETTING, STRING_EDIT_SECOND, mIntSecond);
	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_BOX_ENABLE_CHECK_AUTO_CLOSE_DEPOT, mEnableCheckAutoCloseDepot);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_AUTO_CLOSE_THRESHOLD, mUintAutoCloseThreshold);

	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_SERVER_PORT, mServerPort);
	theApp.WriteProfileString(STRING_SETTING, STRING_EDIT_SERVER_HOST, mServerIp);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_DO_HTTP_INTERVAL, mUintDoHttpInterval);
	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_ENABLE_AUTO_THRESHOLD, mBoolEnableAutoThreshold);
	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_ENABLE_MOST_TOP, mEnableWindowMostTop);
}

void CDataManager::initialJoblessRateThread(CLuaEngine * p)
{
	mJoblessRate	= new PEcnomicData[THREAD_NUMBER];
	mJoblessRate[0] = new CJoblessRateData();
	mJoblessRate[0]->SetUrl(p->GetJoblessUrl(0));
	mJoblessRate[0]->SetResult(EcnomicResult::TYPE_UNKOWN);
	mJoblessRate[0]->SetExpectValue(p->GetExpectValue(LUA_FUNCTION_GetExpectJoblessRate));
	mJoblessRate[0]->SetMsgType(WM_DO_HTTP_GET_ECNOMIC_DATA);

	for(int i = 1; i < THREAD_NUMBER; i++)
	{
		mJoblessRate[i] = new CJoblessRateData(*mJoblessRate[0]);
	}
}

void CDataManager::initialLocalPriceThread(void)
{
	mLocalPrice = new PEcnomicData[LOCAL_SERVER_REQUEST_THREADED_NUMBER];
	for (int i =0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
	{
		mLocalPrice[i] = new CLocalServerData();
		mLocalPrice[i]->SetUrl(HTTP_URL_LOCAL_SERVER);
		mLocalPrice[i]->SetResult(EcnomicResult::TYPE_UNKOWN);
		mLocalPrice[i]->SetExpectValue(_T(""));
		mLocalPrice[i]->SetMsgType(WM_DO_HTTP_GET_LOCAL_SERVER_PRICE);
	}
}

void CDataManager::initialNonfarmerThread(CLuaEngine * p)
{
	mNonfarmerNumber = new PEcnomicData[THREAD_NUMBER];
	mNonfarmerNumber[0] = new CNonfarmerNumberData();
	mNonfarmerNumber[0]->SetUrl( p->GetNonfarmerWorkerUrl(0));
	mNonfarmerNumber[0]->SetResult( EcnomicResult::TYPE_UNKOWN);
	mNonfarmerNumber[0]->SetExpectValue( p->GetExpectValue(LUA_FUNCTION_GetExpectNonfarmerWorker));
	mNonfarmerNumber[0]->SetMsgType(WM_DO_HTTP_GET_ECNOMIC_DATA);

	for(int i = 1; i < THREAD_NUMBER; i++)
	{
		mNonfarmerNumber[i] = new CNonfarmerNumberData(*mNonfarmerNumber[0]);
	}
}

void CDataManager::StartUpdateFeiNongData(void)
{

	for (int i =0 ; i < THREAD_NUMBER; i++)
	{
		if (mNonfarmerNumber[i]->IsEnable() && mNonfarmerNumber[i]->GetThreadId() == 0)
		{
			StartHttpThread(mNonfarmerNumber[i]);
		}
	}

	for (int i =0 ; i < THREAD_NUMBER; i++)
	{
		if (mJoblessRate[i]->IsEnable() && mJoblessRate[i]->GetThreadId() == 0)
		{
			StartHttpThread(mJoblessRate[i]);
		}
	}
}

void CDataManager::StartHttpThread(PEcnomicData data)
{
	if (data == NULL)
	{
		return;
	}

	//开启自动平仓检测。
	if(data->GetStartEvent() == NULL)
	{
		data->SetStartEvent(::CreateEvent(0,FALSE,FALSE,0));
	}

	if(data->GetStartEvent() == NULL)
	{
		return;
	}

	if (data->GetThreadHandle() == NULL)
	{
		UINT id;
		data->SetThreadHandle((HANDLE)_beginthreadex( NULL, NULL, &HttpProcess, data, NULL, &id ));
		data->SetThreadId(id);
	} 

	if(data->GetThreadHandle() == NULL)
	{
		TRACE("start thread failed,errno:%d\r\n",::GetLastError());
		CloseHandle(data->GetStartEvent());
		data->SetStartEvent(NULL);
		return ;
	}

	::WaitForSingleObject(data->GetStartEvent(),INFINITE);
	CloseHandle(data->GetStartEvent());
	data->SetStartEvent(NULL);

	int count = 0;
	while(count++ < SEND_MESSAGE_TO_THREAD_MAX_RETTY_TIMES)
	{

		if(!PostThreadMessage(data->GetThreadId(),data->GetMsgType(),(WPARAM)data, NULL))
		{
			TRACE("post message failed,errno:%d\r\n",::GetLastError());
		}
		else
		{
			break;
		}
		::Sleep(500);
	}
}

CHuifengGuadanParamP CDataManager::GetGuaDanParam(int dirct)
{
	CHuifengGuadanParamP param = new CHuifengGuadanParam();
	param->mLowDiff = atoi(mStrLowPriceDiff);
	param->mHighDiff = atoi(mStrHighPriceDiff);
	param->mTradeCount = mIntOrderCount;
	param->mWindowDelay = mIntMsgDelayMilliSeconds;
	param->mDirect = dirct;
	return param;
}

void CDataManager::StartUpdateBaidaoPrice()
{
	mBaidaoHttpThread = DYNAMIC_DOWNCAST(CHttpBaidaoThread, AfxBeginThread(RUNTIME_CLASS(CHttpBaidaoThread)));
	mBaidaoHttpThread->SetChaseHighThreshold(atoi(mStrHighPriceDiff)); 
	mBaidaoHttpThread->SetChaseLowThreshold(atoi(mStrLowPriceDiff)); 
	// 需要执行线程中的操作时
	mBaidaoHttpThread ->PostThreadMessage(WM_DO_HTTP_GET_BAIDAO_DATA,NULL,NULL);
}

void CDataManager::StopBaidaoUpdateThread(void)
{
	if (mBaidaoHttpThread != NULL)
	{
		// 结束线程
		HANDLE hp=mBaidaoHttpThread->m_hThread;
		if (hp) 
		{
			if (WaitForSingleObject(hp, 1) != WAIT_OBJECT_0)
			{
				TerminateThread(hp,0);
			}
			CloseHandle(hp);
		}
		mBaidaoHttpThread = NULL;
	}
}
