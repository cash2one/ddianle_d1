#include "EffectStoneExchangeManager.h"

EffectStoneExchangeManager::EffectStoneExchangeManager()
{}

EffectStoneExchangeManager::~EffectStoneExchangeManager()
{}

EffectStoneExchangeManager & EffectStoneExchangeManager::Instance()
{
    static EffectStoneExchangeManager s_ExchangeProcessor;

    return s_ExchangeProcessor;
}

void EffectStoneExchangeManager::AddActivityConfig(std::string & strTitle, std::string & strContent, unsigned int nBeginTime, unsigned int nEndTime,
        std::string strRegularImageName, std::string strThumbnailName)
{
    m_strActivityTitle = strTitle;
    m_strActivityContent = strContent;
    m_nActivityBeginTime = nBeginTime;
    m_nActivityEndTime = nEndTime;
    m_strRegularImageName = strRegularImageName;
    m_strThumbnailName = strThumbnailName;
}

void EffectStoneExchangeManager::AddExchangeItem(itemtype_t nOldItem, const CItem & newItem)
{
    m_mapExchange[nOldItem] = newItem;
}

bool EffectStoneExchangeManager::IsActivityOpen()
{
    unsigned int nNowTime = (unsigned int)time(NULL);
    return nNowTime >= m_nActivityBeginTime && nNowTime <= m_nActivityEndTime;
}

const std::map<itemtype_t, CItem> & EffectStoneExchangeManager::GetExchangeItem()
{
    return m_mapExchange;
}

void EffectStoneExchangeManager::GetEffectStoneExchangeConfig(EffectStoneExchangeData & data)
{
    data.m_nActivityId = (int)EActivity_EffectStoneExchange;
    data.m_strActivityTitle = m_strActivityTitle;
    data.m_strActivityContent = m_strActivityContent;
    data.m_strRegularImageName = m_strRegularImageName;
    data.m_strThumbnailName = m_strThumbnailName;
}

EffectStoneExchangeData::EffectStoneExchangeData()
:m_nActivityId(0)
{

}

EffectStoneExchangeData::~EffectStoneExchangeData()
{

}

void EffectStoneExchangeData::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddChar((char)m_nActivityId);
    if (m_nActivityId == (int)EActivity_EffectStoneExchange)
    {
        IOBuff.AddStringW(m_strRegularImageName);
        IOBuff.AddStringW(m_strThumbnailName);
        IOBuff.AddStringW(m_strActivityTitle);
        IOBuff.AddStringW(m_strActivityContent);
        IOBuff.AddUShort((unsigned short)m_listExchangeItem.size());
        for (std::list<CItem>::iterator it = m_listExchangeItem.begin(); it != m_listExchangeItem.end(); ++it)
        {
            (*it).doEncode(IOBuff);
        }
    }
}






