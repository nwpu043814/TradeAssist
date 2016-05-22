#pragma once
#include "Constant.h"
#include "LuaEngine.h"
#include "atltypes.h"

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
	void Hop(int dx, int dy);
private:
	// ���һ������ƶ�λ��
	CPoint mLastCusorMove;
public:
	// �����ϴ�����ƶ�
	void RevertLastCusorMove(void);
};

typedef CSimulateAction * CSimulateActionP;
