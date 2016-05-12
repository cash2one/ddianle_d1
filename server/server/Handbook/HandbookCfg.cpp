/************************************************************************
 * HandbookCfg.cc
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-9-15 10:22:20
 ***********************************************************************/
#include "HandbookCfg.h"
#include "../../datastructure/datainfor/FileStream.h"
#include "../../socket/Log.h"
#include "../../datastructure/datainfor/EffectConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"

extern std::string g_sPath;
#define HANDBOOK_FILE_NAME    "data/bytes/Handbook.bytes"

CHandboolCostumeCfg::CHandboolCostumeCfg()
{

}

CHandboolCostumeCfg::~CHandboolCostumeCfg()
{

}

unsigned int CHandboolCostumeCfg::GetClothAttr(EClothAttrType eType)
{
    std::map<EClothAttrType, unsigned int>::iterator it = m_mapClothAttr.find(eType);
    if (it != m_mapClothAttr.end())
    {
        return it->second;
    }

    return 0;
}

bool CHandboolCostumeCfg::LoadInfoFromFile(CFileStream &rFileStream)
{
    if ( !rFileStream.IsOpen() )
        return false;

    rFileStream.ReadInt(m_nItemId);
    rFileStream.ReadString2(m_strWayOfGain);

    unsigned short nCount = 0;
    unsigned int nParams = 0;
    rFileStream.ReadUShort(nCount);
    for ( unsigned short i = 0; i < nCount; ++i )
    {
        rFileStream.ReadUInt( nParams );
        m_listUsageSuggestion.push_back( nParams );
    }

    char nEffectLevel = 0;
    rFileStream.ReadChar(nEffectLevel);
    m_nEffectLevel = (unsigned int)nEffectLevel;

    rFileStream.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        char nAttrID = 0;
        rFileStream.ReadChar(nAttrID);
        unsigned int nAttrValue = 0;
        rFileStream.ReadUInt(nAttrValue);
        m_mapClothAttr[(EClothAttrType)nAttrID] = nAttrValue;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////

CHandbookCfgMgr::CHandbookCfgMgr()
{

}

CHandbookCfgMgr::~CHandbookCfgMgr()
{

}

void CHandbookCfgMgr::Initialize()
{
    LoadConfigFromFile();
    InitializeLevelScore();
}

void CHandbookCfgMgr::LoadConfigFromFile()
{
    std::string strFileName( g_sPath );
    strFileName.append( HANDBOOK_FILE_NAME );

    CFileStream file;
    file.open( strFileName.c_str(), "rb" );

    LoadCostumeConfigForm( file );
    LoadAttrGroupIDConfig(file);
    LoadEffectStoneConfig(file);

    file.close();
}

void CHandbookCfgMgr::LoadCostumeConfigForm( CFileStream &rFileStream )
{
    if ( !rFileStream.IsOpen() )
        return;

    m_MapCostumeCfg.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        CHandboolCostumeCfg costumeInfo;

        if ( costumeInfo.LoadInfoFromFile( rFileStream ) )
        {
            m_MapCostumeCfg.insert( std::make_pair( costumeInfo.GetItemId(), costumeInfo ) );
        }
    }
}

void CHandbookCfgMgr::LoadAttrGroupIDConfig(CFileStream &rFileStream)
{
    m_mapAttrGroupID.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        char nAttrID = 0;
        rFileStream.ReadChar(nAttrID);
        std::string  strName;
        rFileStream.ReadString2(strName);
        rFileStream.ReadString2(strName);
        char nGroupID = 0;
        rFileStream.ReadChar(nGroupID);

        m_mapAttrGroupID[(EClothAttrType)nAttrID] = (unsigned int)nGroupID;
    }
}

void CHandbookCfgMgr::LoadEffectStoneConfig(CFileStream &rFileStream)
{
    m_mapEffectStoneAttr.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        itemtype_t itemtype;
        rFileStream.ReadUInt(itemtype);

        unsigned short nAttrCount = 0;
        rFileStream.ReadUShort(nAttrCount);
        for (unsigned short j = 0; j < nAttrCount; ++j)
        {
            char nGroupID = 0;
            unsigned int nAttr = 0;
            rFileStream.ReadChar(nGroupID);
            rFileStream.ReadUInt(nAttr);
            m_mapEffectStoneAttr[itemtype].insert(make_pair((unsigned int)nGroupID, nAttr));
        }
    }
}

unsigned int CHandbookCfgMgr::GetClothLevelScore(int nLevel, bool bIsSuit) const
{
    std::map<int, int>::const_iterator it = m_mapClothLevelScore.find(nLevel);
    if (it != m_mapClothLevelScore.end())
    {
        return bIsSuit ? it->second * 2 : it->second;
    }

    return 0;
}

unsigned int CHandbookCfgMgr::GetBadgeLevelScore(int nLevel) const
{
    std::map<int, int>::const_iterator it = m_mapBadgeLevelScore.find(nLevel);
    if (it != m_mapBadgeLevelScore.end())
    {
        return it->second;
    }

    return 0;
}

void CHandbookCfgMgr::InitializeLevelScore()
{
    // 服饰光效积分配置
    int nScore[5] = { 3, 5, 10, 20, 30 };
    for (int i = (int)EClothEffectLevel_2; i <= (int)EClothEffectLevel_6; ++i)
    {
        m_mapClothLevelScore[i] = nScore[i - 2];
    }
    
    // 徽章光效积分配置
    int nBadgeScore[5] = { 8, 16, 20, 30, 40 };
    for (int i = (int)EClothEffectLevel_2; i <= (int)EClothEffectLevel_6; ++i)
    {
        m_mapBadgeLevelScore[i] = nBadgeScore[i - 2];
    }
}

void CHandbookCfgMgr::GetAllCostumeIDList(std::list<int> &rListID) const
{
    std::map<int, CHandboolCostumeCfg>::const_iterator itr;

    for ( itr = m_MapCostumeCfg.begin(); itr != m_MapCostumeCfg.end(); ++itr )
    {
        rListID.push_back( itr->first );
    }
}

void CHandbookCfgMgr::GetTagList(int nCostumeID, std::list<unsigned int> &rParams ) const
{
    std::map<int, CHandboolCostumeCfg>::const_iterator itr = m_MapCostumeCfg.find( nCostumeID );

    if ( itr != m_MapCostumeCfg.end() )
        rParams = itr->second.GetTag();
}

void CHandbookCfgMgr::GetWayOfGain(int nCostumeID, std::string &rStrWayOfGain) const
{
    std::map<int, CHandboolCostumeCfg>::const_iterator itr = m_MapCostumeCfg.find( nCostumeID );

    if ( itr != m_MapCostumeCfg.end() )
        rStrWayOfGain = itr->second.GetWayOfGain();
}

void CHandbookCfgMgr::GetUsageSuggestion(int nCostumeID, std::list<unsigned int> &rParams ) const
{
    std::map<int, CHandboolCostumeCfg>::const_iterator itr = m_MapCostumeCfg.find( nCostumeID );

    if ( itr != m_MapCostumeCfg.end() )
        rParams = itr->second.GetUsageSuggestion();
}

bool CHandbookCfgMgr::IsEffectCloth(itemtype_t nItemType) const
{
    std::map<int, CHandboolCostumeCfg>::const_iterator itr = m_MapCostumeCfg.find((int)nItemType);
    if (itr != m_MapCostumeCfg.end())
    {
        return true;
    }

    return false;
}

int CHandbookCfgMgr::GetEffectLevel(int nCostumeID) const
{
    std::map<int, CHandboolCostumeCfg>::const_iterator itr = m_MapCostumeCfg.find( nCostumeID );

    if ( itr != m_MapCostumeCfg.end() )
        return itr->second.GetEffectLevel();

    WriteLog( LOGLEVEL_ERROR, "Can not find Handbook config info by id=%d", nCostumeID );

    return 0;
}

unsigned int CHandbookCfgMgr::GetEffectLevelScore(itemtype_t nItemType, EItemClothType eClothType, bool bIsBadge) const
{
    int nLevel = GetEffectLevel((int)nItemType);
    if (bIsBadge)
    {
        return GetBadgeLevelScore(nLevel);
    }

    return GetClothLevelScore(nLevel, eClothType == eItemClothType_Suit);
}

unsigned int CHandbookCfgMgr::GetClothBaseAttr(itemtype_t nItemType, unsigned int nEffectID, EClothAttrType eType)
{
    std::map<int, CHandboolCostumeCfg>::iterator itr = m_MapCostumeCfg.find((int)nItemType);
    if (itr == m_MapCostumeCfg.end())
    {
        return 0;
    }

    if (itr->second.GetEffectLevel() <= 1 && nEffectID <= 0)
    {
        return 0;
    }

    unsigned int nAttr = itr->second.GetClothAttr(eType);
    ClothEffectConfig * pEffectConfig = ConfigManager::Instance().GetEffectConfigMgr().GetClothEffectConfig(nEffectID);
    if (pEffectConfig != NULL && pEffectConfig->m_nRelatedItem > 0)
    {
        nAttr += GetClothStoneAttr(nItemType, GetClothAttrGroupID(eType));
    }

    return nAttr;
}

unsigned int CHandbookCfgMgr::GetClothAttrGroupID(EClothAttrType eType)
{
    std::map<EClothAttrType, unsigned int>::iterator it = m_mapAttrGroupID.find(eType);
    if (it != m_mapAttrGroupID.end())
    {
        return it->second;
    }

    return 0;
}

unsigned int CHandbookCfgMgr::GetClothStoneAttr(itemtype_t nItemType, unsigned int nGroupID)
{
    std::map<itemtype_t, std::map<unsigned int, unsigned int> >::iterator it = m_mapEffectStoneAttr.find(nItemType);
    if (it != m_mapEffectStoneAttr.end())
    {
        std::map<unsigned int, unsigned int>::iterator itr = it->second.find(nGroupID);
        if (itr != it->second.end())
        {
            return itr->second;
        }
    }

    return 0;
}
