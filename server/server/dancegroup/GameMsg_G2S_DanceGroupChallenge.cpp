#include "GameMsg_G2S_DanceGroupChallenge.h"
#include "DanceGroupMsgDef.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DanceGroupInfoDef.h"


DanceGroupChallengeRankInfo::DanceGroupChallengeRankInfo()
    : m_nRankType( EDanceGroupChallengeRankType_ScoreRank )
    , m_nRoleID( 0 )
    , m_nRank( 0 )
    , m_nScore( 0 )
    , m_nMyInspire( 0 )
{
}

DanceGroupChallengeRankInfo::~DanceGroupChallengeRankInfo()
{
}

bool DanceGroupChallengeRankInfo::doEncode( CParamPool& IOBuff ) const
{
    IOBuff << m_nRankType << m_nRoleID << stringw( m_strRoleName ) 
        << m_nRank << m_nScore << m_nMyInspire;

    return true;
}

bool DanceGroupChallengeRankInfo::doDecode( CParamPool& IOBuff )
{
    stringw strRoleName;
    IOBuff >> m_nRankType >> m_nRoleID >> strRoleName 
        >> m_nRank >> m_nScore >> m_nMyInspire;
    strRoleName.swap( m_strRoleName );

    return true;
}


DanceGroupChallengeScoreInfo::DanceGroupChallengeScoreInfo()
    : m_nTargetType( EDanceGroupChallengeTargetType_Score )
    , m_nTargetScore( 0 )
    , m_nCurScore( 0 )
    , m_nMyTargetScore( 0 )
    , m_nMyScore( 0 )
    , m_nAdditional( 0 )
    , m_nGrade( RoomDanceGrade_D )
    , m_nPerfectCount( 0 )
    , m_nCoolCount( 0 )
    , m_nGoodCount( 0 )
    , m_nBadCount( 0 )
    , m_nMissCount( 0 )
    , m_nSpecialCount( 0 )
{
}

DanceGroupChallengeScoreInfo::~DanceGroupChallengeScoreInfo()
{
}

bool DanceGroupChallengeScoreInfo::doEncode( CParamPool& IOBuff ) const
{
    IOBuff << m_nTargetType << m_nTargetScore << m_nCurScore << m_nMyTargetScore 
        << m_nMyScore << m_nAdditional << m_nGrade << m_nPerfectCount << m_nCoolCount 
        << m_nGoodCount << m_nBadCount << m_nMissCount << m_nSpecialCount;

    return true;
}

bool DanceGroupChallengeScoreInfo::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nTargetType >> m_nTargetScore >> m_nCurScore >> m_nMyTargetScore 
        >> m_nMyScore >> m_nAdditional >> m_nGrade >> m_nPerfectCount >> m_nCoolCount 
        >> m_nGoodCount >> m_nBadCount >> m_nMissCount >> m_nSpecialCount;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_EnterChallengeRes::GameMsg_G2S_EnterChallengeRes()
    : GameMsg_Base( MSG_G2S_EnterChallengeRes )
    , m_nResult( EEnterChallengeRes_Success )
    , m_nCurStorey( MIN_CHALLENGE_STOREY )
    , m_nCurStoreyState( EDanceGroupChallengeStoreyState_NotStart )
    , m_nMyInspire( 0 )
{
}

GameMsg_G2S_EnterChallengeRes::~GameMsg_G2S_EnterChallengeRes()
{
}

bool GameMsg_G2S_EnterChallengeRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_nCurStorey << m_nCurStoreyState << m_nMyInspire;

    return true;
}

bool GameMsg_G2S_EnterChallengeRes::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nResult >> m_nCurStorey >> m_nCurStoreyState >> m_nMyInspire;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_GetChallengeStoreyInfoRes::GameMsg_G2S_GetChallengeStoreyInfoRes()
    : GameMsg_Base( MSG_G2S_GetChallengeStoreyInfoRes )
    , m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nStoreyState( EDanceGroupChallengeStoreyState_NotStart )
    , m_nScene( RAN_SCENE_ID )
    , m_nMusic( RAN_MUSIC_ID )
    , m_nMusicMode( MusicMode_None )
    , m_nMusicLevel( MusicLevel_None )
    , m_nTargetScore( 0 )
    , m_nCurrentScore( 0 )
    , m_nAdditional( 0 )
    , m_nMyInspire( 0 )
{
}

GameMsg_G2S_GetChallengeStoreyInfoRes::~GameMsg_G2S_GetChallengeStoreyInfoRes()
{
}

bool GameMsg_G2S_GetChallengeStoreyInfoRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nStorey << m_nStoreyState << m_nScene << m_nMusic << m_nMusicMode 
        << m_nMusicLevel << m_nTargetScore << m_nCurrentScore << m_nAdditional << m_nMyInspire;

    return true;
}

bool GameMsg_G2S_GetChallengeStoreyInfoRes::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nStorey >> m_nStoreyState >> m_nScene >> m_nMusic >> m_nMusicMode >> 
        m_nMusicLevel >> m_nTargetScore >> m_nCurrentScore >> m_nAdditional >> m_nMyInspire;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_GetChallengeRankListRes::GameMsg_G2S_GetChallengeRankListRes()
    : GameMsg_Base( MSG_G2S_GetChallengeRankListRes )
    , m_nRankType( EDanceGroupChallengeRankType_ScoreRank )
    , m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nCurPage( 0 )
    , m_nTotalCount( 0 )
{
}

GameMsg_G2S_GetChallengeRankListRes::~GameMsg_G2S_GetChallengeRankListRes()
{
}

bool GameMsg_G2S_GetChallengeRankListRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nRankType << m_nStorey << m_nCurPage << m_nTotalCount << m_listInfo;

    return true;
}

bool GameMsg_G2S_GetChallengeRankListRes::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nRankType >> m_nStorey >> m_nCurPage >> m_nTotalCount >> m_listInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_ChallengeInspireRes::GameMsg_G2S_ChallengeInspireRes()
    : GameMsg_Base( MSG_G2S_ChallengeInspireRes )
    , m_nResult( EChallengeInspireRes_Success )
    , m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nAdditional( 0 )
    , m_nMyInspire( 0 )
{
}

GameMsg_G2S_ChallengeInspireRes::~GameMsg_G2S_ChallengeInspireRes()
{
}

bool GameMsg_G2S_ChallengeInspireRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_nStorey << m_nAdditional << m_nMyInspire;

    return true;
}

bool GameMsg_G2S_ChallengeInspireRes::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nResult >> m_nStorey >> m_nAdditional >> m_nMyInspire;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_OpenChallengeStoreyRes::GameMsg_G2S_OpenChallengeStoreyRes()
    : GameMsg_Base( MSG_G2S_OpenChallengeStoreyRes )
    , m_nResult( EOpenChallengeStoreyRes_Success )
    , m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nScene( RAN_SCENE_ID )
    , m_nMusic( RAN_MUSIC_ID )
    , m_nMusicMode( MusicMode_None )
    , m_nMusicLevel( MusicLevel_None )
{
}

GameMsg_G2S_OpenChallengeStoreyRes::~GameMsg_G2S_OpenChallengeStoreyRes()
{
}

bool GameMsg_G2S_OpenChallengeStoreyRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_nStorey << m_nScene 
        << m_nMusic << m_nMusicMode << m_nMusicLevel;

    return true;
}

bool GameMsg_G2S_OpenChallengeStoreyRes::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nResult >> m_nStorey >> m_nScene 
        >> m_nMusic >> m_nMusicMode >> m_nMusicLevel;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_PrepareChallengeRes::GameMsg_G2S_PrepareChallengeRes()
    : GameMsg_Base( MSG_G2S_PrepareChallengeRes )
    , m_nResult( EPrepareChallengeRes_Success )
    , m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nStoreyState( EDanceGroupChallengeStoreyState_NotStart )
    , m_nScene( RAN_SCENE_ID )
    , m_nMusic( RAN_MUSIC_ID )
    , m_nMusicMode( MusicMode_None )
    , m_nMusicLevel( MusicLevel_None )
    , m_nTargetScore( 0 )
    , m_nCurrentScore( 0 )
    , m_nAdditional( 0 )
{
}

GameMsg_G2S_PrepareChallengeRes::~GameMsg_G2S_PrepareChallengeRes()
{
}

bool GameMsg_G2S_PrepareChallengeRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_nStorey << m_nStoreyState << m_nScene << m_nMusic 
        << m_nMusicMode << m_nMusicLevel << m_nTargetScore << m_nCurrentScore << m_nAdditional;

    return true;
}

bool GameMsg_G2S_PrepareChallengeRes::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nResult >> m_nStorey >> m_nStoreyState >> m_nScene >> m_nMusic 
        >> m_nMusicMode >> m_nMusicLevel >> m_nTargetScore >> m_nCurrentScore >> m_nAdditional;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_ReportChallengeScoreRes::GameMsg_G2S_ReportChallengeScoreRes()
    : GameMsg_Base( MSG_G2S_ReportChallengeScoreRes )
    , m_nResult( EChallengeEndRes_Success )
{
}

GameMsg_G2S_ReportChallengeScoreRes::~GameMsg_G2S_ReportChallengeScoreRes()
{
}

bool GameMsg_G2S_ReportChallengeScoreRes::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nResult << m_scoreInfo;

    return true;
}

bool GameMsg_G2S_ReportChallengeScoreRes::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nResult >> m_scoreInfo;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_ChallengeStoreyFinishedNotice::GameMsg_G2S_ChallengeStoreyFinishedNotice()
    : GameMsg_Base( MSG_G2S_ChallengeStoreyFinishedNotice )
    , m_nNextStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_G2S_ChallengeStoreyFinishedNotice::~GameMsg_G2S_ChallengeStoreyFinishedNotice()
{
}

bool GameMsg_G2S_ChallengeStoreyFinishedNotice::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nNextStorey;

    return true;
}

bool GameMsg_G2S_ChallengeStoreyFinishedNotice::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nNextStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_ChallengeRewardNotice::GameMsg_G2S_ChallengeRewardNotice()
    : GameMsg_Base( MSG_G2S_ChallengeRewardNotice )
    , m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nRanking( 0 )
{
}

GameMsg_G2S_ChallengeRewardNotice::~GameMsg_G2S_ChallengeRewardNotice()
{
}

bool GameMsg_G2S_ChallengeRewardNotice::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nStorey << m_nRanking;

    return true;
}

bool GameMsg_G2S_ChallengeRewardNotice::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nStorey >> m_nRanking;

    return true;
}

