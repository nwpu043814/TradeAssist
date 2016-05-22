#pragma once

class CLogger
{
public:
	CLogger(void);
	~CLogger(void);
	static int SaveLog();
public:
	static CString sContent;

public:
	static int Add(CString text);
};
