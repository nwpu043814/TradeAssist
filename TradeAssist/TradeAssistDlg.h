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
#include "hotkeymanager.h"
#include "DataManager.h"
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
private:

	// �����λ�¼�
	int dispatchLowAction(void);
	// �����λ�¼�
	int dispatchHighAction(void);
	CActionManager* mActionManager;
	LRESULT OnAltDMsg(WPARAM w , LPARAM l);

	// ���ü����������
	int InitialSetting(void);
	int SaveSetting(void);

	
private:

	CHttpWorkerP mHttpWorker;
private:
	CDataKP mDataK;
	
	LRESULT OnDisplayDataK(WPARAM w, LPARAM l);

	// �Զ�ƽ��Ԥ��
	CProgressCtrl mProgressAutoCloseDepot;
	
	CLuaEngine mLuaEngine;
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	HRESULT  OnHotKey(WPARAM w, LPARAM l);
	LRESULT CheckAutoCloseDepot( CDataPacketP packet, PEcnomicData data ) ;
	// ���sun�Ի������Ͻǵľ������ꡣ
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
