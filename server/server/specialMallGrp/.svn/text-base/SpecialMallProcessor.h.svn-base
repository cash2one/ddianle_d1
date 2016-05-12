/*
 * SpecialMallProcessor.h
 * Description: 特殊商店
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-8 14:54:03
 */

#ifndef __SPECIALMALLPROCESSOR_H__
#define __SPECIALMALLPROCESSOR_H__

#include <map>
#include <set>
#include "Auction.h"
#include "LimitedSales.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/Macro_Define.h"

class CItem;
class CGroupRoleEntity;

/**
* SpecialMallProcessor:
* 
*/

class CSpecialMallProcessor : public GameMsg_Processor
{
protected:
    CSpecialMallProcessor();
    virtual ~CSpecialMallProcessor();

public:
    static CSpecialMallProcessor& Instance();
    void Initialize();

    void OnUpdate(int nTimeElapse);

    void Clear();

    void SendItem(unsigned short idx, unsigned int roleID, const CItem &item, unsigned int price, bool bLastRange);
    void ClearBidPeople(std::set<unsigned short> &setIdxs);
    void AuctionClose();

    void QueryGroupBuyingInfo();
    void QueryLimitedEditionInfo();
    void QueryAuctionInfo();

public:
    void InitLuaInfo(ESpecialMallType type, const CSpecialMallSettings *pSettings);
    void AddTimeRangeInfo(ESpecialMallType type, time_t begin, time_t end, std::string &rTitle);

private:
    void RegisterMessage();

    void LoadAllData();

    void AddBidPeople(unsigned short idx, unsigned int roleID);

private:
    void OnGetSpecialMallState(GameMsg_Base &rMsg, CSlotPeer &rSlot);

    void OnGetGroupBuyingItems(GameMsg_Base &rMsg, CSlotPeer &rSlot);
    void OnGroupBuyingBuy(GameMsg_Base &rMsg, CSlotPeer &rSlot);

    void OnGetLimitedEditionItems(GameMsg_Base &rMsg, CSlotPeer &rSlot);
    void OnLimitedEditionBuy(GameMsg_Base &rMsg, CSlotPeer &rSlot);

    void OnGetAuctionItems(GameMsg_Base &rMsg, CSlotPeer &rSlot);
    void OnAuctionBid(GameMsg_Base &rMsg, CSlotPeer &rSlot);

    void OnSpecialMallPayResult(GameMsg_Base &rMsg, CSlotPeer &rSlot);
    void OnAuctionCheckTopPrice(GameMsg_Base &rMsg, CSlotPeer &rSlot);

    void OnGMGroupBuyControl(GameMsg_Base &rMsg, CSlotPeer &rSlot);

    void OnGetGroupBuyingCount(QueryBase &query);
    void OnGetLimitedEditionCount(QueryBase &query);
    void OnGetAuctionBidInfo(QueryBase &query);
    void OnDBSetAuctionStateSuccess(QueryBase &query);

    void SendBuyRes(CGroupRoleEntity *pEntity, unsigned char nSpecialMallType, unsigned char errorType, const CItem &item);

    void SendAuctionSurpassed(unsigned short idx, unsigned int roleID, unsigned int price, CItem &item);
    void SendAuctionUpdateState(unsigned short idx, unsigned int price, bool bTimeOut = false);

private:
    // 团购
    CLimitedSales       m_cGroupBuying;
    // 限量销售
    CLimitedSales       m_cLimitedEdition;
    // 竞拍行
    CAuction            m_cAuction;

    int                 m_nTimeElapsed;
    int                 m_nAddTimeElapsed;
    int                 m_nSaveCountTimeSlapsed;

    std::map<unsigned short, std::set<unsigned int> > m_mapBidPeople;
};

#endif // __SPECIALMALLPROCESSOR_H__

