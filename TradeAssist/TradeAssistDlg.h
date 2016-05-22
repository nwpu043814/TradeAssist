// TradeAssistDlg.h : ͷ�ļ�
//

#pragma once
#include "simulateaction.h"
#include "afxwin.h"


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
	SimulateAction * mAction;

	LRESULT OnDoCountMsg(WPARAM w , LPARAM l);
	LRESULT OnDoLowMsg(WPARAM w , LPARAM l);
	// ��ü����������
	CString GetContentFromClipboard(void);
	// ���ü����������
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

	// ��ȡ�۸������ؼ���λ������
	POINT GetPrice2CountVector(BOOL isHigh);
	// ���򵽼۸��λ��
	POINT GetDirection2PriceVector(BOOL isHigh);
	// ���Ԥ�Ƶ�����
	CString GetCount(void);
	CEdit mCount2ConfirmDx;
	CEdit mCount2ConfirmDy;
public:
	// ��������������ȷ����ť��Ԥ��λ�ơ�
	POINT GetCount2ConfirmVector(void);
protected:
	CEdit mLowTab2DirectionDx;
	CEdit mLowTab2DirectionDy;
	CEdit mHighTab2DirectionDx;
	CEdit mHighTab2DirectionDy;
	CEdit mStart2TabDx;
	CEdit mStart2TabDy;
public:
	// ���sun�Ի������Ͻǵľ������ꡣ
	POINT GetSunAwtDialogPos(void);
protected:
	// ԭ�㵽ָ��ί�е�λ��
	POINT GetStart2TabVector(void);
public:
	// ָ��ί�е�����
	POINT GetTab2Direction(BOOL isHigh);
public:
	afx_msg void OnClose();
	int ClearResource(void);
protected:
	// �Ƿ��Զ��ύ��
	BOOL mIsAutoSubmits;
	BOOL mFlashComplete;
public:
	// ��ɱ�µ�
	int OnFlashComplete(void);

	void SemicAutoTrade(int direct);
};
