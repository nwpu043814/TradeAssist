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
#include "tiantongguadan.h"

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
	, mDataManager(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mHttpWorker = new CHttpWorker();
	mDataK = new CDataK();
	mActionManager =(CActionManagerP) AfxBeginThread(RUNTIME_CLASS(CActionManager));
	mActionManager->SetWindowOwner(GetSafeHwnd());
	mDataManager = new CDataManager();
	mDataManager->initialNonfarmerThread(&mLuaEngine);
	mDataManager->initialJoblessRateThread(&mLuaEngine);
	mDataManager->initialLocalPriceThread();

	InitializeCriticalSection(&g_cs);
}


void CTradeAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AOTO_SUBMIT, mDataManager->mIsAutoSubmits);
	DDX_Text(pDX, IDC_EDIT_AUTO_COMPLETE_INTERVAL, mDataManager->mAutoCompleteInterval);
	DDV_MaxChars(pDX, mDataManager->mAutoCompleteInterval, 3);
	DDX_Text(pDX, IDC_EDIT_HIGH_PRICE_DIFF, mDataManager->mStrHighPriceDiff);
	DDV_MaxChars(pDX, mDataManager->mStrHighPriceDiff, 4);
	DDX_Text(pDX, IDC_EDIT_COUNT, mDataManager->mIntOrderCount);
	DDV_MinMaxUInt(pDX, mDataManager->mIntOrderCount, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MSG_DELAY, mDataManager->mIntMsgDelayMilliSeconds);
	DDV_MinMaxUInt(pDX, mDataManager->mIntMsgDelayMilliSeconds, 0, 80000);
	DDX_Text(pDX, IDC_EDIT_HOUR,mDataManager-> mIntHour);
	DDX_Text(pDX, IDC_EDIT_MINUTE, mDataManager->mIntMinute);
	DDX_Text(pDX, IDC_EDIT_SECOND, mDataManager->mIntSecond);
	DDX_Check(pDX, IDC_CHECK_AUTO_CLOSE_DEPOT, mDataManager->mEnableCheckAutoCloseDepot);
	DDX_Text(pDX, IDC_EDIT_LOW_PRICE_DIFF, mDataManager->mStrLowPriceDiff);
	DDV_MaxChars(pDX, mDataManager->mStrLowPriceDiff, 4);
	DDX_Text(pDX, IDC_EDIT_DATAK_CURRENT_PRICE, mDataManager->mDataKClose);
	DDX_Text(pDX, IDC_EDIT_DATAK_OPEN_PRICE,mDataManager-> mDataKOpen);
	DDX_Text(pDX, IDC_EDIT_CURRENT_TIME,mDataManager-> mDataKCloseTime);
	DDV_MaxChars(pDX,mDataManager-> mDataKCloseTime, 10);
	DDX_Text(pDX, IDC_EDIT_DATAK_OPEN_TIME, mDataManager->mDataKOpenTime);
	DDV_MaxChars(pDX, mDataManager->mDataKOpenTime, 10);
	DDX_Text(pDX, IDC_EDIT_HIGH_PRICE, mDataManager->mDataKHighPrice);
	DDX_Text(pDX, IDC_EDIT_DATAK_LOW_PRICE, mDataManager->mDataKLowPrice);
	DDX_Text(pDX, IDC_EDIT_AUTO_CLOSE_THRESHOLD,mDataManager-> mUintAutoCloseThreshold);
	DDV_MinMaxUInt(pDX,mDataManager-> mUintAutoCloseThreshold, THRESHOLD_MIN, THRESHOLD_MAX);
	DDX_Text(pDX, IDC_EDIT_DATAK_STATISTICS_UPDROP, mDataManager->mDataKStatisticsUpdrop);
	DDX_Text(pDX, IDC_EDIT_DATAK_DAY_UPDROP,mDataManager-> mDataKDayUpdrop);
	DDX_Text(pDX, IDC_EDIT_CURRENT_EXTREME_DIFF, mDataManager->mDataKCurrent2ExtremeDiff);
	DDV_MinMaxInt(pDX, mDataManager->mDataKCurrent2ExtremeDiff, -100, 100);
	DDX_Control(pDX, IDC_PROGRESS_AUTO_CLOSE_DEPOT, mProgressAutoCloseDepot);
	DDX_Radio(pDX, IDC_RADI_HIGH,mDataManager-> mOpenDirection);
	DDX_Text(pDX, IDC_EDIT_SERVER_HOST,mDataManager-> mServerIp);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT,mDataManager-> mServerPort);
	DDX_Text(pDX, IDC_EDIT_DO_HTTP_REQUEST_INTERVAL, mDataManager->mUintDoHttpInterval);
	DDV_MinMaxUInt(pDX, mDataManager->mUintDoHttpInterval, 0, 1000);
	DDX_Check(pDX, IDC_CHECK_ENABLE_AUTO_THRESHOLD, mDataManager->mBoolEnableAutoThreshold);
	DDX_Text(pDX, IDC_EDIT_NONFARMER_NUMBER, mDataManager->mActualNonfarmerNumber);
	DDX_Text(pDX, IDC_EDIT_JOBLESS_RATE, mDataManager->mActualJoblessRate);
	DDX_Text(pDX, IDC_EDIT_NONFARMER_NUMBER_RESULT,mDataManager-> mNonfarmerNumberResult);
	DDX_Text(pDX, IDC_EDIT_JOBLESS_RATE_RESULT,mDataManager-> mJoblessRateResult);
	DDX_Text(pDX, IDC_EDIT_NONFARMER_NUMBER_WEIGHT, mDataManager->mNonfarmerNumnerWeight);
	DDX_Text(pDX, IDC_EDIT_JOBLESS_RATE_WEIGHT, mDataManager->mJoblessRateWeight);
	DDX_Text(pDX, IDC_EDIT_NONFARMER_NUMBER_WEIGHT2,mDataManager-> mNonfarmerNumberCount);
	DDX_Text(pDX, IDC_EDIT_JOBLESS_RATE_WEIGHT2, mDataManager->mJoblessRateCount);
	DDX_Text(pDX, IDC_EDIT_TOTAL_CONCLUTION, mDataManager->mTotalConclution);
	DDX_Text(pDX, IDC_EDIT_QURERY_PRICE_USE_TIME, mDataManager->mQueryPriceUseTime);
	DDX_Text(pDX, IDC_EDIT_CAPTURE_PRICE_USE_TIME, mDataManager->mCapturePriceUseTime);
}

BEGIN_MESSAGE_MAP(CTradeAssistDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_HOTKEY, &CTradeAssistDlg::OnHotKey) //添加此句
	ON_MESSAGE(WM_DISPLAY_DATAK,  &CTradeAssistDlg::OnDisplayDataK)
	ON_MESSAGE(WM_HTTP_GET_FINISH,&CTradeAssistDlg::OnHttpGetPriceFinish)
	ON_MESSAGE(WM_ALT_D,&CTradeAssistDlg::OnAltDMsg)
	ON_MESSAGE(WM_HTTP_GET_ECNOMIC_DATA_FINISH,&CTradeAssistDlg::OnHttpGetEcnomicData) 
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
	mHotkeyManager = new CHotKeyManager(m_hWnd);
	mHotkeyManager->InstallHotKey();
	InitialSetting();
	EnableWindowMostTop(mDataManager->mEnableWindowMostTop);
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
	mDataManager->mLastTime = GetMilliseconds();
#endif

	UINT Mod = (UINT) LOWORD(lParam); // key-modifier flags 
	UINT uVirtKey = (UINT) HIWORD(lParam); // virtual-key code 

	int result = mHotkeyManager->ParseHotKey(Mod, uVirtKey);

	switch(result)
	{
		case HOT_KEY_CODE_LOW:
		{
			dispatchLowAction();
			break;
		}
		case HOT_KEY_CODE_HIGH:
		{
			break;
		}
		case HOT_KEY_CODE_COUNT:
		{
			
			break;
		}
		case HOT_KEY_FLASH_COMPLETE:
		{
			mActionManager->DoTiantongStopLose(DO_LOW);
			break;;
		}
		case HOT_KEY_DECREASE_PRICE:
		{
			UpdateData();
			mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_ZHONGXIN_GUADAN);
			break;
		}
		case HOT_KEY_INCREASE_PRICE:
		{
			UpdateData();
			mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_LUOGE_GUADAN);
			break;
		}
		case HOT_KEY_TEST_SERVER:
		{
			UpdateData();
			for (int i = 0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
			{
				mDataManager->StartHttpThread(mDataManager->mLocalPrice[i]);
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

			mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM)mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_KUNJIAO_GUADAN);
			break;
		}
		case HOT_KEY_DIRECT_KONG:
		{
			UpdateData(TRUE);
			mDataManager->StartUpdateFeiNongData();
			break;
		}
		case HOT_KEY_INCREASE_THRESHOLD:
		{
			mActionManager->DoTiantongStopLose(DO_HIGH);
			break;
		}
		case HOT_KEY_DECREASE_THRESHOLD:
		{
			UpdateData(TRUE);
			mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_TIANTONG_GUADAN);
			break;
		}
	}

	return 1;
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
	delete mHotkeyManager;

	delete mDataManager->mJoblessRate[0]->GetThreadData();
	delete mDataManager->mNonfarmerNumber[0]->GetThreadData();

	for (int i =0; i < THREAD_NUMBER; i++)
	{
		CloseHttpThread(mDataManager->mJoblessRate[i]);
		CloseHttpThread(mDataManager->mNonfarmerNumber[i]);

		delete mDataManager->mNonfarmerNumber[i];
		delete mDataManager->mJoblessRate[i];
	}

	delete[] mDataManager->mNonfarmerNumber;
	delete[] mDataManager->mJoblessRate;	

	for (int i = 0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
	{
		CloseHttpThread(mDataManager->mLocalPrice[i]);
		delete mDataManager->mLocalPrice[i];
	}

	delete[] mDataManager->mLocalPrice;
	delete mDataManager;
}

int CTradeAssistDlg::InitialSetting(void)
{
	mDataManager->Load();
	UpdateData(FALSE);
	return 0;
}

int CTradeAssistDlg::SaveSetting(void)
{
	UpdateData(TRUE);
	mDataManager->Save();
	return 0;
}


LRESULT CTradeAssistDlg::OnAltDMsg(WPARAM w , LPARAM l)
{
	UpdateData();
	mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_HAIJIAO_GUADAN);
	return LRESULT();
}

int CTradeAssistDlg::ClearResource(void)
{
	mHotkeyManager->UninstallHotkey();
	KillTimer(TIMER_ID_FOR_DO_FLASH_TRADE);
	SaveSetting();
	CLogger::SaveLog();

	return 0;
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
		if (mDataManager->mIntSecond.Trim().GetLength() == 0)
		{
			mDataManager->mIntSecond = _T("0.0");
		}

		int value = atoi(mDataManager->mIntSecond) * 1000;
		seconds = value / 1000;
		millionSecond = value - seconds*1000;

		if ((!mDataManager->mIsTimer4Tomorrow &&mDataManager->mIntHour == time.wHour && mDataManager->mIntMinute == time.wMinute && seconds == time.wSecond && millionSecond <= time.wMilliseconds)
			|| (mDataManager->mIsTimer4Tomorrow&& mDataManager->mIntHour == time.wHour &&mDataManager->mIntMinute == time.wMinute && seconds == time.wSecond && abs(millionSecond - time.wMilliseconds) <= 25 ))
		{

			KillTimer(nIDEvent);	
			GetDlgItem(IDC_BUTTON_START_TIMER)->EnableWindow(TRUE);		
			
			if (mLuaEngine.GetDoubleSideType() == ON_TIMER_LOCALCAPTURE)
			{
				//启动拉取本地服务器数据服务。
				for (int i = 0; i < LOCAL_SERVER_REQUEST_THREADED_NUMBER; i++)
				{
					RetartThread(mDataManager->mLocalPrice[i]);
				}
			}
			else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_ZHONGXIN)
			{
				//中鑫龙祥
				mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_ZHONGXIN_GUADAN);
			}
			else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_TIANTONG)
			{
				//TOD在接收消失时释放该缓存。
				mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_TIANTONG_GUADAN);
			}
			else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_KUNJIAO)
			{
				//TOD在接收消失时释放该缓存。
				mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_KUNJIAO_GUADAN);
			}
			else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_LUOGE)
			{
				//TOD在接收消失时释放该缓存。
				mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_LUOGE_GUADAN);
			}
			else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_HAIJIAO)
			{
				//TOD在接收消失时释放该缓存。
				mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_BOTH), WM_DO_HAIJIAO_GUADAN);
			}
			else if (mLuaEngine.GetDoubleSideType() == ON_TIMER_FEINONG)
			{
				mDataManager->StartUpdateFeiNongData();
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
		mDataManager->StartHttpThread(data);
	}
}

void CTradeAssistDlg::OnBnClickedButtonStartTimer()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	UpdateData(TRUE);
	if (mDataManager->mIntSecond.Trim().GetLength() == 0)
	{
		mDataManager->mIntSecond = _T("0.0");
	}

	if (mDataManager->mServerIp.Trim().GetLength() == 0 ||mDataManager->mServerIp.Trim().GetLength() == 0)
	{
		CString text;
		text.LoadString(IDS_STRING_SERVER_INFO_INVALID);
		MessageBox(text);
		return ;
	}

	if (mDataManager->mIntHour * 3600 + mDataManager->mIntMinute * 60 + atof(mDataManager->mIntSecond) > time.wHour  * 3600 +  time.wMinute * 60 + time.wSecond + time.wMilliseconds/1000.0F)
	{

		mDataManager->mIsTimer4Tomorrow = false;
	}
	else
	{
		mDataManager->mIsTimer4Tomorrow = true;
	}

	GetDlgItem(IDC_BUTTON_START_TIMER)->EnableWindow(FALSE);
	SetTimer(TIMER_ID_FOR_DO_FLASH_TRADE, TIMER_INTERVAL_DO_FLASH_TRADE, NULL); 
	
	if (mDataManager->mIsTimer4Tomorrow)
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
		mDataManager->mPullPriceCount++;
		sendCycleMsg = mDataManager->mPullPriceCount==DO_PULL_PRICE_TIMES_PER_MSG;

		if (sendCycleMsg)
		{
			mDataManager->mPullPriceCount = 0;
		}

		long current =GetTickCount();
		if (current - mDataManager->mUILastUpdateTime > 500)
		{
			mDataManager->mUILastUpdateTime = current;
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
				mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_HIGH), WM_DO_HAIJIAO_GUADAN);
			}
			else if (packet->mChaseDirect == DO_HIGH && !mLuaEngine.GetHasChased())
			{
				//追多挂空
				mLuaEngine.SetHasChased(true);
				mActionManager->PostThreadMessage(WM_DO_GUADAN, (WPARAM) mDataManager->GetGuaDanParam(DO_LOW), WM_DO_HAIJIAO_GUADAN);
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
		mDataManager->mPullPriceCount = 0;
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

	mDataManager->mDataKClose = mDataK->GetClose();
	mDataManager->mDataKOpen = mLuaEngine.GetStartPrice();//mDataK->GetOpen();
	mDataManager->mDataKCloseTime = mDataK->GetCloseTime().Right(8);
	mDataManager->mDataKOpenTime = mDataK->GetOpenTime().Right(8);
	mDataManager->mDataKHighPrice = mDataK->GetHigh();
	mDataManager->mDataKLowPrice = mDataK->GetLow();
	mDataManager->mDataKCurrent2ExtremeDiff = mDataK->GetCurrent2ExtremeDiff();
	mDataManager->mDataKStatisticsUpdrop = mDataK->GetAmplitude();
	mDataManager->mQueryPriceUseTime = mDataK->GetQueryPriceUseTime();
	mDataManager->mCapturePriceUseTime = mDataK->GetCapturePriceUseTime();
	mProgressAutoCloseDepot.SetPos(mDataManager->mDataKCurrent2ExtremeDiff);
	
	//更新进度条范围
	int low, up;
	mProgressAutoCloseDepot.GetRange(low, up);
	if (up != mDataManager->mUintAutoCloseThreshold)
	{
		mProgressAutoCloseDepot.SetRange(0,mDataManager->mUintAutoCloseThreshold);
	}
	UpdateData(FALSE);

	return 0;
}


UINT CTradeAssistDlg::GetServerPort(void)
{
	return mDataManager->mServerPort;
}

CString CTradeAssistDlg::GetServerHost(void)
{
	return mDataManager->mServerIp;
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
	return mDataManager->mUintDoHttpInterval;
}


int CTradeAssistDlg::GetDynamicThreshold(int back)
{

	return	mLuaEngine.GetDynamicThreshold(GetCurrentThreadId(), mDataManager->mBoolEnableAutoThreshold,back,mDataManager->mStrLowPriceDiff, mDataManager->mUintAutoCloseThreshold);
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

	//l为1代表重复数据，不再处理
	if (w == NULL || l == 1)
	{
		return  LRESULT();
	}

	PEcnomicData data = reinterpret_cast<PEcnomicData>(w);
	
	if(dynamic_cast<CNonfarmerNumberData*>(data) 
		&& (mDataManager->mActualNonfarmerNumber == HTML_PARSE_UNKOWN || mDataManager->mActualNonfarmerNumber.IsEmpty()))
	{
		mDataManager->mActualNonfarmerNumber = data->GetActualValue();
		mDataManager->mNonfarmerNumberResult = CUtil::TranslateEcomicResult(data->GetResult());
		mDataManager->mNonfarmerNumnerWeight.Format(_T("%.4f"),data->GetWeight());
		mDataManager->mNonfarmerNumberCount.Format(_T("%u"), data->GetHttpCount());
	}
	else if (dynamic_cast<CJoblessRateData*>(data)
		&& (mDataManager->mActualJoblessRate == HTML_PARSE_UNKOWN || mDataManager->mActualJoblessRate.IsEmpty()))
	{
		mDataManager->mActualJoblessRate = data->GetActualValue();
		mDataManager->mJoblessRateResult = CUtil::TranslateEcomicResult(data->GetResult());
		mDataManager->mJoblessRateWeight.Format(_T("%.4f"),data->GetWeight());
		mDataManager->mJoblessRateCount.Format(_T("%u"), data->GetHttpCount());
	}

	UpdateData(FALSE);

	CString msg;
	msg.Format("threadid=%X,url=%s,type=%s",data, data->GetUrl(), CUtil::TranslateEcomicResult(data->GetResult()));
	TRACE(msg);
	CLogger::Add(msg);

	//这个if代表没有拉取到结果，还要继续拉取。
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
				//再次拉取发送成功。
				break;
			}
		}

		return LRESULT();
	}

	//拉取结果了，处理自动点击天通
	mActionManager->DoEcnomicDataAction(data, mDataManager, GetSafeHwnd());
	UpdateData(FALSE);
	return LRESULT();
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
		mDataManager->mOpenDirection = 0;
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
		mDataManager->mOpenDirection = 1;
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

void CTradeAssistDlg::OnBnClickedCheckSetMostTop()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_SET_MOST_TOP);
	mDataManager->mEnableWindowMostTop = pBtn->GetCheck();
	//当state == 1时表示该复选框没有被选中；
	//当state == 0时表示该复选框被选中；

	EnableWindowMostTop(mDataManager->mEnableWindowMostTop);
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