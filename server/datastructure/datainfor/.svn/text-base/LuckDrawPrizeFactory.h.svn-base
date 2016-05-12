/*
 * LuckDrawPrizeFactory.h
 * Description: 欢乐转奖品工厂类 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-4-10 14:51:54
 */

#ifndef __LUCKDRAWPRIZEFACTORY_H__
#define __LUCKDRAWPRIZEFACTORY_H__

class CLuckDrawPrizeBase;

/**
* CLuckDrawPrizeFactory:
* 
*/

class CLuckDrawPrizeFactory
{
protected:
    CLuckDrawPrizeFactory() {}
    CLuckDrawPrizeFactory(const CLuckDrawPrizeFactory &) {}
    virtual ~CLuckDrawPrizeFactory() {}

public:
    static CLuckDrawPrizeBase* CreateLuckDrawPrizeByType(unsigned char nType);
    static CLuckDrawPrizeBase* CreateLuckDrawPrizeByInstance(const CLuckDrawPrizeBase *pInstance);
};

#endif // __LUCKDRAWPRIZEFACTORY_H__

