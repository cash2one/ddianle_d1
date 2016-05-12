#include "PetEggService.h"
#include "PetComponent.h"
#include "PetEgg.h"

#include "PetEggConfig/PetEggConfigManager.h"
#include "PetEggConfig/PetIncubatorInfo.h"
#include "PetEggConfig/PetEggBaseInfo.h"
#include "PetConfig/PetConfigManager.h"

#include "../mall/MallDefine.h"

#include "../../socket/Log.h"

#include "../../datastructure/ItemBase.h"


PetEggService::PetEggService(void)
    : Service( "PetEggService" )
{
}

PetEggService::~PetEggService(void)
{
}

bool PetEggService::Init()
{
    if (!PetEggConfigManager::Instance().Initialize() )
    {
        WriteLog(LOGLEVEL_ERROR, "PetEggConfigs.Initialize() return false.");

        return false;
    }

    Start();

    return true;
}

void PetEggService::Start()
{
    RegQueryProcessor(QUERY_PetEgg_Create, &PetEggService::OnCreatePetEggCallback);
}

void PetEggService::DoIncubatorLevelUp(PetComponent *pPetComponent)
{
    if (pPetComponent == NULL)
    {
        return;
    }
    else if (!pPetComponent->m_bEggLoadCompleted)
    {
        SendIncubatorLevelUpRes(pPetComponent, ePetIncubatorLevelUpRes_LoadingInfos);
        return;
    }

    const PetIncubatorInfo *pIncubatorInfo = PetEggConfigManager::Instance().GetPetIncubatorInfo(pPetComponent->m_nIncubatorLevel);
    if (pIncubatorInfo == NULL)
    {
        SendIncubatorLevelUpRes(pPetComponent, ePetIncubatorLevelUpRes_MaxLevel);

        return;
    }

    CurrencyMap mapBills;
    pIncubatorInfo->GetLevelUpCost().AddIntoCurrencyMap(mapBills, true);

    if (!pPetComponent->PayForIncubatorLevelUp(mapBills) )
    {
        SendIncubatorLevelUpRes(pPetComponent, ePetIncubatorLevelUpRes_NotEnoughMoney);
        return;
    }

    pPetComponent->IncubatorLevelUp();

    SendIncubatorLevelUpRes(pPetComponent, ePetIncubatorLevelUpRes_Success);
}

void PetEggService::DoStartIncubation(PetComponent *pPetComponent, GameMsg_Base &rMsg)
{
    if (pPetComponent == NULL)
    {
        return;
    }
    else if (!pPetComponent->m_bEggLoadCompleted)
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_LoadingInfos);

        return;
    }
    else if (pPetComponent->m_pEgg != NULL)
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_Incubating);

        return;
    }

    GameMsg_C2S_PetStartIncubation &rStartMsg = (GameMsg_C2S_PetStartIncubation &)rMsg;
    const ItemBase *pItemBase = pPetComponent->GetPetEggItem(rStartMsg.m_nIndex);

    if (pItemBase == NULL)
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_NotFoundItem);

        return;
    }
    else if (!pItemBase->IsPetEgg() )
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_IsNotEgg);

        return;
    }

    itemtype_t nEggTypeID = pItemBase->GetType();
    const PetEggBaseInfo *pEggBaseInfo = PetEggConfigManager::Instance().GetEggInfo(nEggTypeID);

    if (pEggBaseInfo == NULL)
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Can not find egg base info by egg type id(%u).RoleID(%u).", 
            (unsigned int)nEggTypeID, pPetComponent->GetRoleID() );

        return;
    }
    else if (!PetEggConfigManager::Instance().FindPetList(nEggTypeID) )
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Can not find random pet info by egg type id(%u).RoleID(%u).", 
            (unsigned int)nEggTypeID, pPetComponent->GetRoleID() );

        return;
    }
    else if (pEggBaseInfo->GetMinIncubatorLevel() > pPetComponent->m_nIncubatorLevel )
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_IncubatorLevelTooLow);

        return;
    }

    PetEgg *pEgg = PetEgg::Create();

    if (pEgg == NULL)
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Can not create pet egg instance.RoleID(%u).", 
            pPetComponent->GetRoleID() );

        return;
    }

    // 在新手引导特定步骤，第一次使用孵化功能时，可以免费, 其它情况都要扣除相应消耗
    int nFreeStep = PetConfigManager::Instance().GetFreeIncubationGuideStep();
    if (nFreeStep == 0 || pPetComponent->GetGuideStep() != nFreeStep || pPetComponent->HasPet())
    {
        CurrencyMap mapBills;
        pEggBaseInfo->GetIncubateCost().AddIntoCurrencyMap(mapBills, true);

        if (!pPetComponent->PayForStartIncubation(mapBills) )
        {
            SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_NotEnoughMoney);

            SAFE_DELETE(pEgg);

            return;
        }
    }

    unsigned int nFinishedTime = (unsigned int)time(NULL) + pEggBaseInfo->GetIncubationTime();
    pEgg->SetFinishedTime(nFinishedTime);
    pEgg->SetTypeID(nEggTypeID);

    pPetComponent->UseOnePetEggItem(rStartMsg.m_nIndex);
    pPetComponent->AddQuery(QUERY_PetEgg_Create, pPetComponent->GetRoleID(), pEgg);
}

void PetEggService::DoIncubationSpeedUp(PetComponent *pPetComponent)
{
    unsigned int nNow = (unsigned int)time(NULL);

    if (pPetComponent == NULL)
    {
        return;
    }
    else if (!pPetComponent->m_bEggLoadCompleted)
    {
        SendIncubationSpeedUpRes(pPetComponent, ePetIncubationSpeedUpRes_LoadingInfos);

        return;
    }

    const PetEgg* pPetEgg = pPetComponent->GetPetEgg();

    if (pPetEgg == NULL)
    {
        SendIncubationSpeedUpRes(pPetComponent, ePetIncubationSpeedUpRes_NoEgg);

        return;
    }
    else if (pPetEgg->IsFinished() || pPetEgg->GetFinishedTime() <= nNow)
    {
        SendIncubationSpeedUpRes(pPetComponent, ePetIncubationSpeedUpRes_Finished);

        return;
    }

    const PetEggBaseInfo *pEggBaseInfo = PetEggConfigManager::Instance().GetEggInfo(pPetEgg->GetTypeID() );

    if (pEggBaseInfo == NULL)
    {
        SendIncubationSpeedUpRes(pPetComponent, ePetIncubationSpeedUpRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Can not find pet egg base info by egg type id(%u).RoleID(%u).", 
            (unsigned int)pPetEgg->GetTypeID(), pPetComponent->GetRoleID() );

        return;
    }

    // 在新手引导特定步骤，并且第一次使用加速时，可以免费进行加速, 其它情况都要扣除相应消耗
    int nFreeStep = PetConfigManager::Instance().GetFreeAccelerateGuideStep();
    if (nFreeStep == 0 || pPetComponent->GetGuideStep() != nFreeStep || pPetComponent->HasPet())
    {
        unsigned int lastSecond = pPetEgg->GetFinishedTime() - nNow;

        double nRatio = (lastSecond + MINUTE_PRE_SECONDS - 1) / MINUTE_PRE_SECONDS;

        CurrencyMap mapBills;
        pEggBaseInfo->GetSpeedUpPreMinuteCost().AddIntoCurrencyMap(mapBills, true, nRatio);
        if (!pPetComponent->PayForIncubationSpeedUp(mapBills))
        {
            SendIncubationSpeedUpRes(pPetComponent, ePetIncubationSpeedUpRes_NotEnoughMoney);

            return;
        }
    }

    pPetComponent->IncubationSpeedUp();

    SendIncubationSpeedUpRes(pPetComponent, ePetIncubationSpeedUpRes_Success);
}

const RandomPetInfo* PetEggService::DoOpenEgg(PetComponent *pPetComponent)
{
    if (pPetComponent == NULL)
    {
        return NULL;
    }
    else if (!pPetComponent->m_bEggLoadCompleted)
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_LoadingInfos);

        return NULL;
    }
    else if (pPetComponent->m_pEgg == NULL)
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_NoEgg);

        return NULL;
    }
    else if (!pPetComponent->m_pEgg->IsFinished() )
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_NotFinished);

        return NULL;
    }
    else if (pPetComponent->IsPetGridFull() )
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_PetGridFull);

        return NULL;
    }

    unsigned int nEggTypeID = pPetComponent->m_pEgg->GetTypeID();
    const RandomPetInfo *pPetInfo = PetEggConfigManager::Instance().RandOnePet(nEggTypeID);

    if (pPetInfo == NULL)
    {
        pPetComponent->SendOpenEggRes(ePetOpenEggRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Can not find random pet info by egg type id(%u).RoleID(%u).", 
            (unsigned int)nEggTypeID, pPetComponent->GetRoleID() );
    }

    return pPetInfo;
}

void PetEggService::SendIncubatorLevelUpRes(PetComponent *pPetComponent, ePetIncubatorLevelUpRes eErrorCode)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_S2C_PetIncubatorLevelUpRes resMsg;
    resMsg.m_nErrorCode = (unsigned char)eErrorCode;

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetEggService::SendStartIncubationRes(PetComponent *pPetComponent, ePetStartIncubationRes eErrorCode)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_S2C_PetStartIncubationRes resMsg;
    resMsg.m_nErrorCode = (unsigned char)eErrorCode;

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetEggService::SendIncubationSpeedUpRes(PetComponent *pPetComponent, ePetIncubationSpeedUpRes eErrorCode)
{
    if (pPetComponent == NULL)
        return;

    GameMsg_S2C_PetIncubationSpeedUpRes resMsg;
    resMsg.m_nErrorCode = (unsigned char)eErrorCode;

    pPetComponent->SendPlayerMsg(&resMsg);
}

void PetEggService::OnCreatePetEggCallback(QueryBase &rQuery)
{
    PetEgg *pEgg = (PetEgg *)rQuery.m_pAttachment;
    CRoleEntity *pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(rQuery.m_nRoleID);

    if (pRoleEntity == NULL)
    {
        SAFE_DELETE(pEgg);

        return;
    }

    PetComponent *pPetComponent = pRoleEntity->GetComponent<CRolePet>();

    if (pPetComponent == NULL)
    {
        SAFE_DELETE(pEgg);

        return;
    }

    if (rQuery.m_nResult != Query_Res_OK)
    {
        SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_SystemError);
        WriteLog(LOGLEVEL_ERROR, "Database create pet egg fail.RoleID(%u).", rQuery.m_nRoleID);

        SAFE_DELETE(pEgg);

        return;
    }

    pPetComponent->SetPetEgg(pEgg);

    SendStartIncubationRes(pPetComponent, ePetStartIncubationRes_Success);
}

