#include "MailGiftSystem.h"
#include <time.h>
#include "../util/CommonFunc.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../../socket/Log.h"
#include "WordMailManager.h"

CMailGiftInfo::CMailGiftInfo()
{
	m_nMailGiftID = 0;
	m_strTitle = "";
	m_strContent = "";
	m_nMoneyReward = 0;
}
CMailGiftInfo::~CMailGiftInfo()
{

}
//初始化
void CMailGiftInfo::Init(int nMailGiftID, const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward,
					 const char * szTitle, const char * szContent)
{
	m_nMailGiftID = nMailGiftID;
	m_strTitle = szTitle;
	m_strContent = szContent;
	//奖励
	m_nMoneyReward = nMoneyReward;
	if (!TransferItemList(szMaleReward, m_MaleRewardItemList))
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CMailGiftInfo::Init. Transfer male item list error. MailGiftID=%d, itemlist=%s", m_nMailGiftID, szMaleReward);
	}
	if (!TransferItemList(szFemaleReward, m_FemaleRewardItemList))
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CMailGiftInfo::Init. Transfer female item list error. MailGiftID=%d, itemlist=%s", m_nMailGiftID, szFemaleReward);
	}
}

//发送礼物给玩家
void CMailGiftInfo::SendMailGiftToRole(unsigned int nRoleID, bool bMale)
{
    if ( m_nMoneyReward > 0 )
        CWMailManager::Instance().SendSystemMailToPlayer( nRoleID, EMailType_ExternalSendMailGift, m_strTitle, m_strContent, m_nMoneyReward );

    CWMailManager::Instance().SendSystemMailToPlayer( nRoleID, EMailType_ExternalSendMailGift, m_strTitle, m_strContent,
        bMale ? m_MaleRewardItemList : m_FemaleRewardItemList );
}

CMailGiftSystem::CMailGiftSystem()
{
}

CMailGiftSystem::~CMailGiftSystem()
{
	std::map< int, CMailGiftInfo * >::iterator it = m_MailGiftRewardMap.begin();
	for (; it != m_MailGiftRewardMap.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_MailGiftRewardMap.clear();
}

CMailGiftSystem & CMailGiftSystem::Instance()
{
	static CMailGiftSystem s_Instance;
	return s_Instance;
}

//添加时间活动
void CMailGiftSystem::AddMailGift(CMailGiftInfo * pMailGiftInfo)
{
	if (pMailGiftInfo == NULL)
	{
		return;
	}
	std::map< int, CMailGiftInfo * >::iterator it = m_MailGiftRewardMap.find(pMailGiftInfo->m_nMailGiftID);
	if (it != m_MailGiftRewardMap.end())
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CMailGiftSystem::AddMailGift. The MailGift has added. MailGiftID=%d.", pMailGiftInfo->m_nMailGiftID);
		delete it->second;
		it->second = NULL;
		m_MailGiftRewardMap.erase(it);
	}
	m_MailGiftRewardMap[pMailGiftInfo->m_nMailGiftID] = pMailGiftInfo;
}

//获取礼物信息
CMailGiftInfo * CMailGiftSystem::GetMailGiftInfo(int nMailGiftID)
{
	std::map< int, CMailGiftInfo * >::iterator it = m_MailGiftRewardMap.find(nMailGiftID);
	if (it != m_MailGiftRewardMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

