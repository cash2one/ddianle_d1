#include "PetIncubatorInfo.h"

#include "../../../socket/Windefine.h"

#include "../../../datastructure/datainfor/FileStream.h"


PetIncubatorInfo::PetIncubatorInfo(void)
    : m_nLevel(0)
{
}

PetIncubatorInfo::~PetIncubatorInfo(void)
{
}

bool PetIncubatorInfo::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen() )
        return false;

    rFileStream.ReadUShort(m_nLevel);

    unsigned char nCurrencyType = 0;
    unsigned int nCurrencyAmount = 0;

    rFileStream.ReadUChar(nCurrencyType);
    rFileStream.ReadUInt(nCurrencyAmount);

    m_levelUpCost.SetType(nCurrencyType);
    m_levelUpCost.SetAmount(nCurrencyAmount);

    ASSERT(m_nLevel > 0);
    ASSERT(m_levelUpCost.IsValid());

    return true;
}

