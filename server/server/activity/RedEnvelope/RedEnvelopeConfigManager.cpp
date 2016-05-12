#include "RedEnvelopeConfigManager.h"
#include "../../../datastructure/Macro_Define.h"

RedEnvelopeConfig::RedEnvelopeConfig()
    : m_nRedEnvelopeNum(0)
    , m_nRedEnvelopeContinueTime(0)
    , m_nRedEnvelopeGetAllDisappearTime(0)
    , m_nActivityBeginTime(0)
    , m_nActivityEndTime(0)
    , m_nShowBeginTime(0)
    , m_nShowEndTime(0)
    , m_bExhibit(false)
    , m_nWeight(0)
{
}

RedEnvelopeConfig::~RedEnvelopeConfig()
{

};

RedEnvelopeRechareConfig::RedEnvelopeRechareConfig()
    : m_nRechargeMCoin(0)
    , m_nCurrencyType(0)
    , m_nCurrencyValue(0)
    , m_nMinRate(0)
    , m_nFluctuateRate(0)
{

}

RedEnvelopeRechareConfig::~RedEnvelopeRechareConfig()
{

}

void RedEnvelopeRechareConfig::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRechargeMCoin);
    IOBuff.AddUInt(m_nCurrencyType);
    IOBuff.AddUInt(m_nCurrencyValue);
}

RedEnvelopeActivityData::RedEnvelopeActivityData()
: m_nActivityId(EActivity_None)
{

}

RedEnvelopeActivityData::~RedEnvelopeActivityData()
{

}

void RedEnvelopeActivityData::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUChar( (unsigned char )m_nActivityId );
    if( m_nActivityId == (int)EActivity_RedEnvelope )
    {
        IOBuff.AddBool(m_RedEnvelopeConfig.m_bExhibit);
        IOBuff.AddInt(m_RedEnvelopeConfig.m_nWeight);
        IOBuff.AddStringW(m_RedEnvelopeConfig.m_strRegularImageName);
        IOBuff.AddStringW(m_RedEnvelopeConfig.m_strThumbnailName);
        IOBuff.AddStringW(m_RedEnvelopeConfig.m_strTitle);
        IOBuff.AddStringW(m_RedEnvelopeConfig.m_strContent);
        IOBuff.AddUInt(m_RedEnvelopeConfig.m_nActivityBeginTime);
        IOBuff.AddUInt(m_RedEnvelopeConfig.m_nActivityEndTime);

        IOBuff.AddUShort((unsigned short)m_mapRedEnvelopeRechargeConfig.size());
        std::map<int,RedEnvelopeRechareConfig>::iterator it = m_mapRedEnvelopeRechargeConfig.begin();
        for (; it != m_mapRedEnvelopeRechargeConfig.end(); ++it)
        {
            IOBuff.AddUInt(it->second.m_nRechargeMCoin);
            IOBuff.AddUInt(it->second.m_nCurrencyType);
            IOBuff.AddUInt(it->second.m_nCurrencyValue);
        }
    }
}

RedEnvelopeConfigManager::RedEnvelopeConfigManager()
{

}

RedEnvelopeConfigManager::~RedEnvelopeConfigManager()
{

}

RedEnvelopeConfigManager & RedEnvelopeConfigManager::Instance()
{
    static RedEnvelopeConfigManager g_StaticConfigManager;

    return g_StaticConfigManager;
}

void RedEnvelopeConfigManager::AddRedEnvelopeConfig(const RedEnvelopeConfig & config)
{
    m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig = config;
}

void RedEnvelopeConfigManager::AddRedEnvelopeRechargeConfig(const RedEnvelopeRechareConfig & config)
{
    m_RedEnvelopeActivityConfig.m_mapRedEnvelopeRechargeConfig[config.m_nRechargeMCoin] = config;
}

RedEnvelopeRechareConfig * RedEnvelopeConfigManager::GetRedEnvelopeRechargeConfig(int nRechargeMCoin)
{
    std::map<int,RedEnvelopeRechareConfig>::reverse_iterator it = m_RedEnvelopeActivityConfig.m_mapRedEnvelopeRechargeConfig.rbegin();
    for (; it != m_RedEnvelopeActivityConfig.m_mapRedEnvelopeRechargeConfig.rend(); ++it)
    {
        if (it->second.m_nRechargeMCoin <= nRechargeMCoin)
        {
            return &(it->second);
        }
    }

    return NULL;
}

unsigned int RedEnvelopeConfigManager::GetRedEnvelopeCount()
{
    return m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nRedEnvelopeNum;
}

std::string RedEnvelopeConfigManager::GetAnonymityName()
{
    return m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_strAnonymityName;
}

unsigned int RedEnvelopeConfigManager::GetContinueTime()
{
    return m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nRedEnvelopeContinueTime;
}

bool RedEnvelopeConfigManager::IsActivityShow(unsigned int nNowTime)
{
    if (nNowTime >= m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nShowBeginTime && nNowTime <= m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nShowEndTime)
    {
        return true;
    }

    return false;
}

unsigned int RedEnvelopeConfigManager::GetAllDisappearTime()
{
    return m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nRedEnvelopeGetAllDisappearTime;
}

bool RedEnvelopeConfigManager::IsInActivity(unsigned int nNowTime)
{
    if (nNowTime >= m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nActivityBeginTime && nNowTime <= m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nActivityEndTime)
    {
        return true;
    }

    return false;
}

unsigned int RedEnvelopeConfigManager::GetActivityBeginTime()
{
    return m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nActivityBeginTime;
}

unsigned int RedEnvelopeConfigManager::GetActivityEndTime()
{
    return m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nActivityEndTime;
}

unsigned int RedEnvelopeConfigManager::GetShowBeginTime()
{
    return m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nShowBeginTime;
}

unsigned int RedEnvelopeConfigManager::GetShowEndTime()
{
    return m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig.m_nShowEndTime;
}

void RedEnvelopeConfigManager::GetRedEnvelopeConfig(RedEnvelopeActivityData & data)
{
	data.m_nActivityId = (int)EActivity_RedEnvelope;
	data.m_RedEnvelopeConfig = m_RedEnvelopeActivityConfig.m_RedEnvelopeConfig;
    data.m_mapRedEnvelopeRechargeConfig = m_RedEnvelopeActivityConfig.m_mapRedEnvelopeRechargeConfig;
}
