#include "InTimeOnlineActivity.h"
#include "../util/CommonFunc.h"
#include "../logic/PlayerManager.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../../socket/Log.h"
#include "GameMsg_S2C_InTimeOnlineReward.h"
#include "../item/EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/ItemProcess.h"
#include "../mail/MailDefine.h"
#include "InTimeOnlineInfoMgr.h"

CInTimeOnlineActivity::CInTimeOnlineActivity()
{
	m_nActivityID = -1;
	m_nMoneyReward = 0;
}
CInTimeOnlineActivity::~CInTimeOnlineActivity()
{
}
//³õÊ¼»¯
void CInTimeOnlineActivity::Init(int nActivityID, const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime,
								 const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward, const char * szTitle, const char * szContent)
{
	int nHour = 0;
	int nMinute = 0;sscanf(szTriggeringTime, "%2d:%2d", &nHour, &nMinute);

	CDayActivity::Init(nHour, nMinute, SetTime(szBeginTime), SetTime(szEndTime));
	m_nActivityID = nActivityID;
	//½±Àø
	m_nMoneyReward = nMoneyReward;
	if (!TransferItemList(szMaleReward, m_MaleRewardItemList))
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CInTimeOnlineActivity::Init. Transfer male item list error. ActivityID=%d, itemlist=%s", nActivityID, szMaleReward);
	}
	if (!TransferItemList(szFemaleReward, m_FemaleRewardItemList))
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CInTimeOnlineActivity::Init. Transfer female item list error. ActivityID=%d, itemlist=%s", nActivityID, szFemaleReward);
	}
	
	m_strTitle = szTitle;
	m_strContent = szContent;
}
void CInTimeOnlineActivity::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
	CDayActivity::OnUpdate(lTimeElapsed, tNow);

	if (!m_RoleList.empty())
	{
		_DealReward(lTimeElapsed);
	}
}
void CInTimeOnlineActivity::OnTime(const unsigned long &lTimeElapsed, time_t tNow)
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: CInTimeOnlineActivity::OnTime. ActivityID(%d)", m_nActivityID);
	m_RoleList.clear();
	CPlayerManager::Instance().GetRoleIDList(m_RoleList);
}
void CInTimeOnlineActivity::_DealReward(const unsigned long &lTimeElapsed)
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: CInTimeOnlineActivity::_DealReward. ActivityID(%d), RoleCount(%u)", m_nActivityID, (unsigned int)m_RoleList.size());
	int nCount = lTimeElapsed/10 + 1;

	for (std::list< unsigned int >::iterator itList = m_RoleList.begin(); itList != m_RoleList.end();)
	{
		unsigned int nRoleID = *itList;

		CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
		if (pRoleEntity != NULL && pRoleEntity->IsValid())
		{
			CEntityAttributeComponent * pAttrCom = pRoleEntity->GetComponent< CRoleAttribute >();
			if (pAttrCom)
			{
				pAttrCom->ChangeMoney(m_nMoneyReward, EChangeMoneyCause_Add_InTimeOnlineReward, 0);
			}
			if (pRoleEntity->GetSex() == ESexType_Male)
			{
				CItemProcess::AddOrMailItems(*pRoleEntity, m_MaleRewardItemList, EItemAction_Add_InTimeOnlineReward, m_nActivityID, true, EMailType_InTimeOnline_Reward, m_strTitle, m_strContent);
				GameMsg_S2C_InTimeOnlineReward inTimeOnlinRewardMsg(m_nHour, m_nMinute, m_MaleRewardItemList, m_nMoneyReward);
				pRoleEntity->SendPlayerMsg(&inTimeOnlinRewardMsg);
			}
			else
			{
				CItemProcess::AddOrMailItems(*pRoleEntity, m_FemaleRewardItemList, EItemAction_Add_InTimeOnlineReward, m_nActivityID, true, EMailType_InTimeOnline_Reward, m_strTitle, m_strContent);
				GameMsg_S2C_InTimeOnlineReward inTimeOnlinRewardMsg(m_nHour, m_nMinute, m_FemaleRewardItemList, m_nMoneyReward);
				pRoleEntity->SendPlayerMsg(&inTimeOnlinRewardMsg);
			}
		}

		m_RoleList.erase(itList++);

		nCount--;
		if (nCount <= 0)
		{
			break;
		}
	}
}

CShowInTimeOnlineActivityInfo::CShowInTimeOnlineActivityInfo()
{
	SetActivityType(EActivity_InTimeOnlineReward);
}

CShowInTimeOnlineActivityInfo::~CShowInTimeOnlineActivityInfo()
{

}

void CShowInTimeOnlineActivityInfo::Init( time_t tBeginTime, time_t tEndTime )
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CShowInTimeOnlineActivityInfo::OnActivityBegin()
{
	CInTimeOnlineInfoMgr::Instance().SetIsShowOpening(true);
}

void CShowInTimeOnlineActivityInfo::OnActivityEnd()
{
	CInTimeOnlineInfoMgr::Instance().SetIsShowOpening(false);
}


