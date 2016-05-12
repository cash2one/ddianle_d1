#ifndef __AMUSEEVENT_STARTBOAT_H__
#define __AMUSEEVENT_STARTBOAT_H__


#include "AmuseEventBase.h"
#include <vector>
#include <string>

class CAmuseRoom;


class CAmuseEvent_StartBoat : public CAmuseEvent_Base
{
public:
	CAmuseEvent_StartBoat(CAmuseRoom* pAmuseRoom, bool bLoopEvent, int nEventTime, int nBoatDock);
	virtual ~CAmuseEvent_StartBoat();

public:
	virtual CAmuseEventParam_Base* ToParam(int nNowTime);

	virtual void GoStart(int nNowTime);

public:
	CAmuseRoom* m_pOwnerRoom;

	int m_nStartTime;
	int m_nBoatDock;
};

#endif
//file end

