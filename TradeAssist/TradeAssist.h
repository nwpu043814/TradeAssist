// TradeAssist.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTradeAssistApp:
// �йش����ʵ�֣������ TradeAssist.cpp
//

class CTradeAssistApp : public CWinApp
{
public:
	CTradeAssistApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTradeAssistApp theApp;