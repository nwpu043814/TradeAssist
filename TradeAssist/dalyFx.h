#pragma  once
#include "EcnomicPair.h"


class CDailyFx:public CEcnomicData
{
private:
	bool	mIsPositive;
	CString	mTitle;
	CString mUrl;
public:
	EcnomicResult  DoAnalyse(const CString & text);
	CDailyFx();
	CDailyFx(const CEcnomicData & jobless);
	void SetIspositive(bool);
	bool GetIspositive();
	CString GetTitle();
	void SetTitle(CString title);
	CString GetUrl() const;
	void SetUrl(CString url);
	void ConvertGBKToUtf8(CString& strGBK);
	char* UnicodeToGB2312(const wchar_t* szUnicodeString);
	wchar_t* BIG5ToUnicode(const char* szBIG5String);
	char* GetAnsicString(const CString &s, UINT nCodePage) ;
	int ConvUtf8ToAnsi(CString& strSource, CString& strChAnsi);
	virtual BOOL IsEnable();
	bool containsChinese(const CString& text);
};
typedef CDailyFx * PDailyFx;