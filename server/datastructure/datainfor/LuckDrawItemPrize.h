/*
 * LuckDrawItemPrize.h
 * Description: 欢乐转物品奖品类 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-4-10 10:37:02
 */

#ifndef __LUCKDRAWITEMPRIZE_H__
#define __LUCKDRAWITEMPRIZE_H__

#include "LuckDrawPrizeBase.h"
#include "../DataStruct_Base.h"

/**
* CLuckDrawItemPrize:
* 
*/

class CLuckDrawItemPrize : public CLuckDrawPrizeBase
{
public:
    CLuckDrawItemPrize();
    CLuckDrawItemPrize(const CLuckDrawItemPrize &ref);
    virtual ~CLuckDrawItemPrize() {}

public:
    itemtype_t GetItemType() const { return m_nItemType; }
    unsigned short GetItemCount() const { return m_nItemCount; }
    int GetValidTime() const { return m_nValidTime; }

    void SetItemType(itemtype_t nType) { m_nItemType = nType; }
    void SetItemCount(unsigned short nCount) { m_nItemCount = nCount; }
    void SetValidTime(int nValidTime) { m_nValidTime = nValidTime; }

public:
    virtual void LoadFromFile(CFileStream &file);
    virtual void doEncodeToClient(CParamPool &IOBuff) const;
    virtual void doDecodeFromClient(CParamPool &IOBuff);

    virtual void doEncodeToServer(CParamPool &IOBuff) const;
    virtual void doDecodeFromServer(CParamPool &IOBuff);

private:
    void doEncodeSelf(CParamPool &IOBuff) const;
    void doDecodeSelf(CParamPool &IOBuff);

private:
    itemtype_t      m_nItemType;    // 物品类型
    unsigned short  m_nItemCount;   // 物品数量
    int             m_nValidTime;   // 有效期
};

#endif // __LUCKDRAWITEMPRIZE_H__

