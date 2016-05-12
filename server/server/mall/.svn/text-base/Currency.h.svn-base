/*
 * Currency.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-4 14:31:14
 */

#ifndef __CURRENCY_H__
#define __CURRENCY_H__

#include <map>

/**
* Currency:
* 
*/

class CParamPool;

class Currency
{
public:
    Currency();
    virtual ~Currency();

public:
    void SetType(unsigned char nType);
    unsigned char GetType() const { return m_nType; }

    void SetAmount(unsigned int nAmount);
    unsigned int GetAmount() const { return m_nAmount; }

    bool IsValid() const;

    void AddIntoCurrencyMap(std::map<unsigned char, unsigned int> &rMapBills, bool bInit, double nRatio = 1) const;

    void doEncode(CParamPool &IOBuff);

private:
    unsigned char   m_nType;
    unsigned int    m_nAmount;
};

#endif // __CURRENCY_H__

