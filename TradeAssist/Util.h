#pragma once
#include "DataPacket.h"
#include "EcnomicCompareResult.h"
class CUtil
{
private:
	CUtil(void);
	~CUtil(void);
public:
	static void ParseDataString(IN CString text, OUT CDataPacket & result);
private:
	static CString GetValue(IN CString text, IN CString key);

public:
	static INT	GetTimeSpan(IN CString left, IN CString right);

	static ULONGLONG Time2Seconds(IN CString time);
	static int ParseOwnServerString(CString text, CDataPacket& result);
	static CString TranslateEcomicResult(EcnomicResult arg);
	static CString extractExpectValue(CString& s);
	static ULONGLONG CompareSYSTEMTIME(SYSTEMTIME left, SYSTEMTIME right);
};
