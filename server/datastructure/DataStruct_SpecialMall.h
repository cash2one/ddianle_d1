/*
 * DataStruct_SpecialMall.h
 * Description: 特殊商店数据结构
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-10 13:54:08
 */

#ifndef __DATASTRUCT_SPECIALMALL_H__
#define __DATASTRUCT_SPECIALMALL_H__

#include "DataStruct_Base.h"

class CParamPool;

//////////////////////////////////////////////////////////////////////////

/**
* CGroupBuyingItemData:
* 团购物品数据
*/

class CGroupBuyingItemData
{
public:
    CGroupBuyingItemData();
    ~CGroupBuyingItemData() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;             // 索引
    CItem           m_cItem;            // 物品
    unsigned int    m_uPrice;           // 市场价
    unsigned char   m_nPriceType;       // 市场价类型
    unsigned int    m_uDiscount;        // 折扣价
    unsigned char   m_nDiscountType;    // 支付类型
    int             m_nCustomCount;     // 购买人数
    bool            m_bAlreadyBuy;      // 是否购买永久服饰
};

//////////////////////////////////////////////////////////////////////////

/**
* CLimitedEditionItemData:
* 限量销售物品数据
*/

class CLimitedEditionItemData
{
public:
    CLimitedEditionItemData();
    ~CLimitedEditionItemData() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;             // 索引
    CItem           m_cItem;            // 物品
    unsigned int    m_uPrice;           // 市场价
    unsigned char   m_nPriceType;       // 市场价类型
    unsigned int    m_uDiscount;        // 折扣价
    unsigned char   m_nDiscountType;    // 支付类型
    int             m_nRemainder;       // 剩余数量
    bool            m_bAlreadyBuy;      // 是否购买永久服饰
};

//////////////////////////////////////////////////////////////////////////

/**
* CAuctionItemData:
* 竞拍行物品数据
*/

class CAuctionItemData
{
public:
    CAuctionItemData();
    ~CAuctionItemData() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;             // 索引
    CItem           m_cItem;            // 物品
    unsigned int    m_uCurPrice;        // 当前价
    unsigned int    m_uRoleID;          // 出价人
    std::string     m_strRoleName;      // 出价人姓名
    unsigned int    m_uMinAdd;          // 最小加价
    float           m_fCountDown;       // 倒计时
    bool            m_bAlreayTop;       // 是否最高价
    bool            m_bAlreadyBuy;      // 是否购买永久服饰
};

//////////////////////////////////////////////////////////////////////////

class CAuctionTopPriceData
{
public:
    CAuctionTopPriceData();
    ~CAuctionTopPriceData() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    long long           m_lLogID;
    unsigned short      m_uIdx;
    unsigned int        m_tEndTime;
};

#endif // __DATASTRUCT_SPECIALMALL_H__

