#include "StdAfx.h"
#include "TradeAssistController.h"

CTradeAssistController::CTradeAssistController(CHotKeyManagerP hotkey,
											   CDataManagerP data,
											   CActionManagerP action,
											    CWnd* wnd,
												 CDataKP dataK) : CWnd(),mDataManager(data)
,mHotkeyManager(hotkey),mActionManager(action),mMainWnd(wnd)
{
	mDataK = dataK;
}

CTradeAssistController::~CTradeAssistController(void)
{

}



void CTradeAssistController::EnableWindowMostTop(BOOL isTop)
{
	if (isTop)
	{
		::SetWindowPos(mMainWnd->m_hWnd, HWND_TOPMOST, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	} 
	else
	{
		::SetWindowPos(mMainWnd->m_hWnd, HWND_NOTOPMOST, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

	CButton* pBtn =  (CButton*) (mMainWnd->GetDlgItem(IDC_CHECK_SET_MOST_TOP));
	pBtn->SetCheck(isTop?1:0);
}
void CTradeAssistController::SetMainWindow(CWnd* wnd)
{
	mMainWnd = wnd;
}
