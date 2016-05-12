#include "GameMsg_CS_MagicLuckDraw.h"
#include "../../share/ServerMsgDef.h"
#include "../../../datastructure/datainfor/MagicArrayConfigManager.h"
#include "../../../datastructure/datainfor/ConfigManager.h"

GameMsg_C2S_RequestMagicArrayConfig::GameMsg_C2S_RequestMagicArrayConfig(void)
: GameMsg_Base(MSG_C2S_RequestMagicArrayConfig)
{
}

GameMsg_C2S_RequestMagicArrayConfig::~GameMsg_C2S_RequestMagicArrayConfig()
{

}

bool GameMsg_C2S_RequestMagicArrayConfig::doDecode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_S2C_RequestMagicArrayConfigResult::GameMsg_S2C_RequestMagicArrayConfigResult(void)
: GameMsg_Base(MSG_S2C_RequestMagicArrayConfigResult)
{
}

GameMsg_S2C_RequestMagicArrayConfigResult::~GameMsg_S2C_RequestMagicArrayConfigResult()
{

}

bool GameMsg_S2C_RequestMagicArrayConfigResult::doEncode(CParamPool &IOBuff)
{
    int nCurrencyType = 0, nCurrencyValue = 0;
    ConfigManager::Instance().GetMagicArrayConfigMgr().GetRefreshExchangeCost(nCurrencyType, nCurrencyValue);

    IOBuff.AddUChar((unsigned char)nCurrencyType);
    IOBuff.AddInt(nCurrencyValue);

    std::list<unsigned int> listMagicArrayID;
    ConfigManager::Instance().GetMagicArrayConfigMgr().GetMagicArrayID(listMagicArrayID);

    IOBuff.AddUShort((unsigned short)listMagicArrayID.size());
    std::list<unsigned int>::iterator it = listMagicArrayID.begin();
    for (; it != listMagicArrayID.end(); ++it)
    {
        ConfigManager::Instance().GetMagicArrayConfigMgr().doConfigEndoce(*it, IOBuff);

        std::map<unsigned int, unsigned int>::iterator itt = m_mapCDRecord.find(*it);
        if (itt != m_mapCDRecord.end())
        {
            IOBuff.AddFloat((float)itt->second);
        }
        else
        {
            IOBuff.AddFloat(0);
        }

        std::map<unsigned int, MagicArrayRecord>::iterator itr = m_mapRecord.find(*it);
        if (itr != m_mapRecord.end())
        {
            IOBuff.AddBool(itr->second.m_bHaveFirstCall);
        }
        else
        {
            IOBuff.AddBool(false);
        }
    }
    
	return true;
}

GameMsg_C2S_PreViewAllReward::GameMsg_C2S_PreViewAllReward(void)
    : GameMsg_Base(MSG_C2S_PreviewAllReward)
    , m_nMagicArrayID(0)
    , m_nGroupID(0)
    , m_nPage(0)
    , m_nPageCount(0)
{
}

GameMsg_C2S_PreViewAllReward::~GameMsg_C2S_PreViewAllReward()
{

}

bool GameMsg_C2S_PreViewAllReward::doDecode(CParamPool &IOBuff)
{
    m_nMagicArrayID = (unsigned int)IOBuff.GetUShort();
    m_nGroupID = (unsigned int)IOBuff.GetUChar();
    m_nPage = IOBuff.GetInt();
    m_nPageCount = IOBuff.GetInt();

    return true;
}

GameMsg_S2C_PreViewAllRewardResult::GameMsg_S2C_PreViewAllRewardResult(void)
    : GameMsg_Base(MSG_S2C_PreviewAllRewardResult)
    , m_nSex(0)
{
}

GameMsg_S2C_PreViewAllRewardResult::~GameMsg_S2C_PreViewAllRewardResult()
{

}

bool GameMsg_S2C_PreViewAllRewardResult::doEncode(CParamPool &IOBuff)
{
    std::vector<CItem> vecPreView;
    ConfigManager::Instance().GetMagicArrayConfigMgr().GetReward(m_nMagicArrayID, m_nGroupID + 1, m_nSex, vecPreView);

    std::list<CItem> listShowItem;
    int nBeginIndex = m_nPage*m_nPageCount;
    int nEndIndex = (int)vecPreView.size() >= (m_nPage + 1)*m_nPageCount ? (nBeginIndex + m_nPageCount) : (int)vecPreView.size();
    for (; nBeginIndex < nEndIndex; ++nBeginIndex)
    {
        if ((int)vecPreView.size() >= nBeginIndex + 1)
        {
            listShowItem.push_back(vecPreView[nBeginIndex]);
        }
    }

    IOBuff.AddUShort((unsigned short)m_nMagicArrayID);
    IOBuff.AddUChar((unsigned char)m_nGroupID);
    IOBuff.AddInt(m_nPage);
    IOBuff.AddInt((int)vecPreView.size());

    IOBuff.AddUShort((unsigned short)listShowItem.size());
    std::list<CItem>::iterator it = listShowItem.begin();
    for (; it != listShowItem.end(); ++it)
    {
        it->doEncode(IOBuff);
    }
    
    return true;
}

GameMsg_C2S_RequestExchangeItem::GameMsg_C2S_RequestExchangeItem(void)
: GameMsg_Base(MSG_C2S_RequestExchangeConfig)
{
}

GameMsg_C2S_RequestExchangeItem::~GameMsg_C2S_RequestExchangeItem()
{

}

bool GameMsg_C2S_RequestExchangeItem::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_C2S_RefreshExchangeItem::GameMsg_C2S_RefreshExchangeItem(void)
: GameMsg_Base(MSG_C2S_RefreshExchangeItem)
{
}

GameMsg_C2S_RefreshExchangeItem::~GameMsg_C2S_RefreshExchangeItem()
{

}

bool GameMsg_C2S_RefreshExchangeItem::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_RequestExchangeItemFail::GameMsg_S2C_RequestExchangeItemFail(void)
: GameMsg_Base(MSG_S2C_RequestExchangeConfigFail)
, m_nError(0)
{
}

GameMsg_S2C_RequestExchangeItemFail::~GameMsg_S2C_RequestExchangeItemFail()
{

}

bool GameMsg_S2C_RequestExchangeItemFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_nError);
    return true;
}



GameMsg_S2C_RequestExchangeItemResult::GameMsg_S2C_RequestExchangeItemResult(void)
: GameMsg_Base(MSG_S2C_RequestExchangeConfigResult)
, m_nSex(0)
{
}

GameMsg_S2C_RequestExchangeItemResult::~GameMsg_S2C_RequestExchangeItemResult()
{

}

bool GameMsg_S2C_RequestExchangeItemResult::doEncode(CParamPool &IOBuff)
{
    if (m_mapExchangeGroup.empty())
    {
        ConfigManager::Instance().GetMagicArrayConfigMgr().doBaseExchangeEncode(m_nSex, IOBuff);
    }
    else
    {
        std::list<MagicArrayExchange> listExchange;
        std::map<unsigned int,unsigned int>::iterator it = m_mapExchangeGroup.begin();
        for (; it != m_mapExchangeGroup.end(); ++it)
        {
            ConfigManager::Instance().GetMagicArrayConfigMgr().GetExchangeInfo(m_nSex, it->first, it->second, listExchange);
        }
        
        IOBuff.AddUInt(ConfigManager::Instance().GetMagicArrayConfigMgr().GetExchangeCostItemType());
        IOBuff.AddUShort((unsigned short)listExchange.size());
        std::list<MagicArrayExchange>::iterator itr = listExchange.begin();
        for (; itr != listExchange.end(); ++itr)
        {
            IOBuff.AddUShort((unsigned short)itr->m_nExchangeID);
            IOBuff.AddInt(itr->m_CostItem.m_nItemCount);
            itr->m_ExchangeItem.doEncode(IOBuff);
        }
    }
    
    return true;
}

GameMsg_C2S_MagicCall::GameMsg_C2S_MagicCall(void)
    : GameMsg_Base(MSG_C2S_MagicCall)
    , m_nCallType(0)
{
}

GameMsg_C2S_MagicCall::~GameMsg_C2S_MagicCall()
{

}

bool GameMsg_C2S_MagicCall::doDecode(CParamPool &IOBuff)
{
	m_nMagicArrayID = (unsigned int)IOBuff.GetUShort();
    m_nCallType = (unsigned short)IOBuff.GetUChar();
    return true;
}

CallReward::CallReward()
:m_bSpecial(false)
{
}

CallReward::~CallReward()
{}

GameMsg_S2C_MagicCallResult::GameMsg_S2C_MagicCallResult(void)
    : GameMsg_Base(MSG_S2C_MagicCallResult)
    , m_nErrorFlag(0)
    , m_nMagicArrayID(0)
    , m_nLeftCDTime(0)
    , m_nFixedItemType(0)
    , m_nFixedItemCount(0)
{
}

GameMsg_S2C_MagicCallResult::~GameMsg_S2C_MagicCallResult()
{

}

bool GameMsg_S2C_MagicCallResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar((unsigned char)m_nErrorFlag);
    IOBuff.AddUShort((unsigned short)m_nMagicArrayID);
    IOBuff.AddFloat((float)m_nLeftCDTime);
    IOBuff.AddUInt(m_nFixedItemType);
    IOBuff.AddInt(m_nFixedItemCount);

    IOBuff.AddUShort((unsigned short)m_listRewardItem.size());
    std::list<CallReward>::iterator it = m_listRewardItem.begin();
    for (; it != m_listRewardItem.end(); ++it)
    {
        IOBuff.AddBool(it->m_bSpecial);
        it->m_RewardItem.doEncode(IOBuff);
    }

    return true;
}

GameMsg_S2C_NoticeCanFreeCall::GameMsg_S2C_NoticeCanFreeCall(void)
: GameMsg_Base(MSG_S2C_NoticeCanFreeCall)
, m_nMagicArrayID(0)
{
}

GameMsg_S2C_NoticeCanFreeCall::~GameMsg_S2C_NoticeCanFreeCall()
{

}

bool GameMsg_S2C_NoticeCanFreeCall::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_nMagicArrayID);
    return true;
}

GameMsg_C2S_MagicArrayExchange::GameMsg_C2S_MagicArrayExchange(void)
: GameMsg_Base(MSG_C2S_RequestExchange)
, m_nExchangeID(0)
{
}

GameMsg_C2S_MagicArrayExchange::~GameMsg_C2S_MagicArrayExchange()
{

}

bool GameMsg_C2S_MagicArrayExchange::doDecode(CParamPool &IOBuff)
{
    m_nExchangeID = (unsigned int)IOBuff.GetUShort();
    return true;
}

GameMsg_S2C_MagicArrayExchangeResult::GameMsg_S2C_MagicArrayExchangeResult(void)
: GameMsg_Base(MSG_S2C_RequestExchangeResult)
    , m_nErrorFlag(0)
{
}

GameMsg_S2C_MagicArrayExchangeResult::~GameMsg_S2C_MagicArrayExchangeResult()
{

}

bool GameMsg_S2C_MagicArrayExchangeResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar((char)m_nErrorFlag);
    return true;
}
