#include "AmuseEvent_StartSwing.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"


#define AMUSEROOM_SWING_POWER 1


CAmuseEvent_StartSwing::CAmuseEvent_StartSwing(CAmuseRoom* pAmuseRoom, bool bLoopEvent, int nEventTime)
:CAmuseEvent_Base(eAmuseEvent_StartSwing, bLoopEvent, nEventTime)
,m_pOwnerRoom(pAmuseRoom)
,m_nStartTime(0)
{
}

CAmuseEvent_StartSwing::~CAmuseEvent_StartSwing(void)
{
}

CAmuseEventParam_Base* CAmuseEvent_StartSwing::ToParam(int nNowTime)
{
	CAmuseEventParam_StartSwing* pSwingParam = NULL;
	if ( m_nStartTime > 0 )
	{
		pSwingParam = new CAmuseEventParam_StartSwing();
		pSwingParam->m_nStartPower = AMUSEROOM_SWING_POWER;
		pSwingParam->m_nStartTime = nNowTime - m_nStartTime;
	}

	return pSwingParam;
}

void CAmuseEvent_StartSwing::GoStart(int nNowTime)
{
	m_nStartTime = nNowTime;

	CAmuseEventParam_StartSwing* pSwingParam = new CAmuseEventParam_StartSwing();
	pSwingParam->m_nStartPower = AMUSEROOM_SWING_POWER;
	pSwingParam->m_nStartTime = 0;

	GameMsg_S2C_AmuseEventNotice msgNotice;
	msgNotice.m_pEventParam = pSwingParam;
	m_pOwnerRoom->SendMsgToAll( &msgNotice );

	m_bEverStart = true;
}

//file end

