/*
 * AuctionGoodsInfo.h
 * Description: 竞拍商品信息
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-17 13:47:16
 */

#ifndef __AUCTIONGOODSINFO_H__
#define __AUCTIONGOODSINFO_H__

#include <string>
#include "GoodsInfo.h"

#define AUCTION_FILE_NAME   "/data/bytes/Auction.bytes"

//////////////////////////////////////////////////////////////////////////
///拍卖商品信息
//////////////////////////////////////////////////////////////////////////

class AuctionGoodsInfo
{
public:
    AuctionGoodsInfo();
    virtual ~AuctionGoodsInfo();

public:
    bool LoadFromFile(CFileStream &file);
    bool SaveToFile(CFileStream &file);

public:
    unsigned short  m_uSerializeID; // 序列号
    GoodsInfo       m_cGoodsInfo;   // 商品信息
    unsigned short  m_uAddPercent;  // 加价百分比
    unsigned int    m_uMinAdd;      // 最小加价
    float           m_fCountDown;   // 倒计时
    unsigned int    m_uCurPrice;    // 当前价
    unsigned int    m_uRoleID;      // 出价人
    std::string     m_strRoleName;  // 出价人名字
};

#endif // __AUCTIONGOODSINFO_H__
