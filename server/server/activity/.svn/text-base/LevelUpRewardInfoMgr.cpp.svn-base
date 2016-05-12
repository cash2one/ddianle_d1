#include "LevelUpRewardInfoMgr.h"

//////////////////////////////////////////////////////////////////////////
CLevelRewardInfo::CLevelRewardInfo()
    : m_nLevel(0)
    , m_nMoney(0)
    , m_nBindMCoin(0)
{
}

CLevelRewardInfo::~CLevelRewardInfo()
{
}


//////////////////////////////////////////////////////////////////////////
CLevelUpFunction::CLevelUpFunction()
    : m_nLevel(0)
    , m_nFuncID(0)
{
}

CLevelUpFunction::~CLevelUpFunction()
{
}


//////////////////////////////////////////////////////////////////////////
CLevelUpInfoMgr::CLevelUpInfoMgr()
{

}

CLevelUpInfoMgr::~CLevelUpInfoMgr()
{
    m_mapLevelUpRewardInfo.clear();
    m_mapVIPLevelUpRewardInfo.clear();
}

CLevelUpInfoMgr & CLevelUpInfoMgr::Instance()
{
    static CLevelUpInfoMgr s_LevelUpRewardInfoMgr;

    return s_LevelUpRewardInfoMgr;
}

void CLevelUpInfoMgr::AddRewardInfo(unsigned short nLevel, const CLevelRewardInfo &rewardInfo, bool bVIPReward)
{
    if (bVIPReward)
    {
        m_mapVIPLevelUpRewardInfo.insert(make_pair(nLevel, rewardInfo));
    }
    else
    {
        m_mapLevelUpRewardInfo.insert(make_pair(nLevel, rewardInfo));
    }
}

bool CLevelUpInfoMgr::GetRewardInfo(unsigned short nLevel, CLevelRewardInfo &rRewardInfo)
{
    std::map<unsigned short, CLevelRewardInfo>::iterator itr = m_mapLevelUpRewardInfo.find(nLevel);
    if (itr != m_mapLevelUpRewardInfo.end())
    {
        rRewardInfo = itr->second;
        return true;
    }
    return false;
}

bool CLevelUpInfoMgr::GetVIPRewardInfo(unsigned short nLevel, CLevelRewardInfo &rRewardInfo)
{
    std::map<unsigned short, CLevelRewardInfo>::iterator itr = m_mapVIPLevelUpRewardInfo.find(nLevel);
    if (itr != m_mapVIPLevelUpRewardInfo.end())
    {
        rRewardInfo = itr->second;
        return true;
    }
    return false;
}

bool CLevelUpInfoMgr::IsRewardLevel(unsigned short nCurLevel)
{
    if ( m_mapLevelUpRewardInfo.find( nCurLevel ) != m_mapLevelUpRewardInfo.end() )
    {
        return true;
    }

    return false;
}

bool CLevelUpInfoMgr::IsVIPRewardLevel(unsigned short nCurLevel)
{
    if ( m_mapVIPLevelUpRewardInfo.find( nCurLevel ) != m_mapVIPLevelUpRewardInfo.end() )
    {
        return true;
    }

    return false;
}

unsigned short CLevelUpInfoMgr::GetNextRewardLevel(unsigned short nCurLevel)
{
    unsigned short nNextLevel = 0;

    std::map< unsigned short, CLevelRewardInfo>::iterator it = m_mapLevelUpRewardInfo.find(nCurLevel);
    if ( it != m_mapLevelUpRewardInfo.end() )
    {
        ++it;

        if ( it != m_mapLevelUpRewardInfo.end() )
        {
            nNextLevel = it->first; 
        }
    }

    return nNextLevel;
}

void CLevelUpInfoMgr::AddLevelUpRecommend(unsigned short nLevel, int nFuncID, const char *strFuncName)
{
    CLevelUpFunction function;
    function.m_nLevel = nLevel;
    function.m_nFuncID = nFuncID;
    if (strFuncName != NULL)
    {
        function.m_strFuncName = strFuncName;
    }

    m_mapOpenFunction[nLevel] = function;
}

bool CLevelUpInfoMgr::GetLevelUpRecommend(unsigned short nLevel, int &nFuncID, std::string &strFuncName)
{
    std::map<unsigned short, CLevelUpFunction>::iterator itr = m_mapOpenFunction.find(nLevel);
    if (itr != m_mapOpenFunction.end())
    {
        nFuncID = itr->second.m_nFuncID;
        strFuncName = itr->second.m_strFuncName;
        return true;
    }
    return false;
}

