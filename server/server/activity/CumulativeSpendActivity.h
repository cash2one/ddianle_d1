#ifndef		__CUMULATIVESPENDACTIVITY_H__
#define		__CUMULATIVESPENDACTIVITY_H__

#include "TimerActivity.h"

class CCummulativeSpendActivity: 
	public CTimeActivity
{
public:
	CCummulativeSpendActivity();
	~CCummulativeSpendActivity();
public:

public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};



#endif


