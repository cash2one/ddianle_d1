/*
 * PetQualityInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-8 13:34:34
 */

#ifndef __PETQUALITYINFO_H__
#define __PETQUALITYINFO_H__

#include "../../mall/Currency.h"
#include "../../Macros/Macros.h"

#include "../../../datastructure/DataStruct_Base.h"

#include <list>
#include <vector>

class CFileStream;

enum ePetQualityType
{
    ePetQualityType_None = 0,

    ePetQualityType_White,
    ePetQualityType_Pink,
    ePetQualityType_Silvery,
    ePetQualityType_Golden,

    ePetQualityType_Max = 255,
};

/**
* PetImproveCondition:
* 
*/

class PetImproveCondition
{
public:
    PetImproveCondition();
    virtual ~PetImproveCondition();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_money, Money);
    CLASS_PROPERTY_READONLY_BY_REF(std::list<CItem>, m_items, Item);
};

/**
* PetQualityInfo:
* 
*/

class PetQualityInfo
{
public:
    PetQualityInfo();
    virtual ~PetQualityInfo();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    unsigned short GetMaxEndurance(unsigned char nCurrentQualityType) const;
    unsigned short GetNextImproveLevel(unsigned char nCurrentQualityType, unsigned short nDefaultLevel) const;

    bool CanImproveQuality(unsigned char nCurrentQualityType, unsigned short nLevel) const;

    bool GetImproveQualityCondition(unsigned char nCurrentQualityType, PetImproveCondition &rInfo) const;
    const PetImproveCondition* GetImproveQualityCondition(unsigned char nCurrentQualityType) const;

    CLASS_PROPERTY_READONLY(unsigned short, m_nPetTypeID, PetTypeID);
    CLASS_PROPERTY_READONLY(unsigned char, m_nMaxQualityType, MaxQualityType);

private:
    std::vector<unsigned short> m_vectEndurance;
    std::vector<unsigned short> m_vectImproveLevel;
    std::vector<PetImproveCondition> m_vectCondition;
};

#endif // __PETQUALITYINFO_H__

