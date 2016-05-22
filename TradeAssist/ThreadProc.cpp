#include "stdafx.h"
#include "HttpWorker.h"
#include "TradeAssistDlg.h"
#include "DataPacket.h"
#include "Util.h"
#include "logger.h"


UINT	__stdcall HttpProcess(void * param)
{
	if (param == NULL)
	{
		return 103;
	}

	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	PEcnomicData data = (PEcnomicData) param;

	if(!SetEvent(data->GetStartEvent())) 
	{
		TRACE("set start event failed,errno:%d\r\n",::GetLastError());
		return 104;
	}

	CHttpWorker httpWorker;
	CTradeAssistDlg* dlg = (CTradeAssistDlg*)(AfxGetApp()->GetMainWnd());
	bool	cycle = true;
	while(cycle)
	{
		if(GetMessage(&msg,0,0,0))
		{

			switch(msg.message)
			{
			case WM_DO_HTTP_GET_PRICE:
				{
					CString host = dlg->GetServerHost();
					int port = dlg->GetServerPort();
					CString uri = HTTP_URL_LOCAL_DATA;

					for (int i = 0; i < DO_PULL_PRICE_TIMES_PER_MSG; i++)
					{
						CString result = httpWorker.DoGet(host, port , uri);
						CDataPacket packet;				
						CUtil::ParseOwnServerString(result,packet);
						TRACE("result=%s\r\n", result);

						if(result.Find("average") != -1)
						{
							CLogger::Add(result);
						}

						if(dlg->GetDoHttpInterval() > 0)
						{
							Sleep(dlg->GetDoHttpInterval());
						}

						dlg->SendMessage(WM_HTTP_GET_FINISH,(WPARAM)(&packet),(LPARAM)data);
					}
					
					break;
				}
			case WM_DO_HTTP_EXIT:
				{
					cycle = false;
					return 102;
				}
			case WM_DO_HTTP_GET_ECNOMIC_DATA:
				{

					if (msg.wParam == NULL)
					{
						break;
					}

					CString out;
					if (!data->GetUrl().IsEmpty() && httpWorker.GetFile(data->GetUrl(), out) == GET_FILE_RESULT_SUCCESS)
					{
						data->IncreaseHttpCount();
						EcnomicResult result = data->DoAnalyse(out);
						if (result != EcnomicResult::TYPE_UNKOWN)
						{
							//本次拉取有效
							if (data->GetResult() != EcnomicResult::TYPE_UNKOWN)
							{
								//已经有结论，抛弃后来着。
								dlg->SendMessage(WM_HTTP_GET_ECNOMIC_DATA_FINISH,(WPARAM)data,(LPARAM)1);
							} 
							else
							{
								//首次更新。
								data->SetResult(result);
								dlg->SendMessage(WM_HTTP_GET_ECNOMIC_DATA_FINISH,(WPARAM)data,NULL);
							}
						}
						else
						{
							//本次拉取无效
							dlg->SendMessage(WM_HTTP_GET_ECNOMIC_DATA_FINISH,(WPARAM)data,NULL);
						}
					}
					else
					{
						dlg->SendMessage(WM_HTTP_GET_ECNOMIC_DATA_FINISH,(WPARAM)data,NULL);
					}

					break;
				}
			}
		}
	}

	return 100;
}