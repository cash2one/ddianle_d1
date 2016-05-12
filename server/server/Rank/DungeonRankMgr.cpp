#include "DungeonRankMgr.h"
#include "../libServerFrame/QueryManager.h"
#include "../Dungeon/GameMsg_S2G_Dungeon.h"
#include "../Dungeon/GameMsg_G2S_Dungeon.h"
#include "../../socket/GameMsg_Map.h"
#include "../friend/GameMsg_G2S_FriendLogin.h"
#include "../share/ServerMsgDef.h"
#include <algorithm>

extern CQueryManager * g_pQueryMgr;

CDungeonRankInfo::CDungeonRankInfo( DungeonRankInfo & rankinfo )
: CRankInfo(rankinfo.nRoleID, rankinfo.nParcloseID*PARCLOSEID_MAGICSTAR_RATE+rankinfo.nStarCount )
,m_DungeonRankInfo(rankinfo)
{

}

CDungeonRankInfo::~CDungeonRankInfo()
{
}

CDungeonRankMgr::CDungeonRankMgr()
:m_bLoadComplete(false)
{

}

CDungeonRankMgr::~CDungeonRankMgr()
{
}


void CDungeonRankMgr::LoadRank( unsigned int nRoleID )
{
	std::vector<DungeonRankInfo> * pRankPlayersVec = new std::vector<DungeonRankInfo>;
	if ( pRankPlayersVec != NULL )
	{
		g_pQueryMgr->AddQuery( Query_Load_FriendParcloseInfo, nRoleID, pRankPlayersVec );
	}
}

void CDungeonRankMgr::SetLoadComplete( bool bIsTrue )
{
	// 按时间排序导入
	std::vector<DungeonRankInfo>::iterator it = m_RankPlayers.begin();
	for ( ; it != m_RankPlayers.end(); ++it )
	{
		CDungeonRankInfo * pRankInfo = new CDungeonRankInfo( *it );
		if ( pRankInfo != NULL )
		{
            if (0 == CRankMgr::AddNewRankInfo(pRankInfo))
            {
                delete pRankInfo;
                pRankInfo = NULL;
            }
		}
	}

	m_bLoadComplete = bIsTrue;
}

void CDungeonRankMgr::UpdateDungeonInfo( unsigned int nRoleID, const std::string & strName, unsigned int nParcloseID, unsigned int nStarCount )
{
	if (!m_bLoadComplete)
	{
		WriteLog(LOGLEVEL_WARNING, "[WARNING] role dungeon rank loading....");
		return;
	}

	unsigned int nSrcRank = 0;
	unsigned int nDesRank = 0;
	CRankInfo * pRankInfo = CRankMgr::UpdateRankInfo( nRoleID, nParcloseID*PARCLOSEID_MAGICSTAR_RATE+nStarCount, nSrcRank, nDesRank );
	if ( pRankInfo != NULL )
	{
		CDungeonRankInfo * pDungeonInfo = (CDungeonRankInfo*)pRankInfo;
		pDungeonInfo->m_DungeonRankInfo.nParcloseID = nParcloseID;
		pDungeonInfo->m_DungeonRankInfo.nStarCount = nStarCount;
	}
	else
	{
		DungeonRankInfo rankinfo;
		rankinfo.nRoleID = nRoleID;
		rankinfo.nParcloseID = nParcloseID;
		rankinfo.nStarCount = nStarCount;
		rankinfo.strRoleName = strName;

		std::vector<DungeonRankInfo>::iterator it = find( m_RankPlayers.begin(), m_RankPlayers.end(), rankinfo );
		if ( it == m_RankPlayers.end() )
		{
			CDungeonRankInfo * pRankInfo = new CDungeonRankInfo( rankinfo );
			if ( pRankInfo != NULL )
			{
                if (0 == CRankMgr::AddNewRankInfo(pRankInfo))
                {
                    delete pRankInfo;
                    pRankInfo = NULL;
                }
			}

			m_RankPlayers.push_back( rankinfo );
		}
	}
}

void CDungeonRankMgr::RemoveDungeonInfo( unsigned int nRoleID )
{
	if (!m_bLoadComplete)
	{
		WriteLog(LOGLEVEL_WARNING, "[WARNING] role dungeon rank loading....");
		return;
	}

	CRankInfo * pRankInfo = CRankMgr::RemoveRankInfo( nRoleID );
	if ( pRankInfo != NULL )
	{
		CDungeonRankInfo * pDungeonRankInfo = (CDungeonRankInfo*)pRankInfo;
		delete pDungeonRankInfo, pDungeonRankInfo = NULL;
	}

	DungeonRankInfo rankinfo;
	rankinfo.nRoleID = nRoleID;
	std::vector<DungeonRankInfo>::iterator it = find( m_RankPlayers.begin(), m_RankPlayers.end(), rankinfo );
	if ( it != m_RankPlayers.end() )
	{
		m_RankPlayers.erase( it );
	}
}

void CDungeonRankMgr::AddFriend( unsigned int nRoleID, DungeonRankInfo & rankInfo )
{
	if (!m_bLoadComplete)
	{
		WriteLog(LOGLEVEL_WARNING, "[WARNING] role dungeon rank loading....");
		return;
	}

	std::vector<DungeonRankInfo>::iterator it = find( m_RankPlayers.begin(), m_RankPlayers.end(), rankInfo );
	if ( it == m_RankPlayers.end() )
	{
 		CDungeonRankInfo * pRankInfo = new CDungeonRankInfo( rankInfo );
 		if ( pRankInfo != NULL )
 		{
            if (0 == CRankMgr::AddNewRankInfo(pRankInfo))
            {
                delete pRankInfo;
                pRankInfo = NULL;
            }
 		}

		m_RankPlayers.push_back( rankInfo );
		
		// CRankMgr::Reset();
		
		//std::vector<DungeonRankInfo>::iterator it = m_RankPlayers.begin();
		//for ( ; it != m_RankPlayers.end(); ++it )
		//{
		//	CDungeonRankInfo * pRankInfo = new CDungeonRankInfo( *it );
		//	if ( pRankInfo != NULL )
		//	{
		//		CRankMgr::AddNewRankInfo( pRankInfo );
		//	}
		//}
	}
}

void CDungeonRankMgr::GetRankList( std::list<CDungeonRankInfo*> & rankList )
{
	if (!m_bLoadComplete)
	{
		WriteLog(LOGLEVEL_WARNING, "[WARNING] role dungeon rank loading....");
		return;
	}

	unsigned int nRankCount  = CRankMgr::TotalCount();

	std::list< CRankInfo * > oRankInfoList;  
	CRankMgr::GetRange( 1, nRankCount, oRankInfoList);

	std::list<CRankInfo*>::iterator it = oRankInfoList.begin();
	for ( ; it != oRankInfoList.end(); ++it )
	{
		CDungeonRankInfo * pRankInfo = (CDungeonRankInfo*)*it;
		rankList.push_back( pRankInfo );
	}
}

unsigned int CDungeonRankMgr::GetSelfRank( unsigned int nRoleID )
{
	return CRankMgr::GetRankByKey( nRoleID );
}

CDungeonRankProcessor::CDungeonRankProcessor()
:GameMsg_Processor(true, true)
{
}

CDungeonRankProcessor::~CDungeonRankProcessor()
{

}

CDungeonRankProcessor & CDungeonRankProcessor::Instance()
{
	static CDungeonRankProcessor g_Instance;
	return g_Instance;
}

void CDungeonRankProcessor::_RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetRankList);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateParcloseInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ClearDungeonInviteFriendInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AddDungeonInviteFriendInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_RemoveDungeonInviteInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GiveVitToFriend);

}

void CDungeonRankProcessor::_RegisterMessageProcessor()
{
	RegQueryProcessor(Query_Load_FriendParcloseInfo,&CDungeonRankProcessor::OnLoadFriendDungeonInfo);


	RegMsgProcessor(MSG_S2G_GetRankList,&CDungeonRankProcessor::OnGetRankList);
	RegMsgProcessor(MSG_S2G_UpdateParcloseInfo,&CDungeonRankProcessor::OnUpdateParcloseInfo);
    RegMsgProcessor(MSG_S2G_ClearInviteFriendInfo,&CDungeonRankProcessor::OnResetDungeonInviteFrienInfo);
    RegMsgProcessor(MSG_S2G_AddInviteFriendInfo,&CDungeonRankProcessor::OnAddDungeonInviteFriendInfo);
	RegMsgProcessor(MSG_S2G_DelInviteFriendInfo,&CDungeonRankProcessor::OnRemoveDungeonInviteInfo);
    RegMsgProcessor(MSG_S2G_GiveVitToFriend,&CDungeonRankProcessor::OnGiveVitToFriend);
}

void CDungeonRankProcessor::Init()
{
	//注册消息
	_RegisterMessage();

	//注册消息处理
	_RegisterMessageProcessor();
}

void CDungeonRankProcessor::PlayerLogin( CGroupRoleEntity & role )
{
	std::map<unsigned int,CDungeonRankMgr*>::iterator it = m_DungeonRankMgr.find( role.GetRoleID() );
	if ( it == m_DungeonRankMgr.end() )
	{
		CDungeonRankMgr * pDungeonRank = new CDungeonRankMgr;
		pDungeonRank->LoadRank( role.GetRoleID() );
		m_DungeonRankMgr[role.GetRoleID()] = pDungeonRank;
	}

    // sync dungeon invite friend
    std::map<unsigned int,CDungeonInviteInfo>::iterator iter = m_mapDungeonInviteInfo.find( role.GetRoleID());
    if ( iter != m_mapDungeonInviteInfo.end() )
    {
        GameMsg_G2S_SyncDungeonInviteFriendInfo syncinfo;
        syncinfo.m_DungeonInviteInfo = iter->second;
        _SendPlayerMsg( role.GetRoleID(), syncinfo );
    }
}

void CDungeonRankProcessor::PlayerLogout( CGroupRoleEntity & role )
{
	std::map<unsigned int,CDungeonRankMgr*>::iterator it = m_DungeonRankMgr.find( role.GetRoleID() );
	if ( it != m_DungeonRankMgr.end() )
	{
		delete it->second;it->second = NULL;
		m_DungeonRankMgr.erase( it );
	}
}

void CDungeonRankProcessor::OnLoadFriendDungeonInfo(QueryBase & rQuery)
{
	std::vector<DungeonRankInfo> * pRankPlayersVec = (std::vector<DungeonRankInfo> *)rQuery.m_pAttachment;
	if ( pRankPlayersVec != NULL )
	{
		unsigned int nRoleID = rQuery.m_nAttachment;
		std::map<unsigned int,CDungeonRankMgr*>::iterator it = m_DungeonRankMgr.find( nRoleID );
		if ( it != m_DungeonRankMgr.end() )
		{
			it->second->m_RankPlayers = *pRankPlayersVec;
			it->second->SetLoadComplete( true );
		}
	}

	delete (std::vector<DungeonRankInfo> *)rQuery.m_pAttachment;
	rQuery.m_pAttachment = NULL;
}

void CDungeonRankProcessor::DelFriend( unsigned int nRoleID, unsigned int nFriendID )
{
	std::map<unsigned int,CDungeonRankMgr*>::iterator it = m_DungeonRankMgr.find( nRoleID );
	if ( it != m_DungeonRankMgr.end() )
	{
		it->second->RemoveDungeonInfo( nFriendID );
	}
}

void CDungeonRankProcessor::AddFriend( unsigned int nRoleID, unsigned int nFriendID, DungeonRankInfo & rankInfo )
{
	std::map<unsigned int,CDungeonRankMgr*>::iterator it = m_DungeonRankMgr.find( nRoleID );
	if ( it != m_DungeonRankMgr.end() )
	{
		it->second->AddFriend( nFriendID, rankInfo );
	}
}

void CDungeonRankProcessor::OnUpdateParcloseInfo( GameMsg_Base &msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_UpdateParcloseInfo & s2gMsg = (GameMsg_S2G_UpdateParcloseInfo&)msg;

	CGroupRoleEntity * pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
	if ( pRoleEntity != NULL)
	{
		pRoleEntity->SetStarCount( s2gMsg.m_nStarCount );
		pRoleEntity->SetTopParcloseID( (unsigned short)s2gMsg.m_nParcloseID );

		//  更新自己的排行
		std::map<unsigned int,CDungeonRankMgr*>::iterator it = m_DungeonRankMgr.find( s2gMsg.m_nRoleID );
		if ( it != m_DungeonRankMgr.end() )
		{
			it->second->UpdateDungeonInfo( s2gMsg.m_nRoleID, pRoleEntity->GetName(), s2gMsg.m_nParcloseID, s2gMsg.m_nStarCount );
		}

		// 更新好友的排行
		const std::list<unsigned int> & friendlist = pRoleEntity->GetFriendList();
		std::list<unsigned int>::const_iterator iter = friendlist.begin();
		for ( ; iter != friendlist.end(); ++iter )
		{
			std::map<unsigned int,CDungeonRankMgr*>::iterator itfriend = m_DungeonRankMgr.find( *iter );
			if ( itfriend != m_DungeonRankMgr.end() )
			{
				itfriend->second->UpdateDungeonInfo( s2gMsg.m_nRoleID, pRoleEntity->GetName(), s2gMsg.m_nParcloseID, s2gMsg.m_nStarCount );
			}
		}
	}
}

void CDungeonRankProcessor::OnResetDungeonInviteFrienInfo( GameMsg_Base &msg, CSlotPeer & slotPeer )
{
    GameMsg_S2G_ClearDungeonInviteFriendInfo & g2smsg = (GameMsg_S2G_ClearDungeonInviteFriendInfo&)msg;

    std::map<unsigned int,CDungeonInviteInfo>::iterator it = m_mapDungeonInviteInfo.find( g2smsg.m_nRoleID );
    if ( it != m_mapDungeonInviteInfo.end() )
    {
        m_mapDungeonInviteInfo.erase( it );
    }
}

void CDungeonRankProcessor::OnAddDungeonInviteFriendInfo(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_AddDungeonInviteFriendInfo & s2gmsg = (GameMsg_S2G_AddDungeonInviteFriendInfo&)msg;

    m_mapDungeonInviteInfo[ s2gmsg.m_nRoleID ].m_nCostInviteNum++;
    m_mapDungeonInviteInfo[ s2gmsg.m_nRoleID ].m_nLastInviteTime = s2gmsg.m_nInviteTime;
    m_mapDungeonInviteInfo[ s2gmsg.m_nRoleID ].m_setInviteFriends.insert( s2gmsg.m_nFriendRoleID );
}

void CDungeonRankProcessor::OnRemoveDungeonInviteInfo(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_RemoveDungeonInviteInfo & g2smsg = (GameMsg_S2G_RemoveDungeonInviteInfo&)msg;

    std::map<unsigned int,CDungeonInviteInfo>::iterator it = m_mapDungeonInviteInfo.find( g2smsg.m_nRoleID );
    if ( it != m_mapDungeonInviteInfo.end() )
    {
        std::set<unsigned int>::iterator iter = it->second.m_setInviteFriends.find( g2smsg.m_nDeleteRoleID );
        if ( iter != it->second.m_setInviteFriends.end() )
        {
            it->second.m_setInviteFriends.erase( iter );
        }
    }
}

void CDungeonRankProcessor::OnGiveVitToFriend(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_GiveVitToFriend & s2gmsg = (GameMsg_S2G_GiveVitToFriend&)msg;

    std::list<unsigned int>::iterator it = s2gmsg.m_listFriendRoleID.begin();
    for (; it != s2gmsg.m_listFriendRoleID.end(); ++it)
    {
        CGroupRoleEntity * pFriendEntity = CGroupPlayerManager::Instance().SearchRoleByID(*it);
        if (pFriendEntity != NULL)
        {
            GameMsg_G2S_GiveVitToFriend g2smsg;
            g2smsg.m_nFriendRoleID = s2gmsg.m_nGiveVitRoleID;
            g2smsg.m_strFriendRoleName = s2gmsg.m_strGiveVitRoleName;

            pFriendEntity->SendPlayerMsg(&g2smsg);
        }
        else
        {
            unsigned int * pContactInfo  = new unsigned int[2];
            *pContactInfo = s2gmsg.m_nGiveVitRoleID;
            *(pContactInfo + 1) = s2gmsg.m_nVitNum;
            g_pQueryMgr->AddQuery(Query_Update_Offline_FriendContactInfo, *it, pContactInfo);
        }
    }
}

int CDungeonRankProcessor::GetRank( unsigned int nRoleID )
{
	std::map<unsigned int,CDungeonRankMgr*>::iterator it = m_DungeonRankMgr.find( nRoleID );
	if ( it != m_DungeonRankMgr.end() )
	{
		return it->second->GetSelfRank(nRoleID);
	}

	return -1;
}

void CDungeonRankProcessor::OnGetRankList(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_GetRankList & s2gMsg = (GameMsg_S2G_GetRankList &)msg;

	std::map<unsigned int,CDungeonRankMgr*>::iterator it = m_DungeonRankMgr.find( s2gMsg.m_nRoleID );
	if ( it != m_DungeonRankMgr.end() )
	{
		std::list<CDungeonRankInfo*> rankList;
		it->second->GetRankList( rankList );

		GameMsg_G2S_GetRankListResult resultMsg;
		std::list<CDungeonRankInfo*>::iterator iter = rankList.begin();
		for ( ; iter != rankList.end(); ++iter )
		{
			CDungeonRankInfo * pDungeonRankInfo = *iter;

			DungeonRankInfo info;
			info.nParcloseID = pDungeonRankInfo->m_DungeonRankInfo.nParcloseID;
			info.strRoleName = pDungeonRankInfo->m_DungeonRankInfo.strRoleName;
			info.nStarCount = pDungeonRankInfo->m_DungeonRankInfo.nStarCount;
			info.nRoleID = pDungeonRankInfo->m_DungeonRankInfo.nRoleID;

			resultMsg.m_RankList.push_back( info );
		}

		resultMsg.m_nSelfRank = GetRank( s2gMsg.m_nRoleID );
		resultMsg.m_nCurrentPage = s2gMsg.m_nCurrentPage;

		_SendPlayerMsg( s2gMsg.m_nRoleID, resultMsg );
	}
	else
	{
		GameMsg_G2S_GetRankListResult resultMsg;
		_SendPlayerMsg( s2gMsg.m_nRoleID, resultMsg );
	}
}

void CDungeonRankProcessor::_SendPlayerMsg(unsigned int nRoleID, GameMsg_Base & rMsg)
{
	CGroupRoleEntity * pRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
	if (pRole != NULL)
	{
		pRole->SendPlayerMsg(&rMsg);
	}
}

