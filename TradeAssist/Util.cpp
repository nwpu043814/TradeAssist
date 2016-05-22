#include "StdAfx.h"
#include "Util.h"

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

void CUtil::ParseDataString(IN CString text, OUT CDataPacket & result)
{
	//static int increade = 0;
	//static bool isUp = true;

	if (text.GetLength() != 0)
	{
		text = text.MakeLower();
		result.mIsGood = TRUE;

		CString value = GetValue(text, KEY_NAME_UPDROP);
		if (value.GetLength() != 0)
		{
			result.mUpDrop = atoi(value);//atoi(value); revert
		}
		else
		{
			result.mIsGood = FALSE;
		}

		value = GetValue(text, KEY_NAME_PERCENT);
		if (value.GetLength() != 0)
		{
			result.mPercent = value;
		}
		else
		{
			result.mIsGood = FALSE;
		}

		value = GetValue(text, KEY_NAME_PRICETIME);
		if (value.GetLength() != 0)
		{
			result.mPriceTime = value;
		}
		else
		{
			result.mIsGood = FALSE;
		}

		value = GetValue(text, KEY_NAME_UPDATETIME);
		if (value.GetLength() != 0)
		{
			result.mUpdateTime = value;
		}
		else
		{
			result.mIsGood = FALSE;
		}

		value = GetValue(text, KEY_NAME_NICKNAME);
		if (value.GetLength() != 0)
		{
			result.mNickName = value;
		}
		else
		{
			result.mIsGood = FALSE;
		}
		
		value = GetValue(text, KEY_NAME_NOW);
		if (value.GetLength() != 0)
		{
			/*if (isUp)
			{
				result.mPrice = (atoi(value) + increade++);

				if (increade > 40)
				{
					isUp =false;
				}			
			} 
			else
			{
				result.mPrice = (atoi(value) + increade--);				
			}*/
			result.mPrice = atoi(value);
		}
		else
		{
			result.mIsGood = FALSE;
		}
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

 int CUtil::ParseOwnServerString(CString text, CDataPacket& result)
 {
	 //5740.72:1365862696259:2013_04_13_22_18_16:average
	 if (text.GetLength() != 0)
	{
		text = text.MakeLower();
		result.mIsGood = TRUE;
		

		int colPos = text.Find(OWN_SERVER_SPLITER);
		if (colPos != -1)
		{
			result.mPrice = atoi(text.Left(colPos));
			text = text.Right(text.GetLength() - colPos - 1);
		} 
		else
		{
			result.mIsGood = FALSE;
		}

		colPos = text.Find(OWN_SERVER_SPLITER);
		if (colPos != -1)
		{
			//result.mMillionSecond = atol(text.Left(colPos));
			text = text.Right(text.GetLength() - colPos - 1);
		} 
		else
		{
			result.mIsGood = FALSE;
		}

		colPos = text.Find(OWN_SERVER_SPLITER);
		if (colPos != -1)
		{
			result.mPriceTime = text.Left(colPos);
			result.mQueryPriceUseTime= atoi(text.Right(text.GetLength() - colPos - 1));
		} 
		else
		{
			result.mIsGood = FALSE;
		}
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

 ULONGLONG CUtil::CompareSYSTEMTIME(SYSTEMTIME left, SYSTEMTIME right)
 {
	 return ULONGLONG();
 }
