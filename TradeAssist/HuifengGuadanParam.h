#pragma  once

struct CHuifengGuadanParam
{
	int mLowDiff;
	int mHighDiff;
	int mTradeCount;
	int mWindowDelay;
	int mDirect;
};

typedef CHuifengGuadanParam * CHuifengGuadanParamP;


//direct : 0表示两边都做 1 表示做空 2 表示做多
//int CActionManager::DoHFDoubleSide(int lowDiff, int highDiff, int count, int windowDelay,  int direct) const