#include "LiveRoomRankManager.h"
#include "../group/Global.h"
#include "../../socket/SocketDef.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Log.h"
#include "../libServerFrame/QueryManager.h"
#include "../room/GameMsg_LiveRoom.h"
#include "../share/ServerMsgDef.h"
#include "../util/CommonFunc.h"
#include <time.h>


CLiveRoomRankManager::CLiveRoomRankManager()
: GameMsg_Processor(true, true)
{
	m_bInited = false;
	m_nStartTime = 0;
	m_nEndTime = 0;
	m_nLeaderBoardSize = 0;
	m_nTideUpdateInterval = 0;
	m_nTideUpdateTime = 0;
	m_nFirstUpdateTime = 0;
	m_bHasUpdateBoard = false;
    m_nAuditionBaseScore = 0;
}

CLiveRoomRankManager::~CLiveRoomRankManager()
{
}

CLiveRoomRankManager & CLiveRoomRankManager::Instance()
{
	static CLiveRoomRankManager s_instance;
	return s_instance;
}

void CLiveRoomRankManager::RegMessageMap()
{
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_GetLiveRoomHotList );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_GetLiveRoomAuditionScoreList );
	GAMEMSG_REGISTERCREATOR( GameMsg_C2S_GetMyAuditionScore );
	GAMEMSG_REGISTERCREATOR( GameMsg_S2G_UpdateDancerHot );
	GAMEMSG_REGISTERCREATOR( GameMsg_S2G_UpdateDancerPoint );
    GAMEMSG_REGISTERCREATOR( GameMsg_S2G_AddPunisherPlayer );
    GAMEMSG_REGISTERCREATOR( GameMsg_S2G_IsPunisherPlayer );
    GAMEMSG_REGISTERCREATOR( GameMsg_S2G_SyncAuditionScore );

	RegMsgProcessor( MSG_C2S_GetLiveRoomHotList, &CLiveRoomRankManager::OnGetHotList );
	RegMsgProcessor( MSG_C2S_GetLiveRoomAuditionScoreList, &CLiveRoomRankManager::OnGetAuditionScoreList );
	RegMsgProcessor( MSG_C2S_GetMyAuditionScore, &CLiveRoomRankManager::OnGetMyAuditionScore );
	RegMsgProcessor( MSG_S2G_LiveRoom_UpdateDancerHot, &CLiveRoomRankManager::OnUpdateLiveRoomHot );
	RegMsgProcessor( MSG_S2G_LiveRoom_UpdateDancerPoint, &CLiveRoomRankManager::OnUpdateAuditionScore );
    RegMsgProcessor( MSG_S2G_LiveRoom_AddPunishedPlayer, &CLiveRoomRankManager::OnAddAuditionPunishedPlayer );
    RegMsgProcessor( MSG_S2G_LiveRoom_IsPunishedPlayer, &CLiveRoomRankManager::IsPunishedPlayer);
    RegMsgProcessor( MSG_S2G_LiveRoom_SyncAuditionScore, &CLiveRoomRankManager::OnSyncAuditionScore );

	RegQueryProcessor(Query_LiveRoom_LoadPointAndHot, &CLiveRoomRankManager::OnLoadPointAndHot);
}

void CLiveRoomRankManager::Init()
{
	RegMessageMap();

	extern CQueryManager* g_pQueryMgr;

	CAllLiveRoomScoreDB *pAllScore = new CAllLiveRoomScoreDB;
	g_pQueryMgr->AddQuery(Query_LiveRoom_LoadPointAndHot, m_nStartTime, pAllScore, m_nEndTime);
	WriteLog(LOGLEVEL_DEBUG, "Load audition point and dancer hot begin...");
}

void CLiveRoomRankManager::OnUpdate(int nTimeElapse)
{
	if (m_bHasUpdateBoard)
	{
		m_nTideUpdateTime += nTimeElapse;
		if (m_nTideUpdateTime >= m_nTideUpdateInterval)
		{
			m_nTideUpdateTime -= m_nTideUpdateInterval;
			DoUpdate();
		}
	}
	else
	{
		time_t tNow = time(NULL);
		struct tm tmNow;
		
		LocalTime(&tNow, &tmNow);

		int nDailySeconds = tmNow.tm_hour * 3600 + tmNow.tm_min * 60 + tmNow.tm_sec;
		if (nDailySeconds >= m_nFirstUpdateTime)
		{
			DoUpdate();
			m_bHasUpdateBoard = true;
		}
	}
}

void CLiveRoomRankManager::DoUpdate()
{
	m_pointRankManager.Update();
	m_hotRankManager.Update();
}


void CLiveRoomRankManager::SetLiveRoomConfig(
	const char *pstrStartTime, const char *pstrEndTime, 
	int nLeaderBoardSize, const char *pstrFirstUpdateTime, int nUpdateInterval,int nAuditionBaseScore)
{
	m_nStartTime = SetTime(pstrStartTime);
	m_nEndTime = SetTime(pstrEndTime);
	m_nLeaderBoardSize = nLeaderBoardSize;

	std::string strTimeBuff("2014-07-01 ");
	if (pstrFirstUpdateTime)
		strTimeBuff.append(pstrFirstUpdateTime);
	time_t tFirstUpdateTime = SetTime(strTimeBuff.c_str());
	struct tm tmFirstUpdateTime;
	LocalTime(&tFirstUpdateTime, &tmFirstUpdateTime);

	m_nFirstUpdateTime = tmFirstUpdateTime.tm_hour * 3600 + tmFirstUpdateTime.tm_min * 60 + tmFirstUpdateTime.tm_sec;
	m_nTideUpdateInterval = nUpdateInterval * 1000;
	m_nAuditionBaseScore = nAuditionBaseScore;
}

void CLiveRoomRankManager::OnLoadPointAndHot(QueryBase &rQuery)
{
    WriteLog(LOGLEVEL_DEBUG, "Load audition point and dancer hot begin...");
    CAllLiveRoomScoreDB *pAllScore = (CAllLiveRoomScoreDB *)rQuery.m_pAttachment;

    for (std::list<CLiveRoomScoreDB>::iterator it = pAllScore->m_listScore.begin();
        it != pAllScore->m_listScore.end(); ++it)
    {
        if ( it->m_nPoint > m_nAuditionBaseScore )
        {
            CAuditionPointRank rank;
            rank.m_nID = it->m_nRoleID;
            rank.m_strName = it->m_strRoleName;
            rank.m_nScore = it->m_nPoint;

            m_pointRankManager.Add(rank);
        }
    }

    for (std::list<CLiveRoomScoreDB>::iterator it = pAllScore->m_listScore.begin();
        it != pAllScore->m_listScore.end(); ++it)
    {
        if (it->m_nHot > 0)
        {
            CLiveRoomHotRank rank;
            rank.m_nID = it->m_nRoleID;
            rank.m_strName = it->m_strRoleName;
            rank.m_nScore = it->m_nHot;
            m_hotRankManager.Add(rank);
        }
    }

    m_bInited = true;
    WriteLog(LOGLEVEL_DEBUG, "Load audition point and dancer hot end, count=[%u]", (unsigned)pAllScore->m_listScore.size());

    delete (CAllLiveRoomScoreDB *)rQuery.m_pAttachment;
    rQuery.m_pAttachment = NULL;
}

void CLiveRoomRankManager::OnGetHotList(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
	static const int kHotListPageSize = 6;

	CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
	if (pEntity != NULL)
	{
		int nPage = ((GameMsg_C2S_GetLiveRoomHotList &)rMsg).m_nPage;

		int nTotalCount = m_hotRankManager.Count();
		int nTotalPage = nTotalCount / kHotListPageSize;
		if (nTotalCount % kHotListPageSize > 0)
		{
			nTotalPage++;
		}

		if (nPage >= nTotalPage)
		{
			nPage = nTotalPage - 1;
		}
		if (nPage < 0)
		{
			nPage = 0;
		}

		int nStartIndex = nPage * kHotListPageSize;
		std::list<CLiveRoomHotRank> hotList;
		m_hotRankManager.Range(nStartIndex, kHotListPageSize, hotList);

		GameMsg_S2C_GetLiveRoomHotListRes res;
		res.m_nCurPage = (unsigned short)nPage;
        res.m_nTotalPage = (unsigned short)nTotalPage;
		for (std::list<CLiveRoomHotRank>::iterator it = hotList.begin(); it != hotList.end(); ++it)
		{
			LiveRoomHotInfo info;
			info.m_nRoleID = it->m_nID;
			info.m_strRoleName = it->m_strName;
			info.m_nPlace = ++nStartIndex;
			info.m_nHot = it->m_nScore;
			res.m_liveRoomHotList.push_back(info);
		}

		pEntity->SendPlayerMsg(&res);
	}
}

void CLiveRoomRankManager::OnGetAuditionScoreList(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
	static const int kPointListPageSize = 6;

	CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
	if (pEntity != NULL)
	{
		int nPage = ((GameMsg_C2S_GetLiveRoomAuditionScoreList &)rMsg).m_nPage;
		bool bFirstOpen = ((GameMsg_C2S_GetLiveRoomAuditionScoreList &)rMsg).m_bIsFirstOpen;

		int nTotalCount = m_pointRankManager.Count();
		int nTotalPage = nTotalCount / kPointListPageSize;
		if (nTotalCount % kPointListPageSize > 0)
		{
			nTotalPage++;
		}

		if (nPage >= nTotalPage)
		{
			nPage = nTotalPage - 1;
		}
		if (nPage < 0)
		{
			nPage = 0;
		}

		if (bFirstOpen)
		{
			int nSelfRank = m_pointRankManager.Rank(pEntity->GetRoleID());
			if (nSelfRank >= 1)
			{
				nPage = (nSelfRank - 1) / kPointListPageSize;
			}
		}

		int nStartIndex = nPage * kPointListPageSize;
		std::list<CAuditionPointRank> pointList;
		m_pointRankManager.Range(nStartIndex, kPointListPageSize, pointList);

		GameMsg_S2C_GetLiveRoomAuditionScoreListRes res;
        res.m_nCurPage = (unsigned short)nPage;
        res.m_nTotalPage = (unsigned short)nTotalPage;
		for (std::list<CAuditionPointRank>::iterator it = pointList.begin(); it != pointList.end(); ++it)
		{
			LiveRoomAuditionScoreInfo info;
			info.m_nRoleID = it->m_nID;
			info.m_strRoleName = it->m_strName;
			info.m_nPlace = ++nStartIndex;
			info.m_nScore = it->m_nScore;
			if (it->m_nRankTide > 0)
			{
				info.m_nTide = LiveRoomAuditionScoreInfo::Up;
			}
			else if (it->m_nRankTide < 0)
			{
				info.m_nTide = LiveRoomAuditionScoreInfo::Down;
			}
			else
			{
				info.m_nTide = LiveRoomAuditionScoreInfo::NoChange;
			}
			res.m_liveRoomScoreList.push_back(info);
		}

		pEntity->SendPlayerMsg(&res);
	}
}

void CLiveRoomRankManager::OnGetMyAuditionScore(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
	CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
	if (pEntity != NULL)
	{
		GameMsg_S2C_GetMyAuditionScore res;
		res.m_nPlace = m_pointRankManager.Rank(pEntity->m_nRoleID);
		res.m_nRefreshTime = (unsigned int)time(NULL);
		pEntity->SendPlayerMsg(&res);
	}
}

void CLiveRoomRankManager::OnUpdateAuditionScore(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_S2G_UpdateDancerPoint & g2smsg = (GameMsg_S2G_UpdateDancerPoint&)rMsg;

    CAuditionPointRank rank;
    rank.m_nID = g2smsg.m_nRoleID;
    rank.m_strName = g2smsg.m_strRoleName;
    rank.m_nScore = g2smsg.m_nAdded;
    rank.m_nUpdateTime = (unsigned int)time(NULL);
    m_pointRankManager.Set(rank);

    // remove base score id
    CAuditionPointRank *pRemoveRank = (CAuditionPointRank *)m_pointRankManager.Find(g2smsg.m_nRoleID);
    if (pRemoveRank != NULL && pRemoveRank->m_nScore <= m_nAuditionBaseScore )
    {
        m_pointRankManager.Remove( pRemoveRank->m_nID );
    }
}

void CLiveRoomRankManager::OnUpdateLiveRoomHot(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
	unsigned int nRoleID = ((GameMsg_S2G_UpdateDancerHot &)rMsg).m_nRoleID;
	std::string strRoleName = ((GameMsg_S2G_UpdateDancerHot &)rMsg).m_strRoleName;
	int nAdded = ((GameMsg_S2G_UpdateDancerHot &)rMsg).m_nAdded;

	CLiveRoomHotRank *pRank = (CLiveRoomHotRank *)m_hotRankManager.Find(nRoleID);
	if (pRank == NULL)
	{
		CLiveRoomHotRank rank;
		rank.m_nID = nRoleID;
		rank.m_strName = strRoleName;
		rank.m_nScore = nAdded;
		rank.m_nUpdateTime = (unsigned int)time(NULL);
		m_hotRankManager.Set(rank);

		if (m_hotRankManager.Count() > m_nLeaderBoardSize)
		{
			m_hotRankManager.Remove(m_hotRankManager.Tail()->m_nID);
		}
	}
	else
	{
		CLiveRoomHotRank rank = *pRank;
		rank.m_nScore += nAdded;
		rank.m_nUpdateTime = (unsigned int)time(NULL);
		m_hotRankManager.Set(rank);
	}
}


void CLiveRoomRankManager::OnAddAuditionPunishedPlayer(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_S2G_AddPunisherPlayer & s2gmsg = (GameMsg_S2G_AddPunisherPlayer&)rMsg;
    m_AuditionPunishedPlayer[ s2gmsg.m_nRoleID ] = (unsigned int)time(NULL) + s2gmsg.m_nPunishTime;
}

void CLiveRoomRankManager::IsPunishedPlayer(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_S2G_IsPunisherPlayer & s2gmsg = (GameMsg_S2G_IsPunisherPlayer&)rMsg;

    unsigned int nLeftTime = 0;
    std::map<unsigned int,unsigned int>::iterator it = m_AuditionPunishedPlayer.find( s2gmsg.m_nRoleID );
    if ( it != m_AuditionPunishedPlayer.end() )
    {
        unsigned int nNowTime = (unsigned int)time(NULL);
        if ( nNowTime < it->second )
        {
            nLeftTime = it->second - nNowTime;
        }
        else
        {
            m_AuditionPunishedPlayer.erase( it );
        }
    }

    GameMsg_G2S_IsPunisherPlayer g2smsg;
    g2smsg.m_nRoleID = s2gmsg.m_nRoleID;
    g2smsg.m_nLeftPunishTime = nLeftTime;
    g2smsg.m_chPhoneOS = s2gmsg.m_cPhoneOS;
    SendMsg2GameServer(&g2smsg, s2gmsg.m_nLine );
}

void CLiveRoomRankManager::OnSyncAuditionScore(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_S2G_SyncAuditionScore s2gmsg = (GameMsg_S2G_SyncAuditionScore&)rMsg;
    
    CGroupRoleEntity * pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(s2gmsg.m_nRoleID);
    if ( pRoleEntity != NULL)
    {
        GameMsg_G2S_SyncAuditionScore g2smsg;
        g2smsg.m_nRoleID = s2gmsg.m_nRoleID;
        g2smsg.m_nScore = s2gmsg.m_nScore;

        SendMsg2GameServer(&g2smsg, pRoleEntity->GetLine() );
    }
}
