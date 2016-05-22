#include "StdAfx.h"
#include "EcnomicCompareResult.h"


EcnomicResult CJoblessRateData::DoAnalyse(const CString & rawStr)
{
	if (rawStr.IsEmpty())
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	CString preString=HTML_PARSE_PRE_STRING;
	CString nextString=HTML_PARSE_NEXT_STRING;
	CString expectNumber=GetExpectValue();
	int start, targetStart, targetEnd;
	CString target;

	start = rawStr.Find(expectNumber);

	if (start == -1)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	targetStart = rawStr.Find(preString, start);
	if (targetStart == -1)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	targetEnd = rawStr.Find(nextString, targetStart);
	if (targetEnd == -1)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	target = rawStr.Mid(targetStart + preString.GetLength(), targetEnd -(targetStart + preString.GetLength()));
	target.Remove(' ');
	target.Remove('\r');
	target.Remove('\n');
	target.Remove('%');
	target.Remove('+');

	expectNumber.Remove(' ');
	expectNumber.Remove('\r');
	expectNumber.Remove('\n');
	expectNumber.Remove('%');
	expectNumber.Remove('+');
	mThreadData->SetActualValue(target);

	if (target == HTML_PARSE_UNKOWN)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	double actualData = atof(target);
	double expectData = atof(expectNumber);
	double diff = fabs(actualData - expectData);
	mThreadData->SetWeight(diff/expectData);
	if (actualData < expectData)
	{
		return EcnomicResult::TYPE_LOW;
	}
	else if (actualData > expectData)
	{
		return EcnomicResult::TYPE_HIGH;
	}
	else
	{
		return EcnomicResult::TYPE_EQUAL;
	}
}


CJoblessRateData::CJoblessRateData():CEcnomicPair()
{
}


CJoblessRateData::CJoblessRateData(const CEcnomicData & jobless):CEcnomicPair(jobless)
{
}

CNonfarmerNumberData::CNonfarmerNumberData(const CEcnomicData& nonfarmer):CEcnomicPair(nonfarmer)
{
}

CNonfarmerNumberData::CNonfarmerNumberData():CEcnomicPair()
{
}

EcnomicResult CNonfarmerNumberData::DoAnalyse(const CString & rawStr)
{
	if (rawStr.IsEmpty())
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	CString preString=HTML_PARSE_PRE_STRING;
	CString nextString=HTML_PARSE_NEXT_STRING;
	CString expectNumber=GetExpectValue();
	int start, targetStart, targetEnd;
	CString target;

	start = rawStr.Find(expectNumber);

	if (start == -1)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	targetStart = rawStr.Find(preString, start);
	if (targetStart == -1)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	targetEnd = rawStr.Find(nextString, targetStart);
	if (targetEnd == -1)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}

	target = rawStr.Mid(targetStart + preString.GetLength(), targetEnd -(targetStart + preString.GetLength()));
	target.Remove(' ');
	target.Remove('\r');
	target.Remove('\n');
	target.Remove('%');
	target.Remove('+');

	expectNumber.Remove(' ');
	expectNumber.Remove('\r');
	expectNumber.Remove('\n');
	expectNumber.Remove('+');
	expectNumber.Remove('%');
	mThreadData->SetActualValue(target);
	if (target == HTML_PARSE_UNKOWN)
	{
		return EcnomicResult::TYPE_UNKOWN;
	}
	
	double actualData = atof(target);
	double expectData = atof(expectNumber);
	double diff = fabs(actualData - expectData);

	mThreadData->SetWeight(diff/expectData);

	if (actualData < expectData)
	{
		return EcnomicResult::TYPE_HIGH;
	}
	else if (actualData > expectData)
	{
		return EcnomicResult::TYPE_LOW;
	}
	else
	{
		return EcnomicResult::TYPE_EQUAL;
	}
}

EcnomicResult  CLocalServerData::DoAnalyse(const CString & text)
{

	return EcnomicResult();
}

CLocalServerData::CLocalServerData():CEcnomicPair()
{
 
}
