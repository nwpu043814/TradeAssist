#include "StdAfx.h"
#include "HaiJiaoGuaDan.h"
#include "Logger.h"

CHaiJiaoGuaDan::CHaiJiaoGuaDan(CLuaEngineP lua, CSimulateActionP action,HWND wndNewOwner):CBaseGuaDan(lua, action, wndNewOwner)
{
}

CHaiJiaoGuaDan::~CHaiJiaoGuaDan(void)
{
}

CPoint CHaiJiaoGuaDan::GetGuaDanDialogPos(void)
{
	POINT pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(SUN_DIALOG_NAME,_T("��������"));
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

int CHaiJiaoGuaDan::DoSingleSideAction(int diff, int isHigh, int count, int windowDelay)
{
	POINT dialogPos = GetGuaDanDialogPos();
	if (dialogPos.x == 0 && dialogPos.y == 0)
	{
		CLogger::Add(_T("not find sunawtdialog"));
		TRACE("not find sunawtdialog, return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG\r\n");
		//δ���ҵ�sun�Ի���
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}

	diff += isHigh == DO_LOW? 70:2;

	int sleepTime = mLuaEngine->GetDebugSleepInterval();
	TRACE("sleepTime=%d\r\n", sleepTime);
	mAction->MoveCursor(dialogPos.x,dialogPos.y, true);
	TRACE("dialogPos.x=%d, dialogPos.y=%d\r\n", dialogPos.x, dialogPos.y);
	Sleep(150);
	//��ԭ���ƶ���ָ��ί��tab��
	CPoint p = mLuaEngine->getOrigin2Entrust();
	mAction->MoveCursor(p.x,p.y);
	TRACE("start2Tab.x=%d, start2Tab.y=%d\r\n", p.x, p.y);
	mAction->MouseClick();
	Sleep(sleepTime);
	//ָ��ί�е�����
	p = mLuaEngine->getEntrust2Direction(isHigh);
	mAction->MoveCursor(p.x,p.y);
	mAction->MouseClick();
	Sleep(sleepTime);

	//�ӷ����Ƶ��۸�ؼ�
	p = mLuaEngine->getDirection2Price(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("direction2Price.x=%d, direction2Price.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//���Ԥ�Ƶĵ��
	CString outText;

	//ȡ�õ�ǰ�۸�
	CString text = GetEditText();

	double newCount = isHigh==DO_HIGH?atof(text) + diff : atof(text) - diff ;
	outText.Format(_T("%.2f"),newCount);

	CString log;
	log.Format(_T("originalprice=%s, hiDiff=%d, lowDiff=%d newPrice=%s, direction=%d"), text, diff, diff ,outText, isHigh);
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
	p = mLuaEngine->getPrice2Count(isHigh);
	mAction->MoveCursor(p.x,p.y);
	TRACE("price2CountVector.x=%d, price2CountVector.y=%d\r\n", p.x, p.y);
	Sleep(sleepTime);
	//���½�������
	mAction->MouseDoubleClick();
	outText.Format(_T("%d"), count);

	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	//ֹ��checkbox
	DoHop(-81, 95);

	if (isHigh==DO_HIGH)
	{
		DoHop(96,2);
		mAction->MoveCursor(-96,-2);
	}

	//ֹӯcheckbox
	DoHop(0, 25);

	if (isHigh==DO_HIGH)
	{
		//ֹӯ�ϼ�ͷ
		mAction->MoveCursor(96,-5);

		for (int i = 0;i < 40; i++)
		{
			mAction->MouseClick();
		}
	} 
	else
	{
		//ֹӯ�¼�ͷ
		mAction->MoveCursor(96,2);
		for (int i = 0;i < 10; i++)
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