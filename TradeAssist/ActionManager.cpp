#include "StdAfx.h"
#include "ActionManager.h"
#include "Logger.h"
#include "HuifengGuadanParam.h"

BEGIN_MESSAGE_MAP(CActionManager, CWinThread)
	ON_THREAD_MESSAGE(WM_DO_HUIFENG_GUADAN,OnDoHuifengGuadan)
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

void CActionManager::OnDoHuifengGuadan(WPARAM wParam,LPARAM lParam)
{
	if(wParam == NULL)
	{
		return;
	}

	CHuifengGuadanParamP param = (CHuifengGuadanParamP) wParam;
	DoHFDoubleSide(param->mLowDiff,param->mHighDiff, param->mTradeCount, param->mWindowDelay, param->mDirect);
	TRACE("OnDoHuifengGuadan, w=%d, l=%d\r\n", wParam, lParam);
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


// ��ü����������
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

// ���ü����������
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

int CActionManager::DoTrade(const POINT &dialogPos, double highDiff, double lowDiff, BOOL isHigh,
							int mIntOrderCount)
{
	if (dialogPos.x == 0 && dialogPos.y == 0)
	{
		CLogger::Add(_T("not find sunawtdialog"));
		TRACE("not find sunawtdialog, return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG\r\n");
		//δ���ҵ�sun�Ի���
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}

	int sleepTime = mLuaEngine.GetDebugSleepInterval();
	TRACE("sleepTime=%d\r\n", sleepTime);
	mAction->MoveCursor(dialogPos.x,dialogPos.y, true);
	TRACE("dialogPos.x=%d, dialogPos.y=%d\r\n", dialogPos.x, dialogPos.y);
	
	//��ԭ���ƶ���ָ��ί��tab��
	CPoint p = mLuaEngine.getOrigin2Entrust();
	mAction->MoveCursor(p.x,p.y);
	TRACE("start2Tab.x=%d, start2Tab.y=%d\r\n", p.x, p.y);
	mAction->MouseClick();
	Sleep(sleepTime);
	//ָ��ί�е�����
	p = mLuaEngine.getEntrust2Direction(isHigh);
	mAction->MoveCursor(p.x,p.y);
	mAction->MouseClick();
	Sleep(sleepTime);
	
	//�ӷ����Ƶ��۸�ؼ�
	p = mLuaEngine.getDirection2Price(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("direction2Price.x=%d, direction2Price.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//���Ԥ�Ƶĵ��
	CString outText;

	//ȡ�õ�ǰ�۸�
	CString text = GetEditText();

	double newCount = isHigh==DO_HIGH?atof(text) + highDiff : atof(text) - lowDiff ;
	outText.Format(_T("%.2f"),newCount);

	CString log;
	log.Format(_T("originalprice=%s, hiDiff=%f, lowDiff=%f newPrice=%s, direction=%d"), text, highDiff, lowDiff ,outText, isHigh);
	CLogger::Add(log);
	TRACE("%s\r\n",log);

	//�����Ա����
	CString mLastClipboardContent = outText;

	//���ü��������ݲ�ճ��������
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();
	Sleep(sleepTime);
	if(!CheckEditPasteResult(mLastClipboardContent))
	{
		TRACE("CheckEditPasteResult , return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED\r\n");
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	//�ƶ������������Ŀؼ�
	p = mLuaEngine.getPrice2Count(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("price2CountVector.x=%d, price2CountVector.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//���½�������
	mAction->MouseDoubleClick();
	outText.Format(_T("%d"), mIntOrderCount);
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	//ֹ��checkbox
	DoHop(-81, 95);

	//ֹӯcheckbox
	DoHop(0, 25);

	if (isHigh==DO_HIGH)
	{
		//ֹӯ�ϼ�ͷ
		mAction->MoveCursor(96,-5);

		for (int i = 0;i < 30; i++)
		{
			mAction->MouseClick();
		}
	} 
	else
	{
		//ֹӯ�¼�ͷ
		mAction->MoveCursor(96,2);
		for (int i = 0;i < 30; i++)
		{
			mAction->MouseClick();
		}
		mAction->MoveCursor(0,-7);
	}

	//ȷ����ť
	DoHop(273,78);
	Sleep(100);
	mAction->MouseClick();

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
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

//direct : 0��ʾ���߶��� 1 ��ʾ���� 2 ��ʾ����
int CActionManager::DoHFDoubleSide(int lowDiff, int highDiff, int count, int windowDelay,  int direct)
{
	POINT lpPoint;
	GetCursorPos(&lpPoint);
	
	if(direct == 0 || direct == DO_LOW)
	{
		if (direct == 0)
		{
			mAction->MouseDoubleClick();
		}
		
		CLogger::Add("��");
		DoHFSingleSide(lowDiff, DO_LOW, count, windowDelay);
		CPoint pos = GetHFConfirmDialogPos();
		if (pos.x + pos.y != 0)
		{
			CloseHFConfirmDialog(pos.x, pos.y);
		}
	}

	if (direct == 0)
	{
		Sleep(windowDelay);
	}
	
	if(direct == 0 || direct == DO_HIGH)
	{
		if (direct == 0)
		{
			mAction->MoveCursor(lpPoint.x, lpPoint.y, true);
			mAction->MouseDoubleClick();
		}

		CLogger::Add("��");
		DoHFSingleSide(highDiff, DO_HIGH, count, windowDelay);
		CPoint pos = GetHFConfirmDialogPos();
		if (pos.x + pos.y != 0)
		{
			CloseHFConfirmDialog(pos.x, pos.y);
		}
	}

	return 0;
}

// 1 for low 2 for high
int CActionManager::DoHFSingleSide(int diff, int direct, int count, int windowDelay)
{

	//1.��ǰλ��˫�������µ��Ի���
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(NULL,HUIFENG_DIALOG_TITLE_NAME );
		if (wnd)
		{

			CString log;
			log.Format(_T("DoHFSingleSide direct=%d, dialogSearchCount=%d"), direct, searchCount);
			CLogger::Add(log);

			if (DoHFSingleSideAction(diff, direct, count, windowDelay) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
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

	return SEMIC_AUTO_TRADE_CALL_FAILED;
}

int CActionManager::DoHFSingleSideAction(int diff, int direct, int count, int windowDelay)
{
	CPoint dialogPos = GetHFDialogPos();
	if (dialogPos.x + dialogPos.y  == 0)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}
	int time = mLuaEngine.GetDebugSleepInterval();

	CPoint pos = mLuaEngine.GetOrigin2DropListButton();

	//�Ի���ԭ��
	mAction->MoveCursor(dialogPos.x, dialogPos.y, true);

	//�㿪�µ������б�
	DoHop(pos.x, pos.y);
	Sleep(50);

	//ѡ��ί�е�
	DoHop(mLuaEngine.GetOrderTypeButton().x, mLuaEngine.GetOrderTypeButton().y);
	Sleep(time);
	//������
	pos = mLuaEngine.GetTradeCount();
	mAction->MoveCursor(pos.x, pos.y);
	TRACE("count=%d\r\n",count);

	for (int i = 1; i < count; i++)
	{
		//��������
		mAction->MouseClick();
	}

	//�������
	DoHop(mLuaEngine.GetDirectionButton(direct).x -pos.x, mLuaEngine.GetDirectionButton(direct).y -pos.y);
	Sleep(time);

	//����۸�Χֵ
	mAction->MoveCursor(mLuaEngine.GetPriceAdjustButton(direct).x, mLuaEngine.GetPriceAdjustButton(direct).y);
	float newPrice = 0.0f;
	int i = 0;
	while (i++ < CHECK_EDIT_PASTE_RESULT_MAX_TIMES)
	{
		mAction->MouseDoubleClick();
		Sleep(50*i);
		mAction->KeyboardCopy();
		Sleep(50*i);
		CString strPrice = GetContentFromClipboard();
		
		float price = atof(strPrice);
		if (price > VALID_PRICE)
		{
			newPrice = direct == DO_LOW?price - diff: price + diff;

			break;
		}
	}

	//�ɽ��۸������ϼ�ͷ
	DoHop(mLuaEngine.GetPriceRange2Price(direct).x, mLuaEngine.GetPriceRange2Price(direct).y);
	CString buffer;
	buffer.Format(_T("%.1f"), newPrice);
	CLogger::Add("�ҵ���" +buffer);
	
	if (direct == DO_HIGH)
	{
		for (int i =0 ; i < diff ; i++)
		{
			mAction->MouseClick(0);
		}
	}
	else
	{
		mAction->MoveCursor(0, 23);
		for (int i =0 ; i < diff ; i++)
		{
			mAction->MouseClick(0);
		}
		mAction->MoveCursor(0, -23);
	}
	
	//ֹ��checkbox
	DoHop(mLuaEngine.GetEnableStopButton(direct).x, mLuaEngine.GetEnableStopButton(direct).y);
	Sleep(time);

	//ֹ����
	int stopThreshold  = mLuaEngine.GetStopGainThreshold();
	int stoploseDiff  = mLuaEngine.GetStopLoseDiff(direct);
	//ֹ����ֵ�ϰ�ť
	pos = mLuaEngine.GetInitialStopPriceButton(direct);
	Sleep(time);

	//����ֹ��۸���ֵ
	float price = direct == DO_LOW?newPrice + stoploseDiff: newPrice - stoploseDiff;
	
	//����ֹ��۸�
	price = direct == DO_LOW?price + diff: price - diff;

	//�ﵽֹ����ֵ�ϰ�ť
	DoHop( pos.x,  pos.y);
	Sleep(time);
	buffer.Format(_T("%.1f"), price);
	CLogger::Add("ֹ���" +buffer);

	if (direct == DO_LOW)
	{
		for (int i =0 ; i < diff+1 ; i++)
		{
			mAction->MouseClick(0);
		}
	}
	else
	{
		mAction->MoveCursor(0, 12);
		for (int i =0 ; i < diff+1 ; i++)
		{
			mAction->MouseClick(0);
		}
		mAction->MoveCursor(0, -12);
	}
	
	//���ֹӯ��ť
	pos = mLuaEngine.GetPrice2StopCheckbox();
	TRACE("GetPrice2StopCheckbox.x=%d,GetPrice2StopCheckbox.y=%d", pos);
	DoHop(pos.x, pos.y);
	Sleep(time);

	//ֹӯ��ť��ֹӯ�۸������ϼ�ͷ
	DoHop(mLuaEngine.GetInitialStopPriceButton(direct).x,mLuaEngine.GetInitialStopPriceButton(direct).y);
	int stopGainDiff = mLuaEngine.getStopGainDiff(direct); 
	if (direct == DO_HIGH)
	{
		for (int i =0 ; i < stopGainDiff; i++)
		{
			mAction->MouseClick(0);
		}
	}
	else
	{
		mAction->MoveCursor(0, 12);
		for (int i =0 ; i < stopGainDiff ; i++)
		{
			mAction->MouseClick(0);
		}
		mAction->MoveCursor(0, -12);
	}
	
	//ֹӯ�۸�
	price = direct == DO_LOW?newPrice - stopGainDiff - stopThreshold : newPrice + stopGainDiff +stopThreshold;
	buffer.Format(_T("%.1f"), price);
	CLogger::Add("ֹӯ��" +buffer);
	Sleep(time);
	DoHop(mLuaEngine.GetConfirmButton(direct).x, mLuaEngine.GetConfirmButton(direct).y);
	Sleep(time);
	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}

const CPoint & CActionManager::GetHFDialogPos(void) const
{
	CPoint pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(NULL, HUIFENG_DIALOG_TITLE_NAME);
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
