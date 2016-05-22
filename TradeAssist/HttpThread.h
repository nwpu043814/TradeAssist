#pragma once
#include "afxwin.h"
#include "CHttpWorker.h"

#define  WM_DO_HTTP_GET	WM_USER + 200

class CHttpThread :
	public CWinThread
{
public:
	CHttpThread(void);
	~CHttpThread(void);

protected:
	afx_msg LRESULT OnDoHttpGet(WPARAM wParam,LPARAM lParam);

	CHttpWorkerP mHttpWorker;
};
