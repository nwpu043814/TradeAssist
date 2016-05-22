#include "stdafx.h"
#include "HttpWorker.h"
#include "TradeAssistDlg.h"
#include "DataPacket.h"
#include "Util.h"


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
					CString host ;
					int port;
					CString uri;
					if (dlg->IsConnectOwnServer())
					{
						host = dlg->GetServerHost();
						port = dlg->GetServerPort();
						uri = HTTP_URL_LOCAL_DATA;
					} 
					else
					{
						host =HTTP_URL_TIANTONG_HOST;
						port = 80;
						uri =  HTTP_URL_TIANTONG_URI;
					}

					CString result = httpWorker.DoGet(host, port , uri);
					CDataPacket packet;

					if (dlg->IsConnectOwnServer())
					{
						CUtil::ParseOwnServerString(result,packet);
					} 
					else
					{
						CUtil::ParseDataString(result,packet);
					}

					TRACE("result=%s\r\n", result);

					if(dlg->GetDoHttpInterval() > 0)
					{
						Sleep(dlg->GetDoHttpInterval());
					}
					
					dlg->SendMessage(WM_HTTP_GET_FINISH,(WPARAM)(&packet),(LPARAM)data);
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