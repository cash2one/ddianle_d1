#include "LiveRoomManager.h"
#include "RoomComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../player/PlayerInfoComponent.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../chat/ChatManager.h"
#include "LiveRoomDataMgr.h"
#include "../grade/GradeInfoMgr.h"
#include "../Location/LocationMgr.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/DanceGroupDataMgr.h"
#include "../../socket/Md5.h"
#include "../../socket/Formatter.h"
#include "../logic/LogicCirculator.h"
#include "../logic/PlayerManager.h"
#include "../logic/LogicCirculator.h"
#include "../music/MusicStageFileManager.h"

#include "../roomLib/GameMsg_C2S_RoomMsg.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/RoomSettingMgr.h"

#include "GameMsg_C2S_LiveRoomMsg.h"
#include "GameMsg_S2C_LiveRoomMsg.h"
#include "../player/GameMsg_S2G_UpdateRoomInfo.h"
#include "RoomManager.h"
#include "../util/NameDeny.h"

#include <stdio.h>
#include <fstream>
#include <algorithm>

extern CLogicCirculator* g_pCirculator;
extern string g_sPath;
extern int  g_nLine;


#define	ROOMCOUNT_PER_PAGE		6		//客户端每一页显示的房间个数
#define INVITEELIST_INTERVAL	3000	//获取邀请列表时间间隔:ms
#define INVITEPLAYER_INTERVAL	3000	//邀请玩家时间间隔:ms
#define INVITEPLAYER_COUNT		3		//邀请玩家数量限制
#define MAX_INVITEE_ARRAY_NUM   2048	//最大邀请数组	
#define MAX_INVITEE_COUNT       20      //最大邀请数


extern CQueryManager* g_pQueryMgr;

extern void SendMsg2GroupServer(LPGameMsg_Base pMsg);


static const unsigned int kMaxLiveRoomID = 9999;
static const unsigned int kMaxManagerRoomID = 99999;

CLiveRoomManager::CLiveRoomManager()
: GameMsg_Processor(true, true)
, m_bInited(false), m_nMgrNowTime(0), m_nMatchTime(0), m_nLastSyncVoteTime(0)
{
	for (unsigned int i = 1; i < kMaxLiveRoomID; i++ )
	{
		m_IdleLiveRoomID.push_back( i );
	}

	for (unsigned int j = kMaxLiveRoomID; j < kMaxManagerRoomID; j++ )
	{
		m_IdleAuditionRoomID.push_back( j );
	}
}

CLiveRoomManager::~CLiveRoomManager()
{
	for ( AllRoomMap::iterator it = m_AllRooms[MusicMode_None].begin(); it != m_AllRooms[MusicMode_None].end(); ++it )
	{
		delete it->second;
	}

	for (int nMode = MusicMode_None; nMode < MusicMode_Max; ++nMode )
	{
		m_AllRooms[nMode].clear();
	}
}

CLiveRoomManager& CLiveRoomManager::Instance()
{
	static CLiveRoomManager stcRoomMgr;
	if (!stcRoomMgr.m_bInited)
	{
		stcRoomMgr.Init();
	}
	return stcRoomMgr;
}

void CLiveRoomManager::Init()
{
	RegNetMessage();
	m_bInited = true;
}

void CLiveRoomManager::RegNetMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_S2C_GetLiveRoomHotListRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2C_GetLiveRoomAuditionScoreListRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2C_GetMyAuditionScore);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_IsPunisherPlayer);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_SyncAuditionScore);

	RegMsgProcessor(MSG_S2C_GetLiveRoomHotListRes, &CLiveRoomManager::OnGetLiveRoomHotListRes);
	RegMsgProcessor(MSG_S2C_GetLiveRoomAuditionScoreListRes, &CLiveRoomManager::OnGetLiveRoomAuditionScoreListRes);
	RegMsgProcessor(MSG_S2C_GetMyAuditionScore, &CLiveRoomManager::OnGetMyAuditionScoreRes);

    RegMsgProcessor(MSG_G2S_LiveRoom_IsPunishedPlayer, &CLiveRoomManager::_OnLiveRoomJoinAudition);
    RegMsgProcessor(MSG_G2S_LiveRoom_SyncAuditionScore, &CLiveRoomManager::_OnSyncAuditionScore);

	RegQueryProcessor(Query_LiveRoom_GetDancerName, &CLiveRoomManager::OnDBGetDancerName);
	RegQueryProcessor(Query_LiveRoom_UpdateDancerName, &CLiveRoomManager::OnDBUpdateDancerName);
}

void CLiveRoomManager::OnDBGetDancerName(QueryBase &query)
{
	std::string strError;

	unsigned int nGMRoleID = (unsigned int)query.m_nAttachment;
	CLiveRoomDancerDB *pDancer = (CLiveRoomDancerDB *)query.m_pAttachment;
	ERoomType eType = pDancer->m_bIsDanceGroup ? eRoomType_LiveRoomGroup : eRoomType_LiveRoomPlayer;
	CRoomComponent *pRoleRoomCOM = NULL;

	unsigned int nRoomID = _GenerateRoomID(eType);
	if ( nRoomID == 0 )
	{
		strError.assign( "CreateLiveRoom_RoomEnough" );
	}
	else
	{
		bool isDancerExist = true;
		for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
		{
			if (pDancer->m_nDancerID[i] > 0 && pDancer->m_strDancerName[i].empty())
			{
				isDancerExist = false;
				break;
			}
		}
		if (!isDancerExist)
		{
			strError.assign( "CreateLiveRoom_DancerNotExist" );
		}
		else
		{
			std::string strRoomName;
			SafeFormat(strRoomName, CLocalization::Instance().GetString("Live_Room_Name", g_ServerConfig.m_nMaxRoomNameLength), nRoomID );
			NameDeny::Instance().FilterInvalidText(strRoomName);

			CRoom* pRoom = new CRoom(eType, nRoomID);
			CheckDestroyRoom( pRoom );
			pRoom->ChangeRoomInfo(strRoomName.c_str(), "", "");

			if (pDancer->m_bIsDanceGroup)
			{
				for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
				{
					pRoom->SetLiveRoomGroup(i, pDancer->m_nDancerID[i], pDancer->m_strDancerName[i]);
				}
			}
			else
			{
				for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
				{
					pRoom->SetLiveRoomDancer(i, pDancer->m_nDancerID[i], pDancer->m_strDancerName[i]);
				}
			}

			CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(nGMRoleID);
			if (pRole != NULL)
			{
				pRoleRoomCOM = pRole->GetComponent<CRoleRoom>();
			}
			if (pRoleRoomCOM != NULL)
			{
				if( _PlayerInRoom( *pRoleRoomCOM, pRoom, RoleRoomType_GM, RoleRoomState_ToStart, false, RoomEnterType_Create ) < 0 )
				{
					strError.assign( "CreateLiveRoom_Unknow" );

					_ReleaseRoomID( pRoom->RoomID() );
					delete pRoom;
					pRoom = NULL;
				}
			}
			if (pRoom != NULL)
			{
				AddRoom( pRoom );
			}
		}
	}

	delete (CLiveRoomDancerDB *)query.m_pAttachment, query.m_pAttachment = NULL;

	if (!strError.empty() && pRoleRoomCOM != NULL)
	{
		GameMsg_S2C_CreateLiveRoomFail msgResponse;
		msgResponse.m_strError = strError;
		pRoleRoomCOM->SendPlayerMsg( &msgResponse );
	}
}

void CLiveRoomManager::OnDBUpdateDancerName(QueryBase &query)
{
	std::string strError;
	unsigned int nGMRoleID = (unsigned int)query.m_nAttachment;
	CLiveRoomDancerDB *pDancer = (CLiveRoomDancerDB *)query.m_pAttachment;

	CRoomComponent *pRoleRoomCOM = NULL;

	CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(nGMRoleID);
	if (pRole != NULL)
	{
		pRoleRoomCOM = pRole->GetComponent<CRoleRoom>();
	}

	bool isDancerExist = true;
	for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
	{
		if (pDancer->m_nDancerID[i] > 0 && pDancer->m_strDancerName[i].empty())
		{
			isDancerExist = false;
			break;
		}
	}
	if (!isDancerExist)
	{
		strError.assign( "UpdateLiveRoomDancer_DancerNotExist" );
	}
	else
	{
		if (pRoleRoomCOM != NULL)
		{
			CRoom* pRoom = pRoleRoomCOM->Room();
			if (pRoom != NULL)
			{
				for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
				{
					pRoom->SetLiveRoomDancer(i, pDancer->m_nDancerID[i], pDancer->m_strDancerName[i]);
				}

				// dancer
				for ( int nPos = 0; strError.empty() && nPos <= pRoom->MaxDancerPos(); ++nPos )
				{
					CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
					if ( pRoomCOM != NULL && !pRoom->IsLiveRoomDancer(pRoomCOM->AttrCOM()->GetRoleID()) )
					{
						if (_PlayerChangeType(*pRoomCOM, RoleRoomType_Audience) < 0)
						{
							strError = "SetLiveRoomDancer_Unknown";
							break;
						}
					}
				}

				// gm
				for ( int nPos = 0; strError.empty() && nPos <= pRoom->MaxGMPos(); ++nPos )
				{
					CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_GM, nPos );
					if ( pRoomCOM != NULL && pRoom->IsLiveRoomDancer(pRoomCOM->AttrCOM()->GetRoleID()) )
					{
						if (_PlayerChangeType(*pRoomCOM, RoleRoomType_Dancer) < 0)
						{
							strError = "SetLiveRoomDancer_Unknown";
							break;
						}
					}
				}

				// audience
				std::map<unsigned int, CRoomComponent*> audienceMap = pRoom->LiveRoomAudicenceMap();
				for ( std::map<unsigned int, CRoomComponent*>::iterator it = audienceMap.begin();
					strError.empty() && it != audienceMap.end(); ++it )
				{
					CRoomComponent* pRoomCOM = it->second;
					if ( pRoomCOM != NULL && pRoom->IsLiveRoomDancer(pRoomCOM->AttrCOM()->GetRoleID()) )
					{
						if (_PlayerChangeType(*pRoomCOM, RoleRoomType_Dancer) < 0)
						{
							strError = "SetLiveRoomDancer_Unknown";
							break;
						}
					}
				}

				if (strError.empty())
				{
					_NoticePlayerChange(pRoom);
				}
			}
		}
	}

	delete (CLiveRoomDancerDB *)query.m_pAttachment, query.m_pAttachment = NULL;

	if (!strError.empty() && pRoleRoomCOM != NULL)
	{
		GameMsg_S2C_UpdateLiveRoomDancerFail msgResponse;
		msgResponse.m_strError = strError;
		pRoleRoomCOM->SendPlayerMsg( &msgResponse );
	}
}

CRoom* CLiveRoomManager::FindRoom(unsigned int nRoomID)
{
	AllRoomMap::iterator it = m_AllRooms[MusicMode_None].find( nRoomID );
	if( it != m_AllRooms[MusicMode_None].end() && it->second != NULL )
	{
		return it->second;
	}

	return NULL;
}

void CLiveRoomManager::DestroyRoom(bool bOldTopState, unsigned int nRoomID, int nFlag)
{
	CRoom* pRoom = RemoveRoom( nRoomID );
	if ( pRoom != NULL )
	{
		std::set<CRoom*>::iterator itPointer = m_setDestroyedRoom.find( pRoom );
		if ( itPointer != m_setDestroyedRoom.end() )
		{
			WriteLog(LOGLEVEL_ERROR, "CLiveRoomManager::DestroyRoom. delete pRoom double,RoomID=%d",nRoomID);
			return;
		}

		for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
		{
			CRoomComponent* pRoomCOM = pRoom->PlayerOut( RoleRoomType_Dancer, nPos );
			if ( pRoomCOM != NULL )
			{
				pRoomCOM->OutRoom();
				if ( pRoomCOM->AttrCOM() != NULL )
				{
					m_LobbyPlayer.insert( std::make_pair( pRoomCOM->AttrCOM()->GetRoleID(), pRoomCOM ) );
					CLocationMgr::Instance().EnterZone( Zone_LiveLobby, pRoomCOM->AttrCOM()->GetRoleID());
					_NoticeGroupState( pRoomCOM->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None );

					if (pRoomCOM->FriendCOM() != NULL) 
                    {
						pRoomCOM->FriendCOM()->PushOnlineInfoToAllFriends();
					}
				}

				// notice self
				GameMsg_S2C_QuitLiveRoomSuc msgSelfResponse;
				msgSelfResponse.m_chRoomRoleType = RoleRoomType_Dancer;
				msgSelfResponse.m_chRoomRolePos = (char)nPos;
				msgSelfResponse.m_nRoleID = pRoomCOM->AttrCOM()->GetRoleID();
				if (pRoom->IsLiveRoom())
				{
					msgSelfResponse.m_chQuitType = RoomQuitType_NoRoom;
				}
				else
				{
					msgSelfResponse.m_chQuitType = RoomQuitType_Active;
				}
				msgSelfResponse.m_chQuitTo = (char)RoomQuitTo_LiveLobby;
				pRoomCOM->SendPlayerMsg( &msgSelfResponse );
			}
		}

		std::map<unsigned int, CRoomComponent*> audienceMap = pRoom->LiveRoomAudicenceMap();
		for ( std::map<unsigned int, CRoomComponent*>::iterator it = audienceMap.begin();
			it != audienceMap.end(); ++it )
		{
			unsigned int nRoleID = it->first;
			CRoomComponent* pRoomCOM = pRoom->PlayerOut( RoleRoomType_Audience, nRoleID );
			if ( pRoomCOM != NULL )
			{
				pRoomCOM->OutRoom();

				if ( pRoomCOM->AttrCOM() != NULL )
				{
					m_LobbyPlayer.insert( std::make_pair( pRoomCOM->AttrCOM()->GetRoleID(), pRoomCOM ) );
					CLocationMgr::Instance().EnterZone( Zone_LiveLobby, pRoomCOM->AttrCOM()->GetRoleID());
					_NoticeGroupState( pRoomCOM->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None );
					if (pRoomCOM->FriendCOM() != NULL) {
						pRoomCOM->FriendCOM()->PushOnlineInfoToAllFriends();
					}
				}

				// notice self
				GameMsg_S2C_QuitLiveRoomSuc msgSelfResponse;
				msgSelfResponse.m_chRoomRoleType = RoleRoomType_Audience;
				msgSelfResponse.m_chRoomRolePos = 0;
				msgSelfResponse.m_nRoleID = pRoomCOM->AttrCOM()->GetRoleID();
				msgSelfResponse.m_chQuitType = RoomQuitType_NoRoom;
				msgSelfResponse.m_chQuitTo = (char)RoomQuitTo_LiveLobby;
				pRoomCOM->SendPlayerMsg( &msgSelfResponse );
			}
		}

		for ( int nPos = 0; nPos <= pRoom->MaxGMPos(); ++nPos )
		{
			CRoomComponent* pRoomCOM = pRoom->PlayerOut( RoleRoomType_GM, nPos );
			if ( pRoomCOM != NULL )
			{
				pRoomCOM->OutRoom();
				if ( pRoomCOM->AttrCOM() != NULL )
				{
					m_LobbyPlayer.insert( std::make_pair( pRoomCOM->AttrCOM()->GetRoleID(), pRoomCOM ) );
					CLocationMgr::Instance().EnterZone( Zone_LiveLobby, pRoomCOM->AttrCOM()->GetRoleID());
					_NoticeGroupState( pRoomCOM->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None );
					if (pRoomCOM->FriendCOM() != NULL) {
						pRoomCOM->FriendCOM()->PushOnlineInfoToAllFriends();
					}
				}

				// notice self
				GameMsg_S2C_QuitLiveRoomSuc msgSelfResponse;
				msgSelfResponse.m_chRoomRoleType = RoleRoomType_GM;
				msgSelfResponse.m_chRoomRolePos = (char)nPos;
				msgSelfResponse.m_nRoleID = pRoomCOM->AttrCOM()->GetRoleID();
				msgSelfResponse.m_chQuitType = RoomQuitType_NoRoom;
				msgSelfResponse.m_chQuitTo = (char)RoomQuitTo_LiveLobby;
				pRoomCOM->SendPlayerMsg( &msgSelfResponse );
			}
		}

		_OutEndingList( pRoom );
		m_setDestroyedRoom.insert( pRoom );
		m_mapDestroyedRoom[ pRoom->RoomID()] = pRoom;

		delete pRoom;
		pRoom = NULL;
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CLiveRoomManager::DestroyRoom. pRoom == NULL");
	}
}

bool CLiveRoomManager::AddRoom(CRoom* pRoom)
{
	if ( pRoom != NULL )
	{
		if ( FindRoom( pRoom->RoomID() ) == NULL )
		{
			m_AllRooms[MusicMode_None][pRoom->RoomID()] = pRoom;
			WriteLog(LOGLEVEL_DEBUG, "CLiveRoomManager::AddRoom. Add to all room list. RoomID=%d", pRoom->RoomID());
			m_AllRooms[pRoom->ChoosedMusicMode()][pRoom->RoomID()] = pRoom;

			return true;
		}
		else
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CLiveRoomManager::AddRoom. The room ID has been used. RoomID=%d", pRoom->RoomID());
		}
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CLiveRoomManager::AddRoom. pRoom == NULL");
	}

	return false;
}

CRoom* CLiveRoomManager::RemoveRoom(unsigned int nRoomID)
{
	CRoom* pRoom = NULL;

	AllRoomMap::iterator it_All = m_AllRooms[MusicMode_None].find( nRoomID );
	if ( it_All != m_AllRooms[MusicMode_None].end() )
	{
		pRoom = it_All->second;
		m_AllRooms[MusicMode_None].erase( it_All );
		WriteLog(LOGLEVEL_DEBUG, "CLiveRoomManager::RemoveRoom. Remove to all room list. RoomID=%d", pRoom->RoomID());

		if ( pRoom != NULL )
		{
			m_AllRooms[pRoom->ChoosedMusicMode()].erase( pRoom->RoomID() );

			_ReleaseRoomID( pRoom->RoomID() );
		}
	}

	return pRoom;
}

unsigned long CLiveRoomManager::GetMgrTime()
{
	return m_nMgrNowTime;
}

const CLiveRoomManager::AllRoomMap& CLiveRoomManager::GetRooms(unsigned char nMode)
{
	return m_AllRooms[nMode];
}

void CLiveRoomManager::OnUpdate(const unsigned long& lTimeElapsed)
{
	m_nMgrNowTime += lTimeElapsed;

	for ( TimeRoomMap::iterator it = m_NeedStartRooms.begin(); it != m_NeedStartRooms.end(); )
	{
		if ( it->first <= m_nMgrNowTime )
		{
			CRoom* pRoom = it->second;
			m_NeedStartRooms.erase( it++ );

			_AutoStart( pRoom );
		}
		else
		{
			break;
		}
	}

	
    for ( TimeRoomMap::iterator it = m_NeedEndRooms.begin(); it != m_NeedEndRooms.end(); ++it )
    {
        if ( it->first <= m_nMgrNowTime )
        {
             m_TempNeedEndRooms.push_back( it->second );
        }
        else
        {
            break;
        }
    }

    for ( std::vector<CRoom*>::iterator it = m_TempNeedEndRooms.begin(); it != m_TempNeedEndRooms.end(); ++it )
    {
        CRoom* pRoom = *it;
        _AutoEnd( pRoom );
    }
    m_TempNeedEndRooms.clear();

	m_nMatchTime += lTimeElapsed;
    if (m_nMatchTime >= RoomSettingMgr::Instance().AutoMatchInterval())
	{
        m_nMatchTime -= RoomSettingMgr::Instance().AutoMatchInterval();
		_AuditionMatch();
	}

	m_nLastSyncVoteTime += lTimeElapsed;
	if ((int)m_nLastSyncVoteTime >= CLiveRoomDataManager::Instance().m_nRefreshInterval * 1000)
	{
		m_nLastSyncVoteTime -= CLiveRoomDataManager::Instance().m_nRefreshInterval * 1000;

		for ( AllRoomMap::iterator it = m_AllRooms[MusicMode_None].begin(); it != m_AllRooms[MusicMode_None].end(); ++it)
		{
			CRoom* pRoom = it->second;
			if (pRoom->IsLiveRoom())
			{
				pRoom->OnUpdateDancerHot();
			}
		}
	}
}

void CLiveRoomManager::RefreshMaxAudienceCount()
{
	AllRoomMap::iterator it = m_AllRooms[MusicMode_None].begin();
	for (; it != m_AllRooms[MusicMode_None].end(); ++it)
	{
		CRoom *pRoom = it->second;
        if (pRoom != NULL && pRoom->IsLiveRoom() && pRoom->AudienceCapacity() != RoomSettingMgr::Instance().LiveRoomMaxAudienceCount())
		{
            pRoom->SetAudienceCapacity(RoomSettingMgr::Instance().LiveRoomMaxAudienceCount());
		}
	}
}

void CLiveRoomManager::OnEnterLiveLobby(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	if ( rRoleRoomCOM.AttrCOM() != NULL )
	{
		GameMsg_S2C_EnterLiveLobbyRes msg;
		msg.m_bCanCreateLiveRoom = rRoleRoomCOM.AttrCOM()->IsGM();
		msg.m_strTitle = CLiveRoomDataManager::Instance().m_strTitle;
		msg.m_strRule = CLiveRoomDataManager::Instance().m_strRule;
		msg.m_listMaleDefaultCloth = CLiveRoomDataManager::Instance().m_listMaleDefaultCloth;
		msg.m_listFemaleDefaultCloth = CLiveRoomDataManager::Instance().m_listFemaleDefaultCloth;

        if (rRoleRoomCOM.AttrCOM()->GetLevel() >= RoomSettingMgr::Instance().LiveRoomMinRoleLevel())
		{
			m_LobbyPlayer.insert( std::make_pair( rRoleRoomCOM.AttrCOM()->GetRoleID(), &rRoleRoomCOM ) );
			CLocationMgr::Instance().EnterZone( Zone_LiveLobby, rRoleRoomCOM.AttrCOM()->GetRoleID());
		}
		else
		{
			msg.m_strError = "EnterLiveLobby_LevelTooLow";
		}
		rRoleRoomCOM.SendPlayerMsg( &msg );
	}
}

void CLiveRoomManager::OnExitLiveLobby(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	_PlayerOutLobby( rRoleRoomCOM );
}

void CLiveRoomManager::OnGetLiveRoomList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	const GameMsg_C2S_GetLiveRoomList& rMsgRequest = (const GameMsg_C2S_GetLiveRoomList&)rMsg;

	std::map<unsigned int, CRoom *> normalRooms, endRooms;

	const AllRoomMap& rRooms = GetRooms( MusicMode_None );
	for ( AllRoomMap::const_iterator it = rRooms.begin(); it != rRooms.end(); ++it )
	{
		CRoom * pRoom = it->second;
		if (pRoom != NULL && pRoom->IsLiveRoom())
		{
			ERoomState state = (ERoomState)pRoom->RoomState();
			unsigned int createTime = pRoom->CreateTime();
			if (state == eRoomState_Ending || state == eRoomState_End)
			{
				endRooms[createTime] = pRoom;
			}
			else
			{
				normalRooms[createTime] = pRoom;
			}
		}
	}

	unsigned short nRoomCount = normalRooms.size() + endRooms.size();
	std::vector<CRoom*> obtainRooms;
	
	unsigned short nRoomPage = 0;
	for (std::map<unsigned int, CRoom *>::reverse_iterator it = normalRooms.rbegin();
		it != normalRooms.rend(); ++it)
	{
		obtainRooms.push_back(it->second);
	}
	for (std::map<unsigned int, CRoom *>::reverse_iterator it = endRooms.rbegin();
		it != endRooms.rend(); ++it)
	{
		obtainRooms.push_back(it->second);
	}

	nRoomPage = nRoomCount / ROOMCOUNT_PER_PAGE;
	GameMsg_S2C_GetLiveRoomListResult msgResponse;
	int nCurCount = 0;
	int nPage = ( nRoomPage < rMsgRequest.m_nPage ? nRoomPage : rMsgRequest.m_nPage );

	for ( int i = ROOMCOUNT_PER_PAGE * nPage; i < nRoomCount && nCurCount < ROOMCOUNT_PER_PAGE; ++i)
	{
		++nCurCount;

		LiveRoomBriefInfo roomInfo;
		obtainRooms[i]->EncodeLiveBriefInfo( roomInfo );

		msgResponse.m_vecRoomInfo.push_back( roomInfo );
	}

	msgResponse.m_nCurPage = (unsigned short)nPage;
	msgResponse.m_nRoomTotalNum = nRoomCount;
	rRoleRoomCOM.SendPlayerMsg( &msgResponse );
}


void CLiveRoomManager::OnCreateLiveRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	ECreateRoomRes eRes = CreateRoom_Success;
	std::string strError;

	const GameMsg_C2S_CreateLiveRoom& rMsgRequest = (const GameMsg_C2S_CreateLiveRoom&)rMsg;
	rRoleRoomCOM.SetPhoneOS( rMsgRequest.m_chPhoneOS );
    if (rRoleRoomCOM.IsInRoom())
	{
		eRes = CreateRoom_InRoom;
		strError.assign( "CreateLiveRoom_InRoom" );
	}
	else if (!rRoleRoomCOM.AttrCOM()->IsGM())
	{
		eRes = CreateRoom_NoPriv;
		strError.assign("CreateLiveRoom_NoPriv");
	}
	else if (rMsgRequest.m_nRoomType != eRoomType_LiveRoomPlayer 
		&& rMsgRequest.m_nRoomType != eRoomType_LiveRoomGroup 
		&& rMsgRequest.m_nRoomType != eRoomType_LiveRoomOther)
	{
		eRes = CreateRoom_InvalidRoomType;
		strError.assign("CreateLiveRoom_InvalidRoomType");
	}
	else if (rRoleRoomCOM.RoleCouple()->WeddingRoom() != NULL)
	{
		eRes = CreateRoom_InWeddingRoom;
		strError.assign("CreateLiveRoom_InWeddingRoom");
	} 
	else if (rRoleRoomCOM.RolePhotoRoom()->PhotoRoom() != NULL) 
	{
		eRes = CreateRoom_InPhotoRoom;
		strError.assign("CreateLiveRoom_InPhotoRoom");
	}
	else if (rRoleRoomCOM.RoleAmuseRoom()->AmuseRoom() != NULL)
	{
		eRes = CreateRoom_InAmuseRoom;
		strError.assign("CreateLiveRoom_InAmuseRoom");
	}
    else if (rRoleRoomCOM.DanceGroupCOM()->GetCeremonyRoom() != NULL)
    {
        eRes = CreateRoom_InCeremonyRoom;
        strError.assign("CreateLiveRoom_InCeremonyRoom");
    }
	else if (rMsgRequest.m_nRoomType == eRoomType_LiveRoomOther)
	{
		unsigned int nRoomID = _GenerateRoomID(rMsgRequest.m_nRoomType);
		if ( nRoomID == 0 )
		{
			eRes = CreateRoom_RoomEnough;
			strError.assign( "CreateLiveRoom_RoomEnough" );
		}
		else
		{
			std::string strRoomName;
			SafeFormat(strRoomName, CLocalization::Instance().GetString("Live_Room_Name", g_ServerConfig.m_nMaxRoomNameLength), nRoomID);
			NameDeny::Instance().FilterInvalidText(strRoomName);

			CRoom* pRoom = new CRoom((ERoomType)rMsgRequest.m_nRoomType, nRoomID);
			CheckDestroyRoom( pRoom );
			pRoom->ChangeRoomInfo(strRoomName.c_str(), "", rMsgRequest.m_strLiveRoomDesc.c_str());

			if( _PlayerInRoom( rRoleRoomCOM, pRoom, RoleRoomType_GM, RoleRoomState_ToStart, false, RoomEnterType_Create ) < 0 )
			{
				eRes = CreateRoom_Unknow;
				strError.assign( "CreateLiveRoom_Unknow" );

				_ReleaseRoomID( pRoom->RoomID() );
				delete pRoom;
				pRoom = NULL;
			}
			else
			{
				AddRoom( pRoom );
			}
		}
	}
	else
	{
		CLiveRoomDancerDB *pDancer = new CLiveRoomDancerDB;
		pDancer->m_bIsDanceGroup = rMsgRequest.m_nRoomType == eRoomType_LiveRoomGroup ? true : false;
		for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
		{
			pDancer->m_nDancerID[i] = rMsgRequest.m_szPlayerID[i];
		}
		CDelHelper delHelper(pDancer);
		g_pQueryMgr->AddQuery(Query_LiveRoom_GetDancerName, rRoleRoomCOM.AttrCOM()->GetRoleID(), pDancer, 0, NULL, &delHelper);
	}

	if ( eRes != CreateRoom_Success )
	{
		GameMsg_S2C_CreateLiveRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CLiveRoomManager::OnEnterLiveRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	const GameMsg_C2S_EnterLiveRoom& rMsgRequest = (const GameMsg_C2S_EnterLiveRoom&)rMsg;
	rRoleRoomCOM.SetPhoneOS( rMsgRequest.m_chPhoneOS );
	CRoom* pRoom = FindRoom( rMsgRequest.m_nRoomID );
	_EnterPlayer( rRoleRoomCOM, pRoom );
}

void CLiveRoomManager::OnQuitLiveRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	EQuitRoomRes eRes = QuitRoom_Success;
	std::string strError;

	CRoom* pRoom = rRoleRoomCOM.Room();
	if ( pRoom == NULL )
	{
		eRes = QuitRoom_NoRoom;
		strError.assign( "QuitLiveRoom_NoRoom" );
	}
	else if (rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer && (rRoleRoomCOM.RoleRoomState() == RoleRoomState_ToStart || rRoleRoomCOM.RoleRoomState() == RoleRoomState_Start) )
	{
		eRes = QuitRoom_HasReady;
		strError.assign( "QuitLiveRoom_HasReady" );
	}
	else if ( pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
	{
		eRes = QuitRoom_HasStart;
		strError.assign( "QuitLiveRoom_HasStart" );
	}
	else
	{
		GameMsg_C2S_QuitLiveRoom& rQuitMsg = (GameMsg_C2S_QuitLiveRoom&)rMsg;
		_PlayerOutRoom( rRoleRoomCOM, RoomQuitType_Active, rQuitMsg.m_chQuitTo, NULL );
	}

	if ( eRes != QuitRoom_Success )
	{
		GameMsg_S2C_QuitLiveRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CLiveRoomManager::OnStartRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	EStartRoomRes eRes = StartRoom_Success;
	std::string strError;

	CRoom* pRoom = rRoleRoomCOM.Room();
	if ( pRoom == NULL )
	{
		eRes = StartRoom_NoRoom;
		strError.assign( "StartLiveRoom_NoRoom" );
	}
	else if ( !pRoom->HasDancer() )
	{
		eRes = StartLiveRoom_NoDancer;
		strError.assign( "StartLiveRoom_NoDancer" );
	}
	else if ( pRoom->IsLiveRoom() && !rRoleRoomCOM.AttrCOM()->IsGM() )
	{
		eRes = StartLiveRoom_NoPriv;
		strError.assign( "StartLiveRoom_NoPriv" );
	}
	else if ( pRoom->HasStart() )
	{
		eRes = StartRoom_HasStart;
		strError.assign( "StartLiveRoom_HasStart" );
	}
	else if ( pRoom->RoomState() >= eRoomState_Ending )
	{
		eRes = StartLiveRoom_HasEnd;
		strError.assign( "StartLiveRoom_HasEnd" );
	}
	/*else if ( !pRoom->IsStartReady() ) // 直播房间GM可以强行开始房间
	{
		eRes = StartRoom_NotReady;
		strError.assign( "StartLiveRoom_NotReady" );
	}*/
	else
	{
		pRoom->RoomPrepare();

		std::vector<char> vecAndroidStage;
		std::vector<char> vecIosStage;
		std::vector<char> vecWinPhoneStage;

		const std::string& strStageFile = pRoom->PlayMusicStage();

		GameMsg_S2C_PrepareRoom msgResponse;
		msgResponse.m_chScene = (char)pRoom->PlayScene();
		msgResponse.m_nMusic = (short)pRoom->PlayMusicID();
		msgResponse.m_chMode = (char)pRoom->PlayMusicMode();
		msgResponse.m_chLevel = (char)pRoom->PlayMusicLevel();
		msgResponse.m_strCheckKey = pRoom->PlayCheckCode();
		if (pRoom->IsLiveRoom())
		{
            msgResponse.m_nCountDownTime = (unsigned short)RoomSettingMgr::Instance().LiveRoomCountDownTime();
		}
		

		if ( !strStageFile.empty() && CMusicStageFileManager::LoadStage( strStageFile, vecAndroidStage, vecIosStage, vecWinPhoneStage ) )
		{
			int nStageTime = CMusicStageFileManager::CalcuStageTime( pRoom->PlayMusicMode(), vecAndroidStage, vecIosStage, vecWinPhoneStage );
			pRoom->SetCheckTime( nStageTime );

			for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
			{
				CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
				if ( pRoomCOM != NULL )
				{
					_PlayerChangeState( *pRoomCOM, RoleRoomState_ToStart, false );
					pRoomCOM->RoleMarkInfo().ClearMark();
					if ( pRoomCOM->GetPhoneOS() == OS_Ios )
					{
						msgResponse.m_vecStage.assign( vecIosStage.begin(), vecIosStage.end() );
					}
					else if ( pRoomCOM->GetPhoneOS() == OS_WP8 )
					{
						msgResponse.m_vecStage.assign( vecWinPhoneStage.begin(), vecWinPhoneStage.end() );
					}
					else
					{
						msgResponse.m_vecStage.assign( vecAndroidStage.begin(), vecAndroidStage.end() );
					}

					pRoomCOM->SendPlayerMsg( &msgResponse );
				}
			}

			std::map<unsigned int, CRoomComponent*> audienceMap = pRoom->LiveRoomAudicenceMap();
			for ( std::map<unsigned int, CRoomComponent*>::iterator it = audienceMap.begin(); 
				it != audienceMap.end(); ++it )
			{
				CRoomComponent* pRoomCOM = it->second;
				if ( pRoomCOM != NULL )
				{
					if ( pRoomCOM->GetPhoneOS() == OS_Ios )
					{
						msgResponse.m_vecStage.assign( vecIosStage.begin(), vecIosStage.end() );
					}
					else if ( pRoomCOM->GetPhoneOS() == OS_WP8 )
					{
						msgResponse.m_vecStage.assign( vecWinPhoneStage.begin(), vecWinPhoneStage.end() );
					}
					else
					{
						msgResponse.m_vecStage.assign( vecAndroidStage.begin(), vecAndroidStage.end() );
					}

					pRoomCOM->SendPlayerMsg( &msgResponse );
				}
			}

			for ( int nPos = 0; nPos <= pRoom->MaxGMPos(); ++nPos )
			{
				CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_GM, nPos );
				if ( pRoomCOM != NULL )
				{
					if ( pRoomCOM->GetPhoneOS() == OS_Ios )
					{
						msgResponse.m_vecStage.assign( vecIosStage.begin(), vecIosStage.end() );
					}
					else if ( pRoomCOM->GetPhoneOS() == OS_WP8 )
					{
						msgResponse.m_vecStage.assign( vecWinPhoneStage.begin(), vecWinPhoneStage.end() );
					}
					else
					{
						msgResponse.m_vecStage.assign( vecAndroidStage.begin(), vecAndroidStage.end() );
					}

					pRoomCOM->SendPlayerMsg( &msgResponse );
				}
			}
		}
		if (pRoom->PlayMusicMode() == MusicMode_Sweethearts)
		{
			CRoomManager::StartSweetheartsMode(pRoom, vecAndroidStage, vecIosStage);
		}
	}

	if ( eRes != StartRoom_Success )
	{
		GameMsg_S2C_StartRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CLiveRoomManager::OnReadyRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	CRoom* pRoom = rRoleRoomCOM.Room();
	if ( pRoom != NULL && pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer )
	{
		_PlayerChangeState( rRoleRoomCOM, RoleRoomState_ToStart, false );

		if ( pRoom->IsPlayReady() && pRoom->HasEndCountDown() )
		{
			_OutStartingList( pRoom );
			_RoomStart( pRoom );
		}
		else
		{
			if ( pRoom->RoomState() == eRoomState_Preparing )
			{
				if ((pRoom->IsLiveRoom() && pRoom->HasEndCountDown()) || pRoom->IsAuditionRoom())
				{
					pRoom->SetState( eRoomState_Waiting );
					_InStartingList( pRoom );
				}
			}
		}
	}
}

void CLiveRoomManager::OnEndRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    CRoom* pRoom = rRoleRoomCOM.Room();
    if ( pRoom != NULL && pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer )
    {
        int nNowTime = time( NULL );
        int nLegalTime = pRoom->StartTime() + pRoom->GetCheckTime();

        if ( nNowTime >= nLegalTime )
        {
            _PlayerChangeState( rRoleRoomCOM, RoleRoomState_ToEnd, false );

            if ( pRoom->IsEndReady() )
            {
                _OutEndingList( pRoom );
                _RoomEnd( pRoom );

                if (pRoom->IsAuditionRoom())
                {
                    _InEndingList( pRoom );
                }
            }
            else
            {
                if ( pRoom->RoomState() == eRoomState_Playing )
                {
                    pRoom->SetState( eRoomState_Ending );
                    _InEndingList( pRoom );
                }
            }
        }
        else
        {
            _PlayerOutRoom( rRoleRoomCOM, RoomQuitType_EndKick, RoomQuitTo_LiveLobby, NULL );
        }
    }
}

void CLiveRoomManager::OnKickPlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	EKickPlayerRes eRes = KickPlayer_Success;
	std::string strError;

	const GameMsg_C2S_KickPlayer& rMsgRequest = (const GameMsg_C2S_KickPlayer&)rMsg;
	CRoom* pRoom = rRoleRoomCOM.Room();

	if ( pRoom == NULL )
	{
		eRes = KickPlayer_NoRoom;
		strError.assign( "KickPlayer_NoRoom" );
	}
	else if ( (pRoom->IsLiveRoom() && !rRoleRoomCOM.AttrCOM()->IsGM()) 
		|| ( pRoom->IsAuditionRoom() && !rRoleRoomCOM.IsHost() && !rRoleRoomCOM.VipCOM()->IsVIP()) )
	{
		eRes = KickPlayer_NoAuthority;
		strError.assign( "KickPlayer_NoAuthority" );
	} 
	else if ( rRoleRoomCOM.RoleRoomType() == RoleRoomType_Audience )
	{
		eRes = KickPlayer_AudienceCanNotKickPlayer;
		strError.assign( "KickPlayer_AudienceCanNotKickPlayer" );
	}
	else if ( pRoom->HasStart() )
	{
		eRes = KickPlayer_HasStart;
		strError.assign( "KickPlayer_HasStart" );
	}
	else
	{
		int nPos = rMsgRequest.m_chRoleRoomPos;
		if (rMsgRequest.m_chRoleRoomType == RoleRoomType_Audience)
		{
			nPos = rMsgRequest.m_nRoleID;
		}
		CRoomComponent* pPlayer = pRoom->Player( rMsgRequest.m_chRoleRoomType, nPos );
		if ( pPlayer == NULL )
		{
			eRes = KickPlayer_NoPlayer;
			strError.assign( "KickPlayer_NoPlayer" );
		}
		else if ( pPlayer->RoleRoomType() == rRoleRoomCOM.RoleRoomType() && pPlayer->RoleRoomPos() == rRoleRoomCOM.RoleRoomPos() )
		{
			eRes = KickPlayer_IsSelf;
			strError.assign( "KickPlayer_IsSelf" );
		}
		else 
		{
			ERoomQuitType eQuitType;
			if (rRoleRoomCOM.AttrCOM()->IsGM())
			{
				eQuitType = RoomQuitType_KickedByGM;
			}
			else if (rRoleRoomCOM.IsHost())
			{
				eQuitType = RoomQuitType_HostKick;
			}
			else
			{
				eQuitType = RoomQuitType_KickedByVIP;
			}
			_PlayerOutRoom( *pPlayer, eQuitType, RoomQuitTo_LiveLobby, &rRoleRoomCOM);
		}
	}

	if ( eRes != KickPlayer_Success )
	{
		GameMsg_S2C_KickPlayerFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CLiveRoomManager::OnInEquip(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	if ( rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer )
	{
		CRoom* pRoom = rRoleRoomCOM.Room();

		if ( pRoom != NULL && !pRoom->HasStart() )
		{
			if ( rRoleRoomCOM.RoleRoomState() == RoleRoomState_Wait || rRoleRoomCOM.RoleRoomState() == RoleRoomState_Ready )
			{
				_PlayerChangeState( rRoleRoomCOM, RoleRoomState_Equip, true );
			}
		}
	}
}

void CLiveRoomManager::OnOutEquip(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	if ( rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer )
	{
		CRoom* pRoom = rRoleRoomCOM.Room();

		if ( pRoom != NULL && !pRoom->HasStart() )
		{
			if ( rRoleRoomCOM.RoleRoomState() == RoleRoomState_Equip )
			{
				_PlayerChangeState( rRoleRoomCOM, RoleRoomState_Wait, true );
			}
		}
	}
}

void CLiveRoomManager::OnChangeMusicInfo(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	EChangeMusicInfoRes eRes = ChangeMusicInfo_Success;
	std::string strError;

	const GameMsg_C2S_ChangeMusicInfo& rMsgRequest = (GameMsg_C2S_ChangeMusicInfo&)rMsg;
	CRoom* pRoom = rRoleRoomCOM.Room();

	if ( pRoom == NULL )
	{
		eRes = ChangeMusicInfo_NoRoom;
		strError.assign( "ChangeMusicInfo_NoRoom" );
	}
	else if ( pRoom->HasStart() )
	{
		eRes = ChangeMusicInfo_HasStart;
		strError.assign( "ChangeMusicInfo_HasStart" );
	}
	else if ( !rRoleRoomCOM.AttrCOM()->IsGM() )
	{
		eRes = ChangeMusicInfo_NoPriv;
		strError.assign( "ChangeMusicInfo_NoPriv" );
	}
	else
	{
		if ( rMsgRequest.m_chScene != pRoom->ChoosedScene()
			&& ConfigManager::Instance().GetSceneModeManager().GetSceneInfo( rMsgRequest.m_chScene ) == NULL )
		{
			eRes = ChangeMusicInfo_NoScene;
			strError.assign( "ChangeMusicInfo_NoScene" );
		}
		else if ( rMsgRequest.m_chMode < MusicMode_None || rMsgRequest.m_chMode >= MusicMode_Max )
		{
			eRes = ChangeMusicInfo_NoMode;
			strError.assign( "ChangeMusicInfo_NoMode" );
		}
		else if ( rMsgRequest.m_chLevel < MusicLevel_None || rMsgRequest.m_chLevel >= MusicLevel_Max )
		{
			eRes = ChangeMusicInfo_NoLevel;
			strError.assign( "ChangeMusicInfo_NoLevel" );
		}
		else
		{
			if ( rMsgRequest.m_nMusic != RAN_MUSIC_ID
				&& !ConfigManager::Instance().GetMusicStageManager().ExistMusic( rMsgRequest.m_nMusic, rMsgRequest.m_chMode, rMsgRequest.m_chLevel ) )
			{
				eRes = ChangeMusicInfo_NoMusic;
				strError.assign( "ChangeMusicInfo_NoMusic" );
			}
			else
			{
				MusicConfig* pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetByID(rMsgRequest.m_nMusic);
				if ( rMsgRequest.m_nMusic != RAN_MUSIC_ID 
					&& pMusicInfo != NULL 
					&& pMusicInfo->m_bIsFresher == false
					&& g_pCirculator->GetServerInstallerID() == INSTALLER_FRESHER )
				{
					eRes = ChangeMusicInfo_InvalidInstaller;
					strError.assign( "ChangeMusicInfo_InvalidInstaller" );
				}
				else
				{
					int nOldMode = pRoom->ChoosedMusicMode();
					pRoom->ChangeChoosedScene( rMsgRequest.m_chScene );
					pRoom->ChangeChoosedMusic( rMsgRequest.m_nMusic, rMsgRequest.m_chMode, rMsgRequest.m_chLevel );
					_RoomSwitch( nOldMode, pRoom );

					GameMsg_S2C_ChangeMusicInfoSuc msgResponse;
					msgResponse.m_chScene = (char)pRoom->ChoosedScene();
					msgResponse.m_nMusic = (short)pRoom->ChoosedMusicID();
					msgResponse.m_chMode = (char)pRoom->ChoosedMusicMode();
					msgResponse.m_chLevel = (char)pRoom->ChoosedMusicLevel();
					pRoom->SendMsgToAll( &msgResponse );
				}
			}
		}
	}

	if ( eRes != ChangeMusicInfo_Success )
	{
		GameMsg_S2C_ChangeMusicInfoFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CLiveRoomManager::OnSetLiveRoomDefaultCloth(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	std::string strError;
	
	const GameMsg_C2S_SetLiveRoomDefaultCloth& rMsgRequest = (const GameMsg_C2S_SetLiveRoomDefaultCloth&)rMsg;
	CRoom* pRoom = rRoleRoomCOM.Room();
	if (pRoom == NULL)
	{
		strError.assign( "SetLiveRoomDefaultCloth_NoRoom" );
	}
	else if (!rRoleRoomCOM.AttrCOM()->IsGM())
	{
		strError.assign( "SetLiveRoomDefaultCloth_NoPriv" );
	}
	else
	{
		pRoom->SetDefaultCloth(rMsgRequest.m_bSet);
		GameMsg_S2C_SetLiveRoomDefaultClothRes responseMsg;
		responseMsg.m_bSet = rMsgRequest.m_bSet;
		pRoom->SendMsgToAll(&responseMsg);
	}

	if (!strError.empty())
	{
		GameMsg_S2C_SetLiveRoomDefaultClothRes responseMsg;
		responseMsg.m_bSet = false;
		responseMsg.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &responseMsg );
	}
}

void CLiveRoomManager::OnGetLiveRoomScore(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	GameMsg_C2S_GetLiveRoomScore& msg = (GameMsg_C2S_GetLiveRoomScore&)rMsg;
	int nPage = msg.m_nPage;
	CRoom *pRoom = rRoleRoomCOM.Room();
	if (pRoom != NULL)
	{
		const std::map<unsigned int, CLiveRoomRoundDancerScore>& mapScore = pRoom->GetDancerScore();
		if ((int)mapScore.size() > nPage && mapScore.size() > 0)
		{
			std::map<unsigned int, CLiveRoomRoundDancerScore>::const_reverse_iterator it = mapScore.rbegin();
			int i = 0;
			while (it != mapScore.rend() && i != nPage)
			{
				++i;
				++it;
			}
			if (i == nPage)
			{
				GameMsg_S2C_GetLiveRoomScoreRes responseMsg;
                responseMsg.m_nCurrPage = (unsigned short)nPage;
				responseMsg.m_nTotalPage = (unsigned short)mapScore.size();
				responseMsg.m_score = it->second;
				rRoleRoomCOM.SendPlayerMsg(&responseMsg);
			}
			else
			{
				GameMsg_S2C_GetLiveRoomScoreRes responseMsg;
				responseMsg.m_nCurrPage = 0;
				responseMsg.m_nTotalPage = 1;
				rRoleRoomCOM.SendPlayerMsg(&responseMsg);
			}
		}
		else
		{
			GameMsg_S2C_GetLiveRoomScoreRes responseMsg;
			responseMsg.m_nCurrPage = 0;
			responseMsg.m_nTotalPage = 1;
			rRoleRoomCOM.SendPlayerMsg(&responseMsg);
		}
	}
}

void CLiveRoomManager::OnCloseLiveRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	std::string strError;

	//const GameMsg_C2S_CloseLiveRoom& rMsgRequest = (const GameMsg_C2S_CloseLiveRoom&)rMsg;
	CRoom* pRoom = rRoleRoomCOM.Room();
	if (pRoom == NULL)
	{
		strError.assign( "CloseLiveRoom_NoRoom" );
	}
	else if (!rRoleRoomCOM.AttrCOM()->IsGM())
	{
		strError.assign( "CloseLiveRoom_NoPriv" );
	}
	else
	{
		pRoom->SetState(eRoomState_Ending);
		_InEndingList(pRoom);
		GameMsg_S2C_CloseLiveRoomSuc sucMsg;
        sucMsg.m_nCloseCountDownTime = RoomSettingMgr::Instance().LiveRoomCloseTime();
		pRoom->SendMsgToAll(&sucMsg);
	}

	if (!strError.empty())
	{
		GameMsg_S2C_CloseLiveRoomFail errMsg;
		errMsg.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &errMsg );
	}
}

void CLiveRoomManager::OnUpdateLiveRoomDancer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	GameMsg_C2S_UpdateLiveRoomDancer &msg = (GameMsg_C2S_UpdateLiveRoomDancer &)rMsg;
	std::string strError;
	CRoom* pRoom = rRoleRoomCOM.Room();
	if (pRoom == NULL)
	{
		strError.assign( "SetLiveRoomDancer_NoRoom" );
	}
	else if (!rRoleRoomCOM.AttrCOM()->IsGM())
	{
		strError.assign( "SetLiveRoomDancer_NoPriv" );
	}
	else
	{
		CLiveRoomDancerDB *pDancer = new CLiveRoomDancerDB;
		pDancer->m_bIsDanceGroup = false;
		for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
		{
			pDancer->m_nDancerID[i] = msg.m_szPlayerID[i];
		}
		CDelHelper delHelper(pDancer);
		g_pQueryMgr->AddQuery(Query_LiveRoom_UpdateDancerName, rRoleRoomCOM.AttrCOM()->GetRoleID(), pDancer, 0, NULL, &delHelper);
	}

	if (!strError.empty())
	{
		GameMsg_S2C_UpdateLiveRoomDancerFail errMsg;
		errMsg.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &errMsg );
	}

}
void CLiveRoomManager::OnSetLiveRoomDancerToAudience(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	GameMsg_C2S_SetLiveRoomDancerToAudience &msg = (GameMsg_C2S_SetLiveRoomDancerToAudience &)rMsg;
	std::string strError;
	CRoom* pRoom = rRoleRoomCOM.Room();
	if (pRoom == NULL)
	{
		strError.assign( "SetLiveRoomDancerToAudience_NoRoom" );
	}
	else if (!rRoleRoomCOM.AttrCOM()->IsGM())
	{
		strError.assign( "SetLiveRoomDancerToAudience_NoPriv" );
	}
	else
	{
		CRoleEntity *pPlayer = CPlayerManager::Instance().GetEntityByRoleID(msg.m_nRoleID);
		if (pPlayer == NULL)
		{
			strError.assign( "SetLiveRoomDancerToAudience_PlayerNotOnline" );
		}
		else
		{
			CRoomComponent *pRoleRoomCOM = pPlayer->GetComponent<CRoleRoom>();
			if (_PlayerChangeType(*pRoleRoomCOM, RoleRoomType_Audience) < 0)
			{
				strError.assign( "SetLiveRoomDancerToAudience_Unknown" );
			}
			else
			{
				pRoom->RemoveLiveRoomDancer(pRoleRoomCOM->AttrCOM()->GetRoleID());
				_NoticePlayerChange(pRoom);
			}
		}
	}

	if (!strError.empty())
	{
		GameMsg_S2C_SetLiveRoomDancerToAudienceFail errMsg;
		errMsg.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &errMsg );
	}
}

void CLiveRoomManager::OnChangeRoleRoomState(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	EChangeRoleRoomStateRes eRes = ChangeRoleRoomState_Success;
	std::string strError;

	const GameMsg_C2S_ChangeRoleRoomState& rMsgRequest = (const GameMsg_C2S_ChangeRoleRoomState&)rMsg;
	CRoom* pRoom = rRoleRoomCOM.Room();

	if ( pRoom == NULL )
	{
		eRes = ChangeRoleRoomState_NoRoom;
		strError.assign( "ChangeRoleRoomState_NoRoom" );
	}
	else if ( rRoleRoomCOM.IsHost() )
	{
		eRes = ChangeRoleRoomState_IsHost;
		strError.assign( "ChangeRoleRoomState_IsHost" );
	}
	else if ( rRoleRoomCOM.RoleRoomType() != RoleRoomType_Dancer )
	{
		eRes = ChangeRoleRoomState_NotDancer;
		strError.assign( "ChangeRoleRoomState_NotDancer" );
	}
	else if ( pRoom->HasStart() )
	{
		eRes = ChangeRoleRoomState_HasStart;
		strError.assign( "ChangeRoleRoomState_HasStart" );
	}
	else if ( rMsgRequest.m_chRoleRoomState <= RoleRoomState_None || rMsgRequest.m_chRoleRoomState >= RoleRoomState_ToStart )
	{
		eRes = ChangeRoleRoomState_NoState;
		strError.assign( "ChangeRoleRoomState_NoState" );
	}
	else if ( rRoleRoomCOM.RoleRoomState() == rMsgRequest.m_chRoleRoomState )
	{
		eRes = ChangeRoleRoomState_NoChange;
		strError.assign( "ChangeRoleRoomState_NoChange" );
	}
	else
	{
		_PlayerChangeState( rRoleRoomCOM, rMsgRequest.m_chRoleRoomState, true );
	}

	if ( eRes != ChangeRoleRoomState_Success )
	{
		GameMsg_S2C_ChangeRoleRoomStateFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CLiveRoomManager::OnLoadingStartGameProgress(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_LoadingStartRoomProgress & rMsgLoaddingRcv = (const GameMsg_C2S_LoadingStartRoomProgress &)rMsg;
    CRoom *pRoom = rRoleRoomCOM.Room();
    if (pRoom != NULL && (rRoleRoomCOM.RoleRoomState() == RoleRoomState_Ready || rRoleRoomCOM.RoleRoomState() == RoleRoomState_ToStart))
    {
        GameMsg_S2C_LoadingStartRoomProgress msgLoadingSend;
        if (rRoleRoomCOM.AttrCOM() != NULL) {
            msgLoadingSend.m_nRoleID = rRoleRoomCOM.AttrCOM()->GetRoleID();
            msgLoadingSend.m_nRate = rMsgLoaddingRcv.m_nRate >= 100 ? 100 : rMsgLoaddingRcv.m_nRate;
            pRoom->SendMsgToAll(&msgLoadingSend);
        }
    }
}

void CLiveRoomManager::OnReportRoundMark(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	const GameMsg_C2S_ReportRoundMark& rMsgRequest = (const GameMsg_C2S_ReportRoundMark&)rMsg;
	if ( rMsgRequest.m_chKeyRank > RoomRoundRank_None && rMsgRequest.m_chKeyRank < RoomRoundRank_Max && rMsgRequest.m_nRound > 0 )
	{
		CRoom* pRoom = rRoleRoomCOM.Room();

		if ( pRoom != NULL && pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer )
		{
			std::string strSrcCode;
			Formatter(strSrcCode) << pRoom->PlayCheckCode() << (int)rMsgRequest.m_nRound
				<< (int)rMsgRequest.m_chKeyRank << rMsgRequest.m_nMark;

			for (size_t i = 0; i < rMsgRequest.m_vecRoundRank.size(); ++i)
			{
				std::string strRank;
				Formatter(strRank) << (int)rMsgRequest.m_vecRoundRank[i];
				strSrcCode.append(strRank);
			}

			unsigned char szMD5Code[16] = { 0 };
			MDString( strSrcCode.c_str(), szMD5Code );

			std::string strCheckCode = BinaryData2String( szMD5Code, sizeof(szMD5Code) );
			if ( rMsgRequest.m_strMD5Code.compare( strCheckCode ) == 0 )
			{
				bool bIsSweetheartsMode = false;
				int nMusicMode = pRoom->PlayMusicMode();
				if (nMusicMode == MusicMode_Sweethearts)
				{
					bIsSweetheartsMode = true;
					nMusicMode = pRoom->GetMusicModeByRound(rMsgRequest.m_nRound);
					/*if (nMusicMode == MusicMode_None)
					{
						WriteLog("xindong OnReportRoundMark error round %d", (int)rMsgRequest.m_chRound);
						return;
					}*/
				}
				if ( rRoleRoomCOM.RecvRoundMark( nMusicMode, 
					rMsgRequest.m_nRound, rMsgRequest.m_nMark, rMsgRequest.m_chKeyRank, rMsgRequest.m_vecRoundRank ) )
				{
					if (pRoom->TeamMode() != EDanceRoomTeamMode_None) {
						pRoom->AddTeamMarkInfo(rRoleRoomCOM.ColorTeam(), rMsgRequest.m_nMark);
					}
					GameMsg_S2C_SyncRoundMark msgResponse;
					msgResponse.m_chDancerPos = (char)rRoleRoomCOM.RoleRoomPos();
					msgResponse.m_chRound = (char)rRoleRoomCOM.RoleMarkInfo().Round();
					msgResponse.m_chRank = (char)rRoleRoomCOM.RoleMarkInfo().LastRank();//for model3 ,this is AllComboFlag; > 1 = AllCombo;
					msgResponse.m_nMark = rRoleRoomCOM.RoleMarkInfo().Mark();
					msgResponse.m_nSpecial = rRoleRoomCOM.RoleMarkInfo().LastSpecialCount();
					pRoom->SendMsgToAll( &msgResponse, &rRoleRoomCOM );
					if ( bIsSweetheartsMode )
					{
						pRoom->UpdateSweethearsInfo();
					}

					//fufeng to do: award, achievement, quest
					rRoleRoomCOM.AchieveCOM()->RoomPerRoundScoreReport( rRoleRoomCOM, rMsgRequest.m_nMark, rMsgRequest.m_chKeyRank );
				}
			}
		}
	}
}

void CLiveRoomManager::OnReportEffectChange(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	const GameMsg_C2S_ReportEffectChange rMsgRequest = (const GameMsg_C2S_ReportEffectChange&)rMsg;
	CRoom* pRoom = rRoleRoomCOM.Room();
	if ( pRoom != NULL && pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer )
	{
		std::string strSrcCode;
		Formatter(strSrcCode) << pRoom->PlayCheckCode() << rMsgRequest.m_nMark 
			<< rMsgRequest.m_nSpecail << (int)rMsgRequest.m_chRound;

		unsigned char szMD5Code[16] = { 0 };
		MDString( strSrcCode.c_str(), szMD5Code );

		std::string strCheckCode = BinaryData2String( szMD5Code, sizeof(szMD5Code) );
		if ( rMsgRequest.m_strMD5Code.compare( strCheckCode ) == 0 )
		{
			GameMsg_S2C_SyncEffectState msgResponse;
			msgResponse.m_nMark = rMsgRequest.m_nMark;
			msgResponse.m_nSpecial = rMsgRequest.m_nSpecail;
			msgResponse.m_chRound = rMsgRequest.m_chRound;
			msgResponse.m_nDancerPos = (char)rRoleRoomCOM.RoleRoomPos();

			pRoom->SendMsgToAll( &msgResponse, &rRoleRoomCOM );
		}
	}
}
void CLiveRoomManager::OnQuitMarkAward(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	CRoom* pRoom = rRoleRoomCOM.Room();

	if ( pRoom != NULL && rRoleRoomCOM.RoleRoomState() == RoleRoomState_End )
	{
		int nState = ( rRoleRoomCOM.IsHost() ? RoleRoomState_Ready : rRoleRoomCOM.RoleDefaultState() );
		_PlayerChangeState( rRoleRoomCOM, nState, true );
	}
}

void CLiveRoomManager::OnRoleLogin(CRoomComponent& rRoleRoomCOM)
{
}

void CLiveRoomManager::OnRoleLogout(CRoomComponent& rRoleRoomCOM)
{
	_QuitAuditionList( rRoleRoomCOM );
	_PlayerOutLobby( rRoleRoomCOM );
	_PlayerOutRoom( rRoleRoomCOM, RoomQuitType_Logout, RoomQuitTo_World, NULL);
}

void CLiveRoomManager::SendMsgInLobby(GameMsg_Base* pMsg)
{
	for ( PlayerMap::iterator it = m_LobbyPlayer.begin(); it != m_LobbyPlayer.end(); ++it )
	{
		if ( it->second != NULL )
		{
			it->second->SendPlayerMsg( pMsg );
		}
	}
}

void CLiveRoomManager::SendLobbyChatMsg(unsigned int nSenderID ,GameMsg_Base *pMsg)
{
	for ( PlayerMap::iterator it = m_LobbyPlayer.begin(); it != m_LobbyPlayer.end(); ++it )
	{
		if ( it->second != NULL )
		{
			if (it->second->FriendCOM()!= NULL && !it->second->FriendCOM()->IsInBlackList(nSenderID)) {
				it->second->SendPlayerMsg( pMsg );
			}
		}
	}
}

void CLiveRoomManager::ExitLobby(CRoomComponent& rRoleRoomCOM)
{
	_PlayerOutLobby(rRoleRoomCOM);	
}

unsigned int CLiveRoomManager::_GenerateRoomID(unsigned char nRoomType)
{
	unsigned int nRoomID = 0;
	if (nRoomType == eRoomType_LiveRoomAudition)
	{
		if (m_IdleAuditionRoomID.empty())
		{
			nRoomID = 0;
		}
		else
		{
			nRoomID = *m_IdleAuditionRoomID.begin();
			m_IdleAuditionRoomID.pop_front();
		}
	}
	else
	{
		if (m_IdleLiveRoomID.empty())
		{
			nRoomID = 0;
		}
		else
		{
			nRoomID = *m_IdleLiveRoomID.begin();
			m_IdleLiveRoomID.pop_front();
		}
	}

	return nRoomID;
}

void CLiveRoomManager::_ReleaseRoomID(unsigned int nRoomId)
{
	if (nRoomId > kMaxLiveRoomID)
	{
		m_IdleAuditionRoomID.push_back(nRoomId);
	}
	else
	{
		m_IdleLiveRoomID.push_back(nRoomId);
	}
}

void CLiveRoomManager::_EnterPlayer(CRoomComponent& rRoleRoomCOM, CRoom *pRoom)
{
	EEnterRoomRes eRes = EnterRoom_Success;
	std::string strError;

    if (rRoleRoomCOM.IsInRoom())
	{
		eRes = EnterRoom_InRoom;
		strError.assign( "EnterLiveRoom_InRoom" );
	}
	else if ( pRoom == NULL )
	{
		eRes = EnterRoom_NoRoom;
		strError.assign( "EnterLiveRoom_NoRoom" );
	}
	else if ( !pRoom->IsLiveRoom() && !pRoom->IsAuditionRoom() )
	{
		eRes = EnterRoom_RoomTypeNotMatch;
		strError.assign( "EnterLiveRoom_RoomTypeNotMatch" );
	}
	else if ( pRoom->HasStart() )
	{
		eRes = EnterRoom_RoomStart;
		strError.assign( "EnterLiveRoom_RoomStart" );
	}
	else if ( pRoom->RoomState() > eRoomState_Ending )
	{
		eRes = EnterRoom_RoomEnd;
		strError.assign( "EnterLiveRoom_RoomEnd" );
	}
	else if (rRoleRoomCOM.RoleCouple()->WeddingRoom() != NULL) 
	{
		eRes = EnterRoom_InWeddingRoom;
		strError.assign("EnterLiveRoom_InWeddingRoom");
	}
	else if (rRoleRoomCOM.RolePhotoRoom()->PhotoRoom() != NULL) 
	{
		eRes = EnterRoom_InPhotoRoom;
		strError.assign("EnterLiveRoom_InPhotoRoom");
	}
	else if (rRoleRoomCOM.RoleAmuseRoom()->AmuseRoom() != NULL) 
	{
		eRes = EnterRoom_InAmuseRoom;
		strError.assign("EnterLiveRoom_InAmuseRoom");
	}
    else if (rRoleRoomCOM.DanceGroupCOM()->GetCeremonyRoom() != NULL)
    {
        eRes = EnterRoom_InCeremonyRoom;
        strError.assign("EnterLiveRoom_InCeremonyRoom");
    }
	else
	{
		int nRoleRoomType = RoleRoomType_Dancer;
		if (pRoom->IsLiveRoom())
		{
			nRoleRoomType = pRoom->TryEnterLiveRoomRoleType(rRoleRoomCOM);
		}
		if ( _PlayerInRoom( rRoleRoomCOM, pRoom, nRoleRoomType, rRoleRoomCOM.RoleDefaultState(), false, RoomEnterType_Join ) < 0 )
		{
			eRes = EnterRoom_AudienceFull;
			strError.assign("EnterLiveRoom_AudienceFull");
		}
	}

	if ( eRes != EnterRoom_Success )
	{
		GameMsg_S2C_EnterLiveRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &msgResponse );
	}
}


int CLiveRoomManager::_PlayerInRoom(CRoomComponent& rRoleRoomCOM, CRoom* pRoom, int nRoleType, int nRoleState, bool bHost, int nInType)
{
	int nPos = -1;

	if ( pRoom != NULL )
	{
		if ( rRoleRoomCOM.Room() != NULL )
		{
			WriteLog(LOGLEVEL_ERROR, "Player in room error, already in a room, in type: %d", nInType );
		}

		nPos = pRoom->PlayerIn( rRoleRoomCOM, nRoleType, nRoleState, bHost );
		if ( nPos >= 0 )
		{
			rRoleRoomCOM.InRoom( pRoom, EDanceColorTeam_None, nRoleType, nPos, nRoleState, bHost );

			if ( rRoleRoomCOM.AttrCOM() != NULL )
			{
				m_LobbyPlayer.erase( rRoleRoomCOM.AttrCOM()->GetRoleID() );
				CLocationMgr::Instance().RemoveEntity( Zone_LiveLobby, rRoleRoomCOM.AttrCOM()->GetRoleID());
				_NoticeGroupState( rRoleRoomCOM.AttrCOM()->GetRoleID(), pRoom->RoomID(), nRoleType, nRoleState );
				if ( rRoleRoomCOM.FriendCOM() ) {
					rRoleRoomCOM.FriendCOM()->PushOnlineInfoToAllFriends();
				}
			}

			if ( nInType != RoomEnterType_Match )
			{
				// notice self
				GameMsg_S2C_CreateLiveRoomSuc msgSelfResponse;
				pRoom->EncodeLiveWholeInfo( msgSelfResponse.m_RoomInfo );
				msgSelfResponse.m_RoomInfo.m_chRoomRoleType = (char)nRoleType;
				msgSelfResponse.m_RoomInfo.m_bSendAudience = rRoleRoomCOM.AttrCOM()->IsGM();
				rRoleRoomCOM.SendPlayerMsg( &msgSelfResponse );

				// notice others, bHost is true only when player is creating room, there is no others in that case
				if ( !bHost )
				{
					if (nRoleType == RoleRoomType_Dancer || nRoleType == RoleRoomType_GM)
					{
						GameMsg_S2C_LiveRoomEnterPlayer msgOtherResponse;
						rRoleRoomCOM.EncodePlayerInfo( msgOtherResponse.m_PlayerInfo );
						pRoom->SendMsgToAll( &msgOtherResponse, &rRoleRoomCOM );
					}
					else if (nRoleType == RoleRoomType_Audience)
					{
						GameMsg_S2C_LiveRoomEnterAudience msgOtherResponse;
						rRoleRoomCOM.EncodeAudienceInfo( msgOtherResponse.m_AudienceInfo );
						pRoom->SendMsgToGM( &msgOtherResponse, &rRoleRoomCOM );
					}
				}
			}
		}
	}

	return nPos;
}

void CLiveRoomManager::_PlayerOutRoom(CRoomComponent& rOutRoleRoomCOM, int nOutType, int nOutTo, CRoomComponent * pKickedPlayerRoomCOM)
{
	CRoom* pRoom = rOutRoleRoomCOM.Room();
	if ( pRoom != NULL && (pRoom->IsLiveRoom() || pRoom->IsAuditionRoom()))
	{
		//bool bRoomTopState = pRoom->RoomTop();

		int nRoleRoomType = rOutRoleRoomCOM.RoleRoomType();
		int nRoleRoomPos = rOutRoleRoomCOM.RoleRoomPos();
		if (pRoom->IsLiveRoom() && nRoleRoomType == RoleRoomType_Audience)
		{
			nRoleRoomPos = (int)rOutRoleRoomCOM.AttrCOM()->GetRoleID();
		}
		CRoomComponent* pPlayer = pRoom->PlayerOut( nRoleRoomType, nRoleRoomPos );

		if ( pPlayer != NULL )
		{
            if ( pRoom->IsAuditionRoom() && pRoom->RoomState() == eRoomState_Playing )
            {
                GameMsg_S2G_AddPunisherPlayer s2gmsg;
                s2gmsg.m_nRoleID = pPlayer->AttrCOM()->GetRoleID();
                s2gmsg.m_nPunishTime = CLiveRoomDataManager::Instance().GetPunishedTimeByOffline();
                SendMsg2GroupServer( &s2gmsg );
            }

			_LogRoleDancing(pPlayer->AttrCOM()->GetRoleID(), CRoleDancingLog::EDancingAction_End,
				pRoom->PlayScene(), pRoom->TeamMode(), pRoom->PlayMusicID(), pRoom->PlayMusicMode(), pRoom->PlayMusicLevel(),
				pRoom->DancerCount(), -1, 0);
			pPlayer->OutRoom();

			// notice player
			GameMsg_S2C_QuitLiveRoomSuc msgPlayerResponse;
			msgPlayerResponse.m_chRoomRoleType = (char)nRoleRoomType;
			msgPlayerResponse.m_chRoomRolePos = (char)nRoleRoomPos;
			msgPlayerResponse.m_nRoleID = rOutRoleRoomCOM.AttrCOM()->GetRoleID();
			msgPlayerResponse.m_chQuitType = (char)nOutType;
			msgPlayerResponse.m_chQuitTo = (char)nOutTo;

			if ( nOutType != RoomQuitType_Logout )
			{
				if ( pPlayer->AttrCOM() != NULL )
				{
					m_LobbyPlayer.insert( std::make_pair( pPlayer->AttrCOM()->GetRoleID(), pPlayer ) );
					CLocationMgr::Instance().EnterZone( Zone_LiveLobby, pPlayer->AttrCOM()->GetRoleID());
					_NoticeGroupState( pPlayer->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None );

					if (pPlayer->FriendCOM() != NULL) {
						pPlayer->FriendCOM()->PushOnlineInfoToAllFriends();
					}
				}

				pPlayer->SendPlayerMsg( &msgPlayerResponse );
			}

			// notice others
			if ( pRoom->HasDancer() || pRoom->HasAudience() || pRoom->HasGM() )
			{
				pRoom->SendMsgToAll( &msgPlayerResponse, NULL );
			}

			if ( !pRoom->HasDancer() )
			{
				_RoomEnd(pRoom);
				if (pRoom->IsAuditionRoom())
				{
					_AutoEnd(pRoom);
				}
			}

			if ( pRoom->RoomState() == eRoomState_Waiting && pRoom->IsPlayReady() )
			{
				_OutStartingList( pRoom );
				_RoomStart( pRoom );
			}

			if ( pRoom->RoomState() == eRoomState_Ending && pRoom->IsEndReady() )
			{
				if (!pRoom->IsLiveRoom())
				{
					_OutEndingList( pRoom );
				}
				_RoomEnd( pRoom );
			}
		}
		else
		{
			CEntityAttributeComponent* pAttrCOM = rOutRoleRoomCOM.AttrCOM();
			if ( pAttrCOM != NULL )
			{
				WriteLog(LOGLEVEL_ERROR, "Player quit room error, role id: %d, role name: %s, room id: %u, quit type: %d",
					pAttrCOM->GetRoleID(), pAttrCOM->GetRoleName(), pRoom->RoomID(), nOutType );
			}
		}
	}
}

int CLiveRoomManager::_PlayerChangeType(CRoomComponent& rRoleRoomCOM, int nRoleType)
{
	CRoom* pRoom = rRoleRoomCOM.Room();
	int nPos = -1;

	if ( pRoom != NULL )
	{
		//int nOriType = rRoleRoomCOM.RoleRoomType();
		//int nOriPos = rRoleRoomCOM.RoleRoomPos();

		nPos = pRoom->PlayerChangeType( rRoleRoomCOM, nRoleType );
		if ( nPos >= 0 )
		{		
			int nState = ( nRoleType == RoleRoomType_Dancer ? rRoleRoomCOM.RoleDefaultState() : RoleRoomState_Ready );
			rRoleRoomCOM.ChangeRoleType( nRoleType, nPos, nState );

			if (  rRoleRoomCOM.AttrCOM() != NULL )
			{
				_NoticeGroupState( rRoleRoomCOM.AttrCOM()->GetRoleID(), pRoom->RoomID(), nRoleType, nState );
			}
		}
	}

	return nPos;
}

void CLiveRoomManager::_PlayerChangeState(CRoomComponent& rRoleRoomCOM, int nRoleState, bool bNotice)
{
	CRoom* pRoom = rRoleRoomCOM.Room();
	if ( pRoom != NULL )
	{
		rRoleRoomCOM.ChangeRoleState( nRoleState );

		if ( rRoleRoomCOM.AttrCOM() != NULL )
		{
			_NoticeGroupState( rRoleRoomCOM.AttrCOM()->GetRoleID(), pRoom->RoomID(), rRoleRoomCOM.RoleRoomType(), nRoleState );
		}

		if ( bNotice )
		{
			GameMsg_S2C_ChangeRoleRoomStateSuc msgResponse;
			msgResponse.m_chRoleRoomType = (char)rRoleRoomCOM.RoleRoomType();
			msgResponse.m_chRoleRoomPos = (char)rRoleRoomCOM.RoleRoomPos();
			msgResponse.m_chRoleRoomState = (char)nRoleState;
			pRoom->SendMsgToAll( &msgResponse );
		}
	}
}

bool CLiveRoomManager::_ChangeRoomPosState(CRoom* pRoom, int nRoleRoomType, int nRoleRoomPos, int nRoomPosState)
{
	if ( pRoom != NULL )
	{
		if ( pRoom->ChangePosState( nRoleRoomType, nRoleRoomPos, nRoomPosState ) )
		{
			GameMsg_S2C_ChangeRoomPosStateSuc msgResponse;
			msgResponse.m_chRoleRoomType = (char)nRoleRoomType;
			msgResponse.m_chRoleRoomPos = (char)nRoleRoomPos;
			msgResponse.m_chRoomPosState = (char)pRoom->PosState( nRoleRoomType, nRoleRoomPos );
			pRoom->SendMsgToAll( &msgResponse );

			return true;
		}
	}

	return false;
}

void CLiveRoomManager::_PlayerOutLobby(CRoomComponent& rRoleRoomCOM)
{
	if ( rRoleRoomCOM.AttrCOM() != NULL )
	{
		m_LobbyPlayer.erase( rRoleRoomCOM.AttrCOM()->GetRoleID() );
		CLocationMgr::Instance().RemoveEntity( Zone_LiveLobby, rRoleRoomCOM.AttrCOM()->GetRoleID());
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Player out lobby error, attribute component invalid -----" );
	}
}

CRoomComponent* CLiveRoomManager::GetLobbyPlayer(unsigned int nRoleID)
{
	PlayerMap::iterator it = m_LobbyPlayer.find( nRoleID );
	if ( it != m_LobbyPlayer.end() )
	{
		return it->second;
	}

	return NULL;
}

void CLiveRoomManager::_InStartingList(CRoom* pRoom)
{
	unsigned long lNowTime = m_nMgrNowTime;
	if (pRoom->IsLiveRoom())
	{
        lNowTime = m_nMgrNowTime + RoomSettingMgr::Instance().LiveRoomCountDownTime() * 1000;
	}
	m_NeedStartRooms.insert( std::make_pair( lNowTime, pRoom ) );
}

void CLiveRoomManager::_OutStartingList(CRoom* pRoom)
{
    if ( pRoom != NULL )
    {
        for ( TimeRoomMap::iterator it = m_NeedStartRooms.begin(); it != m_NeedStartRooms.end(); ++it )
        {
            if ( it->second != NULL && it->second->RoomID() == pRoom->RoomID() )
            {
                m_NeedStartRooms.erase( it );
                break;
            }
        }
    }
}

void CLiveRoomManager::_InEndingList(CRoom* pRoom)
{
    unsigned long lNowTime = m_nMgrNowTime + RoomSettingMgr::Instance().RoomEndWait();
	if (pRoom->IsLiveRoom())
	{
        lNowTime = m_nMgrNowTime + RoomSettingMgr::Instance().LiveRoomCloseTime() * 1000;
	}
	m_NeedEndRooms.insert( std::make_pair( lNowTime, pRoom ) );
}

void CLiveRoomManager::_OutEndingList(CRoom* pRoom)
{
	if ( pRoom != NULL )
	{
		for ( TimeRoomMap::iterator it = m_NeedEndRooms.begin(); it != m_NeedEndRooms.end(); ++it )
		{
			if ( it->second != NULL && it->second->RoomID() == pRoom->RoomID() )
			{
				m_NeedEndRooms.erase( it );
				break;
			}
		}
	}
}

void CLiveRoomManager::_RoomStart(CRoom* pRoom)
{
	if ( pRoom != NULL )
	{
		pRoom->RoomStart();

		for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
		{
			CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
			if ( pRoomCOM != NULL )
			{
				_PlayerChangeState( *pRoomCOM, RoleRoomState_Start, false );
				//pRoomCOM->AchieveCOM()->RoomStart(*pRoomCOM);
				_LogRoleDancing(pRoomCOM->AttrCOM()->GetRoleID(), CRoleDancingLog::EDancingAction_Start,
					pRoom->PlayScene(), pRoom->TeamMode(), pRoom->PlayMusicID(), pRoom->PlayMusicMode(), pRoom->PlayMusicLevel(),
					pRoom->DancerCount(), -1, 0);
				for ( int nSlot = 0; nSlot <= pRoom->MaxDancerPos(); ++nSlot ) {	// add recently playmate
					CRoomComponent *pPlayMateCOM = pRoom->Player( RoleRoomType_Dancer, nSlot );
					if (nSlot != nPos &&  pRoomCOM->FriendCOM() != NULL && pPlayMateCOM != NULL && pPlayMateCOM->AttrCOM() != NULL 
						&& !pRoomCOM->FriendCOM()->IsInBlackList(pPlayMateCOM->AttrCOM()->GetRoleID())) {
						pRoomCOM->FriendCOM()->AddRecentlyPlayMate(pPlayMateCOM->AttrCOM()->GetRoleID(), pPlayMateCOM->AttrCOM()->GetRoleName());	
					}
				}
			}
		}

		GameMsg_S2C_StartRoomSuc msgResponse;
		pRoom->SendMsgToAll( &msgResponse );
	}
}

int CLiveRoomManager::GetAuditionRoomResult( CRoom * pRoom, CRoomComponent * pRoomComponent)
{
    if ( pRoomComponent->RoleMarkInfo().IsLegalMark( pRoom->ChoosedMusicMode()) )
    {
        CRoomComponent * pOtherComponent = pRoom->GetAuditionRoomRivalPos( pRoomComponent );
        if ( pOtherComponent == NULL )
        {
            int nFailScore = CLiveRoomDataManager::Instance().m_nLossScore;
            int nBaseScore = CLiveRoomDataManager::Instance().m_nBaseScore;
            const AuditionPlayerInfo * pRivalPlayerInfo = pRoom->GetAuditionRivalPlayerInfo( pRoomComponent->AttrCOM()->GetRoleID());
            if ( pRivalPlayerInfo != NULL)
            {
                if ( pRivalPlayerInfo != NULL )
                {
                    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID( pRivalPlayerInfo->m_nRoleID ); 
                    if ( pRoleEntity != NULL )
                    {
                        pRoleEntity->GetRoleRoom()->UpdateAuditionScore( nFailScore ); 
                    }
                    else
                    {
                        GameMsg_S2G_SyncAuditionScore s2gmsg;
                        s2gmsg.m_nRoleID = pRivalPlayerInfo->m_nRoleID;
                        s2gmsg.m_nScore = nFailScore;
                        SendMsg2GroupServer( &s2gmsg );
                    }

                    int nNewAuditionScore = pRivalPlayerInfo->m_nAuditionScore + nFailScore ;
                    nNewAuditionScore = nNewAuditionScore > nBaseScore ? nNewAuditionScore : nBaseScore;
                    int nChangeScore = nNewAuditionScore - pRivalPlayerInfo->m_nAuditionScore;

                    // update database
                    if ( nChangeScore != 0 )
                    {
                        pRoomComponent->AddQuery(Query_LiveRoom_UpdateAuditionPoint, 
                            pRivalPlayerInfo->m_nRoleID, (void *)CLiveRoomDataManager::Instance().m_tLiveStartTime, 
                            nChangeScore, (void *)CLiveRoomDataManager::Instance().m_tLiveEndTime);
                    }

                    if ( (int)pRivalPlayerInfo->m_nAuditionScore > CLiveRoomDataManager::Instance().m_nBaseScore )
                    {
                        GameMsg_S2G_UpdateDancerPoint groupMsg;
                        groupMsg.m_nRoleID = pRivalPlayerInfo->m_nRoleID;
                        groupMsg.m_strRoleName = pRivalPlayerInfo->m_strRoleName;
                        groupMsg.m_nAdded = nNewAuditionScore;
                        SendMsg2GroupServer(&groupMsg);
                    }
                }
            }

            return CLiveRoomDataManager::Instance().m_nWinScore;
        }
        else
        {
            if ( !pOtherComponent->RoleMarkInfo().IsLegalMark( pRoom->ChoosedMusicMode()))
            {
                return CLiveRoomDataManager::Instance().m_nWinScore;
            }
            else
            {
                if ( pRoomComponent->RoleMarkInfo().Place() == 1 )
                {
                    return CLiveRoomDataManager::Instance().m_nWinScore;
                }
                else
                {
                    return CLiveRoomDataManager::Instance().m_nLossScore;
                }
            }
        }
    }
    else
    {
        CRoomComponent * pOtherComponent = pRoom->GetAuditionRoomRivalPos( pRoomComponent );
        if ( pOtherComponent == NULL )
        {
            return 0;
        }
        else
        {
            if ( !pOtherComponent->RoleMarkInfo().IsLegalMark( pRoom->ChoosedMusicMode()))
            {
                return 0;
            }
            else
            {
                return CLiveRoomDataManager::Instance().m_nLossScore;
            }
        }
    }
}

void CLiveRoomManager::_RoomEnd(CRoom* pRoom)
{
    if ( pRoom == NULL || !pRoom->HasStart() )
    {
        return;
    }

    _RoomRanking( pRoom );
    _RoomReward( pRoom );

    GameMsg_S2C_EndRoomSuc msgResponse;
    pRoom->GetSweethearsInfo(msgResponse.m_vecSweetheartsInfo);

    for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
    {
        CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
        if ( pRoomCOM != NULL )
        {
            _PlayerChangeState( *pRoomCOM, RoleRoomState_End, false );
            _LogRoleDancing(pRoomCOM->AttrCOM()->GetRoleID(), CRoleDancingLog::EDancingAction_End,
                pRoom->PlayScene(), pRoom->TeamMode(), pRoom->PlayMusicID(), pRoom->PlayMusicMode(), pRoom->PlayMusicLevel(),
                pRoom->DancerCount(), pRoomCOM->RoleMarkInfo().Place(), pRoomCOM->RoleMarkInfo().Mark());

            msgResponse.m_szDancerPlace[nPos] = (char)pRoomCOM->RoleMarkInfo().Place();
            msgResponse.m_szDancerGrade[nPos] = (char)pRoomCOM->RoleMarkInfo().Grade();
            msgResponse.m_szDancerMark[nPos] = pRoomCOM->RoleMarkInfo().Mark();
            msgResponse.m_szDancerExp[nPos] = pRoomCOM->RoleMarkInfo().Exp();
            msgResponse.m_szDancerPerfect[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Perfect );
            msgResponse.m_szDancerCool[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Cool );
            msgResponse.m_szDancerGood[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Good );
            msgResponse.m_szDancerBad[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Bad );
            msgResponse.m_szDancerMiss[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Miss );
            msgResponse.m_szDancerSpecial[nPos] = pRoomCOM->RoleMarkInfo().MaxSpecialCount();

            if ( pRoom->IsAuditionRoom() )
            {
                // calculate score and update memory
                int nScore = GetAuditionRoomResult( pRoom, pRoomCOM);
                int nOldAuditionScore =pRoomCOM->AuditionScore();
                pRoomCOM->UpdateAuditionScore( nScore );
                int nNewAuditionScore = pRoomCOM->AuditionScore();

                // send message
                GameMsg_S2C_SyncMyAuditionScore syncMsg;
                syncMsg.m_nTotalScore = nNewAuditionScore;
                syncMsg.m_nScore = nScore;
                pRoomCOM->SendPlayerMsg(&syncMsg);

                // update database
                pRoomCOM->AddQuery(Query_LiveRoom_UpdateAuditionPoint, 
                    pRoomCOM->AttrCOM()->GetRoleID(), (void *)CLiveRoomDataManager::Instance().m_tLiveStartTime, 
                    nNewAuditionScore - nOldAuditionScore, (void *)CLiveRoomDataManager::Instance().m_tLiveEndTime);

                // send message to group server
                GameMsg_S2G_UpdateDancerPoint groupMsg;
                groupMsg.m_nRoleID = pRoomCOM->AttrCOM()->GetRoleID();
                groupMsg.m_strRoleName = pRoomCOM->AttrCOM()->GetRoleName();
                groupMsg.m_nAdded = nNewAuditionScore;
                SendMsg2GroupServer(&groupMsg);
            }
        }
    }

    for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
    {
        CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
        if ( pRoomCOM != NULL )
        {
            if ( pRoomCOM->RoleMarkInfo().IsLegalMark(pRoom->PlayMusicMode()))
            {
                // 通知观察者跳舞的模式
                CommonParam param;
                param.SetParam(std::string("activeness"), std::string("dance_mode"));
                param.SetParam(std::string("dance_mode"), pRoom->PlayMusicMode());
                pRoomCOM->NotifyAll(param);
            }
            
            // 清理分数
            pRoomCOM->RoleMarkInfo().ClearMark();
        }
    }

    if ( pRoom->IsLiveRoom() && pRoom->HasDancer())
    {
        CLiveRoomRoundDancerScore roundScore;
        for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
        {
            CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
            if ( pRoomCOM != NULL )
            {
                CLiveRoomDancerScore score;
                score.m_strRoleName = pRoomCOM->AttrCOM()->GetRoleName();
                score.m_nHotValue = pRoomCOM->GetHotValue();
                score.m_score.m_szDancerPlace = msgResponse.m_szDancerPlace[nPos];
                score.m_score.m_szDancerGrade = msgResponse.m_szDancerGrade[nPos];
                score.m_score.m_szDancerMark = msgResponse.m_szDancerMark[nPos];
                score.m_score.m_szDancerExp = msgResponse.m_szDancerExp[nPos];
                score.m_score.m_szDancerPerfect = msgResponse.m_szDancerPerfect[nPos];
                score.m_score.m_szDancerCool = msgResponse.m_szDancerCool[nPos];
                score.m_score.m_szDancerGood = msgResponse.m_szDancerGood[nPos];
                score.m_score.m_szDancerBad = msgResponse.m_szDancerBad[nPos];
                score.m_score.m_szDancerMiss = msgResponse.m_szDancerMiss[nPos];
                score.m_score.m_szDancerSpecial = msgResponse.m_szDancerSpecial[nPos];

                roundScore.m_score[nPos] = score;
            }
        }

        roundScore.m_nRoundEndTime = (unsigned int)time(NULL);
        roundScore.m_nDancerCount = pRoom->DancerCount();
        pRoom->AddDancerScore(roundScore.m_nRoundEndTime, roundScore);
    }

    pRoom->SendMsgToAll( &msgResponse );
    pRoom->RoomEnd();
}

void CLiveRoomManager::_AutoStart(CRoom* pRoom)
{
	if ( pRoom != NULL )
	{
		//bool bRoomTopState = pRoom->RoomTop();

		// kick player
		int nKickerType = RoleRoomType_Dancer;
		std::vector<char> vecKickerPos;

		for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
		{
			CRoomComponent* pPlayer = pRoom->Player( nKickerType, nPos );
			if ( pPlayer != NULL && pPlayer->RoleRoomState() != RoleRoomState_ToStart )
			{

				vecKickerPos.push_back( (char)nPos );

				pRoom->PlayerOut( nKickerType, nPos );
				pPlayer->OutRoom();

				if ( pPlayer->AttrCOM() != NULL )
				{
					m_LobbyPlayer.insert( std::make_pair( pPlayer->AttrCOM()->GetRoleID(), pPlayer ) );
					CLocationMgr::Instance().EnterZone( Zone_LiveLobby, pPlayer->AttrCOM()->GetRoleID());
					_NoticeGroupState( pPlayer->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None );
				}

				// notice player
				GameMsg_S2C_QuitRoomSuc msgQuitResponse;
				msgQuitResponse.m_chQuitType = (char)RoomQuitType_StartKick;
				msgQuitResponse.m_chQuitTo = (char)RoomQuitTo_LiveLobby;
				pPlayer->SendPlayerMsg( &msgQuitResponse );
			}
		}

		// check whether to destroy room
		if ( pRoom->HasDancer() )
		{
			int nHostRoomType = RoleRoomType_None;
			int nHostRoomPos = -1;
			int nHostRoomState = RoleRoomState_None;

			GameMsg_S2C_QuitMany msgOtherResponse;
			msgOtherResponse.m_chQuitType = (char)RoomQuitType_StartKick;
			msgOtherResponse.m_chRoleRoomType = (char)nKickerType;
			msgOtherResponse.m_vecRoleRoomPos.assign( vecKickerPos.begin(), vecKickerPos.end() );
			msgOtherResponse.m_chHostRoomType = (char)nHostRoomType;
			msgOtherResponse.m_chHostRoomPos = (char)nHostRoomPos;
			msgOtherResponse.m_chHostRoomState = (char)nHostRoomState;
			msgOtherResponse.m_strRoomPwd.assign( "" );
			pRoom->SendMsgToAll( &msgOtherResponse);

			_RoomStart( pRoom );
		}
	}
}

void CLiveRoomManager::_AutoEnd(CRoom* pRoom)
{
    if ( pRoom == NULL)
    {
        return;
    }

    if ( pRoom->RoomState() == eRoomState_Ending )
    {
		_RoomEnd( pRoom );
    }

    DestroyRoom( pRoom->RoomTop(), pRoom->RoomID(), 7 );
}

void CLiveRoomManager::_RoomSwitch(int nOldMode, CRoom* pRoom)
{
	if ( pRoom != NULL && nOldMode != pRoom->ChoosedMusicMode() )
	{
		m_AllRooms[nOldMode].erase( pRoom->RoomID() );
		m_AllRooms[pRoom->ChoosedMusicMode()][pRoom->RoomID()] = pRoom;
	}
}

void CLiveRoomManager::_RoomRanking(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        std::list<CRoomComponent*> lstRanking;

        for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
        {
            CRoomComponent* pRoomCOM = pRoom->Player(RoleRoomType_Dancer, nPos);
            if (pRoomCOM != NULL)
            {
                int nGrade = _CalcuGrade(*pRoomCOM);
                pRoomCOM->RoleMarkInfo().WinGrade(nGrade);

                if (lstRanking.empty())
                {
                    lstRanking.push_back(pRoomCOM);
                }
                else
                {
                    for ( std::list<CRoomComponent*>::iterator it = lstRanking.begin(); it != lstRanking.end(); )
                    {
                        if (pRoomCOM->RoleMarkInfo().Mark() > (*it)->RoleMarkInfo().Mark())
                        {
                            lstRanking.insert( it, pRoomCOM );
                            break;
                        }
                        else if (pRoomCOM->RoleMarkInfo().Mark() == (*it)->RoleMarkInfo().Mark())
                        {
                            if (pRoomCOM->RoleMarkInfo().Grade() > (*it)->RoleMarkInfo().Grade())
                            {
                                lstRanking.insert(it, pRoomCOM);
                                break;
                            }
                            else if (pRoomCOM->RoleMarkInfo().Grade() == (*it)->RoleMarkInfo().Grade())
                            {
                                if (pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Perfect ) > (*it)->RoleMarkInfo().TotalRankCount( RoomRoundRank_Perfect ))
                                {
                                    lstRanking.insert( it, pRoomCOM );
                                    break;
                                }
                                else if ( pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Perfect ) == (*it)->RoleMarkInfo().TotalRankCount( RoomRoundRank_Perfect ) )
                                {
                                    if ( pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Good ) > (*it)->RoleMarkInfo().TotalRankCount( RoomRoundRank_Good ) )
                                    {
                                        lstRanking.insert( it, pRoomCOM );
                                        break;
                                    }
                                    else if ( pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Good ) == (*it)->RoleMarkInfo().TotalRankCount( RoomRoundRank_Good ) )
                                    {
                                        if ( pRoomCOM->RoleMarkInfo().TotalRankCount( RoomRoundRank_Cool ) > (*it)->RoleMarkInfo().TotalRankCount( RoomRoundRank_Cool ) )
                                        {
                                            lstRanking.insert( it, pRoomCOM );
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        ++it;
                        if ( it == lstRanking.end() )
                        {
                            lstRanking.push_back( pRoomCOM );
                        }
                    }
                }
            }
        }

        int nPlace = 1;
        for ( std::list<CRoomComponent*>::iterator it = lstRanking.begin(); it != lstRanking.end(); ++it )
        {
            (*it)->RoleMarkInfo().WinPlace(nPlace);
            ++nPlace;

            int nExp = 0;
            if ((*it)->ColorTeam() == EDanceColorTeam_None)
            {
                nExp = _CalcuExp(*(*it), lstRanking.size(), pRoom->PlayMusicMode(), pRoom->PlayMusicLevel());
            }
            else
            {
                nExp = pRoom->TeamExp((*it)->ColorTeam());
            }

            (*it)->RoleMarkInfo().WinExp(nExp);
        }
    }
}

void CLiveRoomManager::_RoomReward(CRoom* pRoom)
{
    if (pRoom == NULL)
        return;

    bool bGroupReward = ( pRoom->DancerCount() >= CDanceGroupDataMgr::Instance().MinRoomDancersGroupHonorBonusNeed() ? true : false );
    for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
    {
        CRoomComponent* pDancerRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
        if ( pDancerRoomCOM != NULL )
        {
            if ( pDancerRoomCOM->RoleMarkInfo().IsLegalMark( pRoom->PlayMusicMode() ) )
            {
                //经验奖励
                int nAddExp = pDancerRoomCOM->SendExpReward();
                pDancerRoomCOM->RoleMarkInfo().WinExp( nAddExp );

                // 金券奖励(宠物技能)
                pDancerRoomCOM->SendRoomCurrencyReward();

                //情侣奖励
                if ( pDancerRoomCOM->CoupleCOM() != NULL && pDancerRoomCOM->CoupleCOM()->PairID() != 0 )
                {
                    for ( int i = nPos + 1; i <= pRoom->MaxDancerPos(); ++i )
                    {
                        CRoomComponent* pCoupleRoomCOM = pRoom->Player( RoleRoomType_Dancer, i );
                        if ( pCoupleRoomCOM != NULL && pCoupleRoomCOM->RoleMarkInfo().IsLegalMark( pRoom->PlayMusicMode() ) )
                        {
                            if ( pCoupleRoomCOM->CoupleCOM() != NULL && pDancerRoomCOM->CoupleCOM()->PairID() == pCoupleRoomCOM->CoupleCOM()->PairID() )
                            {
                                int nPlace = MIN( pDancerRoomCOM->RoleMarkInfo().Place(), pCoupleRoomCOM->RoleMarkInfo().Place() );
                                pDancerRoomCOM->CoupleReward( pDancerRoomCOM, pCoupleRoomCOM, nPlace );
                            }
                        }
                    }
                }

                //舞团奖励
                if ( bGroupReward && pDancerRoomCOM->DanceGroupCOM() != NULL )
                {
                    if ( pDancerRoomCOM->RoleMarkInfo().Place() == 1 )
                    {
                        int nHonorBonus = CDanceGroupDataMgr::Instance().MemberMatchGroupHonorBonusLeve1();
                        if ( pDancerRoomCOM->GeneCOM() != NULL )
                        {
                            GeneData* pGeneData = pDancerRoomCOM->GeneCOM()->FindGene( CDanceGroupDataMgr::Instance().RoomGroupHonorBonusGeneID() );
                            if ( pGeneData != NULL )
                            {
                                nHonorBonus += pGeneData->nParam1;
                            }
                        }

                        pDancerRoomCOM->DanceGroupCOM()->ChangeDanceGroupHonor( nHonorBonus, EChangeDanceGroupHonorCause_Match, pDancerRoomCOM->AttrCOM()->GetRoleID() );
                    }
                }
            }
            else
            {
                pDancerRoomCOM->RoleMarkInfo().WinExp( 0 );
            }
        }
    }
}

int CLiveRoomManager::_CalcuGrade(CRoomComponent& rRoleRoomCOM)
{
    return rRoleRoomCOM.CalcuGrade();
}

unsigned int CLiveRoomManager::_CalcuExp(CRoomComponent& rRoleRoomCOM, int nDancerCount, int nMusicMode, int nMusicLevel)
{
    return (unsigned int)rRoleRoomCOM.CalcExp(nDancerCount, nMusicMode, nMusicLevel);
}

void CLiveRoomManager::_NoticePlayerChange(CRoom* pRoom)
{
	if ( pRoom != NULL )
	{
		for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
		{
			CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
			if ( pRoomCOM != NULL )
			{
				GameMsg_S2C_UpdateLiveRoomDancerSuc msg;
				pRoom->EncodeLivePlayerInfo(msg.m_info, pRoomCOM);
				pRoomCOM->SendPlayerMsg(&msg);
			}
		}

		std::map<unsigned int, CRoomComponent*> audienceMap = pRoom->LiveRoomAudicenceMap();
		for ( std::map<unsigned int, CRoomComponent*>::iterator it = audienceMap.begin();
			it != audienceMap.end(); ++it )
		{
			CRoomComponent* pRoomCOM = it->second;
			if ( pRoomCOM != NULL )
			{
				GameMsg_S2C_UpdateLiveRoomDancerSuc msg;
				pRoom->EncodeLivePlayerInfo(msg.m_info, pRoomCOM);
				pRoomCOM->SendPlayerMsg(&msg);
			}
		}

		for ( int nPos = 0; nPos <= pRoom->MaxGMPos(); ++nPos )
		{
			CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_GM, nPos );
			if ( pRoomCOM != NULL )
			{
				GameMsg_S2C_UpdateLiveRoomDancerSuc msg;
				pRoom->EncodeLivePlayerInfo(msg.m_info, pRoomCOM);
				pRoomCOM->SendPlayerMsg(&msg);
			}
		}
	}
}

void CLiveRoomManager::_NoticeGroupState(unsigned int nRoleID, unsigned int nRoomID, int nRoleRoomType, int nRoleRoomState)
{
	GameMsg_S2G_UpdateRoomInfo msgGroup;
	msgGroup.m_nRoleID = nRoleID;
	msgGroup.m_RoomUpdateInfo.m_nRoomID = nRoomID;
	msgGroup.m_RoomUpdateInfo.m_nRoleRoomType = nRoleRoomType;
	msgGroup.m_RoomUpdateInfo.m_nRoleRoomState = nRoleRoomState;

	SendMsg2GroupServer( &msgGroup );
}

//记录角色跳舞信息
void CLiveRoomManager::_LogRoleDancing(unsigned int nRoleID, CRoleDancingLog::EDancingAction eDancingAction,
								   int nScene, int nTeamMode, int nMusicID, int nMusicMode, int nMusicLevel, int nPlayerCount, int nRank, int nScore)
{
	CRoleDancingLog * pLog = new CRoleDancingLog();
	pLog->m_nRoleID = nRoleID;
	pLog->m_eDancingAction = eDancingAction;
	pLog->m_nScene = nScene;
	pLog->m_nMusicID = nMusicID;
	pLog->m_nMusicMode = nMusicMode;
	pLog->m_nMusicLevel = nMusicLevel;
	pLog->m_nPlayerCount = nPlayerCount;
	pLog->m_nRank = nRank;
	pLog->m_nScore = nScore;
	pLog->m_nTeamMode = nTeamMode;
	g_pQueryMgr->AddQuery(QUERY_LogRoleDancing, 0, pLog);
}

void CLiveRoomManager::InsertAuditionActionLog(CRoleAuditionLog::EAuditionAction eAction, unsigned int nRoleID, unsigned int nMatchRoleID)
{
    CRoleAuditionLog * pLog = new CRoleAuditionLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_eAction = eAction;
    pLog->m_nMatchPlayer = nMatchRoleID;

    g_pQueryMgr->AddQuery(QUERY_LogRoleAudition, 0, pLog);
}

void CLiveRoomManager::OnOpenVote(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	bool bOpen = ((GameMsg_C2S_OpenVote &)rMsg).m_bOpen;
	std::string strError;
	CRoom *pRoom = NULL;

	if (!rRoleRoomCOM.AttrCOM()->IsGM())
	{
		strError = "OpenVote_NoPriv";
	}
	else if ((pRoom = rRoleRoomCOM.Room()) == NULL || (!pRoom->IsLiveRoom()))
	{
		strError = "OpenVote_NoLiveRoom";
	}
	else
	{
		pRoom->OpenVote(bOpen);
		GameMsg_S2C_OpenVoteSuc suc;
		suc.m_bOpen = bOpen;
		pRoom->SendMsgToAll(&suc);

		std::string tip;
		if (bOpen)
		{	
			tip = CLocalization::Instance().GetString( "Live_Room_Vote_Open" );
		}
		else
		{
			tip = CLocalization::Instance().GetString( "Live_Room_Vote_Close" );
		}
		CChatManager::Instance().SendAnnounce(true, false, tip);
	}

	if (!strError.empty())
	{
		GameMsg_S2C_OpenVoteFail fail;
		fail.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg(&fail);
	}
}

void CLiveRoomManager::OnGetLiveRoomHotList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	unsigned char eListType = ((GameMsg_C2S_GetLiveRoomHotList &)rMsg).m_nListType;
	if (eListType == eLiveRoomScoreList_WholeServer)
	{
		SendMsg2GroupServer((GameMsg_Base*)&rMsg);
	}
	else if (eListType == eLiveRoomScoreList_LocalRoom)
	{
		if (rRoleRoomCOM.Room() != NULL)
		{
			rRoleRoomCOM.Room()->OnGetHotList(rRoleRoomCOM, ((GameMsg_C2S_GetLiveRoomHotList &)rMsg).m_nPage);
		}
	}
}

void CLiveRoomManager::OnGetLiveRoomAuditionScoreList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	SendMsg2GroupServer((GameMsg_Base*)&rMsg);
}

void CLiveRoomManager::OnGetMyAuditionScore(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	if (rRoleRoomCOM.AuditionScore() <= CLiveRoomDataManager::Instance().m_nBaseScore)
	{
		GameMsg_S2C_GetMyAuditionScore msg;
		msg.m_nScore = rRoleRoomCOM.AuditionScore();
		msg.m_nPlace = 0;
		msg.m_nRefreshTime = (int)time(NULL);
		rRoleRoomCOM.SendPlayerMsg(&msg);
	}
	else
	{
		SendMsg2GroupServer((GameMsg_Base *)&rMsg);
	}
}

void CLiveRoomManager::OnVote(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	unsigned int nTargetRoleID = ((GameMsg_C2S_Vote &)rMsg).m_nRoleID;
	int nItemCount = ((GameMsg_C2S_Vote &)rMsg).m_nItemCount;
	std::string strError;
	CRoom *pRoom = NULL;

	if ((pRoom = rRoleRoomCOM.Room()) == NULL || (!pRoom->IsLiveRoom()))
	{
		strError = "Vote_NoLiveRoom";
	}
	else if (!pRoom->IsOpenVote())
	{
		strError = "Vote_VoteClosed";
	}
	else if (!pRoom->IsLiveRoomDancer(nTargetRoleID))
	{
		strError = "Vote_NoSuchDancer";
	}
	else
	{
		CEntityItemComponent *pItemCOM = rRoleRoomCOM.ItemCOM();

		itemtype_t nItemID = CLiveRoomDataManager::Instance().m_nVoteItemID;
		int nExistsItemCount = (int)pItemCOM->GetItemCount(nItemID, false, false);
		if (nExistsItemCount < nItemCount || nItemCount <= 0)
		{
			strError = "Vote_NotEnoughItem";
		}
		else
		{
			if (pItemCOM->RemoveItemByType(nItemID, nItemCount, EItemAction_Del_LiveRoomVote))
			{
				int nHot = CLiveRoomDataManager::Instance().m_nVoteItemHotValue * nItemCount;
				g_pQueryMgr->AddQuery(Query_LiveRoom_UpdateDancerHot, 
					nTargetRoleID, (void *)CLiveRoomDataManager::Instance().m_tLiveStartTime, 
					nHot, (void *)CLiveRoomDataManager::Instance().m_tLiveEndTime);

				// update player rank if he/she is online
				CRoleEntity *pTargetRole = CPlayerManager::Instance().GetEntityByRoleID(nTargetRoleID);
				if (pTargetRole != NULL)
				{
					CRoomComponent *pRoleRoom = pTargetRole->GetComponent<CRoleRoom>();
					pRoleRoom->SetHotValue(pRoleRoom->GetHotValue() + nHot);
				}

				// update this room hot rank
				pRoom->OnUpdateDancerHot(nTargetRoleID, nHot);

				// update global rank
				GameMsg_S2G_UpdateDancerHot updateMsg;
				updateMsg.m_nRoleID = nTargetRoleID;
				updateMsg.m_strRoleName = pRoom->GetLiveRoomDancerName(nTargetRoleID);
				updateMsg.m_nAdded = nHot;
				SendMsg2GroupServer(&updateMsg);

				GameMsg_S2C_VoteSuc suc;
				rRoleRoomCOM.SendPlayerMsg(&suc);

				// announce
				if (nItemCount >= CLiveRoomDataManager::Instance().m_nAnounceItemCount 
                    && CLiveRoomDataManager::Instance().m_nAnounceItemCount > 0)
				{
					std::string strContent;
					SafeFormat(strContent, CLocalization::Instance().GetString( "Live_Room_Vote_Tip" ),
						rRoleRoomCOM.AttrCOM()->GetRoleName(), updateMsg.m_strRoleName.c_str(), nHot);

					CChatManager::Instance().SendAnnounce(true, false, strContent);
				}
			}
			else
			{
				strError = "Vote_UseItemFailed";
			}
		}
	}

	if (!strError.empty())
	{
		GameMsg_S2C_VoteFail fail;
		fail.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg(&fail);
	}
}

void CLiveRoomManager::_OnSyncAuditionScore( GameMsg_Base& rMsg, CSlotPeer&rSlotPeer )
{
    GameMsg_G2S_SyncAuditionScore g2smsg = (GameMsg_G2S_SyncAuditionScore&)rMsg;

    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(g2smsg.m_nRoleID);
    if ( pRoleEntity != NULL )
    {
        pRoleEntity->GetRoleRoom()->UpdateAuditionScore( g2smsg.m_nScore );
    }
}

void CLiveRoomManager::_OnLiveRoomJoinAudition(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    GameMsg_G2S_IsPunisherPlayer g2smsg  = (GameMsg_G2S_IsPunisherPlayer&)rMsg;

    std::string strError;
    extern CLogicCirculator* g_pCirculator;

    char chPhoneOS = g2smsg.m_chPhoneOS;
    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(g2smsg.m_nRoleID);
    if ( pRoleEntity != NULL )
    {
        if (chPhoneOS != OS_Ios && chPhoneOS != OS_Android && chPhoneOS != OS_WP8)
        {
            strError = "JoinAudition_UnknownOS";
        }
        else if (!g_pCirculator->IsEnableAudition())
        {
            strError = "JoinAudition_NotAuditionLine";
        }
        else if (!CLiveRoomDataManager::Instance().IsAuditionOpen())
        {
            strError = "JoinAudition_AuditionClosed";
        }
        else if ( g2smsg.m_nLeftPunishTime > 0 )
        {
            strError = "JoinAudition_AuditionPunished";
        }
        else
        {
            pRoleEntity->GetRoleRoom()->SetPhoneOS(chPhoneOS);
            if ( !pRoleEntity->GetRoleRoom()->IsMatching())
            {
                pRoleEntity->GetRoleRoom()->AutoMatch( true, MusicMode_None );
                m_matchMap.insert( std::make_pair(pRoleEntity->GetRoleRoom()->AuditionScore(), pRoleEntity->GetRoleRoom()) );

                // log
                InsertAuditionActionLog(CRoleAuditionLog::EAuditionAction_Match, pRoleEntity->GetRoleID(), 0);
            }
        }

        if (!strError.empty())
        {
            GameMsg_S2C_LiveRoomJoinAuditionFail fail;
            fail.m_strError = strError;
            fail.m_nLeftPunishedTime = g2smsg.m_nLeftPunishTime;
            pRoleEntity->SendPlayerMsg(&fail);
        }
    }
}

void CLiveRoomManager::OnLiveRoomJoinAudition(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    GameMsg_C2S_LiveRoomJoinAudition c2smsg = (GameMsg_C2S_LiveRoomJoinAudition&)rMsg;
    
    GameMsg_S2G_IsPunisherPlayer s2gmsg;
    s2gmsg.m_nRoleID = rRoleRoomCOM.AttrCOM()->GetRoleID();
    s2gmsg.m_cPhoneOS = c2smsg.m_chPhoneOS;
    s2gmsg.m_nLine = g_nLine;
    SendMsg2GroupServer( &s2gmsg );   
}

void CLiveRoomManager::_QuitAuditionList(CRoomComponent& rRoleRoomCOM)
{
	for ( AuditionMatchMap::iterator it = m_matchMap.begin(); it != m_matchMap.end(); ++it )
	{
		if ( it->second == &rRoleRoomCOM )
		{
			it->second->AutoMatch( false, MusicMode_None );
			m_matchMap.erase( it );
			return;
		}
	}
}

void CLiveRoomManager::OnLiveRoomCancelAudition(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	_QuitAuditionList(rRoleRoomCOM);
}

void CLiveRoomManager::OnGetLiveRoomHotListRes(GameMsg_Base& rMsg, CSlotPeer &rSlot)
{
	CRoleEntity *pEntity = CPlayerManager::Instance().GetPlayer(rMsg.nIndex);
	if (pEntity != NULL)
	{
		pEntity->SendPlayerMsg(&rMsg);
	}
}

void CLiveRoomManager::OnGetLiveRoomAuditionScoreListRes(GameMsg_Base& rMsg, CSlotPeer &rSlot)
{
	CRoleEntity *pEntity = CPlayerManager::Instance().GetPlayer(rMsg.nIndex);
	if (pEntity != NULL)
	{
		pEntity->SendPlayerMsg(&rMsg);
	}
}

void CLiveRoomManager::OnGetMyAuditionScoreRes(GameMsg_Base& rMsg, CSlotPeer &rSlot)
{
	CRoleEntity *pEntity = CPlayerManager::Instance().GetPlayer(rMsg.nIndex);
	if (pEntity != NULL)
	{
		GameMsg_S2C_GetMyAuditionScore *pMsg = (GameMsg_S2C_GetMyAuditionScore *)&rMsg;
		CRoomComponent *pRoomCOM = pEntity->GetComponent<CRoleRoom>();
		pMsg->m_nScore = pRoomCOM->AuditionScore();
		pEntity->SendPlayerMsg(pMsg);
	}
}

void CLiveRoomManager::_AuditionMatch()
{
	int nMatchPlayerCount = CLiveRoomDataManager::Instance().m_nMatchPlayerCount;

	while ((int)m_matchMap.size() >= nMatchPlayerCount && nMatchPlayerCount > 0)
	{
		CRoom* pRoom = _CreateMatchRoom();
		if (pRoom != NULL)
		{
            std::vector<CRoomComponent*> vecComponent;
            for (int i = 0; i < nMatchPlayerCount; ++i )
            {
                CRoomComponent* pPlayerRoomCOM = m_matchMap.begin()->second;
                m_matchMap.erase(m_matchMap.begin());

                if (pPlayerRoomCOM != NULL && pPlayerRoomCOM->Room() == NULL)
                {
                    pPlayerRoomCOM->AutoMatch( false, MusicMode_None );
                    _PlayerInRoom( *pPlayerRoomCOM, pRoom, RoleRoomType_Dancer, RoleRoomState_ToStart, false, RoomEnterType_Match );

                    vecComponent.push_back(pPlayerRoomCOM);
                }
            }

            if (vecComponent.size() == 2)
            {
                for (int i = 0; i < 2; ++i)
                {
                    // 匹配玩家信息记录
                    AuditionPlayerInfo playerinfo;
                    playerinfo.m_nRoleID = vecComponent[i]->AttrCOM()->GetRoleID();
                    playerinfo.m_strRoleName = vecComponent[i]->AttrCOM()->GetRoleName();
                    playerinfo.m_nAuditionScore = vecComponent[i]->AuditionScore();
                    pRoom->AddAudiationPlayerInfo( playerinfo );

                    // log
                    InsertAuditionActionLog(CRoleAuditionLog::EAuditionAction_MatchSuc, vecComponent[i]->AttrCOM()->GetRoleID(),
                        vecComponent[1-i]->AttrCOM()->GetRoleID());
                }
            }

			_PrepareMatchRoom( pRoom );
		}
		else
		{
			return;
		}
	}
}

void CLiveRoomManager::CheckDestroyRoom( CRoom * pRoom )
{
    if ( pRoom == NULL )
    {
        return;
    }

    std::map<unsigned int,CRoom*>::iterator it = m_mapDestroyedRoom.find( pRoom->RoomID() );
    if ( it != m_mapDestroyedRoom.end())
    {
        std::set<CRoom*>::iterator iter = m_setDestroyedRoom.find( it->second );
        if ( iter != m_setDestroyedRoom.end() )
        {
            m_setDestroyedRoom.erase( iter );
        }
        m_mapDestroyedRoom.erase( it );
    }

    std::set<CRoom*>::iterator itPointer = m_setDestroyedRoom.find( pRoom );
    if ( itPointer != m_setDestroyedRoom.end() )
    {
        std::map<unsigned int,CRoom*>::iterator iter = m_mapDestroyedRoom.begin();
        for ( ; iter != m_mapDestroyedRoom.end(); ++iter )
        {
            if ( iter->second == pRoom )
            {
                m_mapDestroyedRoom.erase( iter );
                break;
            }
        }
        m_setDestroyedRoom.erase( itPointer );
    }
}

CRoom* CLiveRoomManager::_CreateMatchRoom()
{
    CRoom* pRoom = NULL;
    unsigned int nRoomID = _GenerateRoomID(eRoomType_LiveRoomAudition);

    if ( nRoomID > 0 )
    {
        pRoom = new CRoom(eRoomType_LiveRoomAudition, nRoomID);
        CheckDestroyRoom( pRoom );
        pRoom->ChangeRoomInfo(CLocalization::Instance().GetString("DefaultRoomName").c_str(), "");
        pRoom->SetMatch();

        int nScene = CLiveRoomDataManager::Instance().RandomScence();
        int nMusicID = CLiveRoomDataManager::Instance().RandomMusic();
        int nLevel = CLiveRoomDataManager::Instance().m_nDifficulty;

        if ( nLevel == 0 )
        {
            nLevel = Random(MusicLevel_Easy, MusicLevel_Hard);
        }

        int nMode = CLiveRoomDataManager::Instance().RandomMode();
        if (nMode == MusicMode_None || nMode == MusicMode_Sweethearts)
        {
            nMode = MusicMode_Osu;
        }

        pRoom->ChangeChoosedScene(nScene);
        pRoom->ChangeChoosedMusic(nMusicID, nMode, nLevel);

        WriteLog(LOGLEVEL_DEBUG, "[MATCH ROOM] roomid=%d, musicid=%d, sceneid=%d, level=%d",
            nRoomID, nMusicID, nScene, nLevel);
    }

    return pRoom;
}

void CLiveRoomManager::_PrepareMatchRoom(CRoom* pRoom)
{
    if (pRoom == NULL)
    {
        return;
    }

    CRoomComponent* pHost = pRoom->AssignHost();
    if (pHost == NULL)
    {
        DestroyRoom( pRoom->RoomTop(), pRoom->RoomID(), 4 );
        return;
    }

    pHost->AsHost( true );

    AddRoom( pRoom );
    pRoom->RoomPrepare();

    WriteLog(LOGLEVEL_DEBUG, "playMusicId=%d choosedMusicId=%d choosedSceneId=%d",
        pRoom->PlayMusicID(), pRoom->ChoosedMusicID(), pRoom->ChoosedScene());

    std::vector<char> vecAndroidStage;
    std::vector<char> vecIosStage;
    std::vector<char> vecWinPhoneStage;
    const std::string& strStageFile = pRoom->PlayMusicStage();

    if (!strStageFile.empty() 
        && CMusicStageFileManager::LoadStage( strStageFile, vecAndroidStage, vecIosStage, vecWinPhoneStage ))
    {
        int nStageTime = CMusicStageFileManager::CalcuStageTime( pRoom->PlayMusicMode(), vecAndroidStage, vecIosStage, vecWinPhoneStage );
        pRoom->SetCheckTime( nStageTime );

        for ( int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos )
        {
            CRoomComponent* pRoomCOM = pRoom->Player( RoleRoomType_Dancer, nPos );
            if ( pRoomCOM != NULL )
            {
                GameMsg_S2C_LiveRoomPrepareAudition msgResponse;
                pRoom->EncodeWholeInfo( msgResponse.m_roomInfo, pRoomCOM->IsHost() );
                msgResponse.m_chScenceID = (char)pRoom->PlayScene();
                msgResponse.m_nMusicID = (short)pRoom->PlayMusicID();
                msgResponse.m_chMode = (char)pRoom->PlayMusicMode();
                msgResponse.m_chLevel = (char)pRoom->PlayMusicLevel();
                msgResponse.m_strCheckKey = pRoom->PlayCheckCode();

                if ( pRoomCOM->GetPhoneOS() == OS_Ios )
                {
                    msgResponse.m_vecStage.assign( vecIosStage.begin(), vecIosStage.end() );
                }
                else if ( pRoomCOM->GetPhoneOS() == OS_WP8 )
                {
                    msgResponse.m_vecStage.assign( vecWinPhoneStage.begin(), vecWinPhoneStage.end() );
                }
                else
                {
                    msgResponse.m_vecStage.assign( vecAndroidStage.begin(), vecAndroidStage.end() );
                }

                pRoomCOM->SendPlayerMsg( &msgResponse );
            }
        }
        if (pRoom->PlayMusicMode() == MusicMode_Sweethearts)
        {
            CRoomManager::StartSweetheartsMode(pRoom, vecAndroidStage, vecIosStage);
        }
    }
    else
    {
        DestroyRoom( pRoom->RoomTop(), pRoom->RoomID(), 3 );
    }
}

void CLiveRoomManager::OnQuitRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
	EQuitRoomRes eRes = QuitRoom_Success;
	std::string strError;

	CRoom* pRoom = rRoleRoomCOM.Room();
	if ( pRoom == NULL )
	{
		eRes = QuitRoom_NoRoom;
		strError.assign( "QuitRoom_NoRoom" );
	}
	else if ( pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
	{
		eRes = QuitRoom_HasStart;
		strError.assign( "QuitRoom_HasStart" );
	}
	else
	{
		GameMsg_C2S_QuitRoom& rQuitMsg = (GameMsg_C2S_QuitRoom&)rMsg;
		_PlayerOutRoom( rRoleRoomCOM, RoomQuitType_Active, rQuitMsg.m_chQuitTo, NULL );
	}

	if ( eRes != QuitRoom_Success )
	{
		GameMsg_S2C_QuitRoomFail msgResponse;
		msgResponse.m_strError = strError;
		rRoleRoomCOM.SendPlayerMsg( &msgResponse );
	}
}

void CLiveRoomManager::GetRoomListByGM(std::vector<GMRoomInfo>& roomList)
{
	AllRoomMap::iterator it = m_AllRooms[MusicLevel_None].begin();
	for ( ; it != m_AllRooms[MusicLevel_None].end() ; it++)
	{
		GMRoomInfo roomInfo;
		it->second->ToGMRoomInfo(roomInfo);
		roomList.push_back(roomInfo);
	}
}

