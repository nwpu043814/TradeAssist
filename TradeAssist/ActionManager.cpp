#include "StdAfx.h"
#include "ActionManager.h"
#include "Logger.h"
#include "HuifengGuadanParam.h"

BEGIN_MESSAGE_MAP(CActionManager, CWinThread)
	ON_THREAD_MESSAGE(WM_DO_HUIFENG_GUADAN,OnDoHuiFengGuadan)
	ON_THREAD_MESSAGE(WM_DO_TIANTONG_GUADAN,OnDoTianTongGuadan)
	ON_THREAD_MESSAGE(WM_DO_KUNJIAO_GUADAN,OnDoKunJiaoGuadan)
	ON_THREAD_MESSAGE(WM_DO_LUOGE_GUADAN,OnDoLuoGeGuadan)
	ON_THREAD_MESSAGE(WM_DO_ZHONGXIN_GUADAN,OnDoZhongXinGuadan)
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


void CActionManager::OnDoTianTongGuadan(WPARAM wParam,LPARAM lParam)
{
	if(wParam == NULL)
	{
		return;
	}

	CHuifengGuadanParamP param = (CHuifengGuadanParamP) wParam;
	DoHFDoubleSide(param->mLowDiff,param->mHighDiff, param->mTradeCount, param->mWindowDelay, param->mDirect, ON_TIMER_TIANTONG);
	TRACE("OnDoHuifengGuadan, w=%d, l=%d\r\n", wParam, lParam);
	delete param;
	param = NULL;
}

void CActionManager::OnDoZhongXinGuadan(WPARAM wParam,LPARAM lParam)
{
	if(wParam == NULL)
	{
		return;
	}

	CHuifengGuadanParamP param = (CHuifengGuadanParamP) wParam;
	DoHFDoubleSide(param->mLowDiff,param->mHighDiff, param->mTradeCount, param->mWindowDelay, param->mDirect, ON_TIMER_ZHONXIN);
	TRACE("OnDoLuoGeGuadan, w=%d, l=%d\r\n", wParam, lParam);
	delete param;
	param = NULL;
}

void CActionManager::OnDoLuoGeGuadan(WPARAM wParam,LPARAM lParam)
{
	if(wParam == NULL)
	{
		return;
	}

	CHuifengGuadanParamP param = (CHuifengGuadanParamP) wParam;
	DoHFDoubleSide(param->mLowDiff,param->mHighDiff, param->mTradeCount, param->mWindowDelay, param->mDirect, ON_TIMER_LUOGE);
	TRACE("OnDoLuoGeGuadan, w=%d, l=%d\r\n", wParam, lParam);
	delete param;
	param = NULL;
}

void CActionManager::OnDoHuiFengGuadan(WPARAM wParam,LPARAM lParam)
{
	if(wParam == NULL)
	{
		return;
	}

	CHuifengGuadanParamP param = (CHuifengGuadanParamP) wParam;
	DoHFDoubleSide(param->mLowDiff,param->mHighDiff, param->mTradeCount, param->mWindowDelay, param->mDirect, ON_TIMER_HUIFENG);
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

int CActionManager::DoTrade(double highDiff, double lowDiff, BOOL isHigh,
							int mIntOrderCount)
{
	POINT dialogPos = GetSunAwtDialogPos(_T("订单窗口"));
	if (dialogPos.x == 0 && dialogPos.y == 0)
	{
		CLogger::Add(_T("not find sunawtdialog"));
		TRACE("not find sunawtdialog, return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG\r\n");
		//未能找到sun对话框
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}

	highDiff += 2;
	lowDiff += 70;

	int sleepTime = mLuaEngine.GetDebugSleepInterval();
	TRACE("sleepTime=%d\r\n", sleepTime);
	mAction->MoveCursor(dialogPos.x,dialogPos.y, true);
	TRACE("dialogPos.x=%d, dialogPos.y=%d\r\n", dialogPos.x, dialogPos.y);
	
	//从原点移动到指价委托tab。
	CPoint p = mLuaEngine.getOrigin2Entrust();
	mAction->MoveCursor(p.x,p.y);
	TRACE("start2Tab.x=%d, start2Tab.y=%d\r\n", p.x, p.y);
	mAction->MouseClick();
	Sleep(sleepTime);
	//指价委托到方向
	p = mLuaEngine.getEntrust2Direction(isHigh);
	mAction->MoveCursor(p.x,p.y);
	mAction->MouseClick();
	Sleep(sleepTime);
	
	//从方向移到价格控件
	p = mLuaEngine.getDirection2Price(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("direction2Price.x=%d, direction2Price.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//获得预制的点差
	CString outText;

	//取得当前价格。
	CString text = GetEditText();

	double newCount = isHigh==DO_HIGH?atof(text) + highDiff : atof(text) - lowDiff ;
	outText.Format(_T("%.2f"),newCount);

	CString log;
	log.Format(_T("originalprice=%s, hiDiff=%f, lowDiff=%f newPrice=%s, direction=%d"), text, highDiff, lowDiff ,outText, isHigh);
	CLogger::Add(log);
	TRACE("%s\r\n",log);

	//保存以备检查
	CString mLastClipboardContent = outText;

	//设置剪贴板内容并粘贴到窗口
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();
	Sleep(sleepTime);
	if(!CheckEditPasteResult(mLastClipboardContent))
	{
		TRACE("CheckEditPasteResult , return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED\r\n");
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	//移动到设置手数的控件
	p = mLuaEngine.getPrice2Count(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("price2CountVector.x=%d, price2CountVector.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//更新交易手数
	mAction->MouseDoubleClick();
	outText.Format(_T("%.1f"), mIntOrderCount/10.0F);
	
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	//止损checkbox
	DoHop(-81, 95);

	if (isHigh==DO_HIGH)
	{
		DoHop(96,2);
		mAction->MoveCursor(-96,-2);
	}

	//止盈checkbox
	DoHop(0, 25);

	if (isHigh==DO_HIGH)
	{
		//止盈上箭头
		mAction->MoveCursor(96,-5);

		for (int i = 0;i < 50; i++)
		{
			mAction->MouseClick();
		}
	} 
	else
	{
		//止盈下箭头
		mAction->MoveCursor(96,2);
		for (int i = 0;i < 50; i++)
		{
			mAction->MouseClick();
		}
		mAction->MoveCursor(0,-7);
	}

	//确定按钮
	DoHop(273,78);
	Sleep(100);
	mAction->MouseClick();

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
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
		DoHFSingleSide(lowDiff, DO_LOW, count, windowDelay, softwareType);
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
		DoHFSingleSide(highDiff, DO_HIGH, count, windowDelay, softwareType);
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
		else if (softwareType == ON_TIMER_LUOGE || softwareType == ON_TIMER_ZHONXIN)
		{
			wnd = ::FindWindow(SUN_DIALOG_NAME,_T("定单窗口"));
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

			if (ON_TIMER_TIANTONG == softwareType)
			{
				if (DoTianTongSingleSideAction(diff, direct, count, windowDelay) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
				{
					return SEMIC_AUTO_TRADE_CALL_FAILED;
				}
				else
				{
					return SEMIC_AUTO_TRADE_CALL_SUCCESS;
				}
			}
			else if (ON_TIMER_HUIFENG == softwareType)
			{
				if (DoHuiFengSingleSideAction(diff, direct, count, windowDelay) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
				{
					return SEMIC_AUTO_TRADE_CALL_FAILED;
				}
				else
				{
					return SEMIC_AUTO_TRADE_CALL_SUCCESS;
				}
			}
			else if (ON_TIMER_KUNJIAO == softwareType)
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
			else if (ON_TIMER_LUOGE == softwareType)
			{
				if (DoLuoGeSingleSideAction(diff, direct, count, windowDelay) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
				{
					return SEMIC_AUTO_TRADE_CALL_FAILED;
				}
				else
				{
					return SEMIC_AUTO_TRADE_CALL_SUCCESS;
				}
			}
			else if (ON_TIMER_ZHONXIN == softwareType)
			{
				if (DoZhongXinSingleSideAction(diff, direct, count, windowDelay) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
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

int CActionManager::DoTianTongSingleSideAction(int diff, int direct, int count, int windowDelay)
{
	CPoint dialogPos = GetDialogPosByTitle(HUIFENG_DIALOG_TITLE_NAME);
	if (dialogPos.x + dialogPos.y  == 0)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}
	int time = mLuaEngine.GetDebugSleepInterval();

	CPoint pos = mLuaEngine.GetOrigin2DropListButton();

	//对话框原点
	mAction->MoveCursor(dialogPos.x, dialogPos.y, true);
	Sleep(time);
	//点开下单类型列表
	DoHop(pos.x, pos.y);
	Sleep(50);
	Sleep(time);
	//选择委托单
	DoHop(mLuaEngine.GetOrderTypeButton().x, mLuaEngine.GetOrderTypeButton().y);
	Sleep(time);
	//到手数
	pos = mLuaEngine.GetTradeCount();
	mAction->MoveCursor(pos.x, pos.y);
	TRACE("count=%d\r\n",count);
	Sleep(time);
	for (int i = 1; i < count; i++)
	{
		//控制手数
		mAction->MouseClick();
	}
	Sleep(time);
	//点击方向
	DoHop(mLuaEngine.GetDirectionButton(direct).x -pos.x, mLuaEngine.GetDirectionButton(direct).y -pos.y);
	Sleep(time);

	//点击价格范围值
	mAction->MoveCursor(mLuaEngine.GetPriceAdjustButton(direct).x, mLuaEngine.GetPriceAdjustButton(direct).y);
	float newPrice = 0.0f;
	float originalPrice = 0.0F;
	int i = 0;
	while (i++ < CHECK_EDIT_PASTE_RESULT_MAX_TIMES)
	{
		mAction->MouseDoubleClick();
		Sleep(50*i);
		mAction->KeyboardCopy();
		Sleep(50*i);
		CString strPrice = GetContentFromClipboard();
		
		originalPrice = atof(strPrice);
		if (originalPrice > VALID_PRICE)
		{
			newPrice = direct == DO_LOW?originalPrice - diff: originalPrice + diff;

			break;
		}
	}

	//成交价格设置上箭头
	DoHop(mLuaEngine.GetPriceRange2Price(direct).x, mLuaEngine.GetPriceRange2Price(direct).y);
	CString buffer;
	buffer.Format(_T("%.1f"), newPrice);
	CLogger::Add("挂单价" +buffer);
	
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
	
	//止损checkbox
	DoHop(mLuaEngine.GetEnableStopButton(direct).x, mLuaEngine.GetEnableStopButton(direct).y);
	Sleep(time);

	//止损点差
	int stopThreshold  = mLuaEngine.GetStopGainThreshold();
	int stoploseDiff  = mLuaEngine.GetStopLoseDiff(direct);
	//止损阈值上按钮
	pos = mLuaEngine.GetInitialStopPriceButton(direct);
	Sleep(time);

	//计算止损价格阈值
	float price = direct == DO_LOW?newPrice + stoploseDiff: newPrice - stoploseDiff;
	
	//计算止损价格
	price = direct == DO_LOW?price + diff+1: price - diff-1;

	//达到止损阈值上按钮
	mAction->MoveCursor( pos.x,  pos.y);
	Sleep(time);
	buffer.Format(_T("%.0f"), price);
	CLogger::Add("止损价" +buffer);
	mAction->InputPrice(buffer);

	int stopGainDiff = mLuaEngine.getStopGainDiff(direct); 
	if (stopGainDiff == 0)
	{
		//从止损上箭头到确定按钮。
		DoHop(-65, 99);
		pos = GetHFConfirmDialogPos();
		if (pos.x + pos.y != 0)
		{
			CloseHFConfirmDialog(pos.x, pos.y);
		}

		return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
	}

	//点击止盈按钮
	pos = mLuaEngine.GetPrice2StopCheckbox();
	TRACE("GetPrice2StopCheckbox.x=%d,GetPrice2StopCheckbox.y=%d", pos);
	DoHop(pos.x, pos.y);
	Sleep(time);

	//止盈按钮到止盈价格输入上箭头
	mAction->MoveCursor(mLuaEngine.GetInitialStopPriceButton(direct).x,mLuaEngine.GetInitialStopPriceButton(direct).y);
	
	//止盈价格
	price = direct == DO_LOW?newPrice - stopGainDiff - stopThreshold : newPrice + stopGainDiff +stopThreshold;
	buffer.Format(_T("%.0f"), price);
	CLogger::Add("止盈价" +buffer);
	mAction->InputPrice(buffer);
	Sleep(time);
	DoHop(mLuaEngine.GetConfirmButton(direct).x, mLuaEngine.GetConfirmButton(direct).y);
	Sleep(time);

	pos = GetHFConfirmDialogPos();
	if (pos.x + pos.y != 0)
	{
		CloseHFConfirmDialog(pos.x, pos.y);
	}

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}

const CPoint & CActionManager::GetDialogPosByTitle(CString title) const
{
	CPoint pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
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

int CActionManager::DoHuiFengSingleSideAction(int diff, int direct, int count, int windowDelay)
{

	CPoint dialogPos = GetDialogPosByTitle(HUIFENG_DIALOG_TITLE_NAME);
	if (dialogPos.x + dialogPos.y  == 0)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	int time = mLuaEngine.GetDebugSleepInterval();

	CPoint pos = mLuaEngine.GetOrigin2DropListButton();

	//对话框原点
	mAction->MoveCursor(dialogPos.x, dialogPos.y, true);
	Sleep(time);
	//点开下单类型列表
	DoHop(pos.x, pos.y);
	Sleep(50);
	Sleep(time);
	//选择委托单
	DoHop(mLuaEngine.GetOrderTypeButton().x, mLuaEngine.GetOrderTypeButton().y);
	Sleep(time);
	//到手数
	pos = mLuaEngine.GetTradeCount();
	mAction->MoveCursor(pos.x, pos.y);
	TRACE("count=%d\r\n",count);
	Sleep(time);
	for (int i = 1; i < count; i++)
	{
		//控制手数
		mAction->MouseClick();
	}
	Sleep(time);
	//点击方向
	DoHop(mLuaEngine.GetHuiFengDirectionButton(direct).x -pos.x, mLuaEngine.GetHuiFengDirectionButton(direct).y -pos.y);
	Sleep(time);

	//点击价格范围值
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

	if (newPrice < VALID_PRICE)
	{
		return 1;
	}

	//成交价格设置上箭头
	mAction->MoveCursor(mLuaEngine.GetHuiFengPriceRange2Price(direct).x, mLuaEngine.GetHuiFengPriceRange2Price(direct).y);
	CString buffer;
	buffer.Format(_T("%.1f"), newPrice);
	CLogger::Add("挂单价" +buffer);


	Sleep(time);
	SetClipboardContent(buffer);
	mAction->MouseDoubleClick();
	mAction->KeyboardPaste();

	//止损checkbox
	DoHop(mLuaEngine.GetHuiFengEnableStopButton(direct).x, mLuaEngine.GetHuiFengEnableStopButton(direct).y);
	Sleep(time);

	//止损点差
	int stopThreshold  = mLuaEngine.GetHuiFengStopThreshold();
	int stoploseDiff  = mLuaEngine.GetHuiFengStopLoseThreshold(direct);
	//止损阈值上按钮
	pos = mLuaEngine.GetHuiFengInitialStopPriceButton(direct);
	Sleep(time);

	//计算止损价格阈值
	float price = direct == DO_LOW?newPrice + stoploseDiff: newPrice - stoploseDiff;

	//计算止损价格
	price = direct == DO_LOW?price + diff: price - diff;

	//达到止损输入框
	mAction->MoveCursor(mLuaEngine.GetAdjustStopPriceButton(direct).x + pos.x, mLuaEngine.GetAdjustStopPriceButton(direct).y + pos.y);
	Sleep(time);
	buffer.Format(_T("%.1f"), price);
	SetClipboardContent(buffer);
	CLogger::Add("止损价" +buffer);
	mAction->KeyboardPaste();

	//点击止盈按钮
	pos = mLuaEngine.GetHuiFengPrice2StopCheckbox();
	TRACE("GetPrice2StopCheckbox.x=%d,GetPrice2StopCheckbox.y=%d", pos);
	DoHop(pos.x, pos.y);
	Sleep(time);

	//止盈按钮到止盈价格输入框距离
	mAction->MoveCursor(mLuaEngine.GetGainCheckbox2GainPriceEdit().x,mLuaEngine.GetGainCheckbox2GainPriceEdit().y);
	int stopGainDiff = mLuaEngine.GetHuiFengStopGainDiff(direct); 
	
	//止盈价格
	price = direct == DO_LOW?newPrice - stopGainDiff - stopThreshold : newPrice + stopGainDiff +stopThreshold;
	buffer.Format(_T("%.1f"), price);
	SetClipboardContent(buffer);
	mAction->KeyboardPaste();
	CLogger::Add("止盈价" +buffer);
	Sleep(time);
	DoHop(mLuaEngine.GetHuiFengConfirmButton(direct).x, mLuaEngine.GetHuiFengConfirmButton(direct).y);
	Sleep(time);

	pos = GetHFConfirmDialogPos();
	if (pos.x + pos.y != 0)
	{
		CloseHFConfirmDialog(pos.x, pos.y);
	}

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
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

int CActionManager::DoZhongXinSingleSideAction(int diff, int isHigh, int mIntOrderCount, int windowDelay)
{

	POINT dialogPos = GetSunAwtDialogPos(_T("定单窗口"));
	if (dialogPos.x == 0 && dialogPos.y == 0)
	{
		CLogger::Add(_T("not find sunawtdialog"));
		TRACE("not find sunawtdialog, return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG\r\n");
		//未能找到sun对话框
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}

	diff += 20;

	int sleepTime = mLuaEngine.GetDebugSleepInterval();
	TRACE("sleepTime=%d\r\n", sleepTime);
	mAction->MoveCursor(dialogPos.x,dialogPos.y, true);
	TRACE("dialogPos.x=%d, dialogPos.y=%d\r\n", dialogPos.x, dialogPos.y);
	Sleep(100);
	//从原点移动到指价委托tab。
	CPoint p = mLuaEngine.GetZhongXinOrigin2Entrust(isHigh);
	DoHop(p.x,p.y);
	TRACE("start2Tab.x=%d, start2Tab.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//指价委托到方向
	p = mLuaEngine.GetZhongXinEntrust2Direction(isHigh);
	mAction->MoveCursor(p.x,p.y);
	mAction->MouseClick();
	Sleep(sleepTime);

	//从方向移到价格控件
	p = mLuaEngine.GetZhongXinDirection2Price(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("direction2Price.x=%d, direction2Price.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//获得预制的点差
	CString outText;

	//取得当前价格。
	CString text = GetEditText();
	if(text.Find(_T("."))  == -1)
	{
		TRACE("text.Find(_T(\".\"))== -1, text=%s, return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE\r\n", text);
		return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE;
	}

	double newCount = isHigh==DO_HIGH?atof(text) + diff : atof(text) - diff ;
	outText.Format(_T("%.2f"),newCount);

	CString log;
	log.Format(_T("originalprice=%s, hiDiff=%d, lowDiff=%d newPrice=%s, direction=%d"), text, diff, diff ,outText, isHigh);
	CLogger::Add(log);
	TRACE("%s\r\n",log);

	//保存以备检查
	CString mLastClipboardContent = outText;

	//设置剪贴板内容并粘贴到窗口
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();
	Sleep(sleepTime);
	if(!CheckEditPasteResult(mLastClipboardContent))
	{
		TRACE("CheckEditPasteResult , return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED\r\n");
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	//移动到设置手数的控件
	p = mLuaEngine.GetZhongXinPrice2Count(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("price2CountVector.x=%d, price2CountVector.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//更新交易手数
	mAction->MouseDoubleClick();
	outText.Format(_T("%d"), mIntOrderCount);
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	DoHop(-84, 135);
	DoHop(84, 0);
	Sleep(sleepTime);
	newCount = isHigh==DO_HIGH?newCount - diff : newCount + diff ;
	outText.Format(_T("%.2f"),newCount);
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();
	Sleep(sleepTime);
	mAction->MoveCursor(-84,0);
	mAction->MoveCursor(84,-135);

	//移动到确定按钮上
	mAction->MoveCursor(mLuaEngine.getCount2Confirm().x,mLuaEngine.getCount2Confirm().y);
	Sleep(sleepTime);
	//自动提交
	mAction->MouseClick();
	Sleep(100);
	mAction->MouseClick();

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}
int CActionManager::DoLuoGeSingleSideAction(int diff, int isHigh, int mIntOrderCount, int windowDelay)
{
	POINT dialogPos = 	GetSunAwtDialogPos(_T("定单窗口"));
	if (dialogPos.x == 0 && dialogPos.y == 0)
	{
		CLogger::Add(_T("not find sunawtdialog"));
		TRACE("not find sunawtdialog, return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG\r\n");
		//未能找到sun对话框
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}

	diff += isHigh==DO_HIGH? 16:25;

	int sleepTime = mLuaEngine.GetDebugSleepInterval();
	TRACE("sleepTime=%d\r\n", sleepTime);
	mAction->MoveCursor(dialogPos.x,dialogPos.y, true);
	TRACE("dialogPos.x=%d, dialogPos.y=%d\r\n", dialogPos.x, dialogPos.y);
	Sleep(100);
	//从原点移动到指价委托tab。
	CPoint p = mLuaEngine.GetLuoGeOrigin2Entrust();
	mAction->MoveCursor(p.x,p.y);
	TRACE("start2Tab.x=%d, start2Tab.y=%d\r\n", p.x, p.y);
	mAction->MouseClick();
	Sleep(sleepTime);
	//指价委托到方向
	p = mLuaEngine.GetLuoGeEntrust2Direct(isHigh);
	mAction->MoveCursor(p.x,p.y);
	mAction->MouseClick();
	Sleep(sleepTime);

	//从方向移到价格控件
	p = mLuaEngine.GetLuoGeDirection2Price(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("direction2Price.x=%d, direction2Price.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//获得预制的点差
	CString outText;

	//取得当前价格。
	CString text = GetEditText();

	double newPrice = isHigh==DO_HIGH?atof(text) + diff : atof(text) - diff ;
	outText.Format(_T("%.2f"),newPrice);

	CString log;
	log.Format(_T("originalprice=%s, diff=%d,newPrice=%s, direction=%d"), text,  diff ,outText, isHigh);
	CLogger::Add(log);
	TRACE("%s\r\n",log);

	//保存以备检查
	CString mLastClipboardContent = outText;

	//设置剪贴板内容并粘贴到窗口
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();
	Sleep(sleepTime);
	if(!CheckEditPasteResult(mLastClipboardContent))
	{
		TRACE("CheckEditPasteResult , return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED\r\n");
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	//移动到设置手数的控件
	p = mLuaEngine.GetLuoGePrice2Count(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("price2CountVector.x=%d, price2CountVector.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//更新交易手数
	mAction->MouseDoubleClick();
	outText.Format(_T("%d"), mIntOrderCount);

	SetClipboardContent(outText);
	mAction->KeyboardPaste();
	Sleep(sleepTime);
	//止损checkbox
	DoHop(-76, 102);
	Sleep(sleepTime);
	//止损输入框
	DoHop(82,0);
	double stopLosePrice = isHigh==DO_HIGH?newPrice - mLuaEngine.GetLuoGeStopLoseDiff() : newPrice +mLuaEngine.GetLuoGeStopLoseDiff() ;
	outText.Format(_T("%.2f"),stopLosePrice);
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();
	mAction->MoveCursor(-82,0);
	Sleep(sleepTime);
	//止盈checkbox
	DoHop(0,22);
	Sleep(sleepTime);
	//止盈输入框
	DoHop(82,0);
	double stopGainPrice = isHigh==DO_HIGH?newPrice + mLuaEngine.GetLuoGeStopGainDiff() : newPrice -  mLuaEngine.GetLuoGeStopGainDiff()  ;
	outText.Format(_T("%.2f"),stopGainPrice);
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();
	mAction->MoveCursor(-82,0);
	Sleep(sleepTime);
	//确定按钮
	DoHop(346,74);
	mAction->MouseClick();
	Sleep(500);
	mAction->MouseClick();

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}


void CActionManager::OnDoInputPrice(WPARAM wParam,LPARAM lParam)
{
	Sleep(100);
	mAction->InputPrice(_T("5.8"));
}