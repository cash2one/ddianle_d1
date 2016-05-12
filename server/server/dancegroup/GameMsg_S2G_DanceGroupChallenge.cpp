#include "GameMsg_S2G_DanceGroupChallenge.h"
#include "DanceGroupMsgDef.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DanceGroupInfoDef.h"


GameMsg_S2G_EnterChallenge::GameMsg_S2G_EnterChallenge()
    : GameMsg_Base( MSG_S2G_EnterChallenge )
    , m_nGroupID( 0 )
{
}

GameMsg_S2G_EnterChallenge::~GameMsg_S2G_EnterChallenge()
{
}

bool GameMsg_S2G_EnterChallenge::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID;

    return true;
}

bool GameMsg_S2G_EnterChallenge::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GetChallengeStoreyInfo::GameMsg_S2G_GetChallengeStoreyInfo()
    : GameMsg_Base( MSG_S2G_GetChallengeStoreyInfo )
    , m_nGroupID( 0 )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_S2G_GetChallengeStoreyInfo::~GameMsg_S2G_GetChallengeStoreyInfo()
{
}

bool GameMsg_S2G_GetChallengeStoreyInfo::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID << m_nRequestStorey;

    return true;
}

bool GameMsg_S2G_GetChallengeStoreyInfo::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID >> m_nRequestStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GetChallengeRankList::GameMsg_S2G_GetChallengeRankList()
    : GameMsg_Base( MSG_S2G_GetChallengeRankList )
    , m_nGroupID( 0 )
    , m_nRankType( EDanceGroupChallengeRankType_ScoreRank )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
    , m_nRequestPage( 0 )
    , m_nRequestCount( 0 )
{
}

GameMsg_S2G_GetChallengeRankList::~GameMsg_S2G_GetChallengeRankList()
{
}

bool GameMsg_S2G_GetChallengeRankList::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID << m_nRankType << m_nRequestStorey << m_nRequestPage << m_nRequestCount;

    return true;
}

bool GameMsg_S2G_GetChallengeRankList::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID >> m_nRankType >> m_nRequestStorey >> m_nRequestPage >> m_nRequestCount;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_ChallengeInspire::GameMsg_S2G_ChallengeInspire()
    : GameMsg_Base( MSG_S2G_ChallengeInspire )
    , m_nGroupID( 0 )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_S2G_ChallengeInspire::~GameMsg_S2G_ChallengeInspire()
{
}

bool GameMsg_S2G_ChallengeInspire::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID << m_nRequestStorey;

    return true;
}

bool GameMsg_S2G_ChallengeInspire::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID >> m_nRequestStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_OpenChallengeStorey::GameMsg_S2G_OpenChallengeStorey()
    : GameMsg_Base( MSG_S2G_OpenChallengeStorey )
    , m_nGroupID( 0 )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
    , m_nScene( RAN_SCENE_ID )
    , m_nMusic( RAN_MUSIC_ID )
    , m_nMusicMode( MusicMode_None )
    , m_nMusicLevel( MusicLevel_None )
    , m_nTarget( 0 )
    , m_nActive( 0 )
{
}

GameMsg_S2G_OpenChallengeStorey::~GameMsg_S2G_OpenChallengeStorey()
{
}

bool GameMsg_S2G_OpenChallengeStorey::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID << m_nRequestStorey << m_nScene 
        << m_nMusic << m_nMusicMode << m_nMusicLevel 
        << m_nTarget << m_nActive;

    return true;
}

bool GameMsg_S2G_OpenChallengeStorey::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID >> m_nRequestStorey >> m_nScene 
        >> m_nMusic >> m_nMusicMode >> m_nMusicLevel 
        >> m_nTarget >> m_nActive;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_PrepareChallenge::GameMsg_S2G_PrepareChallenge()
    : GameMsg_Base( MSG_S2G_PrepareChallenge )
    , m_nGroupID( 0 )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_S2G_PrepareChallenge::~GameMsg_S2G_PrepareChallenge()
{
}

bool GameMsg_S2G_PrepareChallenge::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID << m_nRequestStorey;

    return true;
}

bool GameMsg_S2G_PrepareChallenge::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID >> m_nRequestStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_ReportChallengeScore::GameMsg_S2G_ReportChallengeScore()
    : GameMsg_Base( MSG_S2G_ReportChallengeScore )
    , m_nGroupID( 0 )
    , m_nStartTime( 0 )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_S2G_ReportChallengeScore::~GameMsg_S2G_ReportChallengeScore()
{
}

bool GameMsg_S2G_ReportChallengeScore::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID << m_nStartTime << m_nRequestStorey << m_scoreInfo;

    return true;
}

bool GameMsg_S2G_ReportChallengeScore::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID >> m_nStartTime >> m_nRequestStorey >> m_scoreInfo;

    return true;
}

