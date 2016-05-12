#include "FriendDataManager.h"
#include "../socket/ParamPool.h"

CFindFriendCondition::CFindFriendCondition()
{
    m_nIndex = 0;
    m_nConditionMin = 0;
    m_nConditionMax = 0;
}

void CFindFriendCondition::doEncode(CParamPool &ioBuff)
{
    ioBuff.AddUChar((unsigned char)m_nIndex);
    ioBuff.AddStringW(m_strDesc.c_str());
}

CFriendDataManager::CFriendDataManager()
    : m_nFindFriendCoolDown(0)
    , m_nFindFriendMaxCount(0)
    , m_nRecommendResetSeconds(0)
    , m_nRecommendRefreshCount(0)
{
}

CFriendDataManager::~CFriendDataManager()
{
}

CFriendDataManager & CFriendDataManager::Instance()
{
    static CFriendDataManager instance;
    return instance;
}

void CFriendDataManager::SetFindFriend(int nCoolDown, int nPlayerCount)
{
    m_nFindFriendCoolDown = nCoolDown;
    m_nFindFriendMaxCount = nPlayerCount;
}

void CFriendDataManager::AddFindFriendType(int nIndex, const char *strDesc, int nType)
{
    AddFindFriendCondition(m_FindFriendType, nIndex, strDesc, nType, nType);
}

void CFriendDataManager::AddFindFriendLevel(int nIndex, const char *strDesc, int nLevelMin, int nLevelMax)
{
    AddFindFriendCondition(m_FindFriendLevel, nIndex, strDesc, nLevelMin, nLevelMax);
}

void CFriendDataManager::AddFindFriendSex(int nIndex, const char *strDesc, int nSex)
{
    AddFindFriendCondition(m_FindFriendSex, nIndex, strDesc, nSex, nSex);
}

void CFriendDataManager::AddFindFriendAge(int nIndex, const char *strDesc, int nAgeMin, int nAgeMax)
{
    AddFindFriendCondition(m_FindFriendAge, nIndex, strDesc, nAgeMin, nAgeMax);
}

void CFriendDataManager::AddFindFriendConstellation(int nIndex, const char *strDesc, int nConstellation)
{
    AddFindFriendCondition(m_FindFriendConstellation, nIndex, strDesc, nConstellation, nConstellation);
}

void CFriendDataManager::SetRecommendFriend(unsigned int nResetTime, unsigned int nCD, unsigned int nRefreshCount)
{
    m_nRecommendResetSeconds = nResetTime * 60;// transform minute to seconds
    m_nRecommendRefreshCount = nRefreshCount;
    m_nRecommendCD = nCD;
}

void CFriendDataManager::AddFindFriendCondition(std::map<int, CFindFriendCondition> &mapCondition,
                            int nIndex, const char *strDesc, int nCondMin, int nCondMax)
{
    CFindFriendCondition condition;
    condition.m_nIndex = nIndex;
    condition.m_strDesc = strDesc;
    condition.m_nConditionMin = nCondMin;
    condition.m_nConditionMax = nCondMax;

    mapCondition[nIndex] = condition;
}

CFindFriendCondition *CFriendDataManager::GetTypeCondition(int nIndex)
{
    if (m_FindFriendType.find(nIndex) != m_FindFriendType.end())
    {
        return &m_FindFriendType[nIndex];
    }
    return NULL;
}

CFindFriendCondition *CFriendDataManager::GetLevelCondition(int nIndex)
{
    if (m_FindFriendLevel.find(nIndex) != m_FindFriendLevel.end())
    {
        return &m_FindFriendLevel[nIndex];
    }
    return NULL;
}

CFindFriendCondition *CFriendDataManager::GetSexCondition(int nIndex)
{
    if (m_FindFriendSex.find(nIndex) != m_FindFriendSex.end())
    {
        return &m_FindFriendSex[nIndex];
    }
    return NULL;
}

CFindFriendCondition *CFriendDataManager::GetAgeCondition(int nIndex)
{
    if (m_FindFriendAge.find(nIndex) != m_FindFriendAge.end())
    {
        return &m_FindFriendAge[nIndex];
    }
    return NULL;
}

CFindFriendCondition *CFriendDataManager::GetConstellationCondition(int nIndex)
{
    if (m_FindFriendConstellation.find(nIndex) != m_FindFriendConstellation.end())
    {
        return &m_FindFriendConstellation[nIndex];
    }
    return NULL;
}

