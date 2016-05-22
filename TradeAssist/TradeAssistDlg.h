// TradeAssistDlg.h : ͷ�ļ�
//

#pragma once


// CTradeAssistDlg �Ի���
class CTradeAssistDlg : public CDialog
{
// ����
public:
	CTradeAssistDlg(CWnd* pParent = NULL);	// ��׼���캯��

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
	int ParseHotKey(UINT mode, UINT virKey);
	// �����λ�¼�
	int dispatchLowAction(void);
	// �����λ�¼�
	int dispatchHighAction(void);
	// ����������Ŀ
	int dispatchCount(void);
};
