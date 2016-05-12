#ifndef		__LIBSERVERFRAME_ITIMER_H__
#define		__LIBSERVERFRAME_ITIMER_H__

#include <time.h>

#ifndef WIN32
typedef long long         __int64;
#endif
typedef __int64 timerid_t;

class ITimer
{
public:
	ITimer() {m_nTriggerTime = 0; m_nTimerType = 0; m_nTimerID = 0;};
	virtual ~ITimer() {};
public:
	virtual void OnEvent() = 0;
public:
	time_t TriggerTime(){return m_nTriggerTime;}
	int TimerType(){return m_nTimerType;}
	timerid_t TimerID(){return m_nTimerID;}
public:
	time_t m_nTriggerTime;
	int m_nTimerType;
	timerid_t m_nTimerID;
};


#endif



