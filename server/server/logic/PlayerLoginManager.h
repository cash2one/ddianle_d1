/*
 * PlayerLoginManager.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-12-23 14:29:55
 */

#ifndef __PLAYERLOGINMANAGER_H__
#define __PLAYERLOGINMANAGER_H__

#include "../share/ServerDataStruct.h"
#include "../../socket/SharedPtr.h"
#include <map>
#include <list>
#include <string>

struct ROLEINFO_DB;

class LogicSlot
{
    friend class PlayerLoginManager;
protected:
    LogicSlot();

public:
    static SharedPtr<LogicSlot> Create();

    void SetInGame();
    bool IsInGame() const;

private:
    unsigned char   m_nState;

public:
    bool                    m_bSwitchLine;
    PLAYERTAG               m_tag;
    SharedPtr<ROLEINFO_DB>  m_sharedRole;
};

class LoginElement
{
    friend class PlayerLoginManager;

public:
    LoginElement( unsigned int nExpirationTime, SharedPtr<LogicSlot> sharedSlot );
    LoginElement( const LoginElement &ele );
    LoginElement& operator= (const LoginElement &ele);

private:
    unsigned int            m_nExpirationTime;
    SharedPtr<LogicSlot>    m_sharedSlot;
};

class LoadingCacheData
{
    friend class PlayerLoginManager;

    LoadingCacheData();

    void Reset();

    unsigned int    m_nAccountID;
    unsigned short  m_nKey;
    unsigned int    m_nMaxSize;
    unsigned int    m_nCurrentSize;
    std::string     m_buff;
};

/**
* PlayerLoginManager:
* 
*/

enum ERequestCacheRes
{
    ERequestCacheRes_Success = 0,
    ERequestCacheRes_NotFound,
    ERequestCacheRes_InvalidSlot,
    ERequestCacheRes_InvalidSessionID,
    ERequestCacheRes_InvalidState,
    ERequestCacheRes_NotCacheSuccess,
};

class PlayerLoginManager
{
public:
    PlayerLoginManager();
    virtual ~PlayerLoginManager();

public:
    void OnUpdate( int nTimeElapse );

    unsigned int GetOnlinePlayerCount() const { return m_mapSlot.size(); }

    const std::map<unsigned int, SharedPtr<LogicSlot> >& GetAllPlayerInfos() const { return m_mapSlot; }

    SharedPtr<LogicSlot> GetPlayerInfo( unsigned int nAccountID ) const;
    SharedPtr<LogicSlot> GetPlayerInfoAndRemove( unsigned int nAccountID );
    SharedPtr<LogicSlot> GetNextQueryPlayer();

    void GetAllPlayerAccountIDBySlot( unsigned short nSlot, std::list<unsigned int> &rAllPlayerIDs ) const;

    void EnteringLoginQueue( unsigned int nAccountID, SharedPtr<LogicSlot> slot, bool bFromCache = true );

    bool IsDBQueueEmpty() const { return m_listDBQueue.empty(); }

    void RemovePlayer( unsigned int nAccountID );

    int RequestPlayerCacheRes( unsigned int nAccountID, __int64 nSessionID, bool bCacheSuccess, bool bHasCache );
    void ReceiveCacheDataBegin( unsigned int nAccountID );
    void ReceiveCacheData( unsigned int nAccountID, unsigned short nKey, unsigned int nMaxSize, unsigned int nSize, const char *pData );
    void ReceiveCacheDataEnd( unsigned int nAccountID );

protected:
    bool IsPlayerLoginInfoSame( SharedPtr<LogicSlot> slot ) const;

    void MoveFromLoginToDB( std::list<LoginElement>::iterator itr );

private:
    std::map<unsigned int, SharedPtr<LogicSlot> >   m_mapSlot;  // key: Account ID

    std::list<LoginElement>             m_listLoginQueue;       // 登录队列
    std::list<SharedPtr<LogicSlot> >    m_listDBQueue;          // DB加载队列

    LoadingCacheData    m_loadingCacheData; // 当前正在加载的Cache

    int m_nLastTimeElapse;
};

#endif // __PLAYERLOGINMANAGER_H__

