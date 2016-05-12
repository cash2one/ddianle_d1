#ifndef __ANNOUNCE_H__
#define __ANNOUNCE_H__


#include <string>
#include "ITimerGm.h"
#include "GMDataStruct.h"

class CAnnounceTimer : public ITimerGm
{
public:
    CAnnounceTimer();
    ~CAnnounceTimer();

public:
    virtual void OnEvent();
    virtual time_t TriggerTime();
    virtual ETimerType TimerType();
    virtual unsigned int TimerID();

public:
    time_t m_nTriggerTime;
    CAnnounceInfo m_annouceInfo;
    ETimerType m_nTimerType;
};

#endif

