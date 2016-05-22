#pragma once

class SimulateAction
{
public:
	SimulateAction(void);
	virtual ~SimulateAction(void);
	// 模拟鼠标双击
	int MouseDoubleClick(void);
	// 鼠标单击
	int MouseClick(void);
	// 通过模拟Ctrl+C来复制
	int KeyboardCopy(void);
	// 通过键盘Ctrl+V模拟粘贴
	int KeyboardPaste(void);
	int MoveCursor(int dx, int dy, bool b = false);
	// 选中所有内容。
	int SelectAll(void);
};
