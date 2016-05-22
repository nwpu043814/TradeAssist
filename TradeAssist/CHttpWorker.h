#pragma once

#define DOGET_RESULT_ERROR_INVALID_PARAM _T("INVALID_PARAM")
#define DOGET_RESULT_ERROR_NOT_RETURN_OK _T("SERVER_NOT_RETURN_OK")

#define RECEIVE_BUFFER_SIZE 2048

#define HTTP_HEADER _T("Accept: audio/x-aiff, audio/basic, audio/midi,\
						audio/mpeg, audio/wav, image/jpeg, image/gif, image/jpg, image/png,\
						image/mng, image/bmp, text/plain, text/html, text/htm\r\n")

class CHttpWorker
{
public:
	CHttpWorker(void);
	virtual ~CHttpWorker(void);
	CString DoGet(CString host,UINT port, CString uri);
};

typedef CHttpWorker * CHttpWorkerP;
