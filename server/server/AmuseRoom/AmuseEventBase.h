#ifndef __AMUSEEVENT_BASE__H__
#define __AMUSEEVENT_BASE__H__


class CAmuseEventParam_Base;


class CAmuseEvent_Base
{
public:
	CAmuseEvent_Base(int nEventType, bool bLoopEvent, int nEventTime)
	:m_nEventType(nEventType)
	,m_bLoopEvent(bLoopEvent)
	,m_nEventTime(nEventTime)
	,m_bEverStart(false)
	{
	}

	virtual ~CAmuseEvent_Base()
	{

	}

	virtual CAmuseEventParam_Base* ToParam(int nNowTime) = 0;

	virtual void GoStart(int nNowTime) = 0;
	virtual void GoEnd() {}

public:
	int EventType() const
	{
		return m_nEventType;
	}

	bool IsLoop() const
	{
		return m_bLoopEvent;
	}

	int EventTime() const		//ms
	{
		return m_nEventTime;
	}

	bool EverStart() const
	{
		return m_bEverStart;
	}

protected:
	int m_nEventType;
	bool m_bLoopEvent;
	int m_nEventTime;
	bool m_bEverStart;
};

#endif
//file end

