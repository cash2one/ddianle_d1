#include "PetQualityInfo.h"

#include "../../../socket/Windefine.h"

#include "../../../datastructure/datainfor/FileStream.h"


//////////////////////////////////////////////////////////////////////////

PetImproveCondition::PetImproveCondition()
{
}

PetImproveCondition::~PetImproveCondition()
{
}

bool PetImproveCondition::LoadInfoFromFile(CFileStream &rFileStream)
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

    ASSERT(m_money.IsValid() );

    return true;
}

//////////////////////////////////////////////////////////////////////////

PetQualityInfo::PetQualityInfo(void)
    : m_nPetTypeID(0)
    , m_nMaxQualityType(0)
{
}

PetQualityInfo::~PetQualityInfo(void)
{
}

bool PetQualityInfo::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen() )
        return false;

    rFileStream.ReadUShort(m_nPetTypeID);
    rFileStream.ReadUChar(m_nMaxQualityType);

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

        m_vectImproveLevel.push_back(nValue);
    }

    rFileStream.ReadUShort(nCount);
    for (i = 0; i < nCount; ++i)
    {
        PetImproveCondition condition;

        if (condition.LoadInfoFromFile(rFileStream) )
        {
            m_vectCondition.push_back(condition);
        }
    }

    ASSERT(m_nPetTypeID > 0);
    ASSERT(ePetQualityType_None < m_nMaxQualityType && m_nMaxQualityType < ePetQualityType_Max);
    ASSERT(m_vectEndurance.size() == m_nMaxQualityType);
    ASSERT(m_vectImproveLevel.size() >= (m_nMaxQualityType - 1) );

    return true;
}

unsigned short PetQualityInfo::GetMaxEndurance(unsigned char nCurrentQualityType) const
{
    if (ePetQualityType_None < nCurrentQualityType && 
        nCurrentQualityType <= m_nMaxQualityType && 
        nCurrentQualityType <= m_vectEndurance.size() )
    {
        return m_vectEndurance[nCurrentQualityType - 1];
    }

    return 0;
}

unsigned short PetQualityInfo::GetNextImproveLevel(unsigned char nCurrentQualityType, unsigned short nDefaultLevel) const
{
    unsigned short nImproveLevel = nDefaultLevel;

    if (ePetQualityType_None < nCurrentQualityType && 
        nCurrentQualityType < m_nMaxQualityType && 
        nCurrentQualityType <= m_vectImproveLevel.size() )
    {
        nImproveLevel = m_vectImproveLevel[nCurrentQualityType - 1];
    }

    return nImproveLevel;
}

bool PetQualityInfo::CanImproveQuality(unsigned char nCurrentQualityType, unsigned short nLevel) const
{
    if (ePetQualityType_None < nCurrentQualityType && 
        nCurrentQualityType < m_nMaxQualityType && 
        nCurrentQualityType <= m_vectImproveLevel.size() && 
        m_vectImproveLevel[nCurrentQualityType - 1] <= nLevel)
    {
        return true;
    }

    return false;
}

bool PetQualityInfo::GetImproveQualityCondition(unsigned char nCurrentQualityType, PetImproveCondition &rInfo) const
{
    const PetImproveCondition *pInfo = GetImproveQualityCondition(nCurrentQualityType);

    if (pInfo != NULL)
    {
        rInfo = *pInfo;

        return true;
    }

    return false;
}

const PetImproveCondition* PetQualityInfo::GetImproveQualityCondition(unsigned char nCurrentQualityType) const
{
    if (ePetQualityType_None < nCurrentQualityType && 
        nCurrentQualityType < m_nMaxQualityType && 
        nCurrentQualityType <= m_vectCondition.size() )
    {
        return &m_vectCondition[nCurrentQualityType - 1];
    }

    return NULL;
}

