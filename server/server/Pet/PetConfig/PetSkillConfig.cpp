#include "PetSkillConfig.h"

#include "../../../socket/Windefine.h"

#include "../../../datastructure/datainfor/FileStream.h"


PetSkillConfig::PetSkillConfig(void)
    : m_nType(0)
    , m_nSkillID(0)
    , m_nMaxLevel(0)
    , m_nSkillParam(0)
    , m_nMinValue(0)
    , m_nMaxValue(0)
    , m_nLevelUpValue(0)
    , m_nRandomWeight(0)
    , m_strName("skill name")
    , m_strTip("skill tip")
    , m_strAtlasName("skill atlas")
    , m_strSpriteName("skill sprite")
    , m_strLevelUpTip("skill level up tip")
{
}

PetSkillConfig::~PetSkillConfig(void)
{
}

bool PetSkillConfig::IsDanceSkill(SkillType nType)
{
    return nType > ePetSkillType_Dance && nType < ePetSkillType_Fight;
}

bool PetSkillConfig::IsFightSkill(SkillType nType)
{
    return nType > ePetSkillType_Fight && nType < ePetSkillType_Max;
}

int PetSkillConfig::RandomSkillValue()const
{
    return Random(m_nMinValue, m_nMaxValue);
}

bool PetSkillConfig::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return false;

    unsigned char nCurrencyType = 0;
    unsigned int nCurrencyAmount = 0;

    rFileStream.ReadUChar(m_nType);
    rFileStream.ReadString2(m_strName);
    rFileStream.ReadString2(m_strTip);
    rFileStream.ReadUShort(m_nSkillID);
    rFileStream.ReadUShort(m_nMaxLevel);
    rFileStream.ReadInt(m_nSkillParam);
    rFileStream.ReadInt(m_nMinValue);
    rFileStream.ReadInt(m_nMaxValue);
    rFileStream.ReadInt(m_nLevelUpValue);
    rFileStream.ReadUChar(nCurrencyType);
    rFileStream.ReadUInt(nCurrencyAmount);
    rFileStream.ReadInt(m_nRandomWeight);
    rFileStream.ReadString2(m_strAtlasName);
    rFileStream.ReadString2(m_strSpriteName);
    rFileStream.ReadString2(m_strLevelUpTip);

    m_levelUpCost.SetType(nCurrencyType);
    m_levelUpCost.SetAmount(nCurrencyAmount);

    ASSERT(m_nType > ePetSkillType_None && m_nType < ePetSkillType_Max);
    ASSERT(m_nSkillID > 0);
    ASSERT(m_nMaxLevel > 0);

    ASSERT(m_nMinValue >= 0 && m_nMinValue <= m_nMaxValue);
    ASSERT(m_nRandomWeight >= 0);

    ASSERT(!m_strName.empty() && !m_strTip.empty());
    ASSERT(!m_strAtlasName.empty() && !m_strSpriteName.empty());

    return true;
}

PetSkillUnlockConfig::PetSkillUnlockConfig()
    : m_nSkillCount(0)
    , m_nRequireLevel(0)
    , m_nRequireQualityType(0)
    , m_nRequireEvolutionTimes(0)
    , m_strUnlockTip("skill unlock tip")
{
}

PetSkillUnlockConfig::~PetSkillUnlockConfig()
{
}

bool PetSkillUnlockConfig::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return false;

    rFileStream.ReadUChar(m_nSkillCount);
    rFileStream.ReadUShort(m_nRequireLevel);
    rFileStream.ReadUChar(m_nRequireQualityType);
    rFileStream.ReadUChar(m_nRequireEvolutionTimes);
    rFileStream.ReadString2(m_strUnlockTip);

    ASSERT(m_nSkillCount > 0);
    ASSERT(m_nRequireLevel > 0);
    ASSERT(m_nRequireLevel > 0);
    ASSERT(m_nRequireQualityType > 0);

    return true;
}


