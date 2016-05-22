#include "StdAfx.h"
#include "Util.h"
#include "json/json.h"

#ifdef _DEBUG
	#pragma comment(lib,"json/json_vc71_libmtd.lib")
#else
	#pragma comment(lib,"json/json_vc71_libmt.lib")
#endif

CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}

//将形如 “。。。。 23:07:11”的时间转化为秒并返回
ULONGLONG CUtil::Time2Seconds(IN CString time)
{
	ULONGLONG result = 0;
	const int timeLength = 8;
	if (time.GetLength() > timeLength)
	{
		CString rightStr = time.Trim().Right(timeLength).Trim();
	
		int hour = atoi(rightStr.Left(2));
		int minutes = atoi(rightStr.Mid(3,2));
		int seconds = atoi(rightStr.Right(2));

		result = (hour * 60 + minutes) * 60 + seconds;
	}

	return result;
}


//解析baidao的天通银数据，text内容类似：[{"sid":"TPME.XAGUSD","percent":"-0.29%","updatetime":"20150312 09:18:23","nickname":"XAGUSD","buy":3068,"status":1,"updrop":-9,"close":3060,"id":1,"open":3070,"sell":3060,"now":3060,"pricetime":"20150312 09:18:23","name":"现货白银","high":3079,"low":3057,"preclose":3069,"avg":3071,"amplitude":"0.72%"}]
void CUtil::ParseBaidaoApiDataString(IN CString text, OUT CDataPacket & result)
{

	if (text.GetLength() != 0)
	{
		result.mIsGood = TRUE;
		Json::Reader reader;
		Json::Value root;

		//去除前后的括号
		text.Remove('[');
		text.Remove(']');

		if(!reader.parse(text.GetBuffer(0),root))
		{
			printf("failed to parse!\n");
			result.mIsGood = FALSE;
		}

		result.mPrice = root[BAIDAO_KEY_CURRENT_PRICE].asDouble();
		result.mUpDrop = root[BAIDAO_KEY_NAME_UPDROP].asInt();
		result.mPriceTime = root[BAIDAO_KEY_PRICE_TIME].asString().c_str();
		result.mNickName =  root[BAIDAO_KEY_NAME_NICKNAME].asString().c_str() ;
		result.mPercent =  root[BAIDAO_KEY_NAME_PERCENT].asString().c_str() ;
		result.mUpdateTime =  root[BAIDAO_KEY_NAME_UPDATETIME].asString().c_str() ;
	}
	else
	{
		result.mIsGood = FALSE;
	}
}


CString CUtil::GetValue(IN CString text, IN CString key)
{
	CString result = _T("");
	CString keyName = key;
	int updropPos = text.Find(keyName);
	if (updropPos != -1)
	{
		int valueStart = updropPos + keyName.GetLength() + KEY_VALUE_DISTANCE;
		int valueCount = text.Find(VALUE_TAIL,valueStart) - valueStart;
		result = text.Mid(valueStart, valueCount);
	}

	return result;
}

 INT	CUtil::GetTimeSpan(IN CString left, IN CString right)
{
	CString leftDay =left.Left(left.Find(_T(" "))); 
	CString rightDay = right.Left(right.Find(_T(" ")));

	int dayDiff = (atoi(leftDay.Left(4)) - atoi(rightDay.Left(4))) * 12 * 30 + (atoi(leftDay.Mid(4,2)) - atoi(rightDay.Mid(4,2))) *30 
		+ (atoi(leftDay.Right(2)) - atoi(rightDay.Right(2)));

	const int timeLength = 8;

	int secondsDiff = (atoi( left.Right(timeLength).Left(2)) - atoi( right.Right(timeLength).Left(2)))* 60 *60
			+ (atoi(left.Right(timeLength).Mid(3,2)) - atoi(right.Right(timeLength).Mid(3,2))) * 60 
			+ (atoi(left.Right(timeLength).Right(2)) - atoi(right.Right(timeLength).Right(2)));

	secondsDiff += dayDiff*24*3600;

	return secondsDiff ;
 }

 int CUtil::ParseLocalPriceDataString(CString text, CDataPacket& result)
 {
	 //5740.72:1365862696259:2013_04_13_22_18_16:average
	 if (text.GetLength() != 0)
	{
		result.mIsGood = TRUE;
		Json::Reader reader;
	    Json::Value root;

	    if(!reader.parse(text.GetBuffer(0),root))
		{
			printf("failed to parse!\n");
			result.mIsGood = FALSE;
			return 0;
		}//result={"TIME":"2013_07_29_15_29_58","PRICE":3996,"QUERY_TIME":"0.00","MILLION_SECOND":1375111798656,"CAPTURE_TIME":"0.0"}

		result.mPrice = root[NETWORK_KEY_PRICE].asDouble();
		result.mPriceTime = root[NETWORK_KEY_TIME].asString().c_str();
		result.mQueryPriceUseTime= atof(root[NETWORK_KEY_QUERY_TIME].asString().c_str());
		result.mCapturePriceUseTime =atof(root[NETWORK_KEY_CAPTURE_TIME].asString().c_str());
		result.mChaseDirect = root[NETWORK_KEY_CHASE_DIRECT].asInt();
	}
	else
	{
		result.mIsGood = FALSE;
	}
	 return 0;
 }

 CString CUtil::TranslateEcomicResult(EcnomicResult arg)
 {
	 switch (arg)
	 {
	 case 0:
			return ECNOMIC_RESULT_TYPE_HIGH;
	 case 1:
		 return ECNOMIC_RESULT_TYPE_LOW;
	 case 2:
		 return ECNOMIC_RESULT_TYPE_EQUAL;
	 case 3:
		return ECNOMIC_RESULT_TYPE_UNKNOWN;	
	 }
	 return CString();
 }

 CString CUtil::extractExpectValue(CString &expect)
 {
	 if (expect.IsEmpty())
	 {
		return expect;
	 }

	 expect.Remove(' ');
	 expect.Remove('\r');
	 expect.Remove('\n');
	 expect.Remove('%');
	 expect.Remove('+');
	 return expect;
 }
