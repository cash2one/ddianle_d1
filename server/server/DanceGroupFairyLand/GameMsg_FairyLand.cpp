#include "GameMsg_FairyLand.h"


GameMsg_G2S_AnnouncementFairyLandEnd::GameMsg_G2S_AnnouncementFairyLandEnd()
    : GameMsg_Base(MSG_G2S_FAIRYLAND_ANNOUNCEMENT_END)
{

}

GameMsg_G2S_AnnouncementFairyLandEnd::~GameMsg_G2S_AnnouncementFairyLandEnd()
{

}


//////////////////////////////////////////////////////////////////////////
GameMsg_G2S_FairyLandJoinAndRandomReward::GameMsg_G2S_FairyLandJoinAndRandomReward()
    :GameMsg_Base(MSG_G2S_FAIRYLAND_JOIN_RANDOM_REWARD)
    , m_nChapter(0)
    , m_nIsSendJoin(false)
{

}

GameMsg_G2S_FairyLandJoinAndRandomReward::~GameMsg_G2S_FairyLandJoinAndRandomReward()
{

}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_FairyLandReward::GameMsg_S2C_FairyLandReward()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_REWARD)
    , m_nBindMb(0)
    , m_nMoney(0)
{

}

GameMsg_S2C_FairyLandReward::~GameMsg_S2C_FairyLandReward()
{}

bool GameMsg_S2C_FairyLandReward::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nRewardType);
    IOBuff.AddInt(m_nBindMb);
    IOBuff.AddInt(m_nMoney);
    IOBuff.AddUShort((unsigned short)m_listReward.size());

    for (std::list<CItem>::iterator it = m_listReward.begin(); it != m_listReward.end(); ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

bool GameMsg_S2C_FairyLandReward::doDecode(CParamPool &IOBuff)
{
    m_nRewardType = IOBuff.GetUChar();
    m_nBindMb = IOBuff.GetInt();
    m_nMoney = IOBuff.GetInt();

    unsigned short uLen = IOBuff.GetUShort();
    for (unsigned short i = 0; i < uLen; ++i)
    {
        CItem u;
        u.doDecode(IOBuff);
        m_listReward.push_back(u);
    }

    return true;
}

GameMsg_S2C_FairyLandRandomReward::GameMsg_S2C_FairyLandRandomReward()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_END_RANDOMREWARD)
    , m_nBindMb(0)
    , m_nMoney(0)
{}

GameMsg_S2C_FairyLandRandomReward::~GameMsg_S2C_FairyLandRandomReward()
{}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_FairyLandCheckIsInYesterdatRank::GameMsg_C2S_FairyLandCheckIsInYesterdatRank()
    :GameMsg_Base(MSG_C2S_FAIRYLAND_CHECKISINRANK)
{}
GameMsg_C2S_FairyLandCheckIsInYesterdatRank::~GameMsg_C2S_FairyLandCheckIsInYesterdatRank()
{}

GameMsg_S2C_FairyLandCheckIsInYesterdatRank::GameMsg_S2C_FairyLandCheckIsInYesterdatRank()
    : GameMsg_Base(MSG_S2C_FAIRYLAND_CHECKISINRANK)
    , m_nYesterdayScore(0)
{}

GameMsg_S2C_FairyLandCheckIsInYesterdatRank::~GameMsg_S2C_FairyLandCheckIsInYesterdatRank()
{}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_FairyLandStorageboxInfo::GameMsg_C2S_FairyLandStorageboxInfo()
    :GameMsg_Base(MSG_C2S_FAIRYLAND_STORAGEBOX_INFO)
{}

GameMsg_C2S_FairyLandStorageboxInfo::~GameMsg_C2S_FairyLandStorageboxInfo()
{}


GameMsg_S2C_FairyLandStorageboxInfo::GameMsg_S2C_FairyLandStorageboxInfo()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_STORAGEBOX_INFO)
    , m_nTotalStorageBoxItemsCount(0)
{}

GameMsg_S2C_FairyLandStorageboxInfo::~GameMsg_S2C_FairyLandStorageboxInfo()
{}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_FairyLandRankConfig::GameMsg_C2S_FairyLandRankConfig()
    :GameMsg_Base(MSG_C2S_FAIRYLAND_RANK_CONFIG)
{}

GameMsg_C2S_FairyLandRankConfig::~GameMsg_C2S_FairyLandRankConfig()
{}

GameMsg_S2C_FairyLandRankConfig::GameMsg_S2C_FairyLandRankConfig()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_RANK_CONFIG)
    , m_strRankRule("")
{}

GameMsg_S2C_FairyLandRankConfig::~GameMsg_S2C_FairyLandRankConfig()
{}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_FairyLandConfig::GameMsg_C2S_FairyLandConfig()
    :GameMsg_Base(MSG_C2S_FAIRYLAND_CONFIG)
{}

GameMsg_C2S_FairyLandConfig::~GameMsg_C2S_FairyLandConfig()
{}

GameMsg_S2C_FairyLandConfig::GameMsg_S2C_FairyLandConfig()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_CONFIG)
    , m_nTotalChatper(0)
    , m_strStorageBoxRule("")
    , m_nBlessCost(0)
    , m_nDefaultChallengeCount(0)
{}

GameMsg_S2C_FairyLandConfig::~GameMsg_S2C_FairyLandConfig()
{}
//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_FairyLandDanceEndInfo::GameMsg_S2C_FairyLandDanceEndInfo()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_END_DANCE_INFO)
    , m_nChapterScore(0)
    , m_nChapterTotalScore(0)
{}

GameMsg_S2C_FairyLandDanceEndInfo::~GameMsg_S2C_FairyLandDanceEndInfo()
{}

bool GameMsg_S2C_FairyLandDanceEndInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nChapterScore);
    IOBuff.AddInt(m_nChapterTotalScore);
    IOBuff.AddInt(m_nBlessBuf);

    IOBuff.AddUShort((unsigned short)m_listScore.size());
    for (LSIT it = m_listScore.begin(); it != m_listScore.end(); ++it)
    {
        IOBuff.AddInt(it->m_nRoleId);
        IOBuff.AddInt(it->m_nScore);
        IOBuff.AddInt(it->m_nVipBuf);
        IOBuff.AddInt(it->m_nGrade);
    }

    return true;
}

bool GameMsg_S2C_FairyLandDanceEndInfo::doDecode(CParamPool &IOBuff)
{
    m_nChapterScore = IOBuff.GetInt();
    m_nChapterTotalScore = IOBuff.GetInt();
    m_nBlessBuf = IOBuff.GetInt();

    unsigned short nLen = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nLen; ++i)
    {
        SDancerScore s;
        s.m_nRoleId = IOBuff.GetInt();
        s.m_nScore = IOBuff.GetInt();
        s.m_nVipBuf = IOBuff.GetInt();
        s.m_nGrade = IOBuff.GetInt();
        m_listScore.push_back(s);
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_FairyLandChapterInfo::GameMsg_C2S_FairyLandChapterInfo()
    :GameMsg_Base(MSG_C2S_FAIRYLAND_CHAPTER_INFO)
    , m_nChatper(0)
{}

GameMsg_C2S_FairyLandChapterInfo::~GameMsg_C2S_FairyLandChapterInfo()
{}


GameMsg_S2C_FairyLandChapterInfo::GameMsg_S2C_FairyLandChapterInfo()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_CHAPTER_INFO)
    , m_nChatper(0)
    , m_nDanceType(0)
    , m_nMusicId(0)
    , m_nDifficultyLvl(0)
{}

GameMsg_S2C_FairyLandChapterInfo::~GameMsg_S2C_FairyLandChapterInfo()
{}
//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_FairyLandRankInfo::GameMsg_C2S_FairyLandRankInfo()
    :GameMsg_Base(MSG_C2S_FAIRYLAND_RANK)
    , m_nType(0)
{}

GameMsg_C2S_FairyLandRankInfo::~GameMsg_C2S_FairyLandRankInfo()
{}

GameMsg_S2G_FairyLandRankInfo::GameMsg_S2G_FairyLandRankInfo()
    :GameMsg_Base(MSG_S2G_FAIRYLAND_RANK)
    , m_nType(0)
    , m_nRoleId(0)
    , m_nGroupId(0)
{}

GameMsg_S2G_FairyLandRankInfo::~GameMsg_S2G_FairyLandRankInfo()
{}

GameMsg_S2C_FairyLandRankInfo::GameMsg_S2C_FairyLandRankInfo()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_RANK)
    , m_nType(0)
    , m_bIsComplete(false)
{}

GameMsg_S2C_FairyLandRankInfo::~GameMsg_S2C_FairyLandRankInfo()
{}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_FairyLandGetYesterdayRankReward::GameMsg_C2S_FairyLandGetYesterdayRankReward()
    :GameMsg_Base(MSG_C2S_FAIRYLAND_GET_YESTERDAY_RANK_REWARD)
{}

GameMsg_C2S_FairyLandGetYesterdayRankReward::~GameMsg_C2S_FairyLandGetYesterdayRankReward()
{}


GameMsg_G2S_FairyLandGetYesterdayRankReward::GameMsg_G2S_FairyLandGetYesterdayRankReward()
    :GameMsg_Base(MSG_G2S_FAIRYLAND_GET_YESTERDAY_RANK_REWARD)
    , m_nRewardId(0)
{}

GameMsg_G2S_FairyLandGetYesterdayRankReward::~GameMsg_G2S_FairyLandGetYesterdayRankReward()
{}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_FairyLandOpenBless::GameMsg_C2S_FairyLandOpenBless()
    : GameMsg_Base(MSG_C2S_FAIRYLAND_OpenBless)
{

}
GameMsg_C2S_FairyLandOpenBless::~GameMsg_C2S_FairyLandOpenBless()
{

}

bool GameMsg_C2S_FairyLandOpenBless::doEncode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_C2S_FairyLandOpenBless::doDecode(CParamPool &IOBuff)
{
    return true;
}


GameMsg_S2C_FairyLandOpenBless::GameMsg_S2C_FairyLandOpenBless()
    : GameMsg_Base(MSG_S2C_FAIRYLAND_OpenBless)
    , m_nRetCode(Sucess)
    , m_nLeftTime(0)
{

}
GameMsg_S2C_FairyLandOpenBless::~GameMsg_S2C_FairyLandOpenBless()
{

}

bool GameMsg_S2C_FairyLandOpenBless::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nRetCode);
    IOBuff.AddUInt(m_nLeftTime);
    IOBuff.AddStringW(m_strLastBlessRole);

    return true;
}

bool GameMsg_S2C_FairyLandOpenBless::doDecode(CParamPool &IOBuff)
{
    m_nRetCode = IOBuff.GetUChar();
    m_nLeftTime = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strLastBlessRole);

    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_FairyLandBless::GameMsg_C2S_FairyLandBless()
    :GameMsg_Base(MSG_C2S_FAIRYLAND_BLESS)
{}

GameMsg_C2S_FairyLandBless::~GameMsg_C2S_FairyLandBless()
{}

GameMsg_S2C_FairyLandBless::GameMsg_S2C_FairyLandBless()
    :GameMsg_Base(MSG_S2C_FAIRYLAND_BLESS)
    , m_nFlag(0)
{}

GameMsg_S2C_FairyLandBless::~GameMsg_S2C_FairyLandBless()
{}


GameMsg_S2C_FairyLandChanllegeTimesNotify::GameMsg_S2C_FairyLandChanllegeTimesNotify()
    : GameMsg_Base(MSG_S2C_FAIRYLandChanllegeTimesNotify)
    , m_nChanllegeTimes(0)
{

}

GameMsg_S2C_FairyLandChanllegeTimesNotify::~GameMsg_S2C_FairyLandChanllegeTimesNotify(){}

bool GameMsg_S2C_FairyLandChanllegeTimesNotify::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nChanllegeTimes);
    return true;
}
bool GameMsg_S2C_FairyLandChanllegeTimesNotify::doDecode(CParamPool &IOBuff)
{
    m_nChanllegeTimes = IOBuff.GetUShort();
    return true;
}


GameMsg_S2C_FairyLandBlessNotify::GameMsg_S2C_FairyLandBlessNotify()
    : GameMsg_Base(MSG_S2C_FAIRYLandBlessNotify)
    , m_nBelss(0)
{

}

GameMsg_S2C_FairyLandBlessNotify::~GameMsg_S2C_FairyLandBlessNotify(){}

bool GameMsg_S2C_FairyLandBlessNotify::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nBelss);
    return true;
}
bool GameMsg_S2C_FairyLandBlessNotify::doDecode(CParamPool &IOBuff)
{
    m_nBelss = IOBuff.GetUChar();
    return true;
}


GameMsg_S2C_FairyLandCurrentChapterNotify::GameMsg_S2C_FairyLandCurrentChapterNotify()
    : GameMsg_Base(MSG_S2C_FAIRYLandCurrentChapterNotify)
    , m_nCurrentChapter(0)
    , m_nTotalScore()
{

}

GameMsg_S2C_FairyLandCurrentChapterNotify::~GameMsg_S2C_FairyLandCurrentChapterNotify()
{}

bool GameMsg_S2C_FairyLandCurrentChapterNotify::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nCurrentChapter);
    IOBuff.AddUInt(m_nTotalScore);

    return true;
}

bool GameMsg_S2C_FairyLandCurrentChapterNotify::doDecode(CParamPool &IOBuff)
{
    m_nCurrentChapter = IOBuff.GetUChar();
    m_nTotalScore = IOBuff.GetUInt();

    return true;
}

GameMsg_S2C_FairyLandChapterScoreNotify::GameMsg_S2C_FairyLandChapterScoreNotify()
    : GameMsg_Base(MSG_S2C_FAIRYLandChapterScoreNotify)
    , m_nScore(0)
{

}

GameMsg_S2C_FairyLandChapterScoreNotify::~GameMsg_S2C_FairyLandChapterScoreNotify()
{}

bool GameMsg_S2C_FairyLandChapterScoreNotify::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nScore);
    return  true;
}


bool GameMsg_S2C_FairyLandChapterScoreNotify::doDecode(CParamPool &IOBuff)
{
    m_nScore = IOBuff.GetUInt();
    return true;
}


GameMsg_S2C_FairyLandStateNotify::GameMsg_S2C_FairyLandStateNotify()
    : GameMsg_Base(MSG_S2C_FAIRYLandStateNotify)
    , m_nState(0)
    , m_nCurrentChapter(0)
    , m_nTotalScore(0)
{

}

GameMsg_S2C_FairyLandStateNotify::~GameMsg_S2C_FairyLandStateNotify(){}

bool GameMsg_S2C_FairyLandStateNotify::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nState);
    if (EOpen == m_nState)
    {
        IOBuff.AddUChar(m_nCurrentChapter);
        IOBuff.AddUInt(m_nTotalScore);
    }

    return true;
}

bool GameMsg_S2C_FairyLandStateNotify::doDecode(CParamPool &IOBuff)
{
    m_nState = IOBuff.GetUChar();
    if (EOpen == m_nState)
    {
        m_nCurrentChapter = IOBuff.GetUChar();
        m_nTotalScore = IOBuff.GetUInt();
    }

    return true;
}



////////////////////////////////////////////////////////////////////////// 
// fairyland room list:
GameMsg_C2S_GetFairyLandRoomList::GameMsg_C2S_GetFairyLandRoomList(void)
    : GameMsg_Base(MSG_C2S_GetFairyLandRoomList)
    , m_nPage(0)
{

}
GameMsg_C2S_GetFairyLandRoomList::~GameMsg_C2S_GetFairyLandRoomList(void)
{

}

bool GameMsg_C2S_GetFairyLandRoomList::doDecode(CParamPool& IOBuff)
{
    m_nPage = IOBuff.GetUInt();
    return true;
}

bool GameMsg_C2S_GetFairyLandRoomList::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nPage);

    return true;
}



FairyLandRoomBriefInfo::FairyLandRoomBriefInfo()
    : m_nRoomIndex(0)
    , m_bHasStart(true)
{
}

FairyLandRoomBriefInfo::~FairyLandRoomBriefInfo()
{

}

bool FairyLandRoomBriefInfo::doDecode(CParamPool& IOBuff)
{
    m_nRoomIndex = IOBuff.GetUInt();
    m_bHasStart = IOBuff.GetBool();

    unsigned short nSize = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nSize; ++i)
    {
        FairyLandRoomRole role;
        IOBuff.GetStringW(role.m_strRoleName);
        role.m_nVip = IOBuff.GetUChar();
        role.m_bIsVip = IOBuff.GetBool();
        m_vecRoles.push_back(role);
    }

    return true;
}

bool FairyLandRoomBriefInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoomIndex);
    IOBuff.AddBool(m_bHasStart);

    IOBuff.AddUShort((unsigned short)m_vecRoles.size());
    for (size_t i = 0; i < m_vecRoles.size(); ++i)
    {
        IOBuff.AddStringW(m_vecRoles[i].m_strRoleName);
        IOBuff.AddUChar(m_vecRoles[i].m_nVip);
        IOBuff.AddBool(m_vecRoles[i].m_bIsVip);
    }

    return true;
}


GameMsg_S2C_GetFairyLandRoomListResult::GameMsg_S2C_GetFairyLandRoomListResult(void)
    : GameMsg_Base(MSG_S2C_GetFairyLandRoomListResult)
    , m_nCurPage(0)
    , m_nRoomNum(0)
{

}
GameMsg_S2C_GetFairyLandRoomListResult::~GameMsg_S2C_GetFairyLandRoomListResult(void)
{

}

bool GameMsg_S2C_GetFairyLandRoomListResult::doDecode(CParamPool& IOBuff)
{
    m_nCurPage = IOBuff.GetUInt();
    m_nRoomNum = IOBuff.GetUInt();

    unsigned short s = IOBuff.GetUShort();
    for (unsigned i = 0; i < s; ++i)
    {
        FairyLandRoomBriefInfo b;
        b.doDecode(IOBuff);
        m_vecRoomBriefInfo.push_back(b);
    }

    return true;
}

bool GameMsg_S2C_GetFairyLandRoomListResult::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nCurPage);
    IOBuff.AddUInt(m_nRoomNum);

    IOBuff.AddUShort((unsigned short)m_vecRoomBriefInfo.size());
    for (size_t i = 0; i < m_vecRoomBriefInfo.size(); ++i)
    {
        m_vecRoomBriefInfo[i].doEncode(IOBuff);
    }

    return true;
}

