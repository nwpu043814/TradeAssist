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

	LRESULT OnDeleteOrderMsg(WPARAM w , LPARAM l);
	LRESULT OnDoTradeMsg(WPARAM w , LPARAM l);

	void CheckEditPasteResult();

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
protected:
	// �Ƿ��Զ��ύ��
	BOOL mIsAutoSubmits;
public:
	// ��ɱ�µ�
	int OnFlashComplete(void);

	void SemicAutoTrade(int direct);
protected:
	// �Զ��µ����������������λΪ��
	UINT mAutoCompleteInterval;
public:
	// ͨ��˫�����ƻ�ñ༭�����ݡ�
	CString GetEditText(void);
protected:
	// ����������ϴ����ݡ�
	CString mLastClipboardContent;
public:
	WORD GetMilliseconds(void);
private:
	WORD mLastTime;
	CString mStrPriceDiff;
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
};
