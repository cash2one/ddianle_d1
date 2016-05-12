#include "AmuseAction_UseDevice.h"
#include "AmuseAction_UsePose.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseRoomDevice.h"
#include "AmuseRoomDataManager.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "AmuseEvent_PushSwing.h"
#include "AmuseEvent_StartSwing.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../friend/EntityFriendComponent.h"


CAmuseAction_UseDevice::CAmuseAction_UseDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_UseDevice, true, true)
,m_nDeviceID(0)
,m_nUseType(eAmuseUseDevice_Normal)
,m_nPartnerType(eAmusePartner_None)
,m_nStartTime(0)
,m_bIsInvited(false)
{
}

CAmuseAction_UseDevice::~CAmuseAction_UseDevice(void)
{
}

bool CAmuseAction_UseDevice::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_UseDevice )
	{
		CAmuseActionParam_UseDevice* pUseDeviceParam = (CAmuseActionParam_UseDevice*)pActionParam;
		m_nDeviceID = pUseDeviceParam->m_nDeviceID;

		return true;
	}

	return false;
}

CAmuseActionParam_Base* CAmuseAction_UseDevice::ToParam(int nNowTime)
{
	CAmuseActionParam_UseDevice* pUseDeviceParam = new CAmuseActionParam_UseDevice();
	pUseDeviceParam->m_nPlayerID = m_pActPlayer->RoleID();
	pUseDeviceParam->m_nDeviceID = (char)m_nDeviceID;
	pUseDeviceParam->m_nPartnerType = (char)m_nPartnerType;
	pUseDeviceParam->m_nStartTime = ( m_nUseType == eAmuseUseDevice_Normal ? 0 : nNowTime - m_nStartTime );
	pUseDeviceParam->m_bInviter = !m_bIsInvited;
	CAmuseRoomDevice* pAmuseDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( m_nDeviceID );
	if ( pAmuseDevice != NULL && pAmuseDevice->GetDeviceType() == AmuseRoomDeviceType_TimeEgg )
	{
		TimeEggSetting& setting = CAmuseRoomDataManager::Instance().GetTimeEggSetting();
		pUseDeviceParam->m_listCost = setting.m_listCostItem;
	}
	return pUseDeviceParam;
}

bool CAmuseAction_UseDevice::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice ));
	if ( pUseDeviceAction != NULL )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_AlreadyInDevice";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
	}
	else
	{
		CAmuseRoomDevice* pAmuseDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( m_nDeviceID );
		if ( pAmuseDevice != NULL )
		{
			unsigned int nNow = (unsigned int)time(NULL);
			if ( pAmuseDevice->GetDeviceType() == AmuseRoomDeviceType_TimeEgg )
			{
				TimeEggEntry* pEgg = m_pActPlayer->GetTimeEggInCurScene();
				if ( pEgg && nNow < pEgg->m_nEndTime )
				{
					GameMsg_S2C_DoAmuseActionFail msgResponse;
					msgResponse.m_strError = "DoAmuseAction_OpenTimeEggNotTime";
					m_pActPlayer->SendPlayerMsg( &msgResponse );
					return false;
				}
			}
			if ( pAmuseDevice->HasUser( m_pActPlayer->RoleID() ) )
			{
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				msgResponse.m_strError = "DoAmuseAction_UseSameDevice";
				m_pActPlayer->SendPlayerMsg( &msgResponse );
			}
			else
			{
				if ( pAmuseDevice->ReachCapacity() )
				{
					GameMsg_S2C_DoAmuseActionFail msgResponse;
					msgResponse.m_strError = "DoAmuseAction_UseFullDevice";
					m_pActPlayer->SendPlayerMsg( &msgResponse );
				}
				else
				{
					if ( _ProcessUseDevice( *pAmuseDevice, rNewAction ) )
					{
						CAmuseAction_UsePose* pUsePoseAction = (CAmuseAction_UsePose*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UsePose ));
						if ( pUsePoseAction != NULL )
						{
							m_pActPlayer->AmuseActionOver( pUsePoseAction );

							CAmuseActionParam_LeavePose* pLeavePoseParam = new CAmuseActionParam_LeavePose();
							pLeavePoseParam->m_nPlayerID = m_pActPlayer->RoleID();

							GameMsg_S2C_DoAmuseActionSuc msgLeavePose;
							msgLeavePose.m_pActionParam = pLeavePoseParam;
							m_pActPlayer->AmuseRoom()->SendMsgToAll( &msgLeavePose );
						}

						if ( !m_pActPlayer->CoupleCOM()->IsSingle() && pAmuseDevice->HasUser( m_pActPlayer->CoupleCOM()->CoupleID() ) )
						{
							if ( m_pActPlayer->CoupleCOM()->IsMarried() )
							{
								m_nPartnerType = eAmusePartner_Couple;
							}
							else
							{
								m_nPartnerType = eAmusePartner_Lover;
							}
						}

						pAmuseDevice->UserIn( m_pActPlayer->RoleID() );

						if ( m_nPartnerType == eAmusePartner_None )
						{
							CAmuseRoomComponent* pPartner = pAmuseDevice->GetOtherPlayer(m_pActPlayer->RoleID());
							if ( pPartner && m_pActPlayer->FriendCOM()->IsFriend(pPartner->RoleID()) )
							{
								m_nPartnerType = eAmusePartner_Friend;
							}
						}

						CAmuseActionParam_UseDevice* pUseDeviceParam = new CAmuseActionParam_UseDevice();
						pUseDeviceParam->m_nPlayerID = m_pActPlayer->RoleID();
						pUseDeviceParam->m_nDeviceID = (char)m_nDeviceID;
						pUseDeviceParam->m_nPartnerType = (char)m_nPartnerType;
						pUseDeviceParam->m_bInviter = !m_bIsInvited;
						if ( pAmuseDevice->GetDeviceType() == AmuseRoomDeviceType_TimeEgg )
						{
							TimeEggSetting& setting = CAmuseRoomDataManager::Instance().GetTimeEggSetting();
							pUseDeviceParam->m_listCost = setting.m_listCostItem;
						}

						GameMsg_S2C_DoAmuseActionSuc msgUseDevice;
						msgUseDevice.m_pActionParam = pUseDeviceParam;
						m_pActPlayer->AmuseRoom()->SendMsgToAll( &msgUseDevice );

						m_bSuccess = true;
						return true;
					}
				}
			}
		}
	}

	return false;
}

void CAmuseAction_UseDevice::GoTerminate()
{
	if ( m_bSuccess )
	{
		CAmuseRoomDevice* pAmuseDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( m_nDeviceID );
		if ( pAmuseDevice != NULL )
		{
			_TerminateUseDevice( *pAmuseDevice );

			pAmuseDevice->UserOut( m_pActPlayer->RoleID() );
		}
	}
}

void CAmuseAction_UseDevice::SetAsInvited()
{
	m_bIsInvited = true;
}

int CAmuseAction_UseDevice::_GetUseDeviceType(CAmuseRoomDevice& rAmuseDevice)
{
	int nUseType = eAmuseUseDevice_Normal;

	CAmuseSwingData swingData;
	if ( CAmuseRoomDataManager::Instance().GetSwingData( m_pActPlayer->AmuseRoom()->RoomScene(), swingData ) )
	{
		if ( swingData.m_nDeviceID == rAmuseDevice.DeviceID() )
		{
			nUseType = eAmuseUseDevice_Swing;
		}
	}
	if ( nUseType == eAmuseUseDevice_Normal && 
		CAmuseRoomDataManager::Instance().GetEggData( m_pActPlayer->AmuseRoom()->RoomScene(), rAmuseDevice.DeviceID() ) )
	{
		nUseType = eAmuseUseDevice_TimeEgg;
	}

	return nUseType;
}

bool CAmuseAction_UseDevice::_ProcessUseDevice(CAmuseRoomDevice& rAmuseDevice, std::list<CAmuseAction_Base*>& rNewAction)
{
	m_nUseType = _GetUseDeviceType( rAmuseDevice );

	if ( m_nUseType == eAmuseUseDevice_Normal )
	{
		return _ProcessUseNormal( rAmuseDevice, rNewAction );
	}
	else if ( m_nUseType == eAmuseUseDevice_TimeEgg )
	{
		return _ProcessUseTimeEgg( rAmuseDevice, rNewAction );
	}
	else if ( m_nUseType == eAmuseUseDevice_Swing )
	{
		return _ProcessUseSwing( rAmuseDevice, rNewAction );
	}
	
	return false;
}

bool CAmuseAction_UseDevice::_ProcessUseNormal(CAmuseRoomDevice& rAmuseDevice, std::list<CAmuseAction_Base*>& rNewAction)
{
	if ( rAmuseDevice.InviteType() != eAmuseInvite_None && rAmuseDevice.HasUser() && !m_bIsInvited )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_UseInviteDevice";
		m_pActPlayer->SendPlayerMsg( &msgResponse );

		return false;
	}

	return true;
}

bool CAmuseAction_UseDevice::_ProcessUseTimeEgg(CAmuseRoomDevice& rAmuseDevice, std::list<CAmuseAction_Base*>& rNewAction)
{
	if ( rAmuseDevice.HasUser() && !m_bIsInvited )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_UseInviteDevice";
		m_pActPlayer->SendPlayerMsg( &msgResponse );

		return false;
	}

	return true;
}

bool CAmuseAction_UseDevice::_ProcessUseSwing(CAmuseRoomDevice& rAmuseDevice, std::list<CAmuseAction_Base*>& rNewAction)
{
	if ( !rAmuseDevice.HasUser() && !m_bIsInvited )
	{
		CAmuseSwingData swingData;

		if ( CAmuseRoomDataManager::Instance().GetSwingData( m_pActPlayer->AmuseRoom()->RoomScene(), swingData ) )
		{
			CAmuseEvent_PushSwing* pPushSwingEvent = new CAmuseEvent_PushSwing( m_pActPlayer, false, swingData.m_nPushTime, m_nActionID );
			m_pActPlayer->UpdateAmusePlayerEvent( 0, pPushSwingEvent );

			return true;
		}
	}

	return false;
}

void CAmuseAction_UseDevice::_TerminateUseDevice(CAmuseRoomDevice& rAmuseDevice)
{
	if ( m_nUseType == eAmuseUseDevice_Swing )
	{
		_TerminateUseSwing( rAmuseDevice );
	}
	else if ( m_nUseType == eAmuseUseDevice_TimeEgg )
	{
		_TerminateUseTimeEgg( rAmuseDevice );
	}
}

void CAmuseAction_UseDevice::_TerminateUseSwing(CAmuseRoomDevice& rAmuseDevice)
{
	CAmuseSwingData swingData;

	if ( CAmuseRoomDataManager::Instance().GetSwingData( m_pActPlayer->AmuseRoom()->RoomScene(), swingData ) )
	{
		CAmuseEvent_StartSwing* pStartSwintEvent = new CAmuseEvent_StartSwing( m_pActPlayer->AmuseRoom(), false, swingData.m_nSwingTime );
		m_pActPlayer->AmuseRoom()->UpdateAmuseRoomEvent( 0, pStartSwintEvent );
	}
}

void CAmuseAction_UseDevice::_TerminateUseTimeEgg( CAmuseRoomDevice& rAmuseDevice )
{
	//时光蛋的邀请者离开时其他玩家也下设备
	if ( !m_bIsInvited )
	{
		std::vector<CAmuseRoomComponent*> players;
		rAmuseDevice.GetPlayers(players);
		for (std::vector<CAmuseRoomComponent*>::iterator it = players.begin();
			it != players.end(); ++it)
		{
			CAmuseRoomComponent* pPlayer = *it;
			if ( pPlayer && pPlayer != m_pActPlayer )
			{
				pPlayer->AmuseActionOver();

				CAmuseActionParam_LeaveDevice* pLeaveDeviceParam = new CAmuseActionParam_LeaveDevice();
				pLeaveDeviceParam->m_nPlayerID = pPlayer->RoleID();

				GameMsg_S2C_DoAmuseActionSuc msgResponse;
				msgResponse.m_pActionParam = pLeaveDeviceParam;
				pPlayer->AmuseRoom()->SendMsgToAll( &msgResponse );
			}
		}
	}
}

//file end

