/*
 * RankConfigData.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-2-5 15:37:03
 */

#ifndef __RANKCONFIGDATA_H__
#define __RANKCONFIGDATA_H__

#include "RankDefine.h"
#include "../../socket/GameMsg_Processor.h"
#include <map>

/**
* CRankConfigData:
* 
*/

class CRankConfigData : public GameMsg_Processor
{
protected:
    CRankConfigData();
    virtual ~CRankConfigData();

public:
    static CRankConfigData& Instance();
    void Initialize();

    bool IsOpening() const { return m_bIsOpening; }

public:
    void LoadStartTime(unsigned int nStartWeekDay, unsigned int nStartHour);
    void LoadMaxOfflineTime( int nMaxOfflineTime );
    void AddRankHall(unsigned short nType);
    void LoadRule( const std::string &rRewardRule, const std::string &rScoreRule );
    void AddRankReward( unsigned short nDataType, unsigned short nSecond, const std::list<CItem> &rItems, int nMoney, int nBindMCoin );

    void GetStartTime(int &nStartWeekDay, int &nStartHour) const;

    void GetAllTypes( std::list<unsigned short> &rTypes ) const;
    void GetAllRewards( unsigned short nDataType, std::list<CRankReward> &rRewards ) const;

    const std::string& GetRewardRule() const { return m_strRewardRule; }
    const std::string& GetScoreRule() const { return m_strScoreRule; }

    bool IsBiggerThanMinValue( unsigned short nDataType, unsigned int nValue );
    bool IsBeyondMaxOfflineTime( unsigned int nLastOfflineTime ) const;

private:
    void RegComponentNetMsgMap();

    void OnUpdateRankMinValue( GameMsg_Base &msg, CSlotPeer &SlotPeer );

private:
    int         m_nStartWeekDay;        // [0, 6]
    int         m_nStartHour;           // [0, 23]
    int         m_nMaxOfflineTime;      // > 0

    bool        m_bIsOpening;

    std::string m_strRewardRule;
    std::string m_strScoreRule;

    std::list<unsigned short>                   m_listTypes;        // all types
    std::map<unsigned short, unsigned int>      m_mapType2MinValue; // data type -> min value
    std::map<unsigned short, std::set<CRankReward> > m_mapRewards;
};

#endif // __RANKCONFIGDATA_H__

