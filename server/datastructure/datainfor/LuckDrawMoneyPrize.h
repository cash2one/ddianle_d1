/*
 * LuckDrawMoneyPrize.h
 * Description: 欢乐转金钱奖品类 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-4-10 10:37:52
 */

#ifndef __LUCKDRAWMONEYPRIZE_H__
#define __LUCKDRAWMONEYPRIZE_H__

#include "LuckDrawPrizeBase.h"

/**
* CLuckDrawMoneyPrize:
* 
*/

class CLuckDrawMoneyPrize : public CLuckDrawPrizeBase
{
public:
    CLuckDrawMoneyPrize();
    CLuckDrawMoneyPrize(const CLuckDrawMoneyPrize &ref);
    virtual ~CLuckDrawMoneyPrize() {}

public:
    unsigned char GetCurrencyType() const { return m_nCurrencyType; }
    unsigned int GetAmount() const { return m_nAmount; }

    void SetCurrencyType(unsigned char nCurrencyType) { m_nCurrencyType = nCurrencyType; }
    void SetAmount(int nAmount) { m_nAmount = nAmount; }

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
    unsigned char   m_nCurrencyType;    // 货币类型
    unsigned int    m_nAmount;          // 总量
};

#endif // __LUCKDRAWMONEYPRIZE_H__

