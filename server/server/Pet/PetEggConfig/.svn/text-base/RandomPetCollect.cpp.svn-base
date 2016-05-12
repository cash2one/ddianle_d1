#include "RandomPetCollect.h"

#include "../../../socket/Utility.h"


RandomPetInfo::RandomPetInfo(unsigned short nPetTypeID, unsigned char nPotentialID, bool bAnnounce)
    : m_nPetTypeID(nPetTypeID)
    , m_nPotentialID(nPotentialID)
    , m_bAnnounce(bAnnounce)
{
}

RandomPetInfo::~RandomPetInfo(void)
{
}

//////////////////////////////////////////////////////////////////////////

RandomPetCollect::RandomPetCollect(void)
{
}

RandomPetCollect::~RandomPetCollect(void)
{
}

bool RandomPetCollect::AddRandomPetInfo(RandomPetInfo& rInfo, int nRate)
{
    if (nRate < 0 || rInfo.GetPetTypeID() == 0)
        return false;

    m_vectRandomPetTypeID.push_back(rInfo);
    m_vectRandomRate.push_back(nRate);

    return true;
}

bool RandomPetCollect::RandomOnePet(RandomPetInfo& rInfo) const
{
    const RandomPetInfo *pInfo = RandomOnePet();

    if (pInfo != NULL)
    {
        rInfo = *pInfo;

        return true;
    }

    return false;
}

const RandomPetInfo* RandomPetCollect::RandomOnePet() const
{
    if (!m_vectRandomRate.empty() )
    {
        int nIndex = RateRandom(m_vectRandomRate);

        if (0 <= nIndex && nIndex < (int)m_vectRandomPetTypeID.size() )
        {
            return &(m_vectRandomPetTypeID[nIndex]);
        }
    }

    return NULL;
}

