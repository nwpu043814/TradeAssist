// TradeAssistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TradeAssist.h"
#include "TradeAssistDlg.h"
#include "Constant.h"
#include "SimulateAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTradeAssistDlg �Ի���




CTradeAssistDlg::CTradeAssistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeAssistDlg::IDD, pParent)
	, mIsAutoSubmits(FALSE)
	, mAutoCompleteInterval(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mAction = new SimulateAction();
}

void CTradeAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PRICE_DIFF, mEditPriceDiff);
	DDX_Control(pDX, IDC_EDIT_COUNT, mCount);
	DDX_Control(pDX, IDC_EDIT_HIGH_DIRECTION_PRICE_DX, mHighDirect2PriceDx);
	DDX_Control(pDX, IDC_EDIT_HIGH_DIRECTION_PRICE_DY, mHighDirect2PriceDy);
	DDX_Control(pDX, IDC_EDIT_HIGH_PRICE_COUNT_DX, mHighPrice2CountDx);
	DDX_Control(pDX, IDC_EDIT_HIGH_PRICE_COUNT_DY, mHighPrice2CountDy);
	DDX_Control(pDX, IDC_EDIT_LOW_DIRECTION_PRICE_DX, mLowDirect2PriceDx);
	DDX_Control(pDX, IDC_EDIT_LOW_DIRECTION_PRICE_DY, mLowDirect2PriceDy);
	DDX_Control(pDX, IDC_EDIT_LOW_PRICE_COUNT_DX, mLowPrice2CountDx);
	DDX_Control(pDX, IDC_EDIT_LOW_PRICE_COUNT_DY, mLowPrice2CountDy);
	DDX_Control(pDX, IDC_EDIT_COUNT_CONFIRM_DX, mCount2ConfirmDx);
	DDX_Control(pDX, IDC_EDIT_COUNT_CONFIRM_Dy, mCount2ConfirmDy);
	DDX_Control(pDX, IDC_EDIT_LOW_TAB_DIRECTION_DX, mLowTab2DirectionDx);
	DDX_Control(pDX, IDC_EDIT_LOW_TAB_DIRECTION_DY, mLowTab2DirectionDy);
	DDX_Control(pDX, IDC_EDIT_HIGH_TAB_DIRECTION_DX, mHighTab2DirectionDx);
	DDX_Control(pDX, IDC_EDIT_HIGH_TAB_DIRECTION_DY, mHighTab2DirectionDy);
	DDX_Control(pDX, IDC_EDIT_LEFT_TOP_TAB_DX, mStart2TabDx);
	DDX_Control(pDX, IDC_EDIT_LEFT_TOP_TAB_DY, mStart2TabDy);
	DDX_Check(pDX, IDC_CHECK_AOTO_SUBMIT, mIsAutoSubmits);
	DDX_Text(pDX, IDC_EDIT_AUTO_COMPLETE_INTERVAL, mAutoCompleteInterval);
	DDV_MaxChars(pDX, mAutoCompleteInterval, 3);
}

BEGIN_MESSAGE_MAP(CTradeAssistDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_HOTKEY,OnHotKey) //��Ӵ˾�
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DO_TRADE, OnDoLowMsg)
	ON_MESSAGE(WM_DO_COUNT, OnDoCountMsg)
	ON_BN_CLICKED(IDCANCEL, &CTradeAssistDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CTradeAssistDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTradeAssistDlg ��Ϣ�������

BOOL CTradeAssistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InstallHotKey();
	InitialSetting();
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTradeAssistDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTradeAssistDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTradeAssistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTradeAssistDlg::InstallHotKey()
{

	::RegisterHotKey(m_hWnd, HOT_KEY_CODE_LOW, MOD_WIN, VK_Z);
	::RegisterHotKey(m_hWnd, HOT_KEY_CODE_HIGH, MOD_WIN, VK_X);
	::RegisterHotKey(m_hWnd, HOT_KEY_CODE_COUNT, MOD_WIN, VK_C);
	::RegisterHotKey(m_hWnd, HOT_KEY_FLASH_COMPLETE, MOD_WIN, VK_A);

}
void CTradeAssistDlg::OnBnClickedCancel()
{

	ClearResource();
	OnCancel();
}

void CTradeAssistDlg::OnBnClickedOk()
{
	ClearResource();
	OnOK();
	
}

HRESULT  CTradeAssistDlg::OnHotKey(WPARAM w, LPARAM lParam)
{

	UINT Mod = (UINT) LOWORD(lParam); // key-modifier flags 
	UINT uVirtKey = (UINT) HIWORD(lParam); // virtual-key code 

	int result = ParseHotKey(Mod, uVirtKey);

	switch(result)
	{
		case HOT_KEY_CODE_LOW:
		{
			dispatchLowAction();
			break;
		}
		case HOT_KEY_CODE_HIGH:
		{
			dispatchHighAction();
			break;
		}
		case HOT_KEY_CODE_COUNT:
		{
			 dispatchCount();
			break;
		}
		case HOT_KEY_FLASH_COMPLETE:
		{
			OnFlashComplete();
			break;;
		}

	}


	return 1;
}


/**
*	�����ȼ����͡�
*/
int CTradeAssistDlg::ParseHotKey(UINT mode, UINT virKey)
{

	if (mode == MOD_WIN)
	{
		if (virKey == VK_Z)
		{
			return HOT_KEY_CODE_LOW;
		}
		else if (virKey == VK_X)
		{
			return HOT_KEY_CODE_HIGH;
		}
		else if (virKey == VK_C)
		{
			return HOT_KEY_CODE_COUNT;
		}
		else if (virKey == VK_A)
		{
			return HOT_KEY_FLASH_COMPLETE;
		}

	}

	return VK_INVALID;
}

int CTradeAssistDlg::dispatchLowAction(void)
{

	this->PostMessage(WM_DO_TRADE, DO_LOW, MSG_DELAY_YES);

	return 0;
}

int CTradeAssistDlg::dispatchHighAction(void)
{
	this->PostMessage(WM_DO_TRADE, DO_HIGH, MSG_DELAY_YES);
	return 0;
}

int CTradeAssistDlg::dispatchCount(void)
{
	this->PostMessage(WM_DO_COUNT);
	AfxMessageBox("HOT_KEY_CODE_COUNT");
	return 0;
}

LRESULT CTradeAssistDlg::OnDoLowMsg(WPARAM w , LPARAM l)
{
	
	BOOL direction = (UINT) LOWORD(w)== DO_HIGH? TRUE:FALSE;
	BOOL isDelay = (UINT) LOWORD(l)== MSG_DELAY_YES? TRUE:FALSE;

	if (isDelay)
	{
		Sleep(MSG_DELAY);
	}
	UpdateData(TRUE);

	//���ί��tab��
	POINT start = GetSunAwtDialogPos();
	if (start.x == 0)
	{
		//δ���ҵ�sun�Ի���
		return LRESULT();
	}
	mAction->MoveCursor(start.x,start.y, true);

	//��ԭ���ƶ���ָ��ί��tab��
	POINT start2Tab = GetStart2TabVector(); 
	mAction->MoveCursor(start2Tab.x,start2Tab.y);
	mAction->MouseClick();

	//ָ��ί�е�����
	POINT tab2Direction = GetTab2Direction(direction);
	mAction->MoveCursor(tab2Direction.x,tab2Direction.y);
	mAction->MouseClick();

	//�ӷ����Ƶ��۸�ؼ�
	POINT direction2PriceVector = GetDirection2PriceVector(direction);
	mAction->MoveCursor(direction2PriceVector.x,direction2PriceVector.y);

	//���Ԥ�Ƶĵ��
	CString outText;
	mEditPriceDiff.GetWindowText(outText);
	float diff = atof(outText);

	//ȡ�õ�ǰ�۸�
	CString text = GetEditText();
	float newCount = direction?atof(text) + diff : atof(text) - diff ;
	outText.Format("%.2f",newCount);

	//���ü��������ݲ�ճ��������
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	//�ƶ������������Ŀؼ�
	POINT price2CountVector = GetPrice2CountVector(direction);
	mAction->MoveCursor(price2CountVector.x,price2CountVector.y);

	//���½�������
	mAction->MouseDoubleClick();
	EmptyClipboard(); 
	SetClipboardContent(GetCount());
	mAction->KeyboardPaste();

	//�ƶ���ȷ����ť��
	POINT count2ConfirmVector = GetCount2ConfirmVector();
	mAction->MoveCursor(count2ConfirmVector.x,count2ConfirmVector.y);

	//�Զ��ύ
	if(mIsAutoSubmits)
	{
		mAction->MouseClick();
	}

	return LRESULT();
}

// ��ü����������
CString CTradeAssistDlg::GetContentFromClipboard(void)
{

	char *buffer = NULL;
	
	CString fromClipboard;
	if (OpenClipboard())
	{
		HANDLE hData = GetClipboardData(CF_TEXT);  
		char * buffer = (char*)GlobalLock(hData);  
		fromClipboard = buffer;  GlobalUnlock(hData);  
		CloseClipboard();
	}

	return fromClipboard;
}

// ���ü����������
BOOL CTradeAssistDlg::SetClipboardContent(CString source)
{
	if( OpenClipboard() ) 
	{  
		HGLOBAL clipbuffer; 
		char * buffer;  
		EmptyClipboard(); 
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
		buffer = (char*)GlobalLock(clipbuffer);  strcpy_s(buffer, source.GetLength()+1,  LPCSTR(source));
		GlobalUnlock(clipbuffer);  SetClipboardData(CF_TEXT,clipbuffer); 
		CloseClipboard();

		return 1;
	}
	return 0;
}

CTradeAssistDlg::~CTradeAssistDlg()
{


}

int CTradeAssistDlg::InitialSetting(void)
{
	CString outText;
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_PRICE_DIFF, 10));
	mEditPriceDiff.SetWindowText(outText);

	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_KEY_COUNT, 6));
	mCount.SetWindowText(outText);

	//���෽�򵽼۸�
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DX, 0));
	mHighDirect2PriceDx.SetWindowText(outText);
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DY, 0));
	mHighDirect2PriceDy.SetWindowText(outText);

	//����۸񵽽��ױ���
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DX, 0));
	mHighPrice2CountDx.SetWindowText(outText);
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DY, 0));
	mHighPrice2CountDy.SetWindowText(outText);

	//���շ��򵽼۸�
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DX, 0));
	mLowDirect2PriceDx.SetWindowText(outText);
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DY, 0));
	mLowDirect2PriceDy.SetWindowText(outText);

	//���ռ۸񵽽��ױ���
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DX, 0));
	mLowPrice2CountDx.SetWindowText(outText);
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DY, 0));
	mLowPrice2CountDy.SetWindowText(outText);

	//���ױ�����ȷ����ť
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DX, 0));
	mCount2ConfirmDx.SetWindowText(outText);
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DY, 0));
	mCount2ConfirmDy.SetWindowText(outText);
	
	//ԭ�㵽ָ��ί��
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_START_TAB_DX, 0));
	mStart2TabDx.SetWindowText(outText);
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_START_TAB_DY, 0));
	mStart2TabDy.SetWindowText(outText);

	//����ָ��ί�е�����
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DX, 0));
	mLowTab2DirectionDx.SetWindowText(outText);
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DY, 0));
	mLowTab2DirectionDy.SetWindowText(outText);

	//����ָ��ί�е�����
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DX, 0));
	mHighTab2DirectionDx.SetWindowText(outText);
	outText.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DY, 0));
	mHighTab2DirectionDy.SetWindowText(outText);

	mIsAutoSubmits = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, FALSE);
	mAutoCompleteInterval.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, 2));
	UpdateData(FALSE);

	return 0;
}

int CTradeAssistDlg::SaveSetting(void)
{
	UpdateData(TRUE);
	CString contant;
	mEditPriceDiff.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_PRICE_DIFF , atoi(contant));

	mCount.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_KEY_COUNT , atoi(contant));

	mHighDirect2PriceDx.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DX , atoi(contant));
	mHighDirect2PriceDy.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DY , atoi(contant));

	mHighPrice2CountDx.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DX , atoi(contant));
	mHighPrice2CountDy.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DY , atoi(contant));

	mLowDirect2PriceDx.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DX , atoi(contant));
	mLowDirect2PriceDy.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DY , atoi(contant));

	mLowPrice2CountDx.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DX , atoi(contant));
	mLowPrice2CountDy.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DY , atoi(contant));

	mCount2ConfirmDx.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DX , atoi(contant));
	mCount2ConfirmDy.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DY , atoi(contant));

	mStart2TabDx.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_START_TAB_DX , atoi(contant));
	mStart2TabDy.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_START_TAB_DY , atoi(contant));

	mLowTab2DirectionDx.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DX , atoi(contant));
	mLowTab2DirectionDy.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DY , atoi(contant));

	mHighTab2DirectionDx.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DX , atoi(contant));
	mHighTab2DirectionDy.GetWindowText(contant);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DY , atoi(contant));

	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, mIsAutoSubmits);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, atoi(mAutoCompleteInterval));
	return 0;
}

// ��ȡ�۸������ؼ���λ������
POINT CTradeAssistDlg::GetPrice2CountVector(BOOL isHigh)
{
	POINT point;
	CString buffer;

	if (isHigh)
	{
		mHighPrice2CountDx.GetWindowText(buffer);
		point.x = atoi(buffer);

		mHighPrice2CountDy.GetWindowText(buffer);
		point.y = atoi(buffer);
	}
	else
	{
		mLowPrice2CountDx.GetWindowText(buffer);
		point.x = atoi(buffer);

		mLowPrice2CountDy.GetWindowText(buffer);
		point.y = atoi(buffer);
	}

	return point;
}

// ���򵽼۸��λ��
POINT CTradeAssistDlg::GetDirection2PriceVector(BOOL isHigh)
{
	POINT point;
	CString buffer;

	if (isHigh)
	{
		mHighDirect2PriceDx.GetWindowText(buffer);
		point.x = atoi(buffer);

		mHighDirect2PriceDy.GetWindowText(buffer);
		point.y = atoi(buffer);
	}
	else
	{
		mLowDirect2PriceDx.GetWindowText(buffer);
		point.x = atoi(buffer);

		mLowDirect2PriceDy.GetWindowText(buffer);
		point.y = atoi(buffer);
	}

	return point;
}

// ���Ԥ�ƵĽ�������
CString CTradeAssistDlg::GetCount(void)
{
	CString buffer;
	mCount.GetWindowText(buffer);

	return buffer;
}

// ��������������ȷ����ť��Ԥ��λ�ơ�
POINT CTradeAssistDlg::GetCount2ConfirmVector(void)
{
	POINT point;
	CString buffer;

	mCount2ConfirmDx.GetWindowText(buffer);
	point.x = atoi(buffer);

	mCount2ConfirmDy.GetWindowText(buffer);
	point.y = atoi(buffer);
	
	return point;
}


LRESULT CTradeAssistDlg::OnDoCountMsg(WPARAM w , LPARAM l)
{

	return LRESULT();
}
// ���sun�Ի������Ͻǵľ������ꡣ
POINT CTradeAssistDlg::GetSunAwtDialogPos(void)
{
	HWND wnd=::FindWindow(SUN_DIALOG_NAME,NULL);
	POINT pos;


	if(wnd)
	{
		CRect rect;
		::GetWindowRect(wnd,rect);
		pos.x = rect.left;
		pos.y = rect.top;
		//::SendMessage(wnd,WM_CLOSE,0,0);
	}
	else
	{
		pos.x = 0;
		pos.y = 0;
		MessageBox("���Ҵ���ʧ�ܣ�");
	}

	return pos;
}

// ԭ�㵽ָ��ί�е�λ��
POINT CTradeAssistDlg::GetStart2TabVector(void)
{
	POINT point;
	CString buffer;

	mStart2TabDx.GetWindowText(buffer);
	point.x = atoi(buffer);

	mStart2TabDy.GetWindowText(buffer);
	point.y = atoi(buffer);

	return point;
}

// ָ��ί�е�����
POINT CTradeAssistDlg::GetTab2Direction(BOOL isHigh)
{
	POINT point;
	CString buffer;
	
	if (isHigh)
	{
		mHighTab2DirectionDx.GetWindowText(buffer);
		point.x = atoi(buffer);
		mHighTab2DirectionDy.GetWindowText(buffer);
		point.y = atoi(buffer);
	}
	else
	{
		mLowTab2DirectionDx.GetWindowText(buffer);
		point.x = atoi(buffer);
		mLowTab2DirectionDy.GetWindowText(buffer);
		point.y = atoi(buffer);
	}

	return point;
}

void CTradeAssistDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnClose();
}

int CTradeAssistDlg::ClearResource(void)
{
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_CODE_LOW);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_CODE_HIGH);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_CODE_COUNT);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_FLASH_COMPLETE);
	SaveSetting();
	delete mAction;
	return 0;
}

// ��ɱ�µ�
int CTradeAssistDlg::OnFlashComplete(void)
{
	POINT lpPoint;
	GetCursorPos(&lpPoint);
	mAction->MouseDoubleClick();
	UpdateData(TRUE);
	mIsAutoSubmits = TRUE;	
	UpdateData(FALSE);
	SemicAutoTrade(DO_LOW);
	//2.��ʱ�µ����
	Sleep(atoi(mAutoCompleteInterval)*1000);

	//3.�ƶ���굽˫��λ�á�
	SetCursorPos(lpPoint.x, lpPoint.y);
	mAction->MouseDoubleClick();
	SemicAutoTrade(DO_HIGH);
	return 0;
}

void CTradeAssistDlg::SemicAutoTrade(int direct) 
{

	//1.��ǰλ��˫�������µ��Ի���
	int searchCount = 10;
	while (searchCount-- > 0)
	{
		HWND wnd=::FindWindow(SUN_DIALOG_NAME,NULL);
		if (wnd)
		{
			SendMessage(WM_DO_TRADE, direct, MSG_DELAY_NO);
			break;;
		} 
		Sleep(WINDOW_CHECK_INTERVAL);
	}
}
// ͨ��˫�����ƻ�ñ༭�����ݡ�
CString CTradeAssistDlg::GetEditText(void)
{
	int doubleClickCount = 0;
	CString currentPrice;

	while(doubleClickCount++ < 2)
	{
		mAction->MouseDoubleClick();
		int copyCount = 0;
		while(copyCount++  < 2)
		{
			EmptyClipboard();
			mAction->KeyboardCopy();
			Sleep(copyCount*GET_CLIPBOARD_CONTENT_DELAY);
			currentPrice = GetContentFromClipboard();
			currentPrice = currentPrice.Trim();
			if (currentPrice.GetLength() < 4)
			{
				continue;			
			}
			else
			{
					doubleClickCount = 2;
					break;
			}
		}
	}

	return currentPrice;
}
