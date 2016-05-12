#include "PetEggBaseInfo.h"

#include "../../../socket/Windefine.h"

#include "../../../datastructure/datainfor/FileStream.h"


PetEggBaseInfo::PetEggBaseInfo(void)
    : m_nTypeID(0)
    , m_nIncubationTime(0)
    , m_nMinIncubatorLevel(0)
{
}

PetEggBaseInfo::~PetEggBaseInfo(void)
{
}

bool PetEggBaseInfo::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen() )
        return false;

    rFileStream.ReadUInt(m_nTypeID);
    rFileStream.ReadString2(m_strModeName);
    rFileStream.ReadUInt(m_nIncubationTime);
    rFileStream.ReadUShort(m_nMinIncubatorLevel);

    unsigned char nType = 0;
    unsigned int nAmount = 0;

    rFileStream.ReadUChar(nType);
    rFileStream.ReadUInt(nAmount);

    m_incubateCost.SetType(nType);
    m_incubateCost.SetAmount(nAmount);

    rFileStream.ReadUChar(nType);
    rFileStream.ReadUInt(nAmount);

    m_speedUpPreMinuteCost.SetType(nType);
    m_speedUpPreMinuteCost.SetAmount(nAmount);

    ASSERT(m_nTypeID > 0);
    ASSERT(m_strModeName.size() > 0);
    ASSERT(m_nIncubationTime > 0);
    ASSERT(m_incubateCost.IsValid() );
    ASSERT(m_speedUpPreMinuteCost.IsValid() );

    return true;
}

