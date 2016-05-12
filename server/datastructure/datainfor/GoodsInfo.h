/*
 * GoodsInfo.h
 * Description: 特殊商店商品信息
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-5 17:14:50
 */

#ifndef __GOODSINFO_H__
#define __GOODSINFO_H__
#include "../DataStruct_Base.h"

class CItem;
class CFileStream;

//////////////////////////////////////////////////////////////////////////
///商品信息数据结构
//////////////////////////////////////////////////////////////////////////

class GoodsInfo
{
public:
    GoodsInfo();
    ~GoodsInfo();

public:
    bool LoadFromFile(CFileStream &file);

    bool SaveToFile(CFileStream &file);

    void CopyInfoToCItem(CItem *pItem) const;

public:
    unsigned short      m_uIdx;         // 序号
    itemtype_t          m_uGoodsID;     // 商品ID
    unsigned short      m_uNum;         // 商品数量
    int                 m_nValidTime;   // 商品期限
    unsigned char       m_nSellType;    // 市场价类型(1/3/4)
    unsigned int        m_uSellPrice;   // 市场价
};

#endif // __GOODSINFO_H__

