#include "AmuseEvent_PushSwing.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "AmuseAction_LeaveDevice.h"
#include "GameMsg_S2C_AmuseMsg.h"


CAmuseEvent_PushSwing::CAmuseEvent_PushSwing(CAmuseRoomComponent* pAmusePlayer, bool bLoopEvent, int nEventTime, unsigned int nActionID)
:CAmuseEvent_Base(eAmuseEvent_PushSwing, bLoopEvent, nEventTime)
,m_pOwnerPlayer(pAmusePlayer)
,m_nActionID(nActionID)
{
}

CAmuseEvent_PushSwing::~CAmuseEvent_PushSwing(void)
{
}

CAmuseEventParam_Base* CAmuseEvent_PushSwing::ToParam(int nNowTime)
{
	return NULL;
}

void CAmuseEvent_PushSwing::GoStart(int nNowTime)
{
	m_bEverStart = true;
}

void CAmuseEvent_PushSwing::GoEnd()
{
	CAmuseAction_Base* pUseDeviceAction = m_pOwnerPlayer->UniqueAmuseAction( eAmuseAction_UseDevice );
	if ( pUseDeviceAction != NULL && pUseDeviceAction->ActionID() == m_nActionID )
	{
		m_pOwnerPlayer->AmuseActionOver( pUseDeviceAction );

		CAmuseActionParam_LeaveDevice* pLeaveDeviceParam = new CAmuseActionParam_LeaveDevice();
		pLeaveDeviceParam->m_nPlayerID = m_pOwnerPlayer->RoleID();

		GameMsg_S2C_DoAmuseActionSuc msgResponse;
		msgResponse.m_pActionParam = pLeaveDeviceParam;
		m_pOwnerPlayer->AmuseRoom()->SendMsgToAll( &msgResponse );
	}
}

//file end

