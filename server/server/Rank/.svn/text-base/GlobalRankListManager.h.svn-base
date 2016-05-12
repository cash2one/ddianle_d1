/*
 * GlobalRankListManager.h
 * Description: 全局排行榜列表管理
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-2-3 16:03:18
 */

#ifndef __GLOBALRANKLISTMANAGER_H__
#define __GLOBALRANKLISTMANAGER_H__

#include "RankDefine.h"
#include "../../socket/SharedPtr.h"
#include "../../socket/GameMsg_Processor.h"

#include <map>

class RankDataMgr;
class CRankMailInfo;
class CRankUnsaveData;

/**
* CGlobalRankListManager:
* 
*/

class CGlobalRankListManager : public GameMsg_Processor
{
protected:
    CGlobalRankListManager();
    virtual ~CGlobalRankListManager();

public:
    static CGlobalRankListManager& Instance();
    void Initialize();

    void OnUpdate(time_t tNow);

    void SetClosed();

public:
    void LoadStartTime(unsigned int nStartWeekDay, unsigned int nStartHour);
    void LoadMaxOfflineTime( int nMaxOfflineTime );
    void SetMailInfo( const std::string &rSender, const std::string &rTitle, const std::string &rContent );
    void AddHeaderName( unsigned short nHeadIndex, const std::list<std::string> &rNameList );
    void AddRankHall( unsigned short nIndex, unsigned short nDataType,
        unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount );
    void AddRankReward( unsigned short nDataType, unsigned short nSecond, const std::list<CItem> &rItems, int nMoney, int nBindMCoin );

    void RemoveFromRank(ERankDataType eDataType, unsigned int nKey);

    void ChangeDanceGroupRankValue( unsigned int nKey, unsigned int nValue );
    void ChangeCoupleRankValue( unsigned int nKey, unsigned int nValue, unsigned int nMaleRoleID, unsigned int nFemaleRoleID, unsigned int nSmallRoleID );

private:
    void RegisterMessage();

    void QueryRankDatas(unsigned short nDataType, unsigned char nDetailType, unsigned int nStartTime, unsigned int nEndTime, unsigned int nMaxCount);

    unsigned int CalcCurrentStartTime() const;
    unsigned int CalcCurrentLoadTime( unsigned short nDataType, unsigned int nCurrentStartTime ) const;
    unsigned int CalcLastStartTime(unsigned int nCurrentStartTime) const;
    void CalcActiveTime(unsigned int &nStartTime, unsigned int &nEndTime) const;
    unsigned int CalcSaveDataTime(time_t nNow) const;

    void ChangeToNextRange();

    bool SaveOneDataPreDay(unsigned short nDataType, unsigned int nKey, int nValue, unsigned int nSaveTime);
    void SaveUnsaveDatas(unsigned short nDataType, std::map<unsigned int, int> &rUnsaveMap, unsigned int nMaxCount, unsigned int nSaveTime);

    bool NotFoundHeaderName( unsigned short nHeadIndex ) const;
    SharedPtr<RankDataMgr> GetDataByType( unsigned short nDataType ) const;

    void GetRoleRank( unsigned short nDataType, unsigned char nDetailType, unsigned int nKey, unsigned int &rRoleRank ) const;

    bool CanChangeCurrentWeekValue( unsigned short nDataType, unsigned int nStartTime ) const;

private:
    // 获取排行榜大厅列表
    void OnGetRankHall(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取排行榜数据
    void OnGetRankDatas(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 更新数据
    void OnChangeRankValue(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取大部分排行榜数据
    void OnGetAllRankDatasForMedal(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 加载排行榜数据
    void OnQueryGetAllDatas(QueryBase &rQuery);
    //获取大部分排行榜数据
    void OnGetAllRankDatas(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    int             m_nStartWeekDay;        // [0, 6]
    int             m_nStartHour;           // [0, 23]
    int             m_nMaxOfflineTime;      // > 0

    int             m_nLastHour;
    unsigned int    m_nLastSaveGroupTime;
    unsigned int    m_nLastSaveIntimacyTime;

    bool            m_bOpening;
    bool            m_bCallUpdate;
    bool            m_bSetClosed;
    bool            m_bShowGay;

    CRankMailInfo   *m_pMailInfo;

    std::map<unsigned short, std::list<std::string> >   m_mapHeaderName;            // header index -> list<string>
    std::map<unsigned short, SharedPtr<RankDataMgr> >   m_mapIndex2Data;            // show index -> CRankData *
    std::map<unsigned short, SharedPtr<RankDataMgr> >   m_mapType2Data;             // data type -> CRankData *

    std::multimap<unsigned int, unsigned short>         m_mapUnRemoveKey;           // key -> data type

    std::map<unsigned int, int>                         m_mapUnsaveGroupHonor;      // key -> value
    std::map<unsigned int, int>                         m_mapUnsaveIntimacy;        // key -> value
};

#endif // __GLOBALRANKLISTMANAGER_H__

