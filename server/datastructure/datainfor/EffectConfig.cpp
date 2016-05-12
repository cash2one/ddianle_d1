#include "EffectConfig.h"
#include "./FileStream.h"
#include "../../server/Macros/Macros.h"

void EffectBaseConfig::Save(CFileStream&file)
{
    file.WriteUShort(m_nEffectID) ;
    file.WriteUShort((unsigned short)m_EffectName.length()) ;
    file.WriteString(m_EffectName) ;
    file.WriteUShort(m_nboneIndex) ;
    file.WriteInt(m_nParam1) ;
    file.WriteInt(m_nParam2) ;
}

void EffectBaseConfig::Load(CFileStream&file)
{
    file.ReadUShort(m_nEffectID);
    file.ReadString2(m_EffectName);
    file.ReadUShort(m_nboneIndex);
    file.ReadInt(m_nParam1);
    file.ReadInt(m_nParam2);
}

void ChatPapawConfig::Save(CFileStream&file)
{
    file.WriteUShort(m_PapawID);
    file.WriteUShort((unsigned short)m_BGAtlasName.length()) ;
    file.WriteString(m_BGAtlasName);
    file.WriteUShort((unsigned short)m_BGSpriteName.length()) ;
    file.WriteString(m_BGSpriteName);
    file.WriteInt(m_ColorR);
    file.WriteInt(m_ColorG);
    file.WriteInt(m_ColorB);
    file.WriteInt(m_ColorA);
}

void ChatPapawConfig::Load(CFileStream& file)
{
    file.ReadUShort(m_PapawID);
    file.ReadString2(m_BGAtlasName);
    file.ReadString2(m_BGSpriteName);
    file.ReadInt(m_ColorR);
    file.ReadInt(m_ColorG);
    file.ReadInt(m_ColorB);
    file.ReadInt(m_ColorA);
}

void RoomBadgeConfig::Save(CFileStream&file)
{
    file.WriteUShort(m_RoomBadgeID);
    file.WriteUShort((unsigned short)m_ForegroundAtalasName.length()) ;
    file.WriteString(m_ForegroundAtalasName);
    file.WriteUShort((unsigned short)m_ForegroundSpriteName.length()) ;
    file.WriteString(m_ForegroundSpriteName);
    file.WriteUShort((unsigned short)m_BackgroundAtalasName.length()) ;
    file.WriteString(m_BackgroundAtalasName);
    file.WriteUShort((unsigned short)m_BackgroundSpriteName.length()) ;
    file.WriteString(m_BackgroundSpriteName);
    file.WriteUShort((unsigned short)m_MiShiDiAtalasName.length()) ;
    file.WriteString(m_MiShiDiAtalasName);
    file.WriteUShort((unsigned short)m_MiShiDiSpriteName.length()) ;
    file.WriteString(m_MiShiDiSpriteName);
    file.WriteUShort((unsigned short)m_RoomBackgroundAtalasName.length()) ;
    file.WriteString(m_RoomBackgroundAtalasName);
    file.WriteUShort((unsigned short)m_RoomBackgroundSpriteName.length()) ;
    file.WriteString(m_RoomBackgroundSpriteName);
}

void RoomBadgeConfig::Load(CFileStream&file)
{
    file.ReadUShort(m_RoomBadgeID);
    file.ReadString2(m_ForegroundAtalasName);
    file.ReadString2(m_ForegroundSpriteName);
    file.ReadString2(m_BackgroundAtalasName);
    file.ReadString2(m_BackgroundSpriteName);
    file.ReadString2(m_MiShiDiAtalasName);
    file.ReadString2(m_MiShiDiSpriteName);
    file.ReadString2(m_RoomBackgroundAtalasName);
    file.ReadString2(m_RoomBackgroundSpriteName);
}

void HornEffectConfig::Save(CFileStream&file)
{
    file.WriteChar(m_HornType);
    file.WriteUInt(m_HornItemID);
    file.WriteUShort((unsigned short)m_HornBtnName.length()) ;
    file.WriteString(m_HornBtnName);
    file.WriteUShort((unsigned short)m_HornEffectName.length()) ;
    file.WriteString(m_HornEffectName);
}

void HornEffectConfig::Load(CFileStream& file)
{
    file.ReadChar(m_HornType);
    file.ReadUInt(m_HornItemID);
    file.ReadString2(m_HornBtnName);
    file.ReadString2(m_HornEffectName);
}

void CDanceGroupBadgeConfig::Save( CFileStream &file )
{
    file.WriteUShort( nId );
    file.WriteUShort( (unsigned short)m_tuanhuiBadgeName.length() );
    file.WriteString( m_tuanhuiBadgeName );
    file.WriteUShort( (unsigned short)m_tuanhuiBadgeSpriteName.length() );
    file.WriteString( m_tuanhuiBadgeSpriteName );
    file.WriteUShort( (unsigned short)m_tuanhuiBadgeAtlasName.length() );
    file.WriteString( m_tuanhuiBadgeAtlasName);
    file.WriteUShort( m_nUnlockLevel );
    file.WriteInt( nParam1 );
    file.WriteInt( nParam2 );
    file.WriteInt( nParam3 );
    file.WriteInt( nParam4 );
    file.WriteInt( nParam5 );
    file.WriteInt( nParam6 );
    file.WriteInt( nParam7 );
    file.WriteInt( nParam8 );
    file.WriteInt( nParam9 );
}//end

bool CDanceGroupBadgeConfig::Load( CFileStream& file )
{
    file.ReadUShort( nId );
    unsigned short nLen = 0;
    file.ReadUShort( nLen );
    file.ReadString( m_tuanhuiBadgeName, nLen );
    file.ReadUShort( nLen );
    file.ReadString( m_tuanhuiBadgeSpriteName, nLen );
    file.ReadUShort( nLen );
    file.ReadString( m_tuanhuiBadgeAtlasName, nLen );
    file.ReadUShort( m_nUnlockLevel );
    file.ReadInt( nParam1 );
    file.ReadInt( nParam2 );
    file.ReadInt( nParam3 );
    file.ReadInt( nParam4 );
    file.ReadInt( nParam5 );
    file.ReadInt( nParam6 );
    file.ReadInt( nParam7 );
    file.ReadInt( nParam8 );
    file.ReadInt( nParam9 );

    return true;
}//end

void CDanceGroupBadgeEffectConfig::Save( CFileStream &file )
{
    file.WriteUShort( nId );
    file.WriteUShort( (unsigned short)m_tuanhuiEffectName.length() );
    file.WriteString( m_tuanhuiEffectName );
    file.WriteUShort( (unsigned short)m_tuanhuiEffectSpriteName.length() );
    file.WriteString( m_tuanhuiEffectSpriteName );
    file.WriteUShort( (unsigned short)m_tuanhuiEffectAtlasName.length() );
    file.WriteString( m_tuanhuiEffectAtlasName );
    file.WriteInt( nParam1 );
    file.WriteInt( nParam2 );
    file.WriteInt( nParam3 );
    file.WriteInt( nParam4 );
    file.WriteInt( nParam5 );
    file.WriteInt( nParam6 );
    file.WriteInt( nParam7 );
    file.WriteInt( nParam8 );
    file.WriteInt( nParam9 );
}//end

bool CDanceGroupBadgeEffectConfig::Load( CFileStream& file )
{
    file.ReadUShort( nId );
    unsigned short nLen = 0;
    file.ReadUShort( nLen );
    file.ReadString( m_tuanhuiEffectName, nLen );
    file.ReadUShort( nLen );
    file.ReadString( m_tuanhuiEffectSpriteName, nLen );
    file.ReadUShort( nLen );
    file.ReadString( m_tuanhuiEffectAtlasName, nLen );
    file.ReadInt( nParam1 );
    file.ReadInt( nParam2 );
    file.ReadInt( nParam3 );
    file.ReadInt( nParam4 );
    file.ReadInt( nParam5 );
    file.ReadInt( nParam6 );
    file.ReadInt( nParam7 );
    file.ReadInt( nParam8 );
    file.ReadInt( nParam9 );

    return true;
}//end

CWeddingRingEffectConfig::CWeddingRingEffectConfig()
{
    m_nEffectID = 0;
}

CWeddingRingEffectConfig::~CWeddingRingEffectConfig()
{

}

void CWeddingRingEffectConfig::Save(CFileStream& file)
{
    file.WriteUInt(m_nEffectID);
    file.WriteString2(m_strEffectName);
}

bool CWeddingRingEffectConfig::Load(CFileStream& file)
{
    file.ReadUInt(m_nEffectID);

    unsigned short nLen = 0;
    file.ReadUShort( nLen );
    file.ReadString( m_strEffectName, nLen );

    return true;
}

WeddingSceneEffectConfig::WeddingSceneEffectConfig()
    :m_nEffectID(0)
    ,m_strEffectName("")
    ,m_nBlessingValue(0)
{

}

WeddingSceneEffectConfig::~WeddingSceneEffectConfig()
{

}

void WeddingSceneEffectConfig::Load(CFileStream & file)
{
    file.ReadUInt(m_nEffectID);
    file.ReadString2(m_strEffectName);
    file.ReadUInt(m_nBlessingValue);
}

void WeddingSceneEffectConfig::Save(CFileStream & file)
{
    file.WriteUInt(m_nEffectID);

    unsigned short nLen = 0;
    file.ReadUShort( nLen );
    file.ReadString( m_strEffectName, nLen );

    file.WriteUInt(m_nBlessingValue);
}

ClothEffectConfig::ClothEffectConfig()
    :m_nEffectID(0)
    ,m_nSuitID(0)
    ,m_strEffectName("")
    ,m_strEffectResource("")
    ,m_nLevel(0)
    ,m_nClothPos(0)
    ,m_nRelatedItem(0)
    ,m_nCostCount(0)
{

}

ClothEffectConfig::~ClothEffectConfig()
{

}

void ClothEffectConfig::Save(CFileStream & file)
{
    file.WriteUInt(m_nEffectID);
    file.WriteUShort(m_nSuitID);
    file.WriteString2(m_strEffectName);
    file.WriteString2(m_strEffectResource);
    file.WriteUShort(m_nLevel);
    file.WriteUShort(m_nClothPos);
    file.WriteUInt(m_nRelatedItem);
    file.WriteUInt(m_nCostCount);
}

void ClothEffectConfig::Load(CFileStream & file)
{
    file.ReadUInt(m_nEffectID);
    file.ReadUShort(m_nSuitID);

    unsigned short nLen = 0;
    file.ReadUShort( nLen );
    file.ReadString( m_strEffectName, nLen );

    file.ReadUShort( nLen );
    file.ReadString( m_strEffectResource, nLen );

    file.ReadUShort(m_nLevel);
    file.ReadUShort(m_nClothPos);
    file.ReadUInt(m_nRelatedItem);
    file.ReadUInt(m_nCostCount);
}

ClothEffectUpgrade::ClothEffectUpgrade()
    :m_nEffectID(0)
    ,m_nUpgradeID(0)
    ,m_nCostMoneyType(0)
    ,m_nCostMoneyNum(0)
    ,m_nDiscountCostMoneyType(0)
    ,m_nDiscountCostMoneyNum(0)
{

}

ClothEffectUpgrade::~ClothEffectUpgrade()
{

}

void ClothEffectUpgrade::Save(CFileStream & file)
{
    file.WriteUInt(m_nEffectID);
    file.WriteUInt(m_nUpgradeID);

    file.WriteUShort((unsigned short)m_listCostItem.size());
    std::list<CItem>::iterator it = m_listCostItem.begin();
    for (; it != m_listCostItem.end(); ++it)
    {
        file.WriteUInt(it->m_nItemType);
        file.WriteUShort(it->m_nItemCount);
        file.WriteInt(it->m_nValidTime);
    }

    file.WriteUShort(m_nCostMoneyType);
    file.WriteUInt(m_nCostMoneyNum);

    file.WriteUShort((unsigned short)m_listDiscountCostItem.size());
    std::list<CItem>::iterator iter = m_listDiscountCostItem.begin();
    for (; iter != m_listDiscountCostItem.end(); ++iter)
    {
        file.WriteUInt(iter->m_nItemType);
        file.WriteUShort(iter->m_nItemCount);
        file.WriteInt(iter->m_nValidTime);
    }

    file.WriteUShort(m_nDiscountCostMoneyType);
    file.WriteUInt(m_nDiscountCostMoneyNum);
}

void ClothEffectUpgrade::Load(CFileStream & file)
{
    file.ReadUInt(m_nEffectID);
    file.ReadUInt(m_nUpgradeID);

    unsigned short nItemCount = 0;
    file.ReadUShort(nItemCount);
    for (unsigned short i = 0 ; i < nItemCount; ++i)
    {
        CItem item;
        file.ReadUInt(item.m_nItemType);
        file.ReadUShort(item.m_nItemCount);
        file.ReadInt(item.m_nValidTime);
        m_listCostItem.push_back(item);
    }

    file.ReadUShort(m_nCostMoneyType);
    file.ReadUInt(m_nCostMoneyNum);

    file.ReadUShort(nItemCount);
    for (unsigned short i = 0 ; i < nItemCount; ++i)
    {
        CItem item;
        file.ReadUInt(item.m_nItemType);
        file.ReadUShort(item.m_nItemCount);
        file.ReadInt(item.m_nValidTime);
        m_listDiscountCostItem.push_back(item);
    }

    file.ReadUShort(m_nDiscountCostMoneyType);
    file.ReadUInt(m_nDiscountCostMoneyNum);
}

ClothCombinationalEffectConfig::ClothCombinationalEffectConfig()
    :m_nCombinationID(0)
    ,m_strName("")
    ,m_strResourceName("")
{

}

ClothCombinationalEffectConfig::~ClothCombinationalEffectConfig()
{

}

void ClothCombinationalEffectConfig::Save(CFileStream & file)
{
    file.WriteUInt(m_nCombinationID);
    file.WriteString2(m_strName);
    file.WriteUShort((unsigned short)m_listClothEffectID.size());
    std::list<unsigned int>::iterator it = m_listClothEffectID.begin();
    for (; it != m_listClothEffectID.end(); ++it)
    {
        file.WriteUInt(*it);
    }

    file.WriteString2(m_strResourceName);
}

void ClothCombinationalEffectConfig::Load(CFileStream & file)
{
    file.ReadUInt(m_nCombinationID);

    unsigned short nLen = 0;
    file.ReadUShort( nLen );
    file.ReadString( m_strName, nLen );

    unsigned short nEffectCount = 0;
    file.ReadUShort(nEffectCount);
    for (unsigned short i = 0; i < nEffectCount; ++i)
    {
        unsigned int nEffectID = 0;
        file.ReadUInt(nEffectID);
        m_listClothEffectID.push_back(nEffectID);
    }

    file.ReadUShort( nLen );
    file.ReadString( m_strResourceName, nLen );
}

ClothEffectSuit::ClothEffectSuit()
    :m_nSuitID(0)
    ,m_strName("")
    ,m_strDesc("")
    ,m_strIcon("")
    ,m_strAtlas("")
{

}

ClothEffectSuit::~ClothEffectSuit()
{

}

void ClothEffectSuit::Save(CFileStream & file)
{
    file.WriteUShort(m_nSuitID);
    file.WriteString2(m_strName);
    file.WriteString2(m_strDesc);
    file.WriteString2(m_strIcon);
    file.WriteString2(m_strAtlas);
}

void ClothEffectSuit::Load(CFileStream & file)
{
    file.ReadUShort(m_nSuitID);

    unsigned short nLen = 0;
    file.ReadUShort( nLen );
    file.ReadString( m_strName, nLen );

    file.ReadUShort( nLen );
    file.ReadString( m_strDesc, nLen );

    file.ReadUShort( nLen );
    file.ReadString( m_strIcon, nLen );

    file.ReadUShort( nLen );
    file.ReadString( m_strAtlas, nLen );
}

ClothEffectPos::ClothEffectPos()
{

}

ClothEffectPos::~ClothEffectPos()
{

}

void ClothEffectPos::LoadData(unsigned short nPos, unsigned int nEffectID)
{
    std::map<unsigned short, std::set<unsigned int> >::iterator it = m_mapPosEffectID.find(nPos);
    if (it != m_mapPosEffectID.end())
    {
        it->second.insert(nEffectID);
    }
    else
    {
        m_mapPosEffectID[nPos].insert(nEffectID);
    }
}

void ClothEffectPos::GetBetweenEffectID(unsigned short nPos, unsigned int nLastEffectID, unsigned int nNewEffectID, std::set<unsigned int> & setEffectID)
{
    std::map<unsigned short,std::set<unsigned int> >::iterator it = m_mapPosEffectID.find(nPos);
    if (it != m_mapPosEffectID.end())
    {
        std::set<unsigned int>::iterator iter = it->second.begin();
        for (; iter != it->second.end(); ++iter)
        {
            unsigned int nEffectID = *iter;
            if (nEffectID > nLastEffectID && nEffectID <= nNewEffectID)
            {
                setEffectID.insert(nEffectID);
            }
        }
    }
}

unsigned int ClothEffectPos::GetMaxLevelEffectID(unsigned short nPos)
{
    std::map<unsigned short,std::set<unsigned int> >::iterator it = m_mapPosEffectID.find(nPos);
    if (it != m_mapPosEffectID.end())
    {
        std::set<unsigned int>::reverse_iterator iter = it->second.rbegin();
        if (iter != it->second.rend())
        {
            return *iter;
        }
    }

    return 0;
}

unsigned int ClothEffectPos::GetEffectNum()
{
    unsigned int nEffectNum = 0;
    std::map<unsigned short,std::set<unsigned int> >::iterator it = m_mapPosEffectID.begin();
    for (; it != m_mapPosEffectID.end(); ++it)
    {
        nEffectNum += (unsigned int)it->second.size();
    }

    return nEffectNum;
}

unsigned int ClothEffectPos::GetEffectNum(unsigned short nPos)
{
    unsigned int nEffectNum = 0;
    std::map<unsigned short,std::set<unsigned int> >::iterator it = m_mapPosEffectID.find(nPos);
    if (it != m_mapPosEffectID.end())
    {
        return (unsigned int)it->second.size();
    }

    return nEffectNum;
}

void ClothEffectPos::GetClothPos(std::set<EItemClothType> & setClothPos)
{
    std::map<unsigned short,std::set<unsigned int> >::iterator it = m_mapPosEffectID.begin();
    for (; it != m_mapPosEffectID.end(); ++it)
    {
        EItemClothType eType = (EItemClothType)it->first;
        setClothPos.insert(eType);
    }
}




