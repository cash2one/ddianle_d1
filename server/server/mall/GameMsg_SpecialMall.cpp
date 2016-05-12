#include "GameMsg_SpecialMall.h"
#include "../share/ServerMsgDef.h"

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_SpecialMall_State::GameMsg_C2S_SpecialMall_State()
    : GameMsg_Base(MSG_C2S_SpecialMall_State)
{
}

bool GameMsg_C2S_SpecialMall_State::doDecode(CParamPool &)
{
    return true;
}

bool GameMsg_C2S_SpecialMall_State::doEncode(CParamPool &)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_SpecialMall_StateResult::GameMsg_S2C_SpecialMall_StateResult()
    : GameMsg_Base(MSG_S2C_SpecialMall_StateResult)
{
    m_nGroupBuyingState = 0;
    m_nLimitedEditionState = 0;
    m_nAuctionState = 0;
}

bool GameMsg_S2C_SpecialMall_StateResult::doDecode(CParamPool &IOBuff)
{
    m_nGroupBuyingState = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strGroupBuyingTitle);

    m_nLimitedEditionState = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strLimitedEditionTitle);

    m_nAuctionState = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strAuctionTitle);
    IOBuff.GetStringW(m_strAuctionRule);

    return true;
}

bool GameMsg_S2C_SpecialMall_StateResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nGroupBuyingState);
    IOBuff.AddStringW(m_strGroupBuyingTitle);

    IOBuff.AddUChar(m_nLimitedEditionState);
    IOBuff.AddStringW(m_strLimitedEditionTitle);

    IOBuff.AddUChar(m_nAuctionState);
    IOBuff.AddStringW(m_strAuctionTitle);
    IOBuff.AddStringW(m_strAuctionRule);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GroupBuying_Items::GameMsg_C2S_GroupBuying_Items()
    : GameMsg_Base(MSG_C2S_GroupBuying_Items)
{
    m_bSearch = false;
    m_uNextPage = 0;
    m_uCount = 1;
}

bool GameMsg_C2S_GroupBuying_Items::doDecode(CParamPool &IOBuff)
{
    m_bSearch = IOBuff.GetBool();
    IOBuff.GetStringW(m_strSearchKey);
    m_uNextPage = IOBuff.GetUShort();
    m_uCount = IOBuff.GetUShort();

    return true;
}

bool GameMsg_C2S_GroupBuying_Items::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bSearch);
    IOBuff.AddStringW(m_strSearchKey);
    IOBuff.AddUShort(m_uNextPage);
    IOBuff.AddUShort(m_uCount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_GroupBuying_ItemsResult::GameMsg_S2C_GroupBuying_ItemsResult()
    : GameMsg_Base(MSG_S2C_GroupBuying_ItemsResult)
{
    m_nState = 0;
    m_fCD = 0;
    m_bSearch = false;
    m_uCurPage = 0;
    m_uTotalCount = 0;
}

bool GameMsg_S2C_GroupBuying_ItemsResult::doDecode(CParamPool &IOBuff)
{
    m_nState = IOBuff.GetUChar();
    m_fCD = IOBuff.GetFloat();
    m_bSearch = IOBuff.GetBool();
    IOBuff.GetStringW(m_strSearchKey);
    m_uCurPage = IOBuff.GetUShort();
    m_uTotalCount = IOBuff.GetUShort();

    unsigned short size = IOBuff.GetUShort();
    for (unsigned short i = 0; i < size; ++i)
    {
        CGroupBuyingItemData data;
        data.doDecode(IOBuff);

        m_listItems.push_back(data);
    }

    return true;
}

bool GameMsg_S2C_GroupBuying_ItemsResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nState);
    IOBuff.AddFloat(m_fCD);
    IOBuff.AddBool(m_bSearch);
    IOBuff.AddStringW(m_strSearchKey);
    IOBuff.AddUShort(m_uCurPage);
    IOBuff.AddUShort(m_uTotalCount);

    IOBuff.AddUShort((unsigned short)m_listItems.size());
    for (std::list<CGroupBuyingItemData>::iterator bIt = m_listItems.begin(), eIt = m_listItems.end(); 
         bIt != eIt; ++bIt)
         bIt->doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GroupBuying_Buy::GameMsg_C2S_GroupBuying_Buy()
    : GameMsg_Base(MSG_C2S_GroupBuying_Buy)
{
    m_uIdx = 0;
    m_uGoodsID = 0;
}

bool GameMsg_C2S_GroupBuying_Buy::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_uGoodsID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_C2S_GroupBuying_Buy::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    IOBuff.AddUInt(m_uGoodsID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_GroupBuying_BuyResult::GameMsg_S2C_GroupBuying_BuyResult()
    : GameMsg_Base(MSG_S2C_GroupBuying_BuyResult)
{
    m_nErrorType = 0;
}

bool GameMsg_S2C_GroupBuying_BuyResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorType = IOBuff.GetUChar();
    m_cItem.doDecode(IOBuff);

    return true;
}

bool GameMsg_S2C_GroupBuying_BuyResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);
    m_cItem.doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_LimitedEdition_Items::GameMsg_C2S_LimitedEdition_Items()
    : GameMsg_Base(MSG_C2S_LimitedEdition_Items)
{
    m_bSearch = false;
    m_uNextPage = 0;
    m_uCount = 1;
}

bool GameMsg_C2S_LimitedEdition_Items::doDecode(CParamPool &IOBuff)
{
    m_bSearch = IOBuff.GetBool();
    IOBuff.GetStringW(m_strSearchKey);
    m_uNextPage = IOBuff.GetUShort();
    m_uCount = IOBuff.GetUShort();

    return true;
}

bool GameMsg_C2S_LimitedEdition_Items::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bSearch);
    IOBuff.AddStringW(m_strSearchKey);
    IOBuff.AddUShort(m_uNextPage);
    IOBuff.AddUShort(m_uCount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_LimitedEdition_ItemsResult::GameMsg_S2C_LimitedEdition_ItemsResult()
    : GameMsg_Base(MSG_S2C_LimitedEdition_ItemsResult)
{
    m_nState = 0;
    m_fCD = 0;
    m_bSearch = false;
    m_uCurPage = 0;
    m_uTotalCount = 0;
}

bool GameMsg_S2C_LimitedEdition_ItemsResult::doDecode(CParamPool &IOBuff)
{
    m_nState = IOBuff.GetUChar();
    m_fCD = IOBuff.GetFloat();
    m_bSearch = IOBuff.GetBool();
    IOBuff.GetStringW(m_strSearchKey);
    m_uCurPage = IOBuff.GetUShort();
    m_uTotalCount = IOBuff.GetUShort();

    unsigned short size = IOBuff.GetUShort();
    for (unsigned short i = 0; i < size; ++i)
    {
        CLimitedEditionItemData data;
        data.doDecode(IOBuff);

        m_listItems.push_back(data);
    }

    return true;
}

bool GameMsg_S2C_LimitedEdition_ItemsResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nState);
    IOBuff.AddFloat(m_fCD);
    IOBuff.AddBool(m_bSearch);
    IOBuff.AddStringW(m_strSearchKey);
    IOBuff.AddUShort(m_uCurPage);
    IOBuff.AddUShort(m_uTotalCount);

    IOBuff.AddUShort((unsigned short)m_listItems.size());
    for (std::list<CLimitedEditionItemData>::iterator bIt = m_listItems.begin(), eIt = m_listItems.end(); 
        bIt != eIt; ++bIt)
        bIt->doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_LimitedEdition_Buy::GameMsg_C2S_LimitedEdition_Buy()
    : GameMsg_Base(MSG_C2S_LimitedEdition_Buy)
{
    m_uIdx = 0;
    m_uGoodsID = 0;
}

bool GameMsg_C2S_LimitedEdition_Buy::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_uGoodsID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_C2S_LimitedEdition_Buy::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    IOBuff.AddUInt(m_uGoodsID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_LimitedEdition_BuyResult::GameMsg_S2C_LimitedEdition_BuyResult()
    : GameMsg_Base(MSG_S2C_LimitedEdition_BuyResult)
{
    m_nErrorType = 0;
}

bool GameMsg_S2C_LimitedEdition_BuyResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorType = IOBuff.GetUChar();
    m_cItem.doDecode(IOBuff);

    return true;
}

bool GameMsg_S2C_LimitedEdition_BuyResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);
    m_cItem.doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_Auction_Items::GameMsg_C2S_Auction_Items()
    : GameMsg_Base(MSG_C2S_Auction_Items)
{
    m_bSearch = false;
    m_uNextPage = 0;
    m_uCount = 1;
}

bool GameMsg_C2S_Auction_Items::doDecode(CParamPool &IOBuff)
{
    m_bSearch = IOBuff.GetBool();
    IOBuff.GetStringW(m_strSearchKey);
    m_uNextPage = IOBuff.GetUShort();
    m_uCount = IOBuff.GetUShort();

    return true;
}

bool GameMsg_C2S_Auction_Items::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bSearch);
    IOBuff.AddStringW(m_strSearchKey);
    IOBuff.AddUShort(m_uNextPage);
    IOBuff.AddUShort(m_uCount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_Auction_ItemsResult::GameMsg_S2C_Auction_ItemsResult()
    : GameMsg_Base(MSG_S2C_Auction_ItemsResult)
{
    m_nState = 0;
    m_fCD = 0;
    m_bSearch = false;
    m_uCurPage = 0;
    m_uTotalCount = 0;
}

bool GameMsg_S2C_Auction_ItemsResult::doDecode(CParamPool &IOBuff)
{
    m_nState = IOBuff.GetUChar();
    m_fCD = IOBuff.GetFloat();
    m_bSearch = IOBuff.GetBool();
    IOBuff.GetStringW(m_strSearchKey);
    m_uCurPage = IOBuff.GetUShort();
    m_uTotalCount = IOBuff.GetUShort();

    unsigned short size = IOBuff.GetUShort();
    for (unsigned short i = 0; i < size; ++i)
    {
        CAuctionItemData data;
        data.doDecode(IOBuff);

        m_listItems.push_back(data);
    }

    return true;
}

bool GameMsg_S2C_Auction_ItemsResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nState);
    IOBuff.AddFloat(m_fCD);
    IOBuff.AddBool(m_bSearch);
    IOBuff.AddStringW(m_strSearchKey);
    IOBuff.AddUShort(m_uCurPage);
    IOBuff.AddUShort(m_uTotalCount);

    IOBuff.AddUShort((unsigned short)m_listItems.size());
    for (std::list<CAuctionItemData>::iterator bIt = m_listItems.begin(), eIt = m_listItems.end(); 
        bIt != eIt; ++bIt)
        bIt->doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_Auction_Bid::GameMsg_C2S_Auction_Bid()
    : GameMsg_Base(MSG_C2S_Auction_Bid)
{
    m_uIdx = 0;
    m_uGoodsID = 0;
    m_uPrice = 0;
}

bool GameMsg_C2S_Auction_Bid::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_uGoodsID = IOBuff.GetUInt();
    m_uPrice = IOBuff.GetUInt();

    return true;
}

bool GameMsg_C2S_Auction_Bid::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    IOBuff.AddUInt(m_uGoodsID);
    IOBuff.AddUInt(m_uPrice);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_Auction_BidResult::GameMsg_S2C_Auction_BidResult()
    : GameMsg_Base(MSG_S2C_Auction_BidResult)
{
    m_uIdx = 0;
    m_nErrorType = 0;
}

bool GameMsg_S2C_Auction_BidResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorType = IOBuff.GetUChar();
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);

    return true;
}

bool GameMsg_S2C_Auction_BidResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_Auction_Surpassed::GameMsg_S2C_Auction_Surpassed()
    : GameMsg_Base(MSG_S2C_Auction_Surpassed)
{
    m_uIdx = 0;
}

bool GameMsg_S2C_Auction_Surpassed::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);

    return true;
}

bool GameMsg_S2C_Auction_Surpassed::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_Auction_BidSuccess::GameMsg_S2C_Auction_BidSuccess()
    : GameMsg_Base(MSG_S2C_Auction_BidSuccess)
{
}

bool GameMsg_S2C_Auction_BidSuccess::doDecode(CParamPool &IOBuff)
{
    m_cItem.doDecode(IOBuff);

    return true;
}

bool GameMsg_S2C_Auction_BidSuccess::doEncode(CParamPool &IOBuff)
{
    m_cItem.doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_SpecialMall_Pay::GameMsg_G2S_SpecialMall_Pay()
    : GameMsg_Base(MSG_G2S_SpecialMall_Pay)
{
    m_nSpecialMallType = ESpecialMallType_None;
    m_uRange = 1;
    m_uIdx = 0;
    m_uBuyPrice = 0;
    m_nBuyType = 1;
    m_tEndTime = 0;
}

bool GameMsg_G2S_SpecialMall_Pay::doDecode(CParamPool &IOBuff)
{
    m_nSpecialMallType = IOBuff.GetUChar();
    m_uRange = IOBuff.GetUShort();
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);
    m_uBuyPrice = IOBuff.GetUInt();
    m_nBuyType = IOBuff.GetUChar();
    m_tEndTime = (time_t)IOBuff.GetUInt();
    IOBuff.GetStringW(m_strMailTitle);
    IOBuff.GetStringW(m_strMailContent);

    return true;
}

bool GameMsg_G2S_SpecialMall_Pay::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nSpecialMallType);
    IOBuff.AddUShort(m_uRange);
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);
    IOBuff.AddUInt(m_uBuyPrice);
    IOBuff.AddUChar(m_nBuyType);
    IOBuff.AddUInt((unsigned int)m_tEndTime);
    IOBuff.AddStringW(m_strMailTitle);
    IOBuff.AddStringW(m_strMailContent);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_SpecialMall_PayResult::GameMsg_S2G_SpecialMall_PayResult()
    : GameMsg_Base(MSG_S2G_SpecialMall_PayResult)
{
    m_nSpecialMallType = ESpecialMallType_None;
    m_uRange = 1;
    m_uIdx = 0;
    m_bSuccess = false;
}

bool GameMsg_S2G_SpecialMall_PayResult::doDecode(CParamPool &IOBuff)
{
    m_nSpecialMallType = IOBuff.GetUChar();
    m_uRange = IOBuff.GetUShort();
    m_uIdx = IOBuff.GetUShort();
    m_bSuccess = IOBuff.GetBool();

    return true;
}

bool GameMsg_S2G_SpecialMall_PayResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nSpecialMallType);
    IOBuff.AddUShort(m_uRange);
    IOBuff.AddUShort(m_uIdx);
    IOBuff.AddBool(m_bSuccess);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_Auction_UpdateState::GameMsg_G2S_Auction_UpdateState()
    : GameMsg_Base(MSG_G2S_Auction_UpdateState)
{
    m_uIdx = 0;
    m_uCurPrice = 0;
    m_bTimeOut = false;
}

bool GameMsg_G2S_Auction_UpdateState::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_uCurPrice = IOBuff.GetUInt();
    m_bTimeOut = IOBuff.GetBool();

    return true;
}

bool GameMsg_G2S_Auction_UpdateState::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    IOBuff.AddUInt(m_uCurPrice);
    IOBuff.AddBool(m_bTimeOut);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_Auction_BidSuccess::GameMsg_G2S_Auction_BidSuccess()
    : GameMsg_Base(MSG_G2S_Auction_BidSuccess)
{
    m_uIdx = 0;
    m_uPrice = 0;
}

bool GameMsg_G2S_Auction_BidSuccess::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);
    m_uPrice = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_Auction_BidSuccess::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);
    IOBuff.AddUInt(m_uPrice);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_Auction_BidResult::GameMsg_G2S_Auction_BidResult()
    : GameMsg_Base(MSG_G2S_Auction_BidResult)
{
    m_nErrorType = 0;
    m_uIdx = 0;
    m_uPrice = 0;
}

bool GameMsg_G2S_Auction_BidResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorType = IOBuff.GetUChar();
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);
    m_uPrice = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_Auction_BidResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);
    IOBuff.AddUInt(m_uPrice);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_Auction_Surpassed::GameMsg_G2S_Auction_Surpassed()
    : GameMsg_Base(MSG_G2S_Auction_Surpassed)
{
    m_uIdx = 0;
    m_uPrice = 0;
}

bool GameMsg_G2S_Auction_Surpassed::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);
    m_uPrice = IOBuff.GetUInt();

    return true;
}

bool GameMsg_G2S_Auction_Surpassed::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);
    IOBuff.AddUInt(m_uPrice);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_Auction_ItemsResult::GameMsg_G2S_Auction_ItemsResult()
    : GameMsg_Base(MSG_G2S_Auction_ItemsResult)
{
    m_nState = 0;
    m_fCD = 0;
    m_bSearch = false;
    m_uCurPage = 0;
    m_uTotalCount = 0;
    m_tEndTime = 0;
}

bool GameMsg_G2S_Auction_ItemsResult::doDecode(CParamPool &IOBuff)
{
    m_nState = IOBuff.GetUChar();
    m_fCD = IOBuff.GetFloat();
    m_bSearch = IOBuff.GetBool();
    IOBuff.GetStringW(m_strSearchKey);
    m_uCurPage = IOBuff.GetUShort();
    m_uTotalCount = IOBuff.GetUShort();
    m_tEndTime = IOBuff.GetUInt();

    unsigned short size = IOBuff.GetUShort();
    for (unsigned short i = 0; i < size; ++i)
    {
        CAuctionItemData data;
        data.doDecode(IOBuff);

        m_listItems.push_back(data);
    }

    return true;
}

bool GameMsg_G2S_Auction_ItemsResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nState);
    IOBuff.AddFloat(m_fCD);
    IOBuff.AddBool(m_bSearch);
    IOBuff.AddStringW(m_strSearchKey);
    IOBuff.AddUShort(m_uCurPage);
    IOBuff.AddUShort(m_uTotalCount);
    IOBuff.AddUInt(m_tEndTime);

    IOBuff.AddUShort((unsigned short)m_listItems.size());
    for (std::list<CAuctionItemData>::iterator bIt = m_listItems.begin(), eIt = m_listItems.end(); 
        bIt != eIt; ++bIt)
        bIt->doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_Auction_CheckTopPrice::GameMsg_S2G_Auction_CheckTopPrice()
    : GameMsg_Base(MSG_S2G_Auction_CheckTopPrice)
{
}

bool GameMsg_S2G_Auction_CheckTopPrice::doDecode(CParamPool &IOBuff)
{
    unsigned short size = IOBuff.GetUShort();

    for (unsigned short i = 0; i < size; ++i)
    {
        CAuctionTopPriceData topData;
        topData.doDecode(IOBuff);

        m_mapTopInfo.insert(std::make_pair(topData.m_lLogID, topData));
    }

    return true;
}

bool GameMsg_S2G_Auction_CheckTopPrice::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_mapTopInfo.size());
    for (std::map<long long, CAuctionTopPriceData>::iterator itr = m_mapTopInfo.begin(); 
         itr != m_mapTopInfo.end(); ++itr)
    {
        itr->second.doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_Auction_CheckTopPriceResult::GameMsg_G2S_Auction_CheckTopPriceResult()
    : GameMsg_Base(MSG_G2S_Auction_CheckTopPriceResult)
{
}

bool GameMsg_G2S_Auction_CheckTopPriceResult::doDecode(CParamPool &IOBuff)
{
    unsigned short size = IOBuff.GetUShort();
    long long logID;
    bool bTop;

    for (unsigned short i = 0; i < size; ++i)
    {
        logID = IOBuff.GetInt64();
        bTop = IOBuff.GetBool();

        m_mapTopInfo.insert(std::make_pair(logID, bTop));
    }

    return true;
}

bool GameMsg_G2S_Auction_CheckTopPriceResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_mapTopInfo.size());
    for (std::map<long long, bool>::const_iterator itr = m_mapTopInfo.begin(); 
         itr != m_mapTopInfo.end(); ++itr)
    {
        IOBuff.AddInt64(itr->first);
        IOBuff.AddBool(itr->second);
    }

    return true;
}

