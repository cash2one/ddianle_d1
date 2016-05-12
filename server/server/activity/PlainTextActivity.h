#ifndef		_PLAINTEXTACTIVITY_H___
#define		_PLAINTEXTACTIVITY_H___

#include "TimerActivity.h"

class CPlainTextActivityShow: 
	public CTimeActivity
{
public:
	CPlainTextActivityShow();
	~CPlainTextActivityShow();
public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};



#endif

