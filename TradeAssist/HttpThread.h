#pragma once
#include "afxwin.h"
#include "CHttpWorker.h"

#define  WM_DO_HTTP_GET	WM_USER + 200

//每次请求以后休息的时间
#define  SLEEP_TIME		3000

class CHttpThread :
	public CWinThread
{
public:
	CHttpThread(void);
	~CHttpThread(void);
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnDoHttpGet(WPARAM wParam,LPARAM lParam);

	CHttpWorkerP mHttpWorker;
};


typedef CHttpThread *CHttpThreadP;
