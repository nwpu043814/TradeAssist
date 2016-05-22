#pragma once

#define HOT_KEY_CODE_LOW 1000
#define HOT_KEY_CODE_HIGH 1001
#define HOT_KEY_CODE_COUNT 1002
#define HOT_KEY_FLASH_COMPLETE 1003


#define VK_Z  'Z'
#define VK_X  'X'
#define VK_C  'C'
#define VK_V  'V'
#define VK_A  'A'

#define VK_INVALID	1003

#define WM_DO_TRADE  (WM_USER+100)
#define WM_DO_HIGH  (WM_USER+101)
#define WM_DO_COUNT  (WM_USER+102)

#define WINDOW_CHECK_INTERVAL	100
#define	MSG_DELAY 400
#define	KEYBD_DELAY 250
#define	MOUSE_DOUBLE_CLICK_DELAY 200
#define	MOUSE_CLICK_DELAY 50

#define		STRING_KEY_COUNT _T("COUNT")
#define		STRING_SETTING _T("SETTING")
#define		STRING_PRICE_DIFF _T("PRICE_DIFF")

//做多方向到价格
#define		STRING_HIGH_DIRECTION_PRICE_DX _T("HIGH_DIRECTION_PRICE_DX")
#define		STRING_HIGH_DIRECTION_PRICE_DY _T("HIGH_DIRECTION_PRICE_DY")

//做多价格到交易笔数
#define		STRING_HIGH_PRICE_COUNT_DX _T("HIGH_PRICE_COUNT_DX")
#define		STRING_HIGH_PRICE_COUNT_DY _T("HIGH_PRICE_COUNT_DY")

//做空方向到价格
#define		STRING_LOW_DIRECTION_PRICE_DX _T("LOW_DIRECTION_PRICE_DX")
#define		STRING_LOW_DIRECTION_PRICE_DY _T("LOW_DIRECTION_PRICE_DY")

//做空价格到交易笔数
#define		STRING_LOW_PRICE_COUNT_DX _T("LOW_PRICE_COUNT_DX")
#define		STRING_LOW_PRICE_COUNT_DY _T("LOW_PRICE_COUNT_DY")

//交易笔数到确定按钮
#define		STRING_COUNT_CONFIRM_DX _T("COUNT_CONFIRM_DX")
#define		STRING_COUNT_CONFIRM_DY _T("COUNT_CONFIRM_DY")

//原点到指价委托
#define		STRING_START_TAB_DX _T("STRING_START_TAB_DX")
#define		STRING_START_TAB_DY _T("STRING_START_TAB_DY")

//做多指价委托到方向
#define	STRING_HIGH_TAB_DIRECT_DX _T("STRING_HIGH_TAB_DIRECT_DX")
#define	STRING_HIGH_TAB_DIRECT_DY _T("STRING_HIGH_TAB_DIRECT_DY")

//做空指价委托到方向
#define	STRING_LOW_TAB_DIRECT_DX _T("STRING_LOW_TAB_DIRECT_DX")
#define	STRING_LOW_TAB_DIRECT_DY _T("STRING_LOW_TAB_DIRECT_DY")

#define	STRING_CHECK_BOX_AUTO_SUBMIT _T("STRING_CHECK_BOX_AUTO_SUBMIT")

#define	STRING_CHECK_BOX_FLASH_COMPLETE _T("STRING_CHECK_BOX_FLASH_COMPLETE")

#define		DO_LOW	1
#define		DO_HIGH	2
#define		DO_COUNT 3

#define		SUN_DIALOG_NAME _T("SunAwtDialog")