/*
 * PetEggConfigManager.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-4 19:25:04
 */

#ifndef __PETEGGCONFIGMANAGER_H__
#define __PETEGGCONFIGMANAGER_H__

#include "../../Macros/Macros.h"

#include <map>

class PetIncubatorInfo;
class PetEggBaseInfo;
class RandomPetInfo;
class RandomPetCollect;

/**
* PetEggConfigManager:
* 
*/

class PetEggConfigManager
{
protected:
    PetEggConfigManager();

public:
    CLASS_INSTANCE_FUNC(PetEggConfigManager)
    virtual ~PetEggConfigManager();

public:
    bool Initialize();

    void AddRandomPetInfo(unsigned int nEggTypeID, RandomPetInfo &rInfo, int nRate);

    bool FindEggBaseInfo(unsigned int nEggTypeID) const;
    bool FindPetList(unsigned int nEggTypeID) const;

    bool GetPetIncubatorInfo(unsigned short nLevel, PetIncubatorInfo &rInfo) const;
    const PetIncubatorInfo* GetPetIncubatorInfo(unsigned short nLevel) const;

    bool GetEggInfo(unsigned int nEggTypeID, PetEggBaseInfo &rInfo) const;
    const PetEggBaseInfo* GetEggInfo(unsigned int nEggTypeID) const;

    bool RandOnePet(unsigned int nEggTypeID, RandomPetInfo &rInfo) const;
    const RandomPetInfo* RandOnePet(unsigned int nEggTypeID) const;

protected:
    void LoadEggInfoFromFile();

    bool CheckConfig() const;

private:
    std::map<unsigned short, PetIncubatorInfo>  m_mapIncubatorLevelUpCost;
    std::map<unsigned int, PetEggBaseInfo>      m_mapEggInfos;
    std::map<unsigned int, RandomPetCollect>    m_mapRandomPetInfos;
};

#endif // __PETEGGCONFIGMANAGER_H__

