#ifndef		__EXCHANGEITEMACTIVITYCTIVITY_H__
#define		__EXCHANGEITEMACTIVITYCTIVITY_H__

#include "TimerActivity.h"

class CExchangeItemActivity: 
	public CTimeActivity
{
public:
	CExchangeItemActivity();
	~CExchangeItemActivity();
public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};



#endif


