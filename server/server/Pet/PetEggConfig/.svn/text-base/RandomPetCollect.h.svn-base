/*
 * RandomPetCollect.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-5 11:08:32
 */

#ifndef __RANDOMPETCOLLECT_H__
#define __RANDOMPETCOLLECT_H__

#include "../../Macros/Macros.h"

#include <vector>

/**
* RandomPetInfo:
* 
*/

class RandomPetInfo
{
public:
    RandomPetInfo(unsigned short nPetTypeID, unsigned char nPotentialID, bool bAnnounce);
    virtual ~RandomPetInfo();

    CLASS_PROPERTY_READONLY(unsigned short, m_nPetTypeID, PetTypeID);
    CLASS_PROPERTY_READONLY(unsigned char, m_nPotentialID, PotentialID);
    CLASS_BOOL_PROPERTY_READONLY(m_bAnnounce, Announce);
};

//////////////////////////////////////////////////////////////////////////

/**
* RandomPetCollect:
* 
*/

class RandomPetCollect
{
public:
    RandomPetCollect();
    virtual ~RandomPetCollect();

public:
    bool AddRandomPetInfo(RandomPetInfo& rInfo, int nRate);

    bool RandomOnePet(RandomPetInfo& rInfo) const;
    const RandomPetInfo* RandomOnePet() const;

private:
    std::vector<RandomPetInfo>  m_vectRandomPetTypeID;
    std::vector<int>            m_vectRandomRate;
};

#endif // __RANDOMPETCOLLECT_H__

