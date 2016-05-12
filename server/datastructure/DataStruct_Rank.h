/*
 * DataStruct_Rank.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-3-6 16:46:27
 */

#ifndef __DATASTRUCT_RANK_H__
#define __DATASTRUCT_RANK_H__

#include "../server/Rank/RankMgr.h"
#include <map>
#include <list>

//////////////////////////////////////////////////////////////////////////

class CRankSaveEveryDataDB
{
public:
    CRankSaveEveryDataDB();
    ~CRankSaveEveryDataDB() {}

public:
    unsigned int            m_nKey;
    unsigned short          m_nDataType;
    unsigned int            m_nValue;
};

//////////////////////////////////////////////////////////////////////////

class CRankUpdateDataStateDB
{
public:
    CRankUpdateDataStateDB();
    ~CRankUpdateDataStateDB() {}

public:
    unsigned int            m_nKey;
    unsigned short          m_nDataType;
    unsigned int            m_nTime;
};

//////////////////////////////////////////////////////////////////////////

// key: ÇéÂÂID
class CRankCoupleValue
{
public:
    CRankCoupleValue();
    ~CRankCoupleValue() {}

public:
    unsigned int        m_nPairID;
    unsigned int        m_nMaleRoleID;
    unsigned int        m_nFemaleRoleID;
    unsigned int        m_nSamllRoleID;
    unsigned int        m_nValue;
};

//////////////////////////////////////////////////////////////////////////

class CRankCoupleInfo : public CRankInfo
{
protected:
    CRankCoupleInfo();

public:
    CRankCoupleInfo(const CRankCoupleValue &rValue);
    virtual ~CRankCoupleInfo() {}

    virtual void SetValue( unsigned int nValue );
    virtual bool ChangeValue( int nChangeValue );

public:
    CRankCoupleValue    m_RankValue;
};

//////////////////////////////////////////////////////////////////////////

class CRankRoleCumulativeValue
{
public:
    CRankRoleCumulativeValue();
    ~CRankRoleCumulativeValue() {}

public:
    unsigned int        m_nRoleID;
    unsigned int        m_nValue;
    unsigned int        m_nOfflineTime;
};

class CRankRoleCumulativeInfo : public CRankInfo
{
protected:
    CRankRoleCumulativeInfo();

public:
    CRankRoleCumulativeInfo( const CRankRoleCumulativeValue &rValue );
    ~CRankRoleCumulativeInfo() {}

    virtual void SetValue( unsigned int nValue );
    virtual bool ChangeValue( int nChangeValue );

public:
    CRankRoleCumulativeValue    m_rankValue;
};

//////////////////////////////////////////////////////////////////////////

class RankGetAllDatasDB
{
public:
    RankGetAllDatasDB( unsigned short nDataType );
    virtual ~RankGetAllDatasDB();

    unsigned short GetDataType() const { return m_nDataType; }
    const std::list<CRankInfo *>& GetRankInfo() const { return m_listRankInfo; }

    virtual void GetSQL( std::string &rSQL ) const = 0;
    virtual void DecodeByRow( const char **row ) = 0;

private:
    unsigned short          m_nDataType;

public:
    unsigned char           m_nDetailType;
    unsigned int            m_nStartTime;
    unsigned int            m_nEndTime;
    unsigned int            m_nMaxCount;
    bool                    m_bShowGay;

protected:
    std::list<CRankInfo *>  m_listRankInfo;
};

class RankRoleGetAllDatasDB : public RankGetAllDatasDB
{
public:
    RankRoleGetAllDatasDB( unsigned short nDataType );
    virtual ~RankRoleGetAllDatasDB() {}

    virtual void DecodeByRow( const char **row );
};

class RankMCoinGetAllDatasDB : public RankRoleGetAllDatasDB
{
public:
    RankMCoinGetAllDatasDB( unsigned short nDataType );
    virtual ~RankMCoinGetAllDatasDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankHornGetAllDatasDB : public RankRoleGetAllDatasDB
{
public:
    RankHornGetAllDatasDB( unsigned short nDataType );
    virtual ~RankHornGetAllDatasDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankExpGetAllDatasDB : public RankRoleGetAllDatasDB
{
public:
    RankExpGetAllDatasDB( unsigned short nDataType );
    virtual ~RankExpGetAllDatasDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankGroupHonorGetAllDatasDB : public RankGetAllDatasDB
{
public:
    RankGroupHonorGetAllDatasDB( unsigned short nDataType );
    virtual ~RankGroupHonorGetAllDatasDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
    virtual void DecodeByRow( const char **row );
};

class RankIntimacyGetAllDatasDB : public RankGetAllDatasDB
{
public:
    RankIntimacyGetAllDatasDB( unsigned short nDataType );
    virtual ~RankIntimacyGetAllDatasDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
    virtual void DecodeByRow( const char **row );
};

class RankRoleCumulativeGetAllDatasDB : public RankGetAllDatasDB
{
public:
    RankRoleCumulativeGetAllDatasDB( unsigned short nDataType );
    virtual ~RankRoleCumulativeGetAllDatasDB() {}

    virtual void DecodeByRow( const char **row );
};

class RankClothsGetAllDatasDB : public RankRoleCumulativeGetAllDatasDB
{
public:
    RankClothsGetAllDatasDB( unsigned short nDataType );
    virtual ~RankClothsGetAllDatasDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankClothEffectGetAllDatasDB : public RankRoleCumulativeGetAllDatasDB
{
public:
    RankClothEffectGetAllDatasDB( unsigned short nDataType );
    virtual ~RankClothEffectGetAllDatasDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankMedalGetAllDatasDB : public RankRoleCumulativeGetAllDatasDB
{
public:
    RankMedalGetAllDatasDB( unsigned short nDataType );
    virtual ~RankMedalGetAllDatasDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankGetAllDatasDBFactory
{
public:
    static RankGetAllDatasDB* Create( unsigned short nDataType );
};

//////////////////////////////////////////////////////////////////////////

class RankGetRoleDataDB
{
public:
    RankGetRoleDataDB( unsigned short nDataType );
    virtual ~RankGetRoleDataDB() {}

    unsigned short GetDataType() const { return m_nDataType; }
    unsigned int GetValue() const { return m_nValue; }

    virtual void GetSQL( std::string &rSQL ) const = 0;
    virtual void DecodeByRow( const char **row );

private:
    unsigned short          m_nDataType;

public:
    unsigned int            m_nKey;
    unsigned int            m_nStartTime;

protected:
    unsigned int            m_nValue;
};

class RankMCoinGetRoleDataDB : public RankGetRoleDataDB
{
public:
    RankMCoinGetRoleDataDB( unsigned short nDataType );
    virtual ~RankMCoinGetRoleDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankHornGetRoleDataDB : public RankGetRoleDataDB
{
public:
    RankHornGetRoleDataDB( unsigned short nDataType );
    virtual ~RankHornGetRoleDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankExpGetRoleDataDB : public RankGetRoleDataDB
{
public:
    RankExpGetRoleDataDB( unsigned short nDataType );
    virtual ~RankExpGetRoleDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankRoleCumulativeGetRoleDataDB : public RankGetRoleDataDB
{
public:
    RankRoleCumulativeGetRoleDataDB( unsigned short nDataType );
    virtual ~RankRoleCumulativeGetRoleDataDB() {}

    virtual void DecodeByRow( const char **row );
};

class RankClothsGetRoleDataDB : public RankRoleCumulativeGetRoleDataDB
{
public:
    RankClothsGetRoleDataDB( unsigned short nDataType );
    virtual ~RankClothsGetRoleDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankClothEffectGetRoleDataDB : public RankRoleCumulativeGetRoleDataDB
{
public:
    RankClothEffectGetRoleDataDB( unsigned short nDataType );
    virtual ~RankClothEffectGetRoleDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankMedalGetRoleDataDB : public RankRoleCumulativeGetRoleDataDB
{
public:
    RankMedalGetRoleDataDB( unsigned short nDataType );
    virtual ~RankMedalGetRoleDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankGetRoleDataDBFactory
{
public:
    static RankGetRoleDataDB* Create( unsigned short nDataType );
};

//////////////////////////////////////////////////////////////////////////

class RankSaveDataDB
{
public:
    RankSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankSaveDataDB() {}

    unsigned short GetDataType() const { return m_nDataType; }
    int GetValue() const { return m_nValue; }

    virtual bool CanProcessValue() const;

    virtual void GetSQL( std::string &rSQL ) const = 0;

private:
    unsigned short          m_nDataType;

protected:
    unsigned int            m_nKey;
    int                     m_nValue;

public:
    unsigned int            m_nTime;
};

class RankMCoinSaveDataDB : public RankSaveDataDB
{
public:
    RankMCoinSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankMCoinSaveDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankHornSaveDataDB : public RankSaveDataDB
{
public:
    RankHornSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankHornSaveDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankExpSaveDataDB : public RankSaveDataDB
{
public:
    RankExpSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankExpSaveDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankGroupHonorSaveDataDB : public RankSaveDataDB
{
public:
    RankGroupHonorSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankGroupHonorSaveDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankIntimacySaveDataDB : public RankSaveDataDB
{
public:
    RankIntimacySaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankIntimacySaveDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankRoleCumulativeSaveDataDB : public RankSaveDataDB
{
public:
    RankRoleCumulativeSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankRoleCumulativeSaveDataDB() {}

    virtual bool CanProcessValue() const { return true; }
};

class RankClothsSaveDataDB : public RankRoleCumulativeSaveDataDB
{
public:
    RankClothsSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankClothsSaveDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankClothEffectSaveDataDB : public RankRoleCumulativeSaveDataDB
{
public:
    RankClothEffectSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankClothEffectSaveDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankMedalSaveDataDB : public RankRoleCumulativeSaveDataDB
{
public:
    RankMedalSaveDataDB( unsigned short nDataType, unsigned int nKey, int nValue );
    virtual ~RankMedalSaveDataDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankSaveDataDBFactory
{
public:
    static RankSaveDataDB* Create( unsigned short nDataType, unsigned int nKey, int nValue );
};

//////////////////////////////////////////////////////////////////////////

class RankSaveHistoryDB
{
public:
    RankSaveHistoryDB( unsigned short nDataType );
    virtual ~RankSaveHistoryDB() {}

    virtual void GetSQL( std::string &rSQL ) const = 0;

private:
    unsigned short          m_nDataType;

public:
    std::map<unsigned int, unsigned int>    m_mapHistoryDatas;// key -> value
};

class RankClothsSaveHistoryDB : public RankSaveHistoryDB
{
public:
    RankClothsSaveHistoryDB( unsigned short nDataType );
    virtual ~RankClothsSaveHistoryDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankClothEffectSaveHistoryDB : public RankSaveHistoryDB
{
public:
    RankClothEffectSaveHistoryDB( unsigned short nDataType );
    virtual ~RankClothEffectSaveHistoryDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankMedalSaveHistoryDB : public RankSaveHistoryDB
{
public:
    RankMedalSaveHistoryDB( unsigned short nDataType );
    virtual ~RankMedalSaveHistoryDB() {}

    virtual void GetSQL( std::string &rSQL ) const;
};

class RankSaveHistoryDBFactory
{
public:
    static RankSaveHistoryDB* Create( unsigned short nDataType );
};

#endif // __DATASTRUCT_RANK_H__

