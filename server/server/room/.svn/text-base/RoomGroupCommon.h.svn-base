#ifndef __GROUPSERVER_ROOMGROUPCOMMON_H__
#define __GROUPSERVER_ROOMGROUPCOMMON_H__


#include "../../datastructure/Macro_Define.h"

class LWRoomBase;
class GameMsg_Base;
class LWRoomDanceInfo;
class LWRoomPosManager;
class LWRoomManager;
class RoomPlayerInfo;
class RoomWholeInfo;

namespace RoomGroupCommon
{
    void SendMsgToAll(LWRoomBase *pRoom, GameMsg_Base &msg, unsigned int nExpectRole);
    
    // 使用roomDanceInfo 和 posManager的 房间可以使用这个退出
    void OutDancerRole(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager, 
        unsigned int nKickedRole, char quitTo, ERoomQuitType quitType, unsigned int nKickOperator);


    void EncodeWholeRoomInfo(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager, 
        RoomWholeInfo &rWholeInfo, bool isHost);
    // 通用
    bool EncodePlayerInfo(unsigned int nRoleID, bool isHost, char nRoleType, char nRolePos, char nRoleState, EDanceColorTeam team, RoomPlayerInfo &roomPlayrInfo);


    // 
    void OnRoomDanceAutoStart(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager);
    void OnRoomDanceAutoEnd(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager);
    
    // 意外销毁房间
    void QuitAllDanceRoomRole(LWRoomBase *pRoom, LWRoomDanceInfo *pRoomDanceInfo, LWRoomPosManager *pRoomPosManager);
};


#endif // __GROUPSERVER_ROOMGROUPCOMMON_H__