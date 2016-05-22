#include "stdafx.h"
#include "EcnomicPair.h"


CString CEcnomicPair::GetUrl()  const
{
	return mThreadData->GetUrl();
}

void CEcnomicPair::SetThreadHandle(HANDLE handle)
{
	mThreadHandle = handle;
}

HANDLE CEcnomicPair::GetThreadHandle() const
{
	return mThreadHandle;
}

void CEcnomicPair::SetThreadId(UINT id)
{

	mThreadId = id;
}

int CEcnomicPair::GetThreadId() const
{

	return mThreadId;
}

void CEcnomicPair::SetUrl(CString url)
{
	mThreadData->SetUrl(url);
}


CString CEcnomicPair::GetExpectValue()  const
{
	return mThreadData->GetExpectValue();
}

void CEcnomicPair::SetExpectValue(CString value)
{
	mThreadData->SetExpectValue(value);
}

EcnomicResult CEcnomicPair::GetResult()  const
{
	return mThreadData->GetResult();
}

void CEcnomicPair::SetResult(EcnomicResult re)
{
	mThreadData->SetResult(re);
}


void CEcnomicPair::SetStartEvent(HANDLE startEvent)
{
	mStartEvent = startEvent;
}

HANDLE CEcnomicPair::GetStartEvent() const
{
	return mStartEvent;
}

void CEcnomicPair::SetMsgType(int type)
{
	mMSgType = type;
}

int	 CEcnomicPair::GetMsgType()const
{
	return mMSgType;
}

void CEcnomicPair::SetWeight(double weight)
{
	mThreadData->SetWeight(weight);
}
double CEcnomicPair::GetWeight()  const
{
	return mThreadData->GetWeight();
}

void CEcnomicPair::SetActualValue(CString value)
{
	mThreadData->SetActualValue(value);
}
CString CEcnomicPair::GetActualValue() const
{
	return mThreadData->GetActualValue();
}
void CEcnomicPair::IncreaseHttpCount()
{
	mThreadData->IncreaseHttpCount();
}

UINT CEcnomicPair::GetHttpCount() const
{
	return mThreadData->GetHttpCount();
}

CEcnomicPair::CEcnomicPair()
{
	mThreadData = new CEcnomicThreadData();
	mThreadId = 0;
	mStartEvent =NULL; 
	mThreadHandle = NULL;
	mMSgType = 0;
}

CEcnomicPair::~CEcnomicPair()
{

}


CEcnomicPair::CEcnomicPair(const CEcnomicPair & obj)
{
	mThreadData = obj.mThreadData;

	mThreadId = obj.GetThreadId();
	mStartEvent = obj.GetStartEvent(); 
	mThreadHandle = obj.GetThreadHandle();
	mMSgType = obj.GetMsgType();
}

PEcnomicThreadData CEcnomicPair::GetThreadData()
{
	return mThreadData;
}

BOOL CEcnomicPair::IsEnable()
{
	return !mThreadData->GetUrl().IsEmpty();
}