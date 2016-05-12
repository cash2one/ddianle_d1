/**
* Filename: GameMsg_Activeness
* Description: 活跃度系统消息定义
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-05-14 17:46:12
*/

#ifndef _GAMEMSG_ACTIVENESS_H_
#define _GAMEMSG_ACTIVENESS_H_

#include "../share/CommonMsgDef.h"
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../Pattern/Param.h"

#include <string>

//////////////////////////////////////////////////////////////////////////
// 消息ID定义
enum eMSG_Activeness
{
    MSG_ACTIVENESS_BEGIN = MSG_TYPE_Activeness,

    MSG_C2S_GetActivenessData,      // 获取活跃度系统数据
    MSG_S2C_GetActivenessDataRes,   // 获取活跃度系统数据的返回

    MSG_C2S_GetActivenessReward,    // 领取活跃度系统宝箱
    MSG_S2C_GetActivenessRewardRes, // 领取活跃度系统宝箱的结果
};


//////////////////////////////////////////////////////////////////////////
// 活跃度模块公共数据结构
//////////////////////////////////////////////////////////////////////////

class ActivenessTargetConfig
{
public:
    enum EType
    {
        EType_Invalid = 0,
        EType_Checkin = 1,          // 完成每日签到
        EType_DailyQuest = 2,       // 完成日常任务
        EType_Dungeon = 3,          // 完成魔法神灯关卡
        EType_DanceMode = 4,        // 完成不同模式的歌曲
        EType_QuickDance = 5,       // 使用一次快速开始并完成
        EType_VipReward = 6,        // 领取VIP福利
        EType_SocialShare = 7,      // 进行社交分享
        EType_MagicArrayCall = 8,           // 幸运魔法阵召唤一次
        EType_ClothEffectReplace = 9,       // 服饰特效灌注
        EType_ClothEffectUpgrade = 10,      // 服饰特效升级
        EType_Max,
    };

public:
    ActivenessTargetConfig();
    virtual ~ActivenessTargetConfig();

    void Set(int nTargetType, int nTargetValue, int nTargetScore, const char *strDesc);

    virtual void doEncode(CParamPool & IOBuff);

    // 目标名称和目标类型一一对应，可相互转换
    static const std::string &GetNameFromType(int nType);
    static int GetTypeFromName(const std::string &name);

public:
    int m_nTargetType;          // 目标类型
    int m_nTargetValue;         // 达成目标所需数值
    int m_nTargetScore;         // 奖励的活跃度积分
    std::string m_strName;      // 名称，和目标类型一一对应，调用set方法时，自动设置
    std::string m_strDesc;      // 描述
};

class ActivenessRewardConfig
{
public:
    ActivenessRewardConfig();
    virtual ~ActivenessRewardConfig();

public:
    void Set(int nRequireScore, const char *strMaleItem, const char *strFemaleItem, int nMoney,
        int nBindMCoin, const char *strVIPMaleItem, const char *strVIPFemaleItem, int nVIPMoney, int nVIPBindMCoin);

    virtual void doEncode(CParamPool & IOBuff, ESexType sex);

    int GetRequireScore() const;

public:
    int m_nRequireScore;
    CReward m_maleReward;
    CReward m_femaleReward;
    CReward m_vipMaleReward;
    CReward m_vipFemaleReward;
};

/////////////////////////////////////////////////
class GameMsg_C2S_GetActivenessData :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetActivenessData);
    GameMsg_C2S_GetActivenessData();
    ~GameMsg_C2S_GetActivenessData();
public:
    virtual bool doDecode(CParamPool &IOBuff);
};

/////////////////////////////////////////////////

// 活跃度目标完成进度
class ActivenessTarget 
{
public:
    ActivenessTarget(const ActivenessTargetConfig & targetConfig);
    virtual ~ActivenessTarget(){};
 
private:
    ActivenessTarget & operator = (const ActivenessTarget& progress);

public:
    void doEncode(CParamPool & IOBuff);
    void SetProgress(int nProgress);
    bool IsTargetComplete();
    int  GetTargetType() const;
    int  GetTargetScore() const;
    int  GetProgress() const;
    virtual bool UpdateProgress(IParam & param);

public:
    const ActivenessTargetConfig &  m_TargetConfig;
    int                             m_nProgress;    // 目标进度
};

// 活跃度目标完成进度
class ActivenessDanceModeTarget : public ActivenessTarget
{
public:
    ActivenessDanceModeTarget(const ActivenessTargetConfig & targetConfig);    
    virtual ~ActivenessDanceModeTarget() {};

private:
    ActivenessDanceModeTarget & operator = (const ActivenessDanceModeTarget & danceModeTarget);

public:
    void SetFinishedDanceMode(std::set<int> & finishedDanceMonde);
    bool UpdateProgress(IParam & param);
    void GetFinisheDanceMode(std::set<int> & finishedDanceMode) const;

public:
    std::set<int>                  m_setFinishedDanceMode; //跳舞完成模式
};

class ActivenessTargetFactory
{
public:
    ActivenessTargetFactory();
    virtual ~ActivenessTargetFactory(){};

public:
    static ActivenessTargetFactory & Instance();
    ActivenessTarget * CreateTarget(const ActivenessTargetConfig & targetConfig);

};

//////////////////////////////////////////////////////////////////////////
// 活跃度奖励领取状态
class ActivenessReward
{
public:
    ActivenessReward(const ActivenessRewardConfig & rewardConfig);
    ~ActivenessReward() {};

private:
    ActivenessReward & operator = (ActivenessReward & reward);


public:
    void doEncode(CParamPool & IOBuff, ESexType sex);

public:
    const ActivenessRewardConfig &  m_ActivenessRewardConfig;
    int                             m_nRewardIndex;     // 奖励等级编号
    bool                            m_bReceived;       // 奖励是否领取

};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetActivenessDataRes :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GetActivenessDataRes();
    ~GameMsg_S2C_GetActivenessDataRes();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    ESexType m_eSex;                                        // 玩家性别，方便消息编码
    int m_nActiveness;                                      // 玩家当日活跃度
    std::list<ActivenessReward> m_listRewardState;     // 奖励领取情况
    std::list<ActivenessTarget> m_listTargetProgress;// 目标达成情况
};


/////////////////////////////////////////////////
class GameMsg_C2S_GetActivenessReward :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetActivenessReward);
    GameMsg_C2S_GetActivenessReward();
    ~GameMsg_C2S_GetActivenessReward();
public:
    virtual bool doDecode(CParamPool &IOBuff);
public:
    int m_nRewardIndex;     // 奖励等级编号
};

/////////////////////////////////////////////////
class GameMsg_S2C_GetActivenessRewardRes :
    public GameMsg_Base
{
public:
    GameMsg_S2C_GetActivenessRewardRes();
    ~GameMsg_S2C_GetActivenessRewardRes();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::string m_strError;
    int m_nRewardIndex;     // 奖励等级编号
    bool m_bResult;         // 奖励是否领取成功
};


#endif //_GAMEMSG_ACTIVENESS_H_

