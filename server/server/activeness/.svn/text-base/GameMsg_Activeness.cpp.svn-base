#include "GameMsg_Activeness.h"
#include "ActivenessDataManager.h"

#include "../util/CommonFunc.h"



//////////////////////////////////////////////////////////////////////////
ActivenessTargetConfig::ActivenessTargetConfig()
    : m_nTargetType(0)
    , m_nTargetValue(0)
    , m_nTargetScore(0)
{
}

ActivenessTargetConfig::~ActivenessTargetConfig()
{

}

void ActivenessTargetConfig::Set(int nTargetType, int nTargetValue, int nTargetScore, const char *strDesc)
{
    m_nTargetType = nTargetType;
    m_nTargetValue = nTargetValue;
    m_nTargetScore = nTargetScore;
    if (strDesc != NULL)
    {
        m_strDesc = strDesc;
    }
    m_strName = GetNameFromType(nTargetType);
}

static std::string s_strTargetNameTable[ActivenessTargetConfig::EType_Max + 1] =
{
    "invalid",
    "checkin",
    "quest_daily",
    "dungeon",
    "dance_mode",
    "match_room",
    "vip_daily_reward",
    "social_share",
    "magicarray_call",
    "clotheffect_replace",
    "clotheffect_upgrade",
    "unknown"
};

const std::string &ActivenessTargetConfig::GetNameFromType(int nType)
{
    if (nType > 0 && nType < ActivenessTargetConfig::EType_Max)
    {
        return s_strTargetNameTable[nType];
    }
    return s_strTargetNameTable[ActivenessTargetConfig::EType_Invalid];
}

int ActivenessTargetConfig::GetTypeFromName(const std::string &name)
{
    static std::map<std::string, int> m_sNameMap;
    if (m_sNameMap.empty())
    {
        for (int i = 0; i < ActivenessTargetConfig::EType_Max; ++i)
        {
            m_sNameMap.insert(std::make_pair(s_strTargetNameTable[i], i));
        }
    }

    std::map<std::string, int>::const_iterator it = m_sNameMap.find(name);
    if (it != m_sNameMap.end())
    {
        return it->second;
    }
    return EType_Invalid;
}

void ActivenessTargetConfig::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddInt(m_nTargetType);
    IOBuff.AddInt(m_nTargetValue);
    IOBuff.AddInt(m_nTargetScore);
    IOBuff.AddStringW(m_strDesc);
}

//////////////////////////////////////////////////////////////////////////
ActivenessRewardConfig::ActivenessRewardConfig()
    : m_nRequireScore(0)
{
}

ActivenessRewardConfig::~ActivenessRewardConfig()
{

}

void ActivenessRewardConfig::Set(int nRequireScore, const char *strMaleItem, const char *strFemaleItem, int nMoney,
    int nBindMCoin, const char *strVIPMaleItem, const char *strVIPFemaleItem, int nVIPMoney, int nVIPBindMCoin)
{
    m_nRequireScore = nRequireScore;

    m_femaleReward.m_nRewardType = m_maleReward.m_nRewardType = ERewardType_Normal;
    m_femaleReward.m_nMoney = m_maleReward.m_nMoney = nMoney;
    m_femaleReward.m_nBindMCoin = m_maleReward.m_nBindMCoin = nBindMCoin;

    if (strMaleItem != NULL)
        TransferItemList(strMaleItem, m_maleReward.m_listItems);
    if (strFemaleItem != NULL)
        TransferItemList(strFemaleItem, m_femaleReward.m_listItems);

    m_vipFemaleReward.m_nRewardType = m_vipMaleReward.m_nRewardType = ERewardType_VIP;
    m_vipFemaleReward.m_nMoney = m_vipMaleReward.m_nMoney = nVIPMoney;
    m_vipFemaleReward.m_nBindMCoin = m_vipMaleReward.m_nBindMCoin = nVIPBindMCoin;

    if (strVIPMaleItem != NULL)
        TransferItemList(strVIPMaleItem, m_vipMaleReward.m_listItems);
    if (strVIPFemaleItem != NULL)
        TransferItemList(strVIPFemaleItem, m_vipFemaleReward.m_listItems);
}

void ActivenessRewardConfig::doEncode(CParamPool & IOBuff, ESexType sex)
{
    IOBuff.AddInt(m_nRequireScore);
    if (sex == ESexType_Male)
    {
        m_maleReward.doEncode(IOBuff);
        m_vipMaleReward.doEncode(IOBuff);
    }
    else
    {
        m_femaleReward.doEncode(IOBuff);
        m_vipFemaleReward.doEncode(IOBuff);
    }
}

int ActivenessRewardConfig::GetRequireScore() const
{
    return m_nRequireScore;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetActivenessData::GameMsg_C2S_GetActivenessData()
    : GameMsg_Base(MSG_C2S_GetActivenessData)
{
}

GameMsg_C2S_GetActivenessData::~GameMsg_C2S_GetActivenessData()
{
}

bool GameMsg_C2S_GetActivenessData::doDecode( CParamPool &IOBuff )
{
    return true;
}

//////////////////////////////////////////////////////////////////////////
ActivenessTarget::ActivenessTarget(const ActivenessTargetConfig & targetConfig)
    : m_TargetConfig(targetConfig)
    , m_nProgress(0)
{

}

bool ActivenessTarget::IsTargetComplete()
{
    if ( m_TargetConfig.m_nTargetValue <= m_nProgress )
    {
        return true;
    }

    return false;
}

int ActivenessTarget::GetTargetType() const
{
    return m_TargetConfig.m_nTargetType;
}

int ActivenessTarget::GetTargetScore() const
{
    return m_TargetConfig.m_nTargetScore;
}

int ActivenessTarget::GetProgress() const
{
    return m_nProgress;
}

void ActivenessTarget::SetProgress(int nProgress)
{
    m_nProgress = nProgress;
}

void ActivenessTarget::doEncode(CParamPool & IOBuff)
{
    // 以下和客户端编码保持一致
    IOBuff.AddUChar((unsigned char)m_TargetConfig.m_nTargetType);
    IOBuff.AddUInt((unsigned int)m_TargetConfig.m_nTargetValue);
    IOBuff.AddUInt((unsigned int)m_nProgress);
    IOBuff.AddUInt((unsigned int)m_TargetConfig.m_nTargetScore);
    IOBuff.AddStringW(m_TargetConfig.m_strDesc);
}

bool ActivenessTarget::UpdateProgress(IParam & param)
{
    m_nProgress++;
    return true;
}

ActivenessDanceModeTarget::ActivenessDanceModeTarget(const ActivenessTargetConfig & targetConfig)
    : ActivenessTarget(targetConfig)
{

}

bool ActivenessDanceModeTarget::UpdateProgress(IParam & param)
{
    int nDanceMode = 0;
    if (param.GetParam(ActivenessTargetConfig::GetNameFromType(m_TargetConfig.m_nTargetType), nDanceMode))
    {
        if (m_setFinishedDanceMode.find(nDanceMode) == m_setFinishedDanceMode.end())
        {
            m_setFinishedDanceMode.insert(nDanceMode);
            m_nProgress = (int)m_setFinishedDanceMode.size();

            return true;
        }
    }

    return false;
}

void ActivenessDanceModeTarget::SetFinishedDanceMode(std::set<int> & finishedDanceMonde)
{
    m_setFinishedDanceMode = finishedDanceMonde;
}

void ActivenessDanceModeTarget::GetFinisheDanceMode(std::set<int> & finishedDanceMode) const
{
    finishedDanceMode = m_setFinishedDanceMode;
}

ActivenessTargetFactory::ActivenessTargetFactory()
{

}

ActivenessTargetFactory & ActivenessTargetFactory::Instance()
{
    static ActivenessTargetFactory m_stcTargetFactory;
    return m_stcTargetFactory;
}

ActivenessTarget * ActivenessTargetFactory::CreateTarget(const ActivenessTargetConfig & targetConfig)
{
    switch(targetConfig.m_nTargetType)
    {
    case ActivenessTargetConfig::EType_Checkin:
    case ActivenessTargetConfig::EType_DailyQuest:
    case ActivenessTargetConfig::EType_Dungeon:
    case ActivenessTargetConfig::EType_QuickDance:
    case ActivenessTargetConfig::EType_VipReward:
    case ActivenessTargetConfig::EType_SocialShare:
    case ActivenessTargetConfig::EType_MagicArrayCall:
    case ActivenessTargetConfig::EType_ClothEffectReplace:
    case ActivenessTargetConfig::EType_ClothEffectUpgrade:
        {
            return new ActivenessTarget(targetConfig);
        }
        break;
    case ActivenessTargetConfig::EType_DanceMode:
        {
            return new ActivenessDanceModeTarget(targetConfig);
        }
        break;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////
ActivenessReward::ActivenessReward(const ActivenessRewardConfig & rewardConfig)
    : m_ActivenessRewardConfig(rewardConfig)
    , m_nRewardIndex(0)
    , m_bReceived(false)
{
}

void ActivenessReward::doEncode(CParamPool & IOBuff, ESexType sex)
{
    IOBuff.AddUShort((unsigned short)m_nRewardIndex); // 和客户端编码保持一致
    IOBuff.AddUInt(m_ActivenessRewardConfig.m_nRequireScore);
    IOBuff.AddBool(m_bReceived);

    if (sex == ESexType_Male)
    {
        IOBuff.AddUShort((unsigned short)m_ActivenessRewardConfig.m_maleReward.m_listItems.size());
        for (std::list<CItem>::const_iterator itr = m_ActivenessRewardConfig.m_maleReward.m_listItems.begin();
            itr != m_ActivenessRewardConfig.m_maleReward.m_listItems.end(); ++itr)
        {
            itr->doEncode(IOBuff);
        }

        IOBuff.AddInt(m_ActivenessRewardConfig.m_maleReward.m_nMoney);
        IOBuff.AddInt(m_ActivenessRewardConfig.m_maleReward.m_nBindMCoin);
    }
    else
    {
        IOBuff.AddUShort((unsigned short)m_ActivenessRewardConfig.m_femaleReward.m_listItems.size());
        for (std::list<CItem>::const_iterator itr = m_ActivenessRewardConfig.m_femaleReward.m_listItems.begin();
            itr != m_ActivenessRewardConfig.m_femaleReward.m_listItems.end(); ++itr)
        {
            itr->doEncode(IOBuff);
        }

        IOBuff.AddInt(m_ActivenessRewardConfig.m_femaleReward.m_nMoney);
        IOBuff.AddInt(m_ActivenessRewardConfig.m_femaleReward.m_nBindMCoin);
    }
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetActivenessDataRes::GameMsg_S2C_GetActivenessDataRes()
    : GameMsg_Base(MSG_S2C_GetActivenessDataRes)
{

}

GameMsg_S2C_GetActivenessDataRes::~GameMsg_S2C_GetActivenessDataRes()
{

}

bool GameMsg_S2C_GetActivenessDataRes::doEncode( CParamPool &IOBuff )
{
    // 当日活跃度
    IOBuff << (unsigned short)m_nActiveness;    // 和客户端保持一致，但服务器内部使用int

    // 奖励情况
    IOBuff << (unsigned short)m_listRewardState.size();
    for (std::list<ActivenessReward>::iterator it = m_listRewardState.begin();
        it != m_listRewardState.end(); ++it)
    {
        (*it).doEncode(IOBuff, m_eSex);
    }

    // 目标达成情况
    IOBuff << (unsigned short)m_listTargetProgress.size();
    for (std::list<ActivenessTarget>::iterator it = m_listTargetProgress.begin();
        it != m_listTargetProgress.end(); ++it)
    {
        (*it).doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetActivenessReward::GameMsg_C2S_GetActivenessReward()
    : GameMsg_Base(MSG_C2S_GetActivenessReward)
    , m_nRewardIndex(-1)
{

}

GameMsg_C2S_GetActivenessReward::~GameMsg_C2S_GetActivenessReward()
{

}

bool GameMsg_C2S_GetActivenessReward::doDecode( CParamPool &IOBuff )
{
    m_nRewardIndex = (int)IOBuff.GetUShort(); // 和客户端类似保持一致，但服务器内部方便扩展
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetActivenessRewardRes::GameMsg_S2C_GetActivenessRewardRes()
    : GameMsg_Base(MSG_S2C_GetActivenessRewardRes)
    , m_strError("")
    , m_nRewardIndex(0)
    , m_bResult(false)
{

}

GameMsg_S2C_GetActivenessRewardRes::~GameMsg_S2C_GetActivenessRewardRes()
{

}

bool GameMsg_S2C_GetActivenessRewardRes::doEncode( CParamPool &IOBuff )
{
    IOBuff << (unsigned short)m_nRewardIndex << m_bResult;
    IOBuff.AddStringW(m_strError);
    return true;
}

