#include "StdAfx.h"
#include "HttpWorker.h"
#include "windows.h"
CRITICAL_SECTION g_cs;
CHttpWorker::CHttpWorker(void)
{
	mConnection = NULL;
}

CHttpWorker::~CHttpWorker(void)
{
	if (mConnection != NULL)
	{
		EnterCriticalSection(&g_cs);
		mConnection->Close();
		delete mConnection;
		mConnection = NULL;
		mSession.Close();
		LeaveCriticalSection(&g_cs);
	}
}

CHttpConnection* CHttpWorker::EstablishConnection(CString host, UINT port, CString uri)
{

	mSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * 20);
	mSession.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000);
	mSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2);
	CString result;
	
	try
	{
		return mConnection = mSession.GetHttpConnection(host,  (INTERNET_PORT)port);
	}catch (CException* )
	{
		return NULL;
	}
}

CString CHttpWorker::DoGet(CString host, UINT port, CString uri)
{
	if (host.GetLength() == 0 || uri.GetLength() == 0)
	{
		return DOGET_RESULT_ERROR_INVALID_PARAM;
	}

	if (mConnection == NULL)
	{
		EstablishConnection(host, port, uri);
	}

	if (mConnection == NULL)
	{
		return DOGET_RESULT_ERROR_NOT_RETURN_OK;
	}

	CString result;
	
	try
	{
		
		CHttpFile* pFile = mConnection->OpenRequest( CHttpConnection::HTTP_VERB_GET,uri,NULL, 1, NULL, NULL,
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE );
		CString szHeaders = HTTP_HEADER;
		pFile->AddRequestHeaders(szHeaders);
		pFile->SendRequest();
		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		if(dwRet != HTTP_STATUS_OK)
		{

			result = DOGET_RESULT_ERROR_NOT_RETURN_OK;
		}
		else
		{
			CString buffer;
			while (	pFile->ReadString(buffer))
			{
				result.Append(buffer);
			}
		}

	
		pFile->Close(); 
		delete pFile;
	
	}
	catch (CMemoryException* )
	{
		result = DOGET_RESULT_ERROR_NOT_RETURN_OK;
	}
	catch (CFileException* )
	{
		result = DOGET_RESULT_ERROR_NOT_RETURN_OK;
	}
	catch (CException* )
	{
		result = DOGET_RESULT_ERROR_NOT_RETURN_OK;
	}

	return result;
}


INT CHttpWorker::GetFile(const CString& strUrl,  CString& out)
{
	//检查传入的两个参数
	if (strUrl.IsEmpty())
		return GET_FILE_RESULT_ERRROR_URL;

	unsigned short nPort;		//用于保存目标HTTP服务端口
	CString strServer, strObject;	//strServer用于保存服务器地址，strObject用于保存文件对象名称
	DWORD dwServiceType;

	//解析URL，获取信息
	if(!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
	{
		//解析失败，该Url不正确
		return GET_FILE_RESULT_ERRROR_URL;
	}
	
	out = DoGet(strServer, nPort, strObject);
	if (out == DOGET_RESULT_ERROR_NOT_RETURN_OK || out == DOGET_RESULT_ERROR_INVALID_PARAM)
	{
		return GET_FILE_RESULT_ERROR_SEND;
	}
	else
	{
		return GET_FILE_RESULT_SUCCESS;
	}
}
