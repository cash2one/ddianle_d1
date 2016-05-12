/*
 * PetEggBaseInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-4 17:08:57
 */

#ifndef __PETEGGBASEINFO_H__
#define __PETEGGBASEINFO_H__

#include "../../mall/Currency.h"
#include "../../Macros/Macros.h"

#include <string>

class CFileStream;

/**
* PetEggBaseInfo:
* 
*/

class PetEggBaseInfo
{
public:
    PetEggBaseInfo();
    virtual ~PetEggBaseInfo();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(unsigned int, m_nTypeID, TypeID);
    CLASS_PROPERTY_READONLY(unsigned int, m_nIncubationTime, IncubationTime);
    CLASS_PROPERTY_READONLY(unsigned short, m_nMinIncubatorLevel, MinIncubatorLevel);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strModeName, ModeName);
    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_incubateCost, IncubateCost);
    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_speedUpPreMinuteCost, SpeedUpPreMinuteCost);
};

#endif // __PETEGGBASEINFO_H__

