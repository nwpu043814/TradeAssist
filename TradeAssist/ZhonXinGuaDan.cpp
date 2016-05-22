#include "StdAfx.h"
#include "ZhonXinGuaDan.h"
#include "Logger.h"
CZhonXinGuaDan::CZhonXinGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner):CBaseGuaDan(lua, action, wndNewOwner)
{
}

CZhonXinGuaDan::~CZhonXinGuaDan(void)
{
}


CPoint CZhonXinGuaDan::GetGuaDanDialogPos(void)
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

int CZhonXinGuaDan::DoSingleSideAction(int diff, int isHigh, int count, int windowDelay)
{

	POINT dialogPos = GetGuaDanDialogPos();

	if (dialogPos.x == 0 && dialogPos.y == 0)
	{
		CLogger::Add(_T("not find sunawtdialog"));
		TRACE("not find sunawtdialog, return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG\r\n");
		//未能找到sun对话框
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}

	diff += 10;

	int sleepTime = mLuaEngine->GetDebugSleepInterval();
	TRACE("sleepTime=%d\r\n", sleepTime);
	mAction->MoveCursor(dialogPos.x,dialogPos.y, true);
	TRACE("dialogPos.x=%d, dialogPos.y=%d\r\n", dialogPos.x, dialogPos.y);
	Sleep(100);
	//从原点移动到指价委托tab。
	CPoint p = mLuaEngine->GetZhongXinOrigin2Entrust(isHigh);
	DoHop(p.x,p.y);
	TRACE("start2Tab.x=%d, start2Tab.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//指价委托到方向
	p = mLuaEngine->GetZhongXinEntrust2Direction(isHigh);
	mAction->MoveCursor(p.x,p.y);
	mAction->MouseClick();
	Sleep(sleepTime);

	//从方向移到价格控件
	p = mLuaEngine->GetZhongXinDirection2Price(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("direction2Price.x=%d, direction2Price.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//获得预制的点差
	CString outText;

	//取得当前价格。
	CString text = GetEditText(5);
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
	p = mLuaEngine->GetZhongXinPrice2Count(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("price2CountVector.x=%d, price2CountVector.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);

	//更新交易手数
	mAction->MouseDoubleClick();
	outText.Format(_T("%d"), count);
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	DoHop(-85, 100);
	DoHop(84, 0);
	Sleep(sleepTime);
	newCount = isHigh==DO_HIGH?newCount - diff : newCount + diff ;
	outText.Format(_T("%.2f"),newCount);
	SetClipboardContent(outText);
	mAction->SelectAll();
	mAction->KeyboardPaste();
	Sleep(sleepTime);

	//移动到确定按钮上
	mAction->MoveCursor(262, 95);
	Sleep(sleepTime);
	//自动提交
	mAction->MouseClick();
	Sleep(100);
	mAction->MouseClick();

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}