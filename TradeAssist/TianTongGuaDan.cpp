#include "StdAfx.h"
#include "TianTongGuaDan.h"
#include "Logger.h"

CTianTongGuaDan::CTianTongGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner):CBaseGuaDan(lua, action, wndNewOwner)
, mInitialCursorPos(0)
{
}

CTianTongGuaDan::~CTianTongGuaDan(void)
{
}

CPoint CTianTongGuaDan::GetGuaDanDialogPos(void)
{

	return GetDialogPosByTitle(HUIFENG_DIALOG_TITLE_NAME);
}

void CTianTongGuaDan::CloseHFConfirmDialog(int left, int top)
{
	CPoint pos = mLuaEngine->GetHFConfirDialogOK();

	mAction->MoveCursor(left+pos.x, top + pos.y,true);
	mAction->MouseClick();
}


void  CTianTongGuaDan::GetHFConfirmDialogPos(CPoint&pos,  int retryTimes) const
{
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < retryTimes)
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

}

int CTianTongGuaDan::DoSingleSideAction(int diff, int direct, int count, int windowDelay)
{
	CPoint dialogPos = GetDialogPosByTitle(HUIFENG_DIALOG_TITLE_NAME);
	if (dialogPos.x + dialogPos.y  == 0)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}
	int time = mLuaEngine->GetDebugSleepInterval();

	CPoint pos = mLuaEngine->GetOrigin2DropListButton();

	//对话框原点
	mAction->MoveCursor(dialogPos.x, dialogPos.y, true);
	Sleep(time);
	//点开下单类型列表
	DoHop(pos.x, pos.y);
	Sleep(50);
	Sleep(time);
	//选择委托单
	DoHop(mLuaEngine->GetOrderTypeButton().x, mLuaEngine->GetOrderTypeButton().y);
	Sleep(time);
	//到手数
	pos = mLuaEngine->GetTradeCount();
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
	DoHop(mLuaEngine->GetDirectionButton(direct).x -pos.x, mLuaEngine->GetDirectionButton(direct).y -pos.y);
	Sleep(time);

	//点击价格范围值
	mAction->MoveCursor(mLuaEngine->GetPriceAdjustButton(direct).x, mLuaEngine->GetPriceAdjustButton(direct).y);
	float newPrice = 0.0f;
	float originalPrice = 0.0F;
	int i = 0;
	while (i++ < CHECK_EDIT_PASTE_RESULT_MAX_TIMES)
	{
		SetClipboardContent(_T(""));
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
	DoHop(mLuaEngine->GetPriceRange2Price(direct).x, mLuaEngine->GetPriceRange2Price(direct).y);
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
	else if(direct == DO_LOW)
	{
		mAction->MoveCursor(0, 23);
		for (int i =0 ; i < diff ; i++)
		{
			mAction->MouseClick(0);
		}
		mAction->MoveCursor(0, -23);
	}

	//止损checkbox
	DoHop(mLuaEngine->GetEnableStopButton(direct).x, mLuaEngine->GetEnableStopButton(direct).y);
	Sleep(time);

	//止损点差
	int stopThreshold  = mLuaEngine->GetStopGainThreshold();
	int stoploseDiff  = mLuaEngine->GetStopLoseDiff(direct);
	//止损阈值上按钮
	pos = mLuaEngine->GetInitialStopPriceButton(direct);
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

	int stopGainDiff = mLuaEngine->getStopGainDiff(direct); 
	if (stopGainDiff == 0)
	{
		//从止损上箭头到确定按钮。
		DoHop(-65, 99);
		GetHFConfirmDialogPos(pos);
		if (pos.x + pos.y != 0)
		{
			CloseHFConfirmDialog(pos.x, pos.y);
		}

		return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
	}

	//点击止盈按钮
	pos = mLuaEngine->GetPrice2StopCheckbox();
	TRACE("GetPrice2StopCheckbox.x=%d,GetPrice2StopCheckbox.y=%d", pos);
	DoHop(pos.x, pos.y);
	Sleep(time);

	//止盈按钮到止盈价格输入上箭头
	mAction->MoveCursor(mLuaEngine->GetInitialStopPriceButton(direct).x,mLuaEngine->GetInitialStopPriceButton(direct).y);

	//止盈价格
	price = direct == DO_LOW?newPrice - stopGainDiff - stopThreshold : newPrice + stopGainDiff +stopThreshold;
	buffer.Format(_T("%.0f"), price);
	CLogger::Add("止盈价" +buffer);
	mAction->InputPrice(buffer);
	Sleep(time);
	DoHop(mLuaEngine->GetConfirmButton(direct).x, mLuaEngine->GetConfirmButton(direct).y);
	Sleep(time);

	GetHFConfirmDialogPos(pos, FIND_SUN_DIALOG_MAX_RETRY_TIMES*2);
	if (pos.x + pos.y != 0)
	{
		CloseHFConfirmDialog(pos.x, pos.y);
	}

	Sleep(80);
	dialogPos = GetDialogPosByTitle(HUIFENG_DIALOG_TITLE_NAME,NULL, 1);
	if (dialogPos.x + dialogPos.y  != 0)
	{
		mAction->MoveCursor(dialogPos.x, dialogPos.y, true);	
		Sleep(80);
		DoHop(267, 478);
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}

void CTianTongGuaDan::DoTianTongFastChase(int direct)
{
	CPoint dialogPos = GetDialogPosByTitle(HUIFENG_DIALOG_TITLE_NAME);
	if (dialogPos.x + dialogPos.y  == 0)
	{
		return ;
	}

	//保存动作开始前鼠标的原始位置
	GetCursorPos(&mInitialCursorPos);

	mAction->MoveCursor(dialogPos.x, dialogPos.y, true);
	//坐标数据暂时写死，带后续优化
	if (direct == DO_HIGH)
	{
		//原点到方向的点击
		DoHop(67, 232);

		//方向到确定提交的点击
		DoHop(76,242);
	} 
	else if(direct == DO_LOW)
	{
		//原点到方向的点击
		DoHop(129,232);

		//方向到确定提交的点击
		DoHop(0, 242);
	}
	
}

void CTianTongGuaDan::DoTianTongChaseAction(int direct, int count)
{
	CPoint dialogPos = GetDialogPosByTitle(HUIFENG_DIALOG_TITLE_NAME);
	if (dialogPos.x + dialogPos.y  == 0)
	{
		return ;
	}
	int time = mLuaEngine->GetDebugSleepInterval();
	CPoint pos = mLuaEngine->GetOrigin2DropListButton();

	//保存动作开始前鼠标的原始位置
	GetCursorPos(&mInitialCursorPos);

	//对话框原点
	mAction->MoveCursor(dialogPos.x, dialogPos.y, true);
	Sleep(time);

	//到手数
	pos = mLuaEngine->GetTradeCount();
	mAction->MoveCursor(pos.x, pos.y);
	TRACE("count=%d\r\n",count);
	Sleep(time);
	for (int i = 1; i < count; i++)
	{
		Sleep(time);
		//控制手数
		mAction->MouseClick();
	}
	mAction->RevertLastCusorMove();
	CPoint* count2Direc = new CPoint();
	if (direct == DO_HIGH)
	{
		count2Direc->SetPoint( 60 ,227);
	} 
	else if (direct == DO_LOW)
	{
		count2Direc->SetPoint(122 ,227);
	}
	DoHop(count2Direc->x, count2Direc->y);
	Sleep(time);
	mAction->RevertLastCusorMove();
	mAction->Hop(138 , 478);
	delete count2Direc;
}

CPoint CTianTongGuaDan::GetInitialCursorPos(void)
{
	return mInitialCursorPos;
}
