/*
 * LimitedSales.h
 * Description: 限量销售类
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-8 15:12:31
 */

#ifndef __LIMITEDSALES_H__
#define __LIMITEDSALES_H__

#include <map>
#include <list>
#include "TimeMgr.h"
#include "SpecialMallSettings.h"

enum ELimitedSalesType
{
    ELimitedSalesType_Start = 0,
    ELimitedSalesType_Group,
    ELimitedSalesType_Edition,
    ELimitedSalesType_End
};

//////////////////////////////////////////////////////////////////////////

class CLimitedSalesSettings : public CSpecialMallSettings
{
public:
    CLimitedSalesSettings();
    CLimitedSalesSettings(const CLimitedSalesSettings &rSettings);
    ~CLimitedSalesSettings() {}

    void operator = (const CLimitedSalesSettings &rSettings);

public:
    unsigned int    m_uAdditionLower;
    unsigned int    m_uAdditionUpper;
};

//////////////////////////////////////////////////////////////////////////

class CLimitedSalesGMSettings
{
public:
    CLimitedSalesGMSettings();
    CLimitedSalesGMSettings(const CLimitedSalesGMSettings &rSettings);
    ~CLimitedSalesGMSettings() {}

    void operator = (const CLimitedSalesGMSettings &rSettings);

public:
    unsigned int    m_uStartTime;
    unsigned int    m_uEndTime;
    unsigned int    m_uInterval;
    unsigned int    m_uAdditionLower;
    unsigned int    m_uAdditionUpper;
};

//////////////////////////////////////////////////////////////////////////

class CItem;
class LimitedGoodsInfo;
class CSaveGroupBuyingCount;
class CGetLimitedSalesCount;
class GameMsg_C2S_GroupBuying_Items;
class GameMsg_S2C_GroupBuying_ItemsResult;
class GameMsg_C2S_LimitedEdition_Items;
class GameMsg_S2C_LimitedEdition_ItemsResult;
class GameMsg_G2S_SpecialMall_Pay;

/**
* CLimitedSales:
* 
*/

class CLimitedSales : public CTimeMgr
{
public:
    CLimitedSales();
    virtual ~CLimitedSales();

public:
    bool LoadGoodsInfo(const std::string &strGlobalPath, ELimitedSalesType type);

    void Clear();

    void SetSettings(const CSpecialMallSettings *pSettings);
    void SetGMSettings(const CLimitedSalesGMSettings *pSettings);

    void GetState(unsigned char *pState, std::string *pTitle);

    void GetGroupBuyingItems(unsigned char nSex, const GameMsg_C2S_GroupBuying_Items *pInMsg, GameMsg_S2C_GroupBuying_ItemsResult *pResMsg);
    void GetLimitedEditionItems(unsigned char nSex, const GameMsg_C2S_LimitedEdition_Items *pInMsg, GameMsg_S2C_LimitedEdition_ItemsResult *pResMsg);

    unsigned char BuyItem(GameMsg_G2S_SpecialMall_Pay &rPayMsg);
    void PayResult(unsigned short uRange, unsigned short idx, bool bPaySuccess, CItem &item);

    void UpdateCountAddition(int &timeElapsed);

    void GetSaveCountInfo(std::list<CSaveGroupBuyingCount> &rListCount);
    void UpdateCountInfo(std::list<CGetLimitedSalesCount> *pCountInfo);

private:
    void AddGoodsInfo(LimitedGoodsInfo *pGoodsInfo);

    void CompareGMSettings();

    void CountAddition();
    int RandomAdditionNum(unsigned int lower, unsigned int upper);

protected:
    virtual void SetOpening(bool bOpening);

private:
    std::map<unsigned short, std::map<unsigned short, LimitedGoodsInfo *> > m_mapGoodsInfo;

    CLimitedSalesSettings   m_cSettings;
    CLimitedSalesGMSettings m_cGMSettings;

    ELimitedSalesType       m_eType;

    bool                    m_bFirstQuery;
    bool                    m_bUseGMSettings;
};

#endif // __LIMITEDSALES_H__

