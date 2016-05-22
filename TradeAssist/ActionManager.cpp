#include "StdAfx.h"
#include "ActionManager.h"
#include "Logger.h"
#include "HuifengGuadanParam.h"
#include "TianTongGuaDan.h"
#include "BaseGuaDan.h"
#include "Util.h"
BEGIN_MESSAGE_MAP(CActionManager, CWinThread)
	ON_THREAD_MESSAGE(WM_DO_GUADAN,OnDoGuadan)
	ON_THREAD_MESSAGE(WM_DO_KUNJIAO_GUADAN,OnDoKunJiaoGuadan)
	ON_THREAD_MESSAGE(WM_DO_INPUT_PRICE,OnDoInputPrice) 
	ON_THREAD_MESSAGE(WM_DO_TIANTONG_BAIDAO_CHASE,DoTianTongFastChaseActionExt) 
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
	case WM_DO_TIANTONG_GUADAN:
		{
			guadan= new CTianTongGuaDan(&mLuaEngine, mAction, mWndNewOwner);
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

void CActionManager::GetDialogPosByTitle(CString title,  CPoint & pos, UINT retryTime) const
{
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

}

void CActionManager::DoHop(int x, int y)  const
{
	mAction->MoveCursor(x, y);
	mAction->MouseClick();
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
	else if (direct == DO_HIGH)
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
	else if (direct == DO_HIGH)
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
	else if (direct == DO_HIGH)
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
	else if(direct == DO_LOW)
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
	
	CPoint confirmDlgPos;
	GetDialogPosByTitle(KUNJIAO_CONFIRM_TITLE_NAME, confirmDlgPos);
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

void CActionManager::DoTiantongStopLose(int direction)
{

	CBaseGuaDan *base = new CBaseGuaDan(&mLuaEngine, mAction, mWndNewOwner);
	CPoint	pos ;
	int sleepTime = mLuaEngine.GetDebugSleepInterval();
	mAction->MouseDoubleClick();

	int diff = 2;
	pos = base->GetDialogPosByTitle(HUIFENG_DIALOG_TITLE_NAME_PINGCANG);
	if (pos.x + pos.y == 0)
	{
		delete base;
		return;
	}
	Sleep(sleepTime);
	mAction->MoveCursor(pos.x , pos.y, true );
	Sleep(sleepTime);
	mAction->Hop(334, 64);//点击下单类型
	Sleep(sleepTime);
	mAction->Hop(0, 28);//点击指价平仓
	Sleep(sleepTime);
	mAction->RevertLastCusorMove();
	mAction->Hop(-278, 253);//点击启用止损
	Sleep(sleepTime);
	mAction->RevertLastCusorMove();
	mAction->Hop(-137, 245);//初始化止损价格
	Sleep(sleepTime);

	if (direction == DO_HIGH)
	{
		//调整止损价格
		mAction->RevertLastCusorMove();
		mAction->Hop(-137, 257);
		Sleep(sleepTime);
		mAction->ContinuesClick(diff);
	}
	else if (direction == DO_LOW)
	{
		mAction->ContinuesClick(diff);
	}
	else
	{
		delete base;
		return;
	}
	Sleep(sleepTime);
	mAction->RevertLastCusorMove();
	Sleep(sleepTime);
	mAction->Hop(-207, 379);//点击确定
	delete base;
}


// 给baidao数据查询自动追单用的，带自动追加止损的天通chase
void CActionManager::DoTianTongFastChaseActionExt(WPARAM wParam,LPARAM lParam)
{
	int direction = static_cast<int>(wParam);
	CTianTongGuaDan* action = new CTianTongGuaDan(&mLuaEngine, mAction, mWndNewOwner);
	action->DoTianTongFastChase(direction);
	Sleep(5000);

	//关闭下单成功提示
	CPoint comfirDialogPos = action->GetDialogPosByTitle(HUIFENG_CONFIRM_DIALOG_TITLE_NAME,HUIFENG_DIALOG_TITLE_NAME_CHICANG_CLASS_NAME );
	if(comfirDialogPos.x + comfirDialogPos.y == 0)
	{
		delete action;
		return;
	}

	mAction->MoveCursor(comfirDialogPos.x, comfirDialogPos.y, true);
	mAction->Hop(158, 125);
	Sleep(200);
	mAction->MoveCursor(action->GetInitialCursorPos().x, action->GetInitialCursorPos().y, true);
	DoTiantongStopLose(direction);
	Sleep(1500);

	//关闭修改止损价格成功提示
	comfirDialogPos = action->GetDialogPosByTitle(HUIFENG_CONFIRM_DIALOG_TITLE_NAME, HUIFENG_DIALOG_TITLE_NAME_CHICANG_CLASS_NAME);
	if(comfirDialogPos.x + comfirDialogPos.y == 0)
	{
		delete action;
		return;
	}

	mAction->MoveCursor(comfirDialogPos.x, comfirDialogPos.y, true);
	mAction->Hop(158, 125);

	delete action;
}



// 给fx678两个经济数据查询结果自动追单用的追加止损的天通银操作
void CActionManager::DoTianTongChaseActionExt(int direction, int mIntOrderCount)
{
	CTianTongGuaDan* action = new CTianTongGuaDan(&mLuaEngine, mAction, mWndNewOwner);
	action->DoTianTongChaseAction(direction, mIntOrderCount);
	Sleep(5000);

	//关闭下单成功提示
	CPoint comfirDialogPos = action->GetDialogPosByTitle(HUIFENG_CONFIRM_DIALOG_TITLE_NAME,HUIFENG_DIALOG_TITLE_NAME_CHICANG_CLASS_NAME );
	if(comfirDialogPos.x + comfirDialogPos.y == 0)
	{
		delete action;
		return;
	}

	mAction->MoveCursor(comfirDialogPos.x, comfirDialogPos.y, true);
	mAction->Hop(158, 125);
	Sleep(200);
	mAction->MoveCursor(action->GetInitialCursorPos().x, action->GetInitialCursorPos().y, true);
	DoTiantongStopLose(direction);
	Sleep(1500);

	//关闭修改止损价格成功提示
	comfirDialogPos = action->GetDialogPosByTitle(HUIFENG_CONFIRM_DIALOG_TITLE_NAME,  HUIFENG_DIALOG_TITLE_NAME_CHICANG_CLASS_NAME);
	if(comfirDialogPos.x + comfirDialogPos.y == 0)
	{
		delete action;
		return;
	}
	
	mAction->MoveCursor(comfirDialogPos.x, comfirDialogPos.y, true);
	mAction->Hop(158, 125);

	delete action;
}

void CActionManager::DoEcnomicDataAction(PEcnomicData data, CDataManager* mDataManager, HWND hwnd)
{
	if (data == NULL)
	{
		return;
	}

	CTianTongGuaDan* action = new CTianTongGuaDan(&mLuaEngine, GetAction(), hwnd); 
	if (!mDataManager->mNonfarmerNumber[0]->GetUrl().IsEmpty() && !mDataManager->mJoblessRate[0]->GetUrl().IsEmpty())
	{

		if (mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_HIGH
			&& mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_HIGH)
		{
			mDataManager->mTotalConclution = CUtil::TranslateEcomicResult(data->GetResult());//根据两个数据更新总体多空。
			DoTianTongChaseActionExt(DO_HIGH, mDataManager->mIntOrderCount);
		}
		else if (mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_LOW
			&& mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_LOW)
		{
			mDataManager->mTotalConclution = CUtil::TranslateEcomicResult(data->GetResult());//根据两个数据更新总体多空。
			DoTianTongChaseActionExt(DO_LOW, mDataManager->mIntOrderCount);
		}
		else if (mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_HIGH
			&&  mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_LOW)
		{
			if (mDataManager->mNonfarmerNumber[0]->GetWeight() > mDataManager->mJoblessRate[0]->GetWeight())
			{
				mDataManager->mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_HIGH);//根据两个数据更新总体多空。
				//TODO : do chase action
			} 
			else if (mDataManager->mNonfarmerNumber[0]->GetWeight() < mDataManager->mJoblessRate[0]->GetWeight())
			{
				mDataManager->mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_LOW);//根据两个数据更新总体多空。
				//TODO : do chase action
			}
		}
		else if (mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_LOW
			&&  mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_HIGH)
		{
			if (mDataManager->mNonfarmerNumber[0]->GetWeight() > mDataManager->mJoblessRate[0]->GetWeight())
			{
				mDataManager->mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_LOW);
				//TODO : do chase action
			} 
			else if (mDataManager->mNonfarmerNumber[0]->GetWeight() < mDataManager->mJoblessRate[0]->GetWeight())
			{
				mDataManager->mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_HIGH);
				//TODO : do chase action
			}
		}
		else if (mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_EQUAL)
		{
			mDataManager->mTotalConclution = CUtil::TranslateEcomicResult(mDataManager->mNonfarmerNumber[0]->GetResult());

			if (mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_LOW)
			{
				DoTianTongChaseActionExt(DO_LOW,mDataManager-> mIntOrderCount);
			}
			else if (mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_HIGH)
			{
				DoTianTongChaseActionExt(DO_HIGH, mDataManager->mIntOrderCount);
			}
		}
		else if (mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_EQUAL)
		{
			mDataManager->mTotalConclution = CUtil::TranslateEcomicResult(mDataManager->mJoblessRate[0]->GetResult());
			if (mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_LOW)
			{
				DoTianTongChaseActionExt(DO_LOW, mDataManager->mIntOrderCount);
			}
			else if (mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_HIGH)
			{
				DoTianTongChaseActionExt(DO_HIGH, mDataManager->mIntOrderCount);
			}
		}
	}
	else if (!mDataManager->mNonfarmerNumber[0]->GetUrl().IsEmpty())
	{
		if (mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_HIGH)
		{
		} 
		else if(mDataManager->mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_LOW)
		{
		}
	}
	else if ( !mDataManager->mJoblessRate[0]->GetUrl().IsEmpty())
	{
		if (mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_HIGH)
		{
		} 
		else if(mDataManager->mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_LOW)
		{
		}
	}

	delete action;
}
