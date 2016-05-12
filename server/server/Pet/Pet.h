/*
 * Pet.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-13 11:31:01
 */

#ifndef __PET_H__
#define __PET_H__

#include "../Macros/Macros.h"
#include "PetConfig/PetBaseInfo.h"
#include "PetConfig/PetQualityInfo.h"
#include "PetConfig/PetEvolutionInfo.h"
#include "PetConfig/PetSkillConfig.h"

#include <set>
#include <string>

struct PetDB;

enum ePetState
{
    ePetState_Rest = 0,         // 休息
    ePetState_Carrying,         // 携带
    ePetState_Destroy,          // 分解

    ePetState_Max
};

/**
 * 宠物技能实例
 */
class PetSkill
{
public:
    PetSkill();
    virtual ~PetSkill();

public:
    // virtual void OnCast() = 0; // 现在的跳舞技能全是被动技能，暂不需要此方法，待增加主动技能后需要增加

protected:
    CLASS_PROPERTY(int, m_nSkillIndex, SkillIndex);
    CLASS_PROPERTY(SkillID, m_nSkillID, SkillID);
    CLASS_PROPERTY(int, m_nLevel, Level);
    CLASS_PROPERTY(int, m_nSkillValue, SkillValue);
};

/**
* Pet:
* 
*/
class Pet
{
protected:
    Pet();

public:
    CLASS_CREATE_FUNC(Pet);
    virtual ~Pet();

public:
    void SetTypeID(unsigned short nTypeID);
    void SetPotentialID(unsigned char nPotentialID);

    bool IsResting() const;
    bool IsCarrying() const;
    bool IsDestroyed() const;

    void SetState(unsigned char nState);

    bool CanImproveQuality() const;
    unsigned char GetMaxQuality() const;
    void ImproveQuality();

    bool CanEvolution() const;
    unsigned char GetMaxEvolutionTimes() const;
    void Evolution();

    unsigned short GetMaxEndurance() const;

    bool CanLevelUp() const;
    unsigned short GetTopLevel() const;
    void LevelUp();
    unsigned int GetMaxExpOfLevel() const;

    void PrepareSkillIndex();
    bool HasSkill(SkillID nSkillID) const;
    bool HasSkill(SkillType nSkillType) const;
    bool IsActivatedSkill(const PetSkill &skill) const;
    PetSkill *GetSkill(SkillID nSkillID) const;
    PetSkill *GetSkill(SkillType nSkillType) const;
    void SetSkill(SkillID nOldSkillID, PetSkill *skill);
    void RemoveSkill(SkillID nSkillID);
    void RemoveSkill(SkillType nSkillType);
    int GetDanceSkillCount() const;
    int GetFightSkillCount() const;

    void GetSkillList(std::list<PetSkill *> &skillList) const;
    void GetOrderedSkillList(std::list<PetSkill *> &skillList) const; // 查询根据技能栏位排序好的技能列表
    void GetDanceSkillList(std::list<PetSkill *> &skillList) const;
    void GetActivatedDanceSkillList(std::list<PetSkill *> &skillList) const;
    int GetActivatedSkillCount() const;
    void GetFightSkillList(std::list<PetSkill *> &skillList) const;
    void ClearSkill();
    void ClearSkillExclude(const std::set<SkillID> &excludeSkillList);

    void Init(unsigned int nPetID, unsigned char nState, const std::string &strName, unsigned char nPotentialID,
        unsigned short nLevel, unsigned int nExp, unsigned short nEndurance, unsigned int nEnduranceRecoveryTime,
        unsigned char nAttributeType, unsigned char nQualityType, unsigned char nEvolutionTimes,
        unsigned short nPetTypeID, const std::string &strSkillList, unsigned int nResetSkillTime, int nResetSkillTimes);
    void Init(const PetDB &petDb);
    void ToPetDB(PetDB &petDb);

    void LoadSkill(const std::string &strSkillList);
    void SaveSkill(std::string &strSkillList);

protected:
    unsigned short ResetMaxEndurance() const;

protected:
    CLASS_PROPERTY(unsigned int, m_nID, ID);                            // 宠物ID，由数据库生成
    CLASS_PROPERTY_READONLY(unsigned char, m_nState, State);            // 状态
    CLASS_PROPERTY(unsigned short, m_nLevel, Level);                    // 等级
    CLASS_PROPERTY(unsigned int, m_nExp, Exp);                          // 当前等级经验
    CLASS_PROPERTY(unsigned char, m_nQualityType, QualityType);         // 品质类型
    CLASS_PROPERTY(unsigned char, m_nEvolutionTimes, EvolutionTimes);   // 进化次数
    CLASS_PROPERTY(unsigned char, m_nAttributeType, AttributeType);     // 宠物属性
    CLASS_PROPERTY_READONLY(unsigned short, m_nTypeID, TypeID);         // 宠物类型ID, Set方法手写
    CLASS_PROPERTY_READONLY(unsigned char, m_nPotentialID, PotentialID);// 宠物潜质, Set方法手写
    CLASS_PROPERTY_BY_REF(std::string, m_strName, Name);                // 宠物名
    CLASS_PROPERTY(unsigned short, m_nEndurance, Endurance);            // 当前耐力
    CLASS_PROPERTY(unsigned int, m_nEnduranceRecoveryTime, EnduranceRecoveryTime);// 当前耐力恢复时间
    CLASS_PROPERTY(unsigned int, m_nResetSkillTime, ResetSkillTime);    // 技能重置时间
    CLASS_PROPERTY(int, m_nResetSkillTimes, ResetSkillTimes);           // 当天技能重置次数

protected:
    std::map<SkillID, PetSkill *> m_petSkill;                           // 该宠物具有的技能列表 -- 存放指针以方便后期技能功能扩展
    std::map<int, SkillID> m_petSkillIndex;                             // 宠物技能栏对应的技能ID，技能栏编号->技能ID

    unsigned short m_nMaxEndurance;                                     // 最大耐力，内存值，不写入数据库

    const PetBaseConfig *m_pBaseConfig;
    const PetPotentialConfig *m_pPotentialConfig;
    const PetQualityInfo *m_pQualityConfig;
    const PetEvolutionInfo *m_pEvolutionConfig;
};

#endif // __PET_H__

