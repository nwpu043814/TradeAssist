#pragma once

class Logger
{
public:
	Logger(void);
	~Logger(void);
	static int SaveLog();
public:
	static CString sContent;

public:
	static int Add(CString text);
};
