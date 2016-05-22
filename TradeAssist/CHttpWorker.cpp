#include "StdAfx.h"
#include "HttpWorker.h"
#include "windows.h"
CRITICAL_SECTION g_cs;
CHttpWorker::CHttpWorker(void)
{
	mConnection = NULL;
	pFile = NULL;
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

	if (pFile != NULL)
	{

		pFile->Close(); 
		pFile->EndRequest();
		delete pFile;
		pFile = NULL;
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

void CHttpWorker::DoGet(const CString & host, UINT port,const CString & uri, CString & resultString)
{
	if (host.GetLength() == 0 || uri.GetLength() == 0)
	{
		resultString = DOGET_RESULT_ERROR_INVALID_PARAM;
		return ;
	}

	if (mConnection == NULL)
	{
		EstablishConnection(host, port, uri);
	}

	if (mConnection == NULL)
	{
		resultString =  DOGET_RESULT_ERROR_NOT_RETURN_OK;
		return;
	}

	resultString = "";
	
	try
	{

		if (pFile == NULL)
		{
			pFile = mConnection->OpenRequest( CHttpConnection::HTTP_VERB_GET,uri,NULL, 1, NULL, NULL,
				INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE );
			CString szHeaders = HTTP_HEADER;
			pFile->AddRequestHeaders(szHeaders);
		}
		
		pFile->SendRequest();
		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		if(dwRet != HTTP_STATUS_OK)
		{
			resultString = DOGET_RESULT_ERROR_NOT_RETURN_OK;
		}
		else
		{
			CString buffer;
			while (	pFile->ReadString(buffer))
			{
				resultString.Append(buffer);
			}
		}
	}
	catch (CMemoryException* )
	{
		resultString = DOGET_RESULT_ERROR_NOT_RETURN_OK;
	}
	catch (CFileException* )
	{
		resultString = DOGET_RESULT_ERROR_NOT_RETURN_OK;
	}
	catch (CException* )
	{
		resultString = DOGET_RESULT_ERROR_NOT_RETURN_OK;
	}
}


INT CHttpWorker::GetFile(const CString& strUrl,  CString& out)
{
	//��鴫�����������
	if (strUrl.IsEmpty())
		return GET_FILE_RESULT_ERRROR_URL;

	unsigned short nPort;		//���ڱ���Ŀ��HTTP����˿�
	CString strServer, strObject;	//strServer���ڱ����������ַ��strObject���ڱ����ļ���������
	DWORD dwServiceType;

	//����URL����ȡ��Ϣ
	if(!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))
	{
		//����ʧ�ܣ���Url����ȷ
		return GET_FILE_RESULT_ERRROR_URL;
	}
	
	DoGet(strServer, nPort, strObject, out);
	if (out == DOGET_RESULT_ERROR_NOT_RETURN_OK || out == DOGET_RESULT_ERROR_INVALID_PARAM)
	{
		return GET_FILE_RESULT_ERROR_SEND;
	}
	else
	{
		return GET_FILE_RESULT_SUCCESS;
	}
}
