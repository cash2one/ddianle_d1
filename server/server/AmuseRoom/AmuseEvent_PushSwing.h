#ifndef __AMUSEEVENT_PUSHSWING__H__
#define __AMUSEEVENT_PUSHSWING__H__


#include "AmuseEventBase.h"

class CAmuseRoomComponent;


class CAmuseEvent_PushSwing : public CAmuseEvent_Base
{
public:
	CAmuseEvent_PushSwing(CAmuseRoomComponent* pAmusePlayer, bool bLoopEvent, int nEventTime, unsigned int nActionID);
	virtual ~CAmuseEvent_PushSwing();

public:
	virtual CAmuseEventParam_Base* ToParam(int nNowTime);

	virtual void GoStart(int nNowTime);
	virtual void GoEnd();

public:
	CAmuseRoomComponent* m_pOwnerPlayer;

	unsigned int m_nActionID;
};

#endif
//file end

