#pragma once
#include "EcnomicResult.h"
#include "EcnomicThreadData.h"
#pragma warning(disable : 4482)

typedef class CEcnomicPair
{
private:

	UINT	mThreadId;
	HANDLE mStartEvent;
	HANDLE mThreadHandle;
	int 	mMSgType;
protected:
	PEcnomicThreadData mThreadData; 
public :

	CEcnomicPair();
	virtual ~CEcnomicPair();
	CEcnomicPair(const CEcnomicPair &);
	virtual EcnomicResult  DoAnalyse(const CString & text)=0;
	virtual  CString GetUrl() const;
	void SetThreadId(UINT id);
	int GetThreadId() const;
	virtual void SetUrl(CString url);
	CString GetExpectValue() const;
	void SetExpectValue(CString value);
	EcnomicResult GetResult() const;
	void SetResult(EcnomicResult re);
	void SetStartEvent(HANDLE startEvent);
	HANDLE GetStartEvent() const;
	void SetThreadHandle(HANDLE handle);
	HANDLE GetThreadHandle()const;
	void SetMsgType(int msg);
	int	 GetMsgType()const;
	void SetWeight(double weight);
	double GetWeight() const;
	void SetActualValue(CString value);
	CString GetActualValue() const;
	void IncreaseHttpCount();
	UINT GetHttpCount() const;
	PEcnomicThreadData GetThreadData();
	virtual BOOL IsEnable();
} CEcnomicData, * PEcnomicData;
