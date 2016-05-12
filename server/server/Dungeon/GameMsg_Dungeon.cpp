#include "GameMsg_Dungeon.h"
#include "../share/ServerMsgDef.h"
#include "DungeonDataMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../Pet/PetComponent.h"
#include "../Pet/PetService.h"
#include "../Medal/EntityMedalComponent.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"

//////////////////////////////////////////////////////////////////////////
DungeonPlayerInfo::DungeonPlayerInfo()
{
    m_nRoleID = 0;
    m_strRoleName = "";
    m_bIsHost = false;
    m_chRoleType = RoleRoomType_None;
    m_nIsBoss  = false;
    m_chRolePos = 0;
    m_chRoleState = RoleRoomState_Wait;
    m_eColorTeam = EDanceColorTeam_None;
    m_nRoleSex = ESexType_No;
    m_nRoleSkin = 1;
    m_nGroupTitle = -1;
    m_strDanceGroupName = "";
    m_nDanceGroupBadge = 0;
    m_nDanceGroupEffect = 0;
    m_bIsVIP = false;
    m_nVIPLevel = 0;
    m_nMoveType = 0;
    m_nTransFormID = 0;
    m_bEncodeMedal = false;

    m_pItemCOM = NULL;
    m_pGeneCOM = NULL;
    m_pPetCOM = NULL;
    m_pMedalCOM = NULL;
}

DungeonPlayerInfo::~DungeonPlayerInfo()
{
}

void DungeonPlayerInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt( m_nRoleID );
    IOBuff.AddStringW( m_strRoleName.c_str() );
    IOBuff.AddBool( m_bIsHost );
    IOBuff.AddUChar( m_chRoleType );
    IOBuff.AddBool( m_nIsBoss );
    IOBuff.AddUChar( m_chRolePos );
    IOBuff.AddUChar( m_chRoleState );
    IOBuff.AddUChar((unsigned char)m_eColorTeam);
    IOBuff.AddUChar( m_nRoleSex );
    IOBuff.AddUChar( m_nRoleSkin );
    IOBuff.AddChar( m_nGroupTitle );
    IOBuff.AddStringW( m_strDanceGroupName.c_str() );
    IOBuff.AddUShort( m_nDanceGroupBadge );
    IOBuff.AddUShort( m_nDanceGroupEffect );
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddUChar(m_nMoveType);
    IOBuff.AddUShort(m_nTransFormID);

    if( m_pItemCOM != NULL )
    {
        m_pItemCOM->PackItemForOther( IOBuff );
    }
    else
    {
        int nOldPos1 = IOBuff.GetCurPosition();
        IOBuff.AddUShort(0);

        int nOldPos2 = IOBuff.GetCurPosition();
        IOBuff.AddUShort( 0 );

        std::map<EItemClothType, itemtype_t>::iterator it = m_Equips.begin();
        short nCount = 0;
        for ( ; it != m_Equips.end(); ++it )
        {
            if ( it->second > 0 )
            {
                IOBuff.AddUInt( it->second );
                IOBuff.AddUShort( (unsigned short)EItemColumn_ClothOnPlayer );
                IOBuff.AddUShort( (unsigned short)it->first );

                char szItemBuff[256] = {0};
                memset(szItemBuff, 0, sizeof(szItemBuff));
                CParamPool itemBuff(szItemBuff, sizeof(szItemBuff));
                itemBuff.AddUShort(0);
                itemBuff.AddUShort( 1 );
                itemBuff.AddInt( -1 );
                itemBuff.AddUChar(1);
                itemBuff.AddUInt(0);

                unsigned int nItemSize = itemBuff.GetValidDataSize();
                ASSERT(nItemSize == itemBuff.GetCurPosition());

                IOBuff.AddUChar((unsigned char)nItemSize);
                IOBuff.AddMemory(itemBuff.GetData(), (unsigned short)nItemSize);

                nCount++;
            }
        }
        int nFinalPos = IOBuff.GetCurPosition();
        IOBuff.SetCurPosition( nOldPos2 );
        IOBuff.ReplaceShort( nCount );
        IOBuff.SetCurPosition( nFinalPos );
        IOBuff.AddUShort(0);

        // ÆßÉ«²ÊºçÐÅÏ¢(AI Boss)
        IOBuff.AddUShort(0);

        if ( IOBuff.GetCurPosition() - nOldPos1 - sizeof(unsigned short) != 0 )
        {
            nFinalPos = IOBuff.GetCurPosition();
            IOBuff.SetCurPosition( nOldPos1 );
            IOBuff.ReplaceShort( (short)(nFinalPos - nOldPos1 - sizeof( unsigned short)) );
            IOBuff.SetCurPosition( nFinalPos );
        }
    }

    if ( m_pGeneCOM != NULL )
    {
        m_pGeneCOM->PackGeneForOther( IOBuff );
    }
    else
    {
        IOBuff.AddUShort( 0 );
    }

    if (m_pPetCOM != NULL)
    {
        Pet *pet = m_pPetCOM->GetCarriedPet();
        IOBuff.AddBool(pet != NULL ? true : false);
        if (pet != NULL)
        {
            PlayerPet petInfo;
            PetService::Instance().EncodePetBrief(*pet, petInfo);
            petInfo.doEncode(IOBuff);
        }
    }
    else
    {
        IOBuff.AddBool(false);
    }

    if ( m_bEncodeMedal && m_pMedalCOM != NULL )
    {
        m_pMedalCOM->SyncAllDisplayMedalsForOther( IOBuff );
    }
    else if ( m_bEncodeMedal )
    {
        IOBuff.AddUShort( 0 );
    }
}

////////////////////////////////////////////////////////
GameMsg_C2S_GetLanternBaseInfo::GameMsg_C2S_GetLanternBaseInfo()
    :GameMsg_Base(MSG_C2S_GetLanternBaseInfo)
    ,m_nOsType(0)
{

};

GameMsg_C2S_GetLanternBaseInfo::~GameMsg_C2S_GetLanternBaseInfo()
{

};

bool GameMsg_C2S_GetLanternBaseInfo::doDecode( CParamPool &IOBuff )
{
    m_nOsType  = (int)IOBuff.GetUChar();
    return true;
}


////////////////////////////////////////////////////////
GameMsg_S2C_GetLanternBaseInfoResult::GameMsg_S2C_GetLanternBaseInfoResult()
    :GameMsg_Base(MSG_S2C_GetLanternBaseInfoResult)
    ,m_pRoleDungeon( NULL )
{

};

GameMsg_S2C_GetLanternBaseInfoResult::~GameMsg_S2C_GetLanternBaseInfoResult()
{

};

bool GameMsg_S2C_GetLanternBaseInfoResult::doEncode( CParamPool &IOBuff )
{
    if ( m_pRoleDungeon != NULL )
    {
        m_pRoleDungeon->_GetLanternBaseInfo( IOBuff );

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////
GameMsg_C2S_GetDungeonStory::GameMsg_C2S_GetDungeonStory()
    :GameMsg_Base(MSG_C2S_GetDungeonStory)
    ,m_bIsMainline(false)
    ,m_nStoryType(0)
    ,m_nDungeonID(0)
    ,m_nParcloseID(0)
{

};

GameMsg_C2S_GetDungeonStory::~GameMsg_C2S_GetDungeonStory()
{

};

bool GameMsg_C2S_GetDungeonStory::doDecode( CParamPool &IOBuff )
{
    m_nStoryType  = (unsigned int)IOBuff.GetUChar();
    m_bIsMainline = IOBuff.GetBool();
    m_nDungeonID  = (unsigned int)IOBuff.GetUChar();
    m_nParcloseID = (unsigned int)IOBuff.GetUChar();
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_GetDungeonStoryResult::GameMsg_S2C_GetDungeonStoryResult()
    :GameMsg_Base(MSG_S2C_GetDungeonStoryResult)
    ,m_nStoryType(0)
    ,m_nDungeonID(0)
    ,m_nParcloseID(0)
{

};

GameMsg_S2C_GetDungeonStoryResult::~GameMsg_S2C_GetDungeonStoryResult()
{

};

bool GameMsg_S2C_GetDungeonStoryResult::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddChar( (char)m_nStoryType );
    IOBuff.AddChar( (char)m_nDungeonID );
    IOBuff.AddChar( (char)m_nParcloseID );
    IOBuff.AddUShort( (unsigned short)m_listNpc.size() );
    for ( size_t i = 0; i < m_listNpc.size(); ++i )
    {
        IOBuff.AddChar( (char)m_listNpc[i] );
    }

    IOBuff.AddUShort( (unsigned short)m_listStory.size() );
    for ( size_t i = 0; i < m_listStory.size(); ++i )
    {
        IOBuff.AddChar( (char)m_listStory[i].m_nNPCID );
        IOBuff.AddStringW( m_listStory[i].m_strContent );
    }

    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_GetDungeonStoryFail::GameMsg_S2C_GetDungeonStoryFail()
    :GameMsg_Base( MSG_S2C_GetDungeonStoryFail )
    ,m_nStoryType( 0 )
    ,m_nDungeonID( 0 )
    ,m_nParcloseID( 0 )
    ,m_strError("")
{

};

GameMsg_S2C_GetDungeonStoryFail::~GameMsg_S2C_GetDungeonStoryFail()
{

};

bool GameMsg_S2C_GetDungeonStoryFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddChar( (char)m_nStoryType );
    IOBuff.AddChar( (char)m_nDungeonID );
    IOBuff.AddChar( (char)m_nParcloseID );
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_GetDungeonInfoDetail::GameMsg_C2S_GetDungeonInfoDetail()
    :GameMsg_Base(MSG_C2S_GetDungeonInfoDetail)
    ,m_bIsMainline(false)
    ,m_nDungeonID( 0 )
{

};

GameMsg_C2S_GetDungeonInfoDetail::~GameMsg_C2S_GetDungeonInfoDetail()
{

};

bool GameMsg_C2S_GetDungeonInfoDetail::doDecode( CParamPool &IOBuff )
{
    m_bIsMainline = IOBuff.GetBool();
    m_nDungeonID = (unsigned int)IOBuff.GetUChar();
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_GetDungeonInfoDetailResult::GameMsg_S2C_GetDungeonInfoDetailResult()
    :GameMsg_Base(MSG_S2C_GetDungeonInfoDetailResult)
    ,m_nCurrentParcloseID( 0 )
    ,m_nServerActiveParcloseID(0)
    ,m_pDungeonConfig(NULL)
    ,m_pThemeConfig(NULL)
{

};

GameMsg_S2C_GetDungeonInfoDetailResult::~GameMsg_S2C_GetDungeonInfoDetailResult()
{

};

unsigned int GameMsg_S2C_GetDungeonInfoDetailResult::GetThemeRunningIndex( unsigned int nThemeID )
{
    std::map<unsigned int,unsigned int>::iterator it = m_ThemeRunningIndex.find( nThemeID );
    if ( it != m_ThemeRunningIndex.end() )
    {
        return it->second;
    }

    return 0;
}

bool GameMsg_S2C_GetDungeonInfoDetailResult::doEncode( CParamPool &IOBuff )
{
    if ( m_pDungeonConfig != NULL )
    {
        IOBuff.AddChar( (char)m_pDungeonConfig->m_nDungeonID );
        IOBuff.AddStringW( m_pDungeonConfig->m_strDungeonName );
        IOBuff.AddStringW( m_pDungeonConfig->m_strIconName );
        IOBuff.AddStringW( m_pDungeonConfig->m_strIconAtlas );
        IOBuff.AddChar( (char)m_nCurrentParcloseID );
        IOBuff.AddUShort( (unsigned short)m_pDungeonConfig->m_ParcloseList.size() );
        std::vector<unsigned int>::iterator it = m_pDungeonConfig->m_ParcloseList.begin();
        for ( ; it != m_pDungeonConfig->m_ParcloseList.end(); ++it )
        {
            CParcloseConfig * pParcloseInfo = ConfigManager::Instance().GetMagicLampConfigMgr().GetMainlineParcloseConfig( *it );
            if ( pParcloseInfo != NULL )
            {
                MapMainlineParcloseRecord::iterator iter = m_DungeonRecord.find( pParcloseInfo->m_nParcloseID );

                IOBuff.AddChar( (char)pParcloseInfo->m_nParcloseID );
                IOBuff.AddStringW( pParcloseInfo->m_strParcloseName );
                IOBuff.AddBool( pParcloseInfo->IsBossParclose() );

                bool bIsActived = iter != m_DungeonRecord.end()? iter->second.bIsActived : false;
                IOBuff.AddBool( bIsActived );
                IOBuff.AddChar( (char)(iter != m_DungeonRecord.end()? iter->second.nStarCount : 0));
                IOBuff.AddFloat( pParcloseInfo->m_fPosX );
                IOBuff.AddFloat( pParcloseInfo->m_fPosY );

                if ( m_nServerActiveParcloseID == m_pDungeonConfig->m_ParcloseList[0] || !bIsActived )
                {
                    IOBuff.AddBool( pParcloseInfo->m_nBeginStoryID ? true : false );
                }
                else
                {
                    IOBuff.AddBool( false );
                }

                unsigned int nCostQuickFinishNum = iter != m_DungeonRecord.end() ? iter->second.nQuickFinishNum : 0;
                IOBuff.AddUInt(nCostQuickFinishNum);
                IOBuff.AddUInt(pParcloseInfo->m_nDailyQuickFinishNum);
            }
            else
            {
                IOBuff.AddChar( 0 );
                IOBuff.AddBool( false );
                IOBuff.AddBool( false );
                IOBuff.AddChar( 0 );
                IOBuff.AddFloat( 0 );
                IOBuff.AddFloat( 0 );
                IOBuff.AddBool( false );
            }
        }
    }

    if ( m_pThemeConfig != NULL )
    {
        IOBuff.AddChar( (char)m_pThemeConfig->m_nThemeID );
        IOBuff.AddStringW( m_pThemeConfig->m_strName );
        IOBuff.AddStringW( m_pThemeConfig->m_strSceneIcon );
        IOBuff.AddStringW( m_pThemeConfig->m_strSceneAtlas );
        IOBuff.AddChar( (char)GetThemeRunningIndex(m_pThemeConfig->m_nThemeID) );

        std::map<unsigned int,CThemeParcloseConfig*> * pThemeParcloseList = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeParcloseList( m_pThemeConfig->m_nThemeID );
        if ( pThemeParcloseList != NULL )
        {
            IOBuff.AddUShort( (unsigned short)pThemeParcloseList->size() );
            std::map<unsigned int,CThemeParcloseConfig*>::iterator it = pThemeParcloseList->begin();
            for ( ; it != pThemeParcloseList->end(); ++it )
            {
                IOBuff.AddChar( (char)it->second->m_nParcloseID );
                IOBuff.AddStringW( it->second->m_strParcloseName );
                IOBuff.AddBool( it->second->IsBossParclose() );

                std::map<unsigned int,ThemeParcloseRecord>::iterator iter = m_ThemeRecord.find( it->second->m_nParcloseID );
                bool bIsActived = iter != m_ThemeRecord.end()? iter->second.bIsActived : false;
                IOBuff.AddBool( bIsActived );
                IOBuff.AddChar( (char)(iter != m_ThemeRecord.end()? iter->second.nStarCount : 0));
                IOBuff.AddFloat( it->second->m_fPosX );
                IOBuff.AddFloat( it->second->m_fPosY );

                if (! bIsActived )
                {
                    IOBuff.AddBool( it->second->m_nBeginStoryID > 0 ?true :false );
                }
                else
                {
                    IOBuff.AddBool( false );
                }

                unsigned int nCostQuickFinishNum = iter != m_ThemeRecord.end() ? iter->second.nQuickFinishNum : 0;
                IOBuff.AddUInt(nCostQuickFinishNum);
                IOBuff.AddUInt(it->second->m_nDailyQuickFinishNum);
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_GetDungeonInfoDetailFail::GameMsg_S2C_GetDungeonInfoDetailFail()
    :GameMsg_Base(MSG_S2C_GetDungeonInfoDetailFail)
    ,m_strError("")
{

};

GameMsg_S2C_GetDungeonInfoDetailFail::~GameMsg_S2C_GetDungeonInfoDetailFail()
{};

bool GameMsg_S2C_GetDungeonInfoDetailFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_GetParcloseInfoDetail::GameMsg_C2S_GetParcloseInfoDetail()
    :GameMsg_Base(MSG_C2S_GetParcloseInfoDetail)
    ,m_bIsMainline(false)
    ,m_nDungeonID(0)
    ,m_nParcloseID( 0 )
{

};

GameMsg_C2S_GetParcloseInfoDetail::~GameMsg_C2S_GetParcloseInfoDetail()
{

};

bool GameMsg_C2S_GetParcloseInfoDetail::doDecode( CParamPool &IOBuff )
{
    m_bIsMainline = IOBuff.GetBool();
    m_nDungeonID = (unsigned int)IOBuff.GetUChar();
    m_nParcloseID = (int)IOBuff.GetUChar();
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_GetParcloseInfoDetailResult::GameMsg_S2C_GetParcloseInfoDetailResult()
    :GameMsg_Base(MSG_S2C_GetParcloseInfoDetailResult)
    ,m_pParcloseConfig(NULL)
    ,m_pThemeParcloseConfig(NULL)
    ,m_nLuckStar(0)
    ,m_nSkipParcloseItem(0)
    ,m_nSkipParcloseItemCount(0)
    ,m_nSex(0)
{

};

GameMsg_S2C_GetParcloseInfoDetailResult::~GameMsg_S2C_GetParcloseInfoDetailResult()
{

};

bool GameMsg_S2C_GetParcloseInfoDetailResult::doEncode( CParamPool &IOBuff )
{	
    if ( m_pParcloseConfig != NULL )
    {
        IOBuff.AddChar( (char)m_pParcloseConfig->m_nParcloseID );
        IOBuff.AddBool( m_pParcloseConfig->IsBossParclose() );
        IOBuff.AddChar( (char)m_nLuckStar );
        IOBuff.AddShort( (short)m_pParcloseConfig->m_nCostCrystal );
        IOBuff.AddChar( (char)m_pParcloseConfig->m_nSceneID );
        IOBuff.AddUShort( (unsigned char)m_pParcloseConfig->m_nMusicID );
        IOBuff.AddStringW( m_pParcloseConfig->m_strMusicName );
        IOBuff.AddChar( (char)m_pParcloseConfig->m_nMusicMode );
        IOBuff.AddChar( (char)m_pParcloseConfig->m_nMusicLevel );

        IOBuff.AddUShort( (unsigned short)m_pParcloseConfig->m_Targets.size() );
        std::vector<unsigned int>::iterator it = m_pParcloseConfig->m_Targets.begin();
        for ( ; it != m_pParcloseConfig->m_Targets.end(); ++it )
        {
            CTargetConfig * pTargetConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetTargetConfig( *it );
            if ( pTargetConfig != NULL )
            {
                char szDescribe[64] = {0};

                int nNeedValue = atoi( pTargetConfig->m_strValue.c_str() );
                if ( pTargetConfig->m_nFlag ==  ETF_GRADE )
                {
                    std::string strValue = "";
                    switch( nNeedValue )
                    {
                    case  RoomDanceGrade_SS:strValue= "SS";break;
                    case  RoomDanceGrade_S:strValue= "S";break;
                    case  RoomDanceGrade_A:strValue= "A";break;
                    case  RoomDanceGrade_B:strValue= "B";break;
                    case  RoomDanceGrade_C:strValue= "C";break;
                    case  RoomDanceGrade_D:strValue= "D";break;
                    }

                    sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), strValue.c_str());
                }
                else
                {
                    if ( nNeedValue > 0 )
                    {
                        char temp[64];
                        sprintf( temp, "%d", nNeedValue );
                        sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), temp );
                    }
                    else
                    {
                        sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), pTargetConfig->m_strValue.c_str());
                    }
                }

                IOBuff.AddUShort((unsigned short)pTargetConfig->m_nFlag);
                IOBuff.AddStringW( szDescribe );
            }
            else
            {
                IOBuff.AddUShort(0);
                IOBuff.AddStringW( "" );
            }
        }

        IOBuff.AddUInt( m_nSkipParcloseItem );
        IOBuff.AddUShort( m_nSkipParcloseItemCount );

        if ( m_nSex == ESexType_Male )
        {
            IOBuff.AddUShort( (unsigned short)m_MaleItems.size());
            std::vector<CItem>::iterator it = m_MaleItems.begin();
            for ( ; it != m_MaleItems.end(); ++it )
            {
                it->doEncode(IOBuff);
            }
        }
        else
        {
            IOBuff.AddUShort( (unsigned short)m_FemaleItems.size());
            std::vector<CItem>::iterator it = m_FemaleItems.begin();
            for ( ; it != m_FemaleItems.end(); ++it )
            {
                it->doEncode(IOBuff);
            }
        }

        IOBuff.AddUShort( (unsigned short)m_DropDebrisInfo.size());
        std::vector<DebrisReward>::iterator itDebris = m_DropDebrisInfo.begin();
        for ( ; itDebris != m_DropDebrisInfo.end(); ++itDebris )
        {
            IOBuff.AddUInt( itDebris->nDebrisID );
            IOBuff.AddUInt( itDebris->nDebrisCount );

            CDebrisConfig * pDebrisConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDebrisConfig(itDebris->nDebrisID);
            if (pDebrisConfig != NULL)
            {
                IOBuff.AddStringW(pDebrisConfig->m_strName);
                IOBuff.AddStringW(pDebrisConfig->m_strIconName);
            }
            else
            {
                IOBuff.AddStringW("");
                IOBuff.AddStringW("");
            }
        }
    }

    if ( m_pThemeParcloseConfig != NULL )
    {
        IOBuff.AddChar( (char)m_pThemeParcloseConfig->m_nParcloseID );
        IOBuff.AddBool( m_pThemeParcloseConfig->IsBossParclose() );
        IOBuff.AddChar( (char)m_nLuckStar );
        IOBuff.AddShort( (short)m_pThemeParcloseConfig->m_nCostCrystal );
        IOBuff.AddChar( (char)m_pThemeParcloseConfig->m_nSceneID );
        IOBuff.AddUShort( (unsigned char)m_pThemeParcloseConfig->m_nMusicID );
        IOBuff.AddStringW( m_pThemeParcloseConfig->m_strMusicName );
        IOBuff.AddChar( (char)m_pThemeParcloseConfig->m_nMusicMode );
        IOBuff.AddChar( (char)m_pThemeParcloseConfig->m_nMusicLevel );

        IOBuff.AddUShort( (unsigned short)m_pThemeParcloseConfig->m_Targets.size() );
        std::vector<unsigned int>::iterator it = m_pThemeParcloseConfig->m_Targets.begin();
        for ( ; it != m_pThemeParcloseConfig->m_Targets.end(); ++it )
        {
            CTargetConfig * pTargetConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetTargetConfig( *it );
            if ( pTargetConfig != NULL )
            {
                char szDescribe[64] = {0};

                int nNeedValue = atoi( pTargetConfig->m_strValue.c_str() );
                if ( pTargetConfig->m_nFlag ==  ETF_GRADE )
                {
                    std::string strValue = "";
                    switch( nNeedValue )
                    {
                    case  RoomDanceGrade_SS:strValue= "SS";break;
                    case  RoomDanceGrade_S:strValue= "S";break;
                    case  RoomDanceGrade_A:strValue= "A";break;
                    case  RoomDanceGrade_B:strValue= "B";break;
                    case  RoomDanceGrade_C:strValue= "C";break;
                    case  RoomDanceGrade_D:strValue= "D";break;
                    }

                    sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), strValue.c_str());
                }
                else
                {
                    if ( nNeedValue > 0 )
                    {
                        char temp[64];
                        sprintf( temp, "%d", nNeedValue );
                        sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), temp );
                    }
                    else
                    {
                        sprintf( szDescribe, pTargetConfig->m_strDesc.c_str(), pTargetConfig->m_strValue.c_str());
                    }
                }

                IOBuff.AddUShort((unsigned short)pTargetConfig->m_nFlag);
                IOBuff.AddStringW( szDescribe );
            }
            else
            {
                IOBuff.AddUShort(0);
                IOBuff.AddStringW( "" );
            }
        }

        IOBuff.AddUInt( m_nSkipParcloseItem );
        IOBuff.AddUShort( m_nSkipParcloseItemCount );

        if ( m_nSex == ESexType_Male )
        {
            IOBuff.AddUShort( (unsigned short)m_MaleItems.size());
            std::vector<CItem>::iterator it = m_MaleItems.begin();
            for ( ; it != m_MaleItems.end(); ++it )
            {
                it->doEncode(IOBuff);
            }
        }
        else
        {
            IOBuff.AddUShort( (unsigned short)m_FemaleItems.size());
            std::vector<CItem>::iterator it = m_FemaleItems.begin();
            for ( ; it != m_FemaleItems.end(); ++it )
            {
                it->doEncode(IOBuff);
            }
        }

        IOBuff.AddUShort( (unsigned short)m_DropDebrisInfo.size());
        std::vector<DebrisReward>::iterator itDebris = m_DropDebrisInfo.begin();
        for ( ; itDebris != m_DropDebrisInfo.end(); ++itDebris )
        {
            IOBuff.AddUInt( itDebris->nDebrisID );
            IOBuff.AddUInt( itDebris->nDebrisCount );

            CDebrisConfig * pDebrisConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDebrisConfig(itDebris->nDebrisID);
            if (pDebrisConfig != NULL)
            {
                IOBuff.AddStringW(pDebrisConfig->m_strName);
                IOBuff.AddStringW(pDebrisConfig->m_strIconName);
            }
            else
            {
                IOBuff.AddStringW("");
                IOBuff.AddStringW("");
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_GetParcloseInfoDetailFail::GameMsg_S2C_GetParcloseInfoDetailFail()
    :GameMsg_Base(MSG_S2C_GetParcloseInfoDetailFail)
    ,m_strError("")
{

};

GameMsg_S2C_GetParcloseInfoDetailFail::~GameMsg_S2C_GetParcloseInfoDetailFail()
{

};

bool GameMsg_S2C_GetParcloseInfoDetailFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_GetStarReward::GameMsg_C2S_GetStarReward()
    :GameMsg_Base(MSG_C2S_GetStarReward)
    ,m_nRewardID( 0 )
{

};

GameMsg_C2S_GetStarReward::~GameMsg_C2S_GetStarReward()
{

};

bool GameMsg_C2S_GetStarReward::doDecode( CParamPool &IOBuff )
{
    m_nRewardID = (int)IOBuff.GetUChar();
    return true;
}

////////////////////////////////////////////////////////

GameMsg_S2C_GetStarRewardSuc::GameMsg_S2C_GetStarRewardSuc()
    :GameMsg_Base(MSG_S2C_GetStarRewardSuc)
    ,m_nRewardID(0)
    ,m_nMoney(0)
{

};

GameMsg_S2C_GetStarRewardSuc::~GameMsg_S2C_GetStarRewardSuc()
{

};

bool GameMsg_S2C_GetStarRewardSuc::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUChar( (unsigned char)m_nRewardID );
    IOBuff.AddUShort( (unsigned short)m_listReward.size() );
    std::list<CItem>::iterator it = m_listReward.begin();
    for ( ; it != m_listReward.end(); ++it )
    {
        it->doEncode( IOBuff );
    }

    IOBuff.AddUInt( m_nMoney );
    return true;
}

////////////////////////////////////////////////////////

GameMsg_S2C_GetStarRewardFail::GameMsg_S2C_GetStarRewardFail()
    :GameMsg_Base(MSG_S2C_GetStarRewardFail)
    ,m_strError("")
{

};

GameMsg_S2C_GetStarRewardFail::~GameMsg_S2C_GetStarRewardFail()
{

};

bool GameMsg_S2C_GetStarRewardFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_ExchangeDustReward::GameMsg_C2S_ExchangeDustReward()
    :GameMsg_Base(MSG_C2S_ExchangeDustReward)
{

};

GameMsg_C2S_ExchangeDustReward::~GameMsg_C2S_ExchangeDustReward()
{

};

bool GameMsg_C2S_ExchangeDustReward::doDecode( CParamPool &IOBuff )
{
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_ExchangeDustRewardSuc::GameMsg_S2C_ExchangeDustRewardSuc()
    :GameMsg_Base(MSG_S2C_ExchangeDustRewardSuc)
    ,m_nDustCount(0)
    ,m_nMoney(0)
{

};

GameMsg_S2C_ExchangeDustRewardSuc::~GameMsg_S2C_ExchangeDustRewardSuc()
{

};

bool GameMsg_S2C_ExchangeDustRewardSuc::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort( (unsigned short)m_nDustCount );
    IOBuff.AddUShort( (unsigned short)m_listReward.size() );
    std::list<CItem>::iterator it = m_listReward.begin();
    for ( ; it != m_listReward.end(); ++it )
    {
        it->doEncode( IOBuff );
    }

    IOBuff.AddUInt( m_nMoney );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_ExchangeDustRewardFail::GameMsg_S2C_ExchangeDustRewardFail()
    :GameMsg_Base(MSG_S2C_ExchangeDustRewardFail)
    ,m_strError("")
{

};

GameMsg_S2C_ExchangeDustRewardFail::~GameMsg_S2C_ExchangeDustRewardFail()
{

};

bool GameMsg_S2C_ExchangeDustRewardFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
// GameMsg_C2S_InviteFriend::GameMsg_C2S_InviteFriend()
// :GameMsg_Base(MSG_C2S_InviteFriend)
// ,m_nFriendID( 0 )
// ,m_nDungeonID( 0 )
// ,m_nParcloseID( 0 )
// {
// 
// };
// 
// GameMsg_C2S_InviteFriend::~GameMsg_C2S_InviteFriend()
// {
// 
// };
// 
// bool GameMsg_C2S_InviteFriend::doDecode( CParamPool &IOBuff )
// {
// 	m_nFriendID   = IOBuff.GetUInt();
// 	m_nDungeonID  = IOBuff.GetUChar();
// 	m_nParcloseID = IOBuff.GetUChar();
// 	return true;
// }
// 
// ////////////////////////////////////////////////////////
// GameMsg_S2C_InviteFriendSuc::GameMsg_S2C_InviteFriendSuc()
// :GameMsg_Base( MSG_S2C_InviteFriendSuc )
// ,m_nDungeonID(0)
// ,m_nParcloseID(0)
// ,m_nInviterID(0)
// ,m_strInviterName("")
// ,m_nIsVip(false)
// ,m_nVipLv(0)
// {
// };
// 
// GameMsg_S2C_InviteFriendSuc::~GameMsg_S2C_InviteFriendSuc()
// {
// 
// };
// 
// bool GameMsg_S2C_InviteFriendSuc::doEncode( CParamPool &IOBuff )
// {
// 	IOBuff.AddUChar( m_nDungeonID );
// 	IOBuff.AddUChar( m_nParcloseID );
// 	IOBuff.AddUInt( m_nInviterID );
// 	IOBuff.AddStringW( m_strInviterName );
// 	IOBuff.AddBool( m_nIsVip );
// 	IOBuff.AddUShort( m_nVipLv );
// 	return true;
// }
// 
// ////////////////////////////////////////////////////////
// GameMsg_S2C_InviteFriendFail::GameMsg_S2C_InviteFriendFail()
// :GameMsg_Base( MSG_S2C_InviteFriendFail )
// ,m_strError("")
// {
// };
// 
// GameMsg_S2C_InviteFriendFail::~GameMsg_S2C_InviteFriendFail()
// {
// 
// };
// 
// bool GameMsg_S2C_InviteFriendFail::doEncode( CParamPool &IOBuff )
// {
// 	IOBuff.AddStringW( m_strError );
// 	return true;
// }

////////////////////////////////////////////////////////
GameMsg_S2C_BeInviteToParclose::GameMsg_S2C_BeInviteToParclose()
    :GameMsg_Base( MSG_S2C_BeInviteByFriend )
    ,m_strParcloseName("")
    ,m_eType( EDungeonType_Mainline )
    ,m_nDungeonID( false )
    ,m_nParcloseID( 0 )
    ,m_nInviterID( 0 )
    ,m_strInviterName( "" )
    ,m_nIsVip( false )
    ,m_nVipLv( 0 )
{
};

GameMsg_S2C_BeInviteToParclose::~GameMsg_S2C_BeInviteToParclose()
{

};

bool GameMsg_S2C_BeInviteToParclose::doEncode( CParamPool &IOBuff )
{
    if (m_eType == EDungeonType_Mainline)
    {
        CDungeonConfig * pDungeonConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDungeonConfig(m_nDungeonID);
        if (pDungeonConfig != NULL)
        {
            IOBuff.AddStringW(pDungeonConfig->m_strDungeonName);
        }
        else
        {
            IOBuff.AddStringW(m_strParcloseName);
        }
        IOBuff.AddBool(true);
    }
    else
    {
        CThemeConfig * pThemeConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetThemeConfig(m_nDungeonID);
        if (pThemeConfig != NULL)
        {
            IOBuff.AddStringW(pThemeConfig->m_strName);
        }
        else
        {
            IOBuff.AddStringW(m_strParcloseName);
        }
        IOBuff.AddBool(false);
    }
    IOBuff.AddChar( (char)m_nDungeonID );
    IOBuff.AddChar( (char)m_nParcloseID );
    IOBuff.AddInt( m_nInviterID );
    IOBuff.AddStringW( m_strInviterName );
    IOBuff.AddBool( m_nIsVip );
    IOBuff.AddUShort( (unsigned short)m_nVipLv );

    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_InviteFriendResponse::GameMsg_C2S_InviteFriendResponse()
    :GameMsg_Base( MSG_C2S_InviteFriendResponse )
    ,m_nInviterID( 0 )
    ,m_bIsAccept( false )
    ,m_bIsMainline( false )
    ,m_nDungeonID( 0 )
    ,m_nParcloseID( 0 )
    ,m_nPhoneOS(0)
{

};

GameMsg_C2S_InviteFriendResponse::~GameMsg_C2S_InviteFriendResponse()
{

};

bool GameMsg_C2S_InviteFriendResponse::doDecode( CParamPool &IOBuff )
{
    m_nInviterID  = IOBuff.GetUInt();
    m_bIsAccept   = IOBuff.GetBool();
    m_bIsMainline = IOBuff.GetBool();
    m_nDungeonID  = IOBuff.GetUChar();
    m_nParcloseID = IOBuff.GetUChar();
    m_nPhoneOS = IOBuff.GetUChar();
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_InviteFriendResponseResult::GameMsg_S2C_InviteFriendResponseResult()
    :GameMsg_Base( MSG_S2C_InviteFriendResponseResult )
    ,m_strError("")
{

};

GameMsg_S2C_InviteFriendResponseResult::~GameMsg_S2C_InviteFriendResponseResult()
{

};

bool GameMsg_S2C_InviteFriendResponseResult::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_GetDungeonList::GameMsg_C2S_GetDungeonList()
    :GameMsg_Base( MSG_C2S_GetDungeonRankList )
    ,m_nPage( 0 )
{
};

GameMsg_C2S_GetDungeonList::~GameMsg_C2S_GetDungeonList()
{

};

bool GameMsg_C2S_GetDungeonList::doDecode( CParamPool &IOBuff )
{
    m_nPage = IOBuff.GetUShort();
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_GetDungeonListResult::GameMsg_S2C_GetDungeonListResult()
    :GameMsg_Base( MSG_S2C_GetDungeonRankListResult )
    ,m_nCurPage( 0 )
    ,m_nTotalCount( 0 )
    ,m_nSelfRank( 0 )
    ,m_nTopDungeonID( 0 )
    ,m_nTopParcloseID( 0 )
    ,m_strTopDungeonName("")
    ,m_strTopParcloseName("")
{
};

GameMsg_S2C_GetDungeonListResult::~GameMsg_S2C_GetDungeonListResult()
{
};

bool GameMsg_S2C_GetDungeonListResult::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort( (unsigned short)m_nCurPage );
    IOBuff.AddUShort( (unsigned short)m_nTotalCount );
    IOBuff.AddUShort( (unsigned short)m_RankList.size() );

    for ( size_t i = 0; i < m_RankList.size(); ++i )
    {
        IOBuff.AddUInt( m_RankList[i].m_nFriendID );
        IOBuff.AddStringW( m_RankList[i].m_strFriendName );
        IOBuff.AddUShort( (unsigned short)m_RankList[i].m_nRank );
        IOBuff.AddUShort( (unsigned short)m_RankList[i].m_nStarCount );
        IOBuff.AddStringW( m_RankList[i].m_strDungeonName );
        IOBuff.AddStringW( m_RankList[i].m_strParcloseName );
    }

    IOBuff.AddUShort( m_nSelfRank );
    IOBuff.AddChar( (char)m_nTopDungeonID );
    IOBuff.AddStringW( m_strTopDungeonName );
    IOBuff.AddChar( (char)m_nTopParcloseID );
    IOBuff.AddStringW( m_strTopParcloseName );
    
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_GetBuyCrystalInfo::GameMsg_C2S_GetBuyCrystalInfo()
    :GameMsg_Base( MSG_C2S_GetBuyCrystalInfo )
{
};

GameMsg_C2S_GetBuyCrystalInfo::~GameMsg_C2S_GetBuyCrystalInfo()
{

};

bool GameMsg_C2S_GetBuyCrystalInfo::doDecode( CParamPool &IOBuff )
{
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_GetBuyCrystalInfoResult::GameMsg_S2C_GetBuyCrystalInfoResult()
    :GameMsg_Base( MSG_S2C_GetBuyCrystalInfoResult )
    , m_nFavorableBuyVitCount(0)
    , m_nHaveBuyVitCount(0)
{
};

GameMsg_S2C_GetBuyCrystalInfoResult::~GameMsg_S2C_GetBuyCrystalInfoResult()
{

};

bool GameMsg_S2C_GetBuyCrystalInfoResult::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort( (unsigned short)m_mapBuyCrystalConfig.size() );
    std::map<int,CBuyCrystalConfig>::iterator itb = m_mapBuyCrystalConfig.begin();
    for ( ; itb != m_mapBuyCrystalConfig.end(); ++itb )
    {
        itb->second.doEncode(IOBuff);
    }
    IOBuff.AddUInt(m_nFavorableBuyVitCount);
    IOBuff.AddUInt(m_nHaveBuyVitCount);
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_BuyCrystal::GameMsg_C2S_BuyCrystal()
    :GameMsg_Base( MSG_C2S_BuyCrystal )
    ,m_nIndex( 0 )
{
};

GameMsg_C2S_BuyCrystal::~GameMsg_C2S_BuyCrystal()
{

};

bool GameMsg_C2S_BuyCrystal::doDecode( CParamPool &IOBuff )
{
    m_nIndex = IOBuff.GetUChar();
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_BuyCrystalSuc::GameMsg_S2C_BuyCrystalSuc()
    :GameMsg_Base( MSG_S2C_BuyCrystalSuc )
    ,m_nCrystalCount( 0 )
    ,m_nTodayVipBuyVitNum(0)
{
};

GameMsg_S2C_BuyCrystalSuc::~GameMsg_S2C_BuyCrystalSuc()
{

};

bool GameMsg_S2C_BuyCrystalSuc::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort( (unsigned short)m_nCrystalCount );
    IOBuff.AddUInt(m_nTodayVipBuyVitNum);
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_BuyCrystalFail::GameMsg_S2C_BuyCrystalFail()
    :GameMsg_Base( MSG_S2C_BuyCrystalFail )
    ,m_strError( "" )
{
};

GameMsg_S2C_BuyCrystalFail::~GameMsg_S2C_BuyCrystalFail()
{

};

bool GameMsg_S2C_BuyCrystalFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_UpdateCrystal::GameMsg_S2C_UpdateCrystal()
    :GameMsg_Base( MSG_S2C_UpdateCrystal )
    ,m_nCrystal(0)
    ,m_nTime(0)
{
};

GameMsg_S2C_UpdateCrystal::~GameMsg_S2C_UpdateCrystal()
{

};

bool GameMsg_S2C_UpdateCrystal::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUShort( (unsigned short)m_nCrystal );
    IOBuff.AddFloat( m_nTime );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_StartParclose::GameMsg_C2S_StartParclose()
    :GameMsg_Base( MSG_C2S_StartParclose )
    ,m_bIsMainline( false )
    ,m_nDungeonID( 0 )
    ,m_nParcloseID( 0 )
    ,m_nFriendID( 0 )
{
};

GameMsg_C2S_StartParclose::~GameMsg_C2S_StartParclose()
{
};

bool GameMsg_C2S_StartParclose::doDecode( CParamPool &IOBuff )
{
    m_bIsMainline = IOBuff.GetBool();
    m_nDungeonID = (unsigned int)IOBuff.GetUChar();
    m_nParcloseID = IOBuff.GetUChar();
    unsigned short nItemCount = IOBuff.GetUShort();
    for ( int i = 0; i < nItemCount; ++i )
    {
        m_listItem.push_back( IOBuff.GetUShort() );
    }
    m_nFriendID = IOBuff.GetUInt();
    return true;
}

////////////////////////////////////////////////////////
GameMsg_C2S_SkipParcose::GameMsg_C2S_SkipParcose()
    :GameMsg_Base( MSG_C2S_SkipParclose )
    ,m_bIsMainline(0)
    ,m_nDungeonID(0)
    ,m_nParcloseID( 0 )
{
};

GameMsg_C2S_SkipParcose::~GameMsg_C2S_SkipParcose()
{
};

bool GameMsg_C2S_SkipParcose::doDecode( CParamPool &IOBuff )
{
    m_bIsMainline = IOBuff.GetBool();
    m_nDungeonID = (unsigned int)IOBuff.GetUChar();
    m_nParcloseID = IOBuff.GetUChar();
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_SkipParcloseSuc::GameMsg_S2C_SkipParcloseSuc()
    :GameMsg_Base( MSG_S2C_SkipParcloseSuc )
    ,m_nCurrentParcloseStar( 0 )
    ,m_nAllStartCount(0)
    ,m_nNextDungeonID( 0 )
    ,m_nNextParcloseID( 0 )
{
};

GameMsg_S2C_SkipParcloseSuc::~GameMsg_S2C_SkipParcloseSuc()
{
};

bool GameMsg_S2C_SkipParcloseSuc::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddChar( (char)m_nCurrentParcloseStar );
    IOBuff.AddUShort( m_nAllStartCount );
    IOBuff.AddChar( (char)m_nNextDungeonID );
    IOBuff.AddChar( (char)m_nNextParcloseID );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_SkipParcloseFail::GameMsg_S2C_SkipParcloseFail()
    :GameMsg_Base( MSG_S2C_SkipParcloseFail )
    ,m_strError( "" )
{
};

GameMsg_S2C_SkipParcloseFail::~GameMsg_S2C_SkipParcloseFail()
{
};

bool GameMsg_S2C_SkipParcloseFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_StartParcloseFail::GameMsg_S2C_StartParcloseFail()
    :GameMsg_Base( MSG_S2C_StartParcloseFail )
    ,m_nErrFlag( 0 )
    ,m_strError("")
{
};

GameMsg_S2C_StartParcloseFail::~GameMsg_S2C_StartParcloseFail()
{
};

bool GameMsg_S2C_StartParcloseFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddChar( (char)m_nErrFlag );
    IOBuff.AddStringW( m_strError );
    return true;
}

////////////////////////////////////////////////////////
GameMsg_S2C_StartParcloseSuc::GameMsg_S2C_StartParcloseSuc()
    :GameMsg_Base( MSG_S2C_StartParcloseSuc )
    ,m_cRoomType( 0 )
    ,m_nSceneID( 0 )
    ,m_nMusicID( 0 )
    ,m_nLevel( 0 )
    ,m_cSelfRoomType( 0 )
    ,m_nIsHost( false )
    ,m_cTeamMode( 0 )
    ,m_strCheckKey( "" )
    ,m_nCrystalNum( 0 )
    ,m_nDustNum( 0 )
{
}

GameMsg_S2C_StartParcloseSuc::~GameMsg_S2C_StartParcloseSuc()
{
}

bool GameMsg_S2C_StartParcloseSuc::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddChar( m_cRoomType );
    IOBuff.AddChar( (char)m_nSceneID );
    IOBuff.AddShort( (short)m_nMusicID );
    IOBuff.AddChar( (char)m_nMode );
    IOBuff.AddChar( (char)m_nLevel );
    IOBuff.AddChar( m_cSelfRoomType );
    IOBuff.AddBool( m_nIsHost );
    IOBuff.AddChar( (char)m_cTeamMode );

    IOBuff.AddUShort( (unsigned short)m_PlayerList.size() );
    DungeonPlayerInfoList::iterator it = m_PlayerList.begin();
    for ( ; it != m_PlayerList.end(); ++it )
    {
        it->doEncode( IOBuff );
    }

    IOBuff.AddStringW( m_strCheckKey );
    IOBuff.AddPerFixSizeMemory( &m_vecStage[0], (unsigned short)m_vecStage.size());
    IOBuff.AddUShort( (unsigned short)m_nCrystalNum );
    IOBuff.AddUShort( (unsigned short)m_nDustNum );
    return true;
}


CParcloseEndTarget::CParcloseEndTarget()
    :m_IsAchieve(false)
{
}
////////////////////////////////////////////////////////
GameMsg_S2C_ParcloseEnd::GameMsg_S2C_ParcloseEnd()
    :GameMsg_Base( MSG_S2C_ParcloseEnd )
    ,m_bSucess( false )
    ,m_nStarCount( 0 )
    ,m_nNextDungeonID( 0 )
    ,m_nLastParcloseID( 0 )
    ,m_nStarAllNum( 0 )
    ,m_bHaveStory(false)
{
};

GameMsg_S2C_ParcloseEnd::~GameMsg_S2C_ParcloseEnd()
{
};

bool GameMsg_S2C_ParcloseEnd::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddBool( m_bSucess );
    IOBuff.AddChar( (char)m_nStarCount );
    IOBuff.AddChar( (char)m_nNextDungeonID );
    IOBuff.AddChar( (char)m_nLastParcloseID );

    IOBuff.AddUShort( (unsigned short)m_listPlayerScore.size() );
    for ( size_t i = 0 ; i < m_listPlayerScore.size(); ++i )
    {
        IOBuff.AddInt( m_listPlayerScore[i].m_nPos );
        IOBuff.AddUInt( m_listPlayerScore[i].m_nScore );
        IOBuff.AddStringW( m_listPlayerScore[i].m_strName.c_str() );
    }

    IOBuff.AddUShort( (unsigned short)m_listNpcScore.size() );
    for ( size_t i = 0 ; i < m_listNpcScore.size(); ++i )
    {
        IOBuff.AddUInt( m_listNpcScore[i] );
    }

    IOBuff.AddUShort( (unsigned short)m_Targets.size() );
    for ( size_t i = 0 ; i < m_Targets.size(); ++i )
    {
        IOBuff.AddBool( m_Targets[i].m_IsAchieve );
        IOBuff.AddStringW( m_Targets[i].m_strTargetDes );
        IOBuff.AddStringW( m_Targets[i].m_strPlayerTarget );
        IOBuff.AddStringW( m_Targets[i].m_strNeedTarget );
    }

    IOBuff.AddUShort( (unsigned short)m_nStarAllNum );

    m_ParcloseReward.doEncode(IOBuff);

    IOBuff.AddBool( m_bHaveStory );

    return true;
}

GameMsg_C2S_ExchangeDebrisEquip::GameMsg_C2S_ExchangeDebrisEquip()
    :GameMsg_Base( MSG_C2S_ExchangeDebrisEquip )
    ,m_nDebrisID(0)
{
}

GameMsg_C2S_ExchangeDebrisEquip::~GameMsg_C2S_ExchangeDebrisEquip()
{
}

bool GameMsg_C2S_ExchangeDebrisEquip::doDecode(CParamPool &IOBuff)
{
    m_nDebrisID = IOBuff.GetUInt();

    return true;
}

GameMsg_S2C_ExchangeDebrisEquipSuc::GameMsg_S2C_ExchangeDebrisEquipSuc()
    :GameMsg_Base( MSG_S2C_ExchangeDebrisEquiSuc )
    ,m_nDebrisID(0)
    ,m_nRemainCount(0)
{
}

GameMsg_S2C_ExchangeDebrisEquipSuc::~GameMsg_S2C_ExchangeDebrisEquipSuc()
{
}

bool GameMsg_S2C_ExchangeDebrisEquipSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt( m_nDebrisID );
    IOBuff.AddUInt( m_nRemainCount );
    IOBuff.AddUShort( (unsigned short)m_listExchangeEquip.size() );
    std::list<CItem>::iterator it = m_listExchangeEquip.begin();
    for ( ; it != m_listExchangeEquip.end(); ++it )
    {
        it->doEncode( IOBuff );
    }

    return true;
}

GameMsg_S2C_ExchangeDebrisEquipFail::GameMsg_S2C_ExchangeDebrisEquipFail()
    :GameMsg_Base( MSG_S2C_ExchangeDebrisEquiFail )
    ,m_cErrorFlag(0)
{
}

GameMsg_S2C_ExchangeDebrisEquipFail::~GameMsg_S2C_ExchangeDebrisEquipFail()
{
}

bool GameMsg_S2C_ExchangeDebrisEquipFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar( m_cErrorFlag );

    return true;
}

////////////////////////////////////////////////////////
// GameMsg_S2G_GetRankList1::GameMsg_S2G_GetRankList1()
// :GameMsg_Base( MSG_G2S_GetRankListResult )
// ,m_nRoleID( 0 )
// {
// };
// 
// GameMsg_S2G_GetRankList1::~GameMsg_S2G_GetRankList1()
// {
// };
// 
// bool GameMsg_S2G_GetRankList1::doEncode( CParamPool &IOBuff )
// {
// 	m_nRoleID = IOBuff.GetUInt();
// 	return true;
// }
// 
// bool GameMsg_S2G_GetRankList1::doDecode(CParamPool &IOBuff)
// {
// 	IOBuff.AddUInt( m_nRoleID );
// 	return true;
// }

GameMsg_C2S_GetMainlineInfo::GameMsg_C2S_GetMainlineInfo()
    :GameMsg_Base( MSG_C2S_GetMainlineInfo )
{
}

GameMsg_C2S_GetMainlineInfo::~GameMsg_C2S_GetMainlineInfo()
{
}

bool GameMsg_C2S_GetMainlineInfo::doDecode(CParamPool &IOBuff)
{

    return true;
}

GameMsg_S2C_GetMainlineInfoResult::GameMsg_S2C_GetMainlineInfoResult()
    :GameMsg_Base( MSG_S2C_GetMainlineInfoResult )
{
}

GameMsg_S2C_GetMainlineInfoResult::~GameMsg_S2C_GetMainlineInfoResult()
{
}

bool GameMsg_S2C_GetMainlineInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort( 0);
    IOBuff.AddUShort( (unsigned short)m_listDungeonInfo.size());
    std::list<MsgDungeonInfo>::iterator it = m_listDungeonInfo.begin();
    for ( ; it != m_listDungeonInfo.end(); ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

GameMsg_C2S_GetThemeInfo::GameMsg_C2S_GetThemeInfo()
    :GameMsg_Base( MSG_C2S_GetThemeInfo )
{
}

GameMsg_C2S_GetThemeInfo::~GameMsg_C2S_GetThemeInfo()
{
}

bool GameMsg_C2S_GetThemeInfo::doDecode(CParamPool &IOBuff)
{

    return true;
}

GameMsg_S2C_GetThemeInfoResult::GameMsg_S2C_GetThemeInfoResult()
    :GameMsg_Base( MSG_S2C_GetThemeInfoResult )
{
}

GameMsg_S2C_GetThemeInfoResult::~GameMsg_S2C_GetThemeInfoResult()
{
}

bool GameMsg_S2C_GetThemeInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort( 0);
    IOBuff.AddUShort( (unsigned short)m_listDungeonInfo.size());
    std::list<MsgDungeonInfo>::iterator it = m_listDungeonInfo.begin();
    for ( ; it != m_listDungeonInfo.end(); ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

GameMsg_S2C_UpdateInviteFriend::GameMsg_S2C_UpdateInviteFriend()
    :GameMsg_Base( MSG_S2C_UpdateInvieFriend )
{
}

GameMsg_S2C_UpdateInviteFriend::~GameMsg_S2C_UpdateInviteFriend()
{
}

bool GameMsg_S2C_UpdateInviteFriend::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort( (unsigned short)m_nRemainInviteFriendCount );
    IOBuff.AddUShort( (unsigned short)m_setInvitedFriends.size());
    std::set<unsigned int>::iterator it = m_setInvitedFriends.begin();
    for ( ; it != m_setInvitedFriends.end(); ++it )
    {
        IOBuff.AddUInt( *it );
    }
    return true;
}

GameMsg_C2S_GetFriendContactInfo::GameMsg_C2S_GetFriendContactInfo()
    :GameMsg_Base( MSG_C2S_GetFriendContactInfo )
{
}

GameMsg_C2S_GetFriendContactInfo::~GameMsg_C2S_GetFriendContactInfo()
{
}

bool GameMsg_C2S_GetFriendContactInfo::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GetFriendContactInfoResult::GameMsg_S2C_GetFriendContactInfoResult()
    :GameMsg_Base( MSG_S2C_GetFriendContactInfoResult )
{
}

GameMsg_S2C_GetFriendContactInfoResult::~GameMsg_S2C_GetFriendContactInfoResult()
{
}

bool GameMsg_S2C_GetFriendContactInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_mapFriendContact.size());
    std::map<unsigned int,FriendContactInfo>::iterator it = m_mapFriendContact.begin();
    for (; it != m_mapFriendContact.end(); ++it)
    {
        it->second.doEncode(IOBuff);
    }
    return true;
}

GameMsg_C2S_GiveVitToFriend::GameMsg_C2S_GiveVitToFriend()
    :GameMsg_Base( MSG_C2S_GiveVitToFriend )
{
}

GameMsg_C2S_GiveVitToFriend::~GameMsg_C2S_GiveVitToFriend()
{
}

bool GameMsg_C2S_GiveVitToFriend::doDecode(CParamPool &IOBuff)
{
    unsigned short nCount = IOBuff.GetUShort();
    for (unsigned short i = 0;  i < nCount; ++i)
    {
        m_listFriendRoleID.push_back(IOBuff.GetUInt());
    }
    return true;
}

GameMsg_S2C_GiveVitToFriendResult::GameMsg_S2C_GiveVitToFriendResult()
    :GameMsg_Base( MSG_S2C_GiveVitToFriendResult )
{
}

GameMsg_S2C_GiveVitToFriendResult::~GameMsg_S2C_GiveVitToFriendResult()
{
}

bool GameMsg_S2C_GiveVitToFriendResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listSucessFriendRoleID.size());
    std::list<unsigned int>::iterator it = m_listSucessFriendRoleID.begin();
    for (; it != m_listSucessFriendRoleID.end(); ++it)
    {
        IOBuff.AddUInt(*it);
    }

    IOBuff.AddUShort((unsigned short)m_listFailFriendRoleID.size());
    std::list<unsigned int>::iterator iter = m_listFailFriendRoleID.begin();
    for (; iter != m_listFailFriendRoleID.end(); ++iter)
    {
        IOBuff.AddUInt(*iter);
    }

    return true;
}

GameMsg_S2C_FriendNoticeByGiveVit::GameMsg_S2C_FriendNoticeByGiveVit()
    :GameMsg_Base( MSG_S2C_GiveVitToFriendNotice )
    , m_nGiveVitRoleID(0)
    , m_strGiveVitRoleName("")
    , m_nGiveVitNum(0)
{
}

GameMsg_S2C_FriendNoticeByGiveVit::~GameMsg_S2C_FriendNoticeByGiveVit()
{
}

bool GameMsg_S2C_FriendNoticeByGiveVit::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGiveVitRoleID);
    IOBuff.AddStringW(m_strGiveVitRoleName);
    IOBuff.AddUInt(m_nGiveVitNum);

    return true;
}

GameMsg_C2S_GetVitFromFriend::GameMsg_C2S_GetVitFromFriend()
    :GameMsg_Base( MSG_C2S_GetVitFromFrined )
{
}

GameMsg_C2S_GetVitFromFriend::~GameMsg_C2S_GetVitFromFriend()
{
}

bool GameMsg_C2S_GetVitFromFriend::doDecode(CParamPool &IOBuff)
{
    unsigned short nCount = IOBuff.GetUShort();
    for (unsigned short i = 0;  i < nCount; ++i)
    {
        m_listFriendRoleID.push_back(IOBuff.GetUInt());
    }
    return true;
}

GameMsg_S2C_GetVitFromFriendResult::GameMsg_S2C_GetVitFromFriendResult()
    :GameMsg_Base( MSG_S2C_GetVitFromFrined )
{
}

GameMsg_S2C_GetVitFromFriendResult::~GameMsg_S2C_GetVitFromFriendResult()
{
}

bool GameMsg_S2C_GetVitFromFriendResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listScuessFriendRoleID.size());
    std::list<unsigned int>::iterator it = m_listScuessFriendRoleID.begin();
    for (; it != m_listScuessFriendRoleID.end(); ++it)
    {
        IOBuff.AddUInt(*it);
    }

    IOBuff.AddUShort((unsigned short)m_mapFailFriend.size());
    std::map<unsigned int,unsigned short>::iterator iter = m_mapFailFriend.begin();
    for (; iter != m_mapFailFriend.end(); ++iter)
    {
        IOBuff.AddUInt(iter->first);
        IOBuff.AddUShort(iter->second);
    }

    return true;
}

GameMsg_C2S_QuickFinishParclose::GameMsg_C2S_QuickFinishParclose()
    :GameMsg_Base( MSG_C2S_QuickFinishParclose )
{
}

GameMsg_C2S_QuickFinishParclose::~GameMsg_C2S_QuickFinishParclose()
{
}

bool GameMsg_C2S_QuickFinishParclose::doDecode(CParamPool &IOBuff)
{
    m_bIsMainline = IOBuff.GetBool();
    m_nDungeonID = IOBuff.GetUInt();
    m_nParcloseID = IOBuff.GetUInt();
    return true;
}

GameMsg_S2C_QuickFinishParcloseSucess::GameMsg_S2C_QuickFinishParcloseSucess()
    :GameMsg_Base( MSG_S2C_QuickFinishParcloseSucess )
{
}

GameMsg_S2C_QuickFinishParcloseSucess::~GameMsg_S2C_QuickFinishParcloseSucess()
{
}

bool GameMsg_S2C_QuickFinishParcloseSucess::doEncode(CParamPool &IOBuff)
{
    m_ParcloseReward.doEncode(IOBuff);
    return true;
}

GameMsg_S2C_QuickFinishParcloseFail::GameMsg_S2C_QuickFinishParcloseFail()
    :GameMsg_Base( MSG_S2C_QuickFinishParcloseFail )
{
}

GameMsg_S2C_QuickFinishParcloseFail::~GameMsg_S2C_QuickFinishParcloseFail()
{
}

bool GameMsg_S2C_QuickFinishParcloseFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nError);
    return true;
}

