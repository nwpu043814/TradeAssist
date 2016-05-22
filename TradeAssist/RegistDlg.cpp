// RegistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TradeAssist.h"
#include "RegistDlg.h"
#include "Constant.h"


// RegistDlg 对话框

IMPLEMENT_DYNAMIC(RegistDlg, CDialog)

RegistDlg::RegistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RegistDlg::IDD, pParent)
{
	m_strRegCode = _T("");
	m_strUserID = _T("");
}

void RegistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRegister)
	DDX_Text(pDX, IDC_EDIT_PC_CODE, m_strUserID);
	DDX_Text(pDX, IDC_EDIT_REGIST_CODE, m_strRegCode);
	//}}AFX_DATA_MAP
}

int RegistDlg::mIntResult = 0x47614300;

RegistDlg::~RegistDlg()
{
}


BEGIN_MESSAGE_MAP(RegistDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &RegistDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDCCHECK, &RegistDlg::OnBnClickedCcheck)
END_MESSAGE_MAP()


// RegistDlg 消息处理程序

void RegistDlg::OnBnClickedCancel()
{
	OnCancel();
}

DWORD RegistDlg::GetInterval(void)
{
	DWORD dwVolumeSerialNumber,dwUserID,dwDecodeNumber; 
	DWORD dwMask=85225111;  //运算子，随便设定

	CString strUserID,strRegCode;
	char str[200];
	int str_len=200;

	// 将硬盘序列号与运算子异或后做为用户号
	::GetVolumeInformation("C:\\",NULL,NULL,&dwVolumeSerialNumber,NULL,NULL,NULL,NULL);  
	dwUserID=dwVolumeSerialNumber^dwMask;  
	TRACE("1:%X", dwVolumeSerialNumber);
	for (int i = 0; i < 5; i ++)
	{
		dwVolumeSerialNumber ^= (dwMask << i);
	}

	return dwUserID;
}

void RegistDlg::PreInitDialog()
{
	int number = 3;
	int differ = RegistDlg::mIntResult;
	DWORD * codes  =  new DWORD[number] ;
	codes[0] = GetInterval();
	for (int i = 0; i < number ;i++)
	{
		codes[i] = codes[0] << (i) + (i*differ);
	}
	codes[number-1] = ~codes[number-1]; 
	m_strUserID.Format("%X-%X-%X", codes[0],codes[1],codes[2]);

	delete [] codes;
	CDialog::PreInitDialog();
}

void RegistDlg::OnBnClickedCcheck()
{
	UpdateData();
	Explorer(m_strRegCode);
	CDialog::OnOK();
}

int RegistDlg::SaveRegCode(CString w)
{
	::WriteProfileString(APP_NAME,STRING_REG_CODE,w);
	return 0;
}

CString RegistDlg::GetRegCode(void)
{

	char str[200];
	int str_len=200;
	CString strRegCode = "";
	if( ::GetProfileString( APP_NAME,STRING_REG_CODE,"",str,str_len)!=0 )
	{
		strRegCode=str;
	}

	return strRegCode;
}

BOOL RegistDlg::Explorer(CString p)
{
	int pos = p.Find('-');
	if (pos < 0 )
	{
		return 2;
	}

	DWORD valid = strtoul(p.Left(pos), NULL, 16);
	DWORD left = GetInterval();
	DWORD right = valid ^ RegistDlg::mIntResult; 
	if ( right == left)
	{
		SaveRegCode(p);
		RegistDlg::mIntResult = 0;
		return 0;
	}

	return 1;
}
