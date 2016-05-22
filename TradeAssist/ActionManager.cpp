#include "StdAfx.h"
#include "ActionManager.h"
#include "Logger.h"

CActionManager::CActionManager(__in HWND hWndNewOwner)
{
	mAction = new CSimulateAction();
	mWndNewOwner = hWndNewOwner;
}

CActionManager::~CActionManager(void)
{
	delete mAction;
}


CSimulateAction* CActionManager::GetAction()
{

	return mAction;
}


// ��ü����������
CString CActionManager::GetContentFromClipboard(void)
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

// ���ü����������
BOOL CActionManager::SetClipboardContent(CString source)
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

int CActionManager::DoTrade(const POINT &dialogPos,const POINT &	direction2Price,const CPoint & start2Tab,const POINT & tab2Direction, const POINT & price2CountVector,const CPoint & count2Confirm, double highDiff, double lowDiff, CString & mLastClipboardContent, BOOL direction,
							int mIntOrderCount, BOOL mIsAutoSubmits)
{
	if (dialogPos.x == 0 && dialogPos.y == 0)
	{
		CLogger::Add(_T("not find sunawtdialog"));
		TRACE("not find sunawtdialog, return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG\r\n");
		//δ���ҵ�sun�Ի���
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}


	mAction->MoveCursor(dialogPos.x,dialogPos.y, true);
	TRACE("dialogPos.x=%d, dialogPos.y=%d\r\n", dialogPos.x, dialogPos.y);
#if DEBUG_SLEEP
	Sleep(DEBUG_SLEEP_INTERVAL);
#endif

	//��ԭ���ƶ���ָ��ί��tab��
	mAction->MoveCursor(start2Tab.x,start2Tab.y);
	TRACE("start2Tab.x=%d, start2Tab.y=%d\r\n", start2Tab.x, start2Tab.y);
	mAction->MouseClick();

	//ָ��ί�е�����
	mAction->MoveCursor(tab2Direction.x,tab2Direction.y);
	mAction->MouseClick();

	//�ӷ����Ƶ��۸�ؼ�
	mAction->MoveCursor(direction2Price.x,direction2Price.y);
	TRACE("direction2Price.x=%d, direction2Price.y=%d\r\n", direction2Price.x, direction2Price.y);

	//���Ԥ�Ƶĵ��
	CString outText;

	//ȡ�õ�ǰ�۸�
	CString text = GetEditText();
	if(text.Find(_T("."))  == -1)
	{
		TRACE("text.Find(_T(\".\"))== -1, text=%s, return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE\r\n", text);
		return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE;
	}

	double newCount = direction?atof(text) + highDiff : atof(text) - lowDiff ;
	outText.Format(_T("%.2f"),newCount);

	CString log;
	log.Format(_T("originalprice=%s, hiDiff=%f, lowDiff=%f newPrice=%s, direction=%d"), text, highDiff, lowDiff ,outText, direction);
	CLogger::Add(log);
	TRACE("%s\r\n",log);

	//�����Ա����
	mLastClipboardContent = outText;

	//���ü��������ݲ�ճ��������
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();

	if(!CheckEditPasteResult(mLastClipboardContent))
	{
		TRACE("CheckEditPasteResult , return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED\r\n");
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	//�ƶ������������Ŀؼ�
	mAction->MoveCursor(price2CountVector.x,price2CountVector.y);
	TRACE("price2CountVector.x=%d, price2CountVector.y=%d\r\n", price2CountVector.x, price2CountVector.y);

	//���½�������
	mAction->MouseDoubleClick();
	outText.Format(_T("%d"), mIntOrderCount);
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	//�ƶ���ȷ����ť��
	mAction->MoveCursor(count2Confirm.x,count2Confirm.y);
	TRACE("count2Confirm.x=%d, count2Confirm.y=%d\r\n", count2Confirm.x, count2Confirm.y);

	//�Զ��ύ
	if(mIsAutoSubmits)
	{
		mAction->MouseClick();
	}


	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}

LRESULT CActionManager::OnDoTradeMsg(UINT w , UINT l)
{
	return LRESULT();
}

int CActionManager::UpdatePrice(bool isAdd, float diff)
{
	//ȡ�õ�ǰ�۸�
	CString text = GetEditText();
	if(text.Find(_T("."))  == -1)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE;
	}

	CString outText;

	double newCount = isAdd?atof(text) + diff : atof(text) - diff ;
	outText.Format("%.2f",newCount);

	//���ü��������ݲ�ճ��������
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	return 0;
}

LRESULT CActionManager::SemicAutoTrade(int direct) 
{

	//1.��ǰλ��˫�������µ��Ի���
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(SUN_DIALOG_NAME,NULL);
		if (wnd)
		{

			CString log;
			log.Format(_T("SemicAutoTrade direct=%d, dialogSearchCount=%d"), direct, searchCount);
			CLogger::Add(log);

			if (OnDoTradeMsg(direct, MSG_DELAY_YES) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
			{
				return SEMIC_AUTO_TRADE_CALL_FAILED;
			}
			else
			{
				return SEMIC_AUTO_TRADE_CALL_SUCCESS;
			}
		} 
		Sleep(WINDOW_CHECK_INTERVAL);
	}

	return SEMIC_AUTO_TRADE_CALL_FAILED;
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
			//ͨ����飬������ȷ��
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

			//δ������ȷ����Ҫ��������
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

	POINT dialogPos = GetSunAwtDialogPos();
	if (dialogPos.x != 0 && dialogPos.y != 0)
	{

		TRACE("MakeOrder: useTime1=%u\r\n", GetMilliseconds() - start);

		mAction->MoveCursor(dialogPos.x, dialogPos.y, true);
		if (isDirectly)
		{
			mAction->MoveCursor(start2Button.x, start2Button.y);
			mAction->MouseClick();

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
				mAction->MouseClick();
				SetClipboardContent(out);
				mAction->SelectAll();
				mAction->KeyboardPaste();
				toCheck = GetEditText(out.GetLength());
			} while(i++ < CHECK_EDIT_PASTE_RESULT_MAX_TIMES && toCheck != out);

			TRACE("MakeOrder: useTime2.2=%u, i=%d\r\n", GetMilliseconds() - start, i);

			if (toCheck == out || toCheck.Trim().GetLength() == 0)
			{
				mAction->MoveCursor(start2Button.x - start2Count.x, start2Button.y - start2Count.y);
				mAction->MouseClick();
				TRACE("MakeOrder: useTime2.3=%u\r\n", GetMilliseconds() - start);

			}
		}

		TRACE("MakeOrder: useTime3=%u\r\n", GetMilliseconds() - start);	
	}
 
	return 0;
}


// ���sun�Ի������Ͻǵľ������ꡣ
POINT CActionManager::GetSunAwtDialogPos(void)
{
	POINT pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(SUN_DIALOG_NAME,NULL);
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
