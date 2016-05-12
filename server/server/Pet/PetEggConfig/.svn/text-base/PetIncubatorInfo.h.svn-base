/*
 * PetIncubatorInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-9 9:43:02
 */

#ifndef __PETINCUBATORINFO_H__
#define __PETINCUBATORINFO_H__

#include "../../mall/Currency.h"
#include "../../Macros/Macros.h"

#include <string>

class CFileStream;

/**
* PetIncubatorInfo:
* 
*/

class PetIncubatorInfo
{
public:
    PetIncubatorInfo();
    virtual ~PetIncubatorInfo();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(unsigned short, m_nLevel, Level);
    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_levelUpCost, LevelUpCost);
};

#endif // __PETINCUBATORINFO_H__

