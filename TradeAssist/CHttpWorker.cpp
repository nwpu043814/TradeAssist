#include "StdAfx.h"
#include "CHttpWorker.h"
#include "afxinet.h"

CHttpWorker::CHttpWorker(void)
{
}

CHttpWorker::~CHttpWorker(void)
{
}

CString CHttpWorker::DoGet(CString host, UINT port, CString uri)
{
	if (host.GetLength() == 0 || uri.GetLength() == 0)
	{
		return DOGET_RESULT_ERROR_INVALID_PARAM;
	}

	CInternetSession session;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * 20);
	session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000);
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2);

	CHttpConnection* pConnection = session.GetHttpConnection(host,  (INTERNET_PORT)port);
	CHttpFile* pFile = pConnection->OpenRequest( CHttpConnection::HTTP_VERB_GET,uri);

	CString szHeaders = HTTP_HEADER;

	pFile->AddRequestHeaders(szHeaders);
	pFile->SendRequest();

	DWORD dwRet;
	pFile->QueryInfoStatusCode(dwRet);
	CString result;

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

	session.Close();
	pFile->Close(); 
	delete pFile;

	return result;
}
