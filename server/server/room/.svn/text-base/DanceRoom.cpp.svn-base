#include "DanceRoom.h"
#include "../roomLib/RoomCheckDataMgr.h"
#include "../grade/GradeInfoMgr.h"
#include "../logic/PlayerManager.h"
#include "../logic/LogicCirculator.h"
#include "../music/MusicBoardMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include <algorithm>

extern CLogicCirculator* g_pCirculator;


RoomDancer::RoomDancer( unsigned int nRoleID )
    : RoomRoleBase( nRoleID )
    , m_nPosition( 0 )
    , m_eState( RoleRoomState_None )
    , m_bHost( false )
    , m_bDancer( false )
    , m_nTotalMark( 0 )
{
}

RoomDancer::~RoomDancer()
{
}

void RoomDancer::SendMessage( GameMsg_Base &msg )
{
    CRoleEntity *pEntity = CPlayerManager::Instance().GetEntityByRoleID( m_nRoleID );

    if ( pEntity != NULL )
        pEntity->SendPlayerMsg( &msg );
}

bool RoomDancer::RecvRoundMark( int nMode, int nRound, unsigned int nMark, 
    char nKeyRank, const std::vector<int>& rRoundRank )
{
    return m_markInfo.AddRoundMark( nMode, nRound, nMark, nKeyRank, rRoundRank );
}

void RoomDancer::ClearMark()
{
    m_nTotalMark = 0;
    m_markInfo.ClearMark();
}

void RoomDancer::SumTotalMark( int nRank, int nRankMark )
{
    m_nTotalMark += m_markInfo.TotalRankCount( nRank ) * nRankMark;
}

void RoomDancer::CalcGrade()
{
    int nGrade = RoomDanceGrade_D;

    if ( m_markInfo.TotalRankBase() > 0 )
    {
        int nAccuracy = m_nTotalMark / m_markInfo.TotalRankBase();
        if ( nAccuracy >= 100 )
        {
            nGrade = RoomDanceGrade_SS;
        }
        else if ( nAccuracy >= 90 )
        {
            if ( m_markInfo.TotalRankPercent( RoomRoundRank_Good ) < 1 && 
                m_markInfo.TotalRankPercent( RoomRoundRank_Bad ) < 1 && 
                m_markInfo.TotalRankCount( RoomRoundRank_Miss ) == 0 )
            {
                nGrade = RoomDanceGrade_S;
            }
            else
            {
                nGrade = RoomDanceGrade_A;
            }
        }
        else if ( nAccuracy >= 80 )
        {
            if ( m_markInfo.TotalRankCount( RoomRoundRank_Miss ) == 0 )
            {
                nGrade = RoomDanceGrade_A;
            }
            else
            {
                nGrade = RoomDanceGrade_B;
            }
        }
        else if ( nAccuracy >= 70 )
        {
            if ( m_markInfo.TotalRankCount( RoomRoundRank_Miss ) == 0 )
            {
                nGrade = RoomDanceGrade_B;
            }
            else
            {
                nGrade = RoomDanceGrade_C;
            }
        }
        else if ( nAccuracy >= 60 )
        {
            nGrade = RoomDanceGrade_C;
        }
    }

    m_markInfo.WinGrade( nGrade );
}

unsigned int RoomDancer::GetTotalScore() const
{
    return m_markInfo.Mark();
}

//////////////////////////////////////////////////////////////////////////

enum EDanceRoomPosState
{
    EDanceRoomPosState_Close = -1,
    EDanceRoomPosState_Idle,
    EDanceRoomPosState_Dancer
};

DanceRoomPosManager::DanceRoomPosManager( int nMaxCount )
    : m_nMaxCount( nMaxCount )
    , m_nDancerCount( 0 )
    , m_vectDancer( nMaxCount, 0 )
    , m_vectPosState( nMaxCount, EDanceRoomPosState_Idle )
{
}

DanceRoomPosManager::~DanceRoomPosManager()
{
}

void DanceRoomPosManager::Clear()
{
    m_nDancerCount = 0;
    std::fill( m_vectDancer.begin(), m_vectDancer.end(), 0 );
    std::fill( m_vectPosState.begin(), m_vectPosState.end(), EDanceRoomPosState_Idle );
}

int DanceRoomPosManager::GetIdelPos() const
{
    if ( !HasFull() )
        return FindPos( EDanceRoomPosState_Idle );

    return INVALID_DANCE_ROOM_POS;
}

int DanceRoomPosManager::GetFirstDancerPos() const
{
    if ( !HasEmpty() )
        return FindPos( EDanceRoomPosState_Dancer );

    return INVALID_DANCE_ROOM_POS;
}

bool DanceRoomPosManager::AddDancer( int nPos, unsigned int nRoleID )
{
    if ( HasFull() || !IsValidPos( nPos ) )
        return false;
    else if ( m_vectPosState[nPos] != EDanceRoomPosState_Idle )
        return false;

    m_vectPosState[nPos] = EDanceRoomPosState_Dancer;
    m_vectDancer[nPos] = nRoleID;
    ++m_nDancerCount;

    return true;
}

void DanceRoomPosManager::RemoveDancer( int nPos )
{
    if ( HasEmpty() || !IsValidPos( nPos ) )
        return;
    else if ( m_vectPosState[nPos] != EDanceRoomPosState_Dancer )
        return;

    m_vectPosState[nPos] = EDanceRoomPosState_Idle;
    m_vectDancer[nPos] = 0;
    --m_nDancerCount;
}

unsigned int DanceRoomPosManager::GetDancer( int nPos ) const
{
    if ( HasEmpty() || !IsValidPos( nPos ) )
        return 0;
    else if ( m_vectPosState[nPos] != EDanceRoomPosState_Dancer )
        return 0;

    return m_vectDancer[nPos];
}

bool DanceRoomPosManager::MoveDancerPos( int nOldPos, int nNewPos )
{
    if ( HasEmpty() || !IsValidPos( nOldPos ) || !IsValidPos( nNewPos ) )
        return false;
    else if ( m_vectPosState[nOldPos] != EDanceRoomPosState_Dancer )
        return false;
    else if ( m_vectPosState[nNewPos] != EDanceRoomPosState_Idle )
        return false;

    std::swap( m_vectPosState[nOldPos], m_vectPosState[nNewPos] );
    std::swap( m_vectDancer[nOldPos], m_vectDancer[nNewPos] );

    return true;
}

bool DanceRoomPosManager::ClosePos( int nPos )
{
    if ( !IsValidPos( nPos ) )
        return true;
    else if ( m_vectPosState[nPos] == EDanceRoomPosState_Close )
        return true;
    else if ( m_vectPosState[nPos] == EDanceRoomPosState_Dancer )
        return false;

    m_vectPosState[nPos] = EDanceRoomPosState_Close;

    return true;
}

void DanceRoomPosManager::OpenPos( int nPos )
{
    if ( !IsValidPos( nPos ) )
        return;

    if ( m_vectPosState[nPos] == EDanceRoomPosState_Close )
        m_vectPosState[nPos] = EDanceRoomPosState_Idle;
}

bool DanceRoomPosManager::IsPosClosed( int nPos ) const
{
    if ( !IsValidPos( nPos ) )
        return true;

    return (m_vectPosState[nPos] == EDanceRoomPosState_Close);
}

bool DanceRoomPosManager::IsPosOpend( int nPos ) const
{
    return !IsPosClosed( nPos );
}

int DanceRoomPosManager::FindPos( int nState ) const
{
    std::vector<int>::const_iterator pos = std::find( m_vectPosState.begin(), m_vectPosState.end(), nState );

    if ( pos != m_vectPosState.end() )
        return ( pos - m_vectPosState.begin() );
    else
        return INVALID_DANCE_ROOM_POS;
}

bool DanceRoomPosManager::IsValidPos( int nPos ) const
{
    if ( MIN_DANCE_ROOM_POS <= nPos && nPos < m_nMaxCount )
        return true;
    else
        return false;
}

//////////////////////////////////////////////////////////////////////////

DanceBaseRoom::DanceBaseRoom( ERoomType eType, unsigned int nRoomID, const std::string &rName, const std::string &rPassword )
    : RoomBase( eType, nRoomID, rName, rPassword )
    , m_eState( EDanceRoomState_Idle )
    , m_nChoosedMusic( RAN_MUSIC_ID )
    , m_nChoosedMusicMode( MusicMode_None )
    , m_nChoosedMusicLevel( MusicLevel_None )
    , m_nPlayScene( RAN_SCENE_ID )
    , m_nPlayMusic( RAN_MUSIC_ID )
    , m_nPlayMusicMode( MusicMode_None )
    , m_nPlayMusicLevel( MusicLevel_None )
    , m_nCheckTime( 0 )
    , m_nHostPos( MIN_DANCE_ROOM_POS )
    , m_nStartTime( 0 )
    , m_posMgr( MAX_ROOM_PLAYER_COUNT )
{
}

DanceBaseRoom::~DanceBaseRoom()
{
}

void DanceBaseRoom::Prepare()
{
    m_eState = EDanceRoomState_Preparing;
    m_nStartTime = 0;
    CodeRandom( MAX_ROOMKEY_LENGTH ).swap( m_strCheckCode );

    PrepareMusicAndScene();

    m_nCheckTime = CRoomCheckDataMgr::Instance().GetRoomCheck( m_nPlayMusicMode ).m_nCheckTime;

    SetAllDancerState( RoleRoomState_ToStart );
}

void DanceBaseRoom::Start()
{
    m_eState = EDanceRoomState_Playing;
    m_nStartTime = (unsigned int)time( NULL );

    SetAllDancerState( RoleRoomState_Start );
}

void DanceBaseRoom::End()
{
    m_eState = EDanceRoomState_Idle;
    m_nPlayScene = RAN_SCENE_ID;
    m_nPlayMusic = RAN_MUSIC_ID;
    m_nPlayMusicMode = MusicMode_None;
    m_nPlayMusicLevel = MusicLevel_None;
    m_nStartTime = 0;
    m_nCheckTime = 0;
    m_strCheckCode.clear();
    m_strMusicSource.clear();

    SetAllDancerState( RoleRoomState_End );
}

void DanceBaseRoom::Clear()
{
    // not clear m_eType and m_nRoomID for reuse
    m_eState = EDanceRoomState_Idle;
    m_nPlayScene = m_nScene = RAN_SCENE_ID;
    m_nPlayMusic = m_nChoosedMusic = RAN_MUSIC_ID;
    m_nPlayMusicMode = m_nChoosedMusicMode = MusicMode_None;
    m_nPlayMusicLevel = m_nChoosedMusicLevel = MusicLevel_None;
    m_nHostPos = INVALID_DANCE_ROOM_POS;
    m_nStartTime = m_nCheckTime = 0;

    m_strName.clear();
    m_strPassword.clear();
    m_strCheckCode.clear();
    m_strMusicSource.clear();
    m_posMgr.Clear();
    RoomBase::ClearPlayer();
}

void DanceBaseRoom::ChangeChoosedMusicInfo( int nMusic, unsigned char nMusicMode, unsigned char nMusicLevel )
{
    m_nChoosedMusic = nMusic;
    m_nChoosedMusicMode = nMusicMode;
    m_nChoosedMusicLevel = nMusicLevel;
}

bool DanceBaseRoom::DancerIn( RoomDancer *dancer )
{
    if ( dancer == NULL )
        return false;

    int nPos = m_posMgr.GetIdelPos();
    if ( nPos < MIN_DANCE_ROOM_POS )
        return false;

    if ( PlayerIn( dancer ) )
    {
        m_posMgr.AddDancer( nPos, dancer->GetRoleID() );

        dancer->SetPosition( nPos );
        dancer->SetState( RoleRoomState_Wait );
        dancer->SetDancer( true );
        dancer->SetHost( m_nHostPos == nPos );

        return true;
    }

    return false;
}

void DanceBaseRoom::DancerOut( unsigned int nRoleID )
{
    RoomDancer *dancer = (RoomDancer *)PlayerOut( nRoleID );

    if ( dancer == NULL )
        return;

    if ( dancer->GetPosition() >= MIN_DANCE_ROOM_POS )
        m_posMgr.RemoveDancer( dancer->GetPosition() );

    SAFE_DELETE( dancer );
}

int DanceBaseRoom::GetDancerPos( unsigned int nRoleID )
{
    RoomDancer *dancer = (RoomDancer *)FindPlayer( nRoleID );

    if ( dancer == NULL )
        return INVALID_DANCE_ROOM_POS;

    return dancer->GetPosition();
}

bool DanceBaseRoom::MoveDancerPos( int nOldPos, int nNewPos )
{
    if ( nOldPos == nNewPos )
        return false;

    RoomDancer *dancer = FindDancerByPos( nOldPos );
    if ( dancer == NULL )
        return false;

    if ( m_posMgr.MoveDancerPos( nOldPos, nNewPos ) )
    {
        dancer->SetPosition( nNewPos );

        if ( dancer->HasHost() )
        {
            m_nHostPos = nNewPos;
        }

        return true;
    }

    return false;
}

bool DanceBaseRoom::HasStart() const
{
    return (m_eState > EDanceRoomState_Idle);
}

bool DanceBaseRoom::HasLegalEndTime() const
{
    unsigned int nNow = (unsigned int)time( NULL );
    unsigned int nLegalTime = m_nStartTime + m_nCheckTime;

    if ( nNow >= nLegalTime )
        return true;
    else
        return false;
}

const RoomDancer* DanceBaseRoom::GetHost()
{
    return FindDancerByPos( m_nHostPos );
}

const RoomDancer* DanceBaseRoom::AssignHost()
{
    RoomDancer *dancer = FindDancerByPos( m_nHostPos );

    if ( dancer != NULL )
        return dancer;

    int nPos = m_posMgr.GetFirstDancerPos();
    dancer = FindDancerByPos( nPos );

    if ( dancer != NULL )
    {
        m_nHostPos = nPos;
        dancer->SetHost( true );
    }

    return dancer;
}

const RoomDancer* DanceBaseRoom::AssignHost( int nPos )
{
    RoomDancer *pOldHost = FindDancerByPos( m_nHostPos );

    if ( m_nHostPos == nPos )
        return pOldHost;

    RoomDancer *pNewHost = FindDancerByPos( nPos );

    if ( pNewHost == NULL )
        return pOldHost;

    m_nHostPos = nPos;
    pNewHost->SetHost( true );

    if ( pOldHost != NULL )
        pOldHost->SetHost( false );

    return pNewHost;
}

void DanceBaseRoom::PrepareMusicAndScene()
{
    bool bOnlyFresher = ( ( m_eType == eRoomType_Dungeon ) || ( g_pCirculator->GetServerInstallerID() == INSTALLER_FRESHER ) );

    m_nPlayScene = m_nScene;
    m_nPlayMusic = m_nChoosedMusic;
    m_nPlayMusicMode = m_nChoosedMusicMode;
    m_nPlayMusicLevel = m_nChoosedMusicLevel;

    if ( m_nPlayScene == RAN_SCENE_ID )
        m_nPlayScene = (int)ConfigManager::Instance().GetSceneModeManager().RandomScene( bOnlyFresher );

    MusicStageManager& stageMgr = ConfigManager::Instance().GetMusicStageManager();

    if ( m_nPlayMusicMode == MusicMode_None )
        m_nPlayMusicMode = stageMgr.RandomMode( (unsigned short)m_nPlayMusic, MusicMode_Sweethearts );

    if ( m_nPlayMusicLevel == MusicLevel_None )
        m_nPlayMusicLevel = stageMgr.RandomLevel( (unsigned short)m_nPlayMusic );

    const MusicConfig* pMusicInfo = NULL;
    MusicConfigManager &musicMgr = ConfigManager::Instance().GetMusicConfigManager();

    if ( m_nPlayMusic == RAN_MUSIC_ID )
    {
        if ( bOnlyFresher )
        {
            m_nPlayMusic = (int)stageMgr.RandomMusic( m_nPlayMusicMode, m_nPlayMusicLevel, bOnlyFresher );

            pMusicInfo = musicMgr.GetByID( (unsigned short)m_nPlayMusic );
            if ( pMusicInfo != NULL && !pMusicInfo->m_bIsFresher )
            {
                pMusicInfo = musicMgr.GetAnyFresherMusic();
                if ( pMusicInfo != NULL )
                {
                    m_nPlayMusic = (int)pMusicInfo->m_nMusicID;

                    if ( !stageMgr.IsHaveMode( (unsigned short)m_nPlayMusic, m_nPlayMusicMode ) )
                    {
                        m_nPlayMusicMode = stageMgr.RandomMode( (unsigned short)m_nPlayMusic, MusicMode_Sweethearts );
                    }
                    if ( !stageMgr.IsHaveLevel( (unsigned short)m_nPlayMusic, m_nPlayMusicLevel ) )
                    {
                        m_nPlayMusicLevel = stageMgr.RandomLevel( (unsigned short)m_nPlayMusic );
                    }
                }
            }
        }
        else
        {
            m_nPlayMusic = (int)stageMgr.RandomMusic( m_nPlayMusicMode, m_nPlayMusicLevel );
        }
    }

    if ( m_eType == eRoomType_NewPlayer )
    {
        int nMusic;
        for ( int nChance = 3; nChance > 0; --nChance )
        {
            nMusic = CMusicBoardManager::Instance().RandomFresherSong( m_nPlayMusicMode );
            if ( nMusic != RAN_MUSIC_ID )
            {
                m_nPlayMusic = nMusic;
                break;
            }
        }
    }

    if ( pMusicInfo == NULL || pMusicInfo->m_nMusicID != (unsigned short)m_nPlayMusic )
        pMusicInfo = musicMgr.GetByID( (unsigned short)m_nPlayMusic );

    if ( pMusicInfo != NULL )
        m_strMusicSource = pMusicInfo->m_strMusicSource;
}

bool DanceBaseRoom::RecvRoundMark( unsigned int nRoleID, int nMode, int nRound, 
    unsigned int nMark, char nKeyRank, const std::vector<int>& rRoundRank )
{
    RoomDancer *dancer = (RoomDancer *)FindPlayer( nRoleID );

    if ( dancer == NULL )
        return false;

    return dancer->RecvRoundMark( nMode, nRound, nMark, nKeyRank, rRoundRank );
}

void DanceBaseRoom::CalcDanceResult()
{
    RoomDancer *dancer = NULL;

    for ( std::map<unsigned int, RoomRoleBase *>::const_iterator itr = m_mapPlayers.begin(); 
        itr != m_mapPlayers.end(); ++itr )
    {
        dancer = (RoomDancer *)itr->second;

        if ( dancer == NULL )
        {
            continue;
        }

        SumRankMark( dancer );
        DanceResultRanking( dancer );
    }
}

unsigned int DanceBaseRoom::GetDancerScore( unsigned int nRoleID ) const
{
    RoomDancer *dancer = (RoomDancer *)FindPlayer( nRoleID );

    if ( dancer == NULL )
        return 0;

    return dancer->GetTotalScore();
}

void DanceBaseRoom::GetDancerMark( unsigned int nRoleID, CRoomMarkInfo &rMark ) const
{
    RoomDancer *dancer = (RoomDancer *)FindPlayer( nRoleID );

    if ( dancer == NULL )
        return;

    rMark = dancer->GetMarkInfo();
}

RoomDancer* DanceBaseRoom::FindDancerByPos( int nPos ) const
{
    unsigned int nRoleID = m_posMgr.GetDancer( nPos );

    return (RoomDancer *)FindPlayer( nRoleID );
}

void DanceBaseRoom::SetAllDancerState( ERoleRoomState eState )
{
    RoomDancer *dancer = NULL;

    for ( std::map<unsigned int, RoomRoleBase *>::iterator itr = m_mapPlayers.begin(); 
        itr != m_mapPlayers.end(); ++itr )
    {
        dancer = (RoomDancer *)itr->second;

        if ( dancer == NULL )
        {
            continue;
        }

        dancer->SetState( eState );
    }
}

void DanceBaseRoom::SumRankMark( RoomDancer *pDancer ) const
{
    if ( pDancer == NULL )
        return;

    int nRankMark = 0;

    for ( int nRank = RoomRoundRank_Miss; nRank < RoomRoundRank_Max; ++nRank )
    {
        nRankMark = CGradeInfoManager::Instance().GetGradePercent( nRank );

        pDancer->SumTotalMark( nRank, nRankMark );
    }

    pDancer->CalcGrade();
}

void DanceBaseRoom::DanceResultRanking( RoomDancer * )
{
    //do nothing
}

