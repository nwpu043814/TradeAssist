#include "StdAfx.h"
#include "HttpThread.h"
#include "Constant.h"
#include "DataPacket.h"
#include "Util.h"

BEGIN_MESSAGE_MAP(CHttpThread, CWinThread)
	//{{AFX_MSG_MAP(CCalculateThread)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_DO_HTTP_GET,OnDoHttpGet)
END_MESSAGE_MAP()


CHttpThread::CHttpThread(void)
{
	mHttpWorker = new CHttpWorker();
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
	Sleep(SLEEP_TIME);

	if (!::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_HTTP_GET_FINISH,(WPARAM)packet,NULL))
	{
		delete packet;

		this->PostThreadMessage(WM_DO_HTTP_GET, NULL, NULL);
	}
}