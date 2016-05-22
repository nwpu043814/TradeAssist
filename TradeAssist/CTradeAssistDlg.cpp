// TradeAssistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TradeAssist.h"
#include "TradeAssistDlg.h"
#include "Constant.h"
#include "SimulateAction.h"
#include "Logger.h"
#include "HttpWorker.h"
#include "Util.h"
#include "DataPacket.h"
#include "HttpThread.h"
#include "aboutDlg.h"
#include <process.h>
#include "afxmt.h"
#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>
#include<crtdbg.h>
#include <mmsystem.h>
#include "dalyFx.h"
#include "RegistDlg.h"
#include "HuifengGuadanParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib,_T("lua5.1.lib"))
#pragma comment(lib,_T("winmm"))

#define CLOSE_THREAD(data, number ) for (int i = 0 ; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)\
{\
	CloseHttpThread(data[i]);\
}

extern CRITICAL_SECTION g_cs;
UINT	__stdcall HttpProcess(void * param);

CTradeAssistDlg::CTradeAssistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTradeAssistDlg::IDD, pParent)
	, mIsAutoSubmits(FALSE)
	, mAutoCompleteInterval(_T(""))
	, mStrHighPriceDiff(_T(""))
	, mIntOrderCount(1)
	, mIntMsgDelayMilliSeconds(1000)
	, mIntHour(0)
	, mIntMinute(0)
	, mIntSecond(_T(""))
	, mEnableCheckAutoCloseDepot(FALSE)
	, mStrLowPriceDiff(_T(""))
	, mDataKClose(0.0F)
	, mDataKOpen(0.0F)
	, mDataKCloseTime(_T(""))
	, mDataKOpenTime(_T(""))
	, mDataKHighPrice(0.0F)
	, mDataKLowPrice(0.0F)
	, mUintAutoCloseThreshold(0)
	, mDataKStatisticsUpdrop(0)
	, mDataKDayUpdrop(0)
	, mDataKCurrent2ExtremeDiff(0)
	, mOpenDirection(-1)
	, mServerIp(_T("127.0.0.1"))
	, mServerPort(80)
	, mUintDoHttpInterval(0)
	, mBoolEnableAutoThreshold(FALSE)
	, mIsTimer4Tomorrow(false)
	, mActualNonfarmerNumber(_T(""))
	, mNonfarmerNumberResult(_T(""))
	, mJoblessRateResult(_T(""))
	, mNonfarmerNumnerWeight(_T(""))
	, mJoblessRateWeight(_T(""))
	, mNonfarmerNumberCount(_T(""))
	, mJoblessRateCount(_T(""))
	, mEnableChaseTimer(FALSE)
	, mTotalConclution(_T(""))
	, mPullPriceCount(0)
	, mQueryPriceUseTime(0.0F)
	, mCapturePriceUseTime(0)
	, mUILastUpdateTime(0)
	, mEnableWindowMostTop(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mHttpWorker = new CHttpWorker();
	mDataK = new CDataK();
	mActionManager =(CActionManagerP) AfxBeginThread(RUNTIME_CLASS(CActionManager));
	mActionManager->SetWindowOwner(GetSafeHwnd());

	mNonfarmerNumber = new PEcnomicData[THREAD_NUMBER];
	mNonfarmerNumber[0] = new CNonfarmerNumberData();
	mNonfarmerNumber[0]->SetUrl( mLuaEngine.GetNonfarmerWorkerUrl(0));
	mNonfarmerNumber[0]->SetResult( EcnomicResult::TYPE_UNKOWN);
	mNonfarmerNumber[0]->SetExpectValue( mLuaEngine.GetExpectValue(LUA_FUNCTION_GetExpectNonfarmerWorker));
	mNonfarmerNumber[0]->SetMsgType(WM_DO_HTTP_GET_ECNOMIC_DATA);

	for(int i = 1; i < THREAD_NUMBER; i++)
	{
		if (i < THREAD_NUMBER/2)
		{
			mNonfarmerNumber[i] = new CNonfarmerNumberData(*mNonfarmerNumber[0]);
		}
		else
		{
			PDailyFx dailyFx = new CDailyFx(*mNonfarmerNumber[0]);
			dailyFx->SetIspositive(true);
			dailyFx->SetUrl(mLuaEngine.GetDailyFxUrl());
			dailyFx->SetTitle(mLuaEngine.GetNonfarmerTitle());
			mNonfarmerNumber[i] = dailyFx;
		}
	}

	mJoblessRate	= new PEcnomicData[THREAD_NUMBER];
	mJoblessRate[0] = new CJoblessRateData();
	mJoblessRate[0]->SetUrl(mLuaEngine.GetJoblessUrl(0));
	mJoblessRate[0]->SetResult(EcnomicResult::TYPE_UNKOWN);
	mJoblessRate[0]->SetExpectValue(mLuaEngine.GetExpectValue(LUA_FUNCTION_GetExpectJoblessRate));
	mJoblessRate[0]->SetMsgType(WM_DO_HTTP_GET_ECNOMIC_DATA);

	for(int i = 1; i < THREAD_NUMBER; i++)
	{
		if (i < THREAD_NUMBER/2)
		{
			mJoblessRate[i] = new CJoblessRateData(*mJoblessRate[0]);
		}
		else
		{
			PDailyFx dailyFx = new CDailyFx(*mNonfarmerNumber[0]);
			dailyFx->SetIspositive(false);
			dailyFx->SetUrl(mLuaEngine.GetDailyFxUrl());
			dailyFx->SetTitle(mLuaEngine.GetJoblessTitle());
			mJoblessRate[i] = dailyFx;
		}
	}

	mLocalPrice = new PEcnomicData[LOCAL_SERVER_REQUEST_THREADED_NUMBER];
	for (int i =0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
	{
		mLocalPrice[i] = new CLocalServerData();
		mLocalPrice[i]->SetUrl(HTTP_URL_LOCAL);
		mLocalPrice[i]->SetResult(EcnomicResult::TYPE_UNKOWN);
		mLocalPrice[i]->SetExpectValue(_T(""));
		mLocalPrice[i]->SetMsgType(WM_DO_HTTP_GET_PRICE);
	}

	InitializeCriticalSection(&g_cs);
}


void CTradeAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AOTO_SUBMIT, mIsAutoSubmits);
	DDX_Text(pDX, IDC_EDIT_AUTO_COMPLETE_INTERVAL, mAutoCompleteInterval);
	DDV_MaxChars(pDX, mAutoCompleteInterval, 3);
	DDX_Text(pDX, IDC_EDIT_HIGH_PRICE_DIFF, mStrHighPriceDiff);
	DDV_MaxChars(pDX, mStrHighPriceDiff, 4);
	DDX_Text(pDX, IDC_EDIT_COUNT, mIntOrderCount);
	DDV_MinMaxUInt(pDX, mIntOrderCount, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MSG_DELAY, mIntMsgDelayMilliSeconds);
	DDV_MinMaxUInt(pDX, mIntMsgDelayMilliSeconds, 0, 80000);
	DDX_Text(pDX, IDC_EDIT_HOUR, mIntHour);
	DDX_Text(pDX, IDC_EDIT_MINUTE, mIntMinute);
	DDX_Text(pDX, IDC_EDIT_SECOND, mIntSecond);
	DDX_Check(pDX, IDC_CHECK_AUTO_CLOSE_DEPOT, mEnableCheckAutoCloseDepot);
	DDX_Text(pDX, IDC_EDIT_LOW_PRICE_DIFF, mStrLowPriceDiff);
	DDV_MaxChars(pDX, mStrLowPriceDiff, 4);
	DDX_Text(pDX, IDC_EDIT_DATAK_CURRENT_PRICE, mDataKClose);
	DDX_Text(pDX, IDC_EDIT_DATAK_OPEN_PRICE, mDataKOpen);
	DDX_Text(pDX, IDC_EDIT_CURRENT_TIME, mDataKCloseTime);
	DDV_MaxChars(pDX, mDataKCloseTime, 10);
	DDX_Text(pDX, IDC_EDIT_DATAK_OPEN_TIME, mDataKOpenTime);
	DDV_MaxChars(pDX, mDataKOpenTime, 10);
	DDX_Text(pDX, IDC_EDIT_HIGH_PRICE, mDataKHighPrice);
	DDX_Text(pDX, IDC_EDIT_DATAK_LOW_PRICE, mDataKLowPrice);
	DDX_Text(pDX, IDC_EDIT_AUTO_CLOSE_THRESHOLD, mUintAutoCloseThreshold);
	DDV_MinMaxUInt(pDX, mUintAutoCloseThreshold, THRESHOLD_MIN, THRESHOLD_MAX);
	DDX_Text(pDX, IDC_EDIT_DATAK_STATISTICS_UPDROP, mDataKStatisticsUpdrop);
	DDX_Text(pDX, IDC_EDIT_DATAK_DAY_UPDROP, mDataKDayUpdrop);
	DDX_Text(pDX, IDC_EDIT_CURRENT_EXTREME_DIFF, mDataKCurrent2ExtremeDiff);
	DDV_MinMaxInt(pDX, mDataKCurrent2ExtremeDiff, -100, 100);
	DDX_Control(pDX, IDC_PROGRESS_AUTO_CLOSE_DEPOT, mProgressAutoCloseDepot);
	DDX_Radio(pDX, IDC_RADI_HIGH, mOpenDirection);
	DDX_Text(pDX, IDC_EDIT_SERVER_HOST, mServerIp);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, mServerPort);
	DDX_Text(pDX, IDC_EDIT_DO_HTTP_REQUEST_INTERVAL, mUintDoHttpInterval);
	DDV_MinMaxUInt(pDX, mUintDoHttpInterval, 0, 1000);
	DDX_Check(pDX, IDC_CHECK_ENABLE_AUTO_THRESHOLD, mBoolEnableAutoThreshold);
	DDX_Text(pDX, IDC_EDIT_NONFARMER_NUMBER, mActualNonfarmerNumber);
	DDX_Text(pDX, IDC_EDIT_JOBLESS_RATE, mActualJoblessRate);
	DDX_Text(pDX, IDC_EDIT_NONFARMER_NUMBER_RESULT, mNonfarmerNumberResult);
	DDX_Text(pDX, IDC_EDIT_JOBLESS_RATE_RESULT, mJoblessRateResult);
	DDX_Text(pDX, IDC_EDIT_NONFARMER_NUMBER_WEIGHT, mNonfarmerNumnerWeight);
	DDX_Text(pDX, IDC_EDIT_JOBLESS_RATE_WEIGHT, mJoblessRateWeight);
	DDX_Text(pDX, IDC_EDIT_NONFARMER_NUMBER_WEIGHT2, mNonfarmerNumberCount);
	DDX_Text(pDX, IDC_EDIT_JOBLESS_RATE_WEIGHT2, mJoblessRateCount);
	DDX_Check(pDX, IDC_CHECK_AOTO_CHASE, mEnableChaseTimer);
	DDX_Text(pDX, IDC_EDIT_TOTAL_CONCLUTION, mTotalConclution);
	DDX_Text(pDX, IDC_EDIT_QURERY_PRICE_USE_TIME, mQueryPriceUseTime);
	DDX_Text(pDX, IDC_EDIT_CAPTURE_PRICE_USE_TIME, mCapturePriceUseTime);
}

BEGIN_MESSAGE_MAP(CTradeAssistDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_HOTKEY, &CTradeAssistDlg::OnHotKey) //添加此句
	ON_MESSAGE(WM_DISPLAY_DATAK,  &CTradeAssistDlg::OnDisplayDataK)
	ON_MESSAGE(WM_DO_TRADE,  &CTradeAssistDlg::OnDoTradeMsg) 
	ON_MESSAGE(WM_HTTP_GET_FINISH,&CTradeAssistDlg::OnHttpGetPriceFinish)
	ON_MESSAGE(WM_ALT_D,&CTradeAssistDlg::OnAltDMsg)
	ON_MESSAGE(WM_HTTP_GET_ECNOMIC_DATA_FINISH,&CTradeAssistDlg::OnHttpGetEcnomicData) 
	ON_MESSAGE(WM_DO_CANCEL_ORDER,  &CTradeAssistDlg::OnDeleteOrderMsg)
	ON_BN_CLICKED(IDCANCEL, &CTradeAssistDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CTradeAssistDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_TIMER, &CTradeAssistDlg::OnBnClickedButtonStartTimer)
	ON_BN_CLICKED(IDC_CHECK_SET_MOST_TOP, &CTradeAssistDlg::OnBnClickedCheckSetMostTop)
END_MESSAGE_MAP()


// CTradeAssistDlg 消息处理程序

BOOL CTradeAssistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RegistDlg dlg;

	if (dlg.Explorer(dlg.GetRegCode()))
	{

		dlg.DoModal();

		if (RegistDlg::mIntResult > 0)
		{
			ExitProcess(RegistDlg::mIntResult);
		} 
	}

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
	_CrtDumpMemoryLeaks();
	// TODO: 在此添加额外的初始化代码
	InstallHotKey();
	InitialSetting();
	EnableWindowMostTop(mEnableWindowMostTop);
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
	::RegisterHotKey(m_hWnd, HOT_KEY_TEST_SERVER, MOD_SHIFT, VK_T);
	::RegisterHotKey(m_hWnd, HOT_KEY_DIRECT_DUO, MOD_SHIFT, VK_D);
	::RegisterHotKey(m_hWnd, HOT_KEY_ALT_D, MOD_ALT, VK_D);
	::RegisterHotKey(m_hWnd, HOT_KEY_DIRECT_KONG, MOD_SHIFT, VK_K);
	::RegisterHotKey(m_hWnd, HOT_KEY_INCREASE_THRESHOLD, MOD_SHIFT, VK_S);
	::RegisterHotKey(m_hWnd, HOT_KEY_DECREASE_THRESHOLD, MOD_SHIFT, VK_A);

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
			mActionManager->UpdatePrice(false, 10);
			break;
		}
		case HOT_KEY_INCREASE_PRICE:
		{
			UpdateData();
			//mActionManager->UpdatePrice(true, 10);
			mActionManager->PostThreadMessage(WM_DO_LUOGE_GUADAN, (WPARAM) GetGuaDanParam(DO_BOTH), NULL);
			break;
		}
		case HOT_KEY_TEST_SERVER:
		{
			UpdateData();
			for (int i = 0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
			{
				StartHttpThread(mLocalPrice[i]);
			}
			break;
		}
		case HOT_KEY_ALT_D:
		{
			
			this->PostMessage(WM_ALT_D);
			break;
		}
		case HOT_KEY_DIRECT_DUO:
		{

			UpdateData(TRUE);

			mActionManager->PostThreadMessage(WM_DO_KUNJIAO_GUADAN, (WPARAM)GetGuaDanParam(DO_BOTH), NULL);
			
			//mBoolEnableAutoThreshold = !mBoolEnableAutoThreshold;
			//UpdateData(FALSE);
			break;
		}
		case HOT_KEY_DIRECT_KONG:
		{
			UpdateData(TRUE);
			for (int i =0 ; i < THREAD_NUMBER; i++)
			{
				if (mNonfarmerNumber[i]->IsEnable() && mNonfarmerNumber[i]->GetThreadId() == 0)
				{
					StartHttpThread(mNonfarmerNumber[i]);
				}
			}

			for (int i =0 ; i < THREAD_NUMBER; i++)
			{
				if (mJoblessRate[i]->IsEnable() && mJoblessRate[i]->GetThreadId() == 0)
				{
					StartHttpThread(mJoblessRate[i]);
				}
			}

			break;
		}
		case HOT_KEY_INCREASE_THRESHOLD:
		{
			UpdateData(TRUE);
			//if (mUintAutoCloseThreshold + THRESHOLD_STEP <= THRESHOLD_MAX)
			//{
			//	mUintAutoCloseThreshold += THRESHOLD_STEP;
			//	UpdateData(FALSE);
			//}

			mActionManager->PostThreadMessage(WM_DO_HUIFENG_GUADAN, (WPARAM) GetGuaDanParam(DO_BOTH), NULL);
			break;
		}
		case HOT_KEY_DECREASE_THRESHOLD:
		{
			UpdateData(TRUE);
			//if (mUintAutoCloseThreshold - THRESHOLD_STEP >= THRESHOLD_MIN)
			//{

			//	mUintAutoCloseThreshold -= THRESHOLD_STEP;
			//	UpdateData(FALSE);
			//}

			mActionManager->PostThreadMessage(WM_DO_TIANTONG_GUADAN, (WPARAM) GetGuaDanParam(DO_BOTH), NULL);
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
		else if (virKey == VK_T)
		{
			return HOT_KEY_TEST_SERVER;
		}
		else if (virKey == VK_D)
		{
			return HOT_KEY_DIRECT_DUO;
		}
		else if (virKey == VK_K)
		{
			return HOT_KEY_DIRECT_KONG;
		}
		else if (virKey == VK_S)
		{
			return HOT_KEY_INCREASE_THRESHOLD;
		}
		else if (virKey == VK_A)
		{
			return HOT_KEY_DECREASE_THRESHOLD;
		}
	}
	else if (mode == MOD_ALT)
	{
		if (virKey == VK_D)
		{
			return HOT_KEY_ALT_D;
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
	this->PostMessage(WM_DO_CANCEL_ORDER);
	return 0;
}

LRESULT CTradeAssistDlg::OnDoTradeMsg(WPARAM w , LPARAM l)
{
	BOOL direction = (UINT) LOWORD(w);
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

	return mActionManager->DoTrade(atof(mStrHighPriceDiff), atof(mStrLowPriceDiff),direction, mIntOrderCount);

}

// 获得剪贴板的内容
CString CTradeAssistDlg::GetContentFromClipboard(void)
{
	char *buffer = NULL;
	
	CString fromClipboard;
	int i = 0;
	while (i++ < OPEN_CLIPBOARD_MAX_RETRY_TIMES)
	{
		if (OpenClipboard())
		{
			HANDLE hData = GetClipboardData(CF_TEXT);  
			char * buffer = (char*)GlobalLock(hData);  
			fromClipboard = buffer;  
			GlobalUnlock(hData);  
			CloseClipboard();
			break;
		}
		else
		{
			TRACE("GetContentFromClipboard failed, error=%d\r\n", ::GetLastError());
		}
	}
	TRACE("GetContentFromClipboard i=%d\r\n", i);

	return fromClipboard;
}

CTradeAssistDlg::~CTradeAssistDlg()
{
	HANDLE hp=mActionManager->m_hThread;
	if (hp) 
	{
		if (WaitForSingleObject(hp, 1) != WAIT_OBJECT_0)
		{
			TerminateThread(hp,0);
		}
		//CloseHandle(hp);
	}

	delete mHttpWorker;
	delete mDataK;


	delete mJoblessRate[0]->GetThreadData();
	delete mNonfarmerNumber[0]->GetThreadData();

	for (int i =0; i < THREAD_NUMBER; i++)
	{
		CloseHttpThread(mJoblessRate[i]);
		CloseHttpThread(mNonfarmerNumber[i]);

		delete mNonfarmerNumber[i];
		delete mJoblessRate[i];
	}

	delete[] mNonfarmerNumber;
	delete[] mJoblessRate;	

	for (int i = 0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
	{
		CloseHttpThread(mLocalPrice[i]);
		delete mLocalPrice[i];
	}

	delete[] mLocalPrice;
}

int CTradeAssistDlg::InitialSetting(void)
{
	CString outText;
	mStrHighPriceDiff.Format(_T("%d"), theApp.GetProfileInt(STRING_SETTING, STRING_HIGH_PRICE_DIFF, 10));
	mStrLowPriceDiff.Format(_T("%d"), theApp.GetProfileInt(STRING_SETTING, STRING_LOW_PRICE_DIFF, 10));
	mIntOrderCount = theApp.GetProfileInt(STRING_SETTING, STRING_KEY_COUNT, 6);

	mIsAutoSubmits = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, FALSE);
	mAutoCompleteInterval = theApp.GetProfileString(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, NULL);
	mIntMsgDelayMilliSeconds = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_MSG_DELAY_TIME, 100);
	mIntHour = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_HOUR, 0);
	mIntMinute = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_MINUTE, 0);
	mIntSecond = theApp.GetProfileString(STRING_SETTING, STRING_EDIT_SECOND, NULL);
	mEnableCheckAutoCloseDepot = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_BOX_ENABLE_CHECK_AUTO_CLOSE_DEPOT, 0);
	mUintAutoCloseThreshold = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_AUTO_CLOSE_THRESHOLD, 20);

	if (mUintAutoCloseThreshold <THRESHOLD_MIN || mUintAutoCloseThreshold > THRESHOLD_MAX)
	{
		mUintAutoCloseThreshold = 10;
	}

	mServerIp = theApp.GetProfileString(STRING_SETTING, STRING_EDIT_SERVER_HOST, NULL);
	mServerPort = theApp.GetProfileInt(STRING_SETTING, STRING_EDIT_SERVER_PORT, 80);
	mUintDoHttpInterval = theApp.GetProfileInt(STRING_SETTING,STRING_EDIT_DO_HTTP_INTERVAL, 0) ;
	mBoolEnableAutoThreshold = theApp.GetProfileInt(STRING_SETTING, STRING_CHECK_ENABLE_AUTO_THRESHOLD, 1);
	mEnableChaseTimer = theApp.GetProfileInt(  STRING_SETTING, STRING_EDIT_ENABLE_CHASE_TIMER, 1);
	mEnableWindowMostTop =  theApp.GetProfileInt(  STRING_SETTING, STRING_CHECK_ENABLE_MOST_TOP, 1);
	UpdateData(FALSE);

	return 0;
}

int CTradeAssistDlg::SaveSetting(void)
{
	UpdateData(TRUE);
	theApp.WriteProfileInt(STRING_SETTING, STRING_HIGH_PRICE_DIFF , atoi(mStrHighPriceDiff));
	theApp.WriteProfileInt(STRING_SETTING, STRING_LOW_PRICE_DIFF , atoi(mStrLowPriceDiff));
	theApp.WriteProfileInt(STRING_SETTING, STRING_KEY_COUNT , mIntOrderCount);

	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_BOX_AUTO_SUBMIT, mIsAutoSubmits);
	theApp.WriteProfileString(STRING_SETTING, STRING_EDIT_AUTO_COMPLETE_INTERVAL, mAutoCompleteInterval);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_MSG_DELAY_TIME, mIntMsgDelayMilliSeconds);
	
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_HOUR, mIntHour);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_MINUTE, mIntMinute);
	theApp.WriteProfileString(STRING_SETTING, STRING_EDIT_SECOND, mIntSecond);
	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_BOX_ENABLE_CHECK_AUTO_CLOSE_DEPOT, mEnableCheckAutoCloseDepot);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_AUTO_CLOSE_THRESHOLD, mUintAutoCloseThreshold);

	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_SERVER_PORT, mServerPort);
	theApp.WriteProfileString(STRING_SETTING, STRING_EDIT_SERVER_HOST, mServerIp);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_DO_HTTP_INTERVAL, mUintDoHttpInterval);
	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_ENABLE_AUTO_THRESHOLD, mBoolEnableAutoThreshold);
	theApp.WriteProfileInt(STRING_SETTING, STRING_EDIT_ENABLE_CHASE_TIMER, mEnableChaseTimer);
	theApp.WriteProfileInt(STRING_SETTING, STRING_CHECK_ENABLE_MOST_TOP, mEnableWindowMostTop);
	return 0;
}

// 获取价格到手数控件的位移设置
POINT CTradeAssistDlg::GetPrice2CountVector(BOOL isHigh)
{
	POINT point;
	CPoint p = mLuaEngine.getPrice2Count(isHigh?DO_HIGH:DO_LOW);
	point.x = p.x;
	point.y = p.y;
	return point;
}

// 方向到价格的位移
POINT CTradeAssistDlg::GetDirection2PriceVector(BOOL isHigh)
{
	POINT point;
	CPoint p = mLuaEngine.getDirection2Price(isHigh?DO_HIGH:DO_LOW);
	point.x =p.x;
	point.y =p.y;
	return point;
}

LRESULT CTradeAssistDlg::OnAltDMsg(WPARAM w , LPARAM l)
{
	//mActionManager->GetAction()->MouseDoubleClick();
	//mActionManager->GetAction()->KeyboardCopy();
	//AfxMessageBox(GetContentFromClipboard());
	OnFlashComplete(); 
	return LRESULT();
}

LRESULT CTradeAssistDlg::OnDeleteOrderMsg(WPARAM w , LPARAM l)
{
	mActionManager->GetAction()->MouseClick();
	UpdateData(TRUE);
	Sleep(DELETE_ORDER_DELAY);

	POINT lpPoint;
	GetCursorPos(&lpPoint);

	//1.当前位置双击弹出下单对话框。
	int searchCount = 10;
	while (searchCount-- > 0)
	{
		POINT pos =mActionManager->GetSunAwtDialogPos(_T("订单窗口"));
		if (pos.x != 0 && pos.y != 0)
		{
			CPoint origin2Remove = mLuaEngine.getOrigin2Remove();
			CPoint delete2Confirm = mLuaEngine.getRemove2Confirm();
			mActionManager->GetAction()->MoveCursor(pos.x, pos.y, true);
			mActionManager->GetAction()->MoveCursor(origin2Remove.x,origin2Remove.y);
			Sleep(DELETE_ORDER_DELAY);
			mActionManager->GetAction()->MouseClick();
			mActionManager->GetAction()->MoveCursor(delete2Confirm.x, delete2Confirm.y);
			Sleep(DELETE_ORDER_DELAY*2);
			mActionManager->GetAction()->MouseClick();
			break;
		}
		Sleep(WINDOW_CHECK_INTERVAL);
	}

	mActionManager->GetAction()->MoveCursor(lpPoint.x, lpPoint.y, true);

	return LRESULT();
}


// 指价委托到方向
POINT CTradeAssistDlg::GetTab2Direction(BOOL isHigh)
{
	POINT point;
	CPoint p = mLuaEngine.getEntrust2Direction(isHigh?DO_HIGH:DO_LOW);
	point.x = p.x;
	point.y = p.y;
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
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_TEST_SERVER);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_DIRECT_DUO);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_DIRECT_KONG);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_INCREASE_THRESHOLD);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_DECREASE_THRESHOLD);
	::UnregisterHotKey(GetSafeHwnd(),HOT_KEY_ALT_D);
	
	KillTimer(TIMER_ID_FOR_DO_FLASH_TRADE);
	SaveSetting();


	CLogger::SaveLog();

	return 0;
}

// 秒杀下单
int CTradeAssistDlg::OnFlashComplete(void)
{
	UpdateData(TRUE);
	mIsAutoSubmits = TRUE;	
	UpdateData(FALSE);

	WORD	start = GetMilliseconds();
	POINT lpPoint;
	GetCursorPos(&lpPoint);
	mActionManager->GetAction()->MouseClick();

	int retryTimes = 0;
	int result = SemicAutoTrade(DO_LOW);
	while (result != SEMIC_AUTO_TRADE_CALL_SUCCESS && retryTimes++ < SEMIC_AUTO_TRADE_RETRY_TIMES)
	{
		SetCursorPos(lpPoint.x, lpPoint.y);
		mActionManager->GetAction()->MouseClick();
		result = SemicAutoTrade(DO_LOW);
	}

	CString log;
	log.Format(_T("OnFlashComplete low time1=%d, retryTimes=%d"), GetMilliseconds() - start, retryTimes);
	CLogger::Add(log);

	//2.延时下单间隔

#ifdef _DEBUG
	TRACE("OnFlashComplete time1=%d\r\n", GetMilliseconds() - start);
#endif // _DEBUG

	if (mAutoCompleteInterval.Trim().GetLength() > 0)
	{	
		Sleep(static_cast<DWORD>(atof(mAutoCompleteInterval.Trim())*1000));
	}

	//3.移动鼠标到双击位置。
	SetCursorPos(lpPoint.x, lpPoint.y);
	mActionManager->GetAction()->MouseClick();

	retryTimes = 0;
	result = SemicAutoTrade(DO_HIGH);
	while (result != SEMIC_AUTO_TRADE_CALL_SUCCESS && retryTimes++ < SEMIC_AUTO_TRADE_RETRY_TIMES)
	{
		SetCursorPos(lpPoint.x, lpPoint.y);
		mActionManager->GetAction()->MouseClick();
		result = SemicAutoTrade(DO_HIGH);
	}

	log.Format(_T("OnFlashComplete high time2=%d, retryTimes=%d"), GetMilliseconds() - start, retryTimes);
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
			log.Format(_T("SemicAutoTrade direct=%d, dialogSearchCount=%d"), direct, searchCount);
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


WORD CTradeAssistDlg::GetMilliseconds(void)
{

	SYSTEMTIME time;
	GetSystemTime(&time);
	return time.wSecond * 1000 + time.wMilliseconds;
}


void CTradeAssistDlg::OnTimer(UINT_PTR nIDEvent)
{

	if (nIDEvent == TIMER_ID_FOR_DO_FLASH_TRADE)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		int seconds = 0;
		int millionSecond = 0;
		if (mIntSecond.Trim().GetLength() == 0)
		{
			mIntSecond = _T("0.0");
		}

		int value = atoi(mIntSecond) * 1000;
		seconds = value / 1000;
		millionSecond = value - seconds*1000;

		if ((!mIsTimer4Tomorrow && mIntHour == time.wHour && mIntMinute == time.wMinute && seconds == time.wSecond && millionSecond <= time.wMilliseconds)
			|| (mIsTimer4Tomorrow&& mIntHour == time.wHour && mIntMinute == time.wMinute && seconds == time.wSecond && abs(millionSecond - time.wMilliseconds) <= 25 ))
		{

			KillTimer(nIDEvent);	
			GetDlgItem(IDC_BUTTON_START_TIMER)->EnableWindow(TRUE);

			if (mEnableChaseTimer)
			{
				for (int i = 0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
				{
					RetartThread(mLocalPrice[i]);
				}
			}
			else
			{
				if (mLuaEngine.GetDoubleSideType() == ON_TIMER_HUIFENG)
				{
					//TOD在接收消失时释放该缓存。
					mActionManager->PostThreadMessage(WM_DO_HUIFENG_GUADAN, (WPARAM) GetGuaDanParam(DO_BOTH), NULL);
				} 
				else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_ZHONXIN)
				{
					//中鑫龙祥
					OnFlashComplete(); 
					if(mEnableCheckAutoCloseDepot)
					{
						mProgressAutoCloseDepot.SetRange(0, mUintAutoCloseThreshold);
						for (int i = 0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
						{
							RetartThread(mLocalPrice[i]);
						}
					}
				}
				else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_TIANTONG)
				{
					//TOD在接收消失时释放该缓存。
					mActionManager->PostThreadMessage(WM_DO_TIANTONG_GUADAN, (WPARAM) GetGuaDanParam(DO_BOTH), NULL);
				}
				else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_KUNJIAO)
				{
					//TOD在接收消失时释放该缓存。
					mActionManager->PostThreadMessage(WM_DO_KUNJIAO_GUADAN, (WPARAM) GetGuaDanParam(DO_BOTH), NULL);
				}
				else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_LUOGE)
				{
					//TOD在接收消失时释放该缓存。
					mActionManager->PostThreadMessage(WM_DO_LUOGE_GUADAN, (WPARAM) GetGuaDanParam(DO_BOTH), NULL);
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CTradeAssistDlg::RetartThread(PEcnomicData data)
{
	if (data != NULL)
	{
		CloseHttpThread(data);
		StartHttpThread(data);
	}
}

void CTradeAssistDlg::OnBnClickedButtonStartTimer()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	UpdateData(TRUE);
	if (mIntSecond.Trim().GetLength() == 0)
	{
		mIntSecond = _T("0.0");
	}

	if (mServerIp.Trim().GetLength() == 0 || mServerIp.Trim().GetLength() == 0)
	{
		CString text;
		text.LoadString(IDS_STRING_SERVER_INFO_INVALID);
		MessageBox(text);
		return ;
	}

	if (mIntHour * 3600 + mIntMinute * 60 + atof(mIntSecond) > time.wHour  * 3600 +  time.wMinute * 60 + time.wSecond + time.wMilliseconds/1000.0F)
	{

		mIsTimer4Tomorrow = false;
	}
	else
	{
		mIsTimer4Tomorrow = true;
	}

	GetDlgItem(IDC_BUTTON_START_TIMER)->EnableWindow(FALSE);
	SetTimer(TIMER_ID_FOR_DO_FLASH_TRADE, TIMER_INTERVAL_DO_FLASH_TRADE, NULL); 
	
	if (mIsTimer4Tomorrow)
	{
		CString text;
		text.LoadString(IDS_INVALID_TIMER);
		MessageBox(text);
	}
}

LRESULT CTradeAssistDlg::OnHttpGetPriceFinish(WPARAM w , LPARAM l)
{

	PEcnomicData data = (PEcnomicData)l;
	bool	updateUI  = true;
	bool	sendCycleMsg = false;
	if (w != NULL &&& l != NULL)
	{
		CDataPacketP packet = (CDataPacketP)w;
		mPullPriceCount++;
		sendCycleMsg = mPullPriceCount==DO_PULL_PRICE_TIMES_PER_MSG;

		if (sendCycleMsg)
		{
			mPullPriceCount = 0;
		}

		long current =GetTickCount();
		if (current - mUILastUpdateTime > 500)
		{
			mUILastUpdateTime = current;
			updateUI = true;
		}
		else
		{
			updateUI = false;
		}

		if (packet->mIsGood)
		{
			//暂时关闭中鑫的追单
			//CheckChaseMoment(packet); 
			if(packet->mChaseDirect == DO_LOW && !mLuaEngine.GetHasChased())
			{
				//追空挂多
				mLuaEngine.SetHasChased(true);
				mActionManager->PostThreadMessage(WM_DO_HUIFENG_GUADAN, (WPARAM) GetGuaDanParam(DO_HIGH), NULL);
			}
			else if (packet->mChaseDirect == DO_HIGH && !mLuaEngine.GetHasChased())
			{
				//追多挂空
				mLuaEngine.SetHasChased(true);
				mActionManager->PostThreadMessage(WM_DO_HUIFENG_GUADAN, (WPARAM) GetGuaDanParam(DO_LOW), NULL);
			}
			

			if (updateUI)
			{
				if( CheckAutoCloseDepot(packet, data) != 0)
				{
					PostMessage(WM_DISPLAY_DATAK);	
				}
			}
		}		
	}

	//发送消息最多尝试三次
	if (sendCycleMsg)
	{
		mPullPriceCount = 0;
		int i = 0;
		while (i++ < SEND_MESSAGE_TO_THREAD_MAX_RETTY_TIMES && data != NULL)
		{
			if(!PostThreadMessage(data->GetThreadId(),WM_DO_HTTP_GET_PRICE,NULL, (LPARAM)data))//post thread msg
			{
				TRACE("post message failed,errno:%d\r\n",::GetLastError());
			}
			else
			{
				break;
			}
		}
}

	return 1;
}

LRESULT CTradeAssistDlg::OnDisplayDataK(WPARAM w, LPARAM l)
{
	UpdateData(TRUE);

	mDataKClose = mDataK->GetClose();
	mDataKOpen = mLuaEngine.GetStartPrice();//mDataK->GetOpen();
	mDataKCloseTime = mDataK->GetCloseTime().Right(8);
	mDataKOpenTime = mDataK->GetOpenTime().Right(8);
	mDataKHighPrice = mDataK->GetHigh();
	mDataKLowPrice = mDataK->GetLow();
	mDataKCurrent2ExtremeDiff = mDataK->GetCurrent2ExtremeDiff();
	mDataKStatisticsUpdrop = mDataK->GetAmplitude();
	mQueryPriceUseTime = mDataK->GetQueryPriceUseTime();
	mCapturePriceUseTime = mDataK->GetCapturePriceUseTime();
	mProgressAutoCloseDepot.SetPos(mDataKCurrent2ExtremeDiff);
	
	//更新进度条范围
	int low, up;
	mProgressAutoCloseDepot.GetRange(low, up);
	if (up != mUintAutoCloseThreshold)
	{
		mProgressAutoCloseDepot.SetRange(0, mUintAutoCloseThreshold);
	}
	UpdateData(FALSE);

	return 0;
}


UINT CTradeAssistDlg::GetServerPort(void)
{
	return mServerPort;
}

CString CTradeAssistDlg::GetServerHost(void)
{
	return mServerIp;
}

int CTradeAssistDlg::CloseHttpThread(PEcnomicData data)
{
	if (data != NULL && data->GetThreadId() != NULL)
	{

		if(!PostThreadMessage(data->GetThreadId(),WM_DO_HTTP_EXIT, NULL, NULL))//post thread msg
		{
			TRACE("post message failed,errno:%d\r\n",::GetLastError());
		}
		else
		{
			data->SetThreadId(NULL);
		}

		if (data->GetStartEvent() != NULL)
		{
			CloseHandle(data->GetStartEvent());
			data->SetStartEvent(NULL);
		}
	}

	return 0;
}

UINT CTradeAssistDlg::GetDoHttpInterval(void)
{
	return mUintDoHttpInterval;
}


int CTradeAssistDlg::GetDynamicThreshold(int back)
{

	return	mLuaEngine.GetDynamicThreshold(GetCurrentThreadId(), mBoolEnableAutoThreshold,back,mStrLowPriceDiff, mUintAutoCloseThreshold);
}

int CTradeAssistDlg::PlaySoundResource(int idRes)
{
	PlaySound(MAKEINTRESOURCE(idRes ),	AfxGetApp()->m_hInstance ,SND_RESOURCE | SND_ASYNC | SND_LOOP);
	return 0;
}

CLuaEngine CTradeAssistDlg::GetLuaEngine(void)
{
	return mLuaEngine;
}

LRESULT CTradeAssistDlg::OnHttpGetEcnomicData(WPARAM w, LPARAM l)
{

	if (w == NULL)
	{
		return  LRESULT();
	}

	PEcnomicData data = reinterpret_cast<PEcnomicData>(w);
	
	if(dynamic_cast<CNonfarmerNumberData*>(data) 
		&& (mActualNonfarmerNumber == HTML_PARSE_UNKOWN || mActualNonfarmerNumber.IsEmpty()))
	{
		mActualNonfarmerNumber = data->GetActualValue();
		mNonfarmerNumberResult = CUtil::TranslateEcomicResult(data->GetResult());
		mNonfarmerNumnerWeight.Format(_T("%.4f"),data->GetWeight());
		mNonfarmerNumberCount.Format(_T("%u"), data->GetHttpCount());
	}
	else if (dynamic_cast<CJoblessRateData*>(data)
		&& (mActualJoblessRate == HTML_PARSE_UNKOWN || mActualJoblessRate.IsEmpty()))
	{
		mActualJoblessRate = data->GetActualValue();
		mJoblessRateResult = CUtil::TranslateEcomicResult(data->GetResult());
		mJoblessRateWeight.Format(_T("%.4f"),data->GetWeight());
		mJoblessRateCount.Format(_T("%u"), data->GetHttpCount());
	}

	UpdateData(FALSE);

	CString msg;
	msg.Format("threadid=%X,url=%s,type=%d\r\n",data, data->GetUrl(), data->GetResult());
	TRACE(msg);
	CLogger::Add(msg);

	if (l == 1)
	{
		return LRESULT();
	}

	if (data->GetResult() == EcnomicResult::TYPE_UNKOWN)
	{
		//发送消息最多尝试三次
		int i = 0;
		while (i++ < SEND_MESSAGE_TO_THREAD_MAX_RETTY_TIMES)
		{
			if(!PostThreadMessage(data->GetThreadId(),data->GetMsgType(), (WPARAM)data, NULL))//post thread msg
			{
				TRACE("post message failed,errno:%d\r\n",::GetLastError());
			}
			else
			{
				break;
			}
		}

		return LRESULT();
	}

	DoEcnomicDataAction(data);

	return LRESULT();
}

void CTradeAssistDlg::StartHttpThread(PEcnomicData data)
{
	if (data == NULL)
	{
		return;
	}

	//开启自动平仓检测。
	if(data->GetStartEvent() == NULL)
	{
		data->SetStartEvent(::CreateEvent(0,FALSE,FALSE,0));
	}

	if(data->GetStartEvent() == NULL)
	{
		return;
	}

	if (data->GetThreadHandle() == NULL)
	{
		UINT id;
		data->SetThreadHandle((HANDLE)_beginthreadex( NULL, NULL, &HttpProcess, data, NULL, &id ));
		data->SetThreadId(id);
	} 

	if(data->GetThreadHandle() == NULL)
	{
		TRACE("start thread failed,errno:%d\r\n",::GetLastError());
		CloseHandle(data->GetStartEvent());
		data->SetStartEvent(NULL);
		return ;
	}

	::WaitForSingleObject(data->GetStartEvent(),INFINITE);
	CloseHandle(data->GetStartEvent());
	data->SetStartEvent(NULL);

	int count = 0;
	while(count++ < SEND_MESSAGE_TO_THREAD_MAX_RETTY_TIMES)
	{

		if(!PostThreadMessage(data->GetThreadId(),data->GetMsgType(),(WPARAM)data, NULL))
		{
			TRACE("post message failed,errno:%d\r\n",::GetLastError());
		}
		else
		{
			break;
		}
		::Sleep(500);
	}
}

LRESULT CTradeAssistDlg::OnDoChase(WPARAM wp, LPARAM lp)
{

	bool isDoHigh = 0;
	if (wp == DO_LOW)
	{
		mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoLow(),
			mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_LOW),mIntOrderCount, TRUE);
	}
	else if (wp == DO_HIGH)
	{
		mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoHigh(), 
			mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_HIGH),mIntOrderCount, TRUE);
	}
	else
	{
		return LRESULT();
	}

	return LRESULT();
}

void CTradeAssistDlg::CheckChaseMoment(CDataPacketP packet)
{

	if (packet == NULL)
	{
		return;
	}

	if (mLuaEngine.GetStartPrice() < 0.1F)
	{
		mLuaEngine.SetStartPrice(packet->mPrice);
		mLuaEngine.GetStartTime() = CTime::GetCurrentTime();
	}

	CTimeSpan span = CTime::GetCurrentTime() - mLuaEngine.GetStartTime();

	if (!mLuaEngine.GetHasChased() && span.GetTotalSeconds() < mLuaEngine.GetChaseMaxTime())
	{

		double diff = packet->mPrice - mLuaEngine.GetStartPrice();
		TRACE("diff=%f, priceThreshold=%d\r\n",diff,  mLuaEngine.GetChasePriceThreshold());
		if (diff > 0 && diff >= mLuaEngine.GetChasePriceThreshold()
			&& mLuaEngine.GetChasePriceMax() >= diff)
		{
			SendMessage(WM_DO_CHASE, DO_HIGH, NULL);
			mLuaEngine.SetHasChased(true);
		} 
		else if ( diff < 0 && abs(diff) >= mLuaEngine.GetChasePriceThreshold()
			&& mLuaEngine.GetChasePriceMax() >= abs(diff))
		{
			SendMessage(WM_DO_CHASE, DO_LOW, NULL);
			mLuaEngine.SetHasChased(true);
		}
		else if (mLuaEngine.GetChasePriceMax() < abs(diff))
		{
			mLuaEngine.SetHasChased(true);
		}
	}
}


LRESULT CTradeAssistDlg::CheckAutoCloseDepot( CDataPacketP packet, PEcnomicData data ) 
{

	if (packet == NULL || data == NULL)
	{
		return 1;
	}

	UpdateData();

	//更新价格
	mDataK->SetClose(packet->mPrice, packet->mPriceTime);
	mDataK->SetQueryPriceUseTime(packet->mQueryPriceUseTime);
	mDataK->SetCapturePriceUseTime(packet->mCapturePriceUseTime);

	//这里判读是否回调
	if (mDataK->IsPositive())
	{
		mDataK->SetCurrent2ExtremeDiff(static_cast<int>( mDataK->GetHigh() - mDataK->GetClose())) ;
		mOpenDirection = 0;
		UpdateData(FALSE);

		//做多分支
		if(mDataK->GetCurrent2ExtremeDiff() >= GetDynamicThreshold(static_cast<int>(mDataK->GetAmplitude())))
		{
			mActionManager->GetAction()->MouseClick();
			CloseHttpThread(data);
			PlaySoundResource(IDR_WAVE_DO_HIGH_CLOSE);
			return 0;
		}

	}
	else if(mDataK->IsNegtive() )
	{

		mDataK->SetCurrent2ExtremeDiff( static_cast<int>(mDataK->GetClose() - mDataK->GetLow()) );
		mOpenDirection = 1;
		UpdateData(FALSE);
		//做空分支
		if(mDataK->GetCurrent2ExtremeDiff() >=  GetDynamicThreshold(static_cast<int>(mDataK->GetAmplitude())))
		{
			mActionManager->GetAction()->MouseClick();
			CloseHttpThread(data);
			PlaySoundResource(IDR_WAVE_DO_LOW_CLOSE);
			return 0;
		}
	}

	return 1;
}

void CTradeAssistDlg::DoEcnomicDataAction( PEcnomicData data) 
{

	if (data == NULL)
	{
		return;
	}

	if (!mNonfarmerNumber[0]->GetUrl().IsEmpty() && !mJoblessRate[0]->GetUrl().IsEmpty())
	{

		if (mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_HIGH
			&& mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_HIGH)
		{
			mTotalConclution = CUtil::TranslateEcomicResult(data->GetResult());
			mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoHigh(), 
				mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_HIGH),mIntOrderCount, FALSE);
		}
		else if (mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_LOW
			&& mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_LOW)
		{
			mTotalConclution = CUtil::TranslateEcomicResult(data->GetResult());
			mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoLow(),
				mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_LOW),mIntOrderCount, FALSE);
		}
		else if (mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_HIGH
			&&  mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_LOW)
		{
			if (mNonfarmerNumber[0]->GetWeight() > mJoblessRate[0]->GetWeight())
			{
				mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_HIGH);
				mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoHigh(), 
					mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_HIGH),mIntOrderCount, TRUE);
			} 
			else if (mNonfarmerNumber[0]->GetWeight() < mJoblessRate[0]->GetWeight())
			{
				mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_LOW);
				mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoLow(),
					mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_LOW),mIntOrderCount, TRUE);
			}
		}
		else if (mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_LOW
			&&  mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_HIGH)
		{
			if (mNonfarmerNumber[0]->GetWeight() > mJoblessRate[0]->GetWeight())
			{
				mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_LOW);
				mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoLow(),
					mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_LOW),mIntOrderCount, TRUE);
			} 
			else if (mNonfarmerNumber[0]->GetWeight() < mJoblessRate[0]->GetWeight())
			{

				mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_HIGH);
				mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoHigh(), 
					mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_HIGH),mIntOrderCount, TRUE);
			}
		}
		else if (mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_EQUAL)
		{
			if (mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_LOW)
			{
				mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_LOW);
				mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoLow(),
					mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_LOW),mIntOrderCount, TRUE);
			}
			else if (mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_HIGH)
			{
				mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_HIGH);
				mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoHigh(), 
					mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_HIGH),mIntOrderCount, TRUE);
			}
		}
		else if (mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_EQUAL)
		{
			if (mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_LOW)
			{
				mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_LOW);
				mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoLow(),
					mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_LOW),mIntOrderCount, TRUE);
			}
			else if (mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_HIGH)
			{
				mTotalConclution = CUtil::TranslateEcomicResult(EcnomicResult::TYPE_HIGH);
				mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoHigh(), 
					mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_HIGH),mIntOrderCount, TRUE);
			}
		}

		UpdateData(FALSE);

	}
	else if (!mNonfarmerNumber[0]->GetUrl().IsEmpty())
	{
		if (mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_HIGH)
		{
			mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoHigh(), 
				mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_HIGH),mIntOrderCount, FALSE);
		} 
		else if(mNonfarmerNumber[0]->GetResult() == EcnomicResult::TYPE_LOW)
		{
			mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoLow(),
				mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_LOW),mIntOrderCount, FALSE);
		}
	}
	else if ( !mJoblessRate[0]->GetUrl().IsEmpty())
	{
		if (mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_HIGH)
		{
			mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoHigh(), 
				mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_HIGH),mIntOrderCount, FALSE);
		} 
		else if(mJoblessRate[0]->GetResult() == EcnomicResult::TYPE_LOW)
		{
			mActionManager->MakeOrder(mLuaEngine.GetOriginal2DoLow(),
				mLuaEngine.GetOrigin2Count(),mLuaEngine.GetCount2OrderButton(DO_LOW),mIntOrderCount, FALSE);
		}
	}
}
void CTradeAssistDlg::OnBnClickedCheckSetMostTop()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_SET_MOST_TOP);
	mEnableWindowMostTop = pBtn->GetCheck();
	//当state == 1时表示该复选框没有被选中；
	//当state == 0时表示该复选框被选中；

	EnableWindowMostTop(mEnableWindowMostTop);
}

void CTradeAssistDlg::EnableWindowMostTop(BOOL isTop)
{
	if (isTop)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	} 
	else
	{
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

		CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_SET_MOST_TOP);
		pBtn->SetCheck(isTop?1:0);
}

CHuifengGuadanParamP CTradeAssistDlg::GetGuaDanParam(int dirct)
{
	CHuifengGuadanParamP param = new CHuifengGuadanParam();
	param->mLowDiff = atoi(mStrLowPriceDiff);
	param->mHighDiff = atoi(mStrHighPriceDiff);
	param->mTradeCount = mIntOrderCount;
	param->mWindowDelay = mIntMsgDelayMilliSeconds;
	param->mDirect = dirct;
	return param;
}
