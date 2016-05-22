// TradeAssistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TradeAssist.h"
#include "CTradeAssistDlg.h"
#include "Constant.h"
#include "CSimulateAction.h"
#include "CLogger.h"
#include "CHttpWorker.h"
#include "Util.h"
#include "DataPacket.h"

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
	, mAutoCompleteInterval(0)
	, mLastClipboardContent(_T(""))
	, mStrPriceDiff(_T(""))
	, mIntOrderCount(1)
	, mIntLowTab2DirectDx(0)
	, mIntLowTab2DirectDy(0)
	, mIntLowDirect2PriceDx(0)
	, mIntLowDirect2PriceDy(0)
	, mIntLowPrice2CountDx(0)
	, mIntLowPrice2CountDy(0)
	, mIntHighTab2DirectDx(0)
	, mIntHighTab2DirectDy(0)
	, mIntHighDirect2PriceDx(0)
	, mIntHighDirect2PriceDy(0)
	, mIntCount2ConfirmDx(0)
	, mIntCount2ConfirmDy(0)
	, mIntStart2TabDx(0)
	, mIntStart2TabDy(0)
	, mIntHighPrice2CountDx(0)
	, mIntHighPrice2CountDy(0)
	, mIntStart2DeleteOrderDx(0)
	, mIntStart2DeleteOrderDy(0)
	, mIntDelete2ConfirmDx(0)
	, mIntDelete2ConfirmDy(0)
	, mIntMsgDelayMilliSeconds(100)
	, mIntHour(0)
	, mIntMinute(0)
	, mIntSecond(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mAction = new CSimulateAction();
	mHttpWorker = new CHttpWorker();
	mDataK = new CDataK();
}

void CTradeAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AOTO_SUBMIT, mIsAutoSubmits);
	DDX_Text(pDX, IDC_EDIT_AUTO_COMPLETE_INTERVAL, mAutoCompleteInterval);
	DDV_MinMaxUInt(pDX, mAutoCompleteInterval, 0, 100);
	DDX_Text(pDX, IDC_EDIT_PRICE_DIFF, mStrPriceDiff);
	DDV_MaxChars(pDX, mStrPriceDiff, 4);
	DDX_Text(pDX, IDC_EDIT_COUNT, mIntOrderCount);
	DDV_MinMaxUInt(pDX, mIntOrderCount, 0, 100);
	DDX_Text(pDX, IDC_EDIT_LOW_TAB_DIRECTION_DX, mIntLowTab2DirectDx);
	DDV_MinMaxInt(pDX, mIntLowTab2DirectDx, -500, 500);
	DDX_Text(pDX, IDC_EDIT_LOW_TAB_DIRECTION_DY, mIntLowTab2DirectDy);
	DDV_MinMaxUInt(pDX, mIntLowTab2DirectDy, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_LOW_DIRECTION_PRICE_DX, mIntLowDirect2PriceDx);
	DDV_MinMaxUInt(pDX, mIntLowDirect2PriceDx, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_LOW_DIRECTION_PRICE_DY, mIntLowDirect2PriceDy);
	DDV_MinMaxUInt(pDX, mIntLowDirect2PriceDy, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_LOW_PRICE_COUNT_DX, mIntLowPrice2CountDx);
	DDV_MinMaxInt(pDX, mIntLowPrice2CountDx, -500, 500);
	DDX_Text(pDX, IDC_EDIT_LOW_PRICE_COUNT_DY, mIntLowPrice2CountDy);
	DDV_MinMaxUInt(pDX, mIntLowPrice2CountDy, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_HIGH_TAB_DIRECTION_DX, mIntHighTab2DirectDx);
	DDV_MinMaxInt(pDX, mIntHighTab2DirectDx, -500, 500);
	DDX_Text(pDX, IDC_EDIT_HIGH_TAB_DIRECTION_DY, mIntHighTab2DirectDy);
	DDV_MinMaxUInt(pDX, mIntHighTab2DirectDy, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_HIGH_DIRECTION_PRICE_DX, mIntHighDirect2PriceDx);
	DDV_MinMaxInt(pDX, mIntHighDirect2PriceDx, -500, 500);
	DDX_Text(pDX, IDC_EDIT_HIGH_DIRECTION_PRICE_DY, mIntHighDirect2PriceDy);
	DDV_MinMaxInt(pDX, mIntHighDirect2PriceDy, -500, 500);
	DDX_Text(pDX, IDC_EDIT_COUNT_CONFIRM_DX, mIntCount2ConfirmDx);
	DDV_MinMaxInt(pDX, mIntCount2ConfirmDx, 0, 500);
	DDX_Text(pDX, IDC_EDIT_COUNT_CONFIRM_Dy, mIntCount2ConfirmDy);
	DDV_MinMaxInt(pDX, mIntCount2ConfirmDy, 0, 500);
	DDX_Text(pDX, IDC_EDIT_LEFT_TOP_TAB_DX, mIntStart2TabDx);
	DDV_MinMaxUInt(pDX, mIntStart2TabDx, 0, 500);
	DDX_Text(pDX, IDC_EDIT_LEFT_TOP_TAB_DY, mIntStart2TabDy);
	DDV_MinMaxUInt(pDX, mIntStart2TabDy, 0, 500);
	DDX_Text(pDX, IDC_EDIT_HIGH_PRICE_COUNT_DX, mIntHighPrice2CountDx);
	DDV_MinMaxInt(pDX, mIntHighPrice2CountDx, -500, 500);
	DDX_Text(pDX, IDC_EDIT_HIGH_PRICE_COUNT_DY, mIntHighPrice2CountDy);
	DDV_MinMaxInt(pDX, mIntHighPrice2CountDy, -500, 500);
	DDX_Text(pDX, IDC_EDIT_START_DELETE_ORDER_DX, mIntStart2DeleteOrderDx);
	DDV_MinMaxUInt(pDX, mIntStart2DeleteOrderDx, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_START_DELETE_ORDER_DY, mIntStart2DeleteOrderDy);
	DDV_MinMaxUInt(pDX, mIntStart2DeleteOrderDy, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_DELETE_CONFIRM_DX, mIntDelete2ConfirmDx);
	DDV_MinMaxInt(pDX, mIntDelete2ConfirmDx, -500, 500);
	DDX_Text(pDX, IDC_EDIT_DELETE_CONFIRM_DY, mIntDelete2ConfirmDy);
	DDV_MinMaxInt(pDX, mIntDelete2ConfirmDy, -500, 500);
	DDX_Text(pDX, IDC_EDIT_MSG_DELAY, mIntMsgDelayMilliSeconds);
	DDV_MinMaxUInt(pDX, mIntMsgDelayMilliSeconds, 100, 500);
	DDX_Text(pDX, IDC_EDIT_HOUR, mIntHour);
	DDX_Text(pDX, IDC_EDIT_MINUTE, mIntMinute);
	DDX_Text(pDX, IDC_EDIT_SECOND, mIntSecond);
}

BEGIN_MESSAGE_MAP(CTradeAssistDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_HOTKEY,OnHotKey) //��Ӵ˾�
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DO_TRADE, OnDoTradeMsg)
	ON_MESSAGE(WM_DO_COUNT, OnDeleteOrderMsg)
	ON_BN_CLICKED(IDCANCEL, &CTradeAssistDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CTradeAssistDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_TIMER, &CTradeAssistDlg::OnBnClickedButtonStartTimer)
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
	::RegisterHotKey(m_hWnd, HOT_KEY_DECREASE_PRICE, MOD_SHIFT, VK_Z);
	::RegisterHotKey(m_hWnd, HOT_KEY_INCREASE_PRICE, MOD_SHIFT, VK_X);

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

UINT HttpThread(LPVOID param)
{
	CString * p = (CString *) param;

	*p = "lian";

	return 0;
}

HRESULT  CTradeAssistDlg::OnHotKey(WPARAM w, LPARAM lParam)
{

#ifdef _DEBUG
	mLastTime = GetMilliseconds();
#endif

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
		case HOT_KEY_DECREASE_PRICE:
		{
			UpdatePrice(false);
			break;
		}
		case HOT_KEY_INCREASE_PRICE:
		{
			//UpdatePrice(true);
			//CString result = mHttpWorker->DoGet(TEXT("jry.baidao.com"), 80, TEXT("/api/hq/npdata.do?ids=1&markettype=ttj"));
			//MessageBox(result);

			CDataPacket packet;
			CString text = _T("[{\"percent\":\"-1.65%\",\"updatetime\":\"20130322 23:20:55\",\"nickname\":\"XAGUSD\",\"buy\":\"5794\",\"status\":true,\"updrop\":\"-97\",\"close\":\"5784\",\"id\":1,\"open\":\"5883\",\"sell\":\"5784\",\"now\":\"5784\",\"pricetime\":\"20130322 23:20:54\",\"name\":\"�ֻ�����\",\"high\":\"5888\",\"low\":\"5757\",\"preclose\":\"5881\",\"avg\":\"5836.99\",\"amplitude\":\"2.23%\"}]");
			CUtil::ParseDataString(text,packet);
			CString out;
			out.Format("updrop=%d,status=%d",packet.mUpDrop, packet.mIsGood );

			int diff = CUtil::GetTimeSpan(_T("20130322 22:48:44"), _T("20130322 22:47:43"));
			out.Format("diff=%d", diff);
			MessageBox(out);


			AfxBeginThread(HttpThread, &out);
			MessageBox(out);
			break;
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
	else if (mode == MOD_SHIFT)
	{
		if (virKey == VK_Z)
		{
			return HOT_KEY_DECREASE_PRICE;
		}
		else if (virKey == VK_X)
		{
			return HOT_KEY_INCREASE_PRICE;
		}
	}

	return VK_INVALID;
}

int CTradeAssistDlg::dispatchLowAction(void)
{
	PostMessage(WM_DO_TRADE, DO_LOW, MSG_DELAY_YES);
	return 0;
}

int CTradeAssistDlg::dispatchHighAction(void)
{
	PostMessage(WM_DO_TRADE, DO_HIGH, MSG_DELAY_YES);
	return 0;
}

int CTradeAssistDlg::dispatchCount(void)
{
	this->PostMessage(WM_DO_COUNT);
	return 0;
}

LRESULT CTradeAssistDlg::OnDoTradeMsg(WPARAM w , LPARAM l)
{
	BOOL direction = (UINT) LOWORD(w)== DO_HIGH? TRUE:FALSE;
	BOOL isDelay = (UINT) LOWORD(l)== MSG_DELAY_YES? TRUE:FALSE;

	TRACE("OnDoTradeMsg time=%d, direction=%d, isDelay=%d\r\n", GetMilliseconds() - mLastTime,direction, isDelay);
#ifdef _DEBUG
	WORD	startTime = GetMilliseconds();
#endif // _DEBUG

	if (isDelay)
	{
		Sleep(mIntMsgDelayMilliSeconds);
	}
	UpdateData(TRUE);

	//���ί��tab��
	POINT start = GetSunAwtDialogPos();
	if (start.x == 0 && start.y == 0)
	{
		CLogger::Add("not find sunawtdialog");
		//δ���ҵ�sun�Ի���
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}
	mAction->MoveCursor(start.x,start.y, true);

	//��ԭ���ƶ���ָ��ί��tab��
	mAction->MoveCursor(mIntStart2TabDx,mIntStart2TabDy);
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
	float diff = atof(mStrPriceDiff);

	//ȡ�õ�ǰ�۸�
	CString text = GetEditText();
	if(text.Find(_T("."))  == -1)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE;
	}

	float newCount = direction?atof(text) + diff : atof(text) - diff ;
	outText.Format("%.2f",newCount);

	CString log;
	log.Format("originalprice=%s, diff=%f, newPrice=%s, direction=%d", text, diff, outText, direction);
	CLogger::Add(log);

	//�����Ա����
	mLastClipboardContent = outText;

	//���ü��������ݲ�ճ��������
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	if(!CheckEditPasteResult())
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	//�ƶ������������Ŀؼ�
	POINT price2CountVector = GetPrice2CountVector(direction);
	mAction->MoveCursor(price2CountVector.x,price2CountVector.y);

	//���½�������
	mAction->MouseDoubleClick();
	outText.Format("%d", mIntOrderCount);
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	//�ƶ���ȷ����ť��
	mAction->MoveCursor(mIntCount2ConfirmDx,mIntCount2ConfirmDy);

	//�Զ��ύ
	if(mIsAutoSubmits)
	{
		mAction->MouseClick();
	}

#ifdef _DEBUG
	TRACE("OnDoTradeMsg submit time=%d\r\n", GetMilliseconds() - startTime);
#endif

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
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
	delete mAction;
	delete mHttpWorker;
	delete mDataK;
}

int CTradeAssistDlg::InitialSetting(void)
{
	CString outText;
	mStrPriceDiff.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_PRICE_DIFF, 10));
	mIntOrderCount = theApp.GetProfileInt(STRING_SETTING, STRING_KEY_COUNT, 6);

	//���෽�򵽼۸�
	mIntHighDirect2PriceDx = theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DX, 0);
	mIntHighDirect2PriceDy = theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DY, 0);
	
	//����۸񵽽��ױ���
	mIntHighPrice2CountDx = theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DX, 0);
	mIntHighPrice2CountDy = theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DY, 0);

	//���շ��򵽼۸�
	mIntLowDirect2PriceDx = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DX, 0);
	mIntLowDirect2PriceDy = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DY, 0);

	//���ռ۸񵽽��ױ���
	mIntLowPrice2CountDx = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DX, 0);
	mIntLowPrice2CountDy = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DY, 0);	

	//���ױ�����ȷ����ť
	mIntCount2ConfirmDx = theApp.GetProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DX, 0);
	mIntCount2ConfirmDy = theApp.GetProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DY, 0);
	
	//ԭ�㵽ָ��ί��
	mIntStart2TabDx = theApp.GetProfileInt(STRING_SETTING, STRING_START_TAB_DX, 0);
	mIntStart2TabDy = theApp.GetProfileInt(STRING_SETTING, STRING_START_TAB_DY, 0);

	//����ָ��ί�е�����
	mIntLowTab2DirectDx = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DX, 0);
	mIntLowTab2DirectDy = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DY, 0);

	//����ָ��ί�е�����
	mIntHighTab2DirectDx =theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DX, 0);
	mIntHighTab2DirectDy =theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DY, 0);

	//ɾ������ԭ����ɾ��
	mIntStart2DeleteOrderDx =theApp.GetProfileInt(STRING_SETTING, STRING_START_DELETE_ORDER_DX, 0);
	mIntStart2DeleteOrderDy =theApp.GetProfileInt(STRING_SETTING, STRING_START_DELETE_ORDER_DY, 0);

	//ɾ����ȷ��
	mIntDelete2ConfirmDx =theApp.GetProfileInt(STRING_SETTING, STRING_DELETE_CONFIRM_DX, 0);
	mIntDelete2ConfirmDy =theApp.GetProfileInt(STRING_SETTING, STRING_DELETE_CONFIRM_DY, 0);

	mIsAutoSubmits = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, FALSE);
	mAutoCompleteInterval = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, 2);

	mIntMsgDelayMilliSeconds = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_MSG_DELAY_TIME, 100);

	mIntHour = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_HOUR, 0);
	mIntMinute = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_MINUTE, 0);
	mIntSecond = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_SECOND, 0);

	UpdateData(FALSE);

	return 0;
}

int CTradeAssistDlg::SaveSetting(void)
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(STRING_SETTING, STRING_PRICE_DIFF , atoi(mStrPriceDiff));
	theApp.WriteProfileInt(STRING_SETTING, STRING_KEY_COUNT , mIntOrderCount);

	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DX , mIntHighDirect2PriceDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DY , mIntHighDirect2PriceDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DX ,mIntHighPrice2CountDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DY , mIntHighPrice2CountDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DX , mIntLowDirect2PriceDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DY , mIntLowDirect2PriceDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DX , mIntLowPrice2CountDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DY , mIntLowPrice2CountDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DX , mIntCount2ConfirmDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DY , mIntCount2ConfirmDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_START_TAB_DX , mIntStart2TabDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_START_TAB_DY , mIntStart2TabDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DX , mIntLowTab2DirectDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DY , mIntLowTab2DirectDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DX , mIntHighTab2DirectDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DY , mIntHighTab2DirectDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_START_DELETE_ORDER_DX , mIntStart2DeleteOrderDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_START_DELETE_ORDER_DY , mIntStart2DeleteOrderDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, mIsAutoSubmits);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, mAutoCompleteInterval);

	theApp.WriteProfileInt(STRING_SETTING, STRING_DELETE_CONFIRM_DX, mIntDelete2ConfirmDx);
	theApp.WriteProfileInt(STRING_SETTING, STRING_DELETE_CONFIRM_DY, mIntDelete2ConfirmDy);

	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_MSG_DELAY_TIME, mIntMsgDelayMilliSeconds);
	
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_HOUR, mIntHour);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_MINUTE, mIntMinute);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_SECOND, mIntSecond);

	return 0;
}

// ��ȡ�۸������ؼ���λ������
POINT CTradeAssistDlg::GetPrice2CountVector(BOOL isHigh)
{
	POINT point;
	CString buffer;

	if (isHigh)
	{
		point.x = mIntHighPrice2CountDx;
		point.y = mIntHighPrice2CountDy;
	}
	else
	{
		point.x = mIntLowPrice2CountDx;
		point.y = mIntLowPrice2CountDy;
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
		point.x =mIntHighDirect2PriceDx;
		point.y =mIntHighDirect2PriceDy;
	}
	else
	{
		point.x = mIntLowDirect2PriceDx;
		point.y =  mIntLowDirect2PriceDy;
	}

	return point;
}

LRESULT CTradeAssistDlg::OnDeleteOrderMsg(WPARAM w , LPARAM l)
{
	mAction->MouseClick();
	UpdateData(TRUE);
	Sleep(DELETE_ORDER_DELAY);

	POINT lpPoint;
	GetCursorPos(&lpPoint);

	//1.��ǰλ��˫�������µ��Ի���
	int searchCount = 10;
	while (searchCount-- > 0)
	{
		POINT pos =GetSunAwtDialogPos();
		if (pos.x != 0 && pos.y != 0)
		{
			mAction->MoveCursor(pos.x, pos.y, true);
			mAction->MoveCursor(mIntStart2DeleteOrderDx,mIntStart2DeleteOrderDy);
			Sleep(DELETE_ORDER_DELAY);
			mAction->MouseClick();
			mAction->MoveCursor(mIntDelete2ConfirmDx, mIntDelete2ConfirmDy);
			Sleep(DELETE_ORDER_DELAY);
			mAction->MouseClick();
			break;
		}
		Sleep(WINDOW_CHECK_INTERVAL);
	}

	mAction->MoveCursor(lpPoint.x, lpPoint.y, true);

	return LRESULT();
}
// ���sun�Ի������Ͻǵľ������ꡣ
POINT CTradeAssistDlg::GetSunAwtDialogPos(void)
{
	POINT pos;
	pos.x = 0;
	pos.y = 0;
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(SUN_DIALOG_NAME,NULL);
		if (wnd)
		{
			CRect rect;
			::GetWindowRect(wnd,rect);
			pos.x = rect.left;
			pos.y = rect.top;

			break;;
		} 
		Sleep(WINDOW_CHECK_INTERVAL);
	}

#ifdef _DEBUG
	TRACE("GetSunAwtDialogPos count=%d x=%d, y=%d\r\n", searchCount, pos.x, pos.y);
#endif // _DEBUG

	return pos;
}


// ָ��ί�е�����
POINT CTradeAssistDlg::GetTab2Direction(BOOL isHigh)
{
	POINT point;
	CString buffer;
	
	if (isHigh)
	{
		point.x = mIntHighTab2DirectDx;
		point.y = mIntHighTab2DirectDy;
	}
	else
	{
		point.x = mIntLowTab2DirectDx;
		point.y = mIntLowTab2DirectDy;
	}

	return point;
}


int CTradeAssistDlg::ClearResource(void)
{
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_CODE_LOW);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_CODE_HIGH);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_CODE_COUNT);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_FLASH_COMPLETE);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_DECREASE_PRICE);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_INCREASE_PRICE);
	
	KillTimer(TIMER_ID_FOR_DO_FLASH_TRADE);
	SaveSetting();

	CLogger::SaveLog();

	return 0;
}

// ��ɱ�µ�
int CTradeAssistDlg::OnFlashComplete(void)
{

#ifdef _DEBUG
	WORD	start = GetMilliseconds();
#endif // _DEBUG
	POINT lpPoint;
	GetCursorPos(&lpPoint);
	mAction->MouseClick();
	UpdateData(TRUE);
	mIsAutoSubmits = TRUE;	
	UpdateData(FALSE);

	int retryTimes = 0;
	int result = SemicAutoTrade(DO_LOW);
	while (result != SEMIC_AUTO_TRADE_CALL_SUCCESS && retryTimes++ < SEMIC_AUTO_TRADE_RETRY_TIMES)
	{
		SetCursorPos(lpPoint.x, lpPoint.y);
		mAction->MouseClick();
		result = SemicAutoTrade(DO_LOW);
	}

	CString log;
	log.Format("OnFlashComplete low time1=%d, retryTimes=%d", GetMilliseconds() - start, retryTimes);
	CLogger::Add(log);

	//2.��ʱ�µ����

#ifdef _DEBUG
	TRACE("OnFlashComplete time1=%d\r\n", GetMilliseconds() - start);
#endif // _DEBUG

	if (mAutoCompleteInterval > 0)
	{
		Sleep(mAutoCompleteInterval*1000);
	}

	//3.�ƶ���굽˫��λ�á�
	SetCursorPos(lpPoint.x, lpPoint.y);
	mAction->MouseClick();

	retryTimes = 0;
	result = SemicAutoTrade(DO_HIGH);
	while (result != SEMIC_AUTO_TRADE_CALL_SUCCESS && retryTimes++ < SEMIC_AUTO_TRADE_RETRY_TIMES)
	{
		SetCursorPos(lpPoint.x, lpPoint.y);
		mAction->MouseClick();
		result = SemicAutoTrade(DO_HIGH);
	}

	log.Format("OnFlashComplete high time2=%d, retryTimes=%d", GetMilliseconds() - start, retryTimes);
	CLogger::Add(log);

#ifdef _DEBUG
	TRACE("OnFlashComplete time2=%d\r\n", GetMilliseconds() - start);
#endif // _DEBUG

	return 0;
}

LRESULT CTradeAssistDlg::SemicAutoTrade(int direct) 
{

	//1.��ǰλ��˫�������µ��Ի���
	int searchCount = 0;
	while (searchCount++ < FIND_SUN_DIALOG_MAX_RETRY_TIMES)
	{
		HWND wnd=::FindWindow(SUN_DIALOG_NAME,NULL);
		if (wnd)
		{

			CString log;
			log.Format("SemicAutoTrade direct=%d, dialogSearchCount=%d", direct, searchCount);
			CLogger::Add(log);

			if (OnDoTradeMsg(direct, MSG_DELAY_YES) != DO_TRADE_MSG_RESULT_TYPE_SUCCESS)
			{
				return SEMIC_AUTO_TRADE_CALL_FAILED;
			}
			else
			{
				return SEMIC_AUTO_TRADE_CALL_SUCCESS;
			}
		} 
		Sleep(WINDOW_CHECK_INTERVAL);
	}

	return SEMIC_AUTO_TRADE_CALL_FAILED;
}

// ͨ��˫�����ƻ�ñ༭�����ݡ�
CString CTradeAssistDlg::GetEditText(BOOL needDoubleClick)
{
	CString text;

	mAction->SelectAll();
	
#ifdef _DEBUG
	WORD	start = GetMilliseconds();
#endif // _DEBUG
	SetClipboardContent("");
	int copyCount = 0;
	while(copyCount++  < GET_EDIT_CONTENT_MAX_TIMES)
	{
		
		mAction->KeyboardCopy();
		Sleep((copyCount-1)*GET_CLIPBOARD_CONTENT_DELAY);
		text = GetContentFromClipboard();
		text = text.Trim();
		if (text.GetLength() < PRICE_LENGTH)
		{
			continue;			
		}
		else
		{
			break;
		}
	}
#ifdef _DEBUG
	TRACE("GetEditText time=%d, copycount=%d, text=%s\r\n", GetMilliseconds() - start, copyCount, text);
#endif

	return text;
}

BOOL CTradeAssistDlg::CheckEditPasteResult()
{

#ifdef _DEBUG
	WORD	start = GetMilliseconds();
#endif // _DEBUG

	BOOL	result = TRUE;

	int priceCheckCount = 0;
	while(priceCheckCount++ < CHECK_EDIT_PASTE_RESULT_MAX_TIMES)
	{
		CString toCheckPrice = GetEditText();

		if (toCheckPrice.Compare(mLastClipboardContent) == 0)
		{
			//ͨ����飬������ȷ��
			result = TRUE;
			break;
		}
		else
		{
#ifdef _DEBUG
			CString log;
			log.Format("CheckEditPasteResult failed Good=%s, Bad=%s",mLastClipboardContent, toCheckPrice);
			CLogger::Add(log);
			TRACE(log+"\r\n");
#endif

			//δ������ȷ����Ҫ��������
			SetClipboardContent(mLastClipboardContent);
			mAction->KeyboardPaste();
			result = FALSE;
		}
	}

#ifdef _DEBUG
	TRACE("CheckEditPasteResult time=%d, checkCount=%d\r\n", GetMilliseconds() - start, priceCheckCount);
#endif

	return result;
}

WORD CTradeAssistDlg::GetMilliseconds(void)
{

	SYSTEMTIME time;
	GetSystemTime(&time);
	return time.wSecond * 1000 + time.wMilliseconds;
}

// ��ռ�����
BOOL CTradeAssistDlg::GlearClipboard(void)
{
	if( OpenClipboard() ) 
	{  
		if (EmptyClipboard())
		{
			CloseClipboard();
			TRACE("GlearClipboard return 1\r\n");
			return 1;
		}
		else
		{
			CloseClipboard();
		}
	}

	TRACE("GlearClipboard return 0\r\n");
	return 0;
}

int CTradeAssistDlg::UpdatePrice(bool isAdd)
{
	Sleep(mIntMsgDelayMilliSeconds);
	//ȡ�õ�ǰ�۸�
	CString text = GetEditText();
	if(text.Find(_T("."))  == -1)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE;
	}

	CString outText;
	float diff = 10.0f;

	float newCount = isAdd?atof(text) + diff : atof(text) - diff ;
	outText.Format("%.2f",newCount);

	//���ü��������ݲ�ճ��������
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	return 0;
}

void CTradeAssistDlg::OnTimer(UINT_PTR nIDEvent)
{

	if (nIDEvent == TIMER_ID_FOR_DO_FLASH_TRADE)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);

		if (mIntHour == time.wHour && mIntMinute == time.wMinute && mIntSecond == time.wSecond)
		{

			KillTimer(nIDEvent);	
			GetDlgItem(IDC_BUTTON_START_TIMER)->EnableWindow(TRUE);
			OnFlashComplete();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CTradeAssistDlg::OnBnClickedButtonStartTimer()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	UpdateData(TRUE);
	if (mIntHour * 3600 + mIntMinute * 60 + mIntSecond > time.wHour  * 3600 +  time.wMinute * 60 + time.wSecond)
	{
		GetDlgItem(IDC_BUTTON_START_TIMER)->EnableWindow(FALSE);
		SetTimer(TIMER_ID_FOR_DO_FLASH_TRADE, 50, NULL);
	}
	else
	{

		CString text;
		text.LoadString(IDS_INVALID_TIMER);
		MessageBox(text);
	}
}
