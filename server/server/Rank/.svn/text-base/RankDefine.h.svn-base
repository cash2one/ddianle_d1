/*
 * RankDefine.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-3-6 13:15:00
 */

#ifndef __RANKDEFINE_H__
#define __RANKDEFINE_H__

class CParamPool;

#include "../../datastructure/DataStruct_Base.h"

#include <list>
#include <string>

//////////////////////////////////////////////////////////////////////////

enum ERankDataType
{
    ERankDataType_MCoin = 0,                // 购物狂
    ERankDataType_Horn,                     // 喇叭王
    ERankDataType_Exp,                      // 升级达人
    ERankDataType_Group_Honor,              // 荣誉舞团
    ERankDataType_Intimacy,                 // 明星夫妻
    ERankDataType_Cloths,                   // 收藏家
    ERankDataType_ClothEffect,              // 闪亮之星
    ERankDataType_Medal,                    // 勋章积分
    ERankDataType_Max
};

//////////////////////////////////////////////////////////////////////////

enum ERankDetailType
{
    ERankDetailType_CurrentWeek = 0,        // 本周榜
    ERankDetailType_LastWeek,               // 上周榜
    ERankDetailType_Active,                 // 活跃榜

    ERankDetailType_Max
};

//////////////////////////////////////////////////////////////////////////

class CRankTopOneInfo
{
public:
    CRankTopOneInfo();
    ~CRankTopOneInfo() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff) const;

public:
    unsigned char           m_nType;
    std::string             m_strTitle;
    bool                    m_bActiveOpend;
    std::list<std::string>  m_listHeaderName;
    std::string             m_strDisplayInfo1;
    std::string             m_strDisplayInfo2;
    bool                    m_bInitialized;
};

//////////////////////////////////////////////////////////////////////////

class CRankOfflineInfo
{
public:
    CRankOfflineInfo();
    ~CRankOfflineInfo() {}

public:
    void doDecode( CParamPool &IOBuff );
    void doEncode( CParamPool &IOBuff );

public:
    unsigned short          m_nRank;
    std::list<std::string>  m_listOtherInfo;
    unsigned int            m_nSmallRoleID;
    std::list<RoleImageOfflineData> m_listOfflineData;
};

//////////////////////////////////////////////////////////////////////////

class CRankReward
{
public:
    CRankReward( unsigned short nSecond, const std::list<CItem> &rItems, int nMoney, int nBindMCoin );
    virtual ~CRankReward() {}

    bool operator< (const CRankReward &rReward) const;

public:
    unsigned short      m_nSecond;
    std::list<CItem>    m_listItems;
    int                 m_nMoney;
    int                 m_nBindMCoin;
};

#endif // __RANKDEFINE_H__

