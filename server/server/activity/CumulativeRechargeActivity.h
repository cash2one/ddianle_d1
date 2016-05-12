#ifndef		__CUMULATIVERECHARGEACTIVITY_H__
#define		__CUMULATIVERECHARGEACTIVITY_H__

#include "TimerActivity.h"

class CCummulativeRechargeActivity: 
	public CTimeActivity
{
public:
	CCummulativeRechargeActivity();
	~CCummulativeRechargeActivity();
public:

public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};



#endif


