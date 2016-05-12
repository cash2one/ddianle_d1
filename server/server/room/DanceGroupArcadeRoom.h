/*
 * DanceGroupArcadeRoom.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-10-25 11:35:55
 */

#ifndef __DANCEGROUPARCADEROOM_H__
#define __DANCEGROUPARCADEROOM_H__

#include "../Macros/Macros.h"
#include <list>
#include <string>
#include <vector>

class DanceBaseRoom;
class CRoomMarkInfo;

/**
* DanceGroupArcadeRoomManager:
* manager dance group arcade room
*/

class DanceGroupArcadeRoomManager
{
private:
    DanceGroupArcadeRoomManager();
    virtual ~DanceGroupArcadeRoomManager();
    DanceGroupArcadeRoomManager( const DanceGroupArcadeRoomManager & );

public:
    CLASS_INSTANCE_FUNC(DanceGroupArcadeRoomManager)

public:
    unsigned int CreateAndEnterRoom( unsigned int nRoleID, const std::string &rRoomName, 
        int nScene, int nMusic, unsigned char nMusicMode, unsigned char nMusicLevel );

    bool Prepare( unsigned int nRoomID, unsigned char nPhoneOS, std::string &rCheckKey, std::string &rStage );
    void Start( unsigned int nRoomID );
    bool End( unsigned int nRoomID, unsigned int nRoleID, unsigned int &rScore, CRoomMarkInfo &rMark );

    void ReportRoundMark( unsigned int nRoomID, unsigned int nRoleID, const std::string &rMD5Code, 
        unsigned int nMark, char nKeyRank, unsigned char nRound, const std::vector<char> &rRoundRank );

private:
    bool IsValidRoomID( unsigned int nRoomID ) const;

    void RemoveRoom( unsigned int nRoomID );

    DanceBaseRoom* FindRoom( unsigned int nRoomID ) const;

    bool LoadStageInfo( DanceBaseRoom *pRoom, unsigned char nPhoneOS, std::string &rStage ) const;

private:
    unsigned int    m_nPreUsedRoomID;
    std::vector<DanceBaseRoom *> m_vectRooms;
    std::list<DanceBaseRoom *> m_listIdleRooms;
};

#endif // __DANCEGROUPARCADEROOM_H__

