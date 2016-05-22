#include "afxinet.h"
#pragma once

#define DOGET_RESULT_ERROR_INVALID_PARAM _T("INVALID_PARAM")
#define DOGET_RESULT_ERROR_NOT_RETURN_OK _T("SERVER_NOT_RETURN_OK")

#define RECEIVE_BUFFER_SIZE 2048
#define  GET_FILE_RESULT_SUCCESS 0
#define  GET_FILE_RESULT_ERRROR_URL 1
#define  GET_FILE_RESULT_ERROR_NETWORK 2
#define  GET_FILE_RESULT_ERROR_SEND 3
#define  GET_FILE_RESULT_ERROR_SERVER_RESPONSE 4
#define  GET_FILE_RESULT_ERROR_FILE 5

#define HTTP_HEADER _T("Accept:*/*\r\naccept_language:zh-cn\r\nuser_agent:Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; QQDownload 734; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)\r\naccept_encoding:gzip, deflate\r\nconnection:Keep-Alive")

class CHttpWorker
{
private:
	CInternetSession mSession;
	CHttpConnection* mConnection;
public:
	CHttpWorker(void);
	virtual ~CHttpWorker(void);
	CString DoGet(CString host,UINT port, CString uri);
	int GetFile(const CString& url, CString& out);
	CHttpConnection* EstablishConnection(CString host, UINT port, CString uri);
};

typedef CHttpWorker * CHttpWorkerP;
