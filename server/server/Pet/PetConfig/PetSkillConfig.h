/*
 * PetSkillConfig.h
 * Description: ���＼������
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

    ePetSkillType_Dance                     = 1,    // ���輼��

    ePetSkillType_DanceTotalScore           = 2,    // ����ģʽ�ܷ�����
    ePetSkillType_DanceModePerfectScore     = 3,    // ����ģʽPerfect����������
    ePetSkillType_DanceLevelPerfectScore    = 4,    // �����Ѷ�Perfect����������
    ePetSkillType_DanceModeExp              = 5,    // ����ģʽ��������
    ePetSkillType_DanceLevelExp             = 6,    // �����ѶȾ�������
    ePetSkillType_DanceDropMoney            = 7,    // �����Ǯ��������
    ePetSkillType_DanceDropRate             = 8,    // ����ؿ����伸������

    ePetSkillType_Fight                     = 128,  // ս������

    ePetSkillType_Max                       = 255,
};


/**
*  ���＼�ܻ���������
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

    // ���ܻ�����ֵ����
    CLASS_PROPERTY_READONLY(SkillType, m_nType, Type);
    CLASS_PROPERTY_READONLY(SkillID, m_nSkillID, SkillID);
    CLASS_PROPERTY_READONLY(unsigned short, m_nMaxLevel, MaxLevel);

    // ���ܳ�ʼ��ֵ����
    CLASS_PROPERTY_READONLY(int, m_nSkillParam, SkillParam);    // �̶����������缼�ܿ���ֻ��ģʽ��Ч����ô���ֵ����ģʽֵ
    CLASS_PROPERTY_READONLY(int, m_nMinValue, MinValue);        // ���ü���ʱ���������ϵĶ�̬������ֵ��[Min, Max]֮�����
    CLASS_PROPERTY_READONLY(int, m_nMaxValue, MaxValue); 

    // ���������������
    CLASS_PROPERTY_READONLY(int, m_nLevelUpValue, LevelUpValue);// ��������ʱ�����ӵļ�����ֵ��
    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_levelUpCost, LevelUpCost);

    // ���ü���Ȩ��Weight
    CLASS_PROPERTY_READONLY(int, m_nRandomWeight, RandomWeight);

    // ����������Ϣ
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strName, Name);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strTip, Tip);

    // ������ʾ����
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
* ���＼�ܽ�������
*/
class PetSkillUnlockConfig
{
public:
    PetSkillUnlockConfig();
    virtual ~PetSkillUnlockConfig();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY(unsigned char, m_nSkillCount, SkillCount);                       // Ҫ�����ļ�������
    CLASS_PROPERTY(unsigned short, m_nRequireLevel, RequireLevel);                  // ����ȼ�Ҫ��
    CLASS_PROPERTY(unsigned char, m_nRequireQualityType, RequireQualityType);       // Ʒ��Ҫ��
    CLASS_PROPERTY(unsigned char, m_nRequireEvolutionTimes, RequireEvolutionTimes); // ����Ҫ��
    CLASS_PROPERTY_BY_REF(std::string, m_strUnlockTip, UnlockTip);                  // ����˵��
};

#endif // __PET_SKILL_CONFIG_H__

