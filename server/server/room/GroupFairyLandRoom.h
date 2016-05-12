#ifndef __GROUPSERVER_FAIRYLANDROOM_H__
#define __GROUPSERVER_FAIRYLANDROOM_H__

#include <vector>
#include "../roomLib/RoomLibBase.h"


class GameMsg_Base;
class LWRoomDanceInfo;
class LWRoomPosManager;
class RoomWholeInfo;
class RoomPlayerInfo;

#define FAIRY_LAND_ROOM_SERIAL_START 800000

// 舞团秘境房间
class GroupFairyLandRoom : public LWRoomBase
{
public: 
    GroupFairyLandRoom(unsigned int nRoomID);
    virtual ~GroupFairyLandRoom();


    virtual EQuitRoomRes OnQuitRoom(unsigned int nRoleID, char quitTo);
//    virtual void OnGetRoomList(GameMsg_C2S_GetRoomList &msg);
    virtual EEnterRoomRes OnTryEnterRoom(unsigned int nRoleID, char nRoomRoleType, char nPhoneOs);
//    virtual EEnterRoomRes OnEnterPlayer(GameMsg_C2S_EnterRoom &msg);
    virtual EKickPlayerRes OnKickPlayer(unsigned int nKickRole, char nKickedRoleRoomType, char nKickedRolePos, unsigned int nKickedRole);
    virtual EChangeRoleRoomStateRes OnChangeRoleRoomState(unsigned int nRoleID, char toState);
    virtual EChangeRoomPosStateRes OnChangeRoomPosState(unsigned int nRoleID, ERoleRoomType eRoleType, char roomPos, char roomState);
    virtual EPromoteRoomHost OnPromoteRoomHost(unsigned int nRoleID, char nNewHostRoomType, char nNewHostPos);

    virtual EStartRoomRes StartRoomDanceSpecialTest(unsigned int nRoleID);


    virtual LWRoomDanceInfo* GetRoomDanceInfo();
    virtual LWRoomPosManager* GetPosManager();

    virtual void ListenerRole(std::vector<std::pair<unsigned int, char> > &vecListner);

    // 房间启动回调
    virtual void OnRoomDanceAutoStart();
    virtual void OnRoomDanceAutoEnd();
    virtual void OnRoomDanceStartCallBack();
    virtual void OnRoomDanceEndCallBack();

    virtual void OnRoomDestoryCallBack();

    virtual unsigned int RoomSerial() const;


    void SetDanceGroup(unsigned int nDanceGroup);
    unsigned int GetDanceGroup() const;

public:
    LWRoomDanceInfo *m_pRoomDanceInfo;
    LWRoomPosManager *m_pRoomPosManager;

    unsigned int m_nChapterID;                      // 当前第几轮
    unsigned int m_nDanceGroup;                      // 房间的舞团ID
};


#endif // __GROUPSERVER_FAIRYLANDROOM_H__