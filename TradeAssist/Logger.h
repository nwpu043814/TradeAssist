#pragma once

class Logger
{
public:
	Logger(void);
	~Logger(void);
	static int SaveLog(CString fileName);
public:
	static CString sContent;

public:
	static int Add(CString text);
};
