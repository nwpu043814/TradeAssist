#pragma once

#define HOT_KEY_CODE_LOW 1000
#define HOT_KEY_CODE_HIGH 1001
#define HOT_KEY_CODE_COUNT 1002
#define HOT_KEY_FLASH_COMPLETE 1003
#define HOT_KEY_INCREASE_PRICE 1004
#define HOT_KEY_DECREASE_PRICE 1005

#define VK_Z  'Z'
#define VK_X  'X'
#define VK_C  'C'
#define VK_V  'V'
#define VK_A  'A'

#define VK_INVALID	1003

#define WM_DO_TRADE  (WM_USER+100)
#define WM_DO_HIGH  (WM_USER+101)
#define WM_DO_COUNT  (WM_USER+102)

#define WINDOW_CHECK_INTERVAL	25
#define	KEYBD_DELAY 20
#define	MOUSE_DOUBLE_CLICK_DELAY 20
#define	MOUSE_CLICK_DELAY 20
#define GET_CLIPBOARD_CONTENT_DELAY 10

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

//删除订单原定到删除
#define	STRING_START_DELETE_ORDER_DX _T("STRING_START_DELETE_ORDER_DX")
#define	STRING_START_DELETE_ORDER_DY _T("STRING_START_DELETE_ORDER_DY")

//删除到确定
#define STRING_DELETE_CONFIRM_DX	_T("STRING_DELETE_CONFIRM_DX")
#define STRING_DELETE_CONFIRM_DY	_T("STRING_DELETE_CONFIRM_DY")

#define	STRING_CHECK_BOX_AUTO_SUBMIT _T("STRING_CHECK_BOX_AUTO_SUBMIT")

#define	STRING_CHECK_BOX_FLASH_COMPLETE _T("STRING_CHECK_BOX_FLASH_COMPLETE")

//自动完成单间隔
#define	STRING_EDIT_AUTO_COMPLETE_INTERVAL	 _T("STRING_EDIT_AUTO_COMPLETE_INTERVAL")

//消息延时，单位毫秒
#define	STRING_EDIT_MSG_DELAY_TIME	 _T("STRING_EDIT_MSG_DELAY_TIME")

#define	STRING_EDIT_HOUR	 _T("STRING_EDIT_HOUR")
#define	STRING_EDIT_MINUTE	 _T("STRING_EDIT_MINUTE")
#define	STRING_EDIT_SECOND	 _T("STRING_EDIT_SECOND")

#define		DO_LOW	1
#define		DO_HIGH	2
#define		DO_COUNT 3

#define		MSG_DELAY_YES	1
#define		MSG_DELAY_NO	2

#define		SUN_DIALOG_NAME _T("SunAwtDialog")

#define		DELETE_ORDER_DELAY	300

//检查粘贴有效的最大次数
#define  CHECK_EDIT_PASTE_RESULT_MAX_TIMES 2

//尝试获取edit内容的最大重试次数
#define	 GET_EDIT_CONTENT_MAX_TIMES 4

//寻找sundialog的最大重试次数
#define	 FIND_SUN_DIALOG_MAX_RETRY_TIMES 12

//有效价格的位数
#define  PRICE_LENGTH	7

//未找到对话框
#define DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG 1 

//未正确获得价格
#define DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE 2 

//未通过价格正确检查
#define DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED 3

//正常返回
#define DO_TRADE_MSG_RESULT_TYPE_SUCCESS 3

#define SEMIC_AUTO_TRADE_CALL_SUCCESS 1
#define SEMIC_AUTO_TRADE_CALL_FAILED 2

//半自动交易重试次数
#define SEMIC_AUTO_TRADE_RETRY_TIMES 3

#define  TIMER_ID 123