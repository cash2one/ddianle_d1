#ifndef __GROUPSERVER_GROUPFAIRYLANDROOM_H__
#define __GROUPSERVER_GROUPFAIRYLANDROOM_H__

#include <map>
#include "../../socket/GameMsg_Processor.h"
#include "../roomLib/RoomLibBase.h"

class GroupFairyLandRoomManager : public GameMsg_Processor, public LWRoomManager
{
public:
    GroupFairyLandRoomManager();
    virtual ~GroupFairyLandRoomManager();

    virtual bool Init();
    virtual bool RegisterMessage(); // 注册消息

    // 注册房间列表获取消息
    void OnGetRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    virtual ECreateRoomRes CreateRoom(unsigned int nCreateRole, const
        std::string &strRoomName, const std::string strRoomPwd, const char phoneOs); 
    virtual EQuitRoomRes QuitFromRoom(LWRoomBase *room, unsigned int nRoleID, char quitTo); // 退出房间
    virtual void OnRoleLogout(LWRoomBase *room, unsigned int nRoleID); // 玩家退出房间

    virtual void OutRole(LWRoomBase *room, unsigned int nRoleID, char quitTo, ERoomQuitType quitType);
    virtual void OnDestroyRoom(unsigned int nRoomID);

    virtual unsigned int RoomSerialToID(unsigned int nRoomSerial) const;

    void OnGroupDisband(unsigned int nDanceGroupID);

protected:
    virtual LWRoomBase *RoomFactory(unsigned int nRoomID);

    void AddRoom(unsigned int nGroupID, unsigned int nRoomID);
    void RemoveRoom(unsigned int nRoomID);

private:
    
// 管理房间列表
    typedef std::map< unsigned int, std::vector<unsigned int>* > GroupRoomMap; // [groupID, rooms[roomID]]
    GroupRoomMap m_dancegroupRoomMap;
};



#endif // __GROUPSERVER_GROUPFAIRYLANDROOM_H__