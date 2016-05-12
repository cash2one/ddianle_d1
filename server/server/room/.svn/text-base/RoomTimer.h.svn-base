#ifndef __ROOM_TIMER_H__
#define __ROOM_TIMER_H__

#include "../util/ITimer.h"
#include <map>

enum ETimerType
{
	ETimerType_None,
	ETimerType_BeginToSelectPartner,
	ETimerType_EndToSelectPartner,
};

class CBeginToSelectPartnerTimer : public ITimer
{
public:
	CBeginToSelectPartnerTimer();
	~CBeginToSelectPartnerTimer();
public:
	virtual void OnEvent();
};

class CEndToSelectPartnerTimer : public ITimer
{
public:
	CEndToSelectPartnerTimer();
	~CEndToSelectPartnerTimer();
public:
	virtual void OnEvent();
};

#endif

