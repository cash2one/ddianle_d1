#include "GameMsg_C2S_DanceGroupChallenge.h"
#include "DanceGroupMsgDef.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DanceGroupInfoDef.h"


GameMsg_C2S_RequestEnterChallenge::GameMsg_C2S_RequestEnterChallenge()
    : GameMsg_Base( MSG_C2S_RequestEnterChallenge )
{
}

GameMsg_C2S_RequestEnterChallenge::~GameMsg_C2S_RequestEnterChallenge()
{
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestChallengeStoreyList::GameMsg_C2S_RequestChallengeStoreyList()
    : GameMsg_Base( MSG_C2S_RequestChallengeStoreyList )
    , m_nRequestPage( 0 )
    , m_nRequestCount( 0 )
{
}

GameMsg_C2S_RequestChallengeStoreyList::~GameMsg_C2S_RequestChallengeStoreyList()
{
}

bool GameMsg_C2S_RequestChallengeStoreyList::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nRequestPage >> m_nRequestCount;

    if ( m_nRequestPage < 0 )
        m_nRequestPage = 0;
    if ( m_nRequestCount < 0 )
        m_nRequestCount = 0;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestChallengeStoreyInfo::GameMsg_C2S_RequestChallengeStoreyInfo()
    : GameMsg_Base( MSG_C2S_RequestChallengeStoreyInfo )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_C2S_RequestChallengeStoreyInfo::~GameMsg_C2S_RequestChallengeStoreyInfo()
{
}

bool GameMsg_C2S_RequestChallengeStoreyInfo::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nRequestStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestChallengeRankList::GameMsg_C2S_RequestChallengeRankList()
    : GameMsg_Base( MSG_C2S_RequestChallengeRankList )
    , m_nRankType( EDanceGroupChallengeRankType_ScoreRank )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
    , m_nRequestPage( 0 )
    , m_nRequestCount( 0 )
{
}

GameMsg_C2S_RequestChallengeRankList::~GameMsg_C2S_RequestChallengeRankList()
{
}

bool GameMsg_C2S_RequestChallengeRankList::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nRankType >> m_nRequestStorey >> m_nRequestPage >> m_nRequestCount;

    if ( m_nRankType >= EDanceGroupChallengeRankType_Max )
        m_nRankType = EDanceGroupChallengeRankType_ScoreRank;
    if ( m_nRequestPage < 0 )
        m_nRequestPage = 0;
    if ( m_nRequestCount < 0 )
        m_nRequestCount = 0;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestGetChallengeReward::GameMsg_C2S_RequestGetChallengeReward()
    : GameMsg_Base( MSG_C2S_RequestGetChallengeReward )
    , m_bGetAll( false )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_C2S_RequestGetChallengeReward::~GameMsg_C2S_RequestGetChallengeReward()
{
}

bool GameMsg_C2S_RequestGetChallengeReward::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_bGetAll >> m_nRequestStorey;

    if ( m_nRequestStorey < MIN_CHALLENGE_STOREY )
        m_nRequestStorey = MIN_CHALLENGE_STOREY;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestChallengeInspire::GameMsg_C2S_RequestChallengeInspire()
    : GameMsg_Base( MSG_C2S_RequestChallengeInspire )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_C2S_RequestChallengeInspire::~GameMsg_C2S_RequestChallengeInspire()
{
}

bool GameMsg_C2S_RequestChallengeInspire::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nRequestStorey;

    if ( m_nRequestStorey < MIN_CHALLENGE_STOREY )
        m_nRequestStorey = MIN_CHALLENGE_STOREY;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestAddChallengeChance::GameMsg_C2S_RequestAddChallengeChance()
    : GameMsg_Base( MSG_C2S_RequestAddChallengeChance )
{
}

GameMsg_C2S_RequestAddChallengeChance::~GameMsg_C2S_RequestAddChallengeChance()
{
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestOpenChallengeStorey::GameMsg_C2S_RequestOpenChallengeStorey()
    : GameMsg_Base( MSG_C2S_RequestOpenChallengeStorey )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_C2S_RequestOpenChallengeStorey::~GameMsg_C2S_RequestOpenChallengeStorey()
{
}

bool GameMsg_C2S_RequestOpenChallengeStorey::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nRequestStorey;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestPrepareChallenge::GameMsg_C2S_RequestPrepareChallenge()
    : GameMsg_Base( MSG_C2S_RequestPrepareChallenge )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
{
}

GameMsg_C2S_RequestPrepareChallenge::~GameMsg_C2S_RequestPrepareChallenge()
{
}

bool GameMsg_C2S_RequestPrepareChallenge::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nRequestStorey;

    if ( m_nRequestStorey < MIN_CHALLENGE_STOREY )
        m_nRequestStorey = MIN_CHALLENGE_STOREY;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestStartChallenge::GameMsg_C2S_RequestStartChallenge()
    : GameMsg_Base( MSG_C2S_RequestStartChallenge )
    , m_nRequestStorey( MIN_CHALLENGE_STOREY )
    , m_nPhoneOS( OS_None )
{
}

GameMsg_C2S_RequestStartChallenge::~GameMsg_C2S_RequestStartChallenge()
{
}

bool GameMsg_C2S_RequestStartChallenge::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nRequestStorey >> m_nPhoneOS;

    if ( m_nPhoneOS >= OS_Max )
        m_nPhoneOS = OS_None;

    return true;
}


