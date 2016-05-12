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
    ERankDataType_MCoin = 0,                // �����
    ERankDataType_Horn,                     // ������
    ERankDataType_Exp,                      // ��������
    ERankDataType_Group_Honor,              // ��������
    ERankDataType_Intimacy,                 // ���Ƿ���
    ERankDataType_Cloths,                   // �ղؼ�
    ERankDataType_ClothEffect,              // ����֮��
    ERankDataType_Medal,                    // ѫ�»���
    ERankDataType_Max
};

//////////////////////////////////////////////////////////////////////////

enum ERankDetailType
{
    ERankDetailType_CurrentWeek = 0,        // ���ܰ�
    ERankDetailType_LastWeek,               // ���ܰ�
    ERankDetailType_Active,                 // ��Ծ��

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

