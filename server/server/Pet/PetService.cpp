#include "PetService.h"
#include "PetComponent.h"
#include "Pet.h"

#include "PetConfig/PetConfigManager.h"
#include "PetConfig/PetQualityInfo.h"
#include "PetConfig/PetEvolutionInfo.h"
#include "PetConfig/PetBaseInfo.h"

#include "../mall/MallDefine.h"
#include "../util/NameDeny.h"

#include "../../socket/Log.h"
#include "../../socket/Formatter.h"
#include "../../datastructure/datainfor/ConfigManager.h"


PetService::PetService(void)
    : Service( "PetService" )
{
}

PetService::~PetService(void)
{
}

bool PetService::Init()
{
    if (!PetConfigManager::Instance().Initialize())
    {
        WriteLog(LOGLEVEL_ERROR, "PetConfigs.Initialize() return false.");

        return false;
    }

    RegQueryProcessor(QUERY_Pet_Create, &PetService::OnCreatePetCallback);

    return true;
}

void PetService::DoCreatePet(PetComponent *pPetComponent, unsigned short nPetTypeID,
    unsigned char nPotentialID, bool bAnnounce)
{
    if (pPetComponent == NULL)
        return;

    const PetBaseConfig *pPetBaseInfo = PetConfigManager::Instance().GetPetBaseConfig(nPetTypeID);
    const PetQualityInfo *pPetQualityInfo = PetConfigManager::Instance().GetPetQualityInfo(nPetTypeID);
    if (pPetBaseInfo == NULL)
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Can not find pet base info by pet type id(%u).RoleID(%u).", 
            nPetTypeID, pPetComponent->GetRoleID() );

        return;
    }
    else if (pPetQualityInfo == NULL)
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Can not find pet quality info by type id(%u).RoleID(%u).", 
            (unsigned int)ePetQualityType_White, pPetComponent->GetRoleID() );

        return;
    }

    Pet *pPet = Pet::Create();
    if (pPet == NULL)
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Can not create pet instance.RoleID(%u).", 
            pPetComponent->GetRoleID() );

        return;
    }

    pPet->SetTypeID(nPetTypeID);
    pPet->SetPotentialID(nPotentialID);
    pPet->SetAttributeType(pPetBaseInfo->GetAttributeType());
    pPet->SetQualityType(ePetQualityType_White);
    pPet->SetEvolutionTimes(0);
    pPet->SetName(pPetBaseInfo->GetDefaultName());
    pPet->SetEndurance(pPet->GetMaxEndurance());
    pPet->PrepareSkillIndex();

    pPetComponent->ResetSkill(pPet, std::set<SkillID>(), true); // 刚获得宠物时，做技能随机生成

    pPetComponent->AddQuery(QUERY_Pet_Create, pPetComponent->GetRoleID(), pPet, bAnnounce ? 1 : 0);
}

void PetService::DoUnlockGrid(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    unsigned short nUnlockCount = (unsigned short)((GameMsg_C2S_PetUnlockGrid &)rMsg).m_nCount;

    if (pPetComponent == NULL)
    {
        return;
    }
    else if (!pPetComponent->m_bEggLoadCompleted)
    {
        SendUnlockGridRes(pPetComponent, ePetUnlockGridRes_LoadingInfo);
        return;
    }

    unsigned short nMaxExtendGridCount = PetConfigManager::Instance().GetExtendGridCount();
    if (pPetComponent->m_nExtendPetGridCount >= nMaxExtendGridCount)
    {
        SendUnlockGridRes(pPetComponent, ePetUnlockGridRes_UnlockedAll);
        return;
    }

    if (nUnlockCount > PetConfigManager::Instance().GetMaxUnlockCountOnce())
    {
        SendUnlockGridRes(pPetComponent, ePetUnlockGridRes_TooManyCount);
        return;
    }

    if ((pPetComponent->m_nExtendPetGridCount + nUnlockCount) > nMaxExtendGridCount)
    {
        SendUnlockGridRes(pPetComponent, ePetUnlockGridRes_NotEnoughGrid);
        return;
    }

    CurrencyMap mapBills;
    PetConfigManager::Instance().GetExtendGridCost().AddIntoCurrencyMap(mapBills, true, (double)nUnlockCount);
    if (!pPetComponent->PayForUnlockGrid(mapBills) )
    {
        SendUnlockGridRes(pPetComponent, ePetUnlockGridRes_NotEnoughMoney);

        return;
    }

    pPetComponent->UnlockGrid(nUnlockCount);

    SendUnlockGridRes(pPetComponent, ePetUnlockGridRes_Success, pPetComponent->GetPetGridCount());
}

void PetService::DoChangeName(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_C2S_PetChangeName &rChangeNameMsg = (GameMsg_C2S_PetChangeName &)rMsg;
    unsigned int nPetID = rChangeNameMsg.m_nPetID;
    std::string &strNewName = rChangeNameMsg.m_strNewName;

    if (strNewName.empty())
    {
        SendChangeNameRes(pPetComponent, ePetChangeNameRes_EmptyName, nPetID);
        return;
    }

    NameDeny::Instance().FilterInvalidText(strNewName);

    const Pet* pPet = pPetComponent->GetPet(nPetID);
    if (pPet == NULL)
    {
        SendChangeNameRes(pPetComponent, ePetChangeNameRes_NotFoundPet, nPetID);
        return;
    }
    else if (pPet->GetName() == strNewName)
    {
        SendChangeNameRes(pPetComponent, ePetChangeNameRes_SameName, nPetID);
        return;
    }

    CurrencyMap mapBills;
    PetConfigManager::Instance().GetChangeNameCost().AddIntoCurrencyMap(mapBills, true);
    if (!pPetComponent->PayForChangeName(mapBills) )
    {
        SendChangeNameRes(pPetComponent, ePetChangeNameRes_NotEnoughMoney, nPetID);
        return;
    }

    pPetComponent->ChangePetName(pPet->GetID(), strNewName);

    SendChangeNameRes(pPetComponent, ePetChangeNameRes_Success, nPetID, strNewName);
}

void PetService::DoChangeCarried(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_C2S_PetChangeCarried &rChangeCarriedMsg = (GameMsg_C2S_PetChangeCarried &)rMsg;
    unsigned int nNewCarryingPetID = rChangeCarriedMsg.m_nPetID;
    unsigned char nSceneID = rChangeCarriedMsg.m_nSceneID;

    if (pPetComponent->m_nCarriedPetID == nNewCarryingPetID)
    {
        SendChangeCarriedRes(pPetComponent, ePetChangeCarriedRes_SamePet);
        return;
    }
    else if (nNewCarryingPetID == 0)
    {
        SendChangeCarriedRes(pPetComponent, ePetChangeCarriedRes_NotFoundPet);
        return;
    }

    Pet *pPet = pPetComponent->GetPet(nNewCarryingPetID);
    if (pPet == NULL)
    {
        SendChangeCarriedRes(pPetComponent, ePetChangeCarriedRes_NotFoundPet);
        return;
    }

    pPetComponent->ChangeCarried(nNewCarryingPetID, nSceneID);
    SendChangeCarriedRes(pPetComponent, ePetChangeCarriedRes_Success, pPet);
}

void PetService::DoUnloadCarried(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_C2S_PetUnloadCarried &rChangeCarriedMsg = (GameMsg_C2S_PetUnloadCarried &)rMsg;
    unsigned char nSceneID = rChangeCarriedMsg.m_nSceneID;

    GameMsg_S2C_PetUnloadCarriedRes unloadMsg;
    unloadMsg.m_nSceneID = nSceneID;

    if (pPetComponent->m_nCarriedPetID != 0)
    {
        pPetComponent->ChangeCarried(0, nSceneID);
        unloadMsg.m_nErrorCode = ePetUnloadCarriedRes_Success;
    }
    else
    {
        unloadMsg.m_nErrorCode = ePetUnloadCarriedRes_NoPetCarried;
    }

    pPetComponent->SendPlayerMsg(&unloadMsg);
}

void PetService::DoImprove(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    if (pPetComponent == NULL)
        return;

    unsigned int nPetID = ((GameMsg_C2S_PetImprove &)rMsg).m_nPetID;

    GameMsg_S2C_PetImproveRes resMsg;
    resMsg.m_nPetID = nPetID;

    Pet* pPet = pPetComponent->GetPet(nPetID);
    if (pPet == NULL)
    {
        resMsg.m_nErrorCode = ePetImproveRes_NotFoundPet;
        pPetComponent->SendPlayerMsg(&resMsg);
        return;
    }

    const PetQualityInfo *pQualityInfo = PetConfigManager::Instance().GetPetQualityInfo(pPet->GetTypeID());
    unsigned char nQualityType = pPet->GetQualityType();
    if (pQualityInfo == NULL)
    {
        resMsg.m_nErrorCode = ePetImproveRes_SystemError;
        pPetComponent->SendPlayerMsg(&resMsg);

        WriteLog(LOGLEVEL_ERROR, "Can not find pet quality info by pet type ID(%u).RoleID(%u).", 
            pPet->GetTypeID(), pPetComponent->GetRoleID());

        return;
    }
    else if (nQualityType >= pPet->GetMaxQuality())
    {
        resMsg.m_nErrorCode = ePetImproveRes_MaxQuality;
        pPetComponent->SendPlayerMsg(&resMsg);

        return;
    }
    else if (!pPet->CanImproveQuality())
    {
        resMsg.m_nErrorCode = ePetImproveRes_NotUpToImproveLevel;
        pPetComponent->SendPlayerMsg(&resMsg);

        return;
    }

    const PetImproveCondition *pCondition = pQualityInfo->GetImproveQualityCondition(nQualityType);
    if (pCondition == NULL)
    {
        resMsg.m_nErrorCode = ePetImproveRes_SystemError;
        pPetComponent->SendPlayerMsg(&resMsg);

        WriteLog(LOGLEVEL_ERROR, "Can not find pet quality condition info by pet type ID(%u) and quality type(%u).RoleID(%u).", 
            pPet->GetTypeID(), nQualityType, pPetComponent->GetRoleID());

        return;
    }

    CurrencyMap mapBills;
    pCondition->GetMoney().AddIntoCurrencyMap(mapBills, true);

    CItem itemCost;
    if (pCondition->GetItem().size() > 0)
        itemCost = *(pCondition->GetItem().begin());

    if (!pPetComponent->PayForImproveQuality(mapBills, itemCost))
    {
        resMsg.m_nErrorCode = ePetImproveRes_NotEnoughMoneyOrItems;
        pPetComponent->SendPlayerMsg(&resMsg);

        return;
    }

    pPetComponent->ImproveQuality(nPetID);

    resMsg.m_nErrorCode = ePetImproveRes_Success;
    GetImproveQualityCondition(pPet->GetTypeID(), nQualityType + 1,
        resMsg.m_nNextCostType, resMsg.m_nNextCostAmount, resMsg.m_nextItemImprove);
    EncodePetMsgInfo(*pPet, resMsg.m_petInfo);

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetService::DoEvolution(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_C2S_PetEvolution &rEvolutionMsg = (GameMsg_C2S_PetEvolution &)rMsg;
    GameMsg_S2C_PetEvolutionRes resMsg;
    resMsg.m_nPetID = rEvolutionMsg.m_nPetID;

    Pet *pPet = pPetComponent->GetPet(resMsg.m_nPetID);
    if (pPet == NULL)
    {
        resMsg.m_nErrorCode = ePetEvolutionRes_NotFoundPet;

        pPetComponent->SendPlayerMsg(&resMsg);

        return;
    }

    const PetEvolutionInfo *pEvolutionInfo = PetConfigManager::Instance().GetPetEvolutionInfo(pPet->GetTypeID());
    unsigned char nCurrentEvolutionTimes = pPet->GetEvolutionTimes();
    unsigned char nNextEvolutionTimes = nCurrentEvolutionTimes + 1;

    if (pEvolutionInfo == NULL || nCurrentEvolutionTimes >= pPet->GetMaxEvolutionTimes())
    {
        resMsg.m_nErrorCode = ePetEvolutionRes_MaxEvolutionTimes;
        pPetComponent->SendPlayerMsg(&resMsg);
        return;
    }
    else if (!pPet->CanEvolution())
    {
        resMsg.m_nErrorCode = ePetEvolutionRes_NotUpToEvolutionLevel;
        pPetComponent->SendPlayerMsg(&resMsg);
        return;
    }

    const PetEvolutionCondition *pCondition = pEvolutionInfo->GetEvolutionCondition(nNextEvolutionTimes);
    if (pCondition == NULL)
    {
        resMsg.m_nErrorCode = ePetEvolutionRes_SystemError;
        pPetComponent->SendPlayerMsg(&resMsg);
        WriteLog(LOGLEVEL_ERROR, "Can not find pet evolution condition info by pet type ID(%u) and evolution times(%u).RoleID(%u).",
            pPet->GetTypeID(), nNextEvolutionTimes, pPetComponent->GetRoleID());

        return;
    }

    int nRate = pCondition->GetRate();
    bool bHasExtraItem = pCondition->HasExtraItem();
    CItem itemExtra;

    if (bHasExtraItem)
    {
        itemExtra.m_nItemType = pCondition->GetExtraItem().m_nItemType;
        ItemConfig *pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(itemExtra.m_nItemType);

        if (pItemConfig == NULL || pItemConfig->m_listGene.empty())
        {
            resMsg.m_nErrorCode = ePetEvolutionRes_SystemError;
            pPetComponent->SendPlayerMsg(&resMsg);

            WriteLog(LOGLEVEL_ERROR, "Get extra item(%u) info error.RoleID(%u).", 
                itemExtra.m_nItemType, pPetComponent->GetRoleID());

            return;
        }

        GeneInfo *pGene = *(pItemConfig->m_listGene.begin());
        if (pGene != NULL)
        {
            itemExtra.m_nItemCount = (unsigned short)rEvolutionMsg.m_nExtraItemCount;
            nRate += pGene->m_nParam1 * rEvolutionMsg.m_nExtraItemCount;
        }
    }

    CurrencyMap mapBills;
    pCondition->GetMoney().AddIntoCurrencyMap(mapBills, true);

    CItem itemCost;
    if (pCondition->GetItem().size() > 0)
        itemCost = *(pCondition->GetItem().begin());

    if (!pPetComponent->PayForEvolution(mapBills, itemCost, itemExtra))
    {
        resMsg.m_nErrorCode = ePetEvolutionRes_NotEnoughMoneyOrItems;
        pPetComponent->SendPlayerMsg(&resMsg);
        return;
    }

    int random = Random(0, MAX_RATE);
    if (random <= nRate)
    {
        pPetComponent->Evolution(resMsg.m_nPetID);

        resMsg.m_nErrorCode = ePetEvolutionRes_Success;
        GetEvolutionCondition(pPet->GetTypeID(), nNextEvolutionTimes + 1, resMsg.m_nNextCostType, 
            resMsg.m_nNextCostAmount, resMsg.m_nextItemEvolution, resMsg.m_nextItemExtra, resMsg.m_nNextBaseRate);
        EncodePetMsgInfo(*pPet, resMsg.m_petInfos);
    }
    else
    {
        resMsg.m_nErrorCode = ePetEvolutionRes_Failed;
    }

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetService::DoFeeding(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_C2S_PetFeeding &rFeedingMsg = (GameMsg_C2S_PetFeeding &)rMsg;
    GameMsg_S2C_PetFeedingRes resMsg;
    resMsg.m_nPetID = rFeedingMsg.m_nPetID;

    Pet *pPet = pPetComponent->GetPet(resMsg.m_nPetID);
    if (pPet == NULL)
    {
        resMsg.m_nErrorCode = ePetFeedingRes_NotFoundPet;
    }
    else if (PetConfigManager::Instance().IsFeedExpItem(rFeedingMsg.m_nItemType))
    {
        FeedExp(pPetComponent, pPet, rFeedingMsg.m_nItemType, rFeedingMsg.m_nItemCount, resMsg);
    }
    else if (PetConfigManager::Instance().IsFeedEnduranceItem(rFeedingMsg.m_nItemType))
    {
        FeedEndurance(pPetComponent, pPet, rFeedingMsg.m_nItemType, rFeedingMsg.m_nItemCount, resMsg);
    }
    else
    {
        resMsg.m_nErrorCode = ePetFeedingRes_ItemValid;
    }

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetService::DoExchange(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_C2S_PetExchange &rExchangeMsg = (GameMsg_C2S_PetExchange &)rMsg;

    if (rExchangeMsg.m_listPetID.size() == 0)
    {
        SendExchangeRes(pPetComponent, ePetExchangeRes_NotSelectedPet);
        return;
    }

    std::list<unsigned int>::const_iterator listItr;
    for (listItr = rExchangeMsg.m_listPetID.begin(); listItr != rExchangeMsg.m_listPetID.end(); ++listItr)
    {
        unsigned int nPetID = *listItr;
        if (!pPetComponent->HasPet(nPetID))
        {
            WriteLog(LOGLEVEL_ERROR, "Can not find pet info by pet ID(%u) for exchange.RoleID(%u).", 
                nPetID, pPetComponent->GetRoleID() );

            SendExchangeRes(pPetComponent, ePetExchangeRes_NotFoundPet);
            return;
        }
        else if (pPetComponent->GetCarriedPetID() == nPetID)
        {
            SendExchangeRes(pPetComponent, ePetExchangeRes_CarriedPet);
            return;
        }
    }

    for (listItr = rExchangeMsg.m_listPetID.begin(); listItr != rExchangeMsg.m_listPetID.end(); ++listItr)
    {
        Pet *pPet = pPetComponent->GetPet(*listItr);
        if (pPet != NULL)
        {
            std::list<CItem> itemList;
            PetConfigManager::Instance().GetExchangeItem(pPet->GetAttributeType(), pPet->GetQualityType(), itemList);

            pPetComponent->DestroyPet(*listItr);
            pPetComponent->SendExchangeItems(itemList);
        }
    }

    SendExchangeRes(pPetComponent, ePetExchangeRes_Success);
}

void PetService::EncodePetMsgInfo(const Pet &rPet, PetMsgInfo &rInfo)
{
    const PetBaseConfig *pBaseInfo = PetConfigManager::Instance().GetPetBaseConfig(rPet.GetTypeID());
    if (pBaseInfo == NULL)
        return;

    int nResetCostRatio = PetConfigManager::Instance().GetResetSkillCost(rPet.GetResetSkillTimes());

    rInfo.m_nPetID = rPet.GetID();
    rInfo.m_nTypeID = rPet.GetTypeID();
    rInfo.m_strDefaultName = pBaseInfo->GetDefaultName();
    rInfo.m_strName = rPet.GetName();
    rInfo.m_nPotentialID = rPet.GetPotentialID();
    rInfo.m_nAttributeType = rPet.GetAttributeType();
    rInfo.m_nLevel = rPet.GetLevel();
    rInfo.m_nMaxLevel = rPet.GetTopLevel();
    rInfo.m_nExp = rPet.GetExp();
    rInfo.m_nMaxExp = rPet.GetMaxExpOfLevel();
    rInfo.m_nEndurance = rPet.GetEndurance();
    rInfo.m_nMaxEndurance = rPet.GetMaxEndurance();
    rInfo.m_fRecoveryTime = (float)GetPetRecoveryTime(rPet);
    rInfo.m_bCanImproveQuality = rPet.CanImproveQuality();
    rInfo.m_nQualityType = rPet.GetQualityType();
    rInfo.m_bCanEvolution = rPet.CanEvolution();
    rInfo.m_nEvolutionTimes = rPet.GetEvolutionTimes();
    rInfo.m_nResetSkillCostType = pBaseInfo->GetResetSkillCost().GetType();
    rInfo.m_nResetSkillCostAmount = pBaseInfo->GetResetSkillCost().GetAmount() * nResetCostRatio;
    rInfo.m_nDanceSkillCount = rPet.GetDanceSkillCount();
    rInfo.m_nFightSkillCount = rPet.GetFightSkillCount();
    EncodePetSkillList(rPet, rInfo.m_listDanceSkill, rInfo.m_listFightSkill);
}

void PetService::EncodePetBrief(const Pet &rPet, PlayerPet &briefPet)
{
    const PetBaseConfig *pBaseInfo = PetConfigManager::Instance().GetPetBaseConfig(rPet.GetTypeID());
    if (pBaseInfo == NULL)
        return;

    briefPet.m_nPetID = rPet.GetID();
    briefPet.m_strName = rPet.GetName();
    briefPet.m_strDefaultName = pBaseInfo->GetDefaultName();
    briefPet.m_nPotentialID = rPet.GetPotentialID();
    briefPet.m_nPetAttributeType = rPet.GetAttributeType();
    briefPet.m_nPetType = rPet.GetTypeID();
    briefPet.m_nLevel = rPet.GetLevel();
    briefPet.m_nMaxLevel = rPet.GetTopLevel();
    briefPet.m_nExp = rPet.GetExp();
    briefPet.m_nMaxExpOfLevel = rPet.GetMaxExpOfLevel();
    briefPet.m_nQualityType = rPet.GetQualityType();
    briefPet.m_nEvolutionTimes = rPet.GetEvolutionTimes();
    briefPet.m_nEndurance = rPet.GetEndurance();
    briefPet.m_nMaxEndurance = rPet.GetMaxEndurance();
    briefPet.m_nEnduranceRecoveryTime = (float)GetPetRecoveryTime(rPet);
}

int PetService::GetPetRecoveryTime(const Pet &rPet) const
{
    unsigned int nNow = (unsigned int)time(NULL);
    if (rPet.GetMaxEndurance() > rPet.GetEndurance() && rPet.GetEnduranceRecoveryTime() < nNow)
    {
        int nLostEndurance = rPet.GetMaxEndurance() - rPet.GetEndurance();
        int nUnitTime = PetConfigManager::Instance().GetEnduranceRecoveryTime();
        int nNeedTime = nUnitTime * nLostEndurance;
        int nElapsedTime = nNow - rPet.GetEnduranceRecoveryTime();
        return nNeedTime - nElapsedTime;
    }

    return 0;
}

void PetService::EncodePetSkill(const Pet &rPet, PlayerPetSkill &playerSkill)
{
    playerSkill.m_nPetID = rPet.GetID();
    playerSkill.m_nDanceSkillCount = rPet.GetDanceSkillCount();
    playerSkill.m_nFightSkillCount = rPet.GetFightSkillCount();
    EncodePetSkillList(rPet, playerSkill.m_listDanceSkill, playerSkill.m_listFightSkill);
}

void PetService::EncodePetSkillList(const Pet &rPet, 
    std::list<PetMsgSkill> &listDanceSkill, std::list<PetMsgSkill> &listFightSkill)
{
    std::list<PetSkill *> skillList;
    rPet.GetOrderedSkillList(skillList);

    for (std::list<PetSkill *>::iterator it = skillList.begin(); it != skillList.end(); ++it)
    {
        PetSkill *skill = *it;
        PetMsgSkill skillMsg;
        if (skill != NULL)
        {
            SkillType nType = EncodePetSkill(rPet, *skill, skillMsg);

            if (PetSkillConfig::IsDanceSkill(nType))
                listDanceSkill.push_back(skillMsg);
            else if (PetSkillConfig::IsFightSkill(nType))
                listFightSkill.push_back(skillMsg);
        }
    }
}

SkillType PetService::EncodePetSkill(const Pet &rPet, const PetSkill &skill, PetMsgSkill &skillMsg)
{
    PetSkillConfig skillConfig;
    const PetSkillUnlockConfig *pUnlockConfig = PetConfigManager::Instance().GetPetSkillUnlockConfig(skill.GetSkillIndex());

    if (PetConfigManager::Instance().GetPetSkill(skill.GetSkillID(), skillConfig))
    {
        skillMsg.m_nSkillID = skillConfig.GetSkillID();
        skillMsg.m_strName  = skillConfig.GetName();
        skillMsg.m_strAtlasName  = skillConfig.GetAtlasName();
        skillMsg.m_strSpriteName  = skillConfig.GetSpriteName();
        skillMsg.m_bActivated = rPet.IsActivatedSkill(skill);
        skillMsg.m_nLevel = (unsigned short)skill.GetLevel();
        skillMsg.m_nMaxLevel = skillConfig.GetMaxLevel();
        skillMsg.m_nLevelUpCostType = skillConfig.GetLevelUpCost().GetType();
        skillMsg.m_nLevelUpCostAmount = skillConfig.GetLevelUpCost().GetAmount();
        SafeFormat(skillMsg.m_strTip, skillConfig.GetTip(), skill.GetSkillValue());
        skillMsg.m_strNextTip = skillConfig.GetLevelUpTip();
        if (pUnlockConfig != NULL)
        {
            skillMsg.m_strActivationTip = pUnlockConfig->GetUnlockTip();
        }

        return skillConfig.GetType();
    }

    return 0;
}

void PetService::EncodePetExchangeMsgInfo(const Pet &rPet, PetExchangeMsgInfo &rInfo)
{
    rInfo.m_nPetID = rPet.GetID();
    rInfo.m_nTypeID = rPet.GetTypeID();
    rInfo.m_strName = rPet.GetName();
    rInfo.m_nAttributeType = rPet.GetAttributeType();
    rInfo.m_nLevel = rPet.GetLevel();
    rInfo.m_nQualityType = rPet.GetQualityType();
    rInfo.m_nEvolutionTimes = rPet.GetEvolutionTimes();
    rInfo.m_nPotentialID = rPet.GetPotentialID();
    EncodePetSkillList(rPet, rInfo.m_listDanceSkill, rInfo.m_listFightSkill);
    PetConfigManager::Instance().GetExchangeItem(rInfo.m_nAttributeType, rInfo.m_nQualityType, rInfo.m_listExchangeItems);
}

void PetService::GetImproveQualityCondition(unsigned short nPetTypeID, unsigned char nQualityType, 
    unsigned char &rCostType, unsigned int &rCostAmount, CItem &rCostItem)
{
    const PetQualityInfo *pQualityInfo = PetConfigManager::Instance().GetPetQualityInfo(nPetTypeID);
    if (pQualityInfo == NULL)
        return;

    const PetImproveCondition *pCondition = pQualityInfo->GetImproveQualityCondition(nQualityType);
    if (pCondition == NULL)
        return;

    rCostType = pCondition->GetMoney().GetType();
    rCostAmount = pCondition->GetMoney().GetAmount();

    if (pCondition->GetItem().size() > 0)
        rCostItem = *(pCondition->GetItem().begin());
}

void PetService::GetEvolutionCondition(unsigned short nPetTypeID, unsigned char nNextEvolutionTimes, 
    unsigned char &rCostType, unsigned int &rCostAmount, CItem &rCostItem, CItem &rExtraItem, int &rBaseRate)
{
    const PetEvolutionInfo *pEvolutionInfo = PetConfigManager::Instance().GetPetEvolutionInfo(nPetTypeID);
    if (pEvolutionInfo == NULL)
        return;

    const PetEvolutionCondition *pCondition = pEvolutionInfo->GetEvolutionCondition(nNextEvolutionTimes);
    if (pCondition == NULL)
        return;

    rCostType = pCondition->GetMoney().GetType();
    rCostAmount = pCondition->GetMoney().GetAmount();
    rBaseRate = pCondition->GetRate();

    if (pCondition->GetItem().size() > 0)
        rCostItem = *(pCondition->GetItem().begin() );

    if (pCondition->HasExtraItem() )
        rExtraItem = pCondition->GetExtraItem();
}

void PetService::SendUnlockGridRes(PetComponent *pPetComponent, ePetUnlockGridRes eErrorCode, unsigned short nGridCount /* = 0 */)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_S2C_PetUnlockGridRes resMsg;
    resMsg.m_nErrorCode = (unsigned char)eErrorCode;
    resMsg.m_nGridCount = (int)nGridCount;

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetService::SendChangeNameRes(PetComponent *pPetComponent, ePetChangeNameRes eErrorCode, unsigned int nPetID, const std::string &rNewName /* = "" */)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_S2C_PetChangeNameRes resMsg;
    resMsg.m_nErrorCode = (unsigned char)eErrorCode;
    resMsg.m_nPetID = nPetID;
    resMsg.m_strNewName = rNewName;

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetService::SendChangeCarriedRes(PetComponent *pPetComponent, ePetChangeCarriedRes eErrorCode, Pet *pPet /* = NULL */)
{
    if (pPetComponent == NULL)
        return;
    else if (eErrorCode == ePetChangeCarriedRes_Success && pPet == NULL)
        return;

    GameMsg_S2C_PetChangeCarriedRes resMsg;
    resMsg.m_nErrorCode = (unsigned char)eErrorCode;
    resMsg.m_nSceneID = pPetComponent->GetSceneID();
    if (pPet != NULL)
    {
        EncodePetMsgInfo(*pPet, resMsg.m_petInfos);
    }

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetService::SendExchangeRes(PetComponent *pPetComponent, ePetExchangeRes eErrorCode)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_S2C_PetExchangeRes resMsg;
    resMsg.m_nErrorCode = (unsigned char)eErrorCode;

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetService::FeedExp(PetComponent *pPetComponent, Pet *pPet, unsigned int nItemType, int nItemCount, GameMsg_S2C_PetFeedingRes &rResMsg)
{
    if (pPet == NULL)
        return;

    if (!pPet->CanLevelUp())
    {
        rResMsg.m_nErrorCode = ePetFeedingRes_MaxLevel;
        return;
    }

    ItemConfig *pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pItemConfig == NULL || pItemConfig->m_listGene.empty())
    {
        rResMsg.m_nErrorCode = ePetFeedingRes_SystemError;
        WriteLog(LOGLEVEL_ERROR, "Get feeding item(%u) info error.RoleID(%u).", 
            nItemType, pPetComponent->GetRoleID() );
        return;
    }

    unsigned int nAddExp = 0;
    GeneInfo *pGene = *(pItemConfig->m_listGene.begin());
    if (pGene != NULL)
    {
        unsigned char nItemAttributeType = (unsigned char)atoi(pGene->m_strParam.c_str());
        if (pPet->GetAttributeType() == nItemAttributeType)
        {
            nAddExp = (unsigned int)(pGene->m_nParam2 * nItemCount);
        }
        else
        {
            nAddExp = (unsigned int)(pGene->m_nParam1 * nItemCount);
        }
    }

    CItem itemCost;
    itemCost.m_nItemType = nItemType;
    itemCost.m_nItemCount = (itemcount_t)nItemCount;
    if (!pPetComponent->PayForFeeding(itemCost))
    {
        rResMsg.m_nErrorCode = ePetFeedingRes_NotEnoughItems;
        return;
    }

    pPetComponent->AddExp(pPet->GetID(), nAddExp);

    rResMsg.m_nErrorCode = ePetFeedingRes_Success;
    EncodePetMsgInfo(*pPet, rResMsg.m_petInfo);
}

void PetService::FeedEndurance(PetComponent *pPetComponent, Pet *pPet, unsigned int nItemType, int nItemCount, GameMsg_S2C_PetFeedingRes &rResMsg)
{
    if (pPet == NULL)
        return;

    unsigned short nCurrentEndurance = pPet->GetEndurance();
    unsigned short nMaxEndurance = pPet->GetMaxEndurance();
    if (nCurrentEndurance >= nMaxEndurance)
    {
        rResMsg.m_nErrorCode = ePetFeedingRes_MaxEndurance;
        return;
    }

    ItemConfig *pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pItemConfig == NULL || pItemConfig->m_listGene.empty())
    {
        rResMsg.m_nErrorCode = ePetFeedingRes_SystemError;
        WriteLog(LOGLEVEL_ERROR, "Get feeding item(%u) info error.RoleID(%u).", 
            nItemType, pPetComponent->GetRoleID() );
        return;
    }

    int nRecoveredEndurance = 0;
    GeneInfo *pGene = *(pItemConfig->m_listGene.begin());
    if (pGene != NULL)
        nRecoveredEndurance = pGene->m_nParam1 * nItemCount;

    CItem itemCost;
    itemCost.m_nItemType = nItemType;
    itemCost.m_nItemCount = (itemcount_t)nItemCount;
    if (!pPetComponent->PayForFeeding(itemCost))
    {
        rResMsg.m_nErrorCode = ePetFeedingRes_NotEnoughItems;
        return;
    }

    pPetComponent->RecoveryEndurance(pPet, nRecoveredEndurance);

    rResMsg.m_nErrorCode = ePetFeedingRes_Success;
    EncodePetMsgInfo(*pPet, rResMsg.m_petInfo);
}

void PetService::OnCreatePetCallback(QueryBase &rQuery)
{
    Pet *pPet = (Pet *)rQuery.m_pAttachment;
    bool bAnnounce = (rQuery.m_nTag == 0 ? false : true);
    CRoleEntity *pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(rQuery.m_nRoleID);

    if (pRoleEntity == NULL)
    {
        SAFE_DELETE(pPet);
        return;
    }

    PetComponent *pPetComponent = pRoleEntity->GetComponent<CRolePet>();
    if (pPetComponent == NULL)
    {
        SAFE_DELETE(pPet);
        return;
    }

    if (rQuery.m_nResult != Query_Res_OK)
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Database create pet fail.RoleID(%u).", rQuery.m_nRoleID);
        SAFE_DELETE(pPet);
        return;
    }

    pPetComponent->OpenedPetEgg();
    pPetComponent->AddPet(pPet);

    if (bAnnounce)
        pPetComponent->SendIncubationGetPetAnnounce(pPet->GetName());

    pPetComponent->SendOpenEggRes(ePetOpenEggRes_Success, pPet->GetTypeID(), pPet->GetName());
}

