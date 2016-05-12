#include "../libServerFrame/QueryManager.h"
#include "WorldHost.h"
#include "../../datastructure/Localization.h"
#include "../chatGrp/WChat.h"
#include "../friend/WFriendManager.h"
#include "../attribute/WAttributeManager.h"
#include "../dancegroup/DanceGroupProcessor.h"
#include "../group/Global.h"
#include "../../socket/inc/SocketLib.h"
#include "../mail/WordMailManager.h"
#include "../../datastructure/Localization.h"
#include "../Couple/CoupleProcessor.h"
#include "../compensation/CompensationProcessor.h"
#include "../Rank/LiveRoomRankManager.h"
#include "../mall/TimeEggMgr.h"
#include "../Rank/DungeonRankMgr.h"
#include "../specialMallGrp/SpecialMallProcessor.h"
#include "../Rank/GlobalRankListManager.h"
#include "../activity/ActivityManager.h"
#include "../InfoCenter/InfoCenterManager.h"
#include "../CeremonyRoom/CeremonyBriefManager.h"
#include "../room/GroupRoomPrecessor.h"
#include "../Handbook/HandbookGlobalMgr.h"
#include "../dancegroup/DanceGroupChallengeProcessor.h"
#include "../dancegroup/BigMamaGroup.h"
#include "../DanceGroupFairyLand/DanceGroupFairyLandProcessor.h"


extern CIni g_Config;

//请将GameServer和GroupServer通信的消息定义在GroupMsgDef.h中或者另行添加文件！！！
//extern CMarriageManager*    g_pMarriageManager;

CWorldHost::~CWorldHost()
{
	//delete g_pMarriageManager,g_pMarriageManager=NULL;
}
bool CWorldHost::Initialize()
{
	CWChatMgr::Instance().Initialize();
	CWFriendManager::Instance().Initialize();
	CWAttributeManager::Instance().Initialize();
    CLocalization::Instance().Init();
    CWMailManager::Instance().Initialize();
    ActivityManager::Instance().Initialize();
	CDanceGroupProcessor::Instance().Init();
    DanceGroupChallengeProcessor::Instance().Initialize();
    CDanceGroupFairyLandProcessor::Instance().Initialize();
    GroupRoomProcessor::Instance().Init();
	CCoupleProcessor::Instance().Initialize();
	CLiveRoomRankManager::Instance().Init();
	CCompensationProcessor::Instance().InitCompensation();
	CTimeEggMgr::Instance().Init();
	CDungeonRankProcessor::Instance().Init();
    CSpecialMallProcessor::Instance().Initialize();
    CGlobalRankListManager::Instance().Initialize();
	InfoCenterManager::Instance().Initialize();

	m_nSilenceDelay = 0;

	return true;
}

void CWorldHost::OnServerMsg(LPGameMsg_Base pMsg, char nSvr)
{
}

void CWorldHost::OnUpdate(int& timeElapse)
{
	m_nSilenceDelay += timeElapse;
	if(m_nSilenceDelay > 1000)
	{
		time_t tNow = time(NULL);
		m_nSilenceDelay -= 1000;
		map< string, int >::iterator ittemp;
		for(map< string, int >::iterator it = m_mapSilence.begin(); it != m_mapSilence.end();)
		{
			 it->second --;
			 ittemp = it;
			 it++;
			 if(ittemp->second <= 0)
				 m_mapSilence.erase(ittemp);
		}
		CDanceGroupProcessor::Instance().OnUpdate(tNow);
        DanceGroupChallengeProcessor::Instance().OnUpdate( tNow );
		CTimeEggMgr::Instance().OnUpdate(timeElapse);
        CGlobalRankListManager::Instance().OnUpdate(tNow);
        CeremonyBriefManager::Instance().OnUpdate(tNow);
        RedEnvelopeManager::Instance().Update(tNow);
        BigMamaGroup::Instance().Update(tNow);
        CDanceGroupFairyLandProcessor::Instance().OnUpdate(tNow);
	}

	CLiveRoomRankManager::Instance().OnUpdate(timeElapse);
    CSpecialMallProcessor::Instance().OnUpdate(timeElapse);
    CWChatMgr::Instance().OnUpdate(timeElapse);
    CHandbookGlobalMgr::Instance().OnUpdate(timeElapse);
}

void CWorldHost::SetClosed()
{
    CGlobalRankListManager::Instance().SetClosed();
    CHandbookGlobalMgr::Instance().SetClosed();
}

void CWorldHost::OnGameServerConnected(unsigned short nSlot)
{
    CCompensationProcessor::Instance().OnGameServerConnected( nSlot );
}

//end file


