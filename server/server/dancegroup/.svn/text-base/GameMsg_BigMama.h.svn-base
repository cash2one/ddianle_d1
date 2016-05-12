#ifndef __GAMEMSG_BIGMAMA_H__
#define __GAMEMSG_BIGMAMA_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include "DataStruct_BigMama.h"



class CEntityItemComponent;
class CEntityGeneComponent;
class PetComponent;
class CEntityMedalComponent;

/// <summary>
/// S2C 同步广场舞大妈活动配置
/// </summary>
class GameMsg_S2C_SyncBigMamaConfig : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncBigMamaConfig)

public:
//     GameMsg_S2C_SyncBigMamaConfig();
//     ~GameMsg_S2C_SyncBigMamaConfig();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    // 当前活动状态: 0-未开启 1-挑战中 2-挑战完成
    EBigMamaState m_nCurState;
    // 最后完成挑战者类型: 0-小恋 1-小软 2-舞团成员
    EBigMamaLastBeatType m_nBeaterType;
    // 最后一轮挑战成功的玩家名
    std::string m_strRoleName ;
    // 奖励状态: 0-没有奖励 1-奖励未领取 2-奖励已领取
    unsigned char m_nRewardState;
};

/// <summary>
/// C2S 请求广场舞大妈数据
/// </summary>
class GameMsg_C2S_RequestBigMamaStageInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestBigMamaStageInfo)

public:
//     GameMsg_C2S_RequestBigMamaStageInfo();
//     ~GameMsg_C2S_RequestBigMamaStageInfo();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);
};

/// <summary>
/// S2C 同步广场舞大妈活动状态
/// </summary>
class GameMsg_S2C_SyncBigMamaStageInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncBigMamaStageInfo)

public:
//     GameMsg_S2C_SyncBigMamaStageInfo();
//     ~GameMsg_S2C_SyncBigMamaStageInfo();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    bool m_bOpened;
    BigMamaProperty m_property;
};

/// <summary>
/// C2S 请求挑战大妈
/// </summary>
class GameMsg_C2S_RequestFightBigMama : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestFightBigMama)

public:
//     GameMsg_C2S_RequestFightBigMama();
//     ~GameMsg_C2S_RequestFightBigMama();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned char m_nPhoneOS;

};


class BigMamaPlayerInfo
{
public:
    BigMamaPlayerInfo();
    ~BigMamaPlayerInfo();

    void doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    std::string  m_strRoleName;
    bool m_bIsHost;
    char m_chRoleType;
    bool m_nIsBoss;
    char m_chRolePos;
    char m_chRoleState;
    EDanceColorTeam m_eColorTeam;
    char m_nRoleSex;
    char m_nRoleSkin;
    char m_nGroupTitle;
    std::string m_strDanceGroupName;
    unsigned short m_nDanceGroupBadge;
    unsigned short m_nDanceGroupEffect;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    unsigned char  m_nMoveType;
    unsigned short m_nTransFormID;
    bool m_bEncodeMedal;

    // 其他信息
    CEntityItemComponent* m_pItemCOM;
    CEntityGeneComponent* m_pGeneCOM;
    PetComponent* m_pPetCOM;
    CEntityMedalComponent* m_pMedalCOM;
    std::map<EItemClothType, itemtype_t>  m_Equips;

};


class BigMamaRoomInfo
{
public:
    BigMamaRoomInfo();
    ~BigMamaRoomInfo();

    void doEncode(CParamPool& IOBuff);

public:
    unsigned char m_nRoomType;
    unsigned char m_nScene;
    unsigned short m_nMusic;
    unsigned char m_nMusicMode;
    unsigned char m_nMusicLevel;
    unsigned char m_nSelfRoomType;
    unsigned char m_nTeamMode;
    bool m_bIsHost;
    std::list<BigMamaPlayerInfo> m_listDancer;
};


/// <summary>
/// S2C 请求挑战结果
/// </summary>
class GameMsg_S2C_RequestFightBigMamaRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestFightBigMamaRes)

public:
//     GameMsg_S2C_RequestFightBigMamaRes();
//     ~GameMsg_S2C_RequestFightBigMamaRes();
    bool doEncode(CParamPool& IOBuff);
//    bool doDecode(CParamPool &IOBuff);

    // 错误枚举: 0-成功
    EFightBigMamaResult m_nErrorType;

    std::string m_strCheckKey;
    std::string m_szStage;
    BigMamaRoomInfo m_roomInfo;
};

/// <summary>
/// C2S 请求大妈奖励
/// </summary>
class GameMsg_C2S_RequestGetBigMamaReward : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestGetBigMamaReward)

public:
//     GameMsg_C2S_RequestGetBigMamaReward();
//     ~GameMsg_C2S_RequestGetBigMamaReward();

    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);
};


/// <summary>
/// S2C 请求奖励结果
/// </summary>
class GameMsg_S2C_RequestGetBigMamaRewardRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestGetBigMamaRewardRes)

public:
//     GameMsg_S2C_RequestGetBigMamaRewardRes();
//     ~GameMsg_S2C_RequestGetBigMamaRewardRes();
    bool doEncode(CParamPool& IOBuff);

    // 错误枚举: 0-成功
    EBigMamaRewardResult m_nErrorType;
    // 奖励信息
    CReward m_RewardInfo;
    unsigned int m_nDanceGroupContribution;
    bool m_bIsGuideReward; // 是否是引导奖励
};

/// <summary>
/// C2S 完成大妈引导
/// </summary>
class GameMsg_C2S_CompleteBigMamaGuide : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_CompleteBigMamaGuide)

public:
//     GameMsg_C2S_CompleteBigMamaGuide();
//     ~GameMsg_C2S_CompleteBigMamaGuide();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

};

/// <summary>
/// C2S 请求大妈奖励
/// </summary>
class GameMsg_C2S_RequestBigMamaGuideReward : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestBigMamaGuideReward)

public:
//     GameMsg_C2S_RequestBigMamaGuideReward();
//     ~GameMsg_C2S_RequestBigMamaGuideReward();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    // 引导奖励类型: 0-正常完成引导 1-跳过引导
    unsigned char m_nRewardType;
};

/// <summary>
/// S2C 通知大妈活动开启或关闭
/// </summary>
class GameMsg_S2C_NoticeBigMamaState : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_NoticeBigMamaState)

public:
//     GameMsg_S2C_NoticeBigMamaState();
//     ~GameMsg_S2C_NoticeBigMamaState();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    // 大妈活动状态: 0-关闭 1-开启
    unsigned char m_nState;
};

class GameMsg_S2C_BigMamaIntroduce : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_BigMamaIntroduce)

public:
    //     GameMsg_S2C_NoticeBigMamaState();
    //     ~GameMsg_S2C_NoticeBigMamaState();
    bool doEncode(CParamPool& IOBuff);
    bool doDecode(CParamPool& IOBuff);

    std::string m_strIntroduce;
};

#endif // __GAMEMSG_BIGMAMA_H__

