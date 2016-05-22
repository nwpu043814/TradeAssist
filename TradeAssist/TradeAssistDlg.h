// TradeAssistDlg.h : 头文件
//

#pragma once
#include "simulateaction.h"
#include "afxwin.h"
#include "httpworker.h"
#include "DataK.h"
#include "HttpThread.h"
#include "afxcmn.h"
#include "luaengine.h"
#include "EcnomicCompareResult.h"
#include "ActionManager.h"
#include "Resource.h"
#include "DataPacket.h"

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
private:
	int ParseHotKey(UINT mode, UINT virKey);
	// 处理低位事件
	int dispatchLowAction(void);
	// 处理高位事件
	int dispatchHighAction(void);
	// 处理交易手数目
	int dispatchCount(void);
	CActionManager* mActionManager;
	LRESULT OnDeleteOrderMsg(WPARAM w , LPARAM l);
	LRESULT OnDoTradeMsg(WPARAM w , LPARAM l);

	// 获得剪贴板的内容
	CString GetContentFromClipboard(void);
	// 设置剪贴板的内容
	int InitialSetting(void);
	int SaveSetting(void);
protected:
	// 获取价格到手数控件的位移设置
	POINT GetPrice2CountVector(BOOL isHigh);
	// 方向到价格的位移
	POINT GetDirection2PriceVector(BOOL isHigh);
	// 是否自动提交。
	BOOL mIsAutoSubmits;
public:
	// 秒杀下单
	int OnFlashComplete(void);
	LRESULT SemicAutoTrade(int direct);
protected:
	// 自动下单的两个单间隔，单位为秒
	CString mAutoCompleteInterval;
protected:
	// 保存剪贴板上次内容。
	CString mLastClipboardContent;
private:
	WORD mLastTime;
	CString mStrHighPriceDiff;
	UINT mIntOrderCount;
	// 消息延时 单位毫秒
	UINT mIntMsgDelayMilliSeconds;
private:
	UINT mIntHour;
	UINT mIntMinute;
	CString mIntSecond;
	CHttpWorkerP mHttpWorker;
private:
	CDataKP mDataK;
	CHttpThreadP mHttpThread;
	// 是否允许检测自动平仓时机
	BOOL mEnableCheckAutoCloseDepot;
	CString mStrLowPriceDiff;
	UINT mDataKClose;
	UINT mDataKOpen;
	CString mDataKCloseTime;
	CString mDataKOpenTime;
	UINT mDataKHighPrice;
	UINT mDataKLowPrice;
	LRESULT OnDisplayDataK(WPARAM w, LPARAM l);
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
	CString mServerIp;
	CLuaEngine mLuaEngine;
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	HRESULT  OnHotKey(WPARAM w, LPARAM l);
	LRESULT CheckAutoCloseDepot( CDataPacketP packet, PEcnomicData data ) ;
	// 获得sun对话框右上角的绝对坐标。
	POINT GetSunAwtDialogPos(void);

	// 指价委托到方向
	POINT GetTab2Direction(BOOL isHigh);
	int ClearResource(void);
	LRESULT OnHttpGetPriceFinish(WPARAM w , LPARAM l);

	WORD GetMilliseconds(void);
	int UpdatePrice(bool isAdd, float diff = 6.0F);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStartTimer();
	int CloseHttpThread(PEcnomicData data);
	UINT GetDoHttpInterval(void);
	int GetDynamicThreshold(int back);
	int PlaySoundResource(int idRes);
	CLuaEngine GetLuaEngine(void);
	LRESULT OnHttpGetEcnomicData(WPARAM w, LPARAM l);
	void DoEcnomicDataAction( PEcnomicData data ) ;
	void StartHttpThread(PEcnomicData data);
	UINT mServerPort;
	UINT GetServerPort(void);
	CString GetServerHost(void);
	void RetartThread(PEcnomicData);
private:
	UINT mUintDoHttpInterval;
	PEcnomicData *mNonfarmerNumber;
	PEcnomicData *mJoblessRate;
	PEcnomicData *mLocalPrice;
	BOOL mBoolEnableAutoThreshold;
	bool mIsTimer4Tomorrow;
	CString mActualNonfarmerNumber;
	CString mActualJoblessRate;
	CString mNonfarmerNumberResult;
	CString mJoblessRateResult;
	CString mNonfarmerNumnerWeight;
	CString mJoblessRateWeight;
	CString mNonfarmerNumberCount;
	CString mJoblessRateCount;

public:
	LRESULT OnDoChase(WPARAM wp, LPARAM lp);
private:
	BOOL mEnableChaseTimer;
	CString mTotalConclution;
	unsigned int mPullPriceCount;
	private:
	void CheckChaseMoment(CDataPacketP packet);
};
