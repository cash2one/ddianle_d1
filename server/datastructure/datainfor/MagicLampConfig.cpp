#include "MagicLampConfig.h"
#include "./FileStream.h"

CBossConfig::CBossConfig()
    :m_nBossID(0)
    ,m_strName("")
    ,m_cSex(0)
    ,m_cSKinColor(0)
{
}

bool CBossConfig::Load(CFileStream&file)
{
    char cBossId = 0;
    file.ReadChar( cBossId);
    m_nBossID = cBossId;

    unsigned short nNameLength = 0;
    file.ReadUShort(nNameLength) ;
    file.ReadString(m_strName, nNameLength) ;

    file.ReadChar( m_cSex );

    unsigned int cColor = 0;
    file.ReadUInt( cColor );
    m_cSKinColor = cColor;

    itemtype_t nItemType = 0;
    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Hair ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Face ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Body ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Glove ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Leg ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Cap ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_FacialContent ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Shoulder ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Wing ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_LeftHand ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_RightHand ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Wrist ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Hip ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Sock ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_Feet ] = nItemType;

    file.ReadUInt( nItemType );
    m_Equips[ eItemClothType_All ] = nItemType;

    return true;
}

bool CBossConfig::Save(CFileStream&file)
{
    file.WriteChar((char)m_nBossID);
    file.WriteUShort((unsigned short)m_strName.size());
    file.WriteString(m_strName);
    file.WriteChar( m_cSex );
    file.WriteUShort((unsigned char)m_cSKinColor);

    std::map<EItemClothType, itemtype_t>::iterator it = m_Equips.begin();
    for ( ; it != m_Equips.end(); ++it )
    {
        file.WriteUInt( it->second );
    }

    return true;
}


CDungeonConfig::CDungeonConfig()
    :m_nDungeonID(0)
    ,m_strDungeonName("")
    ,m_strIconName("")
    ,m_strIconAtlas("")
    ,m_nStoryID(0)
{
}

bool CDungeonConfig::Load(CFileStream&file)
{
    file.ReadUInt( m_nDungeonID );

    short nCount = 0;
    file.ReadShort(nCount) ;
    file.ReadString(m_strDungeonName, nCount);

    file.ReadShort(nCount) ;
    file.ReadString(m_strIconName, nCount);

    file.ReadShort(nCount) ;
    file.ReadString(m_strIconAtlas, nCount);

    file.ReadUInt(m_nStoryID);
    unsigned short nParcloseCount = 0;
    file.ReadUShort( nParcloseCount );
    for ( unsigned int i  = 0 ;i <  nParcloseCount; ++i )
    {
        unsigned int nParcloseID = 0;
        file.ReadUInt( nParcloseID );
        m_ParcloseList.push_back( nParcloseID );
    }

    return true;
}

bool CDungeonConfig::Save(CFileStream&file)
{
    file.WriteUInt( m_nDungeonID );
    file.WriteUShort((unsigned short)m_strDungeonName.size());
    file.WriteString(m_strDungeonName);

    file.WriteUShort((unsigned short)m_strIconName.size());
    file.WriteString(m_strIconName);

    file.WriteUShort((unsigned short)m_strIconAtlas.size());
    file.WriteString(m_strIconAtlas);

    file.WriteUInt( m_nStoryID );
    file.WriteUShort( (unsigned short)m_ParcloseList.size() );

    for ( size_t i = 0; i < m_ParcloseList.size(); ++i)
    {
        file.WriteUInt( m_ParcloseList[i]);
    }

    return true;
}

CBossScoreRange::CBossScoreRange()
    :m_nMinScore(0)
    ,m_nMaxScore(0)
{

}

CDropItemInfo::CDropItemInfo()
    :m_nRate(0)
{

}

CDebrisDropInfo::CDebrisDropInfo()
    :m_nDebrisID(0)
    ,m_nDebrisCount(0)
    ,m_nDropRate(0)
{

}

CMainlineDropConfig::CMainlineDropConfig()
    :m_nParcloseID(0)
    ,m_nMustDropNum(0)
    ,m_nOptionalRate(0)
    ,m_nOptionalNum(0)
{

}

bool CMainlineDropConfig::Load(CFileStream&file)
{
    file.ReadUInt( m_nParcloseID );
    file.ReadUInt( m_nMustDropNum );

    unsigned short nNum = 0;
    file.ReadUShort( nNum );
    for ( unsigned short i = 0; i < nNum; ++i )
    {
        CDropItemInfo info;
        file.ReadUInt(info.m_itemInfo.m_nItemType);
        file.ReadUShort(info.m_itemInfo.m_nItemCount);
        file.ReadInt(info.m_itemInfo.m_nValidTime);
        file.ReadUInt(info.m_nRate);

        m_MustMaleDrop.push_back( info );
    }

    file.ReadUShort( nNum );
    for ( unsigned short i = 0; i < nNum; ++i )
    {
        CDropItemInfo info;
        file.ReadUInt(info.m_itemInfo.m_nItemType);
        file.ReadUShort(info.m_itemInfo.m_nItemCount);
        file.ReadInt(info.m_itemInfo.m_nValidTime);
        file.ReadUInt(info.m_nRate);

        m_MustFemaleDrop.push_back( info );
    }

    file.ReadUShort( nNum );
    for ( unsigned short i = 0; i < nNum; ++i )
    {
        CDebrisDropInfo info;
        file.ReadUInt( info.m_nDebrisID );
        file.ReadUInt( info.m_nDebrisCount );
        file.ReadUInt( info.m_nDropRate );

        m_MustDebrisDrop.push_back( info );
    }

    file.ReadUInt( m_nOptionalRate );
    file.ReadUInt( m_nOptionalNum );

    file.ReadUShort( nNum );
    for ( unsigned short i = 0; i < nNum; ++i )
    {
        CDropItemInfo info;
        file.ReadUInt(info.m_itemInfo.m_nItemType);
        file.ReadUShort(info.m_itemInfo.m_nItemCount);
        file.ReadInt(info.m_itemInfo.m_nValidTime);
        file.ReadUInt(info.m_nRate);

        m_OptionalMaleDrop.push_back( info );
    }

    file.ReadUShort( nNum );
    for ( unsigned short i = 0; i < nNum; ++i )
    {
        CDropItemInfo info;
        file.ReadUInt(info.m_itemInfo.m_nItemType);
        file.ReadUShort(info.m_itemInfo.m_nItemCount);
        file.ReadInt(info.m_itemInfo.m_nValidTime);
        file.ReadUInt(info.m_nRate);

        m_OptionalFemaleDrop.push_back( info );
    }

    file.ReadUShort( nNum );
    for ( unsigned short i = 0; i < nNum; ++i )
    {
        CDebrisDropInfo info;
        file.ReadUInt( info.m_nDebrisID );
        file.ReadUInt( info.m_nDebrisCount );
        file.ReadUInt( info.m_nDropRate );

        m_OptionalDebrisDrop.push_back( info );
    }

    return true;
}

bool CMainlineDropConfig::Save(CFileStream&file)
{
    file.WriteUInt( m_nParcloseID );
    file.WriteUInt( m_nMustDropNum );

    file.WriteUShort( (unsigned short)m_MustMaleDrop.size() );
    std::vector<CDropItemInfo>::iterator it = m_MustMaleDrop.begin();
    for ( ; it != m_MustMaleDrop.end(); ++it )
    {
        file.WriteUInt( it->m_itemInfo.m_nItemType);
        file.WriteUShort( it->m_itemInfo.m_nItemCount);
        file.WriteInt( it->m_itemInfo.m_nValidTime);
        file.WriteUInt( it->m_nRate );
    }

    file.WriteUShort( (unsigned short)m_MustFemaleDrop.size() );
    std::vector<CDropItemInfo>::iterator it1 = m_MustFemaleDrop.begin();
    for ( ; it1 != m_MustFemaleDrop.end(); ++it1 )
    {
        file.WriteUInt( it1->m_itemInfo.m_nItemType);
        file.WriteUShort( it1->m_itemInfo.m_nItemCount);
        file.WriteInt( it1->m_itemInfo.m_nValidTime);
        file.WriteUInt( it1->m_nRate );
    }

    file.WriteUShort( (unsigned short)m_MustDebrisDrop.size() );
    std::vector<CDebrisDropInfo>::iterator itDebris = m_MustDebrisDrop.begin();
    for ( ; itDebris != m_MustDebrisDrop.end(); ++itDebris )
    {
        file.WriteUInt( itDebris->m_nDebrisID);
        file.WriteUInt( itDebris->m_nDebrisCount );
        file.WriteUInt( itDebris->m_nDropRate );
    }

    file.WriteUInt( m_nOptionalRate );
    file.WriteUInt( m_nOptionalNum );

    file.WriteUShort( (unsigned short)m_OptionalMaleDrop.size() );
    std::vector<CDropItemInfo>::iterator it2 = m_OptionalMaleDrop.begin();
    for ( ; it2 != m_OptionalMaleDrop.end(); ++it2 )
    {
        file.WriteUInt( it2->m_itemInfo.m_nItemType);
        file.WriteUShort( it2->m_itemInfo.m_nItemCount);
        file.WriteInt( it2->m_itemInfo.m_nValidTime);
        file.WriteUInt( it2->m_nRate );
    }

    file.WriteUShort( (unsigned short)m_OptionalFemaleDrop.size() );
    std::vector<CDropItemInfo>::iterator it3 = m_OptionalFemaleDrop.begin();
    for ( ; it3 != m_OptionalFemaleDrop.end(); ++it3 )
    {
        file.WriteUInt( it3->m_itemInfo.m_nItemType);
        file.WriteUShort( it3->m_itemInfo.m_nItemCount);
        file.WriteInt( it3->m_itemInfo.m_nValidTime);
        file.WriteUInt( it3->m_nRate );
    }

    file.WriteUShort( (unsigned short)m_OptionalDebrisDrop.size() );
    std::vector<CDebrisDropInfo>::iterator itDebris1 = m_OptionalDebrisDrop.begin();
    for ( ; itDebris1 != m_OptionalDebrisDrop.end(); ++itDebris1 )
    {
        file.WriteUInt( itDebris1->m_nDebrisID);
        file.WriteUInt( itDebris1->m_nDebrisCount );
        file.WriteUInt( itDebris1->m_nDropRate );
    }

    return true;
}

CThemeDropConfig::CThemeDropConfig()
    :m_nThemeID(0)
{

}

bool CThemeDropConfig::Load(CFileStream&file)
{
    file.ReadUInt( m_nThemeID );
    CMainlineDropConfig::Load( file );

    return true;
}

bool CThemeDropConfig::Save(CFileStream&file)
{
    file.WriteUInt( m_nThemeID );
    CMainlineDropConfig::Save( file );

    return true;
}

CRubbishDropConfig::CRubbishDropConfig()
    :m_nRubbishDropID(0)
{

}

bool CRubbishDropConfig::Load(CFileStream&file)
{
    file.ReadUInt( m_nRubbishDropID );

    unsigned short nNum = 0;
    file.ReadUShort( nNum );
    for ( unsigned short i = 0; i < nNum; ++i )
    {
        CDropItemInfo info;
        file.ReadUInt(info.m_itemInfo.m_nItemType);
        file.ReadUShort(info.m_itemInfo.m_nItemCount);
        file.ReadInt(info.m_itemInfo.m_nValidTime);
        file.ReadUInt(info.m_nRate);

        m_MaleDrop.push_back( info );
    }

    file.ReadUShort( nNum );
    for ( unsigned short i = 0; i < nNum; ++i )
    {
        CDropItemInfo info;
        file.ReadUInt(info.m_itemInfo.m_nItemType);
        file.ReadUShort(info.m_itemInfo.m_nItemCount);
        file.ReadInt(info.m_itemInfo.m_nValidTime);
        file.ReadUInt(info.m_nRate);

        m_FemaleDrop.push_back( info );
    }

    return true;
}

bool CRubbishDropConfig::Save(CFileStream&file)
{
    file.WriteUInt( m_nRubbishDropID );

    file.WriteUShort( (unsigned short)m_MaleDrop.size() );
    std::vector<CDropItemInfo>::iterator it = m_MaleDrop.begin();
    for ( ; it != m_MaleDrop.end(); ++it )
    {
        file.WriteUInt( it->m_itemInfo.m_nItemType);
        file.WriteUShort( it->m_itemInfo.m_nItemCount);
        file.WriteInt( it->m_itemInfo.m_nValidTime);
        file.WriteUInt( it->m_nRate );
    }

    file.WriteUShort( (unsigned short)m_FemaleDrop.size() );
    std::vector<CDropItemInfo>::iterator it1 = m_FemaleDrop.begin();
    for ( ; it1 != m_FemaleDrop.end(); ++it1 )
    {
        file.WriteUInt( it1->m_itemInfo.m_nItemType);
        file.WriteUShort( it1->m_itemInfo.m_nItemCount);
        file.WriteInt( it1->m_itemInfo.m_nValidTime);
        file.WriteUInt( it1->m_nRate );
    }

    return true;
}

CDebrisConfig::CDebrisConfig()
{

}

bool CDebrisConfig::Load(CFileStream&file)
{
    file.ReadUInt( m_nDebrisID );

    short nCount = 0;
    file.ReadShort(nCount) ;
    file.ReadString(m_strName, nCount);

    file.ReadShort(nCount) ;
    file.ReadString(m_strIconName, nCount);

    file.ReadShort(nCount) ;
    file.ReadString(m_strAtlasName, nCount);

    file.ReadUInt( m_nNeedNum );
    file.ReadUInt( m_nCurrencyType );
    file.ReadUInt( m_nCurrencyCount );

    unsigned short nMaleItemCount = 0;
    file.ReadUShort( nMaleItemCount );
    for ( unsigned short i = 0; i < nMaleItemCount; ++i )
    {
        CItem item;
        file.ReadUInt( item.m_nItemType );
        file.ReadUShort( item.m_nItemCount );
        file.ReadInt( item.m_nValidTime );

        m_MaleExchangeItem.push_back( item );
    }

    unsigned short nFemaleItemCount = 0;
    file.ReadUShort( nFemaleItemCount );
    for ( unsigned short i = 0; i < nFemaleItemCount; ++i )
    {
        CItem item;
        file.ReadUInt( item.m_nItemType );
        file.ReadUShort( item.m_nItemCount );
        file.ReadInt( item.m_nValidTime );

        m_FemaleExchangeItem.push_back( item );
    }

    return true;
}

bool CDebrisConfig::Save(CFileStream&file)
{
    file.WriteUInt( m_nDebrisID );

    file.WriteUShort((unsigned short)m_strName.size());
    file.WriteString(m_strName);

    file.WriteUShort((unsigned short)m_strIconName.size());
    file.WriteString(m_strIconName);

    file.WriteUShort((unsigned short)m_strAtlasName.size());
    file.WriteString(m_strAtlasName);

    file.WriteUInt( m_nNeedNum );
    file.WriteUInt( m_nCurrencyType );
    file.WriteUInt( m_nCurrencyCount );

    file.WriteUShort( (unsigned short)m_MaleExchangeItem.size() );
    std::vector<CItem>::iterator it = m_MaleExchangeItem.begin();
    for ( ; it != m_MaleExchangeItem.end(); ++it )
    {
        file.WriteUInt( it->m_nItemType );
        file.WriteUShort( it->m_nItemCount );
        file.WriteInt( it->m_nValidTime );
    }

    file.WriteUShort( (unsigned short)m_FemaleExchangeItem.size() );
    std::vector<CItem>::iterator it1 = m_FemaleExchangeItem.begin();
    for ( ; it1 != m_FemaleExchangeItem.end(); ++it1 )
    {
        file.WriteUInt( it1->m_nItemType );
        file.WriteUShort( it1->m_nItemCount );
        file.WriteInt( it1->m_nValidTime );
    }

    return true;
}

CThemeConfig::CThemeConfig()
    :m_nThemeID(0)
    ,m_nOpenTime(0)
    ,m_nEndTime(0)
    ,m_nStoryID(0)
{

}

CThemeConfig::~CThemeConfig()
{

}

bool CThemeConfig::Load(CFileStream & file)
{
    file.ReadUInt( m_nThemeID );

    short nCount = 0;
    file.ReadShort(nCount) ;
    file.ReadString(m_strName, nCount);

    file.ReadUInt( m_nOpenTime);
    file.ReadUInt( m_nEndTime );

    file.ReadShort(nCount) ;
    file.ReadString(m_strThemeIcon, nCount);

    file.ReadShort(nCount) ;
    file.ReadString(m_strThemeAtlas, nCount);

    file.ReadShort(nCount) ;
    file.ReadString(m_strSceneIcon, nCount);

    file.ReadShort(nCount) ;
    file.ReadString(m_strSceneAtlas, nCount);

    file.ReadUInt(m_nStoryID);

    return true;
}

bool CThemeConfig::Save(CFileStream & file)
{
    file.WriteUInt( m_nThemeID );
    file.WriteUShort((unsigned short)m_strName.size());
    file.WriteString(m_strName);

    file.WriteUInt( m_nOpenTime );
    file.WriteUInt( m_nEndTime );

    file.WriteUShort((unsigned short)m_strThemeIcon.size());
    file.WriteString(m_strThemeIcon);

    file.WriteUShort((unsigned short)m_strThemeAtlas.size());
    file.WriteString(m_strThemeAtlas);

    file.WriteUShort((unsigned short)m_strSceneIcon.size());
    file.WriteString(m_strSceneIcon);

    file.WriteUShort((unsigned short)m_strSceneAtlas.size());
    file.WriteString(m_strSceneAtlas);

    file.WriteUInt( m_nStoryID );

    return true;
}

CThemeParcloseConfig::CThemeParcloseConfig()
:m_nThemeID(0)
{

}

CThemeParcloseConfig::~CThemeParcloseConfig()
{

}

bool CThemeParcloseConfig::Load(CFileStream & file)
{
    file.ReadUInt(m_nThemeID);
    CParcloseConfig::Load(file);

    return true;
}

bool CThemeParcloseConfig::Save(CFileStream & file)
{
    file.WriteUInt(m_nThemeID);
    CParcloseConfig::Save(file);

    return true;
}

CParcloseConfig::CParcloseConfig()
    :m_nParcloseID(0)
    ,m_strParcloseName("")
    ,m_nBeginStoryID(0)
    ,m_nEndStoryID(0)
    ,m_nCostCrystal(0)
    ,m_nSceneID(0)
    ,m_nMusicID(0)
    ,m_strMusicName("")
    ,m_nMusicMode(0)
    ,m_nMusicLevel(0)
    ,m_fPosX(0)
    ,m_fPosY(0)
    ,m_nDailyQuickFinishNum(0)
{
}

bool CParcloseConfig::Load(CFileStream&file)
{
    file.ReadUInt( m_nParcloseID );

    short nCount = 0;
    file.ReadShort(nCount) ;
    file.ReadString(m_strParcloseName, nCount);
    file.ReadUInt(m_nBeginStoryID);
    file.ReadUInt(m_nEndStoryID);
    file.ReadUInt(m_nCostCrystal);
    file.ReadUInt(m_nSceneID);
    file.ReadUInt(m_nMusicID);
    file.ReadShort(nCount) ;
    file.ReadString(m_strMusicName, nCount);
    file.ReadUInt(m_nMusicMode);
    file.ReadUInt(m_nMusicLevel);
    file.ReadFloat(m_fPosX);
    file.ReadFloat(m_fPosY);

    unsigned short nTargetCount = 0;
    file.ReadUShort( nTargetCount );
    for ( unsigned int i  = 0 ;i <  nTargetCount; ++i )
    {
        unsigned int nTargetID = 0;
        file.ReadUInt( nTargetID );
        m_Targets.push_back( nTargetID );
    }

    unsigned short nBossCount = 0;
    file.ReadUShort( nBossCount );
    for ( unsigned int i  = 0 ;i <  nBossCount; ++i )
    {
        unsigned int nBossID = 0;
        file.ReadUInt( nBossID );
        m_BossIDList.push_back( nBossID );
    }

    unsigned short nBossScoreCount = 0;
    file.ReadUShort( nBossScoreCount );
    for ( unsigned int i  = 0 ;i <  nBossScoreCount; ++i )
    {
        CBossScoreRange range;
        file.ReadUInt( range.m_nMinScore );
        file.ReadUInt( range.m_nMaxScore );
        m_BossScoreList.push_back( range );
    }

    file.ReadUInt(m_nDailyQuickFinishNum);
    return true;
}

bool CParcloseConfig::IsBossParclose()
{
    return !m_BossIDList.empty();
}

bool CParcloseConfig::Save(CFileStream&file)
{
    file.WriteUInt( m_nParcloseID );
    file.WriteUShort((unsigned short)m_strParcloseName.size());
    file.WriteString(m_strParcloseName);

    file.WriteUInt( m_nBeginStoryID );
    file.WriteUInt( m_nEndStoryID );
    file.WriteUInt( m_nCostCrystal );
    file.WriteUInt( m_nSceneID );
    file.WriteUInt( m_nMusicID );

    file.WriteUShort((unsigned short)m_strMusicName.size());
    file.WriteString(m_strMusicName);

    file.WriteUInt( m_nMusicMode );
    file.WriteUInt( m_nMusicLevel );
    file.WriteFloat(m_fPosX);
    file.WriteFloat(m_fPosY);

    file.WriteUShort( (unsigned short)m_Targets.size() );
    for ( size_t i = 0; i < m_Targets.size(); ++i)
    {
        file.WriteUInt( m_Targets[i]);
    }

    file.WriteUShort( (unsigned short)m_BossIDList.size() );
    for ( size_t i = 0; i < m_BossIDList.size(); ++i)
    {
        file.WriteUInt( m_BossIDList[i]);
    }

    file.WriteUShort( (unsigned short)m_BossScoreList.size() );
    for ( size_t i = 0; i < m_BossScoreList.size(); ++i)
    {
        file.WriteUInt( m_BossScoreList[i].m_nMinScore );
        file.WriteUInt( m_BossScoreList[i].m_nMaxScore );
    }

    file.WriteUInt(m_nDailyQuickFinishNum);
    return true;
}

bool CStoryConfig::Load(CFileStream&file)
{
    file.ReadUInt( m_nStoryID );

    unsigned short nNPCListCount = 0;
    file.ReadUShort( nNPCListCount );
    for ( unsigned int i  = 0 ;i <  nNPCListCount; ++i )
    {
        unsigned int nNPCID = 0;
        file.ReadUInt( nNPCID );
        m_NPCList.push_back( nNPCID );
    }

    unsigned short nContentCount = 0;
    file.ReadUShort( nContentCount );
    for ( unsigned int i  = 0 ;i <  nContentCount; ++i )
    {
        CStoryContent content;
        file.ReadUInt( content.m_nNPCID );

        short nCount = 0;
        file.ReadShort(nCount) ;
        file.ReadString(content.m_strContent, nCount);

        m_ContentList.push_back( content );
    }

    return true;
}

bool CStoryConfig::Save(CFileStream&file)
{

    file.WriteUInt( m_nStoryID );

    file.WriteUShort( (unsigned short)m_NPCList.size() );
    for ( size_t i = 0; i < m_NPCList.size(); ++i)
    {
        file.WriteUInt( m_NPCList[i]);
    }

    file.WriteUShort( (unsigned short)m_ContentList.size() );
    for ( size_t i = 0; i < m_ContentList.size(); ++i)
    {
        file.WriteUInt( m_ContentList[i].m_nNPCID);

        file.WriteUShort((unsigned short)m_ContentList[i].m_strContent.size());
        file.WriteString(m_ContentList[i].m_strContent);
    }

    return true;
}

bool CTargetConfig::Load(CFileStream&file)
{
    file.ReadUInt( m_nTargetID );

    short nCount = 0;
    file.ReadShort(nCount) ;
    file.ReadString(m_strDesc, nCount);

    file.ReadUInt( m_nFlag );

    nCount = 0;
    file.ReadShort(nCount) ;
    file.ReadString(m_strValue, nCount);

    unsigned short nStarValueCount = 0;
    file.ReadUShort(nStarValueCount);
    for (unsigned short i = 0 ; i < nStarValueCount; ++i)
    {
        unsigned short nStarCount = 0;
        unsigned int nStarValue = 0;

        file.ReadUShort(nStarCount);
        file.ReadUInt(nStarValue);

        m_mapStarValue[nStarCount] = nStarValue;
    }

    return true;
}

bool CTargetConfig::Save(CFileStream&file)
{
    file.WriteUInt( m_nTargetID );

    file.WriteUShort((unsigned short)m_strDesc.size());
    file.WriteString(m_strDesc);

    file.WriteUInt( m_nFlag );

    file.WriteUShort((unsigned short)m_strValue.size());
    file.WriteString(m_strValue);

    file.WriteUShort((unsigned short)m_mapStarValue.size());
    std::map<unsigned short,unsigned int>::iterator it = m_mapStarValue.begin();
    for (; it != m_mapStarValue.end(); ++it)
    {
        file.WriteUShort(it->first);
        file.WriteUInt(it->second);
    }

    return true;
}

unsigned int CTargetConfig::GetStarCount(unsigned int nRelatedValue)
{
    unsigned int nStarCount = 0;
    std::map<unsigned short,unsigned int>::iterator it = m_mapStarValue.begin();
    for (; it != m_mapStarValue.end(); ++it)
    {
        if ( it->second > nRelatedValue )
        {
            break;
        }

        nStarCount = it->first;
    }

    return nStarCount;
}

CStoryContent::CStoryContent()
    :m_nNPCID(0)
    ,m_strContent("")
{

}

