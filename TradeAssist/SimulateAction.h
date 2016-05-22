#pragma once
#include "Constant.h"

class CSimulateAction
{
public:
	CSimulateAction(void);
	virtual ~CSimulateAction(void);
	// ģ�����˫��
	int MouseDoubleClick(void);
	// ��굥��
	int MouseClick(int delay = MOUSE_CLICK_DELAY);
	// ͨ��ģ��Ctrl+C������
	int KeyboardCopy(void);
	// ͨ������Ctrl+Vģ��ճ��
	int KeyboardPaste(void);
	int MoveCursor(int dx, int dy, bool b = false);
	// ѡ���������ݡ�
	int SelectAll(void);
};

typedef CSimulateAction * CSimulateActionP;
