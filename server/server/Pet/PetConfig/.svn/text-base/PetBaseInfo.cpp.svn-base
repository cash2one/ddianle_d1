#include "PetBaseInfo.h"

#include "../../../socket/Windefine.h"

#include "../../../datastructure/datainfor/FileStream.h"


PetBaseConfig::PetBaseConfig(void)
    : m_nPetTypeID(0)
    , m_nAttributeType(ePetAttributeType_None)
    , m_nTopLevel(0)
    , m_fExpRatio(1)
    , m_nDanceSkillCount(0)
    , m_nFightSkillCount(0)
    , m_strDefaultName("")
{
}

PetBaseConfig::~PetBaseConfig(void)
{
}

bool PetBaseConfig::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen() )
        return false;

    rFileStream.ReadUShort(m_nPetTypeID);
    rFileStream.ReadString2(m_strDefaultName);
    rFileStream.ReadUChar(m_nAttributeType);
    rFileStream.ReadUShort(m_nTopLevel);
    rFileStream.ReadFloat(m_fExpRatio);

    ASSERT(m_nPetTypeID > 0);
    ASSERT(m_strDefaultName.size() > 0);
    ASSERT(ePetAttributeType_None < m_nAttributeType && m_nAttributeType < ePetAttributeType_Max);
    ASSERT(m_nTopLevel > 0);
    ASSERT(m_fExpRatio > 0);

    rFileStream.ReadUShort(m_nDanceSkillCount);
    rFileStream.ReadUShort(m_nFightSkillCount);

    unsigned char nCurrencyType = 0;
    unsigned int nCurrencyAmount = 0;

    rFileStream.ReadUChar(nCurrencyType);
    rFileStream.ReadUInt(nCurrencyAmount);

    m_resetSkillCost.SetType(nCurrencyType);
    m_resetSkillCost.SetAmount(nCurrencyAmount);

    ASSERT(m_resetSkillCost.IsValid() );

    return true;
}


//////////////////////////////////////////////////////////////////////////
PetPotentialConfig::PetPotentialConfig()
    : m_nPotentialID(0)
    , m_strPotential("D")
    , m_nTopLevel(0)
    , m_nTopQuality(0)
    , m_nTopEvolutionTimes(0)
    , m_nMaxSkillCount(0)
    , m_strPotentialTip("potential tip")
{
}

PetPotentialConfig::~PetPotentialConfig()
{
}

bool PetPotentialConfig::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen() )
        return false;

    rFileStream.ReadUChar(m_nPotentialID);
    rFileStream.ReadString2(m_strPotential);
    rFileStream.ReadUShort(m_nTopLevel);
    rFileStream.ReadUChar(m_nTopQuality);
    rFileStream.ReadUChar(m_nTopEvolutionTimes);
    rFileStream.ReadUChar(m_nMaxSkillCount);
    rFileStream.ReadString2(m_strPotentialTip);

    ASSERT(m_nPotentialID > 0);
    ASSERT(!m_strPotential.empty());
    ASSERT(m_nTopLevel > 0);

    return true;
}


