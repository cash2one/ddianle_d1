#include "Dungeon.h"
#include "../roomLib/RoomDataStruct.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "GameMsg_Dungeon.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../music/MusicStageFileManager.h"
#include "../room/RoomComponent.h"


CDungeon::CDungeon( unsigned int nRoomID, EDungeonType eType, unsigned int nDungeonID, const CParcloseConfig & parclose )
    :CRoom( eRoomType_Dungeon, nRoomID )
    ,m_ParcloseConfig( parclose )
    ,m_DungeonType(eType)
    ,m_nDungeonID(nDungeonID)
{
    m_szRoomBossID[DUNGEON_HOST_POS] = 0;
    m_szRoomBossID[DUNGEON_FRIEND_POS] = 0;
    Initialize();
}

void CDungeon::Initialize()
{
    CRoom::ChangeChoosedScene( m_ParcloseConfig.m_nSceneID );
    CRoom::ChangeChoosedMusic( m_ParcloseConfig.m_nMusicID, m_ParcloseConfig.m_nMusicMode, m_ParcloseConfig.m_nMusicLevel );
}

void CDungeon::RoomPrepare()
{
    // 玩家信息
    for ( int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        if ( m_DancerInfo.szDancerComponent[i] != NULL )
        {
            m_DancerInfo.szDancerID[i] = m_DancerInfo.szDancerComponent[i]->GetRoleID();
            m_DancerInfo.szDancerName[i] = m_DancerInfo.szDancerComponent[i]->GetRoleAttr()->GetRoleName();

            m_DancerInfo.szDancerComponent[i]->GetRoleRoom()->InRoom( this, EDanceColorTeam_None, RoleRoomType_Dancer, RoleRoomState_ToStart, i, true);
        }
    }

    CRoom::RoomPrepare();

    // 加载boss信息
    if ( m_DancerInfo.szDancerComponent[DUNGEON_HOST_POS] != NULL )
    {
        switch( m_DancerInfo.szDancerComponent[DUNGEON_HOST_POS]->GetPhoneOS())
        {
        case OS_Ios:
            LoadBossInfo( CDungeonMgr::Instance().CalcMusicRoundCount( PlayMusicStage(), OS_Ios));
            break;
        case OS_Android:
            LoadBossInfo( CDungeonMgr::Instance().CalcMusicRoundCount( PlayMusicStage(), OS_Android));
            break;
        case OS_WP8:
            LoadBossInfo( CDungeonMgr::Instance().CalcMusicRoundCount( PlayMusicStage(), OS_WP8));
            break;
        }
    }
}

void CDungeon::LoadBossInfo( unsigned int nStageCount )
{
    if ( nStageCount <= 0 )
    {
        return;
    }

    // 加载BOSSID 信息
    for ( size_t i = 0; i < MAX_DUNGEON_BOSS_COUNT; ++i )
    {
        if ( (i + 1) <= m_ParcloseConfig.m_BossIDList.size() && (i + 1) <= m_ParcloseConfig.m_BossScoreList.size() )
        {
            m_szRoomBossID[i] = m_ParcloseConfig.m_BossIDList[i];

            CBossScoreRange range;
            range.m_nMinScore = m_ParcloseConfig.m_BossScoreList[i].m_nMinScore;
            range.m_nMaxScore = m_ParcloseConfig.m_BossScoreList[i].m_nMaxScore;

            unsigned int nMinEveryScore = range.m_nMinScore/nStageCount;
            unsigned int nMaxEveryScore = range.m_nMaxScore/nStageCount;

            unsigned int nLastMark = 0;
            for ( unsigned j = 1; j <= nStageCount; ++j )
            {
                mapTable temp;
                int nRandomMark = Random( nMinEveryScore, nMaxEveryScore );
                temp[j] = nLastMark + nRandomMark;

                std::map<unsigned int,mapTable>::iterator it = m_BossScore.find( m_szRoomBossID[i] );
                if ( it != m_BossScore.end() )
                {
                    it->second.insert( make_pair( j, temp[j]));
                }
                else
                {
                    m_BossScore[ m_szRoomBossID[i] ] = temp;
                }

                nLastMark = temp[j];
            }

            SetRoomTeamMode(EDanceRoomTeamMode_Team);
        }
    }

}

unsigned int CDungeon::DealWithBossID( unsigned int nBossID )
{
    for ( int j = 0; j < MAX_DUNGEON_PLAYER_COUNT; ++j )
    {
        if ( m_DancerInfo.szDancerID[j] == nBossID  )
        {
            unsigned int nMaxBossID = ConfigManager::Instance().GetMagicLampConfigMgr().GetMaxBossID();
            nBossID = m_DancerInfo.szDancerID[j] + nMaxBossID + 1;

            if ( nBossID == m_DancerInfo.szDancerID[1-j])
            {
                nBossID += 1;
            }

            return nBossID;
        }
    }

    return nBossID;
}

void CDungeon::SyncRoomEndScore( std::vector<CRoomEndPlayerScoreInfo> & playerScore )
{
    for ( int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        if ( m_DancerInfo.szDancerComponent[i] != NULL)
        {
            CRoomEndPlayerScoreInfo score;
            score.m_nPos = i;
            score.m_nScore = m_DancerInfo.szDancerScore[i];
            score.m_strName = m_DancerInfo.szDancerComponent[i]->GetRoleAttr()->GetRoleName();

            playerScore.push_back( score );
        }
    }
}

bool CDungeon::IsReadyEnd()
{
    for ( int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        if ( m_DancerInfo.szDancerComponent[i] != NULL  )
        {
            if ( m_DancerInfo.szDancerComponent[i]->GetRoleState() != RoleRoomState_ToEnd )
            {
                return false;
            }
        }
    }

    return true;
};

void CDungeon::SendMsgToAll( GameMsg_Base* pMsg, unsigned int nRoleID )
{
    for( int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        if( m_DancerInfo.szDancerComponent[i] != NULL && m_DancerInfo.szDancerID[i] != nRoleID )
        {
            m_DancerInfo.szDancerComponent[i]->SendPlayerMsg( pMsg );
        }
    }
}

CEntityDungeonComponent * CDungeon::GetComponent( unsigned int nRoleID )
{
    for ( int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        if ( m_DancerInfo.szDancerComponent[i] != NULL && m_DancerInfo.szDancerComponent[i]->GetRoleID() == nRoleID )
        {
            return m_DancerInfo.szDancerComponent[i];
        }
    }

    return NULL;
}

CEntityDungeonComponent * CDungeon::GetComponentByPos(  int nPos )
{
    if ( nPos < 0 || nPos >= MAX_DUNGEON_PLAYER_COUNT )
    {
        return NULL;
    }

    return m_DancerInfo.szDancerComponent[nPos];
}

void CDungeon::SetHostComponent( CEntityDungeonComponent* pRoleComponent )
{
    if ( pRoleComponent != NULL )
    {
        m_DancerInfo.szDancerComponent[ DUNGEON_HOST_POS ] = pRoleComponent;
    }
}

void CDungeon::SetFriendComponent( CEntityDungeonComponent* pRoleComponent )
{
    if ( pRoleComponent != NULL )
    {
        m_DancerInfo.szDancerComponent[ DUNGEON_FRIEND_POS ] = pRoleComponent;
    }
}

unsigned int CDungeon::GetDanceCount()
{
    unsigned int nCount = 0;
    for ( int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        if ( m_DancerInfo.szDancerID[i] > 0 )
        {
            nCount++;
        }
    }

    return nCount;
}

int CDungeon::GetDancerScoreByPos( int pos )
{
    if ( pos < 0 || pos >= MAX_DUNGEON_PLAYER_COUNT )
    {
        return 0;
    }

    return m_DancerInfo.szDancerScore[pos];
}

int CDungeon::GetAllDancerScore()
{
    int nTotalScore = 0;
    for (int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i)
    {
        nTotalScore += (m_DancerInfo.szDancerScore[i] >= 0 ? m_DancerInfo.szDancerScore[i] : 0);
    }

    return nTotalScore;
}

void CDungeon::SetDancerScoreByPos( int pos,  int nScore )
{
    if ( pos < 0 || pos >= MAX_DUNGEON_PLAYER_COUNT )
    {
        return ;
    }

    m_DancerInfo.szDancerScore[pos] = nScore;
}

void CDungeon::SetDancerNameByPos( int pos, std::string & name )
{
    if ( pos < 0 || pos >= MAX_DUNGEON_PLAYER_COUNT )
    {
        return ;
    }

    m_DancerInfo.szDancerName[pos] = name;
}

bool CDungeon::IsHost(unsigned int nRoleID)
{
    if (GetHost() == nRoleID)
    {
        return true;
    }

    return false;
}

std::string CDungeon::GetDancerNameByPos( int pos )
{
    if ( pos < 0 || pos >= MAX_DUNGEON_PLAYER_COUNT )
    {
        return "";
    }

    return m_DancerInfo.szDancerName[pos];
}

void CDungeon::DoEncodePlayerInfo(CEntityDungeonComponent * pRoleComonent, DungeonPlayerInfo & playerInfo)
{
    if (pRoleComonent == NULL)
    {
        return;
    }

    unsigned int nRoleID = pRoleComonent->GetRoleAttr()->GetRoleID();
    playerInfo.m_nRoleID = nRoleID;
    playerInfo.m_strRoleName = pRoleComonent->GetRoleAttr()->GetRoleName();
    playerInfo.m_bIsHost = IsHost(nRoleID);
    playerInfo.m_chRoleType = RoleRoomType_Dancer;
    playerInfo.m_nIsBoss = false;
    playerInfo.m_chRolePos = (char)GetPos(nRoleID);
    playerInfo.m_chRoleState = RoleRoomState_ToStart;
    playerInfo.m_eColorTeam =  TeamMode() == EDanceRoomTeamMode_Team ? EDanceColorTeam_Blue : EDanceColorTeam_None;
    playerInfo.m_nRoleSex = pRoleComonent->GetRoleAttr()->GetSex();
    playerInfo.m_nRoleSkin = pRoleComonent->GetRoleAttr()->GetColor();
    playerInfo.m_nTransFormID = (unsigned short)pRoleComonent->GetRoleAttr()->GetTransformId();

    CDanceGroupInfo * pDGInfo = pRoleComonent->GetRoleDanceGroup()->GetDanceGroupInfo();
    if (pDGInfo)
    {
        playerInfo.m_strDanceGroupName = pDGInfo->m_DGBaseInfo.m_strGroupName;
        playerInfo.m_nDanceGroupBadge = pDGInfo->m_DGBaseInfo.m_nBadge;
        playerInfo.m_nDanceGroupEffect = pDGInfo->m_DGBaseInfo.m_nEffect;
        CDanceGroupMember * pDGMember = pDGInfo->GetGroupMember( playerInfo.m_nRoleID );
        if (pDGMember)
        {
            playerInfo.m_nGroupTitle = (char)pDGMember->m_nTitle;
        }
    }

    playerInfo.m_bIsVIP = pRoleComonent->GetRoleVip()->IsVIP();
    playerInfo.m_nVIPLevel = (unsigned short)pRoleComonent->GetRoleVip()->VIPLevel();
    playerInfo.m_nMoveType = 0;
    playerInfo.m_pItemCOM = pRoleComonent->GetRoleItem();
    playerInfo.m_pGeneCOM = pRoleComonent->GetRoleGene();
    playerInfo.m_pPetCOM = pRoleComonent->GetRolePet();
}

void CDungeon::DoEncodeStage(std::string & strCheckKey, std::vector<char> & vecStage)
{
    std::vector<char> vecAndroidStage;
    std::vector<char> vecIosStage;
    std::vector<char> vecWinStage;
    const std::string& strStageFile =  PlayMusicStage();
    CMusicStageFileManager::LoadStage( strStageFile, vecAndroidStage, vecIosStage, vecWinStage );

    strCheckKey = PlayCheckCode();
    if (  m_DancerInfo.szDancerComponent[0]->GetPhoneOS() == OS_Ios )
    {
        vecStage.assign( vecIosStage.begin(), vecIosStage.end() );
    }
    else if ( m_DancerInfo.szDancerComponent[0]->GetPhoneOS() == OS_WP8 )
    {
        vecStage.assign( vecWinStage.begin(), vecWinStage.end() );
    }
    else
    {
        vecStage.assign( vecAndroidStage.begin(), vecAndroidStage.end() );
    }
}

void CDungeon::DoEncodeBossInfo(unsigned int nBossID, DungeonPlayerInfo & bossInfo)
{
    CBossConfig * pAIDataInfo = ConfigManager::Instance().GetMagicLampConfigMgr().GetAIBossInfo( nBossID );
    if (pAIDataInfo == NULL)
    {
        return;
    }

    bossInfo.m_nRoleID = DealWithBossID(nBossID);
    bossInfo.m_strRoleName = pAIDataInfo->m_strName; 
    bossInfo.m_nRoleSex = pAIDataInfo->m_cSex;
    bossInfo.m_nRoleSkin = (char)pAIDataInfo->m_cSKinColor;
    bossInfo.m_bIsHost = false;
    bossInfo.m_chRoleType = RoleRoomType_Dancer;
    bossInfo.m_nIsBoss = true;
    bossInfo.m_chRoleState = RoleRoomState_ToStart;
    bossInfo.m_eColorTeam = EDanceColorTeam_Red;
    bossInfo.m_nMoveType = 0;
    bossInfo.m_Equips = pAIDataInfo->m_Equips;
}

void CDungeon::StartParclose()
{
    GameMsg_S2C_StartParcloseSuc sucMsg;
    sucMsg.m_cRoomType = eRoomType_Dungeon;
    sucMsg.m_nSceneID =  m_ParcloseConfig.m_nSceneID;
    sucMsg.m_nMusicID =  PlayMusicID();
    sucMsg.m_nMode = m_ParcloseConfig.m_nMusicMode;
    sucMsg.m_nLevel = m_ParcloseConfig.m_nMusicLevel;
    sucMsg.m_cSelfRoomType = RoleRoomType_Dancer;
    sucMsg.m_cTeamMode = TeamMode();

    // Host
    DungeonPlayerInfo hostPlayerInfo;
    DoEncodePlayerInfo(m_DancerInfo.szDancerComponent[0], hostPlayerInfo);
    sucMsg.m_PlayerList.push_back(hostPlayerInfo);

    // Friend
    if (m_DancerInfo.szDancerComponent[1] != NULL)
    {
        DungeonPlayerInfo friendPlayerInfo;
        DoEncodePlayerInfo(m_DancerInfo.szDancerComponent[1], friendPlayerInfo);
        sucMsg.m_PlayerList.push_back(friendPlayerInfo);
    }
    
    
    // Boss
    for ( int i = 0 ; i < MAX_DUNGEON_BOSS_COUNT; ++i )
    {
        if (m_szRoomBossID[i] > 0)
        {
            DungeonPlayerInfo bossInfo;
            DoEncodeBossInfo(m_szRoomBossID[i], bossInfo);
            bossInfo.m_chRolePos =(char)(i+2);
            sucMsg.m_PlayerList.push_back(bossInfo);
        }
    }

    // stage info
    DoEncodeStage(sucMsg.m_strCheckKey, sucMsg.m_vecStage);

    // send msg
    for (int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i)
    {
        if (m_DancerInfo.szDancerComponent[i] == NULL)
        {
            continue;
        }

        sucMsg.m_nCrystalNum = m_DancerInfo.szDancerComponent[i]->GetCrystal();
        sucMsg.m_nDustNum = m_DancerInfo.szDancerComponent[i]->GetMagicDust();

        if (i == DUNGEON_HOST_POS)
        {
            sucMsg.m_nIsHost = true;
            m_DancerInfo.szDancerComponent[i]->OnStartDance();
        }
        else
        {
            sucMsg.m_nIsHost = false;
        }

        m_DancerInfo.szDancerComponent[i]->SendPlayerMsg( &sucMsg );
    }

    // other place
    GameMsg_S2C_StartRoomSuc msgResponse;
    this->SendMsgToAll( &msgResponse );
}

void CDungeon::CalcResult()
{

}

void CDungeon::ParcloseEnd()
{
    // 保存分数
    for ( int i = 0;  i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        CEntityDungeonComponent *pRoleDungeonComponent = m_DancerInfo.szDancerComponent[i];

        if ( pRoleDungeonComponent != NULL )
        {
            m_DancerInfo.szDancerScore[i] = pRoleDungeonComponent->RoleMarkInfo().Mark();
        }
    }

    for ( int i = 0;  i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        CEntityDungeonComponent *pRoleDungeonComponent = m_DancerInfo.szDancerComponent[i];
        if ( pRoleDungeonComponent != NULL )
        {
            pRoleDungeonComponent->ParcloseCalcResult();
        }
    }

    for ( int i = 0;  i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        m_DancerInfo.szDancerID[i] = 0;
        m_DancerInfo.szDancerComponent[i] = NULL;
        m_DancerInfo.szDancerScore[i] = -1;
    }
}

bool CDungeon::IsAllDanceOut()
{
    return m_DancerInfo.szDancerID[0] == 0 && m_DancerInfo.szDancerID[1] == 0;
}

int CDungeon::GetPos( unsigned int nRoleID )
{
    for ( int i = 0;  i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        if ( m_DancerInfo.szDancerID[i] == nRoleID )
        {
            return i;
        }
    }

    return -1;
}

int CDungeon::GetBossCount()
{
    int nCount = 0;
    for ( int i = 0;  i < MAX_DUNGEON_BOSS_COUNT; ++i )
    {
        if ( m_szRoomBossID[i] > 0 )
        {
            nCount++;
        }
    }

    return nCount;
}

int CDungeon::GetBossScore( unsigned int nBossID, int nRound )
{
    std::map<unsigned int,mapTable>::iterator it = m_BossScore.find( nBossID );
    if ( it != m_BossScore.end() )
    {
        mapTable::iterator iter = it->second.find( nRound );
        if ( iter != it->second.end() )
        {
            return iter->second;
        }
    }

    return 0;
}

unsigned int CDungeon::GetBossID( int nIndex )
{
    if ( nIndex >= 0 && nIndex <= 1 )
    {
        return m_szRoomBossID[nIndex];
    }

    return 0;
}

void CDungeon::QuitRoom( unsigned int nRoleID, bool bOut )
{
    for ( int i = 0;  i < MAX_DUNGEON_PLAYER_COUNT; ++i )
    {
        if ( m_DancerInfo.szDancerID[i] == nRoleID )
        {
            m_DancerInfo.szDancerID[i] = 0;
            m_DancerInfo.szDancerComponent[i] = NULL;
        }
    }

    if ( IsAllDanceOut())
    {
        // manager clear
        CDungeonMgr::Instance().RemoveParcloseRoom( RoomID() );	
    }
    else
    {
        if ( bOut )
        {
            for ( int i = 0;  i < MAX_DUNGEON_PLAYER_COUNT; ++i )
            {
                if ( m_DancerInfo.szDancerComponent[i] != NULL )
                {
                    // notice others
                    GameMsg_S2C_QuitPlayer msgOtherResponse;
                    msgOtherResponse.m_chQuitType = (char)RoomQuitType_Logout;
                    msgOtherResponse.m_chRoleRoomType = (char)RoleRoomType_Dancer;
                    msgOtherResponse.m_chRoleRoomPos = (char)(1-i);
                    msgOtherResponse.m_chHostRoomType = (char)RoleRoomType_Dancer;
                    msgOtherResponse.m_chHostRoomPos = (char)DUNGEON_HOST_POS;
                    msgOtherResponse.m_chHostRoomState = (char)RoleRoomState_Start;
                    msgOtherResponse.m_strRoomPwd.assign( "" );

                    m_DancerInfo.szDancerComponent[i]->SendPlayerMsg( &msgOtherResponse );
                }
            }
        }	
    }
}
