/*
 * PlayerCacheManager.h
 * Description: 
 * Copyright (C) 2016 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2016-1-6 14:23:25
 */

#ifndef __PLAYERCACHEMANAGER_H__
#define __PLAYERCACHEMANAGER_H__

#include <map>
#include <list>
#include <string>

class PlayerCacheData;

/**
* PlayerCacheManager:
* 
*/

class PlayerCacheManager
{
public:
    PlayerCacheManager();
    ~PlayerCacheManager();

protected:
    PlayerCacheManager( const PlayerCacheManager & );
    PlayerCacheManager& operator= (const PlayerCacheManager &);

public:
    void Init( unsigned int nMaxCount, unsigned int nValidTime );

    void OnUpdate( int nTimeElapse );

    void ReceiveCacheDataBegin( unsigned int nAccountID );
    void ReceiveCacheData( unsigned int nAccountID, unsigned short nKey, unsigned int nMaxSize, unsigned int nSize, const char *data );
    void ReceiveCacheDataEnd( unsigned int nAccountID );

    bool IsInReceiving( unsigned int nAccountID );

    bool IsCacheSuccess( unsigned int nAccountID );
    bool HasCached( unsigned int nAccountID );
    bool GetNextPlayerCacheData( unsigned int nAccountID, unsigned short &rKey, unsigned int &rMaxSize, std::string &rData );

protected:
    void RemoveExpiredCache( std::list<unsigned int> &cacheQueue, std::map<unsigned int, PlayerCacheData *> &cache, unsigned int nCount, bool bForce = false );

    bool CheckCached( unsigned int nAccountID );
    bool CheckInReceiving( unsigned int nAccountID );

    void AddErrorCache( unsigned int nAccountID );

private:
    unsigned int    m_nMaxCount;
    unsigned int    m_nValidTime;

    int             m_nCheckPlayerElapse;
    int             m_nCheckReceivingElapse;

    std::map<unsigned int, PlayerCacheData *>   m_mapPlayerCache;
    std::list<unsigned int> m_playerCacheQueue;

    std::map<unsigned int, PlayerCacheData *>   m_mapReceivingCache;
    std::list<unsigned int> m_receivingCacheQueue;
};

#endif // __PLAYERCACHEMANAGER_H__

