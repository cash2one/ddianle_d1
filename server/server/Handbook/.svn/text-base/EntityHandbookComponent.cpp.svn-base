#include "EntityHandbookComponent.h"
#include "GameMsg_Handbook.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../util/CommonFunc.h"

#include <sstream>


CEntityHandbookComponent::CEntityHandbookComponent()
    : CEntityComponent(true,true), m_MapCostumeList4DB(NULL), m_MapEffectList4DB(NULL)
{
    m_bIsOpen = CSystemSettingMgr::Instance().IsFunctionOpen( EFunctionSwitch_Handbook );
}

CEntityHandbookComponent::~CEntityHandbookComponent()
{
    DeleteDbMap(m_MapCostumeList4DB);
    DeleteDbMap(m_MapEffectList4DB);

    DeleteNeedInsertMap(m_MapNeedInsertCostumeList);
    DeleteNeedInsertMap(m_MapNeedInsertEffectList);
}

void CEntityHandbookComponent::Start()
{
    m_pNetComponent = GetComponent<CRoleNet>();
    m_pAttrComponent = GetComponent<CRoleAttribute>();
}

void CEntityHandbookComponent::OnLogout()
{
    if (!m_bIsOpen)
    {
        return;
    }

    DBInsertPersonalData(eHandbookType_Costume, m_MapNeedInsertCostumeList);
    DBInsertPersonalData(eHandbookType_Effect,  m_MapNeedInsertEffectList);

    DBUpdatePersonalData(m_SetEffectUpdateID, m_MapEffectList4DB);
    DBUpdatePersonalData(m_SetCostumeUpdateID, m_MapCostumeList4DB);
}


void HandbookDecode(OUT std::list<CHandbookPersonalInfo *>& infolist, std::string src)
{
    std::vector<std::string> vecStrPcInfo;
    SplitString(src, "|", vecStrPcInfo);

    for (std::vector<std::string>::iterator it = vecStrPcInfo.begin();
        it != vecStrPcInfo.end(); ++it)
    {
        std::vector<int> perinfo;

        SplitString(it->c_str(), ",", perinfo);
        if (perinfo.size() != eHandbookDB_End)
        {
            continue;
        }

        CHandbookPersonalInfo *pInfo = new
            CHandbookPersonalInfo
            (perinfo[eHandbookDB_ID],
            perinfo[eHandbookDB_Praise],
            perinfo[eHandbookDB_Envy],
            perinfo[eHandbookDB_Collect]);

        infolist.push_back(pInfo);
    }
}

void HandbookEncode(const std::list<CHandbookPersonalInfo *> &infolist, OUT std::string &src)
{
    stringstream ss;
    ss.clear();
    for (std::list<CHandbookPersonalInfo *>::const_iterator it = infolist.begin();
        it != infolist.end(); ++it)
    {
        ss << (*it)->GetID();
        ss << ",";
        ss << (*it)->GetPraised();
        ss << ",";
        ss << (*it)->GetEnvyed();
        ss << ",";
        ss << (*it)->GetCollected();
        ss << "|";
    }
    src = ss.str();
    src = src.substr(0, src.length() - 1);
}

bool CEntityHandbookComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    //获取数据库 数据
    if (NULL == pRoleInforDB)
    {
        return false;
    }

    m_MapCostumeList4DB = new std::map<int, std::list<CHandbookPersonalInfo *> >();
    m_MapEffectList4DB = new std::map<int, std::list<CHandbookPersonalInfo *> >();
    std::map<int, std::string>::const_iterator citer;
    for (citer = pRoleInforDB->m_MapCostumePersonalInfo.begin(); citer != pRoleInforDB->m_MapCostumePersonalInfo.end(); ++citer)
    {
        std::list<CHandbookPersonalInfo *> infolist;
        HandbookDecode(infolist, citer->second);
        m_MapCostumeList4DB->insert(std::make_pair(citer->first, infolist));
    }

    for (citer = pRoleInforDB->m_MapEffectPersonalInfo.begin(); citer != pRoleInforDB->m_MapEffectPersonalInfo.end(); ++citer)
    {
        std::list<CHandbookPersonalInfo *> infolist;
        HandbookDecode(infolist, citer->second);
        m_MapEffectList4DB->insert(std::make_pair(citer->first, infolist));
    }

    return true;
}

bool CEntityHandbookComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    std::map<int, std::list<CHandbookPersonalInfo *> >::const_iterator citer;
    for (citer = m_MapCostumeList4DB->begin(); citer != m_MapCostumeList4DB->end(); ++citer)
    {
        std::string strValue;
        HandbookEncode(citer->second, strValue);
        pRoleInfoCache->m_MapCostumePersonalInfo.insert(std::make_pair(citer->first, strValue));
    }

    for (citer = m_MapEffectList4DB->begin(); citer != m_MapEffectList4DB->end(); ++citer)
    {
        std::string strValue;
        HandbookEncode(citer->second, strValue);
        pRoleInfoCache->m_MapEffectPersonalInfo.insert(std::make_pair(citer->first, strValue));
    }

    return true;
}


void CEntityHandbookComponent::RegComponentNetMsgMap()
{
    if (!m_bIsOpen)
    {
        return;
    }

    RegMsgCreatorAndProcessor(GameMsg_G2S_GolbalCostumeData, &CEntityHandbookComponent::GroupGolbalCostumeDataRet);
    RegMsgCreatorAndProcessor(GameMsg_C2S_GolbalCostumeData, &CEntityHandbookComponent::ClientGolbalCostumeData);

    RegMsgCreatorAndProcessor(GameMsg_G2S_GolbalEffectData, &CEntityHandbookComponent::GroupGolbalEffectDataRet);

    RegMsgCreatorAndProcessor(GameMsg_C2S_CostumeCollectData, &CEntityHandbookComponent::CostumeCollectRet);
    RegMsgCreatorAndProcessor(GameMsg_C2S_CostumePersonalData, &CEntityHandbookComponent::CostumePersonalDataRet);
    
    RegMsgCreatorAndProcessor(GameMsg_C2S_EffectPersonalData, &CEntityHandbookComponent::EffectPersonalDataRet);
    
    RegMsgCreatorAndProcessor(GameMsg_C2S_SetCostumeState, &CEntityHandbookComponent::SetCostumeState);
    RegMsgCreatorAndProcessor(GameMsg_C2S_SetEffectState, &CEntityHandbookComponent::SetEffectState);
}

void CEntityHandbookComponent::OnLogin()
{
    if (!m_bIsOpen)
    {
        return;
    }
    GameMsg_S2G_GolbalCostumeData GolbalCostumeMsg;
    m_pNetComponent->Send2GroupServer(&GolbalCostumeMsg);

    GameMsg_S2G_GolbalEffectData GolbalEffectMsg;
    m_pNetComponent->Send2GroupServer(&GolbalEffectMsg);
    
    InitPersonalData();
}

//////////////////////////////////////////////////////////////////////////
//func
//////////////////////////////////////////////////////////////////////////
void CEntityHandbookComponent::DBUpdatePersonalData(std::set<int> setUpdateDbKey, std::map<int , std::list<CHandbookPersonalInfo *> > *pDestMap)
{
    if (pDestMap == NULL)
    {
        return;
    }

    if (setUpdateDbKey.empty())
    {
        return;
    }

    //Key循环
    for (std::set<int>::iterator Keyit = setUpdateDbKey.begin();
        Keyit != setUpdateDbKey.end() ; ++Keyit)
    {
        //找到对应Key的list数据
        std::map<int, std::list<CHandbookPersonalInfo*> >::iterator mapit = pDestMap->find(*Keyit);
        if (mapit == pDestMap->end() )
        {
            continue;
        }
//         //复制list数据到pDBmap
//         std::list<CHandbookPersonalInfo*> copylist;
//         for(std::list<CHandbookPersonalInfo*>::iterator listit = mapit->second.begin();
//             listit != mapit->second.end(); ++listit)
//         {
//             CHandbookPersonalInfo* pTmp = new CHandbookPersonalInfo( **listit );
//             copylist.push_back(pTmp);
//         }

        std::map<int, std::string> *pDBMap = new std::map<int, std::string>();
        std::string strValue;
        HandbookEncode(mapit->second, strValue);
        pDBMap->insert(make_pair( (*Keyit), strValue) );
        AddQuery(QUERY_Handnbook_UpdatePersonalData, m_pAttrComponent->GetRoleID(), pDBMap);
    }
}

void CEntityHandbookComponent::DBInsertPersonalData(int nHandbookType, std::map<int ,CHandbookPersonalInfo *> &DestMap)
{
    std::map<int, std::string> *pDBMap = new std::map<int, std::string>();

    int Key = 0;
    std::list<CHandbookPersonalInfo*> list;
    std::string strValue;

    for (std::map<int, CHandbookPersonalInfo*>::iterator mapIt = DestMap.begin() ;
        mapIt != DestMap.end() ; ++mapIt)
    {
        CHandbookPersonalInfo* pTmp = new CHandbookPersonalInfo( *(mapIt->second) );
        list.push_back( pTmp );
        if (list.size() >= MAX_UNIT_SIZE)
        {
            HandbookEncode(list, strValue);
//            pDBMap->insert(std::make_pair(Key, list) );
            pDBMap->insert(std::make_pair(Key, strValue));
            list.clear();
            Key++;
        }
    }

    //加入最后不满
    if (list.size() != 0)
    {
        HandbookEncode(list, strValue);
        pDBMap->insert(std::make_pair(Key, strValue));
//        pDBMap->insert(std::make_pair(++Key, list) );
    }

    AddQuery(QUERY_Handnbook_InsertPersonalData, m_pAttrComponent->GetRoleID() ,pDBMap , nHandbookType);
}

void CEntityHandbookComponent::DeleteDbMap(std::map<int , std::list<CHandbookPersonalInfo *> > *pMap)
{
    for (std::map<int , std::list<CHandbookPersonalInfo *> >::iterator mapit = pMap->begin();
        mapit != pMap->end() ;++mapit)
    {
        for (std::list<CHandbookPersonalInfo *>::iterator listit = mapit->second.begin();
            listit != mapit->second.end(); ++listit)
        {
            delete (*listit);
        }
    }
    pMap->clear();
    delete pMap;
    pMap = NULL;
}

void CEntityHandbookComponent::DeleteNeedInsertMap(std::map<int, CHandbookPersonalInfo *> &mapNeedInsert)
{
    for (std::map<int , CHandbookPersonalInfo *>::iterator mapit = mapNeedInsert.begin();
        mapit != mapNeedInsert.end(); ++mapit)
    {
        delete(mapit->second);
    }
}

int CEntityHandbookComponent::FindNotFullMapDBKey(std::map<int , std::list<CHandbookPersonalInfo *> > *Map)
{
    if (Map == NULL)
    {
        return 0;
    }

    int Key = 0;
    for (std::map<int , std::list<CHandbookPersonalInfo *> >::iterator it = Map->begin();
        it != Map->end() ; ++it)
    {
        if (it->second.size() < MAX_UNIT_SIZE)
        {
            Key = it->first;
            break;
        }
    }
    return Key;
}

void CEntityHandbookComponent::InitPersonalData()
{
    for (std::map<int, std::list<CHandbookPersonalInfo *> >::iterator mapit = m_MapCostumeList4DB->begin();
        mapit != m_MapCostumeList4DB->end() ; ++mapit)
    {
        for (std::list<CHandbookPersonalInfo*>::iterator listit = mapit->second.begin();
            listit != mapit->second.end() ;++listit)
        {
            if (m_MapCostume.find((*listit)->GetID()) == m_MapCostume.end() )
            {
                m_MapCostume.insert(make_pair((*listit)->GetID(), *listit ) );
                //关联DBKEY 和 CostumeID
                m_MapCostumeIDKey.insert(make_pair((*listit)->GetID(), mapit->first) );
                //加入收藏set
                if ((*listit)->GetCollected() == TRUE)
                {
                    m_SetCollectCostume.insert((*listit)->GetID());
                }
            }
        }
    }

    for (std::map<int, std::list<CHandbookPersonalInfo *> >::iterator mapit = m_MapEffectList4DB->begin();
        mapit != m_MapEffectList4DB->end() ; ++mapit)
    {
        for (std::list<CHandbookPersonalInfo*>::iterator listit = mapit->second.begin();
            listit != mapit->second.end() ;++listit)
        {
            if (m_MapEffect.find((*listit)->GetID()) == m_MapEffect.end() )
            {
                m_MapEffect.insert(make_pair((*listit)->GetID(), *listit ) );
                //关联DBKEY 和 CostumeID
                m_MapEffectIDKey.insert(make_pair((*listit)->GetID(), mapit->first) );
//                 //加入收藏set
//                 if (listit->GetCollected() == TRUE)
//                 {
//                     m_SetCollectEffect.insert(listit->m_ID);
//                 }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//Msg
//////////////////////////////////////////////////////////////////////////
void CEntityHandbookComponent::GroupGolbalCostumeDataRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_G2S_GolbalCostumeData &groupMsg = (GameMsg_G2S_GolbalCostumeData&)rMsg;
    for (std::list<GolbalPerInfo>::const_iterator it = groupMsg.m_listPerInfo.begin();
        it != groupMsg.m_listPerInfo.end() ; ++it)
    {
        if (m_GlobalCostumeData.find(it->m_nId) == m_GlobalCostumeData.end() )
        {
            CHandbookUnit unit(it->m_nId, it->m_nPraisedCount, it->m_nEnvyCount, it->m_nCollectCount);
            m_GlobalCostumeData.insert(make_pair(it->m_nId, unit) );
        }
    }
}

void CEntityHandbookComponent::GroupGolbalEffectDataRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_G2S_GolbalEffectData &groupMsg = (GameMsg_G2S_GolbalEffectData&)rMsg;
    for (std::list<GolbalPerInfo>::const_iterator it = groupMsg.m_listPerInfo.begin();
        it != groupMsg.m_listPerInfo.end() ; ++it)
    {
        if (m_GlobalEffectData.find(it->m_nId) == m_GlobalEffectData.end() )
        {
            CHandbookUnit unit(it->m_nId, it->m_nPraisedCount, it->m_nEnvyCount, it->m_nCollectCount);
            m_GlobalEffectData.insert(make_pair(it->m_nId, unit) );
        }
    }
}

void CEntityHandbookComponent::ClientGolbalCostumeData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    int UnitCount = 0;
    GameMsg_S2C_GolbalCostumeData RetMsg;
    for (std::map<int, CHandbookUnit>::iterator it = m_GlobalCostumeData.begin();
        it != m_GlobalCostumeData.end(); ++it)
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
            m_pAttrComponent->SendPlayerMsg(&RetMsg);
            RetMsg.m_listPerInfo.clear();
            UnitCount = 0;
        }
    }

    RetMsg.m_isEnd = true;
    m_pAttrComponent->SendPlayerMsg(&RetMsg);
}

void CEntityHandbookComponent::CostumeCollectRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_S2C_CostumeCollectData RetMsg;
    for (std::set<int>::iterator it = m_SetCollectCostume.begin();
        it != m_SetCollectCostume.end(); ++it)
    {
        RetMsg.m_listCollect.push_back(*it);
    }
    m_pNetComponent->SendPlayerMsg(&RetMsg);
}

void CEntityHandbookComponent::CostumePersonalDataRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_CostumePersonalData &resultMsg = (GameMsg_C2S_CostumePersonalData&)rMsg;
    
    GameMsg_S2C_CostumePersonalData retMsg;
    std::map<int, CHandbookPersonalInfo*>::iterator  it = m_MapCostume.find(resultMsg.m_nCostumeID);
    if ( it != m_MapCostume.end() )
    {
        retMsg.m_bIsPraised = (it->second->GetPraised() == TRUE) ? true : false;
        retMsg.m_bIsCollected = (it->second->GetCollected() == TRUE) ? true : false;
        retMsg.m_bIsEnvied = (it->second->GetEnvyed() == TRUE) ? true : false;
    }
    else
    {
        std::map<int, CHandbookPersonalInfo*>::iterator  it = m_MapNeedInsertCostumeList.find(resultMsg.m_nCostumeID);
        if ( it != m_MapNeedInsertCostumeList.end() )
        {
            retMsg.m_bIsPraised = (it->second->GetPraised() == TRUE) ? true : false;
            retMsg.m_bIsCollected = (it->second->GetCollected() == TRUE) ? true : false;
            retMsg.m_bIsEnvied = (it->second->GetEnvyed() == TRUE) ? true : false;
        }
    }

    m_pNetComponent->SendPlayerMsg(&retMsg);
}

void CEntityHandbookComponent::EffectPersonalDataRet(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_EffectPersonalData &resultMsg = (GameMsg_C2S_EffectPersonalData&)rMsg;

    GameMsg_S2C_EffectPersonalData retMsg;
    std::map<int, CHandbookPersonalInfo*>::iterator  PersonalIt = m_MapEffect.find(resultMsg.m_nEffectID);
    if ( PersonalIt != m_MapEffect.end() )
    {
        retMsg.m_bIsPraised = (PersonalIt->second->GetPraised() == TRUE) ? true : false;
        retMsg.m_bIsCollected = (PersonalIt->second->GetCollected() == TRUE) ? true : false;
        retMsg.m_bIsEnvied = (PersonalIt->second->GetEnvyed() == TRUE) ? true : false;
    }
    else
    {
        std::map<int, CHandbookPersonalInfo*>::iterator  it = m_MapNeedInsertEffectList.find(resultMsg.m_nEffectID);
        if ( it != m_MapNeedInsertEffectList.end() )
        {
            retMsg.m_bIsPraised = (it->second->GetPraised() == TRUE) ? true : false;
            retMsg.m_bIsCollected = (it->second->GetCollected() == TRUE) ? true : false;
            retMsg.m_bIsEnvied = (it->second->GetEnvyed() == TRUE) ? true : false;
        }
    }

    std::map<int, CHandbookUnit>::iterator GolbalIt = m_GlobalEffectData.find(resultMsg.m_nEffectID);
    if (GolbalIt != m_GlobalEffectData.end() )
    {
        retMsg.m_nPraisedTotal = GolbalIt->second.GetPraisedCount();
        retMsg.m_nCollectedTotal = GolbalIt->second.GetCollectCount();
        retMsg.m_nEnviedTotal = GolbalIt->second.GetEnvyCount();
    }

    m_pNetComponent->SendPlayerMsg(&retMsg);
}

void CEntityHandbookComponent::SetCostumeState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_SetCostumeState & clientMsg = (GameMsg_C2S_SetCostumeState&)rMsg;
    SetStateTask(clientMsg.m_nId , eHandbookType_Costume, clientMsg.m_byType, clientMsg.m_bFlag);

}

void CEntityHandbookComponent::SetEffectState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_SetEffectState & clientMsg = (GameMsg_C2S_SetEffectState&)rMsg;
    SetStateTask(clientMsg.m_nId , eHandbookType_Effect, clientMsg.m_byType, clientMsg.m_bFlag);
}

void CEntityHandbookComponent::SetStateTask(unsigned int ID, unsigned int nType, int index ,bool isAdd)
{
    int ret = CostumeStateChangeFail;

    if (nType == eHandbookType_Costume)
    {
       ret =  SetStateTaskSubFunc(ID, nType, index, isAdd, m_MapCostume, m_MapCostumeList4DB, m_MapNeedInsertCostumeList, m_SetCostumeUpdateID, m_MapCostumeIDKey);
    }

    if (nType == eHandbookType_Effect)
    {
       ret = SetStateTaskSubFunc(ID, nType, index, isAdd, m_MapEffect, m_MapEffectList4DB, m_MapNeedInsertEffectList, m_SetEffectUpdateID, m_MapEffectIDKey);
    }

    if (ret != CostumeStateChangeFail)
    {
        //发送group
        GameMsg_S2G_ChangeGolbalData groupMsg;
        groupMsg.m_nId = ID;
        groupMsg.m_nChangeIndex = index;
        groupMsg.m_nHandBookType = nType;
        groupMsg.m_bIsAdd = isAdd;
        m_pNetComponent->Send2GroupServer(&groupMsg);
    }

    if (index == eHandbookDB_Collect && isAdd)  //客户端取消收藏不需要发送
    {
        GameMsg_S2C_SetCostumeState clientMsg;
        clientMsg.m_byRetCode = (unsigned char)ret;
        m_pNetComponent->SendPlayerMsg(&clientMsg);
    }
}

int CEntityHandbookComponent::SetStateTaskSubFunc(int nItemId 
    ,unsigned int nType
    ,int index ,bool isAdd
    ,std::map<int , CHandbookPersonalInfo*> &mapCostumeOrEffect 
    ,std::map<int , std::list<CHandbookPersonalInfo *> >* pMapCostumeOrEffectList4DB
    ,std::map<int , CHandbookPersonalInfo *> &mapNeedInsert
    ,std::set<int> &setUpdateID
    ,std::map<int, int> &mapIdDBId)
{
    int ret = 0;
    int nDbkey = 0;
    std::map<int , CHandbookPersonalInfo*>::iterator itemIt = mapCostumeOrEffect.find(nItemId);
    std::map<int , CHandbookPersonalInfo*>::iterator virtualInsertItemIt = mapNeedInsert.find(nItemId);

    //存在于正常内存数据 m_MapCostumeList4DB中
    if (itemIt != mapCostumeOrEffect.end() )
    {
        map<int, int>::iterator relationit = mapIdDBId.find(itemIt->first);
        if (relationit == mapIdDBId.end())
        {
            return CostumeStateChangeFail;
        }

        nDbkey = relationit->second;

        std::map<int, std::list<CHandbookPersonalInfo*> >::iterator Mapit = pMapCostumeOrEffectList4DB->find(nDbkey);
        if (Mapit == pMapCostumeOrEffectList4DB->end() )
        {
            return CostumeStateChangeFail;
        }

        ret = UpdatePersonalInfo(itemIt->second ,nType ,index ,isAdd);    //更新数据
        setUpdateID.insert(nDbkey);
    }
    else
    {
        //从没有过的数据 或者 等待插入数据集合
        if(virtualInsertItemIt == mapNeedInsert.end())
        {
            std::list<CHandbookPersonalInfo*> list;
            CHandbookPersonalInfo * unit = new CHandbookPersonalInfo(nItemId ,0 ,0 ,0);
            mapNeedInsert.insert( std::make_pair(nItemId, unit) );
            ret = UpdatePersonalInfo(unit ,nType ,index ,isAdd);    //更新数据
        }
        else
        {
            ret = UpdatePersonalInfo(virtualInsertItemIt->second ,nType ,index ,isAdd);    //更新数据
        }
    }

    return ret;
/*
    if ( itemIt == mapCostumeOrEffect.end() )//没有这个数据
    {
        nDbkey = FindNotFullMapDBKey( pMapCostumeOrEffectList4DB);
        if ( nDbkey == 0)  //没有空闲个人数据list
        {
            nDbkey = FindNextDBKey4Insert(pMapCostumeOrEffectList4DB);
            std::list<CHandbookPersonalInfo*> list;
            CHandbookPersonalInfo * unit = new CHandbookPersonalInfo(nItemId ,0 ,0 ,0);
                 
            ret = UpdatePersonalInfo(unit ,nType ,index ,isAdd);

            list.push_back(unit);
            pMapCostumeOrEffectList4DB->insert( make_pair(nDbkey, list) );  //插入数据
            mapIdDBId.insert(make_pair(nItemId,nDbkey) );                       //关联id 和数据库id
            setInertID.insert(nDbkey);                                 //加入新增列表
            mapCostumeOrEffect.insert(std::make_pair(nItemId, unit) );                        //加入ITEMIDKEY内存表
        }
        else    //找到空闲个人数据list
        {
            std::map<int, std::list<CHandbookPersonalInfo*> >::iterator Mapit = pMapCostumeOrEffectList4DB->find(nDbkey);
            if (Mapit == pMapCostumeOrEffectList4DB->end() )
            {
                return CostumeStateChangeFail;
            }

            CHandbookPersonalInfo * unit = new CHandbookPersonalInfo(nItemId ,0 ,0 ,0);
            ret = UpdatePersonalInfo(unit ,nType ,index ,isAdd);
            Mapit->second.push_back(unit);

            mapIdDBId.insert(make_pair(nItemId,nDbkey) );                       //关联id 和数据库id
            mapCostumeOrEffect.insert(std::make_pair(nItemId, unit));          //加入ITEMIDKEY内存表

            setUpdateID.insert(nDbkey);
        }
    }
    else
    {
        map<int, int>::iterator relationit = mapIdDBId.find(itemIt->first);
        if (relationit == mapIdDBId.end())
        {
            return CostumeStateChangeFail;
        }
        nDbkey = relationit->second;

        std::map<int, std::list<CHandbookPersonalInfo*> >::iterator Mapit = pMapCostumeOrEffectList4DB->find(nDbkey);
        if (Mapit == pMapCostumeOrEffectList4DB->end() )
        {
            return CostumeStateChangeFail;
        }

        for(std::list<CHandbookPersonalInfo*>::iterator it = Mapit->second.begin();
            it != Mapit->second.end(); ++it)
        {
            if ( (*it)->GetID() == nItemId)
            {
                ret = UpdatePersonalInfo((*it) ,nType ,index ,isAdd);    //更新数据
                break;
            }
        }
        setUpdateID.insert(nDbkey);
    }
    return ret;*/
}



int CEntityHandbookComponent::UpdatePersonalInfo(CHandbookPersonalInfo* unit ,int nHandbookType ,int index ,bool isAdd)
{
    std::map<int, CHandbookUnit>::iterator unitIt;
    if (nHandbookType == eHandbookType_Costume)
    {
        unitIt = m_GlobalCostumeData.find(unit->GetID() );
        if (unitIt == m_GlobalCostumeData.end() )
        {   
            CHandbookUnit newUnit;
            m_GlobalCostumeData.insert(std::make_pair(unit->GetID(), newUnit ) );
            unitIt = m_GlobalCostumeData.find(unit->GetID() );
        }
    }
    else if (nHandbookType == eHandbookType_Effect)
    {
        unitIt = m_GlobalEffectData.find(unit->GetID() );
        if (unitIt == m_GlobalEffectData.end() )
        {
            CHandbookUnit newUnit;
            m_GlobalEffectData.insert(std::make_pair(unit->GetID(), newUnit ) );
            unitIt = m_GlobalEffectData.find(unit->GetID() );
        }
    }
    else
    {
        return CostumeStateChangeFail;
    }

    CHandbookUnit &golbal_unit = unitIt->second;
    int ret = CostumeStateChangeFail;

    if (index == eHandbookDB_Praise)
    {
        ret = updatePraise(unit, golbal_unit, isAdd);
    }
    if (index == eHandbookDB_Collect)
    {
        ret =updateCollect(unit, golbal_unit, isAdd);
    }
    if (index == eHandbookDB_Envy)
    {
        ret = updateEnvy(unit, golbal_unit, isAdd);
    }
    return ret;
}

int CEntityHandbookComponent::updateCollect(CHandbookPersonalInfo *Pc_unit, CHandbookUnit &Golbal_unit, bool isAdd)
{
    const int MAXCOLLECT = 50;
    if (isAdd)
    {
        if (Pc_unit->GetCollected() == TRUE)
        {
            return CostumeStateChangeFail;
        }

        if ((int)m_SetCollectCostume.size() >= MAXCOLLECT)
        {
            return CostumeStateChangeMax;
        }

        Pc_unit->SetCollected(TRUE);
        m_SetCollectCostume.insert(Pc_unit->GetID() );//加入收藏set
        Golbal_unit.SetCollectCount(Golbal_unit.GetCollectCount() + 1);
    }
    else
    {
        if (Pc_unit->GetCollected() == FALSE)
        {
            return CostumeStateChangeFail;
        }
        Pc_unit->SetCollected(Pc_unit->GetCollected() - 1);
        int value = ((Golbal_unit.GetCollectCount() - 1) < 0) ? 0 : (Golbal_unit.GetCollectCount() - 1);
        Golbal_unit.SetCollectCount(value);
        m_SetCollectCostume.erase(Pc_unit->GetID() );//从收藏set中去除
    }
    return CostumeStateChangeSuc;
}

int CEntityHandbookComponent::updatePraise(CHandbookPersonalInfo *Pc_unit, CHandbookUnit &Golbal_unit, bool isAdd)
{
    if (isAdd)
    {
        if (Pc_unit->GetPraised() == TRUE)
        {
            return CostumeStateChangeFail;
        }
        Pc_unit->SetPraised(TRUE);
        Golbal_unit.SetPraisedCount(Golbal_unit.GetPraisedCount() + 1);
    }
    else
    {
        if (Pc_unit->GetPraised() == FALSE)
        {
            return CostumeStateChangeFail;
        }
        Pc_unit->SetPraised(Pc_unit->GetPraised() - 1);
        int value = ((Golbal_unit.GetPraisedCount() - 1) < 0) ? 0 : (Golbal_unit.GetPraisedCount() - 1);
        Golbal_unit.SetPraisedCount(value);
    }
    return CostumeStateChangeSuc;
}

int CEntityHandbookComponent::updateEnvy(CHandbookPersonalInfo *Pc_unit, CHandbookUnit &Golbal_unit, bool isAdd)
{
    if (isAdd)
    {
        if (Pc_unit->GetEnvyed() == TRUE)
        {
            return CostumeStateChangeFail;
        }
        Pc_unit->SetEnvyed(TRUE);
        Golbal_unit.SetEnvyCount(Golbal_unit.GetEnvyCount() + 1);
    }
    else
    {
        if (Pc_unit->GetEnvyed() == FALSE)
        {
            return CostumeStateChangeFail;
        }
        Pc_unit->SetEnvyed(Pc_unit->GetEnvyed() - 1);
        int value = ((Golbal_unit.GetEnvyCount() - 1) < 0) ? 0 : (Golbal_unit.GetEnvyCount() - 1);
        Golbal_unit.SetEnvyCount(value);
    }
    return CostumeStateChangeSuc;
}


