#include "stdafx.h"
#include "dalyFx.h"
#include "Constant.h"
#include "Logger.h"
#include <cmath>

CDailyFx::CDailyFx(const CEcnomicData& nonfarmer):CEcnomicPair(nonfarmer)
{
	mIsPositive = true;
}

CDailyFx::CDailyFx():CEcnomicPair()
{
	mIsPositive = true;
}

EcnomicResult CDailyFx::DoAnalyse(const CString & rawStr)
{
	CString buffer = rawStr;
	if (buffer.IsEmpty())
	{
		return EcnomicResult::TYPE_UNKOWN;
	}
	ConvertGBKToUtf8(buffer);

	CString preString=GetTitle();
	CStringW prStringW(preString);
	CStringW bufferW(buffer);
	CString nextString=HTML_PARSE_NEXT_STRING;
	CStringW nextStringW(nextString);
	CString expectNumber=GetExpectValue();
	int start;
	CString target;

	start = bufferW.Find(prStringW);
	if (start == -1)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	const int tdNumber = 5;
	int beginPos = 0;
	int endPos = 0;
	start += prStringW.GetLength(); 
	for (int tagTdCount = 0 ; tagTdCount < tdNumber; tagTdCount++)
	{
		start = bufferW.Find(nextStringW, start + nextStringW.GetLength());

		if (tagTdCount == 2)
		{
			beginPos = start;
		}
		else if (tagTdCount == 3)
		{
			endPos = start;
		}
	}

	if (endPos <= 0 || beginPos <= 0 ||  endPos - beginPos > 60)
	{
		CStringW text = bufferW.Mid(beginPos + nextStringW.GetLength(), endPos - beginPos - nextStringW.GetLength());
		CLogger::Add(CString(text));
		return EcnomicResult::TYPE_UNKOWN;
	}

	target = bufferW.Mid(beginPos + nextStringW.GetLength(), endPos - beginPos - nextStringW.GetLength());
	beginPos = target.ReverseFind('>');
	TRACE("target=%s\r\n", target);
	
	if (beginPos == -1)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	target = target.Right(target.GetLength() - beginPos - 1);
	TRACE("target=%s\r\n", target);
	CLogger::Add(target);
	CString mode = _T("Íò");
	CStringW excludeStr(mode);
	target.Remove(' ');
	target.Remove('\r');
	target.Remove('\n');
	target.Remove('%');
	target.Remove('+');

	int pos = target.Find(mode);
	if(pos != -1)
	{
		target = target.Left(pos) + target.Right(target.GetLength() - 1 - pos - excludeStr.GetLength());
	}

	if (containsChinese(target))
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	expectNumber.Remove(' ');
	expectNumber.Remove('\r');
	expectNumber.Remove('\n');
	expectNumber.Remove('+');
	expectNumber.Remove('%');
	
	mThreadData->SetActualValue(target);
	double actualData = atof(target);
	double expectData = atof(expectNumber);
	double diff = fabs(actualData - expectData);

	mThreadData->SetWeight(diff/expectData);

	if (actualData < expectData)
	{
		return  GetIspositive()? EcnomicResult::TYPE_HIGH: EcnomicResult::TYPE_LOW;
	}
	else if (actualData > expectData)
	{
		return GetIspositive()?  EcnomicResult::TYPE_LOW:EcnomicResult::TYPE_HIGH;
	}
	else
	{
		return EcnomicResult::TYPE_EQUAL;
	}
}

void CDailyFx::SetIspositive( bool isPositive)
{
	mIsPositive = isPositive;
}

bool CDailyFx::GetIspositive()
{
	return mIsPositive;
}

CString CDailyFx::GetTitle()
{
	return mTitle;
}

void CDailyFx::SetTitle(CString title)
{	
	mTitle = title;
}

CString CDailyFx::GetUrl() const
{
	return mUrl;
}
void CDailyFx::SetUrl(CString url)
{
	mUrl = url;
}

wchar_t* CDailyFx::BIG5ToUnicode(const char* szBIG5String)
{
	UINT nCodePage = 950; //BIG5
	int nLength=MultiByteToWideChar(nCodePage,0,szBIG5String,-1,NULL,0);
	wchar_t* pBuffer = new wchar_t[nLength+1];
	MultiByteToWideChar(nCodePage,0,szBIG5String,-1,pBuffer,nLength);
	pBuffer[nLength]=0;
	return pBuffer;
}
//Unicode ×ª»»³É GB2312£º
char* CDailyFx::UnicodeToGB2312(const wchar_t* szUnicodeString)
{
	UINT nCodePage = 936; //GB2312
	int nLength=WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,NULL,0,NULL,NULL);
	char* pBuffer=new char[nLength+1];
	WideCharToMultiByte(nCodePage,0,szUnicodeString,-1,pBuffer,nLength,NULL,NULL);
	pBuffer[nLength]=0;
	return pBuffer;
}


void CDailyFx::ConvertGBKToUtf8(CString& strGBK)
{
	//USES_CONVERSION; 
	//LPCWSTR pwcStr = A2CW((LPCSTR)strGBK); 
	CString result;
	ConvUtf8ToAnsi(strGBK ,result);
	strGBK = result;
}

int CDailyFx::ConvUtf8ToAnsi(CString& strSource, CString& strChAnsi)   
{      
	if (strSource.GetLength() <= 0)   
		return 0;   

	CString strWChUnicode;   

	strSource.TrimLeft();   
	strSource.TrimRight();       
	strChAnsi.Empty();   

	int iLenByWChNeed = MultiByteToWideChar(CP_UTF8, 0,    
		strSource.GetBuffer(0),    
		strSource.GetLength(),   
		NULL, 0);   

	int iLenByWchDone = MultiByteToWideChar(CP_UTF8, 0,    
		strSource.GetBuffer(0),    
		strSource.GetLength(),    
		(LPWSTR)strWChUnicode.GetBuffer(iLenByWChNeed * 2),    
		iLenByWChNeed); //MultiByteToWideChar    

	strWChUnicode.ReleaseBuffer(iLenByWchDone * 2);   

	int iLenByChNeed  = WideCharToMultiByte(CP_ACP, 0,    
		(LPCWSTR)strWChUnicode.GetBuffer(0),   
		iLenByWchDone,   
		NULL, 0,    
		NULL, NULL);     

	int iLenByChDone  = WideCharToMultiByte(CP_ACP, 0,    
		(LPCWSTR)strWChUnicode.GetBuffer(0),    
		iLenByWchDone,   
		strChAnsi.GetBuffer(iLenByChNeed),    
		iLenByChNeed,    
		NULL, NULL);   

	strChAnsi.ReleaseBuffer(iLenByChDone);   

	if (iLenByWChNeed != iLenByWchDone || iLenByChNeed != iLenByChDone)   
		return 1;   

	return 0;       
}  

BOOL CDailyFx::IsEnable()
{
	return ! GetTitle().IsEmpty();
}

bool CDailyFx::containsChinese(const CString& text)
{
	int len = text.GetLength();
	for (int i = 0;i < len; i++)
	{
		if (text.GetAt(i) <0 )
		{
			return true;
		}
	}
	return false;
}
