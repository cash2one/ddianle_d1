#include "HandbookGlobalMgr.h"
#include "../libServerFrame/QueryManager.h"
#include "GameMsg_Handbook.h"
#include "../../socket/GameMsg_Map.h"
#include "../group/GroupSystemSetting.h"

extern CQueryManager    *g_pQueryMgr;

#define HANDBOOK_UPDATE_INTERVAL 1800000 //ms
#define MAX_STATE_COUNT 99999999

CHandbookGlobalMgr::CHandbookGlobalMgr()
    :GameMsg_Processor(true,true)
    ,m_nInterval(0)
{
     Initialize();
}

CHandbookGlobalMgr::~CHandbookGlobalMgr()
{

}

void CHandbookGlobalMgr::Initialize()
{
    m_bIsOpening = CGroupSystemSetting::Instance().IsFunctionOpen( EFunctionSwitch_Handbook );

    if(!m_bIsOpening)
    {
        return;
    }

    RegComponentNetMsgMap();
    g_pQueryMgr->AddQuery(QUERY_Handnbook_LoadGolbalCostumeData, 0 , &m_MapCostume);
    g_pQueryMgr->AddQuery(QUERY_Handnbook_LoadGolbalEffectData, 0 , &m_MapEffect);
}

void CHandbookGlobalMgr::RetMsgGolbalCostumeData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    int UnitCount = 0;

    GameMsg_G2S_GolbalCostumeData RetMsg;
    for (std::map<int, CHandbookUnit>::iterator it = m_MapCostume.begin();
        it != m_MapCostume.end(); ++it)
    {
        GolbalPerInfo perinfo;
        perinfo.m_nId = it->second.GetID();
        perinfo.m_nPraisedCount = it->second.GetPraisedCount();
        perinfo.m_nEnvyCount = it->second.GetEnvyCount();
        perinfo.m_nCollectCount = it->second.GetCollectCount();

        RetMsg.m_listPerInfo.push_back(perinfo);

        UnitCount++;
        if (UnitCount > MAX_UNIT_SIZE)
        {
            //达到单条消息最大 单元数
            pRoleEntity->SendPlayerMsg(&RetMsg);
            RetMsg.m_listPerInfo.clear();
            UnitCount = 0;
        }
    }

    RetMsg.m_isEnd = true;
    pRoleEntity->SendPlayerMsg(&RetMsg);
}

void CHandbookGlobalMgr::RetMsgGolbalEffectData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    int UnitCount = 0;

    GameMsg_G2S_GolbalEffectData RetMsg;
    for (std::map<int, CHandbookUnit>::iterator it = m_MapEffect.begin();
        it != m_MapEffect.end(); ++it)
    {
        GolbalPerInfo perinfo;
        perinfo.m_nId = it->second.GetID();
        perinfo.m_nPraisedCount = it->second.GetPraisedCount();
        perinfo.m_nEnvyCount = it->second.GetEnvyCount();
        perinfo.m_nCollectCount = it->second.GetCollectCount();

        RetMsg.m_listPerInfo.push_back(perinfo);

        UnitCount++;
        if (UnitCount > MAX_UNIT_SIZE)
        {
            //达到单条消息最大 单元数
            pRoleEntity->SendPlayerMsg(&RetMsg);
            RetMsg.m_listPerInfo.clear();
            UnitCount = 0;
        }
    }

    RetMsg.m_isEnd = true;
    pRoleEntity->SendPlayerMsg(&RetMsg);
}

void CHandbookGlobalMgr::ChangeGolbalData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_S2G_ChangeGolbalData& resultMsg = (GameMsg_S2G_ChangeGolbalData&)rMsg;
    
    if (resultMsg.m_nHandBookType == eHandbookType_Costume)
    {
        ChangeData(m_MapCostume, resultMsg.m_nId , resultMsg.m_nChangeIndex ,resultMsg.m_bIsAdd);
    }
    else if (resultMsg.m_nHandBookType == eHandbookType_Effect)
    {
        ChangeData(m_MapEffect, resultMsg.m_nId , resultMsg.m_nChangeIndex ,resultMsg.m_bIsAdd);
    }
}

void CHandbookGlobalMgr::ChangeData(std::map<int , CHandbookUnit>& destMap, int Id , int index , bool isAdd)
{
    std::map<int , CHandbookUnit>::iterator it = destMap.find(Id);
    if ( it == destMap.end() )
    {
        CHandbookUnit unit(Id, 0, 0, 0);
        destMap.insert(make_pair(Id, unit) ).second;
        it = destMap.find(Id);
    }

    if (index == eHandbookDB_Praise)
    {
        if (isAdd)
        {
            it->second.SetPraisedCount(it->second.GetPraisedCount() + 1);
        }
        else
        {
            if (it->second.GetPraisedCount() > 0)
            {
                it->second.SetPraisedCount(it->second.GetPraisedCount() - 1);
            }
        }
    }
    else if (index == eHandbookDB_Envy)
    {
        if (isAdd)
        {
            it->second.SetEnvyCount(it->second.GetEnvyCount() + 1);
        }
        else
        {
            if (it->second.GetEnvyCount() > 0)
            {
                it->second.SetEnvyCount(it->second.GetEnvyCount() - 1);
            }
        }
    }
    else if (index == eHandbookDB_Collect)
    {
        if (isAdd)
        {
            it->second.SetCollectCount(it->second.GetCollectCount() + 1);
        }
        else
        {
            if (it->second.GetCollectCount() > 0)
            {
                it->second.SetCollectCount(it->second.GetCollectCount() - 1);
            }
        }
    }
}

void CHandbookGlobalMgr::RegComponentNetMsgMap()
{
    RegMsgCreatorAndProcessor(GameMsg_S2G_GolbalCostumeData, &CHandbookGlobalMgr::RetMsgGolbalCostumeData);
    RegMsgCreatorAndProcessor(GameMsg_S2G_GolbalEffectData, &CHandbookGlobalMgr::RetMsgGolbalEffectData);
    RegMsgCreatorAndProcessor(GameMsg_S2G_ChangeGolbalData, &CHandbookGlobalMgr::ChangeGolbalData);
}

void CHandbookGlobalMgr::OnUpdate(const unsigned long & nTimeElapsed)
{
    if(!m_bIsOpening)
    {
        return;
    }

    m_nInterval += nTimeElapsed;
    if (m_nInterval >= HANDBOOK_UPDATE_INTERVAL)
    {
        m_nInterval = 0;
    }
    else
    {
        return;
    }

    std::map<int ,CHandbookUnit> * pDbEffectCopy = new std::map<int ,CHandbookUnit>(m_MapEffect);
    g_pQueryMgr->AddQuery(QUERY_Handnbook_SaveGlobalEffectData, 0 ,pDbEffectCopy);

    std::map<int ,CHandbookUnit> * pDbCostumeCopy = new std::map<int ,CHandbookUnit>(m_MapCostume);
    g_pQueryMgr->AddQuery(QUERY_Handnbook_SaveGlobalCostumeData, 0 ,pDbCostumeCopy);
}

void CHandbookGlobalMgr::SetClosed()
{
    std::map<int ,CHandbookUnit> * pDbEffectCopy = new std::map<int ,CHandbookUnit>(m_MapEffect);
    g_pQueryMgr->AddQuery(QUERY_Handnbook_SaveGlobalEffectData, 0 ,pDbEffectCopy);

    std::map<int ,CHandbookUnit> * pDbCostumeCopy = new std::map<int ,CHandbookUnit>(m_MapCostume);
    g_pQueryMgr->AddQuery(QUERY_Handnbook_SaveGlobalCostumeData, 0 ,pDbCostumeCopy);

    m_MapCostume.clear();
    m_MapEffect.clear();
}

