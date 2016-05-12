#include "PetEvolutionInfo.h"

#include "../../../socket/Windefine.h"

#include "../../../datastructure/datainfor/FileStream.h"


//////////////////////////////////////////////////////////////////////////

PetEvolutionCondition::PetEvolutionCondition()
    : m_nRate(0)
    , m_bExtraItem(false)
{
}

PetEvolutionCondition::~PetEvolutionCondition()
{
}

bool PetEvolutionCondition::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen() )
        return false;

    unsigned char nCurrentyType = 0;
    unsigned int nCurrentyAmount = 0;

    rFileStream.ReadUChar(nCurrentyType);
    rFileStream.ReadUInt(nCurrentyAmount);

    m_money.SetType(nCurrentyType);
    m_money.SetAmount(nCurrentyAmount);

    unsigned short nItemCount = 0;
    rFileStream.ReadUShort(nItemCount);

    for (unsigned short i = 0; i < nItemCount; ++i)
    {
        CItem item;

        rFileStream.ReadUInt(item.m_nItemType);
        rFileStream.ReadUShort(item.m_nItemCount);
        rFileStream.ReadInt(item.m_nValidTime);

        m_items.push_back(item);
    }

    rFileStream.ReadInt(m_nRate);

    rFileStream.ReadBool(m_bExtraItem);

    if (m_bExtraItem)
    {
        rFileStream.ReadUInt(m_itemExtra.m_nItemType);
        rFileStream.ReadUShort(m_itemExtra.m_nItemCount);
        rFileStream.ReadInt(m_itemExtra.m_nValidTime);
    }

    ASSERT(m_money.IsValid() );
    ASSERT(0 <= m_nRate && m_nRate <= MAX_RATE);

    return true;
}

//////////////////////////////////////////////////////////////////////////

PetEvolutionInfo::PetEvolutionInfo(void)
    : m_nPetTypeID(0)
    , m_nMaxEvolutionTimes(0)
{
}

PetEvolutionInfo::~PetEvolutionInfo(void)
{
}

bool PetEvolutionInfo::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen() )
        return false;

    rFileStream.ReadUShort(m_nPetTypeID);
    rFileStream.ReadUChar(m_nMaxEvolutionTimes);

    unsigned short i = 0;
    unsigned short nCount = 0;
    unsigned short nValue = 0;

    rFileStream.ReadUShort(nCount);
    for (i = 0; i < nCount; ++i)
    {
        rFileStream.ReadUShort(nValue);

        ASSERT(nValue > 0);

        m_vectEndurance.push_back(nValue);
    }

    rFileStream.ReadUShort(nCount);
    for (i = 0; i < nCount; ++i)
    {
        rFileStream.ReadUShort(nValue);

        ASSERT(nValue > 0);

        m_vectEvolutionLevel.push_back(nValue);
    }

    rFileStream.ReadUShort(nCount);
    for (i = 0; i < nCount; ++i)
    {
        PetEvolutionCondition condition;

        if (condition.LoadInfoFromFile(rFileStream) )
        {
            m_vectCondition.push_back(condition);
        }
    }

    ASSERT(m_nPetTypeID > 0);
    ASSERT(m_nMaxEvolutionTimes > 0);
    ASSERT(m_vectEndurance.size() == m_nMaxEvolutionTimes);
    ASSERT(m_vectEvolutionLevel.size() >= (m_nMaxEvolutionTimes - 1) );

    return true;
}

unsigned short PetEvolutionInfo::GetMaxEndurance(unsigned char nCurrentEvolutionTimes) const
{
    if (0 < nCurrentEvolutionTimes && 
        nCurrentEvolutionTimes <= m_nMaxEvolutionTimes && 
        nCurrentEvolutionTimes <= m_vectEndurance.size() )
    {
        return m_vectEndurance[nCurrentEvolutionTimes - 1];
    }

    return 0;
}

unsigned short PetEvolutionInfo::GetNextEvolutionLevel(unsigned char nNextEvolutionTimes, unsigned short nDefaultLevel) const
{
    unsigned short nEvolutionLevel = nDefaultLevel;

    if (0 < nNextEvolutionTimes && 
        nNextEvolutionTimes <= m_nMaxEvolutionTimes && 
        nNextEvolutionTimes <= m_vectEvolutionLevel.size() )
    {
        nEvolutionLevel = m_vectEvolutionLevel[nNextEvolutionTimes - 1];
    }

    return nEvolutionLevel;
}

bool PetEvolutionInfo::CanEvolution(unsigned char nNextEvolutionTimes, unsigned short nLevel) const
{
    if (0 < nNextEvolutionTimes && 
        nNextEvolutionTimes <= m_nMaxEvolutionTimes && 
        nNextEvolutionTimes <= m_vectEvolutionLevel.size() && 
        m_vectEvolutionLevel[nNextEvolutionTimes - 1] <= nLevel)
    {
        return true;
    }

    return false;
}

bool PetEvolutionInfo::GetEvolutionCondition(unsigned char nNextEvolutionTimes, PetEvolutionCondition &rInfo) const
{
    const PetEvolutionCondition *pInfo = GetEvolutionCondition(nNextEvolutionTimes);

    if (pInfo != NULL)
    {
        rInfo = *pInfo;

        return true;
    }

    return false;
}

const PetEvolutionCondition* PetEvolutionInfo::GetEvolutionCondition(unsigned char nNextEvolutionTimes) const
{
    if (0 < nNextEvolutionTimes && 
        nNextEvolutionTimes <= m_nMaxEvolutionTimes && 
        nNextEvolutionTimes <= m_vectCondition.size() )
    {
        return &m_vectCondition[nNextEvolutionTimes - 1];
    }

    return NULL;
}

