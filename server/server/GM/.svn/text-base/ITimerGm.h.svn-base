#ifndef		__GM_ITIMER_H__
#define		__GM_ITIMER_H__

#include "GMDefine.h"


class ITimerGm
{
public:
	ITimerGm() {};
	virtual ~ITimerGm() {};
public:
	virtual void OnEvent() = 0;
	virtual time_t TriggerTime() = 0;
	virtual ETimerType TimerType() = 0;
	virtual unsigned int TimerID() = 0;
};


#endif



