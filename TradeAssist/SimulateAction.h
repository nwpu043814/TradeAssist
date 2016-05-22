#pragma once
#include "Constant.h"
#include "LuaEngine.h"

class CSimulateAction
{
public:
	CSimulateAction(void);
	virtual ~CSimulateAction(void);
	// 模拟鼠标双击
	int MouseDoubleClick(void);
	// 鼠标单击
	int MouseClick(int delay = MOUSE_CLICK_DELAY);
	// 通过模拟Ctrl+C来复制
	int KeyboardCopy(void);
	// 通过键盘Ctrl+V模拟粘贴
	int KeyboardPaste(void);
	int MoveCursor(int dx, int dy, bool isAbslute = false);
	// 选中所有内容。
	int SelectAll(void);
	void PressArrowUp(void) const;
	void PressArrowDown(void) const;
	// 通过shift+End选中所有text
	int SelectAll4HF(void);
	int GetContentFromClipboard(void);
	void ContinuesClick(int clickTimes);
	void InputPrice(CString price);
};

typedef CSimulateAction * CSimulateActionP;
