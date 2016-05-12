#include "AmuseAction_LeavePose.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"


CAmuseAction_LeavePose::CAmuseAction_LeavePose(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_LeavePose, true, false)
{
}

CAmuseAction_LeavePose::~CAmuseAction_LeavePose(void)
{
}

bool CAmuseAction_LeavePose::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_LeavePose )
	{
		return true;
	}

	return false;
}

bool CAmuseAction_LeavePose::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	CAmuseAction_Base* pUsePoseAction = m_pActPlayer->UniqueAmuseAction( eAmuseAction_UsePose );
	if ( pUsePoseAction != NULL )
	{
		m_pActPlayer->AmuseActionOver( pUsePoseAction );

		CAmuseActionParam_LeavePose* pLeavePoseParam = new CAmuseActionParam_LeavePose();
		pLeavePoseParam->m_nPlayerID = m_pActPlayer->RoleID();

		GameMsg_S2C_DoAmuseActionSuc msgResponse;
		msgResponse.m_pActionParam = pLeavePoseParam;
		m_pActPlayer->AmuseRoom()->SendMsgToAll( &msgResponse );

		m_bSuccess = true;
	}

	return false;
}

//file end

