#include "StdAfx.h"
#include "ActionManager.h"
#include "Logger.h"
#include "HuifengGuadanParam.h"
#include "TianTongGuaDan.h"
#include "LuoGeGuaDan.h"
#include "HaiJiaoGuaDan.h"
#include "KeYuGuaDan.h"
#include "ZhonXinGuaDan.h"
BEGIN_MESSAGE_MAP(CActionManager, CWinThread)
	ON_THREAD_MESSAGE(WM_DO_GUADAN,OnDoGuadan)
	ON_THREAD_MESSAGE(WM_DO_KUNJIAO_GUADAN,OnDoKunJiaoGuadan)
	ON_THREAD_MESSAGE(WM_DO_INPUT_PRICE,OnDoInputPrice) 
END_MESSAGE_MAP()
IMPLEMENT_DYNCREATE(CActionManager, CWinThread)

CActionManager::CActionManager()
{
	mAction = new CSimulateAction();
}

BOOL CActionManager::InitInstance()
{
	return TRUE;
}

void CActionManager::OnDoKunJiaoGuadan(WPARAM wParam,LPARAM lParam)
{
	if(wParam == NULL)
	{
		return;
	}

	CHuifengGuadanParamP param = (CHuifengGuadanParamP) wParam;
	DoHFDoubleSide(param->mLowDiff,param->mHighDiff, param->mTradeCount, param->mWindowDelay, param->mDirect, ON_TIMER_KUNJIAO);
	TRACE("OnDoKunJiaoGuadan, w=%d, l=%d\r\n", wParam, lParam);
	delete param;
	param = NULL;
}



void CActionManager::OnDoGuadan(WPARAM wParam,LPARAM lParam)
{
	if(wParam == NULL)
	{
		return;
	}
	CHuifengGuadanParamP param = (CHuifengGuadanParamP) wParam;
	CBaseGuaDan * guadan = NULL;
	
	switch (lParam)
	{
	case WM_DO_HAIJIAO_GUADAN:
		{
			guadan= new CHaiJiaoGuaDan(&mLuaEngine, mAction, mWndNewOwner);
		}
		break;
	case WM_DO_TIANTONG_GUADAN:
		{
			guadan= new CTianTongGuaDan(&mLuaEngine, mAction, mWndNewOwner);
		}
		break;
	case WM_DO_ZHONGXIN_GUADAN:
		{
			guadan= new CZhonXinGuaDan(&mLuaEngine, mAction, mWndNewOwner);
		}
		break;
	case WM_DO_KEYU_GUADAN:
		{
			guadan= new CKeYuGuaDan(&mLuaEngine, mAction, mWndNewOwner);
		}
		break;
	case WM_DO_LUOGE_GUADAN:
		{
			guadan= new CLuoGeGuaDan(&mLuaEngine, mAction, mWndNewOwner);
		}
		break;
	}
	
	if (guadan == NULL)
	{
		return;
	}

	guadan->DoDoubleSide(param->mLowDiff,param->mHighDiff, param->mTradeCount, param->mWindowDelay, param->mDirect);
	delete guadan;
	delete param;
	param = NULL;
}

CActionManager::~CActionManager(void)
{
	delete mAction;
}


CSimulateAction* CActionManager::GetAction()
{

	return mAction;
}


// 获得剪贴板的内容
CString CActionManager::GetContentFromClipboard(void) const
{
	char *buffer = NULL;

	CString fromClipboard;
	int i = 0;
	while (i++ < OPEN_CLIPBOARD_MAX_RETRY_TIMES)
	{
		if (OpenClipboard(mWndNewOwner))
		{
			HANDLE hData = GetClipboardData(CF_TEXT);  
			char * buffer = (char*)GlobalLock(hData);  
			fromClipboard = buffer;  
			GlobalUnlock(hData);  
			CloseClipboard();
			break;
		}
		else
		{
			TRACE("GetContentFromClipboard failed, error=%d\r\n", ::GetLastError());
		}
	}
	TRACE("GetContentFromClipboard i=%d, content=%s\r\n", i, fromClipboard);

	return fromClipboard;
}

// 设置剪贴板的内容
BOOL CActionManager::SetClipboardContent(CString source) const
{
	int i = 0;
	while (i++ < OPEN_CLIPBOARD_MAX_RETRY_TIMES)
	{
		if( OpenClipboard(mWndNewOwner) ) 
		{  
			HGLOBAL clipbuffer; 
			TCHAR * buffer;  
			EmptyClipboard(); 
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
			buffer = (TCHAR*)GlobalLock(clipbuffer); 
			strcpy_s(buffer, source.GetLength()+1,  LPCTSTR(source));
			GlobalUnlock(clipbuffer); 
			SetClipboardData(CF_TEXT,clipbuffer); 
			CloseClipboard();

			return 1;
		}
		else
		{
			TRACE("SetClipboardContent failed, error=%d\r\n", ::GetLastError());
		}
	}

	TRACE("SetClipboardContent i=%d\r\n", i);

	return 0;
}


CString CActionManager::GetEditText(UINT checkLen)
{
	CString text;

	SetClipboardContent(_T(""));
	int copyCount = 0;
	while(copyCount++  < GET_EDIT_CONTENT_MAX_TIMES)
	{


#if DEBUG_SLEEP
		Sleep(DEBUG_SLEEP_INTERVAL);
#endif

		mAction->SelectAll();
		mAction->KeyboardCopy();
		Sleep(copyCount*GET_CLIPBOARD_CONTENT_DELAY);
		text = GetContentFromClipboard();
		text = text.Trim();
		if (text.GetLength() < checkLen)
		{
			TRACE("invalid GetEditText=%s\r\n", text);
			continue;			
		}
		else
		{
			break;
		}
	}

	return text;
}

WORD CActionManager::GetMilliseconds(void)
{

	SYSTEMTIME time;
	GetSystemTime(&time);
	return time.wSecond * 1000 + time.wMilliseconds;
}

int CActionManager::UpdatePrice(bool isAdd, float diff)
{
	//取得当前价格。
	CString text = GetEditText();
	if(text.Find(_T("."))  == -1)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE;
	}

	CString outText;

	double newCount = isAdd?atof(text) + diff : atof(text) - diff ;
	outText.Format("%.2f",newCount);

	//设置剪贴板内容并粘贴到窗口
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	return 0;
}

BOOL CActionManager::CheckEditPasteResult(const CString & mLastClipboardContent)
{

#ifdef _DEBUG
	WORD	start = GetMilliseconds();
#endif // _DEBUG

	BOOL	result = TRUE;

	int priceCheckCount = 0;
	while(priceCheckCount++ < CHECK_EDIT_PASTE_RESULT_MAX_TIMES)
	{
		CString toCheckPrice = GetEditText();

		if (toCheckPrice.Compare(mLastClipboardContent) == 0)
		{
			//通过检查，设置正确。
			result = TRUE;
			break;
		}
		else
		{
#ifdef _DEBUG
			CString log;
			log.Format(_T("CheckEditPasteResult failed expect=%s, actually=%s"),mLastClipboardContent, toCheckPrice);
			CLogger::Add(log);
			TRACE(log+_T("\r\n"));
#endif

			//未设置正确，需要重新设置
			SetClipboardContent(mLastClipboardContent);
			mAction->SelectAll();
			mAction->KeyboardPaste();
			result = FALSE;
		}
#if DEBUG_SLEEP
		Sleep(DEBUG_SLEEP_INTERVAL);
#endif
	}

#ifdef _DEBUG
	TRACE("CheckEditPasteResult time=%d, checkCount=%d\r\n", GetMilliseconds() - start, priceCheckCount);
#endif

	return result;
}

BOOL CActionManager::MakeOrder(const CPoint & start2Button,
							   const CPoint & start2Count,const CPoint & count2Button,
							   UINT count, BOOL isDirectly)
{
	WORD start = GetMilliseconds();

	if (start2Button.x == 0 ||  start2Button.y == 0)
	{
		return 1;
	}

	POINT dialogPos = GetSunAwtDialogPos(_T("订单窗口"));
	if (dialogPos.x != 0 && dialogPos.y != 0)
	{

		TRACE("MakeOrder: useTime1=%u\r\n", GetMilliseconds() - start);

		mAction->MoveCursor(dialogPos.x, dialogPos.y, true);
		if (isDirectly)
		{
			mAction->MoveCursor(start2Button.x, start2Button.y);
			mAction->MouseClick(0);

			TRACE("MakeOrder: useTime2.1=%u\r\n", GetMilliseconds() - start);

		} 
		else
		{
			const int min_valid_len = 1;
			int i = 0; 
			CString out,toCheck;
			out.Format(_T("%u"),count);
			mAction->MoveCursor(start2Count.x, start2Count.y);
			
			do 
			{
				mAction->MouseClick(0);
				SetClipboardContent(out);
				mAction->SelectAll();
				mAction->KeyboardPaste();
				toCheck = GetEditText(out.GetLength());
			} while(i++ < CHECK_EDIT_PASTE_RESULT_MAX_TIMES && toCheck != out);

			TRACE("MakeOrder: useTime2.2=%u, i=%d\r\n", GetMilliseconds() - start, i);

			if (toCheck == out || toCheck.Trim().GetLength() == 0)
			{
				mAction->MoveCursor(start2Button.x - start2Count.x, start2Button.y - start2Count.y);
				mAction->MouseClick(0);
				TRACE("MakeOrder: useTime2.3=%u\r\n", GetMilliseconds() - start);

			}
		}

		TRACE("MakeOrder: useTime3=%u\r\n", GetMilliseconds() - start);	
	}
 
	return 0;
}


// 获得sun对话框右上角的绝对坐标。
POINT CActionManager::GetSunAwtDialogPos(CString title)
{
	POINT pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(SUN_DIALOG_NAME,title);
		if (wnd)
		{
			CRect rect;
			::GetWindowRect(wnd,rect);
			pos.x = rect.left;
			pos.y = rect.top;

			break;;
		} 
		Sleep(WINDOW_CHECK_INTERVAL);
	}

#ifdef _DEBUG
	TRACE("GetSunAwtDialogPos count=%d x=%d, y=%d\r\n", searchCount, pos.x, pos.y);
#endif // _DEBUG

	return pos;
}

//direct : 0表示两边都做 1 表示做空 2 表示做多
int CActionManager::DoHFDoubleSide(int lowDiff, int highDiff, int count, int windowDelay,  int direct, int softwareType)
{
	POINT lpPoint;
	GetCursorPos(&lpPoint);
	
	if(direct == DO_BOTH || direct == DO_LOW)
	{
		mAction->MouseDoubleClick();
	
		CLogger::Add("空");
		int result = DoHFSingleSide(lowDiff, DO_LOW, count, windowDelay, softwareType);
		for (int retry = 0; result == SEMIC_AUTO_TRADE_CALL_FAILED && softwareType == ON_TIMER_TIANTONG && retry < mLuaEngine.GetTianTongRetryTimes(); retry++)
		{
			Sleep(200);
			mAction->MoveCursor(lpPoint.x, lpPoint.y, true);
			mAction->MouseDoubleClick();
			result = DoHFSingleSide(lowDiff, DO_LOW, count, windowDelay, softwareType);
		}
	}

	if (direct == DO_BOTH)
	{
		Sleep(windowDelay);
	}
	
	if(direct == DO_BOTH || direct == DO_HIGH)
	{

		mAction->MoveCursor(lpPoint.x, lpPoint.y, true);
		mAction->MouseDoubleClick();
		
		CLogger::Add("多");
		int result = DoHFSingleSide(lowDiff, DO_HIGH, count, windowDelay, softwareType);
		for (int retry = 0; result == SEMIC_AUTO_TRADE_CALL_FAILED && softwareType == ON_TIMER_TIANTONG && retry < mLuaEngine.GetTianTongRetryTimes(); retry++)
		{
			Sleep(200);
			mAction->MoveCursor(lpPoint.x, lpPoint.y, true);
			mAction->MouseDoubleClick();
			result = DoHFSingleSide(lowDiff, DO_HIGH, count, windowDelay, softwareType);
		}
	}

	return 0;
}

// 1 for low 2 for high
int CActionManager::DoHFSingleSide(int diff, int direct, int count, int windowDelay, int softwareType)
{

	//1.当前位置双击弹出下单对话框。
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd= NULL;
		
		if (softwareType == ON_TIMER_KUNJIAO)
		{
			wnd = ::FindWindow(NULL,KUNJIAO_CONFIRM_DIALOG_TITLE_NAME );
		} 
		else
		{
			wnd = ::FindWindow(NULL,HUIFENG_DIALOG_TITLE_NAME );
		}
		
		if (wnd)
		{

			CString log;
			log.Format(_T("DoHFSingleSide direct=%d, dialogSearchCount=%d"), direct, searchCount);
			CLogger::Add(log);

			if (ON_TIMER_KUNJIAO == softwareType)
			{
				if (DoKunJiaoSingleSideAction(diff, direct, count, windowDelay) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
				{
					return SEMIC_AUTO_TRADE_CALL_FAILED;
				}
				else
				{
					return SEMIC_AUTO_TRADE_CALL_SUCCESS;
				}
			}
		} 
		Sleep(WINDOW_CHECK_INTERVAL*searchCount);
	}

	return SEMIC_AUTO_TRADE_CALL_FAILED;
}

const CPoint & CActionManager::GetDialogPosByTitle(CString title, UINT retryTime) const
{
	CPoint pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < retryTime)
	{
		HWND wnd=::FindWindow(NULL, title);
		if (wnd)
		{
			CRect rect;
			::GetWindowRect(wnd,rect);
			pos.x = rect.left;
			pos.y = rect.top;

			break;;
		} 
		Sleep(WINDOW_CHECK_INTERVAL*searchCount);
	}

#ifdef _DEBUG
	TRACE("GetHFDialogPos count=%d x=%d, y=%d\r\n", searchCount, pos.x, pos.y);
#endif // _DEBUG

	return pos;
}

void CActionManager::DoHop(int x, int y)  const
{
	mAction->MoveCursor(x, y);
	mAction->MouseClick();
}

const CPoint& CActionManager::GetHFConfirmDialogPos(void) const
{
	CPoint pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow("WindowsForms10.Window.8.app.0.33c0d9d", HUIFENG_CONFIRM_DIALOG_TITLE_NAME);
		if (wnd)
		{
			CRect rect;
			::GetWindowRect(wnd,rect);
			pos.x = rect.left;
			pos.y = rect.top;

			break;;
		} 
		Sleep(WINDOW_CHECK_INTERVAL* 2*searchCount);
	}

#ifdef _DEBUG
	TRACE("GetHFConfirmDialogPos count=%d x=%d, y=%d\r\n", searchCount, pos.x, pos.y);
#endif // _DEBUG

	return pos;
}

void CActionManager::CloseHFConfirmDialog(int left, int top)
{
	CPoint pos = mLuaEngine.GetHFConfirDialogOK();

	mAction->MoveCursor(left+pos.x, top + pos.y,true);
	mAction->MouseClick();
}

void CActionManager::SetWindowOwner(HWND owner)
{
	mWndNewOwner = owner;
}

CPoint CActionManager::GetKunJiaoDialogPos(void)
{
	CPoint pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(NULL, KUNJIAO_CONFIRM_DIALOG_TITLE_NAME);
		if (wnd)
		{
			CRect rect;
			::GetWindowRect(wnd,rect);
			pos.x = rect.left;
			pos.y = rect.top;

			break;;
		} 
		Sleep(WINDOW_CHECK_INTERVAL*searchCount);
	}

#ifdef _DEBUG
	TRACE("GetKunJiaoDialogPos count=%d x=%d, y=%d\r\n", searchCount, pos.x, pos.y);
#endif // _DEBUG

	return pos;
}


int CActionManager::DoKunJiaoSingleSideAction(int diff, int direct, int count, int windowDelay)
{
	CPoint pos = GetKunJiaoDialogPos();
	int time = mLuaEngine.GetDebugSleepInterval();
	if (pos.x + pos.y == 0)
	{
		return  DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}
	
	mAction->MoveCursor(pos.x, pos.y, true);
	DoHop(729,158);//下拉列表
	Sleep(time);
	Sleep(20);
	DoHop(0,28);//选中委托单
	Sleep(time);
	mAction->MoveCursor(0,-28);
	Sleep(time);
	DoHop(-180,53);//止损类型下拉列表
	Sleep(time);
	Sleep(20);

	if (direct == DO_LOW)
	{
		DoHop(0,56);
		Sleep(time);
		mAction->MoveCursor(0,-56);
	} 
	else
	{
		DoHop(0,42);
		Sleep(time);
		mAction->MoveCursor(0,-42);
	}
	Sleep(time);
	mAction->MoveCursor(0,25);//价位上箭头

	//交易手数输入框
	DoHop(-70,-160);
	Sleep(time);
	CString buffer;
	buffer.Format(_T("%d"), count);
	SetClipboardContent(buffer);
	mAction->KeyboardPaste();
	Sleep(time);
	mAction->MoveCursor(70,160);
	Sleep(time);

	//价位上箭头
	if (direct == DO_LOW)
	{
		DoHop(0,10);
		Sleep(time);
		mAction->ContinuesClick(21+diff);
		Sleep(time);
		mAction->MoveCursor(0,-10);
	} 
	else
	{
		mAction->ContinuesClick(31+diff);
	}
	Sleep(time);

	//止损价
	if (direct == DO_LOW)
	{
		mAction->MoveCursor(-38, -139);
		Sleep(time);
		mAction->ContinuesClick(1+diff);
	} 
	else
	{
		mAction->MoveCursor(-38, -139);
		Sleep(time);
		mAction->ContinuesClick(10+diff);
	}
	
	//价位上箭头
	mAction->MoveCursor(38, 139);
	Sleep(time);

	//止盈上箭头
	int stopGainDiff = mLuaEngine.GetHuiFengStopGainDiff(direct); 
	if (direct == DO_HIGH)
	{
		mAction->MoveCursor(179, -138);
		Sleep(time);
		mAction->ContinuesClick(stopGainDiff+30+diff);
		mAction->MoveCursor(-179, 138);
	} 
	else
	{
		mAction->MoveCursor(179, -130);
		Sleep(time);
		mAction->ContinuesClick(stopGainDiff+20+diff);
		mAction->MoveCursor(-179, 130);
	}
	Sleep(time);
	DoHop(100,0);
	Sleep(200);
	DoHop(-222,72);
	
	CPoint confirmDlgPos = GetDialogPosByTitle(KUNJIAO_CONFIRM_TITLE_NAME);
	if (confirmDlgPos.x + confirmDlgPos.y != 0)
	{
		mAction->MoveCursor(confirmDlgPos.x, confirmDlgPos.y, true);
		DoHop(108,97);
	}

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}


void CActionManager::OnDoInputPrice(WPARAM wParam,LPARAM lParam)
{
	Sleep(100);
	mAction->InputPrice(_T("5.8"));
}