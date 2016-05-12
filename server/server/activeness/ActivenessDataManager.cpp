#include "ActivenessDataManager.h"

#include "../util/CommonFunc.h"

//////////////////////////////////////////////////////////////////////////
ActivenessDataManager::ActivenessDataManager()
    : m_strActivenessName("")
    , m_strActivenessDesc("")
    , m_nStartTime(0)
    , m_nEndTime(0)
{

}

ActivenessDataManager::~ActivenessDataManager()
{

}

ActivenessDataManager &ActivenessDataManager::Instance()
{
    static ActivenessDataManager s_instance;
    return s_instance;
}


void ActivenessDataManager::SetActiveness(
    const char *strName, const char *strDesc, 
    const char *strStartTime, const char *strEndTime,
    const char *strMailSender, const char *strMailTitle, const char *strMailContent, 
    const char *strVIPMailTitle, const char *strVIPMailContent)
{
    if (strName != NULL)
        m_strActivenessName = strName;
    if (strDesc != NULL)
        m_strActivenessDesc = strDesc;

    if (strStartTime != NULL)
        m_nStartTime = (unsigned int)SetTime(strStartTime);
    if (strEndTime != NULL)
        m_nEndTime = (unsigned int)SetTime(strEndTime);

    if (strMailSender != NULL)
        m_strMailSender = strMailSender;
    if (strMailTitle != NULL)
        m_strMailTitle = strMailTitle;
    if (strMailContent != NULL)
        m_strMailContent = strMailContent;
    if (strVIPMailTitle != NULL)
        m_strVIPMailTitle = strVIPMailTitle;
    if (strVIPMailContent != NULL)
        m_strVIPMailContent = strVIPMailContent;
}

void ActivenessDataManager::AddActivenessTarget(
    int nTargetType, int nTargetValue, int nTargetScore, const char *strDesc)
{
    ActivenessTargetConfig target;
    target.Set(nTargetType, nTargetValue, nTargetScore, strDesc);
    m_mapTarget.insert(std::make_pair(nTargetType, target));
}

void ActivenessDataManager::AddActivenessReward(int nRequireScore,
    const char *strMaleItem, const char *strFemaleItem, int nMoney, int nBindMCoin,
    const char *strVIPMaleItem, const char *strVIPFemaleItem, int nVIPMoney, int nVIPBindMCoin)
{
    ActivenessRewardConfig reward;
    reward.Set(nRequireScore, strMaleItem, strFemaleItem, nMoney, nBindMCoin, 
        strVIPMaleItem, strVIPFemaleItem, nVIPMoney, nVIPBindMCoin);
    m_mapReward.insert(std::make_pair(nRequireScore, reward));
}

bool ActivenessDataManager::IsOpen() const
{
    unsigned int nNow = (unsigned int)time(NULL);
    return (nNow >= m_nStartTime) && (nNow <= m_nEndTime);
}

bool ActivenessDataManager::GetTarget(int nTargetType, ActivenessTargetConfig & targetConfig) const
{
    std::map<int, ActivenessTargetConfig>::const_iterator it = m_mapTarget.find( nTargetType );
    if ( it != m_mapTarget.end())
    {
        targetConfig = (it->second);
        return true;
    }

    return false;
}

bool ActivenessDataManager::GetReward(int nRewardIndex, ActivenessRewardConfig &reward) const
{
    if (nRewardIndex < 0 || nRewardIndex >= (int)m_mapReward.size())
    {
        return false;
    }

    int nIndex = 0;
    std::map<int, ActivenessRewardConfig>::const_iterator it = m_mapReward.begin();
    while (it != m_mapReward.end() && nIndex < nRewardIndex)
    {
        ++it;
        ++nIndex;
    }

    if (it != m_mapReward.end())
    {
        reward = it->second;
        return true;
    }

    return false;
}


void ActivenessDataManager::GetTargetRewardConfigList(std::list<ActivenessRewardConfig> & listRewardConfig)
{
    int nRewardIndex = 0;
    for (std::map<int, ActivenessRewardConfig>::const_iterator it = m_mapReward.begin();
        it != m_mapReward.end(); ++it, ++nRewardIndex)
    {
        listRewardConfig.push_back(it->second);
    }
}

void ActivenessDataManager::GetTargetConfigList(std::list<ActivenessTargetConfig> & listTarget)
{
    for (std::map<int, ActivenessTargetConfig>::iterator it = m_mapTarget.begin();
        it != m_mapTarget.end(); ++it)
    {
        listTarget.push_back(it->second);
    }
}
