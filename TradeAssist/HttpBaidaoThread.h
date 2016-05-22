#pragma once
#include "HttpWorker.h"

#include "DataK.h"

#define  WM_DO_HTTP_GET_LOCAL_SERVER_PRICE	WM_USER + 200
#define  WM_DO_HTTP_EXIT WM_USER + 201
#define  WM_DO_HTTP_GET_ECNOMIC_DATA WM_USER + 202
#define  WM_DO_HTTP_GET_BAIDAO_DATA WM_USER + 203

//每次请求以后休息的时间
#define  SLEEP_TIME		0

class CHttpBaidaoThread :
	public CWinThread
{
public:
	CHttpBaidaoThread(void);
	~CHttpBaidaoThread(void);
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CHttpBaidaoThread)

public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	CDataK		mPriceData;
private:
	afx_msg void OnDoHttpGetTiantongPrice(WPARAM wParam,LPARAM lParam);

	CHttpWorkerP mHttpWorker;
	int mChaseHighThreshold;
	int mChaseLowThreshold;
public:
	void SetChaseHighThreshold(int value);
	void SetChaseLowThreshold(int value);
private:
	// http请求计数器
	int mHttpRequestCounter;
	// 请求计时
	long mRequestTimer;
	float mLastQueryPriceUseTime;
	CString mHttpReturnString;
};


typedef CHttpBaidaoThread *CHttpBaidaoThreadP;
