/*
 * PetService.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-11 16:22:20
 */

#ifndef __PETSERVICE_H__
#define __PETSERVICE_H__

#include "GameMsg_Pet.h"

#include "../Macros/Macros.h"
#include "../Pattern/Service.h"
#include "PetConfig/PetSkillConfig.h"

class Pet;
class PetComponent;
class PetSkill;

/**
* PetService:
* 
*/

class PetService : public Service
{
protected:
    PetService();

public:
    CLASS_INSTANCE_FUNC(PetService);
    virtual ~PetService();

public:
    virtual bool Init();

    void DoCreatePet(PetComponent *pPetComponent, unsigned short nPetTypeID,
        unsigned char nPotentialID, bool bAnnounce);
    void DoUnlockGrid(PetComponent *pPetComponent, GameMsg_Base &rMsg);
    void DoChangeName(PetComponent *pPetComponent, GameMsg_Base &rMsg);
    void DoChangeCarried(PetComponent *pPetComponent, GameMsg_Base &rMsg);
    void DoUnloadCarried(PetComponent *pPetComponent, GameMsg_Base &rMsg);
    void DoImprove(PetComponent *pPetComponent, GameMsg_Base &rMsg);
    void DoEvolution(PetComponent *pPetComponent, GameMsg_Base &rMsg);
    void DoFeeding(PetComponent *pPetComponent, GameMsg_Base &rMsg);
    void DoExchange(PetComponent *pPetComponent, GameMsg_Base &rMsg);

    void EncodePetMsgInfo(const Pet &rPet, PetMsgInfo &rInfo);
    void EncodePetExchangeMsgInfo(const Pet &rPet, PetExchangeMsgInfo &rInfo);
    void EncodePetBrief(const Pet &rPet, PlayerPet &briefPet);
    void EncodePetSkill(const Pet &rPet, PlayerPetSkill &playerSkill);
    void EncodePetSkillList(const Pet &rPet, std::list<PetMsgSkill> &listDanceSkill,
        std::list<PetMsgSkill> &listFightSkill);
    SkillType EncodePetSkill(const Pet &rPet, const PetSkill &skill, PetMsgSkill &skillMsg);

    void GetImproveQualityCondition(unsigned short nPetTypeID, unsigned char nQualityType,
        unsigned char &rCostType, unsigned int &rCostAmount, CItem &rCostItem);
    void GetEvolutionCondition(unsigned short nPetTypeID, unsigned char nNextEvolutionTimes,
        unsigned char &rCostType, unsigned int &rCostAmount, CItem &rCostItem, CItem &rExtraItem, int &rBaseRate);
    int GetPetRecoveryTime(const Pet &rPet) const;

protected:
    void SendUnlockGridRes(PetComponent *pPetComponent, ePetUnlockGridRes eErrorCode, unsigned short nGridCount = 0);
    void SendChangeNameRes(PetComponent *pPetComponent, ePetChangeNameRes eErrorCode, unsigned int nPetID, 
        const std::string &rNewName = "");
    void SendChangeCarriedRes(PetComponent *pPetComponent, ePetChangeCarriedRes eErrorCode, Pet *pPet = NULL);
    void SendExchangeRes(PetComponent *pPetComponent, ePetExchangeRes eErrorCode);

    void FeedExp(PetComponent *pPetComponent, Pet *pPet, unsigned int nItemType, int nItemCount, 
        GameMsg_S2C_PetFeedingRes &rResMsg);
    void FeedEndurance(PetComponent *pPetComponent, Pet *pPet, unsigned int nItemType, int nItemCount, 
        GameMsg_S2C_PetFeedingRes &rResMsg);

protected:
    void OnCreatePetCallback(QueryBase &rQuery);
};

#endif // __PETSERVICE_H__

