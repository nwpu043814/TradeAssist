#include "Constant.h"
#include <cmath>
#include "EcnomicThreadData.h"
#include "EcnomicPair.h"
#pragma warning(disable : 4482)
#pragma  once

class CNonfarmerNumberData:public CEcnomicData
{
public:
	 EcnomicResult  DoAnalyse(const CString & rawStr);
	 CNonfarmerNumberData();
	 CNonfarmerNumberData(const CEcnomicData& nonfarmer);
} ;

class CJoblessRateData:public CEcnomicData
{
public:
	EcnomicResult  DoAnalyse(const CString & text);
	CJoblessRateData();
	CJoblessRateData(const CEcnomicData & jobless);
};

typedef CNonfarmerNumberData * PNonfarmerNumberData;
typedef CJoblessRateData * PJoblessRateData;

class CLocalServerData:public CEcnomicData
{
public:
	EcnomicResult  DoAnalyse(const CString & text);
	CLocalServerData();
};

typedef CLocalServerData * PLocalServerData;


