// TradeAssistDlg.h : 头文件
//

#pragma once
#include "csimulateaction.h"
#include "afxwin.h"
#include "chttpworker.h"
#include "DataK.h"
#include "HttpThread.h"
#include "afxcmn.h"


// CTradeAssistDlg 对话框
class CTradeAssistDlg : public CDialog
{
// 构造
public:
	CTradeAssistDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual	~CTradeAssistDlg();	// 标准xigou函数

// 对话框数据
	enum { IDD = IDD_TRADEASSIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void InstallHotKey();
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	HRESULT  OnHotKey(WPARAM w, LPARAM l);

private:
	int ParseHotKey(UINT mode, UINT virKey);
	// 处理低位事件
	int dispatchLowAction(void);
	// 处理高位事件
	int dispatchHighAction(void);
	// 处理交易手数目
	int dispatchCount(void);
	CSimulateActionP mAction;

	LRESULT OnDeleteOrderMsg(WPARAM w , LPARAM l);
	LRESULT OnDoTradeMsg(WPARAM w , LPARAM l);


	BOOL CheckEditPasteResult();

	// 获得剪贴板的内容
	CString GetContentFromClipboard(void);
	// 设置剪贴板的内容
	BOOL SetClipboardContent(CString content);
	int InitialSetting(void);
	int SaveSetting(void);
protected:
	// 获取价格到手数控件的位移设置
	POINT GetPrice2CountVector(BOOL isHigh);
	// 方向到价格的位移
	POINT GetDirection2PriceVector(BOOL isHigh);
public:
	// 获得sun对话框右上角的绝对坐标。
	POINT GetSunAwtDialogPos(void);

	// 指价委托到方向
	POINT GetTab2Direction(BOOL isHigh);
	int ClearResource(void);
		LRESULT OnDoHttpGetFinish(WPARAM w , LPARAM l);
protected:
	// 是否自动提交。
	BOOL mIsAutoSubmits;
public:
	// 秒杀下单
	int OnFlashComplete(void);

	LRESULT SemicAutoTrade(int direct);
protected:
	// 自动下单的两个单间隔，单位为秒
	UINT mAutoCompleteInterval;
public:
	// 通过双击复制获得编辑框内容。
	CString GetEditText(BOOL needDoubleClick = TRUE);
protected:
	// 保存剪贴板上次内容。
	CString mLastClipboardContent;
public:
	WORD GetMilliseconds(void);
private:
	WORD mLastTime;
	CString mStrHighPriceDiff;
	UINT mIntOrderCount;
	int mIntLowTab2DirectDx;
	UINT mIntLowTab2DirectDy;
	UINT mIntLowDirect2PriceDx;
	UINT mIntLowDirect2PriceDy;
	INT mIntLowPrice2CountDx;
	UINT mIntLowPrice2CountDy;
	int mIntHighTab2DirectDx;
	UINT mIntHighTab2DirectDy;
	int mIntHighDirect2PriceDx;
	int mIntHighDirect2PriceDy;
	int mIntCount2ConfirmDx;
	int mIntCount2ConfirmDy;
	UINT mIntStart2TabDx;
	UINT mIntStart2TabDy;
	int mIntHighPrice2CountDx;
	int mIntHighPrice2CountDy;
	UINT mIntStart2DeleteOrderDx;
	UINT mIntStart2DeleteOrderDy;
	int mIntDelete2ConfirmDx;
	int mIntDelete2ConfirmDy;
	// 消息延时 单位毫秒
	UINT mIntMsgDelayMilliSeconds;
public:
	// 清空剪贴板
	BOOL GlearClipboard(void);
	int UpdatePrice(bool isAdd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	UINT mIntHour;
	UINT mIntMinute;
	UINT mIntSecond;
	CHttpWorkerP mHttpWorker;
public:
	afx_msg void OnBnClickedButtonStartTimer();
private:
	CDataKP mDataK;
	CHttpThreadP mHttpThread;
	// 是否允许检测自动平仓时机
	BOOL mEnableChckAutoCloseDepot;
	CString mStrLowPriceDiff;
	UINT mDataKClose;
	UINT mDataKOpen;
	CString mDataKCloseTime;
	CString mDataKOpenTime;
	UINT mDataKHighPrice;
	UINT mDataKLowPrice;
	LRESULT OnDisplayDataK(WPARAM w, LPARAM l);
	HANDLE mThreadHandle;
	UINT mThreadID;
	// 自动平仓阈值
	UINT mUintAutoCloseThreshold;
	int mDataKStatisticsUpdrop;
	int mDataKDayUpdrop;
	// 当前回调幅度
	int mDataKCurrent2ExtremeDiff;
	// 统计方向和全天方向匹配。
	BOOL mDataKDirectionAgree;
	// 自动平仓预警
	CProgressCtrl mProgressAutoCloseDepot;
	int mOpenDirection;
};
