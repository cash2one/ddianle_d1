#include "EffectConfigManager.h"
#include "./FileStream.h"
#include "../../server/Macros/Macros.h"

EffectManager::EffectManager()
{
    Initialize();
}

EffectManager::~EffectManager()
{
}

bool EffectManager::Load(std::string& fileName)
{
    CFileStream file;
    file.open( fileName.c_str(), "rb" );

    unsigned short nCount = 0;
    file.ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++ )
    {
        EffectBaseConfig doc;
        doc.Load(file);
        m_EffectInfoList.insert(make_pair(doc.m_nEffectID,doc));
    }
    file.ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++ )
    {
        ChatPapawConfig doc;
        doc.Load(file);
        m_ChatPapawList.insert(make_pair(doc.m_PapawID,doc));
    }
    file.ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++ )
    {
        RoomBadgeConfig doc;
        doc.Load(file);
        m_RoomBadgeList.insert(make_pair(doc.m_RoomBadgeID,doc));
    }
    file.ReadUShort(nCount);
    for (unsigned short i = 0 ; i < nCount ; i++ )
    {
        HornEffectConfig doc;
        doc.Load(file);
        m_HornList.insert(make_pair(doc.m_HornType,doc));
    }

    file.ReadUShort( nCount );
    for( unsigned short i = 0;i < nCount; ++i )
    {
        CDanceGroupBadgeConfig *pInfo = new CDanceGroupBadgeConfig();
        pInfo->Load( file );
        m_mDGBadge.insert( make_pair( pInfo->nId, pInfo ) );
    }

    file.ReadUShort( nCount );
    for( unsigned short i = 0;i < nCount; ++i )
    {
        CDanceGroupBadgeEffectConfig *pInfo = new CDanceGroupBadgeEffectConfig();
        pInfo->Load( file );
        m_mDGEffect.insert( make_pair( pInfo->nId, pInfo ) );
    }

    file.ReadUShort(nCount);
    for (unsigned int i = 0 ; i < nCount ; i++)
    {
        CWeddingRingEffectConfig* pInfo = new CWeddingRingEffectConfig();
        pInfo->Load(file);
        m_mWREffect.insert(make_pair((unsigned short)pInfo->m_nEffectID,pInfo));
    }

    file.ReadUShort(nCount);
    for (unsigned int i = 0 ; i < nCount ; i++)
    {
        WeddingSceneEffectConfig tempInfo;
        tempInfo.Load(file);
    }

    file.ReadUShort(nCount);
    for (unsigned int i = 0 ; i < nCount ; i++)
    {
        ClothEffectConfig* pInfo = new ClothEffectConfig();
        pInfo->Load(file);
        m_mapClothEffect.insert(make_pair(pInfo->m_nEffectID, pInfo));
        m_mapClothEffectItem[pInfo->m_nRelatedItem] = pInfo->m_nEffectID;
        LoadClothEffectPos(pInfo);
    }

    file.ReadUShort(nCount);
    for (unsigned int i = 0 ; i < nCount ; i++)
    {
        ClothCombinationalEffectConfig tempInfo;
        tempInfo.Load(file);
    }

    file.ReadUShort(nCount);
    for (unsigned int i = 0 ; i < nCount ; i++)
    {
        ClothEffectUpgrade * pInfo = new ClothEffectUpgrade();
        pInfo->Load(file);
        m_mapClothEffectUpgrade[pInfo->m_nEffectID] = pInfo;
    }

    file.ReadUShort(nCount);
    for (unsigned int i = 0 ; i < nCount ; i++)
    {
        ClothEffectSuit tempInfo;
        tempInfo.Load(file);
    }

    file.close();

    return true;
}

bool EffectManager::Save(std::string& fileName)
{
    CFileStream file;
    file.open( fileName.c_str(), "rw" );

    file.WriteUShort((unsigned short)m_EffectInfoList.size());
    for (std::map<unsigned short,EffectBaseConfig>::iterator it = m_EffectInfoList.begin() ; it != m_EffectInfoList.end() ; it++)
    {
        it->second.Save(file);
    }
    file.WriteUShort((unsigned short)m_ChatPapawList.size());
    for (std::map<unsigned short,ChatPapawConfig>::iterator it = m_ChatPapawList.begin(); it != m_ChatPapawList.end() ; it++)
    {
        it->second.Save(file);
    }
    file.WriteUShort((unsigned short)m_RoomBadgeList.size());
    for (std::map<unsigned short,RoomBadgeConfig>::iterator it = m_RoomBadgeList.begin() ; it != m_RoomBadgeList.end();it++)
    {
        it->second.Save(file);
    }
    file.WriteUShort((unsigned short)m_HornList.size());
    for (std::map<char,HornEffectConfig>::iterator it = m_HornList.begin() ; it != m_HornList.end() ; it++)
    {
        it->second.Save(file);
    }
    file.WriteUShort((unsigned short)m_mDGBadge.size());
    for (std::map<unsigned short, CDanceGroupBadgeConfig*>::iterator it = m_mDGBadge.begin(); it != m_mDGBadge.end();it++)
    {
        it->second->Save(file);
    }
    file.WriteUShort((unsigned short)m_mDGEffect.size());
    for (std::map<unsigned short, CDanceGroupBadgeEffectConfig*>::iterator it = m_mDGEffect.begin() ; it != m_mDGEffect.end() ; it++)
    {
        it->second->Save(file);
    }
    file.WriteUShort((unsigned short)m_mWREffect.size());
    for (std::map<unsigned short, CWeddingRingEffectConfig*>::iterator it = m_mWREffect.begin(); it != m_mWREffect.end() ; it++)
    {
        it->second->Save(file);
    }
    file.close();

    return true;
}

void EffectManager::Initialize()
{
    int nScore[8] = {3, 5, 10, 20, 30, 40, 50, 60};
    for (unsigned short i = 1; i <= 8; ++i)
    {    
        m_mapClothEffectLevelScore[i].insert(make_pair(eItemClothType_Hair, nScore[i - 1]));
        m_mapClothEffectLevelScore[i].insert(make_pair(eItemClothType_Body, nScore[i - 1]));
        m_mapClothEffectLevelScore[i].insert(make_pair(eItemClothType_Leg, nScore[i - 1]));
        m_mapClothEffectLevelScore[i].insert(make_pair(eItemClothType_Feet, nScore[i - 1]));
        m_mapClothEffectLevelScore[i].insert(make_pair(eItemClothType_Suit, nScore[i - 1]*2));
    }
}

itemtype_t EffectManager::GetHornItemBySpeckType(char type)
{
    std::map<char,HornEffectConfig>::iterator it = m_HornList.find(type);
    if (it != m_HornList.end())
    {
        return it->second.m_HornItemID;
    }
    return 0;
}

CDanceGroupBadgeConfig* EffectManager::GetDanceGroupBadgeByID( unsigned short unId )
{
    std::map<unsigned short, CDanceGroupBadgeConfig*>::iterator iter = m_mDGBadge.find( unId );
    if( iter != m_mDGBadge.end() )
        return iter->second;
    return NULL;
}//end

CDanceGroupBadgeEffectConfig* EffectManager::GetDanceGroupBadgeEffectByID( unsigned short unId )
{
    std::map<unsigned short, CDanceGroupBadgeEffectConfig*>::iterator iter = m_mDGEffect.find( unId );
    if( iter != m_mDGEffect.end() )
        return iter->second;

    return NULL;
}//end

std::map<unsigned short, CDanceGroupBadgeConfig*> & EffectManager::GetDanceGroupBageList()
{
    return m_mDGBadge; 
}

std::map<unsigned short, CDanceGroupBadgeEffectConfig*> & EffectManager::GetDanceGroupBadgeEffectList()
{
    return m_mDGEffect; 
}

void EffectManager::Destroy()
{
    std::map<unsigned short, CDanceGroupBadgeConfig*>::iterator iter = m_mDGBadge.begin(), iterEnd = m_mDGBadge.end();
    for( ; iter != iterEnd; ++iter )
    {
        SAFE_DELETE( iter->second );
    }
    m_mDGBadge.clear();

    std::map<unsigned short, CDanceGroupBadgeEffectConfig*>::iterator iter1 = m_mDGEffect.begin(), iter1End = m_mDGEffect.end();
    for( ; iter1 != iter1End; ++iter1 )
    {
        SAFE_DELETE( iter1->second );
    }
    m_mDGEffect.clear();

    std::map<unsigned short, CWeddingRingEffectConfig*>::iterator iterWR = m_mWREffect.begin();
    for (;iterWR != m_mWREffect.end() ; iterWR++)
    {
        SAFE_DELETE(iterWR->second);
    }
    m_mWREffect.clear();

    std::map<unsigned int,ClothEffectConfig*>::iterator itClothEffect = m_mapClothEffect.begin();
    for (;itClothEffect != m_mapClothEffect.end(); ++itClothEffect)
    {
        SAFE_DELETE(itClothEffect->second);
    }
    m_mapClothEffect.clear();
    m_mapClothEffectItem.clear();

    std::map<unsigned int,ClothEffectUpgrade*>::iterator itUpgrade = m_mapClothEffectUpgrade.begin();
    for (; itUpgrade != m_mapClothEffectUpgrade.end(); ++itUpgrade)
    {
        SAFE_DELETE(itUpgrade->second);
    }
    m_mapClothEffectUpgrade.clear();

    std::map<unsigned short,ClothEffectPos*>::iterator itPos = m_mapClothEffectPos.begin();
    for (; itPos != m_mapClothEffectPos.end(); ++itPos)
    {
        SAFE_DELETE(itPos->second);
    }
    m_mapClothEffectPos.clear();
}

ClothEffectConfig * EffectManager::GetClothEffectConfig(unsigned int nEffectID)
{
    std::map<unsigned int,ClothEffectConfig*>::iterator itClothEffect = m_mapClothEffect.find(nEffectID);
    if (itClothEffect != m_mapClothEffect.end())
    {
        return itClothEffect->second;
    }

    return NULL;
}

unsigned int EffectManager::GetClothEffectScore(unsigned int nEffectID, EItemClothType eClothType)
{
    ClothEffectConfig * pConfig = GetClothEffectConfig(nEffectID);
    if (pConfig != NULL)
    {
        std::map<unsigned short, std::map<EItemClothType, int> >::iterator it = m_mapClothEffectLevelScore.find(pConfig->m_nLevel);
        if (it != m_mapClothEffectLevelScore.end())
        {
            std::map<EItemClothType, int>::iterator itr = it->second.find(eClothType);
            if (itr != it->second.end())
            {
                return itr->second;
            }
        }
    }

    return 0;
}

ClothEffectConfig * EffectManager::GetNextLevelEffectConfig(unsigned int nEffectID)
{
    std::map<unsigned int,ClothEffectUpgrade*>::iterator it = m_mapClothEffectUpgrade.find(nEffectID);
    if (it != m_mapClothEffectUpgrade.end())
    {
        return GetClothEffectConfig(it->second->m_nUpgradeID);
    }

    return NULL;
}

ClothEffectUpgrade * EffectManager::GetClothEffectUpgradeConfig(unsigned int nEffectID)
{
    std::map<unsigned int,ClothEffectUpgrade*>::iterator it = m_mapClothEffectUpgrade.find(nEffectID);
    if (it != m_mapClothEffectUpgrade.end())
    {
        return it->second;
    }

    return NULL;
}

ClothEffectConfig * EffectManager::GetClothEffectConfigByItem(itemtype_t nItemType)
{
    std::map<itemtype_t,unsigned int>::iterator it = m_mapClothEffectItem.find(nItemType);
    if (it != m_mapClothEffectItem.end())
    {
        return GetClothEffectConfig(it->second);
    }

    return NULL;
}

unsigned int EffectManager::GetClothEffectIDByItem(itemtype_t nItemType)
{
    ClothEffectConfig * pConfig = GetClothEffectConfigByItem(nItemType);
    if (pConfig != NULL)
    {
        return pConfig->m_nEffectID;
    }

    return 0;
}

void EffectManager::LoadClothEffectPos(ClothEffectConfig * pEffectCnfig)
{
    if (pEffectCnfig == NULL)
    {
        return;
    }

    std::map<unsigned short,ClothEffectPos*>::iterator it = m_mapClothEffectPos.find(pEffectCnfig->m_nSuitID);
    if (it != m_mapClothEffectPos.end())
    {
        it->second->LoadData(pEffectCnfig->m_nClothPos, pEffectCnfig->m_nEffectID);
    }
    else
    {
        ClothEffectPos * pPos = new ClothEffectPos();
        m_mapClothEffectPos[pEffectCnfig->m_nSuitID] = pPos;
        pPos->LoadData(pEffectCnfig->m_nClothPos, pEffectCnfig->m_nEffectID);
    }
}

void EffectManager::GetBetweenEffectID(unsigned short nSuitID, unsigned short nClothPos, unsigned int nLastEffectID, 
                        unsigned int nNewEffectID, std::set<unsigned int> & setEffectID)
{
    std::map<unsigned short,ClothEffectPos*>::iterator it = m_mapClothEffectPos.find(nSuitID);
    if (it != m_mapClothEffectPos.end())
    {
        it->second->GetBetweenEffectID(nClothPos, nLastEffectID, nNewEffectID, setEffectID);
    }
}

unsigned int EffectManager::GetMaxLevelEffectID(unsigned short nSuitID, unsigned short nClothPos)
{
    std::map<unsigned short,ClothEffectPos*>::iterator it = m_mapClothEffectPos.find(nSuitID);
    if (it != m_mapClothEffectPos.end())
    {
        return it->second->GetMaxLevelEffectID(nClothPos);
    }

    return 0;
}

unsigned int EffectManager::GetSuitEffectNum(unsigned short nSuitID)
{
    std::map<unsigned short,ClothEffectPos*>::iterator it = m_mapClothEffectPos.find(nSuitID);
    if (it != m_mapClothEffectPos.end())
    {
        return it->second->GetEffectNum();
    }

    return 0;
}

unsigned int EffectManager::GetSuitEffectNum(unsigned short nSuitID, unsigned short nPos)
{
    std::map<unsigned short,ClothEffectPos*>::iterator it = m_mapClothEffectPos.find(nSuitID);
    if (it != m_mapClothEffectPos.end())
    {
        return it->second->GetEffectNum(nPos);
    }

    return 0;
}

void EffectManager::GetSuitClothPos(unsigned short nSuitID, std::set<EItemClothType> & setClothPos)
{
    std::map<unsigned short,ClothEffectPos*>::iterator it = m_mapClothEffectPos.find(nSuitID);
    if (it != m_mapClothEffectPos.end())
    {
        it->second->GetClothPos(setClothPos);
    }
}

//end


