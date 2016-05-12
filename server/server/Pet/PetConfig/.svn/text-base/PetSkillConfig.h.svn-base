/*
 * PetSkillConfig.h
 * Description: 宠物技能配置
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: xieshaobin
 * Create time: 2015-6-25 09:52:33
 */

#ifndef __PET_SKILL_CONFIG_H__
#define __PET_SKILL_CONFIG_H__

#include "../../mall/Currency.h"
#include "../../Macros/Macros.h"

#include "../../../datastructure/DataStruct_Base.h"

#include <string>

class CFileStream;
class CParamPool;

enum EPetSkillType
{
    ePetSkillType_None = 0,

    ePetSkillType_Dance                     = 1,    // 跳舞技能

    ePetSkillType_DanceTotalScore           = 2,    // 跳舞模式总分增加
    ePetSkillType_DanceModePerfectScore     = 3,    // 跳舞模式Perfect基础分增加
    ePetSkillType_DanceLevelPerfectScore    = 4,    // 跳舞难度Perfect基础分增加
    ePetSkillType_DanceModeExp              = 5,    // 跳舞模式经验增加
    ePetSkillType_DanceLevelExp             = 6,    // 跳舞难度经验增加
    ePetSkillType_DanceDropMoney            = 7,    // 跳舞金钱掉落增加
    ePetSkillType_DanceDropRate             = 8,    // 跳舞关卡掉落几率增加

    ePetSkillType_Fight                     = 128,  // 战斗技能

    ePetSkillType_Max                       = 255,
};


/**
*  宠物技能基础配置类
*/
class PetSkillConfig
{
public:
    PetSkillConfig();
    virtual ~PetSkillConfig();

public:
    static bool IsDanceSkill(SkillType nType);
    static bool IsFightSkill(SkillType nType);

    int RandomSkillValue()const;

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    // 技能基础数值配置
    CLASS_PROPERTY_READONLY(SkillType, m_nType, Type);
    CLASS_PROPERTY_READONLY(SkillID, m_nSkillID, SkillID);
    CLASS_PROPERTY_READONLY(unsigned short, m_nMaxLevel, MaxLevel);

    // 技能初始数值配置
    CLASS_PROPERTY_READONLY(int, m_nSkillParam, SkillParam);    // 固定参数，比如技能可能只对模式有效，那么这个值就是模式值
    CLASS_PROPERTY_READONLY(int, m_nMinValue, MinValue);        // 重置技能时，宠物身上的动态技能数值在[Min, Max]之间随机
    CLASS_PROPERTY_READONLY(int, m_nMaxValue, MaxValue); 

    // 技能升级相关配置
    CLASS_PROPERTY_READONLY(int, m_nLevelUpValue, LevelUpValue);// 技能升级时，增加的技能数值点
    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_levelUpCost, LevelUpCost);

    // 重置技能权重Weight
    CLASS_PROPERTY_READONLY(int, m_nRandomWeight, RandomWeight);

    // 技能描述信息
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strName, Name);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strTip, Tip);

    // 技能显示配置
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strAtlasName, AtlasName);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strSpriteName, SpriteName);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strLevelUpTip, LevelUpTip);
};

class PetSkillResetConfig
{
public:
    PetSkillResetConfig();
    virtual ~PetSkillResetConfig();

    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_resetSkillCost, ResetSkillCost);
};

/**
* 宠物技能解锁配置
*/
class PetSkillUnlockConfig
{
public:
    PetSkillUnlockConfig();
    virtual ~PetSkillUnlockConfig();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY(unsigned char, m_nSkillCount, SkillCount);                       // 要解锁的技能数量
    CLASS_PROPERTY(unsigned short, m_nRequireLevel, RequireLevel);                  // 宠物等级要求
    CLASS_PROPERTY(unsigned char, m_nRequireQualityType, RequireQualityType);       // 品质要求
    CLASS_PROPERTY(unsigned char, m_nRequireEvolutionTimes, RequireEvolutionTimes); // 进化要求
    CLASS_PROPERTY_BY_REF(std::string, m_strUnlockTip, UnlockTip);                  // 解锁说明
};

#endif // __PET_SKILL_CONFIG_H__

