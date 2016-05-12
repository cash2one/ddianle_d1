/*
 * PetExchangeInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-18 19:56:50
 */

#ifndef __PETEXCHANGEINFO_H__
#define __PETEXCHANGEINFO_H__

#include "../../../datastructure/DataStruct_Base.h"

#include <list>
#include <vector>

class CFileStream;

/**
* PetExchangeInfo:
* 
*/

class PetExchangeInfo
{
public:
    PetExchangeInfo();
    virtual ~PetExchangeInfo();

public:
    void LoadInfoFromFile(CFileStream &rFileStream);

    bool IsValid() const;

    void GetExchangeItem(unsigned char nAttributeType, unsigned char nQualityType, std::list<CItem> &rItem) const;
    void AddExchangeItemIntoMap(unsigned char nAttributeType, unsigned char nQualityType, std::map<itemtype_t, CItem> &rItem) const;

protected:
    void AddExchangeInfo(unsigned char nAttributeType, unsigned char nQualityType, const CItem &rItem);

protected:
    std::vector<std::vector<std::list<CItem> > >    m_allExchangeInfos;
};

#endif // __PETEXCHANGEINFO_H__

