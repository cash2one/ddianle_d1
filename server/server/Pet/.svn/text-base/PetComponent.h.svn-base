/*
 * PetComponent.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-8 19:42:44
 */

#ifndef __PETCOMPONENT_H__
#define __PETCOMPONENT_H__

#include "../logic/EntityComponent.h"
#include "../logic/RoleComponentImpl.h"
#include "../Macros/Macros.h"
#include "PetConfig/PetSkillConfig.h"
#include <functional>
#include <set>
#include "Pet.h"

class Pet;
class PetEgg;
class PetMsgInfo;
class PetSkill;

class CEntityNetComponent;
class CEntityAttributeComponent;
class CEntityMallComponent;
class CEntityItemComponent;
class CRoomComponent;
class CAmuseRoomComponent;
class CEntityDungeonComponent;
class CEntityPhotoRoomComponent;
class CEntityCoupleComponent;
class CEntityDanceGroupComponent;
/**
* PetComponent:
* 
*/

class PetComponent : public CEntityComponent, public CommonSubscriber
{
    friend class PetService;
    friend class PetEggService;

public:
    PetComponent();
    virtual ~PetComponent();

public:
    virtual void Start();
    virtual void OnLogin();
    virtual void OnUpdate(const unsigned long &lTimeElapsed);
    virtual const char* SerializeName() { return "Pet"; }
    virtual void SerializeComponent(CParamPool &IOBuff);
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    virtual void RegComponentNetMsgMap();

    virtual void OnNotify(IParam &param);

public:
    const PetEgg* GetPetEgg() const { return m_pEgg; }

    bool HasPet() const;
    bool HasPet(unsigned int nPetID) const;
    bool HasCarriedPet() const;
    Pet *GetCarriedPet() const;
    unsigned int GetCarriedPetID() const;

    unsigned short GetPetGridCount() const;
    bool IsPetGridFull() const;

    bool CastPetDanceSkill(std::map<SkillID, int> &mapPetSkill);

public:
    unsigned int GetRoleID() const;

    void SendPlayerMsg(GameMsg_Base *pMsg);

    bool PayForIncubatorLevelUp(std::map<unsigned char, unsigned int> &rMapBill);
    bool PayForStartIncubation(std::map<unsigned char, unsigned int> &rMapBill);
    bool PayForIncubationSpeedUp(std::map<unsigned char, unsigned int> &rMapBill);
    bool PayForChangeName(std::map<unsigned char, unsigned int> &rMapBill);
    bool PayForUnlockGrid(std::map<unsigned char, unsigned int> &rMapBill);
    bool PayForImproveQuality(std::map<unsigned char, unsigned int> &rMapBill, const CItem &rItem);
    bool PayForEvolution(std::map<unsigned char, unsigned int> &rMapBill, const CItem &rItem, const CItem &rExtraItem);
    bool PayForFeeding(const CItem &rItem);

    const ItemBase* GetPetEggItem(unsigned short nIndex);
    void UseOnePetEggItem(unsigned short nIndex);

    void UnlockGrid(unsigned short nCount);
    void ChangePetName(unsigned int nPetID, const std::string &rNewName);
    void ChangeCarried(unsigned int nPetID, unsigned char nSceneID);

    void IncubatorLevelUp();
    void IncubationSpeedUp();

    void ImproveQuality(unsigned int nPetID);
    void Evolution(unsigned int nPetID);

    void RecoveryEndurance(Pet *pPet, int nRecoveredEndurance = 0);// 默认参数为自然时间恢复，否则恢复指定点数
    bool CostEndurance(Pet *pPet, int nEndurance);

    void AddExp(unsigned int nPetID, unsigned int nAddExp);

    void SendExchangeItems(const std::list<CItem> &rItemList);

    bool ChangeGuideStep(int nStep);

protected:
    void LoadPetFromDB(const std::map<unsigned int, PetDB, std::greater<unsigned int> > &mapPetDB);

    void SetCarriedPetID(unsigned int nPetID);

    void SetPetEgg(PetEgg *pEgg);
    void OpenedPetEgg();
    void AddPet(Pet *pPet);

    void OnPetLevelUp(Pet *pPet);
    void SyncSkillChanged(Pet *pPet);

    Pet* GetPet(unsigned int nPetID) const;
    void DestroyPet(unsigned int nPetID);

    void SaveEggState();
    void SavePetState(unsigned int nPetID, unsigned char nState);

    bool Pay(std::map<unsigned char, unsigned int> &rMapBill, EChangeBillCause eChangeBillCause, 
        EChangeBindBillCause eChangeBindBillCause, EChangeMoneyCause eChangeMoneyCause);

    bool CanUseItems(const CItem &rItem);

    void LoadPetIncubatorFromDB();

    void SendOpenEggRes(unsigned char eErrorCode, unsigned short nPetTypeID = 0, const std::string &rDefaultName = "");

    void SendIncubationGetPetAnnounce(const std::string &rPetName);

    int TrainSkill(Pet *pet, SkillID nSkillID, bool bIsFisrtTime = false);
    int RandomFightSkill(Pet *pet);
    int ResetSkill(Pet *pet, const std::set<SkillID> &setLockSkillID, bool bIsFisrtTime = false);
    int PayResetSkillCost(Pet *pPet, int nLockSkillCount, int nResetTimes);

    void SyncPetChangeInfoToOthers( const Pet *pPet );
    void SyncPetChangeInfoToSelf( const Pet *pPet );

    void RecoveryEndurance();
    void AddEndurance(Pet *pPet, int nAddedEndurance, unsigned int nTime);
    void SetEndurance(Pet *pPet, unsigned short nEndurance, unsigned int nTime);

    //void GetPetTypeMap(std::map<unsigned short, unsigned char> mapPetType);
    const Pet *GetTopPet(unsigned short) const;

    void SendMsgToAll(GameMsg_Base &rMsg, bool bNotSelf = true);

protected:
    void OnEnterIncubationRoom(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnIncubatorLevelUp(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnStartIncubation(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnIncubationRequest(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnIncubationSpeedUp(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnOpenEgg(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);

    void OnRequestList(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnUnlockGrid(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnChangeName(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnChangeCarried(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnUnloadCarried(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);

    void OnRequestImproveCondition(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnImprove(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnRequestEvolutionCondition(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnEvolution(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnFeeding(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);

    void OnTrainSkill(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnResetSkill(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);

    void OnGetExchangeList(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnExchange(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);

    void OnGetPetLibraryTypeList(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnGetPetLibraryList(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);
    void OnReportPetDanceEmotion(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);

    void OnPetGuideStep(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);

    void OnLoadIncubatorCallback(QueryBase &rQuery);
    void OnLoadExtendCountCallback(QueryBase &rQuery);
    void OnLoadPetEggCallback(QueryBase &rQuery);

private:
    CEntityNetComponent         *m_pNetComponent;
    CEntityAttributeComponent   *m_pAttrComponent;
    CEntityMallComponent        *m_pMallComponent;
    CEntityItemComponent        *m_pItemComponent;
    CRoomComponent              *m_pRoomComponent;
    CAmuseRoomComponent         *m_pAmuseRoomComponent;
    CEntityDungeonComponent     *m_pDungeonComponent;
    CEntityPhotoRoomComponent   *m_pPhotoRoomComponent;
    CEntityCoupleComponent      *m_pCoupleComponent;
    CEntityDanceGroupComponent  *m_pRoleDanceGroup;

private:
    bool            m_bOpen;

    PetEgg          *m_pEgg;

    // 玩家所拥有的宠物，包括装备中的和休息中宠物
    std::map<unsigned int, Pet *, std::greater<unsigned int> >  m_mapPet;

    // 玩家已兑换掉的宠物，这些宠物全部是已销毁状态，宠物图鉴功能需要已销毁宠物信息
    std::map<unsigned int, Pet *, std::greater<unsigned int> >  m_mapDestroyedPet;

    unsigned int    m_nCarriedPetID;

    unsigned short  m_nExtendPetGridCount;

    int             m_nPetEggUpdateTime;
    int             m_nPetEnduranceUpdateTime;

    CLASS_BOOL_PROPERTY_READONLY(m_bEggLoadCompleted, EggLoadCompleted);

    CLASS_PROPERTY_READONLY(unsigned short, m_nIncubatorLevel, IncubatorLevel);

    CLASS_PROPERTY(int, m_nGuideStep, GuideStep); // 宠物系统引导的步骤

    CLASS_PROPERTY(unsigned char, m_nSceneID, SceneID); // 宠物所在的场景ID，只存放在内存中，用于客户端信息同步
};

typedef CRoleComponentImpl<PetComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRolePet;

#endif // __PETCOMPONENT_H__

