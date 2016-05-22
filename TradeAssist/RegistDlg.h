#pragma once


// RegistDlg 对话框

class RegistDlg : public CDialog
{
	DECLARE_DYNAMIC(RegistDlg)

public:
	RegistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RegistDlg();
	CString	m_strRegCode;
	CString	m_strUserID;
	afx_msg void OnBnClickedOk();

	static BOOL		mIntResult ;
// 对话框数据
	enum { IDD = IDD_DIALOG_REGIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	DWORD GetInterval(void);
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnBnClickedCcheck();
	int SaveRegCode(CString w);
	CString GetRegCode(void);
	BOOL Explorer(CString p);
};
