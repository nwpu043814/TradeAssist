#pragma once
#include "Constant.h"
#include "LuaEngine.h"

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
	int MoveCursor(int dx, int dy, bool isAbslute = false);
	// ѡ���������ݡ�
	int SelectAll(void);
	void PressArrowUp(void) const;
	void PressArrowDown(void) const;
	// ͨ��shift+Endѡ������text
	int SelectAll4HF(void);
	int GetContentFromClipboard(void);
	void ContinuesClick(int clickTimes);
	void InputPrice(CString price);
};

typedef CSimulateAction * CSimulateActionP;
