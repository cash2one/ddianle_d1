#include "GameMsg_S2C_DanceGroupChallenge.h"
#include "DanceGroupMsgDef.h"
#include "../roomLib/RoomDataStruct.h"


DanceGroupChallengeStoreySimpleInfo::DanceGroupChallengeStoreySimpleInfo()
    : m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nStoreyState( EDanceGroupChallengeStoreyState_NotStart )
{
}

DanceGroupChallengeStoreySimpleInfo::~DanceGroupChallengeStoreySimpleInfo()
{
}

bool DanceGroupChallengeStoreySimpleInfo::doEncode( CParamPool& IOBuff ) const
{
    IOBuff << m_nStorey << m_nStoreyState << stringw( m_strAtlas ) << stringw( m_strIcon );

    return true;
}

DanceGroupChallengeStoreyWholeInfo::DanceGroupChallengeStoreyWholeInfo()
    : m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nStoreyState( EDanceGroupChallengeStoreyState_NotStart )
    , m_nActive( 0 )
    , m_nMusicLevel( MusicLevel_None )
    , m_nMusicMode( MusicMode_None )
    , m_nMusic( RAN_MUSIC_ID )
    , m_nTargetType( EDanceGroupChallengeTargetType_Score )
    , m_nTargetScore( 0 )
    , m_nCurrentScore( 0 )
    , m_nAdditional( 0 )
    , m_nMaxChance( 0 )
    , m_nCurChance( 0 )
{
}

DanceGroupChallengeStoreyWholeInfo::~DanceGroupChallengeStoreyWholeInfo()
{
}

bool DanceGroupChallengeStoreyWholeInfo::doEncode( CParamPool& IOBuff ) const
{
    IOBuff << m_nStorey << m_nStoreyState << m_nActive << m_nMusicLevel << m_nMusicMode 
        << m_nMusic << m_nTargetType << m_nTargetScore << m_nCurrentScore << m_nAdditional 
        << m_nMaxChance << m_nCurChance << m_listReward;

    return true;
}

DanceGroupChallengeRoomInfo::DanceGroupChallengeRoomInfo()
    : m_nRoomType( eRoomType_DanceGroupChallenge )
    , m_nScene( RAN_SCENE_ID )
    , m_nMusic( RAN_MUSIC_ID )
    , m_nMusicMode( MusicMode_None )
    , m_nMusicLevel( MusicLevel_None )
    , m_nSelfRoomType( RoleRoomType_Dancer )
    , m_nTeamMode( EDanceRoomTeamMode_None )
    , m_bIsHost( true )
{
}

DanceGroupChallengeRoomInfo::~DanceGroupChallengeRoomInfo()
{
}

void DanceGroupChallengeRoomInfo::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nRoomType << m_nScene << m_nMusic << m_nMusicMode << m_nMusicLevel 
        << m_nSelfRoomType << m_nTeamMode << m_bIsHost;

    IOBuff.AddUShort( (unsigned short)m_listDancer.size() );
    for ( std::list<DungeonPlayerInfo>::iterator itr = m_listDancer.begin(); 
        itr != m_listDancer.end(); ++itr )
    {
        itr->doEncode( IOBuff );
    }
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_SyncChallengeConfig::GameMsg_S2C_SyncChallengeConfig()
    : GameMsg_Base( MSG_S2C_SyncChallengeConfig )
    , m_nTotalStorey( 0 )
    , m_nOpenLevel( 0 )
    , m_nInspireCostType( 0 )
    , m_nInspireCostAmount( 0 )
{
}

GameMsg_S2C_SyncChallengeConfig::~GameMsg_S2C_SyncChallengeConfig()
{
}

bool GameMsg_S2C_SyncChallengeConfig::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nTotalStorey << m_nOpenLevel << m_nInspireCostType 
        << m_nInspireCostAmount << stringw( m_strHelp ) << m_listRewardStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestEnterChallengeRes::GameMsg_S2C_RequestEnterChallengeRes()
    : GameMsg_Base( MSG_S2C_RequestEnterChallengeRes )
    , m_nResult( EEnterChallengeRes_Success )
    , m_nOpenTime( 0 )
    , m_nCurrentStorey( MIN_CHALLENGE_STOREY )
    , m_nChanceCostType( 0 )
    , m_nChanceCostAmount( 0 )
{
}

GameMsg_S2C_RequestEnterChallengeRes::~GameMsg_S2C_RequestEnterChallengeRes()
{
}

bool GameMsg_S2C_RequestEnterChallengeRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_nOpenTime << m_nCurrentStorey 
        << m_nChanceCostType << m_nChanceCostAmount;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestChallengeStoreyListRes::GameMsg_S2C_RequestChallengeStoreyListRes()
    : GameMsg_Base( MSG_S2C_RequestChallengeStoreyListRes )
    , m_nCurPage( 0 )
    , m_nTotalCount( 0 )
{
}

GameMsg_S2C_RequestChallengeStoreyListRes::~GameMsg_S2C_RequestChallengeStoreyListRes()
{
}

bool GameMsg_S2C_RequestChallengeStoreyListRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nCurPage << m_nTotalCount << m_listInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestChallengeStoreyInfoRes::GameMsg_S2C_RequestChallengeStoreyInfoRes()
    : GameMsg_Base( MSG_S2C_RequestChallengeStoreyInfoRes )
{
}

GameMsg_S2C_RequestChallengeStoreyInfoRes::~GameMsg_S2C_RequestChallengeStoreyInfoRes()
{
}

bool GameMsg_S2C_RequestChallengeStoreyInfoRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_wholeInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestChallengeRankListRes::GameMsg_S2C_RequestChallengeRankListRes()
    : GameMsg_Base( MSG_S2C_RequestChallengeRankListRes )
    , m_nRankType( EDanceGroupChallengeRankType_ScoreRank )
    , m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nCurPage( 0 )
    , m_nTotalCount( 0 )
{
}

GameMsg_S2C_RequestChallengeRankListRes::~GameMsg_S2C_RequestChallengeRankListRes()
{
}

bool GameMsg_S2C_RequestChallengeRankListRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nRankType << m_nStorey << m_nCurPage << m_nTotalCount << m_listInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestGetChallengeRewardRes::GameMsg_S2C_RequestGetChallengeRewardRes()
    : GameMsg_Base( MSG_S2C_RequestGetChallengeRewardRes )
    , m_nResult( EGetChallengeRewardRes_Success )
    , m_bGetAll( false )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_S2C_RequestGetChallengeRewardRes::~GameMsg_S2C_RequestGetChallengeRewardRes()
{
}

bool GameMsg_S2C_RequestGetChallengeRewardRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_bGetAll << m_nRequestStorey << m_listReward;

    return true;
}

GameMsg_S2C_ChallengeRewardNotice::GameMsg_S2C_ChallengeRewardNotice()
    : GameMsg_Base( MSG_S2C_ChallengeRewardNotice )
    , m_nStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_S2C_ChallengeRewardNotice::~GameMsg_S2C_ChallengeRewardNotice()
{
}

bool GameMsg_S2C_ChallengeRewardNotice::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestChallengeInspireRes::GameMsg_S2C_RequestChallengeInspireRes()
    : GameMsg_Base( MSG_S2C_RequestChallengeInspireRes )
    , m_nResult( EChallengeInspireRes_Success )
    , m_nAdditional( 0 )
{
}

GameMsg_S2C_RequestChallengeInspireRes::~GameMsg_S2C_RequestChallengeInspireRes()
{
}

bool GameMsg_S2C_RequestChallengeInspireRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_nAdditional;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestAddChallengeChanceRes::GameMsg_S2C_RequestAddChallengeChanceRes()
    : GameMsg_Base( MSG_S2C_RequestAddChallengeChanceRes )
    , m_nResult( EAddChallengeChanceRes_Success )
    , m_nMaxChance( 0 )
    , m_nCurChance( 0 )
    , m_nNextCostType( 0 )
    , m_nNextCostAmount( 0 )
{
}

GameMsg_S2C_RequestAddChallengeChanceRes::~GameMsg_S2C_RequestAddChallengeChanceRes()
{
}

bool GameMsg_S2C_RequestAddChallengeChanceRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_nMaxChance << m_nCurChance << m_nNextCostType << m_nNextCostAmount;

    return true;
}

GameMsg_S2C_RefreshChallengeChanceNotice::GameMsg_S2C_RefreshChallengeChanceNotice()
    : GameMsg_Base( MSG_S2C_RefreshChallengeChanceNotice )
    , m_nMaxChance( 0 )
    , m_nCurChance( 0 )
{
}

GameMsg_S2C_RefreshChallengeChanceNotice::~GameMsg_S2C_RefreshChallengeChanceNotice()
{
}

bool GameMsg_S2C_RefreshChallengeChanceNotice::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nMaxChance << m_nCurChance;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestOpenChallengeStoreyRes::GameMsg_S2C_RequestOpenChallengeStoreyRes()
    : GameMsg_Base( MSG_S2C_RequestOpenChallengeStoreyRes )
    , m_nResult( EOpenChallengeStoreyRes_Success )
{
}

GameMsg_S2C_RequestOpenChallengeStoreyRes::~GameMsg_S2C_RequestOpenChallengeStoreyRes()
{
}

bool GameMsg_S2C_RequestOpenChallengeStoreyRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_wholeInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestPrepareChallengeRes::GameMsg_S2C_RequestPrepareChallengeRes()
    : GameMsg_Base( MSG_S2C_RequestPrepareChallengeRes )
    , m_nResult( EPrepareChallengeRes_Success )
{
}

GameMsg_S2C_RequestPrepareChallengeRes::~GameMsg_S2C_RequestPrepareChallengeRes()
{
}

bool GameMsg_S2C_RequestPrepareChallengeRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_wholeInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestStartChallengeRes::GameMsg_S2C_RequestStartChallengeRes()
    : GameMsg_Base( MSG_S2C_RequestStartChallengeRes )
    , m_nResult( EStartChallengeRes_Success )
{
}

GameMsg_S2C_RequestStartChallengeRes::~GameMsg_S2C_RequestStartChallengeRes()
{
}

bool GameMsg_S2C_RequestStartChallengeRes::doEncode( CParamPool& IOBuff )
{
    IOBuff.AddUChar( m_nResult );
    IOBuff.AddStringW( m_strCheckKey );
    IOBuff.AddPerFixSizeMemory( m_strStage.c_str(), (unsigned short)m_strStage.size() );
    m_roomInfo.doEncode( IOBuff );

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_ChallengeStageEnd::GameMsg_S2C_ChallengeStageEnd()
    : GameMsg_Base( MSG_S2C_ChallengeStageEnd )
    , m_nResult( EChallengeEndRes_Success )
{
}

GameMsg_S2C_ChallengeStageEnd::~GameMsg_S2C_ChallengeStageEnd()
{
}

bool GameMsg_S2C_ChallengeStageEnd::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_scoreInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_ChallengeStoreyFinishedNotice::GameMsg_S2C_ChallengeStoreyFinishedNotice()
    : GameMsg_Base( MSG_S2C_ChallengeStoreyFinishedNotice )
    , m_nStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_S2C_ChallengeStoreyFinishedNotice::~GameMsg_S2C_ChallengeStoreyFinishedNotice()
{
}

bool GameMsg_S2C_ChallengeStoreyFinishedNotice::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_ChallengeClosedNotice::GameMsg_S2C_ChallengeClosedNotice()
    : GameMsg_Base( MSG_S2C_ChallengeClosedNotice )
{
}

GameMsg_S2C_ChallengeClosedNotice::~GameMsg_S2C_ChallengeClosedNotice()
{
}

