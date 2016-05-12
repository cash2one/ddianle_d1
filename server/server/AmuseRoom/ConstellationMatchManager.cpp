#include "ConstellationMatchManager.h"
#include "ConstellationMatchDataManager.h"
#include "GameMsg_ConstellationMatch.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../chat/ChatManager.h"
#include "../../datastructure/LogStructDef.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/datainfor/AmuseRoomConfig.h"
#include "../../datastructure/Localization.h"
#include "AmuseRoomManager.h"

#include <string>

void BroadcastMsg(LPGameMsg_Base pMsg);

ConstellationMatchManager::ConstellationMatchManager(void)
{
	m_nMatchTime = 0;
	m_nNextNoticeTime = 0;
}

ConstellationMatchManager::~ConstellationMatchManager(void)
{
}

ConstellationMatchManager &ConstellationMatchManager::Instance()
{
	static ConstellationMatchManager s_instance;
	return s_instance;
}

void ConstellationMatchManager::OnUpdate(int nTimeElapse)
{
	m_nMatchTime += nTimeElapse;

	if (m_nMatchTime > ConstellationMatchDataManager::Instance().m_nMatchInterval
		&& ConstellationMatchDataManager::Instance().IsOpen())
	{
		m_nMatchTime -= ConstellationMatchDataManager::Instance().m_nMatchInterval;
		Match();
	}

	unsigned int nNow = (unsigned int)time(NULL);
	if (nNow >= m_nNextNoticeTime && ConstellationMatchDataManager::Instance().IsOpen())
	{
		GameMsg_S2C_GetConstellationMatchInfoRes msg;
		msg.m_nMinRoleLevel = (unsigned short)ConstellationMatchDataManager::Instance().m_nRoleLevelRequired;
		msg.m_bIsOpen = ConstellationMatchDataManager::Instance().IsOpen();
		msg.m_nLeftOpenTime = ConstellationMatchDataManager::Instance().LeftOpenTime();

		BroadcastMsg(&msg);

		m_nNextNoticeTime = nNow + msg.m_nLeftOpenTime + 2; // 加2秒防止到活动结束那一秒不断广播
	}
}

void ConstellationMatchManager::OnLogin(CAmuseRoomComponent &rComponent)
{
	GameMsg_S2C_GetConstellationMatchInfoRes msg;
	msg.m_nMinRoleLevel = (unsigned short)ConstellationMatchDataManager::Instance().m_nRoleLevelRequired;
	msg.m_bIsOpen = ConstellationMatchDataManager::Instance().IsOpen();
	msg.m_nLeftOpenTime = ConstellationMatchDataManager::Instance().LeftOpenTime();

	rComponent.SendPlayerMsg(&msg);
}

void ConstellationMatchManager::OnLogout(CAmuseRoomComponent &rComponent)
{
	m_LobbyPlayer.erase(rComponent.AttrCOM()->GetRoleID());
	QuitMatch(rComponent.AttrCOM()->GetRoleID());
}

void ConstellationMatchManager::OnEnterConstellationLobby(GameMsg_Base &rMsg, CAmuseRoomComponent &rComponent)
{
	const char *strError = NULL;

	if (!ConstellationMatchDataManager::Instance().IsOpen())
	{
		strError = "ConstellationMatch_Closed";
	}
	else if (rComponent.AttrCOM()->GetLevel() < ConstellationMatchDataManager::Instance().m_nRoleLevelRequired)
	{
		strError = "ConstellationMatch_Level_Too_Low";
	}
	else
	{
		m_LobbyPlayer[rComponent.AttrCOM()->GetRoleID()] = rComponent.AttrCOM()->m_nConstellation;

		GameMsg_S2C_EnterConstellationLobbySuc msg;
		msg.m_bFirstEnter = !rComponent.HasEnterConstellationLobby();
		msg.m_strFirstEnterTip = ConstellationMatchDataManager::Instance().m_strFirstEnterTip;
		msg.m_nMaxUseCount = (unsigned short)rComponent.MaxConstellationMatchCount();
		msg.m_nUseCount = (unsigned short)rComponent.ConstellationMatchCount();
		msg.m_strRule = ConstellationMatchDataManager::Instance().m_strRule;

		if (!rComponent.HasEnterConstellationLobby())
		{
			rComponent.SetEnterConstellationLobby();
		}

		rComponent.ShuffleCard();
		rComponent.EncodeConstellationCardInfo(msg.m_listCard);

		rComponent.SendPlayerMsg(&msg);

		LogRoleEnterLobby(rComponent.AttrCOM()->GetRoleID(), rComponent.AttrCOM()->GetRoleName());
	}

	if (strError != NULL)
	{
		GameMsg_S2C_EnterConstellationLobbyFail fail;
		fail.m_strError = strError;
		fail.m_nUseCount = (unsigned short)rComponent.ConstellationMatchCount();
		fail.m_nWillOpenTime = ConstellationMatchDataManager::Instance().WillOpenTime();
		rComponent.SendPlayerMsg(&fail);
	}
}

void ConstellationMatchManager::OnQuitConstellationLobby(GameMsg_Base &rMsg, CAmuseRoomComponent &rComponent)
{
	m_LobbyPlayer.erase(rComponent.AttrCOM()->GetRoleID());
	QuitMatch(rComponent.AttrCOM()->GetRoleID());
}

void ConstellationMatchManager::OnMatch(GameMsg_Base &rMsg, CAmuseRoomComponent &rComponent)
{
	const char *strError = NULL;
	unsigned short nCardIndex = ((GameMsg_C2S_ConstellationMatch &)rMsg).m_nCardIndex;
	SConstellationCardInfo *pCard = rComponent.GetConstellationCard(nCardIndex);
	int nMatchConstellation = -1;
	if (pCard != NULL)
	{
		nMatchConstellation = pCard->m_nConstellation;
	}

	if (!ConstellationMatchDataManager::Instance().IsOpen())
	{
		strError = "ConstellationMatch_Closed";
	}
	else if (rComponent.AttrCOM()->GetLevel() < ConstellationMatchDataManager::Instance().m_nRoleLevelRequired)
	{
		strError = "ConstellationMatch_Level_Too_Low";
	}
	else if (rComponent.ConstellationMatchCount() >= rComponent.MaxConstellationMatchCount())
	{
		strError = "ConstellationMatch_Exceed_Limit";
	}
	else if (pCard == NULL)
	{
		strError = "ConstellationMatch_Invalid_Card";
	}
	else if (!rComponent.ConstellationMatch(nCardIndex))
	{
		strError = "ConstellationMatch_Card_CoolDown";
	}
	else
	{
		AddMatch(rComponent.AttrCOM()->GetRoleID(), rComponent.AttrCOM()->m_nConstellation, nMatchConstellation);

		LogRoleUseCard(rComponent.AttrCOM()->GetRoleID(), rComponent.AttrCOM()->GetRoleName(), nMatchConstellation);
	}

	if (strError != NULL)
	{
		GameMsg_S2C_ConstellationMatchFail fail;
		fail.m_strError = strError;
		rComponent.SendPlayerMsg(&fail);
	}
}

void ConstellationMatchManager::AddMatch(unsigned nRoleID, int nConstellation, int nMatchConstellation)
{
	if (nConstellation >= 0 && nConstellation < MAX_CONSTELLATION_COUNT
		&& nMatchConstellation >= 0 && nMatchConstellation < MAX_CONSTELLATION_MATCH)
	{
		m_MatchSet[nMatchConstellation].insert(nRoleID);
		m_SelfSet[nConstellation].insert(nRoleID);
		m_AllMatchSet.insert(nRoleID);
	}
}

void ConstellationMatchManager::Match()
{
	for (int i = 0; i < MAX_CONSTELLATION_MATCH; i++)
	{
		for (std::set<unsigned int>::iterator it = m_MatchSet[i].begin(); it != m_MatchSet[i].end(); ++it)
		{
			if (!InQuitMatchList(*it))
			{
				unsigned int nMatchRoleID = GetMatchRoleID(*it, i);
				if (nMatchRoleID > 0)
				{
					OnMatchSuc(*it, nMatchRoleID);
				}
				else
				{
					OnMatchFail(*it);
				}
			}
		}
	}

	while (!m_QuitMatchRoleID.empty())
	{
		QuitMatch(*m_QuitMatchRoleID.begin());
	}
}

unsigned int ConstellationMatchManager::GetMatchRoleID(unsigned int nRoleID, int nMatchConstellation)
{
	CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
	if (pRole == NULL)
	{
		return 0;
	}
	CEntityAttributeComponent *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
	CEntityFriendComponent *pRoleFriend = pRole->GetComponent<CRoleFriend>();

	std::set<unsigned int> *pMatchSet = NULL;
	if (nMatchConstellation >= 0 && nMatchConstellation < MAX_CONSTELLATION_COUNT)
	{
		pMatchSet = &m_SelfSet[nMatchConstellation];
	}
	else
	{
		pMatchSet = &m_AllMatchSet;
	}

	if (pMatchSet == NULL)
	{
		return 0;
	}

	std::list<CRoleEntity *> properMatchList;
	for (std::set<unsigned int>::iterator itSet = pMatchSet->begin(); itSet != pMatchSet->end(); ++itSet)
	{
		if (*itSet != nRoleID && !InQuitMatchList(*itSet))
		{
			if (!pRoleFriend->IsFriend(*itSet))
			{
				CRoleEntity *pMatchRole = CPlayerManager::Instance().GetEntityByRoleID(*itSet);
				if (pMatchRole != NULL)
				{
					CAmuseRoomComponent *pMatchRoleAmuse = pMatchRole->GetComponent<CRoleAmuseRoom>();
					if (pMatchRoleAmuse->MatchedConstellation() == MAX_CONSTELLATION_COUNT
						|| pMatchRoleAmuse->MatchedConstellation() == pRoleAttr->m_nConstellation)
					{
						properMatchList.push_back(pMatchRole);
					}
				}
			}
		}
	}

	std::list<CRoleEntity *>::iterator itList;
	for (itList = properMatchList.begin(); properMatchList.size() > 1 && itList != properMatchList.end();) // 优先异性玩家，所以剔除同性的
	{
		CRoleEntity *pMatchRole = *itList;
		if (pMatchRole->GetSex() == pRoleAttr->GetSex())
		{
			properMatchList.erase(itList++);
		}
		else
		{
			++itList;
		}
	}
	for (itList = properMatchList.begin(); properMatchList.size() > 1 && itList != properMatchList.end();) // 优先未婚，所以剔除已婚的
	{
		CRoleEntity *pMatchRole = *itList;
		if (pMatchRole->IsMarried())
		{
			properMatchList.erase(itList++);
		}
		else
		{
			++itList;
		}
	}
	for (itList = properMatchList.begin(); properMatchList.size() > 1 && itList != properMatchList.end();) // 优先VIP，所以剔除非VIP
	{
		CRoleEntity *pMatchRole = *itList;
		if (!pMatchRole->IsVIP())
		{
			properMatchList.erase(itList++);
		}
		else
		{
			++itList;
		}
	}
	for (itList = properMatchList.begin(); properMatchList.size() > 1 && itList != properMatchList.end();) // 优先等级范围+-9的，所以剔除差距较大的
	{
		CRoleEntity *pMatchRole = *itList;
		if (abs(pMatchRole->GetLevel() - pRoleAttr->GetLevel()) > 9)
		{
			properMatchList.erase(itList++);
		}
		else
		{
			++itList;
		}
	}

	if (properMatchList.empty())
	{
		return 0;
	}

	return properMatchList.front()->GetRoleID();
}

bool ConstellationMatchManager::InQuitMatchList(unsigned int nRoleID) const
{
	return (m_QuitMatchRoleID.find(nRoleID) != m_QuitMatchRoleID.end());
}

void ConstellationMatchManager::OnMatchSuc(unsigned int nRoleID, unsigned int nMatchRoleID)
{
	AddQuitMatchList(nRoleID);
	AddQuitMatchList(nMatchRoleID);

	CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
	CRoleEntity *pMatchRole = CPlayerManager::Instance().GetEntityByRoleID(nMatchRoleID);

	if (pRole != NULL && pMatchRole != NULL)
	{
		// Amuse room
		int nSceneID = ConstellationMatchDataManager::Instance().RandomAmuseScene();
		if (nSceneID == 0)
		{
			WriteLog(LOGLEVEL_ERROR, "ERROR: invalid amuse scene id=%d", nSceneID);
			return;
		}
		CAmuseRoomSceneConfig* pSceneInfo = ConfigManager::Instance().GetAmuseMgr().GetAmuseSceneInfo( nSceneID );
		if ( pSceneInfo == NULL )
		{
			WriteLog(LOGLEVEL_ERROR, "ERROR: invalid amuse scene id=%d no scene!", nSceneID);
			return;
		}

		// Add Friend
		CEntityFriendComponent *pRoleFriend = pRole->GetComponent<CRoleFriend>();
		pRoleFriend->MakeOnlineFriend(nMatchRoleID, 1/*callby*/, 1/*callto*/);

		// Send message
		CAmuseRoomComponent *pRoleAmuse = pRole->GetComponent<CRoleAmuseRoom>();
		CAmuseRoomComponent *pMatchRoleAmuse = pMatchRole->GetComponent<CRoleAmuseRoom>();
		
		int nConstellation = (pRole->GetComponent<CRoleAttribute>())->m_nConstellation;
		int nMatchConstellation = (pMatchRole->GetComponent<CRoleAttribute>())->m_nConstellation;

		GameMsg_S2C_ConstellationMatchSuc suc;
		suc.m_nMatchRate = (unsigned char)ConstellationMatchDataManager::Instance().GetMatchRate(nConstellation, nMatchConstellation);
		suc.m_strWishWords = ConstellationMatchDataManager::Instance().RandomWishWords();
		suc.m_nSceneID = (unsigned short)nSceneID;

		suc.m_nRoleID = nMatchRoleID;
		suc.m_nConstellation = (unsigned char)nMatchConstellation;
		suc.m_nLevel = pMatchRole->GetLevel();
		suc.m_strRoleName = pMatchRole->GetRoleName();
		pRoleAmuse->RandomQuestion(suc.m_vecQuestion);
		pRole->SendPlayerMsg(&suc);

		LogRoleMatchSuc(nRoleID, pRole->GetRoleName(), pRoleAmuse->MatchedConstellation(), nMatchRoleID, pMatchRole->GetRoleName(), nSceneID, pSceneInfo->m_strSceneName);

		suc.m_nRoleID = nRoleID;
		suc.m_nConstellation = (unsigned char)nConstellation;
		suc.m_nLevel = pRole->GetLevel();
		suc.m_strRoleName = pRole->GetRoleName();
		pMatchRoleAmuse->RandomQuestion(suc.m_vecQuestion);
		pMatchRole->SendPlayerMsg(&suc);

		LogRoleMatchSuc(nMatchRoleID, pMatchRole->GetRoleName(), pMatchRoleAmuse->MatchedConstellation(), nRoleID, pRole->GetRoleName(), nSceneID, pSceneInfo->m_strSceneName);

		// create amuse room, set amuse room id
		pRoleAmuse->OnConstellationMatchSuc();
		pMatchRoleAmuse->OnConstellationMatchSuc();
		CAmuseRoomManager::Instance().CreateConstellationRoom(*pRoleAmuse, *pMatchRoleAmuse, nSceneID);
	}
}

void ConstellationMatchManager::OnMatchFail(unsigned int nRoleID)
{
	GameMsg_S2C_ConstellationMatchFail fail;
	fail.m_strError = "ConstellationMatch_Fail_Unknown";

	CRoleEntity *pPlayer = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
	std::string strRoleName;
	if (pPlayer != NULL)
	{
		pPlayer->SendPlayerMsg(&fail);
		strRoleName = pPlayer->GetRoleName();
	}
	AddQuitMatchList(nRoleID);

	LogRoleMatchFail(nRoleID, strRoleName);
}

void ConstellationMatchManager::AddQuitMatchList(unsigned int nRoleID)
{
	m_QuitMatchRoleID.insert(nRoleID);
}

void ConstellationMatchManager::QuitMatch(unsigned int nRoleID)
{
	int i = 0;
	for (i = 0; i < MAX_CONSTELLATION_COUNT; i++)
	{
		if (m_SelfSet[i].find(nRoleID) != m_SelfSet[i].end())
		{
			m_SelfSet[i].erase(nRoleID);
		}
	}
	for (i = 0; i < MAX_CONSTELLATION_MATCH; i++)
	{
		if (m_MatchSet[i].find(nRoleID) != m_MatchSet[i].end())
		{
			m_MatchSet[i].erase(nRoleID);
		}
	}
	m_AllMatchSet.erase(nRoleID);
	m_QuitMatchRoleID.erase(nRoleID);

	CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
	if (pRole != NULL)
	{
		CAmuseRoomComponent *pRoleAmuse = pRole->GetComponent<CRoleAmuseRoom>();
		pRoleAmuse->CancelConstellationMatch();
	}
}

void ConstellationMatchManager::OnEnterConstellationRoom(GameMsg_Base &rMsg, CAmuseRoomComponent &rComponent)
{
	CAmuseRoomManager::Instance().OnEnterConstellationRoom(rComponent);
}

void ConstellationMatchManager::LogRoleEnterLobby(unsigned int nRoleID, const std::string &strRoleName)
{
	CRoleConstellationMatchLog *pLog = new CRoleConstellationMatchLog;
	pLog->m_nRoleID = nRoleID;
	pLog->m_strRoleName = strRoleName;
	pLog->m_eAction = CRoleConstellationMatchLog::EConstellationMatchAction_EnterLobby;
	LogRoleAction(pLog);
}

void ConstellationMatchManager::LogRoleUseCard(unsigned int nRoleID, const std::string &strRoleName, int nCardConstellation)
{
	CRoleConstellationMatchLog *pLog = new CRoleConstellationMatchLog;
	pLog->m_nRoleID = nRoleID;
	pLog->m_strRoleName = strRoleName;
	pLog->m_nCardConstellation = nCardConstellation;
	pLog->m_eAction = CRoleConstellationMatchLog::EConstellationMatchAction_UseCard;
	LogRoleAction(pLog);
}

void ConstellationMatchManager::LogRoleMatchFail(unsigned int nRoleID, const std::string &strRoleName)
{
	CRoleConstellationMatchLog *pLog = new CRoleConstellationMatchLog;
	pLog->m_nRoleID = nRoleID;
	pLog->m_strRoleName = strRoleName;
	pLog->m_eAction = CRoleConstellationMatchLog::EConstellationMatchAction_MatchFail;
	LogRoleAction(pLog);
}

void ConstellationMatchManager::LogRoleMatchSuc(unsigned int nRoleID, const std::string &strRoleName, int nCardConstellation, 
												unsigned int nMatchRoleID, const std::string &strMatchRoleName, int nSceneID, 
												const std::string &strSceneName)
{
	CRoleConstellationMatchLog *pLog = new CRoleConstellationMatchLog;
	pLog->m_nRoleID = nRoleID;
	pLog->m_strRoleName = strRoleName;
	pLog->m_nCardConstellation = nCardConstellation;
	pLog->m_nMatchRoleID = nMatchRoleID;
	pLog->m_strMatchRoleName = strMatchRoleName;
	pLog->m_nSceneID = nSceneID;
	pLog->m_strSceneName = strSceneName;
	pLog->m_eAction = CRoleConstellationMatchLog::EConstellationMatchAction_MatchSuc;
	LogRoleAction(pLog);
}

void ConstellationMatchManager::LogRoleAction(ISQLLog *pLog)
{
	g_pQueryMgr->AddQuery(QUERY_LogRoleConstellationMatch, 0, pLog);
}

