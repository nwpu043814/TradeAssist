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


//direct : 0��ʾ���߶��� 1 ��ʾ���� 2 ��ʾ����
//int CActionManager::DoHFDoubleSide(int lowDiff, int highDiff, int count, int windowDelay,  int direct) const