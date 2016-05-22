#include "StdAfx.h"
#include "Logger.h"

CLogger::CLogger(void)
{
}

CLogger::~CLogger(void)
{
}

CString CLogger::sContent = _T("");

int CLogger::SaveLog()
{

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString fileName;

	fileName.Format(_T(".\\log_%d_%d_%d.txt"), st.wYear, st.wMonth, st.wDay);

	CStdioFile out;
	if (out.Open(fileName, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate))
	{
		out.SeekToEnd();
		out.WriteString(CLogger::sContent);
		out.Close();
		CLogger::sContent.Empty();
	}

	return 0;
}

int CLogger::Add(CString text)
{
	
	CString time;
	SYSTEMTIME st;
	GetLocalTime(&st);
	time.Format(_T("%d:%d_%2d:%2d:%2d\t"), st.wMonth, st.wDay, st.wHour, st.wMinute,st.wSecond);

	text += _T("\r\n");
	sContent += time;
	sContent += text;
	return 0;
}
