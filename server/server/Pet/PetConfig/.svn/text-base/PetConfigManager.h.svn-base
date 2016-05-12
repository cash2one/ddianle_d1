/*
 * PetConfigManager.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-8 9:53:32
 */

#ifndef __PETCONFIGMANAGER_H__
#define __PETCONFIGMANAGER_H__

#include "../../mall/Currency.h"
#include "../../Macros/Macros.h"
#include "PetBaseInfo.h"
#include "PetExchangeInfo.h"
#include "PetSkillConfig.h"

#include <list>
#include <map>
#include <set>
#include <vector>

class CItem;
class CFileStream;
class PetBaseConfig;
class PetQualityInfo;
class PetEvolutionInfo;

/**
* PetConfigManager:
* 
*/

class PetConfigManager
{
protected:
    PetConfigManager();

public:
    CLASS_INSTANCE_FUNC(PetConfigManager)
    virtual ~PetConfigManager();

public:
    bool Initialize();

    void SetGridInfo(unsigned short nFreeCount, unsigned short nExtendCount, unsigned short nMaxUnlockCountOnce, const Currency &rCost);
    void AddFeedExpItem(unsigned int nItemType);
    void AddFeedEnduranceItem(unsigned int nItemType);

    void SetDanceDungeonExp( int nSuccessExp, int nFailExp );
    int GetDanceDungeonExp( bool bWin ) const;

    unsigned short GetMaxGridCount() const { return m_nFreeGridCount + m_nExtendGridCount; }

    unsigned int GetLevelUpExp(unsigned short nLevel, float fRatio) const;
    unsigned short GetTopLevel() const;

    bool GetPetQualityInfo(unsigned short nPetTypeID, PetQualityInfo &rInfo) const;
    const PetQualityInfo* GetPetQualityInfo(unsigned short nPetTypeID) const;

    bool GetPetEvolutionInfo(unsigned short nPetTypeID, PetEvolutionInfo &rInfo) const;
    const PetEvolutionInfo* GetPetEvolutionInfo(unsigned short nPetTypeID) const;

    void GetExchangeItem(unsigned char nAttributeType, unsigned char nQualityType, std::list<CItem> &rItem);
    void AddExchangeItemIntoMap(unsigned char nAttributeType, unsigned char nQualityType, std::map<itemtype_t, CItem> &rItem);

    bool GetPetBaseConfig(unsigned short nPetTypeID, PetBaseConfig &rInfo) const;
    const PetBaseConfig* GetPetBaseConfig(unsigned short nPetTypeID) const;

    bool GetPetPotential(unsigned char nPotentialID, PetPotentialConfig &rInfo) const;
    const PetPotentialConfig* GetPetPotential(unsigned char nPotentialID) const;

    const PetSkillUnlockConfig *GetPetSkillUnlockConfig(int nSkillIndex);

    void GetFeedExpItems(std::list<unsigned int> &rItems) const;
    void GetFeedEnduranceItems(std::list<unsigned int> &rItems) const;
    bool IsFeedExpItem(unsigned int nItemType) const;
    bool IsFeedEnduranceItem(unsigned int nItemType) const;

    void AddLockSkillCost(int nSkillCount, itemtype_t nItemType, int nItemCount);
    bool GetLockSkillCost(int nSkillCount, itemtype_t &nItemType, itemcount_t &nItemCount);
    void AddResetSkillCost(int nResetTimes, int nRatio);
    int GetResetSkillCost(int nResetTimes);
    bool GetPetSkill(SkillID skillID, PetSkillConfig &skillConfig) const;
    const PetSkillConfig *GetPetSkill(SkillID skillID) const;
    bool ExistPetSkill(SkillID skillID) const;
    bool RandomPetSkillList(std::set<SkillID> &skillIDSet, const std::set<SkillID> &lockSkillIDSet,
        int nTotalDanceSkillCount, int nTotalFightSkillCount) const;

    void GetPetTypeMap(std::map<unsigned short, unsigned char> &mapPetType) const;

    void SetRewardPetEgg(const std::string &strRewardPetEgg);
    const std::list<CItem> &GetRewardPetEgg() const;

protected:
    void LoadConfigFromFile();

    void LoadPetLevelExpFromFile(CFileStream &rFileStream);
    void LoadPetQualityFromFile(CFileStream &rFileStream);
    void LoadPetEvolutionFromFile(CFileStream &rFileStream);
    void LoadPetBaseConfigFromFile(CFileStream &rFileStream);
    void LoadPetSkillConfigFromFile(CFileStream &rFileStream);
    void LoadPetSkillUnlockConfigFromFile(CFileStream &rFileStream);
    void LoadPetPotentialConfigFromFile(CFileStream &rFileStream);

    bool CheckConfig() const;
    bool CheckQualityInfo(unsigned short nPetTypeID) const;
    bool CheckEvolutionInfo(unsigned short nPetTypeID) const;
    bool CheckExpandableItem(const std::list<CItem> &rItems) const;
    bool CheckExpandableItemAndGene(unsigned int nItemType) const;

    void RandomPetSkillList(std::set<SkillID> &skillIDSet,
        const std::map<SkillID, int> &mapRandomSkillID, int nTotalWeight, int nRandomSkillCount) const;

private:
    std::map<unsigned short, unsigned int>          m_mapPetLevelExp;       // level -> exp
    std::map<unsigned short, PetQualityInfo>        m_mapPetQuality;        // pet type id -> quality info
    std::map<unsigned short, PetEvolutionInfo>      m_mapPetEvolution;      // pet type id -> evolution info
    std::map<unsigned short, PetBaseConfig>         m_mapPetBaseConfig;     // pet type id -> pet base info
    std::map<SkillID, PetSkillConfig>               m_mapPetSkill;          // pet skill id -> pet skill config
    std::map<unsigned char, PetSkillUnlockConfig>   m_mapPetSkillUnlock;    // unlock skill count -> skill unlock config
    std::map<unsigned char, PetPotentialConfig>     m_mapPetPotential;      // pet potential id -> pet potential config

    PetExchangeInfo m_petExchangeInfo;
    std::set<unsigned int>  m_feedExpItems;
    std::set<unsigned int>  m_feedEnduranceItems;

    std::map<int, std::pair<itemtype_t, itemcount_t> > m_lockSkillCost;     // lock skill count -> item cost(item id, item count)
    std::map<int, int> m_mapResetTimesCost;                                 // reset times -> cost ratio

    CLASS_PROPERTY(unsigned short, m_nFreeGridCount, FreeGridCount);
    CLASS_PROPERTY(unsigned short, m_nExtendGridCount, ExtendGridCount);
    CLASS_PROPERTY(unsigned short, m_nMaxUnlockCountOnce, MaxUnlockCountOnce);
    CLASS_PROPERTY_BY_REF(Currency, m_extendGridCost, ExtendGridCost);
    CLASS_PROPERTY_BY_REF(Currency, m_changeNameCost, ChangeNameCost);
    CLASS_PROPERTY_BY_REF(std::string, m_strSkillRule, SkillRule);

    CLASS_PROPERTY(int, m_nDanceBaseExp, DanceBaseExp);
    int m_nDanceDungeonSuccessExp;  // 魔法神灯成功经验
    int m_nDanceDungeonFailExp;     // 魔法神灯失败经验
    CLASS_PROPERTY(unsigned short, m_nDanceCostEndurance, DanceCostEndurance);  // 跳舞消耗的耐力点
    CLASS_PROPERTY(int, m_nEnduranceRecoveryTime, EnduranceRecoveryTime);
    CLASS_PROPERTY(int, m_nFreeIncubationGuideStep, FreeIncubationGuideStep);   // 新手引导过程中，可以免费孵化的步骤
    CLASS_PROPERTY(int, m_nFreeAccelerateGuideStep, FreeAccelerateGuideStep);   // 新手引导过程中，可以免费加速的步骤
    CLASS_PROPERTY(int, m_nRewardPetEggGuideStep, RewardPetEggGuideStep);       // 新手引导过程中，赠送宠物蛋的步骤
    std::list<CItem> m_listGuideRewardPetEgg;                                   // 新手引导过程中，要赠送的宠物蛋
};

#endif // __PETCONFIGMANAGER_H__

