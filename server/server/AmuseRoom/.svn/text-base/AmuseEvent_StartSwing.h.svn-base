#ifndef __AMUSEEVENT_STARTSWING__H__
#define __AMUSEEVENT_STARTSWING__H__


#include "AmuseEventBase.h"

class CAmuseRoom;


class CAmuseEvent_StartSwing : public CAmuseEvent_Base
{
public:
	CAmuseEvent_StartSwing(CAmuseRoom* pAmuseRoom, bool bLoopEvent, int nEventTime);
	virtual ~CAmuseEvent_StartSwing();

public:
	virtual CAmuseEventParam_Base* ToParam(int nNowTime);

	virtual void GoStart(int nNowTime);

public:
	CAmuseRoom* m_pOwnerRoom;

	int m_nStartTime;
};

#endif
//file end

