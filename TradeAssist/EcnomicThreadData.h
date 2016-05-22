#include "afxmt.h"
#include "EcnomicResult.h"
#pragma  once


class CEcnomicThreadData
{
private:
	CString mUrl;
	EcnomicResult mResult;
	CString mExpectValue;
	CString mActualValue;
	double mWeight;
	UINT mHttpCount;
protected:
	CCriticalSection gs;
public:
	CEcnomicThreadData & operator=(const CEcnomicThreadData& obj);
	CEcnomicThreadData();
	CEcnomicThreadData(const CEcnomicThreadData & obj);
	void SetUrl(CString url);
	CString GetUrl()const;
	CString GetExpectValue() const;
	void SetExpectValue(CString value);
	EcnomicResult GetResult()const;
	void SetResult(EcnomicResult re);
	void SetWeight(double weight);
	double GetWeight() const;
	void SetActualValue(CString value);
	CString GetActualValue() const;
	void IncreaseHttpCount();
	const UINT GetHttpCount() const;
};
typedef CEcnomicThreadData * PEcnomicThreadData;