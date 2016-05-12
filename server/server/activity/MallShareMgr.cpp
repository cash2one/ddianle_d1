#include "MallShareMgr.h"
#include "SocialShareActivity.h"
#include "SystemActivityManager.h"
#include "GameMsg_Activity.h"
#include "EntityActivityComponent.h"
#include "../../datastructure/datainfor/ItemConfigManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/ItemProcess.h"
#include "../mall/Mall.h"

CMallSocialShareMgr::CMallSocialShareMgr()
{
	m_bIsOpeningShow = false;
}

CMallSocialShareMgr::~CMallSocialShareMgr()
{

}

CMallSocialShareMgr& CMallSocialShareMgr::Instance()
{
	static CMallSocialShareMgr inst;
	return inst;
}

void CMallSocialShareMgr::Init()
{
	CSocialShareActivityShow* pActivityShow = new CSocialShareActivityShow(EActivity_MallSocialShare);
	pActivityShow->Init(m_data.m_strBeginTimeShow.c_str(), m_data.m_strEndTimeShow.c_str());
	m_beginTime.AssignFormat(m_data.m_strBeginTime, " |");
	m_endTime.AssignFormat(m_data.m_strEndTime, " |");
	m_bIsOpeningShow = pActivityShow->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivityShow);
    /*
	CSocialShareActivity* pActivity = new CSocialShareActivity(EActivity_MallSocialShare);
	pActivity->Init(m_data.m_strBeginTime.c_str(), m_data.m_strEndTime.c_str());
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);*/
}

void CMallSocialShareMgr::OnHandleMsg( CRoleEntity* pRole, CEntityActivityComponent* pActivityComponent, GameMsg_C2S_ActivitySocialShare& msg )
{
	CEntityNetComponent* pNet = pActivityComponent->GetComponent<CRoleNet>();
	if ( pNet && IsActivityOpenNow() && !pActivityComponent->IsSharedToSocial((EUISocialShare)msg.m_nUI) )
	{
		CMallSocialShareData* pData = (CMallSocialShareData*)msg.m_pData;

		std::list<CItem> listItems;
		std::string strItemNames;
		for (std::list<itemtype_t>::iterator it = pData->m_listTryCloth.begin(); it != pData->m_listTryCloth.end(); ++it)
		{
			ItemConfig* pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(*it);
			MallGood * pMallGood = CMall::Instance().FindGoods(*it);
			if ( m_data.m_nRewardItemTime > 0 && pItemInfo != NULL && pMallGood != NULL && (!pMallGood->m_nIsPrecious) && 
				m_data.m_setClothPos.find(pItemInfo->m_anType[2]) != m_data.m_setClothPos.end() )
			{
                if (ESexType_No != pItemInfo->m_SexNeed && pRole->GetSex() != pItemInfo->m_SexNeed)
                {
                    return;
                }

				strItemNames += pItemInfo->m_strName;
				strItemNames += ", ";

				CItem item(pItemInfo->m_nType, 1, m_data.m_nRewardItemTime*24*3600, false);
				listItems.push_back(item);
			}
		}
		if ( strItemNames.size() > 2 )
		{
			strItemNames = strItemNames.substr(0, strItemNames.size()-2);
		}

		pActivityComponent->SetSharedToSocial((EUISocialShare)msg.m_nUI, true);

		if ( listItems.size() > 0 )
		{
			CItemProcess::AddOrMailItems(*pRole, listItems, EItemAction_Add_MallSocialShare, 
				EActivity_MallSocialShare, true, EMailType_MallSocialShare, m_data.m_strMailTitle, m_data.m_strMailContent);

			//只有奖励了服饰，才会返回消息
			GameMsg_S2C_ActivitySocialShare retMsg;
			retMsg.m_nUI = msg.m_nUI;
			//SafeFormat(retMsg.m_strTip, m_data.m_strRewardDesc, strItemNames.c_str());
			pNet->SendPlayerMsg(&retMsg);
		}
	}
}

bool CMallSocialShareMgr::IsActivityOpenInTime(time_t nTime)
{
	return CCrontab::IsBetween(m_beginTime, m_endTime, nTime, false, true);
}

bool CMallSocialShareMgr::IsActivityOpenNow()
{
	return CCrontab::IsBetween(m_beginTime, m_endTime, time(NULL), false, false);
}

