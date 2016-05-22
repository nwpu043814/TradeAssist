#include "StdAfx.h"
#include "HttpThread.h"
#include "Constant.h"
#include "DataPacket.h"
#include "Util.h"

BEGIN_MESSAGE_MAP(CHttpThread, CWinThread)
	//{{AFX_MSG_MAP(CCalculateThread)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_DO_HTTP_GET_PRICE,OnDoHttpGet)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CHttpThread, CWinThread)

CHttpThread::CHttpThread(void)
{
	mHttpWorker = new CHttpWorker();
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
}

CHttpThread::~CHttpThread(void)
{
	delete mHttpWorker;
}

void CHttpThread::OnDoHttpGet(WPARAM wParam,LPARAM lParam)
{

	CString result = mHttpWorker->DoGet(TEXT("jry.baidao.com"), 80, TEXT("/api/hq/npdata.do?ids=1&markettype=ttj"));
	CDataPacketP packet = new CDataPacket();
	CUtil::ParseDataString(result,*packet);
	
	if (!::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_HTTP_GET_FINISH,(WPARAM)packet,NULL))
	{
		

		this->PostThreadMessage(WM_DO_HTTP_GET_PRICE, NULL, NULL);
	}
	Sleep(SLEEP_TIME);
	delete packet;
}