#include "StdAfx.h"
#include "HttpThread.h"
#include "Constant.h"

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

LRESULT CHttpThread::OnDoHttpGet(WPARAM wParam,LPARAM lParam)
{

	CString result = mHttpWorker->DoGet(TEXT("jry.baidao.com"), 80, TEXT("/api/hq/npdata.do?ids=1&markettype=ttj"));
	CDataPacketP packet = new CDataPacket();
	//CString text = _T("[{\"percent\":\"-1.65%\",\"updatetime\":\"20130322 23:20:55\",\"nickname\":\"XAGUSD\",\"buy\":\"5794\",\"status\":true,\"updrop\":\"-97\",\"close\":\"5784\",\"id\":1,\"open\":\"5883\",\"sell\":\"5784\",\"now\":\"5784\",\"pricetime\":\"20130322 23:20:54\",\"name\":\"ÏÖ»õ°×Òø\",\"high\":\"5888\",\"low\":\"5757\",\"preclose\":\"5881\",\"avg\":\"5836.99\",\"amplitude\":\"2.23%\"}]");
	CUtil::ParseDataString(result,*packet);

	::PostMessage((HWND)(GetMainWnd()->GetSafeHwnd()),WM_HTTP_GET_FINISH,(WPARAM)packet,NULL);
 
	return 1;
}