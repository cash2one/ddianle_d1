/*
 * PetEggService.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-9 15:24:23
 */

#ifndef __PETEGGSERVICE_H__
#define __PETEGGSERVICE_H__

#include "GameMsg_Pet.h"

#include "../Macros/Macros.h"
#include "../Pattern/Service.h"

class PetComponent;
class RandomPetInfo;

/**
* PetEggService:
* 
*/

class PetEggService : public Service
{
protected:
    PetEggService();

public:
    CLASS_INSTANCE_FUNC(PetEggService)
    virtual ~PetEggService();

public:
    virtual bool Init();
    virtual void Start();

    void DoIncubatorLevelUp(PetComponent *pPetComponent);
    void DoStartIncubation(PetComponent *pPetComponent, GameMsg_Base &rMsg);
    void DoIncubationSpeedUp(PetComponent *pPetComponent);
    const RandomPetInfo* DoOpenEgg(PetComponent *pPetComponent);

protected:
    void SendIncubatorLevelUpRes(PetComponent *pPetComponent, ePetIncubatorLevelUpRes eErrorCode);
    void SendStartIncubationRes(PetComponent *pPetComponent, ePetStartIncubationRes eErrorCode);
    void SendIncubationSpeedUpRes(PetComponent *pPetComponent, ePetIncubationSpeedUpRes eErrorCode);

protected:
    void OnCreatePetEggCallback(QueryBase &rQuery);
};

#endif // __PETEGGSERVICE_H__

