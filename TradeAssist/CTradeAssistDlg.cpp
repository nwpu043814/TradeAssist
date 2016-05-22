// TradeAssistDlg.cpp : 实现文件
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
#include "HttpThread.h"
#include <process.h>
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTradeAssistDlg 对话框
HANDLE hStartEvent;
CCriticalSection critical_section;

UINT	__stdcall HttpProcess(void * param)
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	if(!SetEvent(hStartEvent)) //set thread start event 
	{
		TRACE("set start event failed,errno:%d/n",::GetLastError());
		return 1;
	}

	CHttpWorkerP mHttpWorker = new CHttpWorker();

	while(true)
	{
		if(GetMessage(&msg,0,0,0)) //get msg from message queue
		{
			switch(msg.message)
			{
				case WM_DO_HTTP_GET:
				{
					TRACE("recv %d\r\n",msg.wParam);
					CString result = mHttpWorker->DoGet(TEXT("jry.baidao.com"), 80, TEXT("/api/hq/npdata.do?ids=1&markettype=ttj"));
					CDataPacketP packet = new CDataPacket();
					CUtil::ParseDataString(result,*packet);
					TRACE("result=%s\r\n", result);
					Sleep(TIMER_INTERVAL_UPDATE_DATAK);
			
					critical_section.Lock();
					if (! ((CTradeAssistDlg*)(param))->OnDoHttpGetFinish((WPARAM)packet,NULL))
					{
						delete packet;

						PostThreadMessage((DWORD)(msg.lParam),WM_DO_HTTP_GET, msg.wParam, msg.lParam);
					}
					critical_section.Unlock();

					break;
				}
				case WM_DO_HTTP_EXIT:
				{
					return 0;
				}
			}
		}
	}

	return 0;
}

CTradeAssistDlg::CTradeAssistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeAssistDlg::IDD, pParent)
	, mIsAutoSubmits(FALSE)
	, mAutoCompleteInterval(0)
	, mLastClipboardContent(_T(""))
	, mStrHighPriceDiff(_T(""))
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
	, mEnableChckAutoCloseDepot(FALSE)
	, mStrLowPriceDiff(_T(""))
	, mDataKClose(0)
	, mDataKOpen(0)
	, mDataKCloseTime(_T(""))
	, mDataKOpenTime(_T(""))
	, mDataKHighPrice(0)
	, mDataKLowPrice(0)
	, mThreadHandle(0)
	, mThreadID(0)
	, mUintAutoCloseThreshold(0)
	, mDataKStatisticsUpdrop(0)
	, mDataKDayUpdrop(0)
	, mDataKCurrent2ExtremeDiff(0)
	, mDataKDirectionAgree(FALSE)
	, mOpenDirection(-1)
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
	DDX_Text(pDX, IDC_EDIT_HIGH_PRICE_DIFF, mStrHighPriceDiff);
	DDV_MaxChars(pDX, mStrHighPriceDiff, 4);
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
	DDV_MinMaxUInt(pDX, mIntHour, 0, 23);
	DDX_Text(pDX, IDC_EDIT_MINUTE, mIntMinute);
	DDV_MinMaxUInt(pDX, mIntMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SECOND, mIntSecond);
	DDV_MinMaxUInt(pDX, mIntSecond, 0, 59);
	DDX_Check(pDX, IDC_CHECK_AUTO_CLOSE_DEPOT, mEnableChckAutoCloseDepot);
	DDX_Text(pDX, IDC_EDIT_LOW_PRICE_DIFF, mStrLowPriceDiff);
	DDV_MaxChars(pDX, mStrLowPriceDiff, 4);
	DDX_Text(pDX, IDC_EDIT_DATAK_CURRENT_PRICE, mDataKClose);
	DDV_MinMaxUInt(pDX, mDataKClose, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_DATAK_OPEN_PRICE, mDataKOpen);
	DDV_MinMaxUInt(pDX, mDataKOpen, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_CURRENT_TIME, mDataKCloseTime);
	DDV_MaxChars(pDX, mDataKCloseTime, 10);
	DDX_Text(pDX, IDC_EDIT_DATAK_OPEN_TIME, mDataKOpenTime);
	DDV_MaxChars(pDX, mDataKOpenTime, 10);
	DDX_Text(pDX, IDC_EDIT_HIGH_PRICE, mDataKHighPrice);
	DDV_MinMaxUInt(pDX, mDataKHighPrice, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_DATAK_LOW_PRICE, mDataKLowPrice);
	DDV_MinMaxUInt(pDX, mDataKLowPrice, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_AUTO_CLOSE_THRESHOLD, mUintAutoCloseThreshold);
	DDV_MinMaxUInt(pDX, mUintAutoCloseThreshold, 10, 30);
	DDX_Text(pDX, IDC_EDIT_DATAK_STATISTICS_UPDROP, mDataKStatisticsUpdrop);
	DDX_Text(pDX, IDC_EDIT_DATAK_DAY_UPDROP, mDataKDayUpdrop);
	DDX_Text(pDX, IDC_EDIT_CURRENT_EXTREME_DIFF, mDataKCurrent2ExtremeDiff);
	DDV_MinMaxInt(pDX, mDataKCurrent2ExtremeDiff, -100, 100);
	DDX_Check(pDX, IDC_CHECK_STATISTICS_DAY_UPDROP_AGREE, mDataKDirectionAgree);
	DDX_Control(pDX, IDC_PROGRESS_AUTO_CLOSE_DEPOT, mProgressAutoCloseDepot);
	DDX_Radio(pDX, IDC_RADI_HIGH, mOpenDirection);
}

BEGIN_MESSAGE_MAP(CTradeAssistDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_HOTKEY,OnHotKey) //添加此句
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DISPLAY_DATAK, OnDisplayDataK)
	ON_MESSAGE(WM_DO_TRADE, OnDoTradeMsg) 
	ON_MESSAGE(WM_HTTP_GET_FINISH,OnDoHttpGetFinish)
	ON_MESSAGE(WM_DO_COUNT, OnDeleteOrderMsg)
	ON_BN_CLICKED(IDCANCEL, &CTradeAssistDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CTradeAssistDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_TIMER, &CTradeAssistDlg::OnBnClickedButtonStartTimer)
END_MESSAGE_MAP()


// CTradeAssistDlg 消息处理程序

BOOL CTradeAssistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InstallHotKey();
	InitialSetting();
	mProgressAutoCloseDepot.SetRange(0, 20);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTradeAssistDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
			mProgressAutoCloseDepot.SetPos(20);
			break;
		}
	}

	return 1;
}


/**
*	解析热键类型。
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

	//点击委托tab。
	POINT start = GetSunAwtDialogPos();
	if (start.x == 0 && start.y == 0)
	{
		CLogger::Add("not find sunawtdialog");
		//未能找到sun对话框
		return DO_TRADE_MSG_RESULT_TYPE_NOT_FIND_DIALOG;
	}
	mAction->MoveCursor(start.x,start.y, true);

	//从原点移动到指价委托tab。
	mAction->MoveCursor(mIntStart2TabDx,mIntStart2TabDy);
	mAction->MouseClick();


	//指价委托到方向
	POINT tab2Direction = GetTab2Direction(direction);
	mAction->MoveCursor(tab2Direction.x,tab2Direction.y);
	mAction->MouseClick();

	//从方向移到价格控件
	POINT direction2PriceVector = GetDirection2PriceVector(direction);
	mAction->MoveCursor(direction2PriceVector.x,direction2PriceVector.y);

	//获得预制的点差
	CString outText;
	float highDiff = atof(mStrHighPriceDiff);
	float lowDiff = atof(mStrLowPriceDiff);

	//取得当前价格。
	CString text = GetEditText();
	if(text.Find(_T("."))  == -1)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE;
	}

	float newCount = direction?atof(text) + highDiff : atof(text) - lowDiff ;
	outText.Format("%.2f",newCount);

	CString log;
	log.Format("originalprice=%s, hiDiff=%f, lowDiff=%f newPrice=%s, direction=%d", text, highDiff, lowDiff ,outText, direction);
	CLogger::Add(log);

	//保存以备检查
	mLastClipboardContent = outText;

	//设置剪贴板内容并粘贴到窗口
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	if(!CheckEditPasteResult())
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_PASSED;
	}

	//移动到设置手数的控件
	POINT price2CountVector = GetPrice2CountVector(direction);
	mAction->MoveCursor(price2CountVector.x,price2CountVector.y);

	//更新交易手数
	mAction->MouseDoubleClick();
	outText.Format("%d", mIntOrderCount);
	SetClipboardContent(outText);
	mAction->KeyboardPaste();

	//移动到确定按钮上
	mAction->MoveCursor(mIntCount2ConfirmDx,mIntCount2ConfirmDy);

	//自动提交
	if(mIsAutoSubmits)
	{
		mAction->MouseClick();
	}

#ifdef _DEBUG
	TRACE("OnDoTradeMsg submit time=%d\r\n", GetMilliseconds() - startTime);
#endif

	return DO_TRADE_MSG_RESULT_TYPE_SUCCESS;
}

// 获得剪贴板的内容
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

// 设置剪贴板的内容
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
	
	if (mThreadID != 0)
	{
		PostThreadMessage(mThreadID,WM_DO_HTTP_EXIT,(WPARAM) (AfxGetMainWnd()),(LPARAM)(mThreadID));
	}
}

int CTradeAssistDlg::InitialSetting(void)
{
	CString outText;
	mStrHighPriceDiff.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_DIFF, 10));
	mStrLowPriceDiff.Format("%d", theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_DIFF, 10));
	mIntOrderCount = theApp.GetProfileInt(STRING_SETTING, STRING_KEY_COUNT, 6);

	//做多方向到价格
	mIntHighDirect2PriceDx = theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DX, 0);
	mIntHighDirect2PriceDy = theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_DIRECTION_PRICE_DY, 0);
	
	//做多价格到交易笔数
	mIntHighPrice2CountDx = theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DX, 0);
	mIntHighPrice2CountDy = theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_COUNT_DY, 0);

	//做空方向到价格
	mIntLowDirect2PriceDx = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DX, 0);
	mIntLowDirect2PriceDy = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_DIRECTION_PRICE_DY, 0);

	//做空价格到交易笔数
	mIntLowPrice2CountDx = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DX, 0);
	mIntLowPrice2CountDy = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_COUNT_DY, 0);	

	//交易笔数到确定按钮
	mIntCount2ConfirmDx = theApp.GetProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DX, 0);
	mIntCount2ConfirmDy = theApp.GetProfileInt(STRING_SETTING, STRING_COUNT_CONFIRM_DY, 0);
	
	//原点到指价委托
	mIntStart2TabDx = theApp.GetProfileInt(STRING_SETTING, STRING_START_TAB_DX, 0);
	mIntStart2TabDy = theApp.GetProfileInt(STRING_SETTING, STRING_START_TAB_DY, 0);

	//做空指价委托到方向
	mIntLowTab2DirectDx = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DX, 0);
	mIntLowTab2DirectDy = theApp.GetProfileInt(STRING_SETTING, STRING_LOW_TAB_DIRECT_DY, 0);

	//做多指价委托到方向
	mIntHighTab2DirectDx =theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DX, 0);
	mIntHighTab2DirectDy =theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_TAB_DIRECT_DY, 0);

	//删除订单原定到删除
	mIntStart2DeleteOrderDx =theApp.GetProfileInt(STRING_SETTING, STRING_START_DELETE_ORDER_DX, 0);
	mIntStart2DeleteOrderDy =theApp.GetProfileInt(STRING_SETTING, STRING_START_DELETE_ORDER_DY, 0);

	//删除到确定
	mIntDelete2ConfirmDx =theApp.GetProfileInt(STRING_SETTING, STRING_DELETE_CONFIRM_DX, 0);
	mIntDelete2ConfirmDy =theApp.GetProfileInt(STRING_SETTING, STRING_DELETE_CONFIRM_DY, 0);

	mIsAutoSubmits = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, FALSE);
	mAutoCompleteInterval = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, 2);

	mIntMsgDelayMilliSeconds = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_MSG_DELAY_TIME, 100);

	mIntHour = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_HOUR, 0);
	mIntMinute = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_MINUTE, 0);
	mIntSecond = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_SECOND, 0);
	mEnableChckAutoCloseDepot = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_BOX_ENABLE_CHECK_AUTO_CLOSE_DEPOT, 0);
	mUintAutoCloseThreshold = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_AUTO_CLOSE_THRESHOLD, 20);
	UpdateData(FALSE);

	return 0;
}

int CTradeAssistDlg::SaveSetting(void)
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_PRICE_DIFF , atoi(mStrHighPriceDiff));
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_PRICE_DIFF , atoi(mStrLowPriceDiff));
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
	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_BOX_ENABLE_CHECK_AUTO_CLOSE_DEPOT, mEnableChckAutoCloseDepot);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_AUTO_CLOSE_THRESHOLD, mUintAutoCloseThreshold);

	return 0;
}

// 获取价格到手数控件的位移设置
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

// 方向到价格的位移
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

	//1.当前位置双击弹出下单对话框。
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
// 获得sun对话框右上角的绝对坐标。
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


// 指价委托到方向
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
	KillTimer(TIMER_ID_FOR_UPDATE_DATAK);
	SaveSetting();

	if (mThreadHandle != 0)
	{
		CloseHandle(mThreadHandle);
		mThreadHandle = 0;
	}

	CLogger::SaveLog();

	return 0;
}

// 秒杀下单
int CTradeAssistDlg::OnFlashComplete(void)
{


	WORD	start = GetMilliseconds();

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

	//2.延时下单间隔

#ifdef _DEBUG
	TRACE("OnFlashComplete time1=%d\r\n", GetMilliseconds() - start);
#endif // _DEBUG

	if (mAutoCompleteInterval > 0)
	{
		Sleep(mAutoCompleteInterval*1000);
	}

	//3.移动鼠标到双击位置。
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

	//1.当前位置双击弹出下单对话框。
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

// 通过双击复制获得编辑框内容。
CString CTradeAssistDlg::GetEditText(BOOL needDoubleClick)
{
	CString text;
	
#ifdef _DEBUG
	WORD	start = GetMilliseconds();
#endif // _DEBUG
	SetClipboardContent("");
	int copyCount = 0;
	while(copyCount++  < GET_EDIT_CONTENT_MAX_TIMES)
	{
		mAction->SelectAll();
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
			//通过检查，设置正确。
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

			//未设置正确，需要重新设置
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

// 清空剪贴板
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
	//取得当前价格。
	CString text = GetEditText();
	if(text.Find(_T("."))  == -1)
	{
		return DO_TRADE_MSG_RESULT_TYPE_NOT_GOT_ORIGINAL_PRICE;
	}

	CString outText;
	float diff = 10.0f;

	float newCount = isAdd?atof(text) + diff : atof(text) - diff ;
	outText.Format("%.2f",newCount);

	//设置剪贴板内容并粘贴到窗口
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

			if(mEnableChckAutoCloseDepot)
			{
				mProgressAutoCloseDepot.SetRange(0, mUintAutoCloseThreshold);

				//开启自动平仓检测。
				hStartEvent = 0;
				hStartEvent = ::CreateEvent(0,FALSE,FALSE,0); //create thread start event
				if(hStartEvent == 0)
				{
					TRACE("create start event failed,errno:%d/n",::GetLastError());
					return ;
				}

				if (mThreadHandle == 0)
				{
					mThreadHandle = (HANDLE)_beginthreadex( NULL, 0, &HttpProcess, this, 0, &mThreadID );
				} 
						
				if(mThreadHandle == 0)
				{
					TRACE("start thread failed,errno:%d/n",::GetLastError());
					CloseHandle(hStartEvent);
					return ;
				}

				//wait thread start event to avoid PostThreadMessage return errno:1444
				::WaitForSingleObject(hStartEvent,INFINITE);
				CloseHandle(hStartEvent);

				int count = 0;
				while(count++ < 3)
				{

					if(!PostThreadMessage(mThreadID,WM_DO_HTTP_GET,(WPARAM) (AfxGetMainWnd()),(LPARAM)(mThreadID)))//post thread msg
					{
						TRACE("post message failed,errno:%d/n",::GetLastError());
					}
					else
					{
						break;
					}
					::Sleep(500);
				}
			}
		}
	}
	else if (TIMER_ID_FOR_UPDATE_DATAK == nIDEvent)
	{
		PostMessage(WM_DISPLAY_DATAK);
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
		KillTimer(TIMER_ID_FOR_UPDATE_DATAK);
		GetDlgItem(IDC_BUTTON_START_TIMER)->EnableWindow(FALSE);
		SetTimer(TIMER_ID_FOR_DO_FLASH_TRADE, TIMER_INTERVAL_DO_FLASH_TRADE, NULL); 
		SetTimer(TIMER_ID_FOR_UPDATE_DATAK, TIMER_INTERVAL_UPDATE_DATAK, NULL);
	}
	else
	{

		CString text;
		text.LoadString(IDS_INVALID_TIMER);
		MessageBox(text);
	}
}

LRESULT CTradeAssistDlg::OnDoHttpGetFinish(WPARAM w , LPARAM l)
{
	TRACE("OnDoHttpGetFinish\r\n");

	if (w != NULL)
	{
		CDataPacketP packet = (CDataPacketP)w;

		if (packet->mIsGood)
		{

			//更新价格
			mDataK->SetClose(packet->mPrice, packet->mPriceTime);
			mDataK->SetDayUpDrop(packet->mUpDrop);
			mDataKStatisticsUpdrop = mDataK->IsPositive();

			//这里判读是否回调
			if (mDataK->IsPositive() && mDataK->GetDayUpDrop() > 0 || mOpenDirection == 0)
			{
				mDataK->SetDirectionAgree(true);
				mDataK->SetCurrent2ExtremeDiff(mDataK->GetHigh() - mDataK->GetClose()) ;
				//做多分支
				if(mDataK->GetCurrent2ExtremeDiff() >= mUintAutoCloseThreshold)
				{
					mAction->MouseClick();
					MessageBox("做多回调立即平仓");

					if(!PostThreadMessage(mThreadID,WM_DO_HTTP_EXIT,(WPARAM) (AfxGetMainWnd()),(LPARAM)(mThreadID)))//post thread msg
					{
						TRACE("post message failed,errno:%d/n",::GetLastError());
					}

					return 0;
				}
				
			}
			else if(mDataK->IsNegtive() && mDataK->GetDayUpDrop() < 0 || mOpenDirection == 1)
			{
				mDataK->SetDirectionAgree(true);
				mDataK->SetCurrent2ExtremeDiff( mDataK->GetClose() - mDataK->GetLow() );
				//做空分支
				if(mDataK->GetCurrent2ExtremeDiff() >= mUintAutoCloseThreshold)
				{
					mAction->MouseClick();
					if(!PostThreadMessage(mThreadID,WM_DO_HTTP_EXIT,(WPARAM) (AfxGetMainWnd()),(LPARAM)(mThreadID)))//post thread msg
					{
						TRACE("post message failed,errno:%d/n",::GetLastError());
					}
					MessageBox("做空反弹立即平仓");
					return 0;
				}
			}
			else
			{
				mDataK->SetDirectionAgree(false);
			}
		}

		delete packet;
	}
	else
	{
		
	}

	if(!PostThreadMessage(mThreadID,WM_DO_HTTP_GET,(WPARAM) (AfxGetMainWnd()),(LPARAM)(mThreadID)))//post thread msg
	{
		TRACE("post message failed,errno:%d/n",::GetLastError());
	}

	return 1;
}

LRESULT CTradeAssistDlg::OnDisplayDataK(WPARAM w, LPARAM l)
{
	UpdateData(TRUE);
	const int TIME_OFFSET = 8;
	mDataKClose = mDataK->GetClose();
	mDataKOpen = mDataK->GetOpen();
	mDataKCloseTime = mDataK->GetCloseTime().Right(TIME_OFFSET);
	mDataKOpenTime = mDataK->GetOpenTime().Right(TIME_OFFSET);
	mDataKHighPrice = mDataK->GetHigh();
	mDataKLowPrice = mDataK->GetLow();
	mDataKDayUpdrop = mDataK->GetDayUpDrop();
	mDataKCurrent2ExtremeDiff = mDataK->GetCurrent2ExtremeDiff();
	mDataKStatisticsUpdrop = mDataK->GetAmplitude();
	mDataKDirectionAgree = mDataK->IsDirectionAgree();
	if (mDataKDirectionAgree)
	{
		mProgressAutoCloseDepot.SetPos(mDataKCurrent2ExtremeDiff);
	}
	UpdateData(FALSE);

	return 0;
}

