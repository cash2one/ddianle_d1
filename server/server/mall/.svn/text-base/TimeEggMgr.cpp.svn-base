#include "TimeEggMgr.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../mail/WordMailManager.h"
#include "../libServerFrame/QueryManager.h"
#include "GameMsg_S2G_TimeEgg.h"
#include "GameMsg_G2S_TimeEgg.h"
#include "../../socket/GameMsg_Map.h"
#include "../../datastructure/Localization.h"
#include "../group/Global.h"
#include "../../socket/Formatter.h"
#include <sstream>

#define TimeEggMgrUpdateInterval 1000
extern CQueryManager* g_pQueryMgr;

CTimeEggMgr::CTimeEggMgr()
: GameMsg_Processor(true,true)
{
	m_lTimeElapsed = 0;
	m_bRecvSetting = false;
}

CTimeEggMgr::~CTimeEggMgr()
{
	for (EggMap::iterator it = m_mapEgg.begin(); it != m_mapEgg.end(); ++it)
	{
		delete it->second;
	}
	m_mapEgg.clear();
}

CTimeEggMgr& CTimeEggMgr::Instance()
{
	static CTimeEggMgr m;
	return m;
}

void CTimeEggMgr::Init()
{
	RegisterMessageProcessor();
	EggMap* pMap = new EggMap();
	g_pQueryMgr->AddQuery(Query_GetTimeEggs, 0, pMap);
	WriteLog(LOGLEVEL_DEBUG, "Load all time eggs");
}

void CTimeEggMgr::RegisterMessageProcessor()
{
	RegMsgCreatorAndProcessor(GameMsg_S2G_CreateTimeEgg, &CTimeEggMgr::OnCreateTimeEgg);
	RegMsgCreatorAndProcessor(GameMsg_S2G_OpenTimeEgg, &CTimeEggMgr::OnOpenTimeEgg);
	RegMsgCreatorAndProcessor(GameMsg_S2G_TimeEggSetting, &CTimeEggMgr::OnTimeEggSetting);
	RegQueryProcessor(Query_OpenTimeEgg, &CTimeEggMgr::OnDBAutoOpenEgg);
	RegQueryProcessor(Query_GetTimeEggs, &CTimeEggMgr::OnDBGetTimeEggs);
}

void CTimeEggMgr::OnUpdate( const unsigned long& lTimeElapsed )
{
	m_lTimeElapsed += lTimeElapsed;
	if (m_lTimeElapsed < TimeEggMgrUpdateInterval)
	{
		return;
	}
	m_lTimeElapsed = 0;
	std::vector<TimeEggEntry*> listOpenEgg;
	if (!m_mapEgg.empty() && m_bRecvSetting)
	{
		unsigned int nNow = (unsigned int)time(NULL);
		for (EggMap::iterator it = m_mapEgg.begin(); it != m_mapEgg.end(); ++it)
		{
			if ( it->second->m_nDestroyTime < nNow )
			{
				listOpenEgg.push_back(it->second);
			}
		}
		if ( !listOpenEgg.empty() )
		{
			for (std::vector<TimeEggEntry*>::iterator it = listOpenEgg.begin();
				it != listOpenEgg.end(); ++it)
			{
				TimeEggEntry* p = *it;
				OpenTimeEggEntry* pOpenTimeEggEntry = new OpenTimeEggEntry;
				pOpenTimeEggEntry->m_nEggID = p->m_nEggID;
				pOpenTimeEggEntry->m_nOpenType = OpenEggType_System;
				m_mapEgg.erase(p->m_nEggID);
				delete p;
				p = NULL;
				pOpenTimeEggEntry->nError = 1;
				g_pQueryMgr->AddQuery(Query_OpenTimeEgg, 0, pOpenTimeEggEntry, 0);
			}
		}
	}
}

void CTimeEggMgr::OnDBAutoOpenEgg( QueryBase &pQuery )
{
	OpenTimeEggEntry* egg = (OpenTimeEggEntry*)pQuery.m_pAttachment;
	if ( egg )
	{
		if ( egg->nError == 0 && egg->m_roleinfo.size() == MAX_EGG_PLAYER_COUNT )
		{
			std::list<CItem> listItem;
			for (size_t i = 0; i < MAX_EGG_PLAYER_COUNT; ++i)
			{
				RoleTimeEggEntry& selfEntry = egg->m_roleinfo[i];
				RoleTimeEggEntry& partnerEntry = egg->m_roleinfo[(i+1)%MAX_EGG_PLAYER_COUNT];
				if ( selfEntry.m_item.m_nItemType > 0 && selfEntry.m_item.m_nItemCount > 0 )
				{
					listItem.push_back(selfEntry.m_item);
				}
				if ( partnerEntry.m_nSex == ESexType_Male )
				{
					listItem.insert(listItem.end(), m_setting.m_listMaleItem.begin(), m_setting.m_listMaleItem.end());
				}
				else if ( partnerEntry.m_nSex == ESexType_Female )
				{
					listItem.insert(listItem.end(), m_setting.m_listFemaleItem.begin(), m_setting.m_listFemaleItem.end());
				}

                string title;
                SafeFormat( title, CLocalization::Instance().GetString( "Time_Egg_Mail_Title" ), selfEntry.m_strRoleName.c_str() );

                if ( m_setting.m_nFixedMoneyReward > 0 )
                {
                    CWMailManager::Instance().SendSystemMailToPlayer( partnerEntry.m_nRoleID, EMailType_TimeEgg, title,
                        selfEntry.m_words, m_setting.m_nFixedMoneyReward );
                }
                CWMailManager::Instance().SendSystemMailToPlayer( partnerEntry.m_nRoleID, EMailType_TimeEgg, title, selfEntry.m_words, listItem );

				CGroupRoleEntity* pPartnerEntry = CGroupPlayerManager::Instance().SearchRoleByID(partnerEntry.m_nRoleID);
				if ( pPartnerEntry )
				{
					GameMsg_G2S_OpenTimeEgg msg;
					msg.m_nEggID = egg->m_nEggID;
					pPartnerEntry->SendPlayerMsg(&msg);
				}
				listItem.clear();
			}
		}
		delete egg;
		egg = NULL;
	}
}

void CTimeEggMgr::OnDBGetTimeEggs( QueryBase &pQuery )
{
	EggMap* pMap = (EggMap*)pQuery.m_pAttachment;
	if ( pMap )
	{
		for (EggMap::iterator it = pMap->begin(); it != pMap->end(); ++it)
		{
			m_mapEgg.insert(make_pair(it->first, it->second));
		}
		delete pMap;
		pMap = NULL;
	}
}

void CTimeEggMgr::OnCreateTimeEgg( GameMsg_Base & Msg ,CSlotPeer & SlotPeer )
{
	GameMsg_S2G_CreateTimeEgg& m = (GameMsg_S2G_CreateTimeEgg&)Msg;
	EggMap* pMap = new EggMap();
	g_pQueryMgr->AddQuery(Query_GetTimeEggs, m.m_nEggID, pMap);
}

void CTimeEggMgr::OnOpenTimeEgg( GameMsg_Base & Msg ,CSlotPeer & SlotPeer )
{
	GameMsg_S2G_OpenTimeEgg& m = (GameMsg_S2G_OpenTimeEgg&)Msg;
	EggMap::iterator fit = m_mapEgg.find(m.m_nEggID);
	if ( fit != m_mapEgg.end() )
	{
		delete fit->second;
		m_mapEgg.erase(fit);
	}
}

void CTimeEggMgr::OnTimeEggSetting( GameMsg_Base & Msg ,CSlotPeer & SlotPeer )
{
	GameMsg_S2G_TimeEggSetting& m = (GameMsg_S2G_TimeEggSetting&)Msg;
	if ( !m_bRecvSetting )
	{
		m_setting = m.m_setting;
		m_bRecvSetting = true;
	}
}

