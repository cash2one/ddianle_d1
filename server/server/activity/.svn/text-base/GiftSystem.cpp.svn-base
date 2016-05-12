#include "SystemActivityManager.h"
#include "InTimeOnlineActivity.h"
#include <time.h>
#include "../util/CommonFunc.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../../socket/Log.h"
#include "../../datastructure/RoleEntity.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/ItemProcess.h"
#include "../mail/MailDefine.h"

CGiftInfo::CGiftInfo()
{
	m_nGiftID = 0;
	m_bSendOnLogin = false;
	m_strMessage = "";
	m_strTitle = "";
	m_strContent = "";
	m_nMoneyReward = 0;
}
CGiftInfo::~CGiftInfo()
{

}
//初始化
void CGiftInfo::Init(int nGiftID, bool bSendOnLogin, const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward,
					 const char * szMessage, const char * szTitle, const char * szContent)
{
	m_nGiftID = nGiftID;
	m_bSendOnLogin = bSendOnLogin;
	m_strMessage = szMessage;
	m_strTitle = szTitle;
	m_strContent = szContent;
	//奖励
	m_nMoneyReward = nMoneyReward;
	if (!TransferItemList(szMaleReward, m_MaleRewardItemList))
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CGiftInfo::Init. Transfer male item list error. GiftID=%d, itemlist=%s", m_nGiftID, szMaleReward);
	}
	if (!TransferItemList(szFemaleReward, m_FemaleRewardItemList))
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CGiftInfo::Init. Transfer female item list error. GiftID=%d, itemlist=%s", m_nGiftID, szFemaleReward);
	}
}

//发送礼物给玩家
void CGiftInfo::SendGiftToRole(CRoleEntity & rRole)
{
	CEntityAttributeComponent * pAttrCom = rRole.GetComponent< CRoleAttribute >();
	if (pAttrCom)
	{
		pAttrCom->ChangeMoney(m_nMoneyReward, EChangeMoneyCause_Add_Gift, 0);
	}
	std::list< CItem > & rItemList = rRole.GetSex() == ESexType_Male ? m_MaleRewardItemList : m_FemaleRewardItemList;
	CItemProcess::AddOrMailItems(rRole, rItemList, EItemAction_Add_Gift, m_nGiftID, true, EMailType_Add_Gift, m_strTitle, m_strContent);
}

CGiftSystem::CGiftSystem()
{
}

CGiftSystem::~CGiftSystem()
{
	std::map< int, CGiftInfo * >::iterator it = m_GiftRewardMap.begin();
	for (; it != m_GiftRewardMap.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_GiftRewardMap.clear();
}
//添加时间活动
void CGiftSystem::AddGift(CGiftInfo * pGiftInfo)
{
	if (pGiftInfo == NULL)
	{
		return;
	}
	std::map< int, CGiftInfo * >::iterator it = m_GiftRewardMap.find(pGiftInfo->m_nGiftID);
	if (it != m_GiftRewardMap.end())
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CGiftSystem::AddTimerActivity. The gift has added. GiftID=%d.", pGiftInfo->m_nGiftID);
		delete it->second;
		it->second = NULL;
		m_GiftRewardMap.erase(it);
	}
	m_GiftRewardMap[pGiftInfo->m_nGiftID] = pGiftInfo;
}

//获取礼物信息
CGiftInfo * CGiftSystem::GetGiftInfo(int nGiftID)
{
	std::map< int, CGiftInfo * >::iterator it = m_GiftRewardMap.find(nGiftID);
	if (it != m_GiftRewardMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

