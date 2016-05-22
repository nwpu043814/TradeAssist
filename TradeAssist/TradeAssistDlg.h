// TradeAssistDlg.h : ͷ�ļ�
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
#include "HuifengGuadanParam.h"
// CTradeAssistDlg �Ի���
class CTradeAssistDlg : public CDialog
{
// ����
public:
	CTradeAssistDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual	~CTradeAssistDlg();	// ��׼xigou����

// �Ի�������
	enum { IDD = IDD_TRADEASSIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void InstallHotKey();
private:
	int ParseHotKey(UINT mode, UINT virKey);
	// �����λ�¼�
	int dispatchLowAction(void);
	// �����λ�¼�
	int dispatchHighAction(void);
	// ����������Ŀ
	int dispatchCount(void);
	CActionManager* mActionManager;
	LRESULT OnDeleteOrderMsg(WPARAM w , LPARAM l);
	LRESULT OnAltDMsg(WPARAM w , LPARAM l);
	// ��ü����������
	CString GetContentFromClipboard(void);
	// ���ü����������
	int InitialSetting(void);
	int SaveSetting(void);
protected:
	// ��ȡ�۸������ؼ���λ������
	POINT GetPrice2CountVector(BOOL isHigh);
	// ���򵽼۸��λ��
	POINT GetDirection2PriceVector(BOOL isHigh);
	// �Ƿ��Զ��ύ��
	BOOL mIsAutoSubmits;
protected:
	// �Զ��µ����������������λΪ��
	CString mAutoCompleteInterval;
private:
	WORD mLastTime;
	CString mStrHighPriceDiff;
	UINT mIntOrderCount;
	// ��Ϣ��ʱ ��λ����
	UINT mIntMsgDelayMilliSeconds;
private:
	UINT mIntHour;
	UINT mIntMinute;
	CString mIntSecond;
	CHttpWorkerP mHttpWorker;
private:
	CDataKP mDataK;
	// �Ƿ��������Զ�ƽ��ʱ��
	BOOL mEnableCheckAutoCloseDepot;
	CString mStrLowPriceDiff;
	double mDataKClose;
	double mDataKOpen;
	CString mDataKCloseTime;
	CString mDataKOpenTime;
	double mDataKHighPrice;
	double mDataKLowPrice;
	LRESULT OnDisplayDataK(WPARAM w, LPARAM l);
	// �Զ�ƽ����ֵ
	UINT mUintAutoCloseThreshold;
	double mDataKStatisticsUpdrop;
	int mDataKDayUpdrop;
	// ��ǰ�ص�����
	int mDataKCurrent2ExtremeDiff;
	// �Զ�ƽ��Ԥ��
	CProgressCtrl mProgressAutoCloseDepot;
	int mOpenDirection;
	CString mServerIp;
	CLuaEngine mLuaEngine;
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	HRESULT  OnHotKey(WPARAM w, LPARAM l);
	LRESULT CheckAutoCloseDepot( CDataPacketP packet, PEcnomicData data ) ;
	// ���sun�Ի������Ͻǵľ������ꡣ
	POINT GetSunAwtDialogPos(void);

	// ָ��ί�е�����
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
	float mQueryPriceUseTime;
	float mCapturePriceUseTime;
	long mUILastUpdateTime;
public:
	afx_msg void OnBnClickedCheckSetMostTop();
private:
	BOOL mEnableWindowMostTop;
	void EnableWindowMostTop(BOOL isTop);
public:
	CHuifengGuadanParamP GetGuaDanParam(int dirct);
	int StartUpdateFeiNongData(void);
};
