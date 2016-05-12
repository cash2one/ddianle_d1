/*
 * Auction.h
 * Description: ¾ºÅÄÐÐÀà
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-17 15:20:58
 */

#ifndef __AUCTION_H__
#define __AUCTION_H__

#include <map>
#include "TimeMgr.h"
#include "SpecialMallSettings.h"

//////////////////////////////////////////////////////////////////////////

class CAuctionSettings : public CSpecialMallSettings
{
public:
    CAuctionSettings() {}
    CAuctionSettings(const CAuctionSettings &rSettings);
    ~CAuctionSettings() {}

    void operator = (const CAuctionSettings &rSettings);

public:
    std::string     m_strRule;
};

//////////////////////////////////////////////////////////////////////////

class CItem;
class AuctionGoodsInfo;
class CGetAuctionBidInfo;
class GameMsg_C2S_Auction_Items;
class GameMsg_G2S_Auction_BidResult;
class GameMsg_G2S_Auction_ItemsResult;

/**
* Auction:
* 
*/

class CAuction : public CTimeMgr
{
public:
    CAuction();
    virtual ~CAuction();

public:
    bool LoadGoodsInfo(const std::string &strGlobalPath);

    void Clear();

    void SetSettings(const CSpecialMallSettings *pSettings);

    void OnUpdateWithInterval(time_t tTime, unsigned int interval);

    bool IsOpening();

    void GetState(unsigned char *pState, std::string *pTitle, std::string *pRule);
    void GetMailInfo(std::string *pMailTitle, std::string *pMailContent);

    void GetItems(unsigned int nRoleID, unsigned char nSex, const GameMsg_C2S_Auction_Items *pInMsg, GameMsg_G2S_Auction_ItemsResult *pResMsg);

    unsigned char BidItem(unsigned int roleID, const char *pName, GameMsg_G2S_Auction_BidResult &rResMsg, unsigned int &rOldRoleID, unsigned int &rOldPrice);

    void UpdateBidInfo(std::map<unsigned short, CGetAuctionBidInfo> *pInfo);

    bool CheckTopPrice(unsigned int nRoleID, unsigned short nIdx, unsigned int nEndTime);

private:
    void AddGoodsInfo(AuctionGoodsInfo *pGoodsInfo);

    bool IsAllItemTimeOut() const;

protected:
    virtual void SetOpening(bool bOpening);

private:
    std::map<unsigned short, std::map<unsigned short, AuctionGoodsInfo *> > m_mapGoodsInfo;

    CAuctionSettings    m_cSettings;

    bool                m_bFirstQuery;
};

#endif // __AUCTION_H__

