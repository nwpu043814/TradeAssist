#include "StdAfx.h"
#include "LuaEngine.h"
#include "Constant.h"
#include <cmath>

#pragma comment(lib,"lua5.1.lib")


CLuaEngine::CLuaEngine(void)
: mExpectNonfarmerNumber(_T(""))
, mNonfarmerWorkerUrl(_T(""))
, mJoblessUrl(_T(""))
, mExpectJoblessRate(_T(""))
, mOriginal2DoHigh(0,0)
, mOriginal2DoLow(0,0)
, mOrigin2Entrust(0,0)
, mLowEntrust2Direction(0,0)
, mLowDirection2Price(0,0)
, mHighPrice2Count(0,0)
, mLowPrice2Count(0,0)
, mHighDirection2Price(0,0)
, mHighEntrust2Direction(0,0)
, mCount2Confirm(0,0)
, mOrigin2Remove(0,0)
, mRemove2Confirm(0,0)
, mOrigin2Count(0,0)
, mCount2OrderButtonLow(0,0)
, mCount2OrderButtonHigh(0,0)
, mNonfarmerTitle(_T(""))
, mJoblessTitle(_T(""))
, mDailyFxUrl(_T(""))
, mStartPrice(0)
, mChasePriceThreshold(0)
, mHasChased(false)
, mChaseMaxTime(0)
, mChasePriceMax(0)
{
	mLua = InitLuaState();
	GetNonfarmerWorkerUrl(0);
	GetJoblessUrl(0);
	GetOriginal2DoHigh();
	GetOriginal2DoLow();
	getRemove2Confirm();
	getOrigin2Remove();
	getCount2Confirm();
	getPrice2Count(DO_HIGH);
	getPrice2Count(DO_LOW);
	getDirection2Price(DO_LOW);
	getDirection2Price(DO_HIGH);
	getEntrust2Direction(DO_LOW);
	getEntrust2Direction(DO_HIGH);
	getOrigin2Entrust();
	GetChasePriceThreshold();
	GetChaseMaxTime();
	GetChasePriceMax();
	memset(&mStartTime, 0, sizeof(CTime));
}

CLuaEngine::~CLuaEngine(void)
{

	if (mLua != NULL)
	{
		lua_close(mLua);
	}
}



lua_State * CLuaEngine::InitLuaState(void)
{
	lua_State * lua = lua_open(); 
	if(NULL!=lua)
	{
		luaopen_base(lua);
		luaopen_table(lua);
		luaopen_string(lua);
		luaopen_math(lua);
		luaopen_debug(lua);
		luaL_dofile(lua,THRESHOLD_FILE_NAME);     // 解释分析lua文件
		lua_resume( lua, 0 );

		return lua;
	} 
	ASSERT(NULL != lua);
	return NULL;
}

int CLuaEngine::GetDynamicThreshold(int pId, BOOL enableAuto, int back, CString diff, UINT threshold)
{
	if (enableAuto)
	{
		lua_State * lua = GetLuaState(pId);
		ASSERT(lua != NULL);
		lua_getglobal(lua,LUA_FUNCTION_GetThreshold);    
		lua_pushnumber(lua,back);
		lua_pushnumber(lua,atof(diff));
		size_t size;
		if(lua_pcall(lua,2,1,0)!= 0)     
		{
			const char * str = lua_tolstring(lua, -1, &size);     
			lua_pop(lua,1);

#ifdef _DEBUG
			CString msg; 
			msg.Format(_T("%s"), str);
			AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      
		
			return 20;
		}

		int result = (int)lua_tonumber(lua, -1);   
		lua_pop(lua,1);  

		return result;
	}
	else
	{
		return threshold;
	}
}

CString CLuaEngine::GetJoblessUrl(int pId)
{
	if (!mJoblessUrl.IsEmpty())
	{
		return mJoblessUrl;
	}

	lua_State * m_plua = GetLuaState(pId);
	lua_getglobal(m_plua,LUA_FUNCTION_GetJoblessUrl);       
	size_t size;
	if(lua_pcall(m_plua,0,3,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      
		
		return _T("");
	}
	
	const char * str = lua_tolstring(m_plua, -1, &size);        
	lua_pop(m_plua,1);
	mJoblessUrl = str;

	str = lua_tolstring(m_plua, -1, &size);        
	lua_pop(m_plua,1);
	mExpectJoblessRate = str;

	str = lua_tolstring(m_plua, -1, &size);        
	lua_pop(m_plua,1);
	mJoblessTitle = str;

	return mJoblessUrl;
}

CString CLuaEngine::GetNonfarmerWorkerUrl(int pId)
{
	if (!mNonfarmerWorkerUrl.IsEmpty())
	{
		return mNonfarmerWorkerUrl;
	}

	lua_State * m_plua = GetLuaState(pId);   
	lua_getglobal(m_plua, LUA_FUNCTION_GetNonfarmerWorkerUrl);       
	size_t size;
	if(lua_pcall(m_plua,0,3,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      
	
		return "";
	}

	const char * str = lua_tolstring(m_plua, -1, &size);        
	lua_pop(m_plua,1);
	mNonfarmerWorkerUrl = str;

	str = lua_tolstring(m_plua, -1, &size);        
	lua_pop(m_plua,1);
	mExpectNonfarmerNumber = str;

	str = lua_tolstring(m_plua, -1, &size);        
	lua_pop(m_plua,1);
	mNonfarmerTitle = str;

	return mNonfarmerWorkerUrl;
}

lua_State* CLuaEngine::GetLuaState(int id)
{
	return mLua;
}

CString CLuaEngine::GetExpectValue(CString funName)
{
	if (LUA_FUNCTION_GetExpectNonfarmerWorker == funName)
	{
		return mExpectNonfarmerNumber;
		
	}
	else if (LUA_FUNCTION_GetExpectJoblessRate == funName)
	{
	
		return mExpectJoblessRate;
	}
	
	return "";
}

CPoint CLuaEngine::GetOriginal2DoHigh(void)
{

	if (mOriginal2DoHigh.x + mOriginal2DoHigh.y != 0)
	{
		return mOriginal2DoHigh;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetOriginal2DoHigh);       
	size_t size;
	if(lua_pcall(m_plua,0,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	mOriginal2DoHigh.y = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	mOriginal2DoHigh.x = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  


	return mOriginal2DoHigh;
}

CPoint CLuaEngine::GetOriginal2DoLow(void)
{
	if (mOriginal2DoLow.x + mOriginal2DoLow.y != 0)
	{
		return mOriginal2DoLow;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetOriginal2DoLow);       
	size_t size;
	if(lua_pcall(m_plua,0,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	mOriginal2DoLow.y = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	mOriginal2DoLow.x = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  


	return mOriginal2DoLow;
}


const CPoint & CLuaEngine::getOrigin2Entrust(void)
{
	if (!(mOrigin2Entrust.x == 0 || mOrigin2Entrust.y == 0))
	{
		return mOrigin2Entrust;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetOrigin2Entrust);       
	size_t size;
	if(lua_pcall(m_plua,0,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	mOrigin2Entrust.y = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	mOrigin2Entrust.x = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	return mOrigin2Entrust;		
}

const CPoint& CLuaEngine::getEntrust2Direction(int direct)
{
	if (direct == DO_LOW)
	{
		if (!(mLowEntrust2Direction.x == 0 || mLowEntrust2Direction.y == 0))
		{
			return mLowEntrust2Direction;
		}

	}
	else
	{
		if (!(mHighEntrust2Direction.x == 0 || mHighEntrust2Direction.y == 0))
		{
			return mHighEntrust2Direction;
		}
	}


	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetEntrust2Direction);    
	lua_pushnumber(m_plua,direct);
	size_t size;
	if(lua_pcall(m_plua,1,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	if (direct == DO_LOW)
	{
		mLowEntrust2Direction.y = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		mLowEntrust2Direction.x = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		return mLowEntrust2Direction;		
	} 
	else
	{
		mHighEntrust2Direction.y = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		mHighEntrust2Direction.x = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		return mHighEntrust2Direction;		
	}
}

const CPoint & CLuaEngine::getDirection2Price(int direct)
{
	if (direct == DO_LOW)
	{
		if (!(mLowDirection2Price.x == 0 || mLowDirection2Price.y == 0))
		{
			return mLowDirection2Price;
		}

	}
	else
	{
		if (!(mHighDirection2Price.x == 0 || mHighDirection2Price.y == 0))
		{
			return mHighDirection2Price;
		}
	}


	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetDirection2Price);    
	lua_pushnumber(m_plua,direct);
	size_t size;
	if(lua_pcall(m_plua,1,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	if (direct == DO_LOW)
	{
		mLowDirection2Price.y = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		mLowDirection2Price.x = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		return mLowDirection2Price;		
	} 
	else
	{
		mHighDirection2Price.y = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		mHighDirection2Price.x = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		return mHighDirection2Price;		
	}
}

const CPoint& CLuaEngine::getPrice2Count(int direct)
{
	if (direct == DO_LOW)
	{
		if (!(mLowPrice2Count.x == 0 || mLowPrice2Count.y == 0))
		{
			return mLowPrice2Count;
		}

	}
	else
	{
		if (!(mHighPrice2Count.x == 0 || mHighPrice2Count.y == 0))
		{
			return mHighPrice2Count;
		}
	}


	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetPrice2Count);    
	lua_pushnumber(m_plua,direct);
	size_t size;
	if(lua_pcall(m_plua,1,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	if (direct == DO_LOW)
	{
		mLowPrice2Count.y = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		mLowPrice2Count.x = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		return mLowPrice2Count;		
	} 
	else
	{
		mHighPrice2Count.y = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		mHighPrice2Count.x = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		return mHighPrice2Count;		
	}
}

const CPoint& CLuaEngine::getCount2Confirm(void)
{
	if (!(mCount2Confirm.x == 0 || mCount2Confirm.y == 0))
	{
		return mCount2Confirm;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetCount2Confirm);       
	size_t size;
	if(lua_pcall(m_plua,0,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	mCount2Confirm.y = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	mCount2Confirm.x = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	return mCount2Confirm;		
}

const CPoint& CLuaEngine::getOrigin2Remove(void)
{
	if (!(mOrigin2Remove.x == 0 || mOrigin2Remove.y == 0))
	{
		return mOrigin2Remove;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetOrigin2Remove);       
	size_t size;
	if(lua_pcall(m_plua,0,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	mOrigin2Remove.y = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	mOrigin2Remove.x = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	return mOrigin2Remove;	
}

const CPoint& CLuaEngine::getRemove2Confirm(void)
{
	if (!(mRemove2Confirm.x == 0 || mRemove2Confirm.y == 0))
	{
		return mRemove2Confirm;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetRemove2Confirm);       
	size_t size;
	if(lua_pcall(m_plua,0,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	mRemove2Confirm.y = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	mRemove2Confirm.x = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	return mRemove2Confirm;	
}

const CPoint& CLuaEngine::GetOrigin2Count(void)
{
	if (!(mOrigin2Count.x == 0 || mOrigin2Count.y == 0))
	{
		return mOrigin2Count;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetOrigin2Count);       
	size_t size;
	if(lua_pcall(m_plua,0,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	mOrigin2Count.y = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	mOrigin2Count.x = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1);  

	return mOrigin2Count;	
}

const CPoint& CLuaEngine::GetCount2OrderButton(UINT direct)
{
	if (direct == DO_LOW)
	{
		if (!(mCount2OrderButtonLow.x == 0 || mCount2OrderButtonLow.y == 0))
		{
			return mCount2OrderButtonLow;
		}

	}
	else
	{
		if (!(mCount2OrderButtonHigh.x == 0 || mCount2OrderButtonHigh.y == 0))
		{
			return mCount2OrderButtonHigh;
		}
	}


	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetPrice2Count);    
	lua_pushnumber(m_plua,direct);
	size_t size;
	if(lua_pcall(m_plua,1,2,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return CPoint();
	}

	if (direct == DO_LOW)
	{
		mCount2OrderButtonLow.y = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		mCount2OrderButtonLow.x = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		return mCount2OrderButtonLow;		
	} 
	else
	{
		mCount2OrderButtonHigh.y = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		mCount2OrderButtonHigh.x = (int)lua_tonumber(m_plua, -1);   
		lua_pop(m_plua,1);  

		return mCount2OrderButtonHigh;		
	}
}

CString CLuaEngine::GetDailyFxUrl(void)
{
	if (!mDailyFxUrl.IsEmpty())
	{
		return mDailyFxUrl;
	}

	lua_State * m_plua = GetLuaState(0);
	lua_getglobal(m_plua,LUA_FUNCTION_GetDailyFxUrl);       
	size_t size;
	if(lua_pcall(m_plua,0,1,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return "";
	}

	const char * str = lua_tolstring(m_plua, -1, &size);        
	lua_pop(m_plua,1);
	mDailyFxUrl = str;

	return mDailyFxUrl;
}

CString CLuaEngine::GetNonfarmerTitle(void)
{
	return mNonfarmerTitle;
}

CString CLuaEngine::GetJoblessTitle(void)
{
	return mJoblessTitle;
}

UINT CLuaEngine::GetStartPrice(void)
{
	return mStartPrice;
}

void CLuaEngine::SetStartPrice(UINT startPrice)
{
	mStartPrice = startPrice;
}

INT CLuaEngine::GetChasePriceThreshold(void)
{
	if (mChasePriceThreshold!= 0)
	{
		return mChasePriceThreshold;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetChasePriceThreshold);       
	size_t size;
	if(lua_pcall(m_plua,0,1,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return 50;
	}

	mChasePriceThreshold = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1); 

	return mChasePriceThreshold;		
}

bool CLuaEngine::GetHasChased(void)
{
	return mHasChased;
}

void CLuaEngine::SetHasChased(bool chase)
{
	mHasChased = chase;
}

CTime & CLuaEngine::GetStartTime(void)
{
	return mStartTime;
}

INT CLuaEngine::GetChaseMaxTime(void)
{
	if (mChaseMaxTime!= 0)
	{
		return mChaseMaxTime;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetChaseMaxTime);       
	size_t size;
	if(lua_pcall(m_plua,0,1,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return 50;
	}

	mChaseMaxTime = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1); 

	return mChaseMaxTime;		
}

int CLuaEngine::GetChasePriceMax(void)
{
	if (mChasePriceMax!= 0)
	{
		return mChasePriceMax;
	}

	lua_State * m_plua = GetLuaState(0);   
	lua_getglobal(m_plua,LUA_FUNCTION_GetChasePriceMax);       
	size_t size;
	if(lua_pcall(m_plua,0,1,0)!= 0)        
	{
		const char * str = lua_tolstring(m_plua, -1, &size);     
		lua_pop(m_plua,1);

#ifdef _DEBUG
		CString msg; 
		msg.Format(_T("%s"), str);
		AfxMessageBox(_T("调用lua脚本函数失败:"+msg));     
#endif // _DEBUG      

		return 20;
	}

	mChasePriceMax = (int)lua_tonumber(m_plua, -1);   
	lua_pop(m_plua,1); 

	return mChasePriceMax;		
}
