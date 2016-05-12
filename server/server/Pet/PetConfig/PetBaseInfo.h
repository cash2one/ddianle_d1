/*
 * PetBaseInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-12 13:52:01
 */

#ifndef __PETBASEINFO_H__
#define __PETBASEINFO_H__

#include "../../mall/Currency.h"
#include "../../Macros/Macros.h"

#include <vector>
#include <string>

class CFileStream;

enum ePetAttributeType
{
    ePetAttributeType_None = 0,

    ePetAttributeType_Earth,    // 土系
    ePetAttributeType_Air,      // 风系
    ePetAttributeType_Water,    // 水系
    ePetAttributeType_Fire,     // 火系

    ePetAttributeType_Max
};

/**
* PetBaseInfo:
* 
*/

class PetBaseConfig
{
public:
    PetBaseConfig();
    virtual ~PetBaseConfig();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(unsigned short, m_nPetTypeID, PetTypeID);
    CLASS_PROPERTY_READONLY(unsigned char, m_nAttributeType, AttributeType);
    CLASS_PROPERTY_READONLY(unsigned short, m_nTopLevel, TopLevel);
    CLASS_PROPERTY_READONLY(float, m_fExpRatio, ExpRatio);
    CLASS_PROPERTY_READONLY(unsigned short, m_nDanceSkillCount, DanceSkillCount);
    CLASS_PROPERTY_READONLY(unsigned short, m_nFightSkillCount, FightSkillCount);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strDefaultName, DefaultName);
    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_resetSkillCost, ResetSkillCost);
};

/**
* PetPotentialConfig:
* 宠物潜质配置
*/
class PetPotentialConfig
{
public:
    PetPotentialConfig();
    virtual ~PetPotentialConfig();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY(unsigned char, m_nPotentialID, PotentialID);
    CLASS_PROPERTY_BY_REF(std::string, m_strPotential, Potential);
    CLASS_PROPERTY(unsigned short, m_nTopLevel, TopLevel);
    CLASS_PROPERTY(unsigned char, m_nTopQuality, TopQuality);
    CLASS_PROPERTY(unsigned char, m_nTopEvolutionTimes, TopEvolutionTimes);
    CLASS_PROPERTY(unsigned char, m_nMaxSkillCount, MaxSkillCount);
    CLASS_PROPERTY_BY_REF(std::string, m_strPotentialTip, PotentialTip);
};

#endif // __PETBASEINFO_H__

