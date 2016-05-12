#include "AmuseAction_ReponseDevice.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseRoomDevice.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseAction_RequestDevice.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "AmuseAction_OpenTimeEgg.h"

CAmuseAction_ResponseDevice::CAmuseAction_ResponseDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_ResponseDevice, true, false)
{
}

CAmuseAction_ResponseDevice::~CAmuseAction_ResponseDevice(void)
{
}

bool CAmuseAction_ResponseDevice::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_ResponseDevice )
	{
		CAmuseActionParam_ResponseDevice* pResponseDeviceParam = (CAmuseActionParam_ResponseDevice*)pActionParam;
		m_nInviterID = pResponseDeviceParam->m_nInviterID;
		m_nDeviceID = pResponseDeviceParam->m_nDeviceID;
		m_bAccept = pResponseDeviceParam->m_bAccept;

		return true;
	}

	return false;
}

bool CAmuseAction_ResponseDevice::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	CAmuseAction_RequestDevice* pRequestDeviceAction = NULL;

	AmuseActionByTypeMap& allAmuseActions = m_pActPlayer->AllAmuseActions();
	AmuseActionByTypeMap::iterator it_Type = allAmuseActions.find( eAmuseAction_RequestDevice );
	if ( it_Type != allAmuseActions.end() )
	{
		for ( AmuseActionByIDMap::iterator it_Action = it_Type->second.begin(); it_Action != it_Type->second.end(); ++it_Action )
		{
			CAmuseAction_RequestDevice* pCurRequestAction = (CAmuseAction_RequestDevice*)(it_Action->second);
			if ( pCurRequestAction != NULL && pCurRequestAction->m_nInviterID == m_nInviterID && pCurRequestAction->m_nDeviceID == m_nDeviceID )
			{
				pRequestDeviceAction = pCurRequestAction;
				break;
			}
		}
	}

	if ( pRequestDeviceAction != NULL )
	{
		CAmuseRoomComponent* pInviter = m_pActPlayer->AmuseRoom()->GetAmusePlayer(m_nInviterID);
		if ( m_bAccept )
		{
			CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice ));
			if ( pUseDeviceAction == NULL )
			{
				CAmuseRoomDevice* pRoomDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( m_nDeviceID );
				if ( pRoomDevice != NULL )
				{
					if ( pRoomDevice->HasUser( m_nInviterID ) )
					{
						pUseDeviceAction = new CAmuseAction_UseDevice( m_pActPlayer, m_pActPlayer->AmuseRoom()->ActionID() );
						pUseDeviceAction->m_nDeviceID = m_nDeviceID;
						pUseDeviceAction->SetAsInvited();
						rNewAction.push_back( pUseDeviceAction );

						if ( pRoomDevice->GetDeviceType() == AmuseRoomDeviceType_TimeEgg )
						{
							TimeEggEntry* pEgg = m_pActPlayer->GetTimeEggInCurScene();
							if ( pEgg )
							{
								CAmuseAction_OpenTimeEgg* pOpenTimeEggAction = new CAmuseAction_OpenTimeEgg( m_pActPlayer, m_pActPlayer->AmuseRoom()->ActionID() );
								pOpenTimeEggAction->m_chOpenType = OpenEggType_Together;
								rNewAction.push_back( pOpenTimeEggAction );
							}
						}

						m_bSuccess = true;
					}
					else
					{
						GameMsg_S2C_DoAmuseActionFail msgResponse;
						msgResponse.m_strError = "DoAmuseAction_ResponseNoInviter";
						m_pActPlayer->SendPlayerMsg( &msgResponse );
					}
				}
			}
			else
			{
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				msgResponse.m_strError = "DoAmuseAction_ResponseInDevice";
				m_pActPlayer->SendPlayerMsg( &msgResponse );
			}
		}
		else
		{
			if ( pInviter )
			{
				CAmuseActionParam_ResponseDevice* pActionParam = new CAmuseActionParam_ResponseDevice();
				pActionParam->m_bAccept = false;

				GameMsg_S2C_DoAmuseActionSuc msgResponse;
				msgResponse.m_pActionParam = pActionParam;
				pInviter->SendPlayerMsg( &msgResponse );
			}
			m_bSuccess = true;
		}

		m_pActPlayer->AmuseActionOver( pRequestDeviceAction );
	}
	else
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_ResponseTimeOut";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
	}

	return false;
}

//file end

