#include "StdAfx.h"
#include "BaseGuaDan.h"
#include "Logger.h"

CBaseGuaDan::CBaseGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner):mLuaEngine(lua), mAction(action),mWndNewOwner(wndNewOwner)
{
}

CBaseGuaDan::~CBaseGuaDan(void)
{
}

CPoint CBaseGuaDan::GetGuaDanDialogPos(void)
{
	return CPoint();
}

int CBaseGuaDan::DoSingleSideAction(int diff, int direct, int count, int windowDelay)
{
	return 0;
}

void CBaseGuaDan::DoHop(int x, int y)  const
{
	mAction->MoveCursor(x, y);
	mAction->MouseClick();
}

int CBaseGuaDan::DoDoubleSide(int lowDiff, int highDiff, int count, int windowDelay, int direct)
{
	POINT lpPoint;
	GetCursorPos(&lpPoint);

	if(direct == DO_BOTH || direct == DO_LOW)
	{
		mAction->MouseDoubleClick();

		CLogger::Add("空");
		int result = DoSingleSideAction(lowDiff, DO_LOW, count, windowDelay);
		for (int retry = 0; result == SEMIC_AUTO_TRADE_CALL_FAILED &&  retry < mLuaEngine->GetTianTongRetryTimes(); retry++)
		{
			Sleep(200);
			mAction->MoveCursor(lpPoint.x, lpPoint.y, true);
			mAction->MouseDoubleClick();
			result = DoSingleSideAction(lowDiff, DO_LOW, count, windowDelay);
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
		int result = DoSingleSideAction(highDiff, DO_HIGH, count, windowDelay);
		for (int retry = 0; result == SEMIC_AUTO_TRADE_CALL_FAILED && retry < mLuaEngine->GetTianTongRetryTimes(); retry++)
		{
			Sleep(200);
			mAction->MoveCursor(lpPoint.x, lpPoint.y, true);
			mAction->MouseDoubleClick();
			result = DoSingleSideAction(highDiff, DO_HIGH, count, windowDelay);
		}
	}

	return 0;
}

int CBaseGuaDan::DoSingleSide(int diff, int direct, int count, int windowDelay)
{
	//1.当前位置双击弹出下单对话框。
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd= NULL;
		CPoint	dlgPos = GetGuaDanDialogPos();
		if (dlgPos.x + dlgPos.y != 0)
		{
			if (DoSingleSideAction(diff, direct, count, windowDelay) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
			{
				return SEMIC_AUTO_TRADE_CALL_FAILED;
			}
			else
			{
				return SEMIC_AUTO_TRADE_CALL_SUCCESS;
			}
		} 
		Sleep(WINDOW_CHECK_INTERVAL*searchCount);
	}
	return 0;
}

CPoint CBaseGuaDan::GetDialogPosByTitle(CString title ,LPCTSTR className,  UINT retryTime)
{
	CPoint pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < retryTime)
	{
		HWND wnd=::FindWindow(className, title);
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


BOOL CBaseGuaDan::CheckEditPasteResult(const CString & mLastClipboardContent)
{

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

	return result;
}

CString CBaseGuaDan::GetEditText(UINT checkLen)
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

BOOL CBaseGuaDan::SetClipboardContent(CString source) const
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

// 获得剪贴板的内容
CString CBaseGuaDan::GetContentFromClipboard(void) const
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