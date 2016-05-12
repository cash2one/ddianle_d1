/*
 * LimitedGoodsInfo.h
 * Description: 限量商品信息
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-8 11:30:57
 */

#ifndef __LIMITEDGOODSINFO_H__
#define __LIMITEDGOODSINFO_H__

#include "GoodsInfo.h"

#define GROUP_BUYING_FILE_NAME      "/data/bytes/GroupBuying.bytes"
#define LIMITED_EDITION_FILE_NAME   "/data/bytes/LimitedEdition.bytes"

//////////////////////////////////////////////////////////////////////////
///限购商品信息
//////////////////////////////////////////////////////////////////////////

class LimitedGoodsInfo
{
public:
    LimitedGoodsInfo();
    ~LimitedGoodsInfo() {}

public:
    bool LoadFromFile(CFileStream &file);
    bool SaveToFile(CFileStream &file);

public:
    unsigned short  m_uSerializeID; // 序列号
    GoodsInfo       m_cGoodsInfo;   // 商品信息
    unsigned char   m_nBuyType;     // 支付类型(1/3/4)
    unsigned int    m_uBuyPrice;    // 售价
    int             m_nCount;       // 销售量/剩余量
    int             m_nLockCount;   // 待支付数量
};

#endif // __LIMITEDGOODSINFO_H__

