#include "PetConfigManager.h"
#include "PetBaseInfo.h"
#include "PetQualityInfo.h"
#include "PetEvolutionInfo.h"

#include "../../../socket/Log.h"

#include "../../../datastructure/datainfor/FileStream.h"
#include "../../../datastructure/datainfor/ConfigManager.h"
#include "../../util/CommonFunc.h"

#include <algorithm>

extern std::string g_sPath;

#define PET_FILE_NAME       "data/bytes/Pet.bytes"

#ifdef WIN32
#pragma warning(disable:4127) // 去除警告ASSERT(false);条件表达式是常量
#endif

PetConfigManager::PetConfigManager(void)
    : m_nFreeGridCount(0)
    , m_nExtendGridCount(0)
    , m_nMaxUnlockCountOnce(0)
    , m_nDanceBaseExp(0)
    , m_nDanceDungeonSuccessExp(0)
    , m_nDanceDungeonFailExp(0)
    , m_nDanceCostEndurance(0)
    , m_nEnduranceRecoveryTime(0)
    , m_nFreeIncubationGuideStep(0)
    , m_nFreeAccelerateGuideStep(0)
    , m_nRewardPetEggGuideStep(0)
{
}

PetConfigManager::~PetConfigManager(void)
{
}

bool PetConfigManager::Initialize()
{
    LoadConfigFromFile();

    return CheckConfig();
}

void PetConfigManager::SetGridInfo(unsigned short nFreeCount, unsigned short nExtendCount, unsigned short nMaxUnlockCountOnce, const Currency &rCost)
{
    if (m_nFreeGridCount < nFreeCount)
        m_nFreeGridCount = nFreeCount;

    if (m_nExtendGridCount < nExtendCount)
        m_nExtendGridCount = nExtendCount;

    if (m_nMaxUnlockCountOnce < nMaxUnlockCountOnce)
        m_nMaxUnlockCountOnce = nMaxUnlockCountOnce;

    m_extendGridCost = rCost;
}

void PetConfigManager::AddFeedExpItem(unsigned int nItemType)
{
    if (!IsFeedExpItem(nItemType))
        m_feedExpItems.insert(nItemType);
}

void PetConfigManager::AddFeedEnduranceItem(unsigned int nItemType)
{
    if (!IsFeedEnduranceItem(nItemType))
        m_feedEnduranceItems.insert(nItemType);
}

void PetConfigManager::SetDanceDungeonExp( int nSuccessExp, int nFailExp )
{
    if ( m_nDanceDungeonSuccessExp < nSuccessExp )
        m_nDanceDungeonSuccessExp = nSuccessExp;

    if ( m_nDanceDungeonFailExp < nFailExp )
        m_nDanceDungeonFailExp = nFailExp;
}

int PetConfigManager::GetDanceDungeonExp( bool bWin ) const
{
    if ( bWin )
        return m_nDanceDungeonSuccessExp;

    return m_nDanceDungeonFailExp;
}

unsigned int PetConfigManager::GetLevelUpExp(unsigned short nLevel, float fRatio) const
{
    std::map<unsigned short, unsigned int>::const_iterator itr = m_mapPetLevelExp.find(nLevel);

    if (itr != m_mapPetLevelExp.end())
        return (unsigned int)(itr->second * fRatio);

    return 0;
}

unsigned short PetConfigManager::GetTopLevel() const
{
    if (m_mapPetLevelExp.empty())
        return 0;
    return m_mapPetLevelExp.rbegin()->first;
}

bool PetConfigManager::GetPetQualityInfo(unsigned short nPetTypeID, PetQualityInfo &rInfo) const
{
    const PetQualityInfo *pInfo = GetPetQualityInfo(nPetTypeID);
    if (pInfo != NULL)
    {
        rInfo = *pInfo;
        return true;
    }

    return false;
}

const PetQualityInfo* PetConfigManager::GetPetQualityInfo(unsigned short nPetTypeID) const
{
    std::map<unsigned short, PetQualityInfo>::const_iterator itr = m_mapPetQuality.find(nPetTypeID);

    if (itr != m_mapPetQuality.end())
        return &(itr->second);

    return NULL;
}

bool PetConfigManager::GetPetEvolutionInfo(unsigned short nPetTypeID, PetEvolutionInfo &rInfo) const
{
    const PetEvolutionInfo* pInfo = GetPetEvolutionInfo(nPetTypeID);

    if (pInfo != NULL)
    {
        rInfo = *pInfo;

        return true;
    }

    return false;
}

const PetEvolutionInfo* PetConfigManager::GetPetEvolutionInfo(unsigned short nPetTypeID) const
{
    std::map<unsigned short, PetEvolutionInfo>::const_iterator itr = m_mapPetEvolution.find(nPetTypeID);

    if (itr != m_mapPetEvolution.end() )
        return &(itr->second);

    return NULL;
}

void PetConfigManager::GetExchangeItem(unsigned char nAttributeType, unsigned char nQualityType, std::list<CItem> &rItem)
{
    m_petExchangeInfo.GetExchangeItem(nAttributeType, nQualityType, rItem);
}

void PetConfigManager::AddExchangeItemIntoMap(unsigned char nAttributeType, unsigned char nQualityType, std::map<itemtype_t, CItem> &rItem)
{
    m_petExchangeInfo.AddExchangeItemIntoMap(nAttributeType, nQualityType, rItem);
}

bool PetConfigManager::GetPetBaseConfig(unsigned short nPetTypeID, PetBaseConfig &rInfo) const
{
    const PetBaseConfig *pInfo = GetPetBaseConfig(nPetTypeID);
    if (pInfo != NULL)
    {
        rInfo = *pInfo;
        return true;
    }

    return false;
}

const PetBaseConfig* PetConfigManager::GetPetBaseConfig(unsigned short nPetTypeID) const
{
    std::map<unsigned short, PetBaseConfig>::const_iterator itr = m_mapPetBaseConfig.find(nPetTypeID);

    if (itr != m_mapPetBaseConfig.end())
        return &(itr->second);

    return NULL;
}

bool PetConfigManager::GetPetPotential(unsigned char nPotentialID, PetPotentialConfig &rInfo) const
{
    const PetPotentialConfig *pInfo = GetPetPotential(nPotentialID);
    if (pInfo != NULL)
    {
        rInfo = *pInfo;
        return true;
    }

    return false;
}

const PetPotentialConfig* PetConfigManager::GetPetPotential(unsigned char nPotentialID) const
{
    std::map<unsigned char, PetPotentialConfig>::const_iterator itr = m_mapPetPotential.find(nPotentialID);

    if (itr != m_mapPetPotential.end())
        return &(itr->second);

    return NULL;
}

void PetConfigManager::GetFeedExpItems(std::list<unsigned int> &rItems) const
{
    for (std::set<unsigned int>::const_iterator itr = m_feedExpItems.begin();
         itr != m_feedExpItems.end(); ++itr)
    {
        rItems.push_back(*itr);
    }
}

void PetConfigManager::GetFeedEnduranceItems(std::list<unsigned int> &rItems) const
{
    for (std::set<unsigned int>::const_iterator itr = m_feedEnduranceItems.begin();
         itr != m_feedEnduranceItems.end(); ++itr)
    {
        rItems.push_back(*itr);
    }
}

bool PetConfigManager::IsFeedExpItem(unsigned int nItemType) const
{
    return (m_feedExpItems.find(nItemType) != m_feedExpItems.end());
}

bool PetConfigManager::IsFeedEnduranceItem(unsigned int nItemType) const
{
    return (m_feedEnduranceItems.find(nItemType) != m_feedEnduranceItems.end());
}

void PetConfigManager::AddLockSkillCost(int nSkillCount, itemtype_t nItemType, int nItemCount)
{
    m_lockSkillCost.insert(std::make_pair(nSkillCount, std::make_pair(nItemType, (itemcount_t)nItemCount)));
}

bool PetConfigManager::GetLockSkillCost(int nSkillCount, itemtype_t &nItemType, itemcount_t &nItemCount)
{
    if (m_lockSkillCost.find(nSkillCount) != m_lockSkillCost.end())
    {
        nItemType = m_lockSkillCost[nSkillCount].first;
        nItemCount = m_lockSkillCost[nSkillCount].second;
        return true;
    }
    return false;
}

void PetConfigManager::AddResetSkillCost(int nResetTimes, int nRatio)
{
    m_mapResetTimesCost[nResetTimes] = nRatio;
}

int PetConfigManager::GetResetSkillCost(int nResetTimes)
{
    if (nResetTimes == 1 || m_mapResetTimesCost.empty())
        return 1;

    if (m_mapResetTimesCost.find(nResetTimes) != m_mapResetTimesCost.end())
    {
        return m_mapResetTimesCost[nResetTimes];
    }

    int nCostRatio = 1;
    for(std::map<int, int>::const_reverse_iterator it = m_mapResetTimesCost.rbegin();
        it != m_mapResetTimesCost.rend(); ++it)
    {
        if (nResetTimes >= it->first)
        {
            nCostRatio = it->second;
            break;
        }
    }
    return nCostRatio;
}

bool PetConfigManager::GetPetSkill(SkillID skillID, PetSkillConfig &skillConfig) const
{
    const PetSkillConfig *pConfig = GetPetSkill(skillID);
    if (pConfig != NULL)
    {
        skillConfig = *pConfig;
        return true;
    }

    return false;
}

const PetSkillConfig *PetConfigManager::GetPetSkill(SkillID skillID) const
{
    std::map<SkillID, PetSkillConfig>::const_iterator itr = m_mapPetSkill.find(skillID);
    if (itr != m_mapPetSkill.end())
    {
        const PetSkillConfig &config = itr->second;
        return &config;
    }

    return NULL;
}

bool PetConfigManager::ExistPetSkill(SkillID skillID) const
{
    std::map<SkillID, PetSkillConfig>::const_iterator itr = m_mapPetSkill.find(skillID);
    if (itr != m_mapPetSkill.end())
    {
        return true;
    }

    return false;
}

bool PetConfigManager::RandomPetSkillList(std::set<SkillID> &skillIDSet,
    const std::set<SkillID> &lockSkillIDSet, int nTotalDanceSkillCount, int nTotalFightSkillCount) const
{
    // 准备随机权重表
    std::map<SkillID, int> danceSkillRandomMap;
    std::map<SkillID, int> fightSkillRandomMap;
    int nTotalDanceSkillWeight = 0;
    int nTotalFightSkillWeight = 0;
    for (std::map<SkillID, PetSkillConfig>::const_iterator it = m_mapPetSkill.begin();
        it != m_mapPetSkill.end(); ++it)
    {
        SkillID nSkillID = it->first;
        if (lockSkillIDSet.find(nSkillID) == lockSkillIDSet.end())
        {
            const PetSkillConfig &skillConfig = it->second;
            if (PetSkillConfig::IsDanceSkill(skillConfig.GetType()))
            {
                danceSkillRandomMap[nSkillID] = skillConfig.GetRandomWeight();
                nTotalDanceSkillWeight += skillConfig.GetRandomWeight();
            }
            else if (PetSkillConfig::IsFightSkill(skillConfig.GetType()))
            {
                fightSkillRandomMap[nSkillID] = skillConfig.GetRandomWeight();
                nTotalFightSkillWeight += skillConfig.GetRandomWeight();
            }
        }
    }

    // 重新计算要随机的技能数量
    int nRandomDanceSkillCount = nTotalDanceSkillCount;
    int nRandomFightSkillCount = nTotalFightSkillCount;
    for (std::set<SkillID>::const_iterator itSet = lockSkillIDSet.begin();
        itSet != lockSkillIDSet.end(); ++itSet)
    {
        const PetSkillConfig *skillConfig = GetPetSkill(*itSet);
        if (skillConfig != NULL)
        {
            if (PetSkillConfig::IsDanceSkill(skillConfig->GetType()))
            {
                nRandomDanceSkillCount--;
            }
            else if (PetSkillConfig::IsFightSkill(skillConfig->GetType()))
            {
                nRandomFightSkillCount--;
            }
        }
    }

    if ((int)danceSkillRandomMap.size() < nRandomDanceSkillCount
        || (int)fightSkillRandomMap.size() < nRandomFightSkillCount)
    {
        WriteLog(LOGLEVEL_ERROR, "Random skill list: not enough skill count");
        ASSERT(false);
    }

    skillIDSet.clear();
    RandomPetSkillList(skillIDSet, danceSkillRandomMap, nTotalDanceSkillWeight, nRandomDanceSkillCount);
    RandomPetSkillList(skillIDSet, fightSkillRandomMap, nTotalFightSkillWeight, nRandomFightSkillCount);

    return true;
}

void PetConfigManager::RandomPetSkillList(std::set<SkillID> &skillIDSet,
    const std::map<SkillID, int> &mapRandomSkillIDWeight, int nTotalWeight, int nRandomSkillCount) const
{
    if (mapRandomSkillIDWeight.size() < (size_t)nRandomSkillCount)
    {
        ASSERT(false);
        return;
    }

    // 根据权重随机
    std::map<SkillID, int> weightMap = mapRandomSkillIDWeight;
    while (nRandomSkillCount > 0)
    {
        int nRandomNumber = Random() % nTotalWeight;
        std::map<SkillID, int>::iterator itWeight = weightMap.begin();
        while (itWeight != weightMap.end())
        {
            nRandomNumber -= itWeight->second;
            if (nRandomNumber <= 0)
            {
                skillIDSet.insert(itWeight->first);
                nTotalWeight -= itWeight->second;
                weightMap.erase(itWeight);
                --nRandomSkillCount;
                break;
            }
            else
            {
                ++itWeight;
            }
        }
    }
}

const PetSkillUnlockConfig *PetConfigManager::GetPetSkillUnlockConfig(int nSkillIndex)
{
    unsigned char nSkillCount = (unsigned char)(nSkillIndex + 1);
    std::map<unsigned char, PetSkillUnlockConfig>::const_iterator itr = m_mapPetSkillUnlock.find(nSkillCount);
    if (itr != m_mapPetSkillUnlock.end())
    {
        return &(itr->second);
    }

    return NULL;
}

void PetConfigManager::GetPetTypeMap(std::map<unsigned short, unsigned char> &mapPetType) const
{
    for (std::map<unsigned short, PetBaseConfig>::const_iterator it = m_mapPetBaseConfig.begin();
        it != m_mapPetBaseConfig.end(); ++it)
    {
        unsigned short nPetType = it->first;
        if (nPetType == it->second.GetPetTypeID())
            mapPetType.insert(std::make_pair(nPetType, it->second.GetAttributeType()));
    }
}

void PetConfigManager::LoadConfigFromFile()
{
    CFileStream file;
    std::string strFileName;

    strFileName = g_sPath + PET_FILE_NAME;
    file.open(strFileName.c_str(), "rb");

    LoadPetLevelExpFromFile(file);
    LoadPetQualityFromFile(file);
    LoadPetEvolutionFromFile(file);
    m_petExchangeInfo.LoadInfoFromFile(file);
    LoadPetBaseConfigFromFile(file);
    LoadPetSkillConfigFromFile(file);
    LoadPetSkillUnlockConfigFromFile(file);
    LoadPetPotentialConfigFromFile(file);

    file.close();
}

void PetConfigManager::LoadPetLevelExpFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return;

    m_mapPetLevelExp.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    unsigned short nLevel = 0;
    unsigned int nExp = 0;

    for (unsigned short i = 0; i < nCount; ++i)
    {
        rFileStream.ReadUShort(nLevel);
        rFileStream.ReadUInt(nExp);

        if (nLevel > 0 && nExp > 0)
        {
            m_mapPetLevelExp.insert(std::make_pair(nLevel, nExp));
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "Invalid pet base exp: level(%u), exp(%u).", (unsigned int)nLevel, nExp);
        }
    }
}

void PetConfigManager::LoadPetQualityFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return;

    m_mapPetQuality.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        PetQualityInfo petQualityInfo;

        if (petQualityInfo.LoadInfoFromFile(rFileStream))
        {
            m_mapPetQuality.insert(std::make_pair(petQualityInfo.GetPetTypeID(), petQualityInfo));
        }
    }
}

void PetConfigManager::LoadPetEvolutionFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return;

    m_mapPetEvolution.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        PetEvolutionInfo petEvolutionInfo;

        if (petEvolutionInfo.LoadInfoFromFile(rFileStream))
        {
            m_mapPetEvolution.insert(std::make_pair(petEvolutionInfo.GetPetTypeID(), petEvolutionInfo));
        }
    }
}

void PetConfigManager::LoadPetBaseConfigFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return;

    m_mapPetBaseConfig.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        PetBaseConfig petBaseInfo;

        if (petBaseInfo.LoadInfoFromFile(rFileStream))
        {
            m_mapPetBaseConfig.insert(std::make_pair(petBaseInfo.GetPetTypeID(), petBaseInfo) );
        }
    }
}

void PetConfigManager::LoadPetSkillConfigFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return;

    m_mapPetSkill.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        PetSkillConfig skillConfig;
        if (skillConfig.LoadInfoFromFile(rFileStream))
        {
            m_mapPetSkill.insert(std::make_pair(skillConfig.GetSkillID(), skillConfig));
        }
    }
}

void PetConfigManager::LoadPetSkillUnlockConfigFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return;

    m_mapPetSkillUnlock.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);
    for (unsigned short j = 0; j < nCount; ++j)
    {
        PetSkillUnlockConfig skillUnlockConfig;

        if (skillUnlockConfig.LoadInfoFromFile(rFileStream))
        {
            m_mapPetSkillUnlock.insert(std::make_pair(skillUnlockConfig.GetSkillCount(), skillUnlockConfig));
        }
    }
}

void PetConfigManager::LoadPetPotentialConfigFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return;

    m_mapPetPotential.clear();
    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short j = 0; j < nCount; ++j)
    {
        PetPotentialConfig config;

        if (config.LoadInfoFromFile(rFileStream))
        {
            m_mapPetPotential.insert(std::make_pair(config.GetPotentialID(), config));
        }
    }
}

bool PetConfigManager::CheckConfig() const
{
    if (m_mapPetLevelExp.size() == 0)
        return false;
    else if (m_mapPetLevelExp.rbegin()->first != (unsigned short)m_mapPetLevelExp.size())
        return false;
    else if (m_mapPetQuality.size() == 0)
        return false;
    else if (m_mapPetEvolution.size() == 0)
        return false;
    else if (m_mapPetBaseConfig.size() == 0)
        return false;
    else if (m_nFreeGridCount == 0)
        return false;
    else if (!m_extendGridCost.IsValid())
        return false;
    else if (!m_changeNameCost.IsValid())
        return false;
    else if (!m_petExchangeInfo.IsValid())
        return false;
    else if (m_feedExpItems.empty())
        return false;
    else if (m_feedEnduranceItems.empty())
        return false;

    for (std::map<unsigned short, PetBaseConfig>::const_iterator petBaseItr = m_mapPetBaseConfig.begin();
         petBaseItr != m_mapPetBaseConfig.end(); ++petBaseItr)
    {
        if (!CheckQualityInfo(petBaseItr->first))
        {
            return false;
        }
        else if (!CheckEvolutionInfo(petBaseItr->first))
        {
            return false;
        }
    }

    for (unsigned char nAttributeType = ePetAttributeType_None + 1;
        nAttributeType < ePetAttributeType_Max; ++nAttributeType)
    {
        for (unsigned char nQualityType = ePetQualityType_None + 1; 
            nQualityType < ePetQualityType_Max; ++nQualityType)
        {
            std::list<CItem> items;
            m_petExchangeInfo.GetExchangeItem(nAttributeType, nQualityType, items);

            if (!CheckExpandableItem(items))
            {
                WriteLog(LOGLEVEL_ERROR, "Pet exchange item error.");

                return false;
            }
        }
    }

    std::set<unsigned int>::const_iterator itemTypeItr;
    for (itemTypeItr = m_feedExpItems.begin(); itemTypeItr != m_feedExpItems.end(); ++itemTypeItr)
    {
        if (!CheckExpandableItemAndGene(*itemTypeItr))
        {
            return false;
        }
    }

    for (itemTypeItr = m_feedEnduranceItems.begin(); itemTypeItr != m_feedEnduranceItems.end(); ++itemTypeItr)
    {
        if (!CheckExpandableItemAndGene(*itemTypeItr))
        {
            return false;
        }
    }

    return true;
}

bool PetConfigManager::CheckQualityInfo(unsigned short nPetTypeID) const
{
    std::map<unsigned short, PetQualityInfo>::const_iterator qualityItr = m_mapPetQuality.find(nPetTypeID);

    if (qualityItr == m_mapPetQuality.end())
    {
        WriteLog(LOGLEVEL_ERROR, "Can not find pet quality info by pet type ID(%u).", nPetTypeID);

        return false;
    }

    const PetImproveCondition *pCondition = NULL;
    unsigned char nMaxQualityType = qualityItr->second.GetMaxQualityType();

    for (unsigned char nQualityType = ePetQualityType_None + 1; nQualityType < nMaxQualityType; ++nQualityType)
    {
        pCondition = qualityItr->second.GetImproveQualityCondition(nQualityType);

        if (pCondition == NULL)
        {
            WriteLog(LOGLEVEL_ERROR, "Can not find pet quality improve condition by pet type ID(%u) and quality type(%u).", 
                nPetTypeID, nQualityType);

            return false;
        }
        else if (!CheckExpandableItem(pCondition->GetItem()))
        {
            return false;
        }
    }

    return true;
}

bool PetConfigManager::CheckEvolutionInfo(unsigned short nPetTypeID) const
{
    std::map<unsigned short, PetEvolutionInfo>::const_iterator evolutionItr = m_mapPetEvolution.find(nPetTypeID);

    if (evolutionItr == m_mapPetEvolution.end())
        return true;

    const PetEvolutionCondition *pCondition = NULL;
    unsigned char nMaxEvolutionTimes = evolutionItr->second.GetMaxEvolutionTimes();

    for (unsigned char nEvolutionTimes = 0; nEvolutionTimes < nMaxEvolutionTimes; ++nEvolutionTimes)
    {
        pCondition = evolutionItr->second.GetEvolutionCondition(nEvolutionTimes + 1);

        if (pCondition == NULL)
        {
            WriteLog(LOGLEVEL_ERROR, "Can not find pet evolution condition by pet type ID(%u) and evolution times(%u).", 
                nPetTypeID, nEvolutionTimes + 1);

            return false;
        }
        else if (!CheckExpandableItem(pCondition->GetItem()))
        {
            return false;
        }
        else if (pCondition->HasExtraItem() && !CheckExpandableItemAndGene(pCondition->GetExtraItem().m_nItemType))
        {
            return false;
        }
    }

    return true;
}

bool PetConfigManager::CheckExpandableItem(const std::list<CItem> &rItems) const
{
    ItemConfig *pItem = NULL;

    for (std::list<CItem>::const_iterator itr = rItems.begin(); itr != rItems.end(); ++itr)
    {
        pItem = ConfigManager::Instance().GetItemConfigManager().GetByID(itr->m_nItemType);

        if (pItem == NULL)
        {
            WriteLog(LOGLEVEL_ERROR, "Item(%u) is not exist.", itr->m_nItemType);

            return false;
        }
        else if (!pItem->IsExpandable())
        {
            WriteLog(LOGLEVEL_ERROR, "Item(%u) is not expandable.", itr->m_nItemType);

            return false;
        }
    }

    return true;
}

bool PetConfigManager::CheckExpandableItemAndGene(unsigned int nItemType) const
{
    ItemConfig *pItem = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);

    if (pItem == NULL)
    {
        WriteLog(LOGLEVEL_ERROR, "Item(%u) is not exist.", nItemType);

        return false;
    }
    else if (!pItem->IsExpandable())
    {
        WriteLog(LOGLEVEL_ERROR, "Item(%u) is not expandable.", nItemType);

        return false;
    }
    else if (pItem->m_listGene.empty())
    {
        WriteLog(LOGLEVEL_ERROR, "Item(%u) gene is empty", nItemType);

        return false;
    }

    return true;
}

void PetConfigManager::SetRewardPetEgg(const std::string &strRewardPetEgg)
{
    TransferItemList(strRewardPetEgg.c_str(), m_listGuideRewardPetEgg);
}

const std::list<CItem> &PetConfigManager::GetRewardPetEgg() const
{
    return m_listGuideRewardPetEgg;
}

