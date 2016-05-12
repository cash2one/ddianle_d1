
#include "GameMsg_Activity.h"
#include "EntityActivityComponent.h"
#include "LongactingCumulativeRechareMgr.h"
#include "../../socket/GameMsg_Map.h"
#include "../item/ItemProcess.h"
#include "../mail/MailMgr.h"
#include "GameMsg_S2S_Activities.h"
#include "../attribute/EntityAttributeComponent.h"

void SendMsg2GroupServer(GameMsg_Base *pMsg);
extern CQueryManager* g_pQueryMgr;

LongactingCumulativeRechargeMgr::LongactingCumulativeRechargeMgr()
    : GameMsg_Processor(true, true)
{
}

LongactingCumulativeRechargeMgr::~LongactingCumulativeRechargeMgr()
{

}

LongactingCumulativeRechargeMgr & LongactingCumulativeRechargeMgr::Instance()
{
	static LongactingCumulativeRechargeMgr inst;
	return inst;
}

void LongactingCumulativeRechargeMgr::Init()
{
    RegisterMessage();
}

void LongactingCumulativeRechargeMgr::RegisterMessage()
{
    RegQueryProcessor(QUERY_LongactingCumulativeRechagre_UpdateIndex, &LongactingCumulativeRechargeMgr::OnGetRewardCallBack);
}

void LongactingCumulativeRechargeMgr::GetLongactingCumulativeRechargeConfig(std::list<LongactingCumulativeRechargeConfig> & listConfig)
{
    std::map<unsigned int,LongactingCumulativeRechargeConfig>::iterator it = m_mapLongactingRechargeConfig.begin();
    for (; it != m_mapLongactingRechargeConfig.end(); ++it)
    {
        listConfig.push_back(it->second);
    }
}

void LongactingCumulativeRechargeMgr::AddLongactingCumulativeRechargeConfig(const LongactingCumulativeRechargeConfig & config)
{
    m_mapLongactingRechargeConfig[config.m_nRewardIndex] = config;
}

void LongactingCumulativeRechargeMgr::AddLongactingCumulativeRechargeMail(const std::string & strMailTtle, const std::string & strMailContent)
{
    m_strMailTitle = strMailTtle;
    m_strMailContent = strMailContent;
}

const LongactingCumulativeRechargeConfig * LongactingCumulativeRechargeMgr::GetNextConfig(unsigned int nCurrentIndex)
{
    std::map<unsigned int,LongactingCumulativeRechargeConfig>::iterator it;
    if (nCurrentIndex == 0)
    {
        std::map<unsigned int,LongactingCumulativeRechargeConfig>::iterator it = m_mapLongactingRechargeConfig.begin();
        if (it != m_mapLongactingRechargeConfig.end())
        {
            return &(it->second);
        }
    }
    else
    {
        it = m_mapLongactingRechargeConfig.find(nCurrentIndex);
        if (it != m_mapLongactingRechargeConfig.end())
        {
            it++;

            if (it != m_mapLongactingRechargeConfig.end())
            {
                return &(it->second);
            }
        }
    }

    return NULL;
}

const LongactingCumulativeRechargeConfig * LongactingCumulativeRechargeMgr::GetCurrentConfig(unsigned int nCurrentIndex)
{
    std::map<unsigned int,LongactingCumulativeRechargeConfig>::iterator it = m_mapLongactingRechargeConfig.find(nCurrentIndex);
    if (it != m_mapLongactingRechargeConfig.end())
    {
        return &(it->second);
    }

    return NULL;
}

void LongactingCumulativeRechargeMgr::OnGetRewardCallBack(QueryBase & rQuery)
{
    unsigned int nRoleID = (unsigned int)rQuery.m_nAttachment;
    unsigned int nRewardIndex = (unsigned int)rQuery.m_nTag;
    LongactingRechargeRole * pRoleInfo = (LongactingRechargeRole*)rQuery.m_pAttachment;

    if (rQuery.m_nResult != Query_Res_OK)
    {
        // log
        UpdateRoleActionLog(nRoleID, RoleLongactingRechargeLog::ELongactingAction_Reward, nRewardIndex, false, false, 0);
        return;
    }

    const LongactingCumulativeRechargeConfig * pRechargeConfig = GetCurrentConfig(nRewardIndex);
    if (pRechargeConfig == NULL)
    {
        return;
    }

    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
    if (pRoleEntity != NULL)
    {
        if(pRoleInfo->m_nSex == ESexType_Male)
        {
            CItemProcess::AddItems( *pRoleEntity, pRechargeConfig->m_listMaleItemReward, EItemAction_Add_LongactingCumulativeRecharge, 0, true);
        }
        else
        {
            CItemProcess::AddItems( *pRoleEntity, pRechargeConfig->m_listFemaleItemReward, EItemAction_Add_LongactingCumulativeRecharge, 0, true);
        }
        pRoleEntity->GetRoleAttr()->ChangeMoney(pRechargeConfig->m_nMoney, EChangeMoneyCause_Add_LongactingRechargeReward, 0);
        pRoleEntity->GetRoleAttr()->ChangeBindBill(pRechargeConfig->m_nBindMCoin,EChangeBindBillCause_Add_LongactingRechargeReward);

        pRoleEntity->GetComponent<CRoleActivity>()->UpdateLongactingRechargeRewardIndex(nRewardIndex);

        GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess sucessmsg;
        pRoleEntity->SendPlayerMsg(&sucessmsg);
    }
    else
    {
        if (pRoleInfo->m_nSex == ESexType_Male)
        {
            CMailMgr::Instance().SendSystemMailToPlayer( nRoleID, EMailType_LongactingCumulativeRecharge, m_strMailTitle,
                m_strMailContent, pRechargeConfig->m_listMaleItemReward );
        }
        else
        {
            CMailMgr::Instance().SendSystemMailToPlayer( nRoleID, EMailType_LongactingCumulativeRecharge, m_strMailTitle,
                m_strMailContent, pRechargeConfig->m_listFemaleItemReward );
        }
    }

    GameMsg_S2G_AddLongactingRechargeBoardCast s2gmsg;
    s2gmsg.m_BoardCast.m_nRoleID = nRoleID;
    s2gmsg.m_BoardCast.m_strRoleName = pRoleInfo->m_strRoleName;
    s2gmsg.m_BoardCast.m_nRewardIndex = nRewardIndex;
    ::SendMsg2GroupServer(&s2gmsg);

    // log
    UpdateRoleActionLog(nRoleID, RoleLongactingRechargeLog::ELongactingAction_Reward, nRewardIndex, true, pRoleEntity == NULL ? false :true, pRoleInfo->m_nTotalMPoint);

    delete (LongactingRechargeRole*)rQuery.m_pAttachment;
    rQuery.m_pAttachment = NULL;
}

int LongactingCumulativeRechargeMgr::GetLongactingCumulativeRechargeReward(unsigned int nRoleID, char cSex, std::string & strRoleName, 
            unsigned int nTotalPoint, unsigned int nHaveRewardIndex, unsigned int nRewardIndex)
{
    if (nRewardIndex <= nHaveRewardIndex)
    {
        return ELCRE_HaveGetReward;
    }

    const LongactingCumulativeRechargeConfig * pRechargeConfig = GetNextConfig(nHaveRewardIndex);
    if (pRechargeConfig == NULL || pRechargeConfig->m_nRewardIndex !=  nRewardIndex)
    {   
        return ELCRE_RewardIndexError;
    }

    if (pRechargeConfig->m_nNeedMCoin <= 0) 
    {
        return ELCRE_ConfigError;
    }

    if (nTotalPoint < pRechargeConfig->m_nNeedMCoin)
    {
        return ELCRE_TotalPointNotEnough;
    }

    LongactingRechargeRole * pRoleInfo = new LongactingRechargeRole();
    if (pRoleInfo != NULL)
    {
        pRoleInfo->m_nSex = cSex;
        pRoleInfo->m_strRoleName = strRoleName;
        pRoleInfo->m_nTotalMPoint = nTotalPoint;
        g_pQueryMgr->AddQuery(QUERY_LongactingCumulativeRechagre_UpdateIndex, nRoleID, pRoleInfo, nRewardIndex);
    }

    return ELCRE_Scuess;
}

void LongactingCumulativeRechargeMgr::UpdateRoleActionLog(unsigned int nRoleID, RoleLongactingRechargeLog::ELongactingAction eAction,unsigned int nRewardIndex, bool bIsSucess, 
    bool bIsOnline, unsigned int nTotalMPoint)
{
    RoleLongactingRechargeLog * pLog = new RoleLongactingRechargeLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_nRewardIndex = nRewardIndex;
    pLog->m_bIsSucess = bIsSucess;
    pLog->m_bIsOnline = bIsOnline;
    pLog->m_nTotalMPoint = nTotalMPoint;
    g_pQueryMgr->AddQuery(QUERY_LogRoleLongacting, 0, pLog);
}

