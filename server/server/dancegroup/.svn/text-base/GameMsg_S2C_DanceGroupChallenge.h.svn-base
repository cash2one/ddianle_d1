/*
 * GameMsg_S2C_DanceGroupChallenge.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-10-22 11:09:00
 */

#ifndef __GAMEMSG_S2C_DANCEGROUPCHALLENGE_H__
#define __GAMEMSG_S2C_DANCEGROUPCHALLENGE_H__

#include "GameMsg_G2S_DanceGroupChallenge.h"
#include "../Dungeon/GameMsg_Dungeon.h"

class DanceGroupChallengeStoreySimpleInfo
{
public:
    DanceGroupChallengeStoreySimpleInfo();
    ~DanceGroupChallengeStoreySimpleInfo();

    bool doEncode( CParamPool& IOBuff ) const;

public:
    int m_nStorey;
    unsigned char m_nStoreyState;
    std::string m_strAtlas;
    std::string m_strIcon;
};

class DanceGroupChallengeStoreyWholeInfo
{
public:
    DanceGroupChallengeStoreyWholeInfo();
    ~DanceGroupChallengeStoreyWholeInfo();

    bool doEncode( CParamPool& IOBuff ) const;

public:
    int m_nStorey;
    unsigned char m_nStoreyState;
    unsigned int m_nActive;
    unsigned char m_nMusicLevel;
    unsigned char m_nMusicMode;
    unsigned short m_nMusic;
    unsigned char m_nTargetType;
    unsigned int m_nTargetScore;
    unsigned int m_nCurrentScore;
    unsigned short m_nAdditional;
    unsigned short m_nMaxChance;
    unsigned short m_nCurChance;
    std::list<CItem> m_listReward;
};

class DanceGroupChallengeRoomInfo
{
public:
    DanceGroupChallengeRoomInfo();
    ~DanceGroupChallengeRoomInfo();

    void doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nRoomType;
    unsigned char m_nScene;
    unsigned short m_nMusic;
    unsigned char m_nMusicMode;
    unsigned char m_nMusicLevel;
    unsigned char m_nSelfRoomType;
    unsigned char m_nTeamMode;
    bool m_bIsHost;
    std::list<DungeonPlayerInfo> m_listDancer;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_SyncChallengeConfig : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncChallengeConfig)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    int m_nTotalStorey;
    unsigned short m_nOpenLevel;
    unsigned char m_nInspireCostType;
    unsigned int m_nInspireCostAmount;
    std::string m_strHelp;
    std::list<int> m_listRewardStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RequestEnterChallengeRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestEnterChallengeRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    unsigned int m_nOpenTime;
    int m_nCurrentStorey;
    unsigned char m_nChanceCostType;
    unsigned int m_nChanceCostAmount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RequestChallengeStoreyListRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestChallengeStoreyListRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    int m_nCurPage;
    int m_nTotalCount;
    std::list<DanceGroupChallengeStoreySimpleInfo> m_listInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RequestChallengeStoreyInfoRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestChallengeStoreyInfoRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    DanceGroupChallengeStoreyWholeInfo m_wholeInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RequestChallengeRankListRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestChallengeRankListRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nRankType;
    int m_nStorey;
    int m_nCurPage;
    int m_nTotalCount;
    std::list<DanceGroupChallengeRankInfo> m_listInfo;
};

//////////////////////////////////////////////////////////////////////////

enum EGetChallengeRewardRes
{
    EGetChallengeRewardRes_Success = 0,
    EGetChallengeRewardRes_NotOpen,
    EGetChallengeRewardRes_NotInGroup,
    EGetChallengeRewardRes_NoRewardInfo,
};

class GameMsg_S2C_RequestGetChallengeRewardRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestGetChallengeRewardRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    bool m_bGetAll;
    int m_nRequestStorey;
    std::list<CItem> m_listReward;
};

class GameMsg_S2C_ChallengeRewardNotice : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_ChallengeRewardNotice)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    int m_nStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RequestChallengeInspireRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestChallengeInspireRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    unsigned short m_nAdditional;
};

//////////////////////////////////////////////////////////////////////////

enum EAddChallengeChanceRes
{
    EAddChallengeChanceRes_Success = 0,
    EAddChallengeChanceRes_NotOpen,
    EAddChallengeChanceRes_NotEnoughMoney,
};

class GameMsg_S2C_RequestAddChallengeChanceRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestAddChallengeChanceRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    unsigned short m_nMaxChance;
    unsigned short m_nCurChance;
    unsigned char m_nNextCostType;
    unsigned int m_nNextCostAmount;
};

class GameMsg_S2C_RefreshChallengeChanceNotice : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RefreshChallengeChanceNotice)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned short m_nMaxChance;
    unsigned short m_nCurChance;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RequestOpenChallengeStoreyRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestOpenChallengeStoreyRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    DanceGroupChallengeStoreyWholeInfo m_wholeInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RequestPrepareChallengeRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestPrepareChallengeRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    DanceGroupChallengeStoreyWholeInfo m_wholeInfo;
};

//////////////////////////////////////////////////////////////////////////

enum EStartChallengeRes
{
    EStartChallengeRes_Success = 0,
    EStartChallengeRes_Initializing,
    EStartChallengeRes_NotOpen,
    EStartChallengeRes_NotInGroup,
    EStartChallengeRes_StoreyNotStart,
    EStartChallengeRes_StoreyFinished,
    EStartChallengeRes_NotEnoughChance,
    EStartChallengeRes_NewMember,
    EStartChallengeRes_LoadStageFailed,
    EStartChallengeRes_AlreadyStarted,
    EStartChallengeRes_InRoom,
    EStartChallengeRes_InCoupleRoom,
    EStartChallengeRes_InPhotoRoom,
    EStartChallengeRes_InAmuseRoom,
    EStartChallengeRes_InCeremonyRoom,
};

class GameMsg_S2C_RequestStartChallengeRes :public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestStartChallengeRes)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    std::string m_strCheckKey;
    std::string m_strStage;
    DanceGroupChallengeRoomInfo m_roomInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChallengeStageEnd : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_ChallengeStageEnd)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    DanceGroupChallengeScoreInfo m_scoreInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChallengeStoreyFinishedNotice : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_ChallengeStoreyFinishedNotice)

public:
    bool doEncode( CParamPool& IOBuff );

public:
    int m_nStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChallengeClosedNotice : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_ChallengeClosedNotice)

public:
    bool doEncode(CParamPool &IOBuff) { return true; }
};

#endif // __GAMEMSG_S2C_DANCEGROUPCHALLENGE_H__

