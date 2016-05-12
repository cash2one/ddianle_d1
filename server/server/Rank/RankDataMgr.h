/*
 * RankData.h
 * Description: ÅÅÐÐ°ñÊý¾Ý
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-2-4 14:21:20
 */

#ifndef __RANKDATA_H__
#define __RANKDATA_H__

#include "RankDefine.h"
#include "../../datastructure/DataStruct_Rank.h"

#include <set>
#include <map>
#include <list>

class CRankMgr;

class CRankMailInfo
{
public:
    std::string m_strSender;
    std::string m_strTitle;
    std::string m_strContent;
};

//////////////////////////////////////////////////////////////////////////

class RankDataMgr
{
public:
    RankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount, bool bActiveOpend = true );
    virtual ~RankDataMgr();

public:
    void Initialize();

    void AddReward( const CRankReward &rReward );
    void LoadRankInfo( unsigned char nDetailType, const std::list<CRankInfo *> &rInfoList );

    unsigned short GetDataType() const { return m_nDataType; }
    unsigned short GetHeaderIndex() const { return m_nHeaderIndex; }
    const std::string& GetTitle() const { return m_strTitle; }
    bool IsInitialized() const { return m_bInitialized; }
    bool IsActiveOpend() const { return m_bActiveOpend; }
    unsigned int GetMaxShowCount() const { return m_nMaxShowCount; }
    unsigned int GetMaxLoadCount( unsigned char nDetailType ) const;

    bool GetRoleRank( unsigned char nDetailType, unsigned int nKey, unsigned int *pRoleRank, unsigned int *pTotal = NULL ) const;
    bool GetRankDatas( unsigned char nDetailType, unsigned short nPage, unsigned short nCount, std::list<CRankOfflineInfo> &rOfflineInfos ) const;

    bool IsInCurrentWeek( unsigned int nKey ) const;
    unsigned int GetCurrentWeekMinValue() const;

    void RemoveRankInfo( unsigned int nKey );

    void ChangeToNextRange( const CRankMailInfo *pMailInfo );

    void ChangeCurrentWeekValue( const CRankInfo *pRankInfo );

protected:
    CRankMgr* GetRankMgrByDetailType( unsigned char nDetailType ) const;

    std::string GetRoleName( unsigned int nRoleID ) const;
    void GetOfflineRoleImage( unsigned int nRoleID, RoleImageOfflineData &rRoleImage ) const;

    void SendMail( unsigned int nRoleID, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const;

public:
    virtual bool GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 );

protected:
    virtual bool AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID ) = 0;

    virtual bool GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const = 0;

    virtual void SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const = 0;

    virtual bool MoveToLastWeek( const CRankInfo *pRankInfo );
    virtual void ResetCurrentWeek();

protected:
    unsigned short          m_nDataType;
    unsigned short          m_nHeaderIndex; // load from lua, begin from 1
    std::string             m_strTitle;
    std::set<CRankReward>   m_setReward;
    unsigned int            m_nMaxShowCount;
    unsigned int            m_nCurrentWeekMaxLoadCount;
    bool                    m_bInitialized;
    bool                    m_bActiveOpend;

    CRankMgr                *m_pCurrentWeekMgr;
    CRankMgr                *m_pLastWeekMgr;
    CRankMgr                *m_pActiveMgr;
};

//////////////////////////////////////////////////////////////////////////

class RoleRankDataMgr : public RankDataMgr
{
public:
    RoleRankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount );
    virtual ~RoleRankDataMgr();

public:
    virtual bool GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 );

protected:
    virtual bool AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID );

    virtual bool GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const;

    virtual void SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const;
};

//////////////////////////////////////////////////////////////////////////

class GroupRankDataMgr : public RankDataMgr
{
public:
    GroupRankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount );
    virtual ~GroupRankDataMgr();

public:
    virtual bool GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 );

protected:
    virtual bool AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID );

    virtual bool GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const;

    virtual void SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const;
};

//////////////////////////////////////////////////////////////////////////

class CoupleRankDataMgr : public RankDataMgr
{
public:
    CoupleRankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount );
    virtual ~CoupleRankDataMgr();

public:
    virtual bool GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 );

protected:
    virtual bool AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID );

    virtual bool GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const;

    virtual void SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const;
};

//////////////////////////////////////////////////////////////////////////

class RoleCumulativeRankDataMgr : public RankDataMgr
{
public:
    RoleCumulativeRankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount );
    virtual ~RoleCumulativeRankDataMgr();

public:
    void RemoveCurrentRoleByOfflineTime( unsigned int nMaxOfflineTime );

public:
    virtual bool GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 );

protected:
    virtual bool AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID );

    virtual bool GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const;

    virtual void SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const;

    virtual bool MoveToLastWeek( const CRankInfo *pRankInfo );
    virtual void ResetCurrentWeek();

private:
    std::map<unsigned int, unsigned int> m_mapLastWeekHistory;// key -> value
};

//////////////////////////////////////////////////////////////////////////

class RankDataFactory
{
public:
    static RankDataMgr* CreateRankData( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount );
};

#endif // __RANKDATA_H__

