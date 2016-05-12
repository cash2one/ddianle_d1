/*
 * GameMsg_S2G_DanceGroupChallenge.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-10-22 11:19:52
 */

#ifndef __GAMEMSG_S2G_DANCEGROUPCHALLENGE_H__
#define __GAMEMSG_S2G_DANCEGROUPCHALLENGE_H__

#include "../../socket/GameMsg_Base.h"
#include "GameMsg_G2S_DanceGroupChallenge.h"

class GameMsg_S2G_EnterChallenge : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_EnterChallenge)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_GetChallengeStoreyInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_GetChallengeStoreyInfo)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
    int m_nRequestStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_GetChallengeRankList : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_GetChallengeRankList)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
    unsigned char m_nRankType;
    int m_nRequestStorey;
    int m_nRequestPage;
    int m_nRequestCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChallengeInspire : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_ChallengeInspire)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
    int m_nRequestStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_OpenChallengeStorey : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_OpenChallengeStorey)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
    int m_nRequestStorey;
    int m_nScene;
    int m_nMusic;
    int m_nMusicMode;
    int m_nMusicLevel;
    unsigned int m_nTarget;
    unsigned int m_nActive;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_PrepareChallenge : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_PrepareChallenge)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
    int m_nRequestStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ReportChallengeScore : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_ReportChallengeScore)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
    unsigned int m_nStartTime;
    int m_nRequestStorey;
    DanceGroupChallengeScoreInfo m_scoreInfo;
};

#endif // __GAMEMSG_S2G_DANCEGROUPCHALLENGE_H__

