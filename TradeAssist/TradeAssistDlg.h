// TradeAssistDlg.h : 头文件
//

#pragma once
#include "simulateaction.h"
#include "afxwin.h"


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
	SimulateAction * mAction;

	LRESULT OnDoCountMsg(WPARAM w , LPARAM l);
	LRESULT OnDoLowMsg(WPARAM w , LPARAM l);
	// 获得剪贴板的内容
	CString GetContentFromClipboard(void);
	// 设置剪贴板的内容
	BOOL SetClipboardContent(CString content);
	CEdit mEditPriceDiff;
	int InitialSetting(void);
	int SaveSetting(void);
protected:
	CEdit mCount;
	CEdit mHighDirect2PriceDx;
	CEdit mHighDirect2PriceDy;
	CEdit mHighPrice2CountDx;
	CEdit mHighPrice2CountDy;
	CEdit mLowDirect2PriceDx;
	CEdit mLowDirect2PriceDy;
	CEdit mLowPrice2CountDx;
	CEdit mLowPrice2CountDy;

	// 获取价格到手数控件的位移设置
	POINT GetPrice2CountVector(BOOL isHigh);
	// 方向到价格的位移
	POINT GetDirection2PriceVector(BOOL isHigh);
	// 获得预制的手数
	CString GetCount(void);
	CEdit mCount2ConfirmDx;
	CEdit mCount2ConfirmDy;
public:
	// 返货交易手数到确定按钮的预制位移。
	POINT GetCount2ConfirmVector(void);
protected:
	CEdit mLowTab2DirectionDx;
	CEdit mLowTab2DirectionDy;
	CEdit mHighTab2DirectionDx;
	CEdit mHighTab2DirectionDy;
	CEdit mStart2TabDx;
	CEdit mStart2TabDy;
public:
	// 获得sun对话框右上角的绝对坐标。
	POINT GetSunAwtDialogPos(void);
protected:
	// 原点到指甲委托的位移
	POINT GetStart2TabVector(void);
public:
	// 指价委托到方向
	POINT GetTab2Direction(BOOL isHigh);
public:
	afx_msg void OnClose();
	int ClearResource(void);
protected:
	// 是否自动提交。
	BOOL mIsAutoSubmits;
	BOOL mFlashComplete;
public:
	// 秒杀下单
	int OnFlashComplete(void);

	void SemicAutoTrade(int direct);
};
