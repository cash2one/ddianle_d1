/*
* GameMsg_Pet.h
* Description: 
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: XieJiangBei
* Create time: 2015-6-9 14:34:12
*/

#ifndef __GAMEMSG_PET_H__
#define __GAMEMSG_PET_H__

#include "../share/CommonMsgDef.h"

#include "../../socket/GameMsg_Base.h"

#include "../../datastructure/DataStruct_Base.h"

#include <list>
#include <string>
#include <set>


//////////////////////////////////////////////////////////////////////////
class PetMsgInfo
{
public:
    PetMsgInfo();
    ~PetMsgInfo();

public:
    void doEncode(CParamPool &IOBuff) const;

public:
    unsigned int    m_nPetID;               // 宠物ID
    unsigned short  m_nTypeID;              // 宠物类型ID
    std::string     m_strDefaultName;       // 默认昵称
    std::string     m_strName;              // 宠物昵称
    unsigned char   m_nPotentialID;         // 宠物潜质
    unsigned char   m_nAttributeType;       // 属性类型
    unsigned short  m_nLevel;               // 当前等级
    unsigned short  m_nMaxLevel;            // 最大等级
    unsigned int    m_nExp;                 // 当前经验
    unsigned int    m_nMaxExp;              // 最大经验
    unsigned short  m_nEndurance;           // 当前耐力
    unsigned short  m_nMaxEndurance;        // 最大耐力
    float           m_fRecoveryTime;        // 恢复时间
    bool            m_bCanImproveQuality;   // 是否能提升品质
    unsigned char   m_nQualityType;         // 当前品质
    bool            m_bCanEvolution;        // 是否能进化
    unsigned char   m_nEvolutionTimes;      // 进化次数
    unsigned char   m_nResetSkillCostType;  // 重置技能消耗货币类型
    unsigned int    m_nResetSkillCostAmount;// 重置技能消耗货币数量
    int             m_nDanceSkillCount;     // 跳舞技能数
    int             m_nFightSkillCount;     // 战斗技能数
    std::list<PetMsgSkill> m_listDanceSkill;// 跳舞技能列表
    std::list<PetMsgSkill> m_listFightSkill;// 斗技能列表
};

//////////////////////////////////////////////////////////////////////////

class PetCommonMsgInfo
{
public:
    PetCommonMsgInfo();
    ~PetCommonMsgInfo();

public:
    void doEncode(CParamPool &IOBuff) const;

public:
    int             m_nPetCount;
    int             m_nGridCount;           // 我的宠物栏个数
    int             m_nMaxGridCount;        // 最大宠物栏个数
    int             m_nMaxUnlockCountOnce;  // 每次最多解锁个数
    unsigned char   m_nUnlockCostType;
    unsigned int    m_nUnlockCostAmount;
    unsigned char   m_nChangeNameCostType;
    unsigned int    m_nChangeNameCostAmount;
    std::list<unsigned int> m_listFeedExpItems;
    std::list<unsigned int> m_listFeedEnduranceItems;
    std::string     m_strPetSkillTip;       // 技能重置和训练规则
    bool            m_bCarriedPet;
    PetMsgInfo      m_carriedPetInfo;
};

class PlayerPetConfig
{
public:
    PlayerPetConfig();
    ~PlayerPetConfig();

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short m_nPetType;             // 宠物类型
    unsigned short m_nPetAttributeType;    // 宠物属性类型
    unsigned short m_nMaxLevel;            // 最大等级
    unsigned char  m_nMaxQualityType;      // 最高品质
    unsigned char  m_nMaxEvolutionTimes;   // 最高进化次数
    unsigned short m_nMaxEndurance;        // 最大耐力
};

//////////////////////////////////////////////////////////////////////////

enum eMSG_Pet
{
    MSG_PET_BEGIN = MSG_TYPE_PET,

    MSG_C2S_Pet_EnterIncubationRoom,        // 进入孵化室
    MSG_S2C_Pet_EnterIncubationRoomRes,
    MSG_C2S_Pet_IncubatorLevelUp,           // 孵化器升级
    MSG_S2C_Pet_IncubatorLevelUpRes,

    MSG_C2S_Pet_StartIncubation,            // 开始孵化
    MSG_S2C_Pet_StartIncubationRes,
    MSG_C2S_Pet_IncubationRequest,          // 孵化状态查询
    MSG_S2C_Pet_IncubationRequestRes,
    MSG_C2S_Pet_SpeedUpIncubation,          // 加速孵化
    MSG_S2C_Pet_SpeedUpIncubationRes,
    MSG_C2S_Pet_OpenEgg,                    // 开蛋
    MSG_S2C_Pet_OpenEggRes,

    MSG_S2C_Pet_SyncCommonInfos,            // 同步宠物基本信息

    MSG_C2S_Pet_RequestList,                // 请求宠物栏列表
    MSG_S2C_Pet_RequestListRes,
    MSG_C2S_Pet_UnlockGrid,                 // 解锁宠物栏
    MSG_S2C_Pet_UnlockGridRes,

    MSG_C2S_Pet_ChangeName,                 // 修改昵称
    MSG_S2C_Pet_ChangeNameRes,

    MSG_C2S_Pet_ChangeCarried,              // 更换携带宠物
    MSG_S2C_Pet_ChangeCarriedRes,
    MSG_C2S_Pet_UnloadCarried,              // 卸下宠物
    MSG_S2C_Pet_UnloadCarriedRes,

    MSG_C2S_Pet_RequestImproveCondition,    // 申请转生条件
    MSG_S2C_Pet_RequestImproveConditionRes,
    MSG_C2S_Pet_Improve,                    // 转生
    MSG_S2C_Pet_ImproveRes,

    MSG_C2S_Pet_RequestEvolutionCondition,  // 申请进化条件
    MSG_S2C_Pet_RequestEvolutionConditionRes,
    MSG_C2S_Pet_Evolution,                  // 进化
    MSG_S2C_Pet_EvolutionRes,

    MSG_C2S_Pet_Feeding,                    // 喂养
    MSG_S2C_Pet_FeedingRes,

    MSG_C2S_Pet_TrainingSkill,              // 升级技能
    MSG_S2C_Pet_TrainingSkillRes,
    MSG_C2S_Pet_ResetSkills,                // 重置技能
    MSG_S2C_Pet_ResetSkillsRes,

    MSG_C2S_Pet_GetExchangeList,            // 获取兑换宠物列表
    MSG_S2C_Pet_GetExchangeListRes,
    MSG_C2S_Pet_Exchange,                   // 宠物兑换
    MSG_S2C_Pet_ExchangeRes,

    MSG_C2S_Pet_GetPetLibraryTypeList,      // 获取宠物图鉴宠物类型列表
    MSG_S2C_Pet_GetPetLibraryTypeListRes,
    MSG_C2S_Pet_GetPetLibraryList,          // 获取宠物图鉴列表
    MSG_S2C_Pet_GetPetLibraryListRes,

    MSG_S2C_Pet_SyncPetChangedInfoToOthers, // 同步宠物信息变更给其它玩家
    MSG_S2C_Pet_SyncPetChangedInfoToSelf,   // 同步宠物信息变更给自己

    MSG_C2S_Pet_ReportPetDanceEmotion,      // 通知宠物跳舞表情
    MSG_S2C_Pet_SyncPetDanceEmotion,        // 同步宠物跳舞表情

    MSG_S2C_Pet_CastPetDanceSkill,          // 释放宠物跳舞技能

    MSG_C2S_Pet_GuideRequest,               // 宠物新手引导
    MSG_S2C_Pet_GuideResponse,

    MSG_S2C_Pet_RefreshPetSkillList,        // 技能激活信息同步
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetEnterIncubationRoom : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetEnterIncubationRoom);

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

enum ePetEnterIncubationRoomRes
{
    ePetEnterIncubationRoomRes_Success = 0,
    ePetEnterIncubationRoomRes_LoadingInfos,
};

class GameMsg_S2C_PetEnterIncubationRoomRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetEnterIncubationRoomRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned short  m_nIncubatorLevel;
    bool            m_bHasEgg;
    unsigned int    m_nEggTypeID;
    unsigned int    m_nLastTime;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetIncubatorLevelUp : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetIncubatorLevelUp);

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

enum ePetIncubatorLevelUpRes
{
    ePetIncubatorLevelUpRes_Success = 0,
    ePetIncubatorLevelUpRes_LoadingInfos,
    ePetIncubatorLevelUpRes_MaxLevel,
    ePetIncubatorLevelUpRes_NotEnoughMoney,
};

class GameMsg_S2C_PetIncubatorLevelUpRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetIncubatorLevelUpRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetStartIncubation : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetStartIncubation);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned short  m_nIndex;    // 背包道具栏中的索引
};

//////////////////////////////////////////////////////////////////////////

enum ePetStartIncubationRes
{
    ePetStartIncubationRes_Success = 0,
    ePetStartIncubationRes_LoadingInfos,
    ePetStartIncubationRes_Incubating,
    ePetStartIncubationRes_NotFoundItem,
    ePetStartIncubationRes_IsNotEgg,
    ePetStartIncubationRes_NotEnoughMoney,
    ePetStartIncubationRes_IncubatorLevelTooLow,
    ePetStartIncubationRes_SystemError,
};

class GameMsg_S2C_PetStartIncubationRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetStartIncubationRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetIncubationRequest : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetIncubationRequest);

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_PetIncubationRequestRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetIncubationRequestRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    bool            m_bFinished;
    unsigned int    m_nLastTime;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetIncubationSpeedUp : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetIncubationSpeedUp);

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

enum ePetIncubationSpeedUpRes
{
    ePetIncubationSpeedUpRes_Success = 0,
    ePetIncubationSpeedUpRes_LoadingInfos,
    ePetIncubationSpeedUpRes_NoEgg,
    ePetIncubationSpeedUpRes_Finished,
    ePetIncubationSpeedUpRes_NotEnoughMoney,
    ePetIncubationSpeedUpRes_SystemError,
};

class GameMsg_S2C_PetIncubationSpeedUpRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetIncubationSpeedUpRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetOpenEgg : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetOpenEgg);

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

enum ePetOpenEggRes
{
    ePetOpenEggRes_Success = 0,
    ePetOpenEggRes_LoadingInfos,
    ePetOpenEggRes_NoEgg,
    ePetOpenEggRes_NotFinished,
    ePetOpenEggRes_PetGridFull,
    ePetOpenEggRes_SystemError,
};

class GameMsg_S2C_PetOpenEggRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetOpenEggRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned short  m_nPetTypeID;
    std::string     m_strPetDefaultName;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_PetSyncCommonInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetSyncCommonInfo);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    PetCommonMsgInfo   m_commonInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetRequestList : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetRequestList);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    int             m_nCurPage;
    int             m_nCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_PetRequestListRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetRequestListRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    int             m_nCurPage;
    int             m_nPetCount;
    int             m_nGridCount;
    int             m_nEmptyCount;
    std::list<PetMsgInfo> m_listPetInfo;
    bool            m_bCanUnlock;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetUnlockGrid : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetUnlockGrid);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    int             m_nCount;
};

//////////////////////////////////////////////////////////////////////////

enum ePetUnlockGridRes
{
    ePetUnlockGridRes_Success = 0,
    ePetUnlockGridRes_NotEnoughGrid,
    ePetUnlockGridRes_NotEnoughMoney,
    ePetUnlockGridRes_LoadingInfo,
    ePetUnlockGridRes_UnlockedAll,
    ePetUnlockGridRes_TooManyCount,
};

class GameMsg_S2C_PetUnlockGridRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetUnlockGridRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    int             m_nGridCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetChangeName : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetChangeName);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
    std::string     m_strNewName;
};

//////////////////////////////////////////////////////////////////////////

enum ePetChangeNameRes
{
    ePetChangeNameRes_Success = 0,
    ePetChangeNameRes_NotFoundPet,
    ePetChangeNameRes_EmptyName,
    ePetChangeNameRes_SameName,
    ePetChangeNameRes_NotEnoughMoney,
};

class GameMsg_S2C_PetChangeNameRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetChangeNameRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned int    m_nPetID;
    std::string     m_strNewName;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetChangeCarried : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetChangeCarried);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
    unsigned char   m_nSceneID;
};

//////////////////////////////////////////////////////////////////////////

enum ePetChangeCarriedRes
{
    ePetChangeCarriedRes_Success = 0,
    ePetChangeCarriedRes_NotFoundPet,
    ePetChangeCarriedRes_SamePet,
};

class GameMsg_S2C_PetChangeCarriedRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetChangeCarriedRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned char   m_nSceneID;
    PetMsgInfo      m_petInfos;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetUnloadCarried : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetUnloadCarried);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned char   m_nSceneID;
};

//////////////////////////////////////////////////////////////////////////

enum ePetUnloadCarriedRes
{
    ePetUnloadCarriedRes_Success = 0,
    ePetUnloadCarriedRes_NoPetCarried,
};

class GameMsg_S2C_PetUnloadCarriedRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetUnloadCarriedRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned char   m_nSceneID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetRequestImproveCondition : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetRequestImproveCondition);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_PetRequestImproveConditionRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetRequestImproveConditionRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
    CItem           m_itemImprove;
    unsigned char   m_nCostType;
    unsigned int    m_nCostAmount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetImprove : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetImprove);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
};

//////////////////////////////////////////////////////////////////////////

enum ePetImproveRes
{
    ePetImproveRes_Success = 0,
    ePetImproveRes_NotFoundPet,
    ePetImproveRes_MaxQuality,
    ePetImproveRes_NotUpToImproveLevel,
    ePetImproveRes_NotEnoughMoneyOrItems,
    ePetImproveRes_SystemError,
};

class GameMsg_S2C_PetImproveRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetImproveRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned int    m_nPetID;
    CItem           m_nextItemImprove;
    unsigned char   m_nNextCostType;
    unsigned int    m_nNextCostAmount;
    PetMsgInfo      m_petInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetRequestEvolutionCondition : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetRequestEvolutionCondition);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_PetRequestEvolutionConditionRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetRequestEvolutionConditionRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
    CItem           m_itemEvolution;
    CItem           m_itemExtra;
    unsigned char   m_nCostType;
    unsigned int    m_nCostAmount;
    int             m_nBaseRate;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetEvolution : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetEvolution);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
    int             m_nExtraItemCount;
};

//////////////////////////////////////////////////////////////////////////

enum ePetEvolutionRes
{
    ePetEvolutionRes_Success = 0,
    ePetEvolutionRes_Failed,
    ePetEvolutionRes_NotFoundPet,
    ePetEvolutionRes_MaxEvolutionTimes,
    ePetEvolutionRes_NotUpToEvolutionLevel,
    ePetEvolutionRes_NotEnoughMoneyOrItems,
    ePetEvolutionRes_SystemError,
};

class GameMsg_S2C_PetEvolutionRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetEvolutionRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned int    m_nPetID;
    CItem           m_nextItemEvolution;
    CItem           m_nextItemExtra;
    unsigned char   m_nNextCostType;
    unsigned int    m_nNextCostAmount;
    int             m_nNextBaseRate;
    PetMsgInfo     m_petInfos;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetFeeding : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetFeeding);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nPetID;
    unsigned int    m_nItemType;
    int             m_nItemCount;
};

//////////////////////////////////////////////////////////////////////////

enum ePetFeedingRes
{
    ePetFeedingRes_Success = 0,
    ePetFeedingRes_NotFoundPet,
    ePetFeedingRes_ItemValid,
    ePetFeedingRes_MaxLevel,
    ePetFeedingRes_MaxEndurance,
    ePetFeedingRes_NotEnoughItems,
    ePetFeedingRes_SystemError,
};

class GameMsg_S2C_PetFeedingRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetFeedingRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned int    m_nPetID;
    PetMsgInfo      m_petInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestTrainingSkill : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestTrainingSkill);

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nPetID;  //需要训练的宠物ID
    SkillID      m_nSkillID;//需要训练的技能类型
};

enum ETrainSkillError
{
    eTrainSkillError_OK,
    eTrainSkillError_NoPet,
    eTrainSkillError_NoSkill,
    eTrainSkillError_AlreadyMaxLevel,
    eTrainSkillError_NotEnoughMoney,
};

class GameMsg_S2C_RequestTrainingSkillRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestTrainingSkillRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned int    m_nPetID;
    SkillID         m_nSkillID;
    PetMsgSkill     m_PetSkill;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_RequestResetSkills : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestResetSkills);

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nPetID;                      //需要重置的宠物ID
    std::set<SkillID> m_setLockSkillID;         //锁定的技能列表
};

enum EResetSkillError
{
    eResetSkillError_OK,
    eResetSkillError_NoPet,
    eResetSkillError_NoSkill,
    eResetSkillError_NotEnoughItem,
    eResetSkillError_NotEnoughMoney,
    eResetSkillError_LockAllSkill,
};
class GameMsg_S2C_RequestResetSkillsRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestResetSkillsRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nErrorCode;
    unsigned int    m_nPetID;
    unsigned char   m_nResetSkillCostType;  // 重置技能消耗货币类型
    unsigned int    m_nResetSkillCostAmount;// 重置技能消耗货币数量
    int             m_nDanceSkillCount;     // 跳舞技能数
    int             m_nFightSkillCount;     // 战斗技能数
    std::list<PetMsgSkill> m_listDanceSkill;//.跳舞技能列表
    std::list<PetMsgSkill> m_listFightSkill;//.斗技能列表
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_PetGetExchangeList : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetGetExchangeList);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned short  m_nCurPage;
    unsigned short  m_nCount;
};

//////////////////////////////////////////////////////////////////////////
class PetExchangeMsgInfo
{
public:
    PetExchangeMsgInfo();
    ~PetExchangeMsgInfo();

public:
    void doEncode(CParamPool &IOBuff) const;

public:
    unsigned int    m_nPetID;
    unsigned short  m_nTypeID;
    std::string     m_strName;
    unsigned char   m_nAttributeType;
    unsigned short  m_nLevel;
    unsigned char   m_nQualityType;
    unsigned char   m_nEvolutionTimes;
    unsigned char   m_nPotentialID;
    std::list<PetMsgSkill> m_listDanceSkill;// 跳舞技能列表
    std::list<PetMsgSkill> m_listFightSkill;// 战斗技能列表
    std::list<CItem> m_listExchangeItems;
};

class GameMsg_S2C_PetGetExchangeListRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetGetExchangeListRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_nCurPage;
    unsigned short  m_nPetCount;
    std::list<PetExchangeMsgInfo> m_listExchangeInfos;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_PetExchange : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetExchange);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    std::list<unsigned int> m_listPetID;    // 所有兑换宠物ID
};

//////////////////////////////////////////////////////////////////////////

enum ePetExchangeRes
{
    ePetExchangeRes_Success = 0,
    ePetExchangeRes_NotSelectedPet,
    ePetExchangeRes_NotFoundPet,
    ePetExchangeRes_CarriedPet,
};

class GameMsg_S2C_PetExchangeRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetExchangeRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned char       m_nErrorCode;
};


//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetPetLibraryTypeList : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_GetPetLibraryTypeList);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
};

class PetLibraryMsg
{
public:
    PetLibraryMsg();
    ~PetLibraryMsg();

public:
    bool doEncode(CParamPool &IOBuff) const;

public:
    unsigned short m_nPetTypeID;
    unsigned char  m_nAttributeType;
    unsigned char  m_nQualityType;
};

class GameMsg_S2C_GetPetLibraryTypeListRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_GetPetLibraryTypeListRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    std::list<PetLibraryMsg> m_listPetType;
};


class GameMsg_C2S_GetPetLibraryList : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_GetPetLibraryList);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    std::list<unsigned short> m_listPetType;
};

class GameMsg_S2C_GetPetLibraryListRes : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_GetPetLibraryListRes);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    std::list<PetMsgInfo> m_listPet;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_SyncPetChangedInfoToOthers : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncPetChangedInfoToOthers);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;
    bool                m_bCarriedPet;
    unsigned char       m_nSceneID;
    PlayerPet           m_pet;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_SyncPetChangeInfoToSelf : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncPetChangeInfoToSelf);

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    PlayerPet           m_pet;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_ReportPetDanceEmotion : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_ReportPetDanceEmotion);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned char m_nEmotionType;
};

class GameMsg_S2C_SyncPetDanceEmotion : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncPetDanceEmotion);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    unsigned char m_nEmotionType;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_CastPetDanceSkill : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_CastPetDanceSkill);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;          // 角色ID
    unsigned int        m_nPetID;           // 角色的宠物ID
    std::list<SkillID>  m_listSkillList;    // 释放的技能列表
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_PetGuideRequest : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_PetGuideRequest);

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned char m_nGuideStep;
};

class GameMsg_S2C_PetGuideResponse : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_PetGuideResponse);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nPetEggType;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_RefreshPetSkillList : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RefreshPetSkillList);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    PlayerPetSkill m_petSkill;
};

#endif // __GAMEMSG_PET_H__

