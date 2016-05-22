#include "stdafx.h"
#include "EcnomicThreadData.h"
void CEcnomicThreadData::SetUrl(CString url)
{
	mUrl=url;
}
CString CEcnomicThreadData::GetUrl()const
{
	return mUrl;
}

CString CEcnomicThreadData::GetExpectValue() const
{
	return mExpectValue;
}

void CEcnomicThreadData::SetExpectValue(CString value)
{
	gs.Lock();
	mExpectValue=value;
	gs.Unlock();
}
EcnomicResult CEcnomicThreadData::GetResult() const
{
	return mResult;
}
void CEcnomicThreadData::SetResult(EcnomicResult re)
{
	gs.Lock();
	mResult =re;
	gs.Unlock();
}
void CEcnomicThreadData::SetWeight(double weight)
{
	gs.Lock();
	mWeight = weight;
	gs.Unlock();
}
double CEcnomicThreadData::GetWeight() const
{
	return mWeight;
}
void CEcnomicThreadData::SetActualValue(CString value)
{
	//gs.Lock();
	mActualValue = value;
	//gs.Unlock();
}
CString CEcnomicThreadData::GetActualValue() const
{
	return mActualValue;
}
void CEcnomicThreadData::IncreaseHttpCount()
{
	//gs.Lock();
	mHttpCount++;
	//gs.Unlock();
}

const UINT CEcnomicThreadData::GetHttpCount() const
{
	return mHttpCount;
}

CEcnomicThreadData & CEcnomicThreadData::operator=(const CEcnomicThreadData& obj)
{

	if (this == &obj)
	{
		return *this;
	}

	mActualValue = obj.GetActualValue();
	mExpectValue = obj.GetExpectValue();
	mHttpCount = obj.GetHttpCount();
	mResult = obj.GetResult();
	mUrl = obj.GetUrl();
	mWeight = obj.GetWeight();

	return *this;
}
CEcnomicThreadData::CEcnomicThreadData()
{
	mActualValue = _T("");
	mExpectValue = _T("");
	mHttpCount = 0;
	mResult =  EcnomicResult::TYPE_UNKOWN;
	mWeight = 0.0F;
	mUrl = _T("");
}

CEcnomicThreadData::CEcnomicThreadData(const CEcnomicThreadData & obj)
{
	*this = obj;
}