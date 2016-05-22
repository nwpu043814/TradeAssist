#include "StdAfx.h"
#include "SimulateAction.h"
#include "Constant.h"

CSimulateAction::CSimulateAction(void)
{
}

CSimulateAction::~CSimulateAction(void)
{
}

// ģ�����˫��
int CSimulateAction::MouseDoubleClick(void)
{
	MouseClick();
	Sleep(MOUSE_DOUBLE_CLICK_DELAY);
	MouseClick();
	return 0;
}

// ��굥��
int CSimulateAction::MouseClick(void)
{
	POINT lpPoint;
	GetCursorPos(&lpPoint);
	SetCursorPos(lpPoint.x, lpPoint.y);

	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0, NULL,NULL);
	Sleep(MOUSE_CLICK_DELAY);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0, NULL,NULL);
	
	return 0;
}

// ͨ��ģ��Ctrl+C������
int CSimulateAction::KeyboardCopy(void)
{
	keybd_event(VK_CONTROL ,0,0,0);
	keybd_event(VK_C ,0,0,0);
	Sleep(KEYBD_DELAY);
	keybd_event(VK_C,0,KEYEVENTF_KEYUP,0); 
	keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0); 
	return 0;
}

// ͨ������Ctrl+Vģ��ճ��
int CSimulateAction::KeyboardPaste(void)
{
	keybd_event(VK_CONTROL ,0,0,0);
	keybd_event(VK_V ,0,0,0);
	Sleep(KEYBD_DELAY);
	keybd_event(VK_V,0,KEYEVENTF_KEYUP,0); 
	keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0); 
	return 0;
}

//�ƶ����
int CSimulateAction::MoveCursor(int dx, int dy, bool isAbslute)
{
	POINT lpPoint;
	GetCursorPos(&lpPoint);

	if (isAbslute)
	{
		SetCursorPos(dx, dy);
	}
	else
	{
		SetCursorPos(lpPoint.x + dx, lpPoint.y + dy);
	}

	return 0;
}



// ѡ���������ݡ�
int CSimulateAction::SelectAll(void)
{
	MouseClick();
	keybd_event(VK_CONTROL ,0,0,0);
	keybd_event(VK_A ,0,0,0);
	Sleep(KEYBD_DELAY);
	keybd_event(VK_A,0,KEYEVENTF_KEYUP,0); 
	keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0); 
	return 0;
}
