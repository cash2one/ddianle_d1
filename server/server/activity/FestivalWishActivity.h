#ifndef		__FESTIVALWISHACTIVITY_H__
#define		__FESTIVALWISHACTIVITY_H__

#include "TimerActivity.h"

class CFestivalWishActivity: 
	public CTimeActivity
{
public:
	CFestivalWishActivity();
	~CFestivalWishActivity();
public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};




#endif





