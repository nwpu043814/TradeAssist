#pragma once

#define		HOT_KEY_CODE_LOW 1000
#define		HOT_KEY_CODE_HIGH 1001
#define		HOT_KEY_CODE_COUNT 1002
#define		HOT_KEY_FLASH_COMPLETE 1003
#define		HOT_KEY_INCREASE_PRICE 1004
#define		HOT_KEY_DECREASE_PRICE 1005
#define		HOT_KEY_TEST_SERVER 1006
#define		HOT_KEY_DIRECT_DUO 1007
#define		HOT_KEY_DIRECT_KONG 1008
#define		HOT_KEY_INCREASE_THRESHOLD 1009
#define		HOT_KEY_DECREASE_THRESHOLD 1010
#define		HOT_KEY_ALT_D 1011

#define		VK_Z  'Z'
#define		VK_X  'X'
#define		VK_C  'C'
#define		VK_V  'V'
#define		VK_A  'A'
#define		VK_T  'T'
#define		VK_D  'D'
#define		VK_K  'K'
#define		VK_S  'S'

#define		VK_INVALID	1003

#define		WM_DO_TRADE  (WM_USER+100)
#define		WM_DO_HIGH  (WM_USER+101)
#define		WM_DO_CANCEL_ORDER  (WM_USER+102)
#define		WM_DISPLAY_DATAK  (WM_USER+103)

//http�����߳̽������͸���Ϣ�����߳�
#define		WM_HTTP_GET_FINISH (WM_USER + 104)

#define		WM_HTTP_GET_ECNOMIC_DATA_FINISH (WM_USER + 105)

#define		WM_DO_CHASE (WM_USER + 106)

#define		WM_ALT_D (WM_USER + 107)
//#define		WINDOW_CHECK_INTERVAL	50
//#define		KEYBD_DELAY 30
//#define		MOUSE_DOUBLE_CLICK_DELAY 30
//#define		MOUSE_CLICK_DELAY 30
//#define		GET_CLIPBOARD_CONTENT_DELAY 30
//�Զ��ҵ���Ҫ�����ĺ궨��


#define		WINDOW_CHECK_INTERVAL	50
#define		KEYBD_DELAY 10
#define		MOUSE_DOUBLE_CLICK_DELAY 10
#define		MOUSE_CLICK_DELAY 10
#define		GET_CLIPBOARD_CONTENT_DELAY 20

#define		STRING_KEY_COUNT _T("COUNT")
#define		STRING_SETTING _T("SETTING")
#define		STRING_HIGH_PRICE_DIFF _T("HIGH_PRICE_DIFF")
#define		STRING_LOW_PRICE_DIFF _T("LOW_PRICE_DIFF")

//���෽�򵽼۸�
#define		STRING_HIGH_DIRECTION_PRICE_DX _T("HIGH_DIRECTION_PRICE_DX")
#define		STRING_HIGH_DIRECTION_PRICE_DY _T("HIGH_DIRECTION_PRICE_DY")

//����۸񵽽��ױ���
#define		STRING_HIGH_PRICE_COUNT_DX _T("HIGH_PRICE_COUNT_DX")
#define		STRING_HIGH_PRICE_COUNT_DY _T("HIGH_PRICE_COUNT_DY")

//���շ��򵽼۸�
#define		STRING_LOW_DIRECTION_PRICE_DX _T("LOW_DIRECTION_PRICE_DX")
#define		STRING_LOW_DIRECTION_PRICE_DY _T("LOW_DIRECTION_PRICE_DY")

//���ռ۸񵽽��ױ���
#define		STRING_LOW_PRICE_COUNT_DX _T("LOW_PRICE_COUNT_DX")
#define		STRING_LOW_PRICE_COUNT_DY _T("LOW_PRICE_COUNT_DY")

//���ױ�����ȷ����ť
#define		STRING_COUNT_CONFIRM_DX _T("COUNT_CONFIRM_DX")
#define		STRING_COUNT_CONFIRM_DY _T("COUNT_CONFIRM_DY")

//ԭ�㵽ָ��ί��
#define		STRING_START_TAB_DX _T("STRING_START_TAB_DX")
#define		STRING_START_TAB_DY _T("STRING_START_TAB_DY")

//����ָ��ί�е�����
#define		STRING_HIGH_TAB_DIRECT_DX _T("STRING_HIGH_TAB_DIRECT_DX")
#define		STRING_HIGH_TAB_DIRECT_DY _T("STRING_HIGH_TAB_DIRECT_DY")

//����ָ��ί�е�����
#define		STRING_LOW_TAB_DIRECT_DX _T("STRING_LOW_TAB_DIRECT_DX")
#define		STRING_LOW_TAB_DIRECT_DY _T("STRING_LOW_TAB_DIRECT_DY")

//ɾ������ԭ����ɾ��
#define		STRING_START_DELETE_ORDER_DX _T("STRING_START_DELETE_ORDER_DX")
#define		STRING_START_DELETE_ORDER_DY _T("STRING_START_DELETE_ORDER_DY")

//ɾ����ȷ��
#define		STRING_DELETE_CONFIRM_DX	_T("STRING_DELETE_CONFIRM_DX")
#define		STRING_DELETE_CONFIRM_DY	_T("STRING_DELETE_CONFIRM_DY")

//�Ƿ��Զ��ύ
#define		STRING_CHECK_BOX_AUTO_SUBMIT _T("STRING_CHECK_BOX_AUTO_SUBMIT") 

//�Զ�ƽ����ֵ
#define		STRING_EDIT_AUTO_CLOSE_THRESHOLD _T("STRING_EDIT_AUTO_CLOSE_THRESHOLD") 

//�Զ���ɵ����
#define		STRING_EDIT_AUTO_COMPLETE_INTERVAL	 _T("STRING_EDIT_STRING_AUTO_COMPLETE_INTERVAL")

//��Ϣ��ʱ����λ����
#define		STRING_EDIT_MSG_DELAY_TIME	 _T("STRING_EDIT_MSG_DELAY_TIME")

//��������Ϣ
#define		STRING_EDIT_SERVER_HOST	 _T("STRING_EDIT_SERVER_HOST")
#define		STRING_EDIT_SERVER_PORT	 _T("STRING_EDIT_SERVER_PORT")
#define		STRING_CHECK_CONNECT_OWN_SERVER	 _T("STRING_CHECK_CONNECT_OWN_SERVER")

//�Զ�ƽ��ʱ������
#define  STRING_CHECK_BOX_ENABLE_CHECK_AUTO_CLOSE_DEPOT _T("STRING_CHECK_BOX_ENABLE_CHECK_AUTO_CLOSE_DEPOT")

#define		STRING_EDIT_HOUR	 _T("STRING_EDIT_HOUR")
#define		STRING_EDIT_MINUTE	 _T("STRING_EDIT_MINUTE")
#define		STRING_EDIT_SECOND	 _T("STRING_EDIT_SECOND_STR")
#define		STRING_EDIT_ENABLE_CHASE_TIMER	 _T("STRING_EDIT_DO_CHASE")
#define		STRING_EDIT_DO_HTTP_INTERVAL	 _T("STRING_EDIT_DO_HTTP_INTERVAL")
#define		STRING_CHECK_ENABLE_AUTO_THRESHOLD	 _T("STRING_CHECK_ENABLE_AUTO_THRESHOLD")
#define		STRING_CHECK_ENABLE_MOST_TOP	 _T("STRING_CHECK_ENABLE_MOST_TOP")

#define		DO_LOW	1
#define		DO_HIGH	2
#define		DO_COUNT 3

#define		MSG_DELAY_YES	1
#define		MSG_DELAY_NO	2

#define		SUN_DIALOG_NAME _T("SunAwtDialog")
#define		HUIFENG_DIALOG_TITLE_NAME _T("���ֵ�")
#define		HUIFENG_CONFIRM_DIALOG_TITLE_NAME _T("��ʾ")

#define		DELETE_ORDER_DELAY	450

//���ճ����Ч��������
#define		CHECK_EDIT_PASTE_RESULT_MAX_TIMES 3

//���Ի�ȡedit���ݵ�������Դ���
#define		GET_EDIT_CONTENT_MAX_TIMES 4

//Ѱ��sundialog��������Դ���
#define		FIND_SUN_DIALOG_MAX_RETRY_TIMES 12

//��Ч�۸��λ��
#define		PRICE_LENGTH	7

//δ�ҵ��Ի���
#define		DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG 1 

//δ��ȷ��ü۸�
#define		DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE 2 

//δͨ���۸���ȷ���
#define		DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED 3

//��������
#define		DO_TRADE_MSG_RESULT_TYPE_SUCCESS 3

#define		SEMIC_AUTO_TRADE_CALL_SUCCESS 1
#define		SEMIC_AUTO_TRADE_CALL_FAILED 2

//���Զ��������Դ���
#define		SEMIC_AUTO_TRADE_RETRY_TIMES 5

//˫��ҵ���ʱ��ID
#define		TIMER_ID_FOR_DO_FLASH_TRADE 123

#define		TIMER_INTERVAL_DO_FLASH_TRADE 50

#define		OPEN_CLIPBOARD_MAX_RETRY_TIMES 3

#define		DEBUG_SLEEP_INTERVAL 3000

#define		DEBUG_SLEEP 0

//���̷߳�����Ϣ�����������
#define		SEND_MESSAGE_TO_THREAD_MAX_RETTY_TIMES 3

//��ֵ���ڲ���
#define		THRESHOLD_STEP	5

#define		THRESHOLD_MIN	3

#define		THRESHOLD_MAX	40

#define		THRESHOLD_FILE_NAME	".\\threshold.lua"

#define		HTML_PARSE_UNKOWN	_T("---")
#define		HTML_PARSE_PRE_STRING	_T("<td>")
#define		HTML_PARSE_NEXT_STRING	_T("</td>")

#define		LUA_FUNCTION_GetExpectNonfarmerWorker _T("getExpectNonfarmerWorker")
#define		LUA_FUNCTION_GetExpectJoblessRate _T("getExpectJoblessRate")
#define		LUA_FUNCTION_GetOrigin2Entrust _T("getOrigin2Entrust")
#define		LUA_FUNCTION_GetEntrust2Direction _T("getEntrust2Direction")
#define		LUA_FUNCTION_GetDirection2Price _T("getDirection2Price")
#define		LUA_FUNCTION_GetPrice2Count _T("getPrice2Count")
#define		LUA_FUNCTION_GetCount2Confirm _T("getCount2Confirm")
#define		LUA_FUNCTION_GetOrigin2Remove _T("getOrigin2Remove")
#define		LUA_FUNCTION_GetRemove2Confirm _T("getRemove2Confirm")
#define		LUA_FUNCTION_GetThreshold _T("getThreshold")
#define		LUA_FUNCTION_GetJoblessUrl _T("getJoblessParam")
#define		LUA_FUNCTION_GetNonfarmerWorkerUrl _T("getNonfarmerWorkerParam")
#define		LUA_FUNCTION_GetOriginal2DoHigh _T("getOriginal2DoHigh")
#define		LUA_FUNCTION_GetOriginal2DoLow _T("getOriginal2DoLow")
#define		LUA_FUNCTION_GetOrigin2Count _T("getOrigin2Count")
#define		LUA_FUNCTION_GetCount2OrderButton _T("getCount2OrderButton")
#define		LUA_FUNCTION_GetDailyFxUrl _T("getDailyFxUrl")
#define		LUA_FUNCTION_GetChasePriceThreshold _T("getChasePriceThreshold")
#define		LUA_FUNCTION_GetChaseMaxTime _T("getChaseMaxTime")
#define		LUA_FUNCTION_GetChasePriceMax _T("getChasePriceMax")

//������
#define		LUA_FUNCTION_GetOrigin2DropListButton _T("getOrigin2DropListButton")
#define		LUA_FUNCTION_GetOrderTypeButton _T("getOrderTypeButton")
#define		LUA_FUNCTION_GetDirectionButton _T("getDirectionButton")
#define		LUA_FUNCTION_GetPriceAdjustButton _T("getPriceAdjustButton")
#define		LUA_FUNCTION_GetEnableStopButton _T("getEnableStopButton")
#define		LUA_FUNCTION_GetInitialStopPriceButton _T("getInitialStopPriceButton")
#define		LUA_FUNCTION_GetAdjustStopPriceButton _T("getAdjustStopPriceButton")
#define		LUA_FUNCTION_GetConfirmButton _T("getConfirmButton")
#define		LUA_FUNCTION_GetDoubleSideType _T("getDoubleSideType")
#define		LUA_FUNCTION_GetTradeCount _T("getTradeCount")
#define		LUA_FUNCTION_GetScaleList _T("getScaleList")

#define		LUA_FUNCTION_GetStopGainDiff _T("getStopGainDiff")
#define		LUA_FUNCTION_GetGainCheckbox2GainPriceEdit _T("getGainCheckbox2GainPriceEdit")
#define		LUA_FUNCTION_GetHFConfirDialogOK _T("getHFConfirDialogOK")
#define		LUA_FUNCTION_GetPriceRange2Price _T("getPriceRange2Price")
#define		LUA_FUNCTION_GetPrice2StopCheckbox _T("getPrice2StopCheckbox")
#define		LUA_FUNCTION_GetStopGainThreshold _T("getStopGainThreshold")


#define		HTTP_URL_LOCAL_DATA _T("/?req_price=?")
#define		HTTP_URL_TIANTONG_HOST _T("jry.baidao.com")
#define		HTTP_URL_TIANTONG_URI _T("/api/hq/npdata.do?ids=1&markettype=ttj")
#define		HTTP_URL_LOCAL _T("http://localhost")

#define		ECNOMIC_RESULT_TYPE_LOW _T("��")
#define		ECNOMIC_RESULT_TYPE_HIGH _T("��")
#define		ECNOMIC_RESULT_TYPE_EQUAL _T("ƽ")
#define		ECNOMIC_RESULT_TYPE_UNKNOWN _T("δ֪")

#define		THREAD_NUMBER 4

#define		LOCAL_SERVER_REQUEST_THREADED_NUMBER 1

#define		DO_PULL_PRICE_TIMES_PER_MSG		10

#define 	NETWORK_KEY_PRICE  "PRICE"
#define 	NETWORK_KEY_MILLION_SECOND "MILLION_SECOND"
#define 	NETWORK_KEY_TIME "TIME"
#define 	NETWORK_KEY_QUERY_TIME  "QUERY_TIME"
#define 	NETWORK_KEY_CAPTURE_TIME "CAPTURE_TIME"

#define     VALID_PRICE		4000

#define  APP_NAME _T("TradeAssist")
#define  STRING_REG_CODE _T("REGCODE")