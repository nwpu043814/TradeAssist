// TradeAssistDlg.h : ͷ�ļ�
//

#pragma once
#include "csimulateaction.h"
#include "afxwin.h"
#include "chttpworker.h"
#include "DataK.h"
#include "HttpThread.h"
#include "afxcmn.h"


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
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	HRESULT  OnHotKey(WPARAM w, LPARAM l);

private:
	int ParseHotKey(UINT mode, UINT virKey);
	// �����λ�¼�
	int dispatchLowAction(void);
	// �����λ�¼�
	int dispatchHighAction(void);
	// ����������Ŀ
	int dispatchCount(void);
	CSimulateActionP mAction;

	LRESULT OnDeleteOrderMsg(WPARAM w , LPARAM l);
	LRESULT OnDoTradeMsg(WPARAM w , LPARAM l);


	BOOL CheckEditPasteResult();

	// ��ü����������
	CString GetContentFromClipboard(void);
	// ���ü����������
	BOOL SetClipboardContent(CString content);
	int InitialSetting(void);
	int SaveSetting(void);
protected:
	// ��ȡ�۸������ؼ���λ������
	POINT GetPrice2CountVector(BOOL isHigh);
	// ���򵽼۸��λ��
	POINT GetDirection2PriceVector(BOOL isHigh);
public:
	// ���sun�Ի������Ͻǵľ������ꡣ
	POINT GetSunAwtDialogPos(void);

	// ָ��ί�е�����
	POINT GetTab2Direction(BOOL isHigh);
	int ClearResource(void);
		LRESULT OnDoHttpGetFinish(WPARAM w , LPARAM l);
protected:
	// �Ƿ��Զ��ύ��
	BOOL mIsAutoSubmits;
public:
	// ��ɱ�µ�
	int OnFlashComplete(void);

	LRESULT SemicAutoTrade(int direct);
protected:
	// �Զ��µ����������������λΪ��
	UINT mAutoCompleteInterval;
public:
	// ͨ��˫�����ƻ�ñ༭�����ݡ�
	CString GetEditText(BOOL needDoubleClick = TRUE);
protected:
	// ����������ϴ����ݡ�
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
	// ��Ϣ��ʱ ��λ����
	UINT mIntMsgDelayMilliSeconds;
public:
	// ��ռ�����
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
	// �Ƿ��������Զ�ƽ��ʱ��
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
	// �Զ�ƽ����ֵ
	UINT mUintAutoCloseThreshold;
	int mDataKStatisticsUpdrop;
	int mDataKDayUpdrop;
	// ��ǰ�ص�����
	int mDataKCurrent2ExtremeDiff;
	// ͳ�Ʒ����ȫ�췽��ƥ�䡣
	BOOL mDataKDirectionAgree;
	// �Զ�ƽ��Ԥ��
	CProgressCtrl mProgressAutoCloseDepot;
	int mOpenDirection;
};
