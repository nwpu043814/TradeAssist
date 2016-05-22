#pragma once

class SimulateAction
{
public:
	SimulateAction(void);
	virtual ~SimulateAction(void);
	// ģ�����˫��
	int MouseDoubleClick(void);
	// ��굥��
	int MouseClick(void);
	// ͨ��ģ��Ctrl+C������
	int KeyboardCopy(void);
	// ͨ������Ctrl+Vģ��ճ��
	int KeyboardPaste(void);
	int MoveCursor(int dx, int dy, bool b = false);
	// ѡ���������ݡ�
	int SelectAll(void);
};
