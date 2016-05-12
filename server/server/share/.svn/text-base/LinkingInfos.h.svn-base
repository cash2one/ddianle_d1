/*
 * LinkingInfos.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-11-10 16:59:46
 */

#ifndef __LINKINGINFOS_H__
#define __LINKINGINFOS_H__

#include <vector>

class GameServerLinkInfo;

/**
* LinkingInfos:
* 
*/

class LinkingInfos
{
public:
    LinkingInfos();
    virtual ~LinkingInfos();

public:
    void Initialize( unsigned short nMaxCount );

    void Clear();

    bool AddServerInfo( const GameServerLinkInfo &rInfo );
    int RemoveServerInfoBySlot( unsigned short nSlot );

    void SetClientBySlot( unsigned short nSlot, unsigned short nClient );

    int GetAvailableLine() const;

    int GetLineBySlot( unsigned short nSlot ) const;
    unsigned short GetSlotByLine( int nLine ) const;

    const GameServerLinkInfo* GetLinkInfoByLine( int nLine ) const;
    const GameServerLinkInfo* GetLinkInfoBySlot( unsigned short nSlot ) const;
    const std::vector<GameServerLinkInfo *>& GetAllLinkInfos() const { return m_vectLinkInfo; }

private:
    GameServerLinkInfo* FindLinkInfoBySlot( unsigned short nSlot ) const;

    bool IsValidLine( int nLine ) const;

private:
    unsigned short                      m_nMaxCount;
    std::vector<unsigned short>         m_vectSlot;     // line -> slot
    std::vector<GameServerLinkInfo *>   m_vectLinkInfo; // slot -> link info
};

#endif // __LINKINGINFOS_H__

