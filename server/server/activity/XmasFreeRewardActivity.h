#ifndef		__XMASFREEREWARDACTIVITY_H__
#define		__XMASFREEREWARDACTIVITY_H__

#include "TimerActivity.h"

class CXmasFreeRewardActivity: 
	public CTimeActivity
{
public:
	CXmasFreeRewardActivity();
	~CXmasFreeRewardActivity();
public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};




#endif





