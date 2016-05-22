#include "StdAfx.h"
#include "LuoGeGuaDan.h"
#include "Logger.h"

CLuoGeGuaDan::CLuoGeGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner):CBaseGuaDan(lua, action, wndNewOwner)
{
}

CLuoGeGuaDan::~CLuoGeGuaDan(void)
{
}

CPoint CLuoGeGuaDan::GetGuaDanDialogPos(void)
{
	POINT pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(SUN_DIALOG_NAME,_T("定单窗口"));
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


	return pos;
}

int CLuoGeGuaDan::DoSingleSideAction(int diff, int isHigh, int count, int windowDelay)
{

	POINT dialogPos = 	GetGuaDanDialogPos();
	if (dialogPos.x == 0 && dialogPos.y == 0)
	{
		CLogger::Add(_T("not find sunawtdialog"));
		TRACE("not find sunawtdialog, return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG\r\n");
		//未能找到sun对话框
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}

	diff += isHigh==DO_HIGH? 16:25;

	int sleepTime = mLuaEngine->GetDebugSleepInterval();
	TRACE("sleepTime=%d\r\n", sleepTime);
	mAction->MoveCursor(dialogPos.x,dialogPos.y, true);
	TRACE("dialogPos.x=%d, dialogPos.y=%d\r\n", dialogPos.x, dialogPos.y);
	Sleep(100);
	//从原点移动到指价委托tab。
	CPoint p = mLuaEngine->GetLuoGeOrigin2Entrust();
	mAction->MoveCursor(p.x,p.y);
	TRACE("start2Tab.x=%d, start2Tab.y=%d\r\n", p.x, p.y);
	mAction->MouseClick();
	Sleep(sleepTime);
	//指价委托到方向
	p = mLuaEngine->GetLuoGeEntrust2Direct(isHigh);
	mAction->MoveCursor(p.x,p.y);
	mAction->MouseClick();
	Sleep(sleepTime);

	//从方向移到价格控件
	p = mLuaEngine->GetLuoGeDirection2Price(isHigh);
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
	p = mLuaEngine->GetLuoGePrice2Count(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("price2CountVector.x=%d, price2CountVector.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//更新交易手数
	mAction->MouseDoubleClick();
	outText.Format(_T("%d"), count);

	SetClipboardContent(outText);
	mAction->KeyboardPaste();
	Sleep(sleepTime);
	//止损checkbox
	DoHop(-76, 102);
	Sleep(sleepTime);
	//止损输入框
	DoHop(82,0);
	double stopLosePrice = isHigh==DO_HIGH?newPrice - mLuaEngine->GetLuoGeStopLoseDiff() : newPrice +mLuaEngine->GetLuoGeStopLoseDiff() ;
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
	double stopGainPrice = isHigh==DO_HIGH?newPrice + mLuaEngine->GetLuoGeStopGainDiff() : newPrice -  mLuaEngine->GetLuoGeStopGainDiff()  ;
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