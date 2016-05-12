#include "MagicArrayDataManager.h"

MagicArrayDataManager::MagicArrayDataManager()
    : m_nFreeInterval(0)
    , m_nBastRollLuckDrawCount(0)
    , m_nNextDayResetTime(0)
    , m_nTenRollCost(0)
    , m_bResetFirstFree(false)
{
}

MagicArrayDataManager::~MagicArrayDataManager()
{

}

MagicArrayDataManager & MagicArrayDataManager::Instance()
{
    static MagicArrayDataManager g_staticConfigManager;
    return g_staticConfigManager;
}

void MagicArrayDataManager::AddMagicLuckDrawReward(int nRewardID, const CItem & maleItem, const CItem & femaleItem, int nRate)
{
//     MagicLuckDrawReward malereward;
//     malereward.m_ItemReward = maleItem;
//     malereward.m_nRate = nRate;
//     m_mapMaleReward[nRewardID].push_back(malereward);
// 
//     MagicLuckDrawReward femalereward;
//     femalereward.m_ItemReward = femaleItem;
//     femalereward.m_nRate = nRate;
//     m_mapFemaleReward[nRewardID].push_back(femalereward);
}

void MagicArrayDataManager::AddOnceRollRate(int nRate_1, int nRate_2, int nRate_3, int nRate_4)
{
    m_vecOnceRollRate.push_back(nRate_1);
    m_vecOnceRollRate.push_back(nRate_2);
    m_vecOnceRollRate.push_back(nRate_3);
    m_vecOnceRollRate.push_back(nRate_4);
}

void MagicArrayDataManager::AddFirstRollRate(int nRate_1, int nRate_2, int nRate_3, int nRate_4)
{
    m_vecFirstRollRate.push_back(nRate_1);
    m_vecFirstRollRate.push_back(nRate_2);
    m_vecFirstRollRate.push_back(nRate_3);
    m_vecFirstRollRate.push_back(nRate_4);
}

void MagicArrayDataManager::AddMagicLuckDrawConfig(unsigned int nInterval, unsigned int nBastRollCount, unsigned int nResetTime, unsigned int nTenRollCost, bool bResetFirstFree)
{
    m_nFreeInterval = nInterval;
    m_nBastRollLuckDrawCount = nBastRollCount;
    m_nNextDayResetTime = nResetTime;
    m_nTenRollCost = nTenRollCost;
    m_bResetFirstFree = bResetFirstFree;
}

unsigned int MagicArrayDataManager::GetFreeIntervalTime()
{
    return m_nFreeInterval;
}

unsigned int MagicArrayDataManager::GetTenRollCost()
{
    return m_nTenRollCost;
}

unsigned int MagicArrayDataManager::GetAccumulatedCountConfig()
{
    return m_nBastRollLuckDrawCount;
}

unsigned int MagicArrayDataManager::GetOnceRollCost(unsigned int nRollCount)
{
    unsigned int nCostCount = (unsigned int)m_vecOnceRollCost.size();
    if (nRollCount == 0)
    {
        return m_vecOnceRollCost[nCostCount - 1];
    }

    if (nRollCount > nCostCount)
    {
        return m_vecOnceRollCost[nCostCount - 1];
    }
    else
    {
        return m_vecOnceRollCost[nRollCount - 1];
    }
}

int MagicArrayDataManager::RollOnce(const vector<int> & vecRollRate)
{
    int nTotalRate = 0;
    std::vector<int>::const_iterator it = vecRollRate.begin();
    for (; it != m_vecFirstRollRate.end(); ++it)
    {
        nTotalRate += *it;
    }

    int nRollValue = Random(1, nTotalRate);
    it = m_vecFirstRollRate.begin();
    int nTempValue = 0;
    int nVecIndex = 0;
    for (; it != m_vecFirstRollRate.end(); ++it)
    {
        nTempValue += *it;
        nVecIndex++;
        if (nRollValue <= nTempValue)
        {
            return nVecIndex;
        }
    }

    return (int)m_vecFirstRollRate.size() + 1;
}

void MagicArrayDataManager::RandomInTen(std::vector<int> & vecGroupID)
{
    std::map<int,std::vector<int> >::iterator it = m_mapTenRollRate.begin();
    for (; it != m_mapTenRollRate.end(); ++it)
    {
        vecGroupID.push_back(RollOnce(it->second));
    }
}

int MagicArrayDataManager::RandomInFirst()
{
    return RollOnce(m_vecFirstRollRate);
}

int MagicArrayDataManager::RandomInOnce()
{
    return RollOnce(m_vecOnceRollRate);
}

void MagicArrayDataManager::AddOnceRollCost(int nCost_1, int nCost_2, int nCost_3, int nCost_4)
{
    m_vecOnceRollCost.push_back(nCost_1);
    m_vecOnceRollCost.push_back(nCost_2);
    m_vecOnceRollCost.push_back(nCost_3);
    m_vecOnceRollCost.push_back(nCost_4);
}

void MagicArrayDataManager::AddTenRollRate(int nRollIndex, int nRewardID, int nRate)
{
    std::map<int,std::vector<int> >::iterator it = m_mapTenRollRate.find(nRollIndex);
    if (it != m_mapTenRollRate.end())
    {
        it->second.push_back(nRate);
    }
    else
    {
       // m_mapTenRollRate.insert(std::make_pair(nRollIndex,nRate));
    }
}
