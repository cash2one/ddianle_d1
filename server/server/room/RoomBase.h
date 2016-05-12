/*
 * RoomBase.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-10-23 17:36:01
 */

#ifndef __ROOMBASE_H__
#define __ROOMBASE_H__

#include "../Macros/Macros.h"
#include "../roomLib/RoomDataStruct.h"

class GameMsg_Base;

/**
* RoomRoleBase:
* basic player or NPC
*/

class RoomRoleBase
{
protected:
    explicit RoomRoleBase( unsigned int nRoleID );

public:
    virtual ~RoomRoleBase();

public:
    virtual void SendMessage( GameMsg_Base &msg ) = 0;

    //virtual bool doEncode( CParamPool& IOBuff ) const = 0;

    CLASS_PROPERTY_READONLY(unsigned int, m_nRoleID, RoleID)
};

//////////////////////////////////////////////////////////////////////////

/**
* RoomBase:
* basic room
*/

#define INVALID_ROOM_ID 0

class RoomBase
{
protected:
    RoomBase( ERoomType eType, unsigned int nRoomID, const std::string &rName = "", const std::string &rPassword = "" );

public:
    virtual ~RoomBase();

public:
    void SetPassword( const std::string &rPassword ) { m_strPassword = rPassword; }
    void ChangeRoomInfo( const std::string &rName, const std::string &rPassword = "" );

    bool HasPassword() const { return !m_strPassword.empty(); }
    bool CheckPassword( const std::string &rPassword ) const { return ( m_strPassword == rPassword); }

    int GetPlayerCount() const { return (int)m_mapPlayers.size(); }

    void SendMessage( GameMsg_Base &msg, unsigned int nExceptRoleID = 0 );

    //virtual bool doEncode( CParamPool& IOBuff ) const = 0;

protected:
    bool PlayerIn( RoomRoleBase *pPlayer );
    RoomRoleBase* PlayerOut( unsigned int nRoleID );

    void RemovePlayer( unsigned int nRoleID );
    void ClearPlayer();

    RoomRoleBase* FindPlayer( unsigned int nRoleID ) const;

    CLASS_PROPERTY_READONLY(ERoomType, m_eType, Type)
    CLASS_PROPERTY_READONLY(unsigned int, m_nRoomID, RoomID)
    CLASS_PROPERTY(int, m_nScene, Scene)
    CLASS_PROPERTY_BY_REF(std::string, m_strName, RoomName)

protected:
    std::string m_strPassword;
    std::map<unsigned int, RoomRoleBase *> m_mapPlayers; // key role id
};

#endif // __ROOMBASE_H__

