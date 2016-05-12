#include "DanceGroupTimerActivity.h"
#include "../util/CommonFunc.h"
#include "../logic/PlayerManager.h"
#include "../../socket/Log.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../../datastructure/datainfor/EffectConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../dancegroup/GameMsg_S2G_DanceGroup.h"

void SendMsg2GroupServer(LPGameMsg_Base pMsg);

/////////////////////////////////////////////DanceGroupActivity
CDanceGroupActivity::CDanceGroupActivity()
{

}//end


CDanceGroupActivity::~CDanceGroupActivity()
{

}//end

void CDanceGroupActivity::Init( const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime )
{
	int nHour = 0;
	int nMinute = 0;
	sscanf(szTriggeringTime, "%2d:%2d", &nHour, &nMinute);

	CDayActivity::Init(nHour, nMinute, SetTime(szBeginTime), SetTime(szEndTime));
}//end

void CDanceGroupActivity::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
	CDayActivity::OnUpdate(lTimeElapsed, tNow);
}//end

void CDanceGroupActivity::OnTime(const unsigned long &lTimeElapsed, time_t tNow)
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: CDanceGroupActivity::OnTime." );
	this->_OnTime();
}//end

void CDanceGroupActivity::_OnTime()
{
	//std::list<unsigned int> roleList;
	//CPlayerManager::Instance().GetRoleIDList(roleList);
	/*
	//团徽每天消耗
	std::map< unsigned int, CDanceGroupInfo * > &dgList= CDanceGroupProcessor::Instance().DanceGroupMgr().GetDanceGroupInfoList();
	std::map< unsigned int, CDanceGroupInfo * >::iterator itor = dgList.begin(), itorEnd = dgList.end();
	for( ; itor != itorEnd; ++itor )
	{
		CDanceGroupInfo *pInfo = itor->second;
		if( pInfo != NULL )
		{
			unsigned short nEffect = pInfo->m_DGBaseInfo.m_nEffect;
			unsigned short nBadge = pInfo->m_DGBaseInfo.m_nBadge;

			CDanceGroupBadgeDoc *pBadgeInfo = CDataManager::Instance().GetEffectInfoMgr().GetDanceGroupBadgeByID( nEffect);
			CDanceGroupBadgeEffectDoc *pEffectInfo = CDataManager::Instance().GetEffectInfoMgr().GetDanceGroupBadgeEffectByID( nBadge );
			if( pBadgeInfo != NULL && pEffectInfo != NULL )
			{
				int nResA = pBadgeInfo->nParam7 + pEffectInfo->nParam7;
				int nResB = pBadgeInfo->nParam8 + pEffectInfo->nParam8;
				int nResC = pBadgeInfo->nParam9 + pEffectInfo->nParam9;
				if( nResA > 0 || nResB > 0 || nResC > 0 )
				{
					GameMsg_S2G_DanceGroupTuanhuiPerDayConsume msg;
					msg.m_nGroupId = pInfo->m_DGBaseInfo.m_nGroupID;
					msg.m_nResA = nResA;
					msg.m_nResB = nResB;
					msg.m_nResC = nResC;
					SendMsg2GroupServer( &msg );
				}
			}
		}
	}//end for
	*/
}//end


/////////////////////////////////////////////CDanceGroupRecord

CDanceGroupRecord::CDanceGroupRecord()
{

}//end

CDanceGroupRecord::~CDanceGroupRecord()
{

}//end

void CDanceGroupRecord::Init( const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime )
{
	int nHour = 0;
	int nMinute = 0;
	sscanf(szTriggeringTime, "%2d:%2d", &nHour, &nMinute);

	CDayActivity::Init(nHour, nMinute, SetTime(szBeginTime), SetTime(szEndTime));
}//end

void CDanceGroupRecord::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
	CDayActivity::OnUpdate(lTimeElapsed, tNow);
}//end

void CDanceGroupRecord::OnTime(const unsigned long &lTimeElapsed, time_t tNow)
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: CDanceGroupActivity::OnTime." );
	this->_OnTime();
}//end

void CDanceGroupRecord::_OnTime()
{
}//end
