/*
 * GameMsg_G2S_DanceGroupChallenge.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-10-22 11:30:07
 */

#ifndef __GAMEMSG_G2S_DANCEGROUPCHALLENGE_H__
#define __GAMEMSG_G2S_DANCEGROUPCHALLENGE_H__

#include "../../socket/GameMsg_Base.h"

class DanceGroupChallengeRankInfo
{
public:
    DanceGroupChallengeRankInfo();
    ~DanceGroupChallengeRankInfo();

    bool doEncode( CParamPool& IOBuff ) const;
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nRankType;
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    unsigned short m_nRank;
    unsigned int m_nScore;
    unsigned short m_nMyInspire;
};

class DanceGroupChallengeScoreInfo
{
public:
    DanceGroupChallengeScoreInfo();
    ~DanceGroupChallengeScoreInfo();

    bool doEncode( CParamPool& IOBuff ) const;
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nTargetType;
    unsigned int m_nTargetScore;
    unsigned int m_nCurScore;
    unsigned int m_nMyTargetScore;
    unsigned int m_nMyScore;
    unsigned short m_nAdditional;
    unsigned char m_nGrade;
    int m_nPerfectCount;
    int m_nCoolCount;
    int m_nGoodCount;
    int m_nBadCount;
    int m_nMissCount;
    int m_nSpecialCount;
};

//////////////////////////////////////////////////////////////////////////

enum EEnterChallengeRes
{
    EEnterChallengeRes_Success = 0,
    EEnterChallengeRes_Initializing,
    EEnterChallengeRes_NotOpen,
    EEnterChallengeRes_NotInGroup,
    EEnterChallengeRes_GroupNotExist,
    EEnterChallengeRes_NotEnoughGroupLevel,
    EEnterChallengeRes_NewMember,
};

class GameMsg_G2S_EnterChallengeRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_EnterChallengeRes)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    int m_nCurStorey;
    unsigned char m_nCurStoreyState;
    unsigned short m_nMyInspire;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_GetChallengeStoreyInfoRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_GetChallengeStoreyInfoRes)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nStorey;
    unsigned char m_nStoreyState;
    int m_nScene;
    int m_nMusic;
    int m_nMusicMode;
    int m_nMusicLevel;
    unsigned int m_nTargetScore;
    unsigned int m_nCurrentScore;
    unsigned short m_nAdditional;
    unsigned short m_nMyInspire;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_GetChallengeRankListRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_GetChallengeRankListRes)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nRankType;
    int m_nStorey;
    int m_nCurPage;
    int m_nTotalCount;
    std::list<DanceGroupChallengeRankInfo> m_listInfo;
};

//////////////////////////////////////////////////////////////////////////

enum EChallengeInspireRes
{
    EChallengeInspireRes_Success = 0,
    EChallengeInspireRes_Initializing,
    EChallengeInspireRes_NotOpen,
    EChallengeInspireRes_NotInGroup,
    EChallengeInspireRes_GroupNotExist,
    EChallengeInspireRes_ChallengeNotExists,
    EChallengeInspireRes_StoreyNotExists,
    EChallengeInspireRes_MaxInspireCount,
    EChallengeInspireRes_StoreyNotStart,
    EChallengeInspireRes_StoreyFinished,
    EChallengeInspireRes_NotEnoughMoney,
};

class GameMsg_G2S_ChallengeInspireRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_ChallengeInspireRes)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    int m_nStorey;
    unsigned short m_nAdditional;
    unsigned short m_nMyInspire;
};

//////////////////////////////////////////////////////////////////////////

enum EOpenChallengeStoreyRes
{
    EOpenChallengeStoreyRes_Success = 0,
    EOpenChallengeStoreyRes_Initializing,
    EOpenChallengeStoreyRes_NotOpen,
    EOpenChallengeStoreyRes_NotInGroup,
    EOpenChallengeStoreyRes_GroupNotExist,
    EOpenChallengeStoreyRes_NotEnoughGroupLevel,
    EOpenChallengeStoreyRes_StoreyNotExists,
    EOpenChallengeStoreyRes_NoAuthority,
    EOpenChallengeStoreyRes_NotEnoughActive,
    EOpenChallengeStoreyRes_StoreyOpend,
    EOpenChallengeStoreyRes_PrevNotFinished,
};

class GameMsg_G2S_OpenChallengeStoreyRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_OpenChallengeStoreyRes)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    int m_nStorey;
    int m_nScene;
    int m_nMusic;
    int m_nMusicMode;
    int m_nMusicLevel;
};

//////////////////////////////////////////////////////////////////////////

enum EPrepareChallengeRes
{
    EPrepareChallengeRes_Success = 0,
    EPrepareChallengeRes_Initializing,
    EPrepareChallengeRes_NotOpen,
    EPrepareChallengeRes_NotInGroup,
    EPrepareChallengeRes_GroupNotExist,
    EPrepareChallengeRes_StoreyNotExists,
    EPrepareChallengeRes_StoreyNotStart,
    EPrepareChallengeRes_StoreyFinished,
    EPrepareChallengeRes_NewMember,
};

class GameMsg_G2S_PrepareChallengeRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_PrepareChallengeRes)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    int m_nStorey;
    unsigned char m_nStoreyState;
    int m_nScene;
    int m_nMusic;
    int m_nMusicMode;
    int m_nMusicLevel;
    unsigned int m_nTargetScore;
    unsigned int m_nCurrentScore;
    unsigned short m_nAdditional;
};

//////////////////////////////////////////////////////////////////////////

enum EChallengeEndRes
{
    EChallengeEndRes_Success = 0,
    EChallengeEndRes_Closed,
    EChallengeEndRes_KickOut,
    EChallengeEndRes_GroupDismiss,
};

class GameMsg_G2S_ReportChallengeScoreRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_ReportChallengeScoreRes)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nResult;
    DanceGroupChallengeScoreInfo m_scoreInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_ChallengeStoreyFinishedNotice : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_ChallengeStoreyFinishedNotice)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nNextStorey;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_ChallengeRewardNotice : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_ChallengeRewardNotice)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    int m_nStorey;
    short m_nRanking;
};

#endif // __GAMEMSG_G2S_DANCEGROUPCHALLENGE_H__

