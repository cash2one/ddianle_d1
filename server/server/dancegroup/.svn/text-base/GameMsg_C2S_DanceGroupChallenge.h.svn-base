/*
 * GameMsg_C2S_DanceGroupChallenge.h
 * Description: ŒËÕ≈ ‘¡∂
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-10-22 10:59:11
 */

#ifndef __GAMEMSG_C2S_DANCEGROUPCHALLENGE_H__
#define __GAMEMSG_C2S_DANCEGROUPCHALLENGE_H__

#include "../../socket/GameMsg_Base.h"

class GameMsg_C2S_RequestEnterChallenge : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestEnterChallenge)

public:
    bool doDecode( CParamPool& IOBuff ) { return true; }
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestChallengeStoreyList : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestChallengeStoreyList)

public:
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nRequestPage;
    int m_nRequestCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestChallengeStoreyInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestChallengeStoreyInfo)

public:
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nRequestStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestChallengeRankList : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestChallengeRankList)

public:
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nRankType;
    int m_nRequestStorey;
    int m_nRequestPage;
    int m_nRequestCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestGetChallengeReward : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestGetChallengeReward)

public:
    bool doDecode( CParamPool& IOBuff );

public:
    bool m_bGetAll;
    int m_nRequestStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestChallengeInspire : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestChallengeInspire)

public:
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nRequestStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestAddChallengeChance : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestAddChallengeChance)

public:
    bool doDecode( CParamPool& IOBuff ) { return true; }
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestOpenChallengeStorey : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestOpenChallengeStorey)

public:
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nRequestStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestPrepareChallenge : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestPrepareChallenge)

public:
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nRequestStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestStartChallenge : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestStartChallenge)

public:
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nRequestStorey;
    unsigned char m_nPhoneOS;
};

#endif // __GAMEMSG_C2S_DANCEGROUPCHALLENGE_H__

