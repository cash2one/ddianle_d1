#include "PetExchangeInfo.h"
#include "PetBaseInfo.h"
#include "PetQualityInfo.h"

#include "../../../socket/Log.h"
#include "../../../socket/Windefine.h"

#include "../../../datastructure/datainfor/FileStream.h"


PetExchangeInfo::PetExchangeInfo(void)
{
    for (unsigned char nAttributeType = ePetAttributeType_None + 1; nAttributeType < ePetAttributeType_Max; ++nAttributeType)
    {
        std::vector<std::list<CItem> > inner;

        for (unsigned char nQualityType = ePetQualityType_None + 1; nQualityType < ePetQualityType_Max; ++nQualityType)
        {
            inner.push_back(std::list<CItem>() );
        }

        m_allExchangeInfos.push_back(inner);
    }
}

PetExchangeInfo::~PetExchangeInfo(void)
{
    for (unsigned char nAttributeType = ePetAttributeType_None + 1; nAttributeType < ePetAttributeType_Max; ++nAttributeType)
    {
        for (unsigned char nQualityType = ePetQualityType_None + 1; nQualityType < ePetQualityType_Max; ++nQualityType)
        {
            m_allExchangeInfos[nAttributeType - 1][nQualityType - 1].clear();
        }

        m_allExchangeInfos[nAttributeType - 1].clear();
    }

    m_allExchangeInfos.clear();
}

void PetExchangeInfo::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen() )
        return;

    unsigned char nAttributeType = 0;
    unsigned char nQualityType = 0;
    unsigned int nItemType = 0;
    unsigned short nItemCount = 0;
    unsigned short nCount = 0;

    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        rFileStream.ReadUChar(nAttributeType);
        rFileStream.ReadUChar(nQualityType);
        rFileStream.ReadUInt(nItemType);
        rFileStream.ReadUShort(nItemCount);

        ASSERT(ePetAttributeType_None < nAttributeType && nAttributeType < ePetAttributeType_Max);
        ASSERT(ePetQualityType_None < nQualityType && nQualityType < ePetQualityType_Max);
        ASSERT(nItemType > 0);
        ASSERT(nItemCount > 0);

        CItem item(nItemType, nItemCount, -1, false);

        AddExchangeInfo(nAttributeType, nQualityType, item);
    }
}

bool PetExchangeInfo::IsValid() const
{
    return true;
}

void PetExchangeInfo::GetExchangeItem(unsigned char nAttributeType, unsigned char nQualityType, std::list<CItem> &rItem) const
{
    if (ePetAttributeType_None < nAttributeType && nAttributeType < ePetAttributeType_Max && 
        ePetQualityType_None < nQualityType && nQualityType < ePetQualityType_Max)
    {
        rItem = m_allExchangeInfos[nAttributeType - 1][nQualityType - 1];
    }
}

void PetExchangeInfo::AddExchangeItemIntoMap(unsigned char nAttributeType, unsigned char nQualityType, std::map<itemtype_t, CItem> &rItem) const
{
    if (ePetAttributeType_None < nAttributeType && nAttributeType < ePetAttributeType_Max && 
        ePetQualityType_None < nQualityType && nQualityType < ePetQualityType_Max)
    {
        const std::list<CItem> &ref = m_allExchangeInfos[nAttributeType - 1][nQualityType - 1];
        std::map<itemtype_t, CItem>::iterator mapItr;

        for (std::list<CItem>::const_iterator listItr = ref.begin(); listItr != ref.end(); ++listItr)
        {
            mapItr = rItem.find(listItr->m_nItemType);

            if (mapItr == rItem.end() )
            {
                rItem.insert(std::make_pair(listItr->m_nItemType, *listItr) );
            }
            else
            {
                mapItr->second.m_nItemCount += listItr->m_nItemCount;
            }
        }
    }
}

void PetExchangeInfo::AddExchangeInfo(unsigned char nAttributeType, unsigned char nQualityType, const CItem &rItem)
{
    m_allExchangeInfos[nAttributeType - 1][nQualityType - 1].push_back(rItem);
}

