/** GameMsg_FairyLand.h
* Description:
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: lht
* Create time: 2016-02-18 11:48:00
*/


#ifndef __GAMEMSG_FAIRYLADN_H__
#define __GAMEMSG_FAIRYLADN_H__

#include <map>
#include <set>
#include <vector>
#include <string>

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../share/ServerMsgDef.h"
#include "DanceGroupFairyLandCfgMgr.h"

enum eMSM_DANCEGROUPFAIRYLAND
{
    MSG_FAIRYLAND_BEGIN = MSG_TYPE_FAIRYLAND,              //0x2900

    MSG_C2S_FAIRYLAND_CHECKISINRANK,               //检测是否在昨日排行榜里面
    MSG_S2C_FAIRYLAND_CHECKISINRANK,               //返回是否在昨日排行榜里面

//     MSG_C2S_FAIRYLAND_GROUP_INFO,                  //请求秘境舞团数据
//     MSG_S2C_FAIRYLAND_GROUP_INFO,                  //返回秘境舞团数据
    
//     MSG_C2S_FAIRYLAND_FINISHED_GUIDE,              //完成引导
// 
//     MSG_C2S_FAIRYLAND_CHECK_GUIDE,             //检测引导
//     MSG_S2C_FAIRYLAND_CHECK_GUIDE,             //返回检测数据

    MSG_C2S_FAIRYLAND_STORAGEBOX_INFO,             //申请储物箱数据
    MSG_S2C_FAIRYLAND_STORAGEBOX_INFO,             //返回储物箱数据

    MSG_C2S_FAIRYLAND_RANK_CONFIG,             //申请排行榜配置数据
    MSG_S2C_FAIRYLAND_RANK_CONFIG,             //返回排行榜配置数据

    MSG_C2S_FAIRYLAND_CONFIG,             //申请配置数据
    MSG_S2C_FAIRYLAND_CONFIG,             //返回配置数据

    MSG_C2S_FAIRYLAND_CHAPTER_INFO,         //申请关卡数据
    MSG_S2C_FAIRYLAND_CHAPTER_INFO,         //返回关卡数据
//    MSG_S2C_FAIRYLAND_UPDATE_CHAPTER_INFO,     //更新关卡数据

    MSG_S2C_FAIRYLAND_END_DANCE_INFO,     //舞蹈结束数据

    MSG_S2C_FAIRYLAND_REWARD,    //秘境奖励奖励
    
    MSG_S2C_FAIRYLAND_END_RANDOMREWARD,    //舞蹈结束随机奖励

    MSG_C2S_FAIRYLAND_RANK,               //申请排行榜数据
    MSG_S2C_FAIRYLAND_RANK,               //返回排行榜数据

    MSG_C2S_FAIRYLAND_GET_YESTERDAY_RANK_REWARD,   //申请领取昨日排行榜奖励

    MSG_C2S_FAIRYLAND_OpenBless,       //祝福
    MSG_S2C_FAIRYLAND_OpenBless,
    MSG_C2S_FAIRYLAND_BLESS,           //祝福
    MSG_S2C_FAIRYLAND_BLESS,

    MSG_S2C_FAIRYLandChanllegeTimesNotify,
    MSG_S2C_FAIRYLandBlessNotify,
    MSG_S2C_FAIRYLandCurrentChapterNotify,
    MSG_S2C_FAIRYLandChapterScoreNotify,
    MSG_S2C_FAIRYLandStateNotify,
//     MSG_S2C_FAIRYLandConfigNotify,

    MSG_C2S_GetFairyLandRoomList,
    MSG_S2C_GetFairyLandRoomListResult,


//    MSG_S2C_FAIRYLAND_ERROR,           //秘境错误
    //内部消息
    MSG_G2S_FAIRYLAND_ANNOUNCEMENT_END,               //秘境结束公告
    MSG_G2S_FAIRYLAND_JOIN_RANDOM_REWARD,             //秘境参与奖励和随机奖励发放
    
//    MSG_S2G_FAIRYLAND_GROUPINFO,                      //秘境全局数据
//    MSG_S2G_FAIRYLAND_STROAGEBOX_INFO,                  //秘境储物箱数据

//    MSG_S2G_FAIRYLAND_CHECKISINRANK,       //检测是否在昨日排行榜里面

    MSG_S2G_FAIRYLAND_RANK,        //申请排行榜数据

//    MSG_S2G_FAIRYLAND_GET_YESTERDAY_RANK_REWARD,
    MSG_G2S_FAIRYLAND_GET_YESTERDAY_RANK_REWARD,

};

class GameMsg_G2S_AnnouncementFairyLandEnd : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_AnnouncementFairyLandEnd)

    bool doEncode(CParamPool &IOBuff){ return true; };
    bool doDecode(CParamPool &IOBuff){ return true; };
};
//////////////////////////////////////////////////////////////////////////
// 检测是否有奖励
class GameMsg_C2S_FairyLandCheckIsInYesterdatRank : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandCheckIsInYesterdatRank)

    bool doEncode(CParamPool &IOBuff){ return true; };
    bool doDecode(CParamPool &IOBuff){ return true; };
};

class GameMsg_S2C_FairyLandCheckIsInYesterdatRank : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandCheckIsInYesterdatRank)

    bool doEncode(CParamPool &IOBuff){ IOBuff.AddInt(m_nYesterdayScore); return true; };
    bool doDecode(CParamPool &IOBuff)
    { //client do
        m_nYesterdayScore = IOBuff.GetInt();
        return true; 
    };

    // > 0:可以领奖励，其他不能领奖励
    int m_nYesterdayScore;
};
//////////////////////////////////////////////////////////////////////////
class GameMsg_G2S_FairyLandJoinAndRandomReward : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_FairyLandJoinAndRandomReward)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nChapter);
        IOBuff.AddBool(m_nIsSendJoin);
        return true; 
    };
    bool doDecode(CParamPool &IOBuff)
    { 
        m_nChapter = IOBuff.GetInt();
        m_nIsSendJoin = IOBuff.GetBool();
        return true; 
    };

    int m_nChapter;
    bool m_nIsSendJoin;
};

class GameMsg_S2C_FairyLandReward : public GameMsg_Base
{
public:

    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandReward)

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    enum {
        YESTERDAY_RANK_REWARD = 0,
        JOIN_REWARD,
    };

    unsigned char m_nRewardType;
    int m_nBindMb;
    int m_nMoney;
    std::list<CItem> m_listReward;
};

class GameMsg_S2C_FairyLandRandomReward :public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandRandomReward)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nBindMb);
        IOBuff.AddInt(m_nMoney);
        m_item.doEncode(IOBuff);
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nBindMb = IOBuff.GetInt();
        m_nMoney = IOBuff.GetInt();
        m_item.doDecode(IOBuff);

        return true;
    }

    int m_nBindMb;
    int m_nMoney;
    CItem m_item;
};

//////////////////////////////////////////////////////////////////////////
// class GameMsg_C2S_FairyLand_GroupInfo : public GameMsg_Base
// {
// public:
//     GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLand_GroupInfo)
// 
//     bool doEncode(CParamPool &IOBuff){ return true; }
//     bool doDecode(CParamPool &IOBuff){ return true; }
// };
// 
// class GameMsg_S2G_FairyLand_GroupInfo : public GameMsg_Base
// {
// public:
//     GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_FairyLand_GroupInfo)
// 
//     bool doEncode(CParamPool &IOBuff)
//     { 
//         IOBuff.AddInt(m_nGroupId); 
//         IOBuff.AddUInt(m_nRoleId);
//         return true; 
//     }
// 
//     bool doDecode(CParamPool &IOBuff)
//     {
//         m_nGroupId = IOBuff.GetInt();
//         m_nRoleId = IOBuff.GetUInt();
// 
//         return true; 
//     }
// 
//     int m_nGroupId;
//     unsigned int m_nRoleId;
// };
// 
// class GameMsg_S2C_FairyLand_GroupInfo : public GameMsg_Base
// {
// public:
//     GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLand_GroupInfo)
// 
//     bool doEncode(CParamPool &IOBuff);
//     bool doDecode(CParamPool &IOBuff);
// 
//     int m_nChanllegeTimes;
//     int m_nBlessBuff;
//     int m_nChapter;
//     int m_nTotalScore;
//     int m_nScore;
// };

//////////////////////////////////////////////////////////////////////////
// class GameMsg_C2S_FairyLandFinsihedGuide : public GameMsg_Base
// {
// public:
//     GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandFinsihedGuide)
// 
//     bool doEncode(CParamPool &IOBuff){ return true; }
//     bool doDecode(CParamPool &IOBuff){ return true; }
// 
// };
// 
// class GameMsg_C2S_FairyLandCheckGuide : public GameMsg_Base
// {
// public:
//     GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandCheckGuide)
// 
//     bool doEncode(CParamPool &IOBuff){ return true; }
//     bool doDecode(CParamPool &IOBuff){ return true; }
// };

// class GameMsg_S2C_FairyLandCheckGuide : public GameMsg_Base
// {
// public:
//     GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandCheckGuide)
// 
//     bool doEncode(CParamPool &IOBuff)
//     { 
//         IOBuff.AddBool(m_bIsComplete);
//         return true; 
//     }
//     bool doDecode(CParamPool &IOBuff)
//     { 
//         m_bIsComplete = IOBuff.GetBool();
//         return true; 
//     }
//     
//     bool m_bIsComplete;
// };
//////////////////////////////////////////////////////////////////////////
// class GameMsg_S2C_FairyLandUpdateChapterInfo : public GameMsg_Base
// {
//     GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandUpdateChapterInfo)
// 
//     bool doEncode(CParamPool &IOBuff)
//     {
//         IOBuff.AddInt(m_nChapter);
//         return true;
//     }
// 
//     bool doDecode(CParamPool &IOBuff)
//     {
//         m_nChapter = IOBuff.GetInt();
//         return true;
//     }
// 
//     int m_nChapter;
// };
//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_FairyLandStorageboxInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandStorageboxInfo)

    bool doEncode(CParamPool &IOBuff)
    {
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        //client do
        return true;
    }
};


class GameMsg_S2C_FairyLandStorageboxInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandStorageboxInfo)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nTotalStorageBoxItemsCount);
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nTotalStorageBoxItemsCount = IOBuff.GetInt();
        //client do
        return true;
    }

    int m_nTotalStorageBoxItemsCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_FairyLandRankConfig : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandRankConfig)

    bool doEncode(CParamPool &IOBuff)
    {
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        //client do
        return true;
    }
};

class GameMsg_S2C_FairyLandRankConfig : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandRankConfig)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddStringW(m_strRankRule);
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        //client do
        IOBuff.GetStringW(m_strRankRule);
        return true;
    }

    std::string m_strRankRule;
};
//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_FairyLandConfig : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandConfig)

    bool doEncode(CParamPool &IOBuff)
    {
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        //client do
        return true;
    }
};

class GameMsg_S2C_FairyLandConfig : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandConfig)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nTotalChatper);
        IOBuff.AddStringW(m_strStorageBoxRule);
        IOBuff.AddStringW(m_strBlessRule);
        IOBuff.AddInt(m_nBlessCost);
        IOBuff.AddInt(m_nDefaultChallengeCount);
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        //client do
        return true;
    }

    int m_nTotalChatper;
    std::string m_strStorageBoxRule;
    std::string m_strBlessRule;
    int m_nBlessCost; // MB
    int m_nDefaultChallengeCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_FairyLandDanceEndInfo :public GameMsg_Base
{
public:
    typedef std::list<SDancerScore>::iterator LSIT;

    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandDanceEndInfo)

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public:
    int m_nChapterScore;
    int m_nChapterTotalScore;
    int m_nBlessBuf;
    std::list<SDancerScore> m_listScore;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_FairyLandChapterInfo :public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandChapterInfo)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nChatper);

        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nChatper = IOBuff.GetInt();
        return true;
    }
    int m_nChatper;
};

class GameMsg_S2C_FairyLandChapterInfo : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandChapterInfo)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nChatper);
        IOBuff.AddUChar(m_nDanceType);
        IOBuff.AddShort(m_nMusicId);
        IOBuff.AddUChar(m_nDifficultyLvl);

        IOBuff.AddUInt(m_nJoinMoney);
        IOBuff.AddUInt(m_nJoinBindMB);

        IOBuff.AddUShort((unsigned short)m_listJoin.size());
        for (std::list<CItem>::iterator it = m_listJoin.begin(); it != m_listJoin.end(); ++it)
        {
            it->doEncode(IOBuff);
        }

        IOBuff.AddUShort((unsigned short)m_listShow.size());
        for (std::list<CItem>::iterator it = m_listShow.begin(); it != m_listShow.end(); ++it)
        {
            it->doEncode(IOBuff);
        }

        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        //client do
        return true;
    }

    int m_nChatper;
    unsigned char m_nDanceType;
    short m_nMusicId;
    unsigned char m_nDifficultyLvl;
    unsigned int m_nJoinMoney;
    unsigned int m_nJoinBindMB;
    std::list<CItem> m_listJoin;
    std::list<CItem> m_listShow;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_FairyLandRankInfo : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandRankInfo)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nType);
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nType = IOBuff.GetInt();
        return true;
    }
    int m_nType;
};

class GameMsg_S2G_FairyLandRankInfo : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_FairyLandRankInfo)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nType);
        IOBuff.AddUInt(m_nRoleId);
        IOBuff.AddInt(m_nGroupId);
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nType = IOBuff.GetInt();
        m_nRoleId = IOBuff.GetUInt();
        m_nGroupId = IOBuff.GetInt();
        return true;
    }

    int m_nType;
    unsigned int m_nRoleId;
    int m_nGroupId;
};

class SRankCell
{
public:
    SRankCell() :m_strName(""), m_nRoleId(0), m_nValue(0){}
    std::string m_strName;
    unsigned int m_nRoleId;
    int m_nValue;
};

class GameMsg_S2C_FairyLandRankInfo : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandRankInfo)

    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nType);
        IOBuff.AddUShort((unsigned short)m_listRank.size());
        for (std::list<SRankCell>::iterator it = m_listRank.begin();
            it != m_listRank.end();++it)
        {
            IOBuff.AddStringW(it->m_strName);
            IOBuff.AddUInt(it->m_nRoleId);
            IOBuff.AddInt(it->m_nValue);
        }
        IOBuff.AddBool(m_bIsComplete);
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nType = IOBuff.GetInt();
        unsigned short nLen = IOBuff.GetUShort();
        for (unsigned short i = 0; i < nLen; ++i)
        {
            SRankCell s;
            IOBuff.GetStringW(s.m_strName);
            s.m_nRoleId = IOBuff.GetUInt();
            s.m_nValue = IOBuff.GetInt();
            m_listRank.push_back(s);
        }

        m_bIsComplete = IOBuff.GetBool();

        return true;
    }
    int m_nType;
    bool m_bIsComplete;
    std::list<SRankCell> m_listRank;
};
//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_FairyLandGetYesterdayRankReward : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandGetYesterdayRankReward)

    bool doEncode(CParamPool &IOBuff)
    {
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        return true;
    }
};

// class GameMsg_S2G_FairyLandGetYesterdayRankReward : public GameMsg_Base
// {
// public:
//     GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_FairyLandGetYesterdayRankReward)
// 
//     bool doEncode(CParamPool &IOBuff)
//     {
//         IOBuff.AddUInt(m_nRoleId);
//         IOBuff.AddInt(m_nGroupId);
//         return true;
//     }
// 
//     bool doDecode(CParamPool &IOBuff)
//     {
//         m_nRoleId = IOBuff.GetUInt();
//         m_nGroupId = IOBuff.GetInt();
//         return true;
//     }
// 
//     unsigned int m_nRoleId;
//     int m_nGroupId;
// };

class GameMsg_G2S_FairyLandGetYesterdayRankReward : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_FairyLandGetYesterdayRankReward)
    bool doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddInt(m_nRewardId);
        return true;
    }

    bool doDecode(CParamPool &IOBuff)
    {
        m_nRewardId = IOBuff.GetInt();
        return true;
    }

    int m_nRewardId;
};
//////////////////////////////////////////////////////////////////////////


class GameMsg_C2S_FairyLandOpenBless : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandOpenBless);

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

};

class GameMsg_S2C_FairyLandOpenBless : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandOpenBless);
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    enum
    {
        Sucess = 0,
        NOGroup,
    };
    unsigned char m_nRetCode;
    unsigned int m_nLeftTime;
    std::string m_strLastBlessRole;
};


class GameMsg_C2S_FairyLandBless :public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_FairyLandBless)
    bool doEncode(CParamPool &IOBuff){return true;}
    bool doDecode(CParamPool &IOBuff){return true;}
};

class GameMsg_S2C_FairyLandBless :public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandBless)
    bool doEncode(CParamPool &IOBuff){ IOBuff.AddInt(m_nFlag); return true; }
    bool doDecode(CParamPool &IOBuff){ m_nFlag = IOBuff.GetInt(); return true; }

    enum {
        eSuc,
        eNoGroup,
        eResourceNotEnough,
    };
    int m_nFlag;
};


class GameMsg_S2C_FairyLandChanllegeTimesNotify :public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandChanllegeTimesNotify);
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned short m_nChanllegeTimes;
};

class GameMsg_S2C_FairyLandBlessNotify :public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandBlessNotify);
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned char m_nBelss;
};

class GameMsg_S2C_FairyLandCurrentChapterNotify :public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandCurrentChapterNotify);
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned char m_nCurrentChapter;
    unsigned int m_nTotalScore;
};

class GameMsg_S2C_FairyLandChapterScoreNotify :public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandChapterScoreNotify);
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned int m_nScore;
};

// 开/关
class GameMsg_S2C_FairyLandStateNotify :public GameMsg_Base
{
public:
    enum // state
    {
        EOpen = 1,
        EClose,
    };

public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_FairyLandStateNotify);
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned char m_nState; // 1:开始， 2： 结束
    unsigned char m_nCurrentChapter;
    unsigned int m_nTotalScore;
};


class GameMsg_C2S_GetFairyLandRoomList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFairyLandRoomList);

    GameMsg_C2S_GetFairyLandRoomList(void);
    ~GameMsg_C2S_GetFairyLandRoomList(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    //    unsigned char m_nRoomType; // ERoomType
    unsigned int m_nPage; // page
};


struct FairyLandRoomRole
{
public:
    std::string m_strRoleName;
    unsigned char m_nVip;
    bool m_bIsVip;
};

class FairyLandRoomBriefInfo
{
public:
    FairyLandRoomBriefInfo();
    ~FairyLandRoomBriefInfo();

    bool doDecode(CParamPool& IOBuff);
    bool doEncode(CParamPool& IOBuff);

    unsigned int m_nRoomIndex;
    bool m_bHasStart;
    std::vector<FairyLandRoomRole> m_vecRoles;

};

class GameMsg_S2C_GetFairyLandRoomListResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetFairyLandRoomListResult);

    GameMsg_S2C_GetFairyLandRoomListResult(void);
    ~GameMsg_S2C_GetFairyLandRoomListResult(void);

    virtual bool doDecode(CParamPool& IOBuff);
    virtual bool doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nCurPage;
    unsigned int m_nRoomNum;
    std::vector<FairyLandRoomBriefInfo> m_vecRoomBriefInfo;
};


#endif