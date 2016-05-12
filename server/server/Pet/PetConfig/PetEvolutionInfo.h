/*
 * PetEvolutionInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-8 15:31:16
 */

#ifndef __PETEVOLUTIONINFO_H__
#define __PETEVOLUTIONINFO_H__

#include "../../mall/Currency.h"
#include "../../Macros/Macros.h"

#include "../../../datastructure/DataStruct_Base.h"

#include <list>
#include <vector>

#define MAX_RATE    10000

class CFileStream;

/**
* PetEvolutionCondition:
* 
*/

class PetEvolutionCondition
{
public:
    PetEvolutionCondition();
    virtual ~PetEvolutionCondition();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(int, m_nRate, Rate);
    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_money, Money);
    CLASS_PROPERTY_READONLY_BY_REF(std::list<CItem>, m_items, Item);
    CLASS_BOOL_PROPERTY_READONLY(m_bExtraItem, ExtraItem);
    CLASS_PROPERTY_READONLY_BY_REF(CItem, m_itemExtra, ExtraItem);
};

/**
* PetEvolutionInfo:
* 
*/

class PetEvolutionInfo
{
public:
    PetEvolutionInfo();
    virtual ~PetEvolutionInfo();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    unsigned short GetMaxEndurance(unsigned char nCurrentEvolutionTimes) const;
    unsigned short GetNextEvolutionLevel(unsigned char nNextEvolutionTimes, unsigned short nDefaultLevel) const;

    bool CanEvolution(unsigned char nNextEvolutionTimes, unsigned short nLevel) const;

    bool GetEvolutionCondition(unsigned char nNextEvolutionTimes, PetEvolutionCondition &rInfo) const;
    const PetEvolutionCondition* GetEvolutionCondition(unsigned char nNextEvolutionTimes) const;

    CLASS_PROPERTY_READONLY(unsigned short, m_nPetTypeID, PetTypeID);
    CLASS_PROPERTY_READONLY(unsigned char, m_nMaxEvolutionTimes, MaxEvolutionTimes);

private:
    std::vector<unsigned short> m_vectEndurance;
    std::vector<unsigned short> m_vectEvolutionLevel;
    std::vector<PetEvolutionCondition> m_vectCondition;
};

#endif // __PETEVOLUTIONINFO_H__

