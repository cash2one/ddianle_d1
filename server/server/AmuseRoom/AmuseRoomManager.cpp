#include "AmuseRoomManager.h"
#include "AmuseRoom.h"
#include "AmuseRoomComponent.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseAction_LeaveDevice.h"
#include "AmuseAction_UsePose.h"
#include "AmuseAction_LeavePose.h"
#include "AmuseAction_InviteDevice.h"
#include "AmuseAction_ReponseDevice.h"
#include "AmuseAction_GetTimeEggGiftList.h"
#include "AmuseAction_CommitTimeEggInfo.h"
#include "AmuseAction_CreateTimeEgg.h"
#include "AmuseAction_OpenTimeEgg.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../room/RoomComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "AmuseRoomDataManager.h"
#include "ConstellationMatchManager.h"
#include "ConstellationMatchDataManager.h"
#include "GameMsg_C2S_AmuseMsg.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/LogStructDef.h"
#include "../activity/MeetActivityMgr.h"
#include "../../datastructure/datainfor/GMDataInfo.h"
#include "../../socket/Formatter.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../util/NameDeny.h"


#define AMUSEROOM_BEGIN_ID 1
#define AMUSEROOM_END_ID 9999
#define AMUSEROOM_COUNT_PAGE 8
#define AMUSEROOM_INVITE_FREQUENCE 3000
#define AMUSEROOM_INVITE_COUNT 3
#define AMUSEROOM_EVENT_CHECK 2000
#define AMUSEROOM_TEST_COUNT 100


CAmuseRoomManager::CAmuseRoomManager()
:m_lMgrNowTime(0)
{
	for ( unsigned int i = AMUSEROOM_BEGIN_ID; i <= AMUSEROOM_END_ID; ++i )
	{
		m_StoreID.push_back( i );
	}
}

CAmuseRoomManager::~CAmuseRoomManager()
{
	for ( AmuseRoomMap::iterator it = m_AllAmuseRooms.begin(); it != m_AllAmuseRooms.end(); ++it )
	{
		delete it->second;
	}

	m_AllAmuseRooms.clear();
	m_TopAmuseRoomID.clear();
	m_TopAmuseRooms.clear();
	m_ConstellationRoomID.clear();
}

CAmuseRoomManager& CAmuseRoomManager::Instance()
{
	static CAmuseRoomManager stcRoomMgr;
	return stcRoomMgr;
}

CAmuseRoom* CAmuseRoomManager::FindAmuseRoom(unsigned int nAmuseRoomID)
{
	AmuseRoomMap::iterator it = m_AllAmuseRooms.find( nAmuseRoomID );
	if( it != m_AllAmuseRooms.end() && it->second != NULL )
	{
		return it->second;
	}

	return NULL;
}

CAmuseRoom* CAmuseRoomManager::FindMeetAmuseRoom(unsigned int nAmuseRoomID)
{
	AmuseRoomMap::iterator it = m_MeetAmuseRooms.find( nAmuseRoomID );
	if( it != m_MeetAmuseRooms.end() && it->second != NULL )
	{
		return it->second;
	}

	return NULL;
}


void CAmuseRoomManager::DestroyAmuseRoom(unsigned int nAmuseRoomID)
{
	_ReleaseAmuseRoomID( nAmuseRoomID );

	CAmuseRoom* pAmuseRoom = RemoveAmuseRoom( nAmuseRoomID );
	if ( pAmuseRoom != NULL )
	{
		_HandleAmuseTopRoom( pAmuseRoom );

		CAmuseRoomComponent* pAmusePlayer = pAmuseRoom->GetAmusePlayer();
		while ( pAmusePlayer != NULL )
		{
			pAmuseRoom->PlayerOut( pAmusePlayer->RoleID() );

			GameMsg_S2C_QuitAmuseRoomSuc msgResponse;
			msgResponse.m_chQuitType = (unsigned char)eAmuseRoomQuit_Destroy;
			msgResponse.m_nKickerID = 0;
			if (pAmuseRoom->IsConstellationRoom())
			{
				if (ConstellationMatchDataManager::Instance().IsOpen())
				{
					msgResponse.m_nQuitTo = (unsigned char)eAmuseRoomQuitTo_ConstellationLobby;
				}
				else
				{
					msgResponse.m_nQuitTo = (unsigned char)eAmuseRoomQuitTo_World;
				}
				_RemoveConstellationRoomEndingList(pAmuseRoom->RoomID());
			}
			else
			{
				msgResponse.m_nQuitTo = (unsigned char)eAmuseRoomQuitTo_AmuseLobby;
			}
			
			pAmusePlayer->SendPlayerMsg( &msgResponse );

			pAmusePlayer = pAmuseRoom->GetAmusePlayer();
		}

		delete pAmuseRoom;
		pAmuseRoom = NULL;
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CAmuseRoomManager::DestroyAmuseRoom. pAmuseRoom == NULL" );
	}
}

void CAmuseRoomManager::DestroyMeetAmuseRoom( unsigned int nAmuseRoomID )
{
	_ReleaseAmuseRoomID( nAmuseRoomID );

	AmuseRoomMap::iterator it = m_MeetAmuseRooms.find( nAmuseRoomID );
	if ( it != m_MeetAmuseRooms.end() )
	{
		m_MeetAmuseRooms.erase( it );
	}
}

bool CAmuseRoomManager::AddAmuseRoom(CAmuseRoom* pAmuseRoom)
{
	if ( pAmuseRoom != NULL )
	{
		if ( FindAmuseRoom( pAmuseRoom->RoomID() ) == NULL )
		{
			m_AllAmuseRooms.insert( std::make_pair( pAmuseRoom->RoomID(), pAmuseRoom ) );
			_HandleAmuseTopRoom( pAmuseRoom );

			return true;
		}
		else
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CAmuseRoomManager::AddAmuseRoom. roomID(%d) already exist", pAmuseRoom->RoomID() );
		}
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CAmuseRoomManager::AddAmuseRoom. pAmuseRoom == NULL" );
	}

	return false;
}

bool CAmuseRoomManager::AddMeetAmuseRoom( CAmuseRoom* pAmuseRoom )
{
	return m_MeetAmuseRooms.insert( std::make_pair( pAmuseRoom->RoomID(), pAmuseRoom )).second;
}

CAmuseRoom* CAmuseRoomManager::RemoveAmuseRoom(unsigned int nAmuseRoomID)
{
	CAmuseRoom* pAmuseRoom = NULL;

	AmuseRoomMap::iterator it = m_AllAmuseRooms.find( nAmuseRoomID );
	if ( it != m_AllAmuseRooms.end() )
	{
		pAmuseRoom = it->second;
		m_AllAmuseRooms.erase( it );
		m_ConstellationRoomID.erase(nAmuseRoomID);

		_HandleAmuseTopRoom( pAmuseRoom );
	}

	return pAmuseRoom;
}

void CAmuseRoomManager::OnUpdate(const unsigned long& lTimeElapsed)
{
	m_lMgrNowTime += lTimeElapsed;

	for ( AmuseRoomMap::iterator it = m_AllAmuseRooms.begin(); it != m_AllAmuseRooms.end(); ++it )
	{
		if ( it->second != NULL )
		{
			it->second->OnUpdate( m_lMgrNowTime, lTimeElapsed );
		}
	}

	for (AmuseRoomTimeMap::iterator it = m_EndConstellationRooms.begin(); it != m_EndConstellationRooms.end();)
	{
		if (it->second == NULL)
		{
			m_EndConstellationRooms.erase( it++ );
			continue;
		}

		if ( it->first <= m_lMgrNowTime )
		{
			CAmuseRoom *pRoom = it->second;
			m_EndConstellationRooms.erase( it++ );
			DestroyAmuseRoom(pRoom->RoomID());
		}
		else
		{
			break;
		}
	}

	ConstellationMatchManager::Instance().OnUpdate(lTimeElapsed);
}

void CAmuseRoomManager::OnAmuseGetRoomList(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
    const GameMsg_C2S_GetAmuseRoomList& rMsgRequest = (const GameMsg_C2S_GetAmuseRoomList&)rMsg;

    std::vector<CAmuseRoom*> vecAmuseRooms;
    int nTotalRoomCount = m_AllAmuseRooms.size() - m_ConstellationRoomID.size();
    int nMaxPage = ( nTotalRoomCount - 1 ) / AMUSEROOM_COUNT_PAGE;
    int nCurPage = ( nMaxPage > rMsgRequest.m_nPage ? rMsgRequest.m_nPage : nMaxPage );

    int nRequestIndex = nCurPage * AMUSEROOM_COUNT_PAGE;
    int nRequestCount = nTotalRoomCount - nRequestIndex;
    if ( nRequestCount > AMUSEROOM_COUNT_PAGE )
    {
        nRequestCount = AMUSEROOM_COUNT_PAGE;
    }

    if ( m_TopAmuseRooms.size() > (size_t)nRequestIndex )
    {
        AmuseRoomList::iterator it = m_TopAmuseRooms.begin();
        advance( it, nRequestIndex );
        nRequestIndex = 0;

        for ( ; it != m_TopAmuseRooms.end(); ++it )
        {
            vecAmuseRooms.push_back( *it );
            --nRequestCount;

            if ( nRequestCount <= 0 )
            {
                break;
            }
        }
    }
    else
    {
        nRequestIndex -= m_TopAmuseRooms.size();
    }

    if ( nRequestCount > 0 )
    {
        for ( AmuseRoomMap::iterator it = m_AllAmuseRooms.begin(); it != m_AllAmuseRooms.end(); ++it )
        {
            if ( m_TopAmuseRoomID.find( it->first ) == m_TopAmuseRoomID.end() 
                && !IsConstellationRoom( it->first ) )
            {
                if ( nRequestIndex > 0 )
                {
                    --nRequestIndex;
                }
                else
                {
                    vecAmuseRooms.push_back( it->second );
                    --nRequestCount;

                    if ( nRequestCount <= 0 )
                    {
                        break;
                    }
                }
            }
        }
    }

    GameMsg_S2C_GetAmuseRoomListRes msgResponse;
    msgResponse.m_nCurPage = (unsigned short)nCurPage;
    msgResponse.m_nTotalCount = (unsigned short)nTotalRoomCount;

    for ( std::vector<CAmuseRoom*>::iterator it = vecAmuseRooms.begin(); it != vecAmuseRooms.end(); ++it )
    {
        if ( *it != NULL )
        {
            CAmuseRoomBriefInfo amuseRoomInfo;
            (*it)->EncodeBriefInfo( amuseRoomInfo );

            msgResponse.m_vecRoomBriefInfo.push_back( amuseRoomInfo );
        }
    }

    rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
}

bool CAmuseRoomManager::IsMeetAmuseSceneID( int nRoomID )
{
	AmuseRoomMap::iterator it = m_MeetAmuseRooms.find( nRoomID );
	if ( it != m_MeetAmuseRooms.end() )
	{
		return true;
	}

	return false;
}

std::string CAmuseRoomManager::CreateMeetAmuseRoom( int nSceneID, int & nAmuseID, CAmuseRoomComponent& rRoleAmuseRoomCOM )
{
	std::string strError;

	if ( rRoleAmuseRoomCOM.AmuseRoom() != NULL )
	{
		return "CreateAmuseRoom_InAmuseRoom";
	}

	if ( rRoleAmuseRoomCOM.StageRoomCOM()->IsInRoom())
	{
		return "CreateAmuseRoom_InStageRoom";
	}

	if ( rRoleAmuseRoomCOM.CoupleCOM()->WeddingRoom() != NULL )
	{
		return "CreateAmuseRoom_InWeddingRoom";
	}

	if ( rRoleAmuseRoomCOM.PhotoRoomCOM()->PhotoRoom() != NULL ) 
	{
		return "CreateAmuseRoom_InPhotoRoom";
	}

    if (rRoleAmuseRoomCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        return "CreateAmuseRoom_InCeremony";
    }

	CAmuseRoomSceneConfig* pSceneInfo = ConfigManager::Instance().GetAmuseMgr().GetAmuseSceneInfo( nSceneID );
	if ( pSceneInfo == NULL )
	{
		return "CreateAmuseRoom_NoScene";
	}

	unsigned int nRoomID = _GenerateAmuseRoomID();
	if ( nRoomID == 0 )
	{
		return "CreateAmuseRoom_RoomEnough";
	}
	else
	{
		nAmuseID = nRoomID;
	}

	CAmuseRoom* pAmuseRoom = new CAmuseRoom( nRoomID, eAmuseTheme_None, nSceneID, "", "", m_lMgrNowTime );
	pAmuseRoom->SetRoomCapacity(2);
	if ( pAmuseRoom == NULL || !AddMeetAmuseRoom( pAmuseRoom ) )
	{
		return "CreateAmuseRoom_AddError";
	}
	
	return "";
}

void CAmuseRoomManager::OnAmuseCreateRoom(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
    const GameMsg_C2S_CreateAmuseRoom& rMsgRequest = (const GameMsg_C2S_CreateAmuseRoom&)rMsg;
    std::string strError;

    if ( rMsgRequest.m_strRoomName.empty() )
    {
        strError.assign( "CreateAmuseRoom_EmptyName" );
    }
    else if ( rMsgRequest.m_strRoomName.length() > (size_t)g_ServerConfig.m_nMaxAmuseRoomNameLength )
    {
        strError.assign( "CreateAmuseRoom_NameTooLong" );
    }
    else if ( rMsgRequest.m_strRoomPwd.length() > MAX_AMUSEROOM_PASSWD )
    {
        strError.assign( "CreateAmuseRoom_PwdTooLong" );
    }
    else if ( rRoleAmuseRoomCOM.AmuseRoom() != NULL )
    {
        strError.assign( "CreateAmuseRoom_InAmuseRoom" );
    }
    else if ( rRoleAmuseRoomCOM.StageRoomCOM()->IsInRoom())
    {
        strError.assign( "CreateAmuseRoom_InStageRoom" );
    }
    else if ( rRoleAmuseRoomCOM.CoupleCOM()->WeddingRoom() != NULL )
    {
        strError.assign( "CreateAmuseRoom_InWeddingRoom" );
    } 
    else if ( rRoleAmuseRoomCOM.PhotoRoomCOM()->PhotoRoom() != NULL ) 
    {
        strError.assign( "CreateAmuseRoom_InPhotoRoom" );
    }
    else if ( rRoleAmuseRoomCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        strError.assign("CreateAmuseRoom_InCeremony");
    }
    else
    {
        CAmuseRoomSceneConfig* pSceneInfo = ConfigManager::Instance().GetAmuseMgr().GetAmuseSceneInfo( rMsgRequest.m_chRoomScene );
        if ( pSceneInfo == NULL )
        {
            strError.assign( "CreateAmuseRoom_NoScene" );
        }
        else
        {
            unsigned int nRoomID = _GenerateAmuseRoomID();
            if ( nRoomID == 0 )
            {
                strError.assign( "CreateAmuseRoom_RoomEnough" );
            }
            else
            {
                std::string strRoomName( rMsgRequest.m_strRoomName );
                NameDeny::Instance().FilterInvalidText( strRoomName );

                CAmuseRoom* pAmuseRoom = new CAmuseRoom( nRoomID, rMsgRequest.m_chRoomTheme, rMsgRequest.m_chRoomScene, strRoomName, rMsgRequest.m_strRoomPwd, m_lMgrNowTime );
                pAmuseRoom->SetRoomCapacity(MAX_AMUSEROOM_PLAYER_COUNT);
                if( !_PlayerInAmuseRoom( rRoleAmuseRoomCOM, pAmuseRoom, true, eAmuseRoomEnter_Create ) )
                {
                    strError.assign( "CreateAmuseRoom_Unknow" );

                    _ReleaseAmuseRoomID( pAmuseRoom->RoomID() );
                    delete pAmuseRoom;
                    pAmuseRoom = NULL;
                }
                else
                {
                    AddAmuseRoom( pAmuseRoom );
                }
            }
        }
    }

    if ( !strError.empty() )
    {
        GameMsg_S2C_CreateAmuseRoomFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
    }
}

void CAmuseRoomManager::OnAmuseQuitRoom(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	std::string strError;

	CAmuseRoom* pAmuseRoom = rRoleAmuseRoomCOM.AmuseRoom();
	if ( pAmuseRoom == NULL )
	{
		strError.assign( "QuitAmuseRoom_NoRoom" );
	}
	else
    {
        GameMsg_C2S_QuitAmuseRoom & quitmsg = (GameMsg_C2S_QuitAmuseRoom&)rMsg;
        if (quitmsg.m_nQuitTo != eAmuseRoomQuitTo_Dungeon)
        {
            if (pAmuseRoom->IsConstellationRoom())
            {
                if (ConstellationMatchDataManager::Instance().IsOpen())
                {
                    quitmsg.m_nQuitTo = (int)eAmuseRoomQuitTo_ConstellationLobby;
                }
                else
                {
                    quitmsg.m_nQuitTo = (int)eAmuseRoomQuitTo_World;
                }
            }
            else if ( IsMeetAmuseSceneID( pAmuseRoom->RoomID() ))
            {
                quitmsg.m_nQuitTo = (int)eAmuseRoomQuitTo_World;
            }
            else
            {
                quitmsg.m_nQuitTo = (int)eAmuseRoomQuitTo_AmuseLobby;
            }
        }

		_PlayerOutAmuseRoom( rRoleAmuseRoomCOM, NULL, eAmuseRoomQuit_Active, (int)quitmsg.m_nQuitTo );
	}

	if ( !strError.empty() )
	{
		GameMsg_S2C_QuitAmuseRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CAmuseRoomManager::OnAmuseTryEnter(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	const GameMsg_C2S_TryEnterAmuseRoom& rMsgRequest = (const GameMsg_C2S_TryEnterAmuseRoom&)rMsg;
	std::string strError;

	CAmuseRoom* pAmuseRoom = FindAmuseRoom( rMsgRequest.m_nRoomID );
	if ( pAmuseRoom != NULL && pAmuseRoom->HasPwd() )
	{
		GameMsg_S2C_AmuseRoomRequriePwd msgResponse;
		msgResponse.m_nRoomID = pAmuseRoom->RoomID();

		rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
	}
	else
	{
		_AmuseEnterPlayer( rRoleAmuseRoomCOM, pAmuseRoom, "" );
	}
}

void CAmuseRoomManager::OnAmuseEnterPlayer(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	const GameMsg_C2S_EnterAmuseRoom& rMsgRequest = (const GameMsg_C2S_EnterAmuseRoom&)rMsg;

	CAmuseRoom* pAmuseRoom = FindAmuseRoom( rMsgRequest.m_nRoomID );
	_AmuseEnterPlayer( rRoleAmuseRoomCOM, pAmuseRoom, rMsgRequest.m_strRoomPwd );
}

void CAmuseRoomManager::EnterMeetAmuse( int nRoomID,  CAmuseRoomComponent& rRoleAmuseRoomCOM )
{
	CAmuseRoom* pAmuseRoom = FindMeetAmuseRoom( nRoomID );
	if ( pAmuseRoom != NULL )
	{
		std::string strError;

		if ( pAmuseRoom->PlayerCount() > 2 )
		{
			return;
		}

        if (rRoleAmuseRoomCOM.StageRoomCOM()->IsInRoom() || rRoleAmuseRoomCOM.CoupleCOM()->WeddingRoom() != NULL ||
			rRoleAmuseRoomCOM.PhotoRoomCOM()->PhotoRoom() != NULL || rRoleAmuseRoomCOM.AmuseRoom() != NULL 
            || rRoleAmuseRoomCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL)
		{
			WriteLog(LOGLEVEL_WARNING, "Player already in room, roleid=[%u]", rRoleAmuseRoomCOM.AttrCOM()->GetRoleID());
			return;
		}

		_PlayerInAmuseRoom( rRoleAmuseRoomCOM, pAmuseRoom, false, eAmuseRoomEnter_Join );	
	}
}

void CAmuseRoomManager::OnAmuseKickPlayer(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	std::string strError;

	CAmuseRoom* pAmuseRoom = rRoleAmuseRoomCOM.AmuseRoom();
	if ( pAmuseRoom == NULL )
	{
		strError.assign( "AmuseKickPlayer_NoRoom" );
	}
	else if ( !rRoleAmuseRoomCOM.IsHost() )
	{
		strError.assign( "AmuseKickPlayer_NoAuth" );
	} 
	else
	{
		const GameMsg_C2S_AmuseKickPlayer& rMsgRequest = (const GameMsg_C2S_AmuseKickPlayer&)rMsg;

		CAmuseRoomComponent* pOuterPlayer = pAmuseRoom->GetAmusePlayer( rMsgRequest.m_nPlayerID );
		if ( pOuterPlayer == NULL )
		{
			strError.assign( "AmuseKickPlayer_NoPlayer" );
		}
		else if ( pOuterPlayer->RoleID() == rRoleAmuseRoomCOM.RoleID() )
		{
			strError.assign( "AmuseKickPlayer_KickSelf" );
		}
		else 
		{
			_PlayerOutAmuseRoom( *pOuterPlayer, &rRoleAmuseRoomCOM, eAmuseRoomQuit_Kicked, eAmuseRoomQuitTo_AmuseLobby );
		}
	}

	if ( !strError.empty() )
	{
		GameMsg_S2C_AmuseKickPlayerFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CAmuseRoomManager::OnAmuseInvitePlayer(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	std::string strError;

	CAmuseRoom* pAmuseRoom = rRoleAmuseRoomCOM.AmuseRoom();
	if ( pAmuseRoom == NULL )
	{
		strError.assign( "AmuseInvitePlayer_NoRoom" );
	}
	else if ( !rRoleAmuseRoomCOM.IsHost() )
	{
		strError.assign( "AmuseInvitePlayer_NotHost" );
	}
    else if (rRoleAmuseRoomCOM.StageRoomCOM()->IsInRoom())
	{
		strError.assign( "AmuseInvitePlayer_InStageRoom" );
	}
	else if ( rRoleAmuseRoomCOM.CoupleCOM()->WeddingRoom() != NULL ) 
	{
		strError.assign( "AmuseInvitePlayer_InWeddingRoom" );
	}
	else if ( rRoleAmuseRoomCOM.PhotoRoomCOM()->PhotoRoom() != NULL )
	{
		strError.assign( "AmuseInvitePlayer_InPhotoRoom" );
	}
    else if ( rRoleAmuseRoomCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        strError.assign("AmuseInvitePlayer_InCeremony");
    }
	else if ( pAmuseRoom->IsConstellationRoom() ) 
	{
		strError.assign( "AmuseInvitePlayer_IsConstellationRoom" );
	}
	else
	{
		const GameMsg_C2S_AmuseInvitePlayer& rMsgRequest = (const GameMsg_C2S_AmuseInvitePlayer&)rMsg;

		if ( rMsgRequest.m_vecInviteeID.size() > AMUSEROOM_INVITE_COUNT )
		{
			strError.assign( "AmuseInvitePlayer_TooMany" );
		}
		else
		{
			unsigned long lNowTime = m_lMgrNowTime;
			if ( rRoleAmuseRoomCOM.LastInviteTime() > 0 && lNowTime - rRoleAmuseRoomCOM.LastInviteTime() < AMUSEROOM_INVITE_FREQUENCE )
			{
				strError.assign( "AmuseInvitePlayer_TooFrequent" );
			}
			else
			{
				rRoleAmuseRoomCOM.SetLastInviteTime( lNowTime );

				// notice self
				GameMsg_S2C_AmuseInvitePlayerSuc msgSelfResponse;
				rRoleAmuseRoomCOM.SendPlayerMsg( &msgSelfResponse );

				// notice others
				GameMsg_S2C_AmuseInviteeNotice msgOtherResponse;
				msgOtherResponse.m_nRoomID = pAmuseRoom->RoomID();
				msgOtherResponse.m_strInviterName = rRoleAmuseRoomCOM.AttrCOM()->GetRoleName();
				msgOtherResponse.m_nInviterRoleID = rRoleAmuseRoomCOM.AttrCOM()->GetRoleID();
				msgOtherResponse.m_bIsInviterVIP = rRoleAmuseRoomCOM.VipCOM()->IsVIP();
				msgOtherResponse.m_nInviterVIPLevel = (unsigned short)rRoleAmuseRoomCOM.VipCOM()->VIPLevel();

				for ( size_t i = 0; i < rMsgRequest.m_vecInviteeID.size(); ++i )
				{
					if ( rRoleAmuseRoomCOM.FriendCOM()->IsFriend( rMsgRequest.m_vecInviteeID[i] ) )
					{
						CRoleEntity *pInviteePlayer = CPlayerManager::Instance().GetEntityByRoleID( rMsgRequest.m_vecInviteeID[i] );	
						if ( pInviteePlayer != NULL )
						{
							CAmuseRoomComponent* pInviteeeAmuseRoom = pInviteePlayer->GetComponent<CRoleAmuseRoom>();
							if ( pInviteeeAmuseRoom->AmuseRoom() == NULL && pInviteeeAmuseRoom->RecvAmuseInvited( pAmuseRoom->RoomID(), pAmuseRoom->CreateTime() ) )
							{
								pInviteeeAmuseRoom->SendPlayerMsg( &msgOtherResponse );
							}
						}
					}
				}
			}
		}
	}

	if ( !strError.empty() )
	{
		GameMsg_S2C_AmuseInvitePlayerFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CAmuseRoomManager::OnAmuseInviteeResponse(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	const GameMsg_C2S_AmuseInviteeResponse& msgRequest = (const GameMsg_C2S_AmuseInviteeResponse&)rMsg;

	if ( msgRequest.m_bAccept )
	{
		std::string strError;

		if ( rRoleAmuseRoomCOM.AmuseRoom() != NULL )
		{
			strError.assign( "AmuseInviteeResponse_InAmuseRoom" );
		}
		else
		{
			CAmuseRoom* pAmuseRoom = FindAmuseRoom( msgRequest.m_nRoomID );
			if ( pAmuseRoom == NULL )
			{
				strError.assign( "AmuseInviteeResponse_NoRoom" );
			}
			else if ( pAmuseRoom->PlayerCount() >= MAX_AMUSEROOM_PLAYER_COUNT )
			{
				strError.assign( "AmuseInviteeResponse_PlayerFull" );
			}
            else if (rRoleAmuseRoomCOM.StageRoomCOM()->IsInRoom())
			{
				strError.assign( "AmuseInviteeResponse_InStageRoom" );
			}
			else if ( rRoleAmuseRoomCOM.CoupleCOM()->WeddingRoom() != NULL )
			{
				strError.assign( "AmuseInviteeResponse_InWeddingRoom" );
			}
			else if ( rRoleAmuseRoomCOM.PhotoRoomCOM()->PhotoRoom() != NULL ) 
			{
				strError.assign( "AmuseInviteeResponse_InPhotoRoom" );
			}
            else if ( rRoleAmuseRoomCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL)
            {
                strError.assign("AmuseInviteeResponse_InCeremony");
            }
			else if ( pAmuseRoom->IsConstellationRoom() ) 
			{
				strError.assign( "AmuseInviteeResponse_IsConstellationRoom" );
			}
			else
			{
				if ( !rRoleAmuseRoomCOM.IsAmuseInvited( pAmuseRoom->RoomID(), pAmuseRoom->CreateTime() ) )
				{
					strError.assign( "AmuseInviteeResponse_NoAuth" );
				}
				else
				{
					if ( !_PlayerInAmuseRoom( rRoleAmuseRoomCOM, pAmuseRoom, false, eAmuseRoomEnter_Invited ) )
					{
						strError.assign( "EnterAmuseRoom_Unknow" );
					}
				}
			}
		}

		if ( !strError.empty() )
		{
			GameMsg_S2C_AmuseInviteeFail msgResponse;
			msgResponse.m_strError = strError;
			rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
		}
	}

	rRoleAmuseRoomCOM.AmuseInvitedOver( msgRequest.m_nRoomID );
}

void CAmuseRoomManager::OnAmuseChangeRoomInfo(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
    const GameMsg_C2S_ChangeAmuseRoomInfo& rMsgRequest = (GameMsg_C2S_ChangeAmuseRoomInfo&)rMsg;
    std::string strError;

    CAmuseRoom* pAmuseRoom = rRoleAmuseRoomCOM.AmuseRoom();
    if ( pAmuseRoom == NULL )
    {
        strError.assign( "AmuseChangeRoomInfo_NoRoom" );
    }
    else if ( !rRoleAmuseRoomCOM.IsHost() )
    {
        strError.assign( "AmuseChangeRoomInfo_NotHost" );
    }
    else if ( rMsgRequest.m_strRoomName.empty() )
    {
        strError.assign( "AmuseChangeRoomInfo_EmptyName" );
    }
    else if ( rMsgRequest.m_strRoomName.length() > (size_t)g_ServerConfig.m_nMaxAmuseRoomNameLength )
    {
        strError.assign( "AmuseChangeRoomInfo_NameTooLong" );
    }
    else if ( rMsgRequest.m_strRoomPwd.length() > MAX_AMUSEROOM_PASSWD )
    {
        strError.assign( "AmuseChangeRoomInfo_PwdTooLong" );
    }
    else
    {
        std::string strRoomName( rMsgRequest.m_strRoomName );
        NameDeny::Instance().FilterInvalidText( strRoomName );

        pAmuseRoom->ChangeRoomInfo( strRoomName.c_str(), rMsgRequest.m_strRoomPwd.c_str() );

        // notice host
        GameMsg_S2C_ChangeAmuseRoomInfoSuc msgResponse;
        msgResponse.m_strRoomName.assign( pAmuseRoom->RoomName() );
        msgResponse.m_strRoomPwd.assign( pAmuseRoom->RoomPwd() );
        msgResponse.m_bHasPwd = pAmuseRoom->HasPwd();
        rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );

        // notice others
        msgResponse.m_strRoomPwd.assign( "" );
        pAmuseRoom->SendMsgToAll( &msgResponse, &rRoleAmuseRoomCOM );
    }

    if ( !strError.empty() )
    {
        GameMsg_S2C_ChangeAmuseRoomInfoFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
    }
}

void CAmuseRoomManager::OnAmusePromoteHost(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	GameMsg_C2S_AmusePromoteHost& rMsgRequest = (GameMsg_C2S_AmusePromoteHost&)rMsg;
	string strError;

	CAmuseRoom *pAmuseRoom = rRoleAmuseRoomCOM.AmuseRoom();
	if ( pAmuseRoom == NULL ) 
	{
		strError = "AmusePromoteHost_NoRoom";
	} 
	else if (!rRoleAmuseRoomCOM.IsHost()) 
	{
		strError = "AmusePromoteHost_NoAuth";
	}
	else
	{
		CAmuseRoomComponent* pNewHostPlayer = pAmuseRoom->GetAmusePlayer( rMsgRequest.m_nNewHostID );
		if ( pNewHostPlayer == NULL )
		{
			strError = "AmusePromoteHost_NoPlayer";
		}
		else
		{
			pAmuseRoom->AssignHost( pNewHostPlayer );
			_HandleAmuseTopRoom( pAmuseRoom );

			//new host
			GameMsg_S2C_AmusePromoteHostSuc msgResponse;
			msgResponse.m_nOldHostID = rRoleAmuseRoomCOM.RoleID();
			msgResponse.m_nNewHostID = pNewHostPlayer->RoleID();
			msgResponse.m_strRoomPwd = pAmuseRoom->RoomPwd();
			pNewHostPlayer->SendPlayerMsg( &msgResponse );

			//others
			msgResponse.m_strRoomPwd = "";
			pAmuseRoom->SendMsgToAll( &msgResponse, pNewHostPlayer );
		}
	}

	if ( !strError.empty() )
	{
		GameMsg_S2C_AmusePromoteHostFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CAmuseRoomManager::OnDoAmuseAction(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	GameMsg_C2S_DoAmuseAction& rMsgRequest = (GameMsg_C2S_DoAmuseAction&)rMsg;

	CAmuseRoom* pAmuseRoom = rRoleAmuseRoomCOM.AmuseRoom();
	if ( pAmuseRoom != NULL )
	{
		if ( rMsgRequest.m_pActionParam != NULL )
		{
			CAmuseAction_Base* pAmuseAction = NULL;

			switch ( rMsgRequest.m_pActionParam->m_nActionType )
			{
			case eAmuseAction_UseDevice:
				{
					pAmuseAction = new CAmuseAction_UseDevice( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_LeaveDevice:
				{
					pAmuseAction = new CAmuseAction_LeaveDevice( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_InviteDevice:
				{
					pAmuseAction = new CAmuseAction_InviteDevice( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_ResponseDevice:
				{
					pAmuseAction = new CAmuseAction_ResponseDevice( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_UsePose:
				{
					pAmuseAction = new CAmuseAction_UsePose( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_LeavePose:
				{
					pAmuseAction = new CAmuseAction_LeavePose( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_GetTimeEggGiftList:
				{
					pAmuseAction = new CAmuseAction_GetTimeEggGiftList( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_CommitTimeEggInfo:
				{
					pAmuseAction = new CAmuseAction_CommitTimeEggInfo( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_CreateTimeEgg:
				{
					pAmuseAction = new CAmuseAction_CreateTimeEgg( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			case eAmuseAction_OpenTimeEgg:
				{
					pAmuseAction = new CAmuseAction_OpenTimeEgg( &rRoleAmuseRoomCOM, pAmuseRoom->ActionID() );
				}
				break;
			}

			if ( pAmuseAction != NULL && pAmuseAction->FormParam( rMsgRequest.m_pActionParam ) )
			{
				rRoleAmuseRoomCOM.DoAmuseAction( pAmuseAction );
			}
		}
	}
}

void CAmuseRoomManager::OnAmusePlayerLogout(CAmuseRoomComponent& rRoleAmuseRoomCOM)
{
	_PlayerOutAmuseRoom( rRoleAmuseRoomCOM, NULL, eAmuseRoomQuit_Logout, eAmuseRoomQuitTo_World );

	for ( AmuseRoomMap::iterator it = m_AllAmuseRooms.begin(); it != m_AllAmuseRooms.end(); ++it )
	{
		CAmuseRoom *pAmuseRoom = it->second;
		if (pAmuseRoom != NULL)
		{
			CAmuseRoomComponent* pOuterPlayer = pAmuseRoom->PlayerOut( rRoleAmuseRoomCOM.RoleID() );
			if (pOuterPlayer != NULL)
			{
				WriteLog(LOGLEVEL_ERROR, "CRITICAL ERROR player in multi room, old room[%u|%s] !!!", 
					pAmuseRoom->RoomID(), pAmuseRoom->RoomName().c_str());
			}
		}
	}

	for ( AmuseRoomMap::iterator it = m_MeetAmuseRooms.begin(); it != m_MeetAmuseRooms.end(); ++it )
	{
		CAmuseRoom *pAmuseRoom = it->second;
		if (pAmuseRoom != NULL)
		{
			CAmuseRoomComponent* pOuterPlayer = pAmuseRoom->PlayerOut( rRoleAmuseRoomCOM.RoleID() );
			if (pOuterPlayer != NULL)
			{
				WriteLog(LOGLEVEL_ERROR, "CRITICAL ERROR player in meet room, old room[%u|%s] !!!", 
					pAmuseRoom->RoomID(), pAmuseRoom->RoomName().c_str());
			}
		}
	}
}

unsigned int CAmuseRoomManager::_GenerateAmuseRoomID()
{
	unsigned int nRoomID = 0;

	if ( !m_IdleID.empty() )
	{
		AmuseRoomIDList::iterator it = m_IdleID.begin();

		nRoomID = *it;
		m_IdleID.erase( it );
	}
	else if ( !m_StoreID.empty() )
	{
		AmuseRoomIDList::iterator it = m_StoreID.begin();

		nRoomID = *it;
		m_StoreID.erase( it );
	}

	return nRoomID;
}

void CAmuseRoomManager::_ReleaseAmuseRoomID(unsigned int nRoomID)
{	
	m_IdleID.push_back( nRoomID );
}

void CAmuseRoomManager::_HandleAmuseTopRoom(CAmuseRoom* pAmuseRoom)
{
	if ( pAmuseRoom != NULL )
	{
		AmuseRoomIDSet::iterator it_ID = m_TopAmuseRoomID.find( pAmuseRoom->RoomID() );
		if ( it_ID != m_TopAmuseRoomID.end() )
		{
			if ( !pAmuseRoom->IsTopRoom() )
			{
				m_TopAmuseRoomID.erase( it_ID );

				for ( AmuseRoomList::iterator it_Room = m_TopAmuseRooms.begin(); it_Room != m_TopAmuseRooms.end(); ++it_Room )
				{
					if ( *it_Room != NULL && (*it_Room)->RoomID() == pAmuseRoom->RoomID() )
					{
						m_TopAmuseRooms.erase( it_Room );
						break;
					}
				}
			}
		}
		else
		{
			if ( pAmuseRoom->IsTopRoom() && !pAmuseRoom->IsConstellationRoom())
			{
				m_TopAmuseRoomID.insert( pAmuseRoom->RoomID() );
				m_TopAmuseRooms.push_back( pAmuseRoom );
			}
		}
	}
}

void CAmuseRoomManager::_AmuseEnterPlayer(CAmuseRoomComponent& rRoleAmuseRoomCOM, CAmuseRoom *pAmuseRoom, const std::string& strRoomPwd)
{
	extern int g_nLine;

	std::string strError;

	if ( pAmuseRoom == NULL )
	{
		strError.assign( "EnterAmuseRoom_NoRoom" );
	}
	else if ( rRoleAmuseRoomCOM.AmuseRoom() != NULL )
	{
		strError.assign( "EnterAmuseRoom_InRoom" );
	}
	else if ( pAmuseRoom->PlayerCount() >= MAX_AMUSEROOM_PLAYER_COUNT )
	{
		strError.assign("EnterAmuseRoom_PlayerFull");
	}
	else if ( pAmuseRoom->HasPwd() && pAmuseRoom->RoomPwd().compare( strRoomPwd ) != 0 )
	{
		strError.assign( "EnterAmuseRoom_WrongPwd" );
	}
    else if (rRoleAmuseRoomCOM.StageRoomCOM()->IsInRoom())
	{
		strError.assign( "EnterAmuseRoom_InStageRoom" );
	}
	else if ( rRoleAmuseRoomCOM.CoupleCOM()->WeddingRoom() != NULL ) 
	{
		strError.assign("EnterAmuseRoom_InWeddingRoom");
	}
	else if ( rRoleAmuseRoomCOM.PhotoRoomCOM()->PhotoRoom() != NULL ) 
	{
		strError.assign("EnterAmuseRoom_InPhotoRoom");
	}
    else if ( rRoleAmuseRoomCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        strError.assign("EnterAmuseRoom_InCeremony");
    }
	else if ( pAmuseRoom->IsConstellationRoom() 
		&& (rRoleAmuseRoomCOM.ConstellationRoomID() != pAmuseRoom->RoomID()
		|| rRoleAmuseRoomCOM.ConstellationRoomCreateTime() != pAmuseRoom->CreateTime()
		|| rRoleAmuseRoomCOM.ConstellationRoomLine() != g_nLine))
	{
		strError.assign("EnterAmuseRoom_CantEnterConstellationRoom");
	}
	else
	{
		if ( !_PlayerInAmuseRoom( rRoleAmuseRoomCOM, pAmuseRoom, false, eAmuseRoomEnter_Join ) )
		{
			strError.assign( "EnterAmuseRoom_Unknow" );
		}
	}

	if ( !strError.empty() )
	{
		GameMsg_S2C_EnterAmuseRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
	}
}


bool CAmuseRoomManager::_PlayerInAmuseRoom(CAmuseRoomComponent& rRoleAmuseRoomCOM, CAmuseRoom* pAmuseRoom, bool bHost, int nInType)
{
	bool bRes = false;

	if ( pAmuseRoom != NULL )
	{
		if ( pAmuseRoom->PlayerIn( rRoleAmuseRoomCOM, bHost ) )
		{
			// notice self
			GameMsg_S2C_CreateAmuseRoomSuc msgSelfResponse;
			pAmuseRoom->EncodeWholeInfo( msgSelfResponse.m_RoomWholeInfo, bHost );
			rRoleAmuseRoomCOM.SendPlayerMsg( &msgSelfResponse );

			// notice others
			if ( !bHost )
			{
				GameMsg_S2C_AmuseEnterPlayer msgOtherResponse;
				rRoleAmuseRoomCOM.EncodePlayerInfo( msgOtherResponse.m_PlayerInfo );
				pAmuseRoom->SendMsgToAll( &msgOtherResponse, &rRoleAmuseRoomCOM );
			}

			if (pAmuseRoom->IsConstellationRoom() && _IsInConstellationRoomEndingList(pAmuseRoom->RoomID()))
			{
				_RemoveConstellationRoomEndingList(pAmuseRoom->RoomID());
			}
			bRes = true;
		}
		else
		{
			WriteLog(LOGLEVEL_ERROR, "Player in amuse room error!" );
		}
	}

	return bRes;
}

void CAmuseRoomManager::_PlayerOutAmuseRoom(CAmuseRoomComponent& rOuterAmuseRoomCOM, CAmuseRoomComponent* pKickerAmuseRoomCOM, int nOutType, int nQuitTo)
{
	CAmuseRoom* pAmuseRoom = rOuterAmuseRoomCOM.AmuseRoom();
	if ( pAmuseRoom != NULL )
	{
		CAmuseRoomComponent* pOuterPlayer = pAmuseRoom->PlayerOut( rOuterAmuseRoomCOM.RoleID() );
		if ( pOuterPlayer != NULL )
		{
			// notice player
			if ( nOutType != eAmuseRoomQuit_Logout )
			{
				GameMsg_S2C_QuitAmuseRoomSuc msgPlayerResponse;
				msgPlayerResponse.m_chQuitType = (char)nOutType;
				msgPlayerResponse.m_nQuitTo = (unsigned char)nQuitTo;
				
				if ( nOutType == eAmuseRoomQuit_Kicked && pKickerAmuseRoomCOM != NULL )
				{
					msgPlayerResponse.m_nKickerID = pKickerAmuseRoomCOM->AttrCOM()->GetRoleID();
				}

				pOuterPlayer->SendPlayerMsg( &msgPlayerResponse );
			}

			// check whether to destroy room
			if ( pAmuseRoom->PlayerCount() > 0 )
			{
				//check whether to reassign host
				CAmuseRoomComponent* pNewHostPlayer = NULL;
				if ( pAmuseRoom->RoomHost() == NULL )
				{
					pNewHostPlayer = pAmuseRoom->AllocHost();
					if ( pNewHostPlayer != NULL )
					{
						GameMsg_S2C_AmuseQuitPlayer msgHostResponse;
						msgHostResponse.m_chQuitType = (char)nOutType;
						msgHostResponse.m_nQuitID = pOuterPlayer->AttrCOM()->GetRoleID();

						if ( IsMeetAmuseSceneID( pAmuseRoom->RoomID()) )
						{
							CMeetActivityMgr::Instance().LeaveAmuseScene( rOuterAmuseRoomCOM.RoleID() );
						}
						else if ( !IsConstellationRoom( pAmuseRoom->RoomID()) )
						{
							msgHostResponse.m_nNewHostID = pNewHostPlayer->AttrCOM()->GetRoleID();
						}

						msgHostResponse.m_strRoomPwd.assign( pAmuseRoom->RoomPwd() );
						pNewHostPlayer->SendPlayerMsg( &msgHostResponse );
					}

					_HandleAmuseTopRoom( pAmuseRoom );
				}

				// notice others
				GameMsg_S2C_AmuseQuitPlayer msgOtherResponse;
				msgOtherResponse.m_chQuitType = (char)nOutType;
				msgOtherResponse.m_nQuitID = pOuterPlayer->AttrCOM()->GetRoleID();
				msgOtherResponse.m_nNewHostID = ( pNewHostPlayer != NULL ? pNewHostPlayer->AttrCOM()->GetRoleID() : 0 );
				msgOtherResponse.m_strRoomPwd.assign( "" );

				pAmuseRoom->SendMsgToAll( &msgOtherResponse, pNewHostPlayer );
			}
			else
			{
				if (pAmuseRoom->IsConstellationRoom())
				{
					if (_IsInConstellationRoomEndingList(pAmuseRoom->RoomID()))
					{
						_RemoveConstellationRoomEndingList(pAmuseRoom->RoomID());
					}

					unsigned long nEndTime = m_lMgrNowTime + ConstellationMatchDataManager::Instance().m_nRoomCloseTime * 1000;
					m_EndConstellationRooms.insert(std::make_pair(nEndTime, pAmuseRoom));
					_AddConstellationRoomEndingList(pAmuseRoom->RoomID(), nEndTime);
				}
				else if ( IsMeetAmuseSceneID( pAmuseRoom->RoomID()))
				{
					CMeetActivityMgr::Instance().LeaveAmuseScene( rOuterAmuseRoomCOM.RoleID() );
				}
				else
				{
					DestroyAmuseRoom( pAmuseRoom->RoomID() );
				}
			}
		}
		else
		{
			WriteLog(LOGLEVEL_ERROR, "Player quit amuse room error!" );
		}
	}
}

bool CAmuseRoomManager::_IsInConstellationRoomEndingList(unsigned int nRoomID) const
{
	return (m_ConstellationRoomEndID.find(nRoomID) != m_ConstellationRoomEndID.end());
}

void CAmuseRoomManager::_AddConstellationRoomEndingList(unsigned int nRoomID, int nTime)
{
	m_ConstellationRoomEndID.insert(std::make_pair(nRoomID, nTime));
}

void CAmuseRoomManager::_RemoveConstellationRoomEndingList(unsigned int nRoomID)
{
	m_ConstellationRoomEndID.erase(nRoomID);
	AmuseRoomTimeMap::iterator it = m_EndConstellationRooms.begin();
	for (; it != m_EndConstellationRooms.end();)
	{
		CAmuseRoom *pRoom = it->second;
		if (pRoom != NULL && pRoom->RoomID() == nRoomID)
		{
			m_EndConstellationRooms.erase(it++);
			continue;
		}
		else
		{
			++it;
		}
	}
}

void CAmuseRoomManager::CreateTestAmuseRoom()
{
	for ( int i = 1; i <= AMUSEROOM_TEST_COUNT; ++i )
	{
		std::string strRoomName;
		int nRoomID = _GenerateAmuseRoomID();

		if ( nRoomID > 0 )
		{
			Formatter(strRoomName) << "AmuseTest" << nRoomID;
			CAmuseRoom* pAmuseRoom = new CAmuseRoom( nRoomID, eAmuseTheme_None, 1, strRoomName, "123456" , m_lMgrNowTime );
			pAmuseRoom->SetRoomCapacity(MAX_AMUSEROOM_PLAYER_COUNT);

			CAmuseRoomComponent* pHostPlayer = new CRoleAmuseRoom();
			pHostPlayer->SetHost(true);
			pHostPlayer->SetRoomColor(RoomColor_SkyBlue + i % 2);
			unsigned int nRand = Random(0, 10);
			pHostPlayer->SetRoomTop(nRand == 1 ? true : false);
			pAmuseRoom->AssignHost( pHostPlayer );

			AddAmuseRoom( pAmuseRoom );
		}
	}

	WriteLog(LOGLEVEL_DEBUG, "共产生房间[%d]个\n", m_AllAmuseRooms.size() );
	WriteLog(LOGLEVEL_DEBUG, "共产生置顶房间[%d]个\n", m_TopAmuseRoomID.size() );
}

void CAmuseRoomManager::CreateTestAmuseRoom(int nSceneID, int nRoomCount, CAmuseRoomComponent *pHost)
{
	std::string strRoomName;
	for ( int i = 0; i < nRoomCount; ++i )
	{
		int nRoomID = _GenerateAmuseRoomID();
		if ( nRoomID > 0 )
		{
			Formatter::Format( strRoomName, "AmuseTest%02d-%03d-%03d", nSceneID, nRoomCount, nRoomID);
			CAmuseRoom* pAmuseRoom = new CAmuseRoom( nRoomID, eAmuseTheme_None, nSceneID, strRoomName, "123456" , m_lMgrNowTime );
			pAmuseRoom->SetRoomCapacity(MAX_AMUSEROOM_PLAYER_COUNT);

			AddAmuseRoom( pAmuseRoom );
		}
	}

	WriteLog(LOGLEVEL_DEBUG, "共创建测试开发性场景房间[%d]个\n", m_AllAmuseRooms.size() );
}

bool CAmuseRoomManager::_ApplyMatchRoom( CAmuseRoomComponent& rRoleAmuseRoomCOM, bool bCreateRoom, const std::string& strRoomName, char chRoomTheme, char chRoomScene )
{
	std::string strError;
	if ( rRoleAmuseRoomCOM.AmuseRoom() != NULL )
	{
		strError.assign( "EnterAmuseRoom_InRoom" );
	}
    else if (rRoleAmuseRoomCOM.StageRoomCOM()->IsInRoom())
	{
		strError.assign( "EnterAmuseRoom_InStageRoom" );
	}
	else if ( rRoleAmuseRoomCOM.CoupleCOM()->WeddingRoom() != NULL ) 
	{
		strError.assign("EnterAmuseRoom_InWeddingRoom");
	}
	else if ( rRoleAmuseRoomCOM.PhotoRoomCOM()->PhotoRoom() != NULL ) 
	{
		strError.assign("EnterAmuseRoom_InPhotoRoom");
	}
    else if ( rRoleAmuseRoomCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        strError.assign("EnterAmuseRoom_InCeremony");
    }

	if (strError.empty())
	{
		CAmuseRoom* pAmuseRoomMatched = NULL;
		for (AmuseRoomMap::iterator it = m_AllAmuseRooms.begin(); it != m_AllAmuseRooms.end(); ++it)
		{
			CAmuseRoom* pAmuseRoom = it->second;
			if (pAmuseRoom->PlayerCount() < MAX_AMUSEROOM_PLAYER_COUNT 
				&& !pAmuseRoom->HasPwd() 
				&& pAmuseRoom->RoomScene() == chRoomScene
				&& !pAmuseRoom->IsConstellationRoom())
			{
				if ( !_PlayerInAmuseRoom( rRoleAmuseRoomCOM, pAmuseRoom, false, eAmuseRoomEnter_Join ) )
				{
					continue;
				}
				else
				{
					pAmuseRoomMatched = pAmuseRoom;
					break;
				}
			}
		}
		if (pAmuseRoomMatched == NULL)
		{
			if (bCreateRoom)
			{
				GameMsg_C2S_CreateAmuseRoom tmpmsg;
				tmpmsg.m_strRoomName = strRoomName;
				tmpmsg.m_chRoomTheme = chRoomTheme;
				tmpmsg.m_chRoomScene = chRoomScene;
				OnAmuseCreateRoom(tmpmsg, rRoleAmuseRoomCOM);
				pAmuseRoomMatched = rRoleAmuseRoomCOM.AmuseRoom();
			}
		}
		if (pAmuseRoomMatched == NULL)
		{
			strError.assign( "EnterAmuseRoom_NoRoom" );
		}
	}
	if (!strError.empty())
	{
		GameMsg_S2C_EnterAmuseRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleAmuseRoomCOM.SendPlayerMsg( &msgResponse );
	}
	return strError.empty();
}

void CAmuseRoomManager::OnAmuseApplyMatch( const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM )
{
	const GameMsg_C2S_AmuseApplyMatch& rMsgRequest = (const GameMsg_C2S_AmuseApplyMatch&)rMsg;
	_ApplyMatchRoom(rRoleAmuseRoomCOM, rMsgRequest.m_bCreate, rMsgRequest.m_strRoomName, rMsgRequest.m_chRoomTheme, rMsgRequest.m_chRoomScene);
}

bool CAmuseRoomManager::IsConstellationRoom(unsigned int nRoomID) const
{
	AmuseRoomIDSet::const_iterator it = m_ConstellationRoomID.find(nRoomID);
	if (it == m_ConstellationRoomID.end())
	{
		return false;
	}
	return true;
}

bool CAmuseRoomManager::CreateConstellationRoom(CAmuseRoomComponent& rFirstRoleCOM, CAmuseRoomComponent& rSecondRoleCOM, int nSceneID)
{
	extern int g_nLine;

	const char *strError = NULL;

	if ( rFirstRoleCOM.AmuseRoom() != NULL || rSecondRoleCOM.AmuseRoom() != NULL )
	{
		strError = "CreateAmuseRoom_InAmuseRoom";
	}
    else if (rFirstRoleCOM.StageRoomCOM()->IsInRoom() || rSecondRoleCOM.StageRoomCOM()->IsInRoom())
	{
		strError = "CreateAmuseRoom_InStageRoom";
	}
	else if ( rFirstRoleCOM.CoupleCOM()->WeddingRoom() != NULL || rSecondRoleCOM.CoupleCOM()->WeddingRoom() != NULL )
	{
		strError = "CreateAmuseRoom_InWeddingRoom";
	} 
	else if ( rFirstRoleCOM.PhotoRoomCOM()->PhotoRoom() != NULL || rSecondRoleCOM.PhotoRoomCOM()->PhotoRoom() != NULL ) 
	{
		strError = "CreateAmuseRoom_InPhotoRoom";
	}
    else if ( rFirstRoleCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL || rSecondRoleCOM.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        strError = ("CreateAmuseRoom_InCeremony");
    }
	else
	{
		CAmuseRoomSceneConfig* pSceneInfo = ConfigManager::Instance().GetAmuseMgr().GetAmuseSceneInfo( nSceneID );
		if ( pSceneInfo == NULL )
		{
			strError = "CreateAmuseRoom_NoScene";
		}
		else
		{
			unsigned int nRoomID = _GenerateAmuseRoomID();
			if ( nRoomID == 0 )
			{
				strError = "CreateAmuseRoom_RoomEnough" ;
			}
			else
			{
				std::string strRoomName;
				SafeFormat(strRoomName, CLocalization::Instance().GetString( "Constellation_Room_Name" ), nRoomID);

				unsigned char chRoomTheme = eAmuseTheme_None;
				CAmuseRoom* pAmuseRoom = new CAmuseRoom( nRoomID, chRoomTheme, nSceneID, strRoomName, "", m_lMgrNowTime );
				pAmuseRoom->SetRoomCapacity(MAX_CONSTELLATION_ROOM_PLAYER_COUNT);

				m_ConstellationRoomID.insert( nRoomID );
				AddAmuseRoom( pAmuseRoom );

				unsigned long nEndTime = m_lMgrNowTime + ConstellationMatchDataManager::Instance().m_nRoomCloseTime * 1000;
				m_EndConstellationRooms.insert(std::make_pair(nEndTime, pAmuseRoom));

				_AddConstellationRoomEndingList(nRoomID, nEndTime);

				rFirstRoleCOM.SetConstellationRoom(nRoomID, m_lMgrNowTime, g_nLine);
				rSecondRoleCOM.SetConstellationRoom(nRoomID, m_lMgrNowTime, g_nLine);
			}
		}
	}

	if ( strError != NULL )
	{
		GameMsg_S2C_CreateAmuseRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rFirstRoleCOM.SendPlayerMsg( &msgResponse );
		rSecondRoleCOM.SendPlayerMsg( &msgResponse );

		return false;
	}

	return true;
}

void CAmuseRoomManager::OnEnterConstellationRoom(CAmuseRoomComponent &rComponent)
{
	extern int g_nLine;

	const char *strError = NULL;
	CAmuseRoom* pAmuseRoom = CAmuseRoomManager::Instance().FindAmuseRoom( rComponent.ConstellationRoomID() );

	if (rComponent.ConstellationRoomID() == 0)
	{
		strError = "EnterAmuseRoom_NoConstellationRoom";
	}
	else if (rComponent.ConstellationRoomLine() != g_nLine)
	{
		strError = "EnterAmuseRoom_NotMatchLine";
	}
	else if (pAmuseRoom == NULL)
	{
		strError = "EnterAmuseRoom_NoRoom";
	}
	else if (!pAmuseRoom->IsConstellationRoom()) 
	{
		strError = "EnterAmuseRoom_NotConstellationRoom";
	}
	else if (rComponent.ConstellationRoomID() != pAmuseRoom->RoomID()
		|| rComponent.ConstellationRoomCreateTime() != pAmuseRoom->CreateTime())
	{
		strError = "EnterAmuseRoom_RoomExpired";
	}

	if (strError != NULL)
	{
		GameMsg_S2C_EnterAmuseRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rComponent.SendPlayerMsg( &msgResponse );
	}
	else
	{
		GameMsg_C2S_TryEnterAmuseRoom tryMsg;
		tryMsg.m_nRoomID = rComponent.ConstellationRoomID();
		CAmuseRoomManager::Instance().OnAmuseTryEnter(tryMsg, rComponent);

		CAmuseRoomSceneConfig* pSceneInfo = ConfigManager::Instance().GetAmuseMgr().GetAmuseSceneInfo( pAmuseRoom->RoomScene() );
		if ( pSceneInfo == NULL )
		{
			WriteLog(LOGLEVEL_ERROR, "ERROR: invalid amuse scene id=%d no scene!", pAmuseRoom->RoomScene());
			return;
		}
		LogRoleEnterRoom(rComponent.AttrCOM()->GetRoleID(), rComponent.AttrCOM()->GetRoleName(), pSceneInfo->m_nSceneID, pSceneInfo->m_strSceneName);
	}
}

void CAmuseRoomManager::LogRoleEnterRoom(unsigned int nRoleID, const std::string &strRoleName, int nSceneID, const std::string &strSceneName)
{
	CRoleConstellationMatchLog *pLog = new CRoleConstellationMatchLog;
	pLog->m_nRoleID = nRoleID;
	pLog->m_strRoleName = strRoleName;
	pLog->m_nSceneID = nSceneID;
	pLog->m_strSceneName = strSceneName;
	pLog->m_eAction = CRoleConstellationMatchLog::EConstellationMatchAction_EnterRoom;

	g_pQueryMgr->AddQuery(QUERY_LogRoleConstellationMatch, 0, pLog);
}

void CAmuseRoomManager::GetRoomListByGM(std::vector<GMRoomInfo>& roomList)
{
	AmuseRoomMap::iterator it = m_AllAmuseRooms.begin();
	for (;it != m_AllAmuseRooms.end() ; it++)
	{
		GMRoomInfo roomInfo;
		it->second->ToGMRoomInfo(roomInfo);
		roomList.push_back(roomInfo);
	}
}

