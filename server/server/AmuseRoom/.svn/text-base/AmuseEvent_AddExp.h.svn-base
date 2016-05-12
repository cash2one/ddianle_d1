#ifndef __AMUSEEVENT_ADDEXP__H__
#define __AMUSEEVENT_ADDEXP__H__


#include "AmuseEventBase.h"

class CAmuseRoomComponent;


class CAmuseEvent_AddExp : public CAmuseEvent_Base
{
public:
	CAmuseEvent_AddExp(CAmuseRoomComponent* pAmusePlayer, bool bLoopEvent, int nEventTime, int nAddExp);
	virtual ~CAmuseEvent_AddExp();

public:
	virtual CAmuseEventParam_Base* ToParam(int nNowTime);

	virtual void GoStart(int nNowTime);

public:
	CAmuseRoomComponent* m_pOwnerPlayer;

	int m_nAddExp;
};

#endif
//file end

