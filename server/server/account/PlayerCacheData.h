/*
 * PlayerCacheData.h
 * Description: 
 * Copyright (C) 2016 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2016-1-6 15:17:35
 */

#ifndef __PLAYERCACHEDATA_H__
#define __PLAYERCACHEDATA_H__

#include <list>
#include <string>

class PlayerCacheSegment;

/**
* PlayerCacheData:
* 
*/

enum EPlayerCacheState
{
    EPlayerCacheState_None = 0,
    EPlayerCacheState_Error,
    EPlayerCacheState_Success
};

class PlayerCacheData
{
public:
    PlayerCacheData( unsigned int nAccountID, unsigned int nExpirationTime );
    ~PlayerCacheData();

protected:
    PlayerCacheData( const PlayerCacheData & );
    PlayerCacheData& operator= (const PlayerCacheData &);

public:
    unsigned int GetAccountID() const { return m_nAccountID; }

    void SetExpirationTime( unsigned int nExpirationTime ) { m_nExpirationTime = nExpirationTime; }
    unsigned int GetExpirationTime() const { return m_nExpirationTime; }

    void AddCacheData( unsigned short nKey, unsigned int nMaxSize, unsigned int nSize, const char *data );
    void AddCacheDataEnd();

    bool PopCacheData( unsigned short &rKey, unsigned int &rMaxSize, std::string &rData );

    bool IsCacheSuccess() const;
    bool HasData() const;

    void SetState( unsigned char nState );

private:
    void CreateCacheSegment( unsigned short nKey, unsigned int nMaxSize, unsigned int nSize, const char *data );

    void Clear();

private:
    unsigned int    m_nAccountID;
    unsigned int    m_nExpirationTime;
    unsigned char   m_nState;

    std::list<PlayerCacheSegment *> m_listSegments;
};

#endif // __PLAYERCACHEDATA_H__

