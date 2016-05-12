#include "AmuseAction_UsePose.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "../../datastructure/datainfor/ConfigManager.h"


CAmuseAction_UsePose::CAmuseAction_UsePose(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_UsePose, true, true)
,m_nPoseID(0)
{
}

CAmuseAction_UsePose::~CAmuseAction_UsePose(void)
{
}

bool CAmuseAction_UsePose::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_UsePose )
	{
		CAmuseActionParam_UsePose* pUsePoseParam = (CAmuseActionParam_UsePose*)pActionParam;
		m_nPoseID = pUsePoseParam->m_nPoseID;

		return true;
	}

	return false;
}

CAmuseActionParam_Base* CAmuseAction_UsePose::ToParam(int nNowTime)
{
	CAmuseActionParam_UsePose* pUsePoseParam = new CAmuseActionParam_UsePose();
	pUsePoseParam->m_nPlayerID = m_pActPlayer->RoleID();
	pUsePoseParam->m_nPoseID = (char)m_nPoseID;

	return pUsePoseParam;
}

bool CAmuseAction_UsePose::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	CAmuseRoomPoseConfig* pPoseInfo = ConfigManager::Instance().GetAmuseMgr().GetAmusePoseInfo( m_nPoseID );
	if ( pPoseInfo != NULL )
	{
		CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice ));
		if ( pUseDeviceAction != NULL )
		{
			GameMsg_S2C_DoAmuseActionFail msgResponse;
			msgResponse.m_strError = "DoAmuseAction_AlreadyInDevice";
			m_pActPlayer->SendPlayerMsg( &msgResponse );
		}
		else
		{
			CAmuseActionParam_UsePose* pUsePoseParam = new CAmuseActionParam_UsePose();
			pUsePoseParam->m_nPlayerID = m_pActPlayer->RoleID();
			pUsePoseParam->m_nPoseID = (char)m_nPoseID;

			GameMsg_S2C_DoAmuseActionSuc msgResponse;
			msgResponse.m_pActionParam = pUsePoseParam;
			m_pActPlayer->AmuseRoom()->SendMsgToAll( &msgResponse );

			m_bSuccess = true;
			return true;
		}
	}

	return false;
}

//file end

