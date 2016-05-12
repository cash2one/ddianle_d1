#include "AmuseAction_RequestDevice.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "GameMsg_S2C_AmuseMsg.h"


CAmuseAction_RequestDevice::CAmuseAction_RequestDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_RequestDevice, false, false)
,m_nInviterID(0)
,m_nDeviceID(0)
{
}

CAmuseAction_RequestDevice::~CAmuseAction_RequestDevice(void)
{
}

bool CAmuseAction_RequestDevice::FormParam(CAmuseActionParam_Base* pActionParam)
{
	return false;
}

bool CAmuseAction_RequestDevice::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	CAmuseRoomComponent* pInviterPlayer = m_pActPlayer->AmuseRoom()->GetAmusePlayer( m_nInviterID );
	if ( pInviterPlayer != NULL )
	{
		bool bHasRequest = false;

		AmuseActionByTypeMap& allAmuseActions = m_pActPlayer->AllAmuseActions();
		AmuseActionByTypeMap::iterator it_Type = allAmuseActions.find( eAmuseAction_RequestDevice );
		if ( it_Type != allAmuseActions.end() )
		{
			for ( AmuseActionByIDMap::iterator it_Action = it_Type->second.begin(); it_Action != it_Type->second.end(); ++it_Action )
			{
				CAmuseAction_RequestDevice* pRequestAction = (CAmuseAction_RequestDevice*)(it_Action->second);
				if ( pRequestAction != NULL && pRequestAction->m_nInviterID == m_nInviterID && pRequestAction->m_nDeviceID == m_nDeviceID )
				{
					bHasRequest = true;
					break;
				}
			}
		}

		if ( !bHasRequest )
		{
			CAmuseActionParam_RequestDevice* pRequestDeviceParam = new CAmuseActionParam_RequestDevice();
			pRequestDeviceParam->m_nPlayerID = m_nInviterID;
			pRequestDeviceParam->m_strPlayerName = pInviterPlayer->AttrCOM()->GetRoleName();
			pRequestDeviceParam->m_chPlayerSex = pInviterPlayer->AttrCOM()->GetSex();
			pRequestDeviceParam->m_bIsVIP = pInviterPlayer->VipCOM()->IsVIP();
			pRequestDeviceParam->m_nVIPLevel = (unsigned short)pInviterPlayer->VipCOM()->VIPLevel();
			pRequestDeviceParam->m_nDeviceID = (char)m_nDeviceID;
			pRequestDeviceParam->m_strRequestTips = "DoAmuseAction_BeInvitedDevice";

			GameMsg_S2C_DoAmuseActionSuc msgResponse;
			msgResponse.m_pActionParam = pRequestDeviceParam;
			m_pActPlayer->SendPlayerMsg( &msgResponse );

			m_bSuccess = true;
			return true;
		}
	}

	return false;
}

//file end

