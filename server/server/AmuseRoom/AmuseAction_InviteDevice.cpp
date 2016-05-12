#include "AmuseAction_InviteDevice.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseRoomDevice.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseAction_RequestDevice.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "AmuseRoomDataManager.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../friend/EntityFriendComponent.h"

CAmuseAction_InviteDevice::CAmuseAction_InviteDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_InviteDevice, true, false)
,m_nInviteeID(0)
{
}

CAmuseAction_InviteDevice::~CAmuseAction_InviteDevice(void)
{
}

bool CAmuseAction_InviteDevice::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_InviteDevice )
	{
		CAmuseActionParam_InviteDevice* pInviteDeviceParam = (CAmuseActionParam_InviteDevice*)pActionParam;
		m_nInviteeID = pInviteDeviceParam->m_nInviteeID;

		return true;
	}

	return false;
}

bool CAmuseAction_InviteDevice::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice ));
	if ( pUseDeviceAction != NULL && m_pActPlayer->AttrCOM()->GetRoleID() != m_nInviteeID )
	{
		int nDeviceID = pUseDeviceAction->m_nDeviceID;

		CAmuseRoomDevice* pAmuseDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( nDeviceID );
		if ( pAmuseDevice != NULL && pAmuseDevice->HasUser( m_pActPlayer->RoleID() ) && pAmuseDevice->InviteType() != eAmuseInvite_None )
		{
			TimeEggEntry* pInviterEgg = NULL;
			TimeEggEntry* pInviteeEgg = NULL;
			//时光蛋相关操作
			EggInviteType nInviterInviteType = EggInviteType_None;
			if (pAmuseDevice->GetDeviceType() == AmuseRoomDeviceType_TimeEgg)
			{
				nInviterInviteType = m_pActPlayer->GetInviteTypeInCurScene();
				pInviterEgg = m_pActPlayer->GetTimeEggInCurScene();
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				if (!m_pActPlayer->CheckInviteTypeInCurScene(nInviterInviteType, m_nInviteeID, msgResponse.m_strError))
				{
					m_pActPlayer->SendPlayerMsg( &msgResponse );
					return false;
				}
				if ( nInviterInviteType == EggInviteType_Create && !pUseDeviceAction->m_bIsInvited )
				{
					if ( m_pActPlayer->CoupleCOM()->IsSingle() || m_pActPlayer->CoupleCOM()->CoupleID() != m_nInviteeID )
					{
						if ( !m_pActPlayer->FriendCOM()->IsFriend(m_nInviteeID) )
						{
							msgResponse.m_strError = "DoAmuseAction_NoRelation";
							m_pActPlayer->SendPlayerMsg( &msgResponse );
							return false;
						}
					}
				}
			}
			if ( !pAmuseDevice->ReachCapacity() )
			{
				CAmuseRoomComponent* pInviteePlayer = m_pActPlayer->AmuseRoom()->GetAmusePlayer( m_nInviteeID );
				if ( pInviteePlayer != NULL )
				{
					if (pAmuseDevice->GetDeviceType() == AmuseRoomDeviceType_TimeEgg)
					{
						pInviteeEgg = pInviteePlayer->GetTimeEggInCurScene();
						GameMsg_S2C_DoAmuseActionFail msgResponse;
						if ( nInviterInviteType == EggInviteType_Create )
						{
							if ( pInviteeEgg )
							{
								msgResponse.m_strError = "DoAmuseAction_PartnerAlreadyExistEgg";
								m_pActPlayer->SendPlayerMsg( &msgResponse );
								return false;
							}
						}
						else if ( nInviterInviteType == EggInviteType_Open )
						{
							if ( pInviteeEgg == NULL )
							{
								msgResponse.m_strError = "DoAmuseAction_PartnerAlreadyOpenTimeEgg";
								m_pActPlayer->SendPlayerMsg( &msgResponse );
								return false;
							}
							else if ( pInviterEgg->m_nEggID != pInviteeEgg->m_nEggID )
							{
								msgResponse.m_strError = "DoAmuseAction_NoRightPartner";
								m_pActPlayer->SendPlayerMsg( &msgResponse );
								return false;
							}
						}
					}
					CAmuseAction_Base* pInviteeAction = pInviteePlayer->UniqueAmuseAction( eAmuseAction_UseDevice );
					if ( pInviteeAction == NULL )
					{
						CAmuseAction_RequestDevice* pRequestAction = new CAmuseAction_RequestDevice( pInviteePlayer, m_pActPlayer->AmuseRoom()->ActionID() );
						pRequestAction->m_nInviterID = m_pActPlayer->RoleID();
						pRequestAction->m_nDeviceID = nDeviceID;
						rNewAction.push_back( pRequestAction );

						m_bSuccess = true;
					}
					else
					{
						GameMsg_S2C_DoAmuseActionFail msgResponse;
						msgResponse.m_strError = "DoAmuseAction_InviteeBusyPlayer";
						m_pActPlayer->SendPlayerMsg( &msgResponse );
					}
				}
				else
				{
					GameMsg_S2C_DoAmuseActionFail msgResponse;
					msgResponse.m_strError = "DoAmuseAction_InviteNoPlayer";
					m_pActPlayer->SendPlayerMsg( &msgResponse );
				}
			}
			else
			{
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				msgResponse.m_strError = "DoAmuseAction_InviteFullDevice";
				m_pActPlayer->SendPlayerMsg( &msgResponse );
			}
		}
	}

	return false;
}

//file end

