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
#include "hotkeymanager.h"
#include "DataManager.h"
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
private:

	// 处理低位事件
	int dispatchLowAction(void);
	// 处理高位事件
	int dispatchHighAction(void);
	CActionManager* mActionManager;
	LRESULT OnAltDMsg(WPARAM w , LPARAM l);

	// 设置剪贴板的内容
	int InitialSetting(void);
	int SaveSetting(void);

	
private:

	CHttpWorkerP mHttpWorker;
private:
	CDataKP mDataK;
	
	LRESULT OnDisplayDataK(WPARAM w, LPARAM l);

	// 自动平仓预警
	CProgressCtrl mProgressAutoCloseDepot;
	
	CLuaEngine mLuaEngine;
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	HRESULT  OnHotKey(WPARAM w, LPARAM l);
	LRESULT CheckAutoCloseDepot( CDataPacketP packet, PEcnomicData data ) ;
	// 获得sun对话框右上角的绝对坐标。
	POINT GetSunAwtDialogPos(void);

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

	UINT GetServerPort(void);
	CString GetServerHost(void);
	void RetartThread(PEcnomicData);	
	
public:
	afx_msg void OnBnClickedCheckSetMostTop();
private:

	void EnableWindowMostTop(BOOL isTop);
public:
	int StartUpdateFeiNongData(void);
private:
	CHotKeyManager* mHotkeyManager;
	CDataManager* mDataManager;
};
