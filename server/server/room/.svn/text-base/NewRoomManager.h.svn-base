#ifndef __GAMESERVER_NEWROOMMANAGER_H__
#define __GAMESERVER_NEWROOMMANAGER_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../socket/GameMsg_Base.h"

class NewRoomManager : public GameMsg_Processor
{
public:
    NewRoomManager();
    ~NewRoomManager();

    static NewRoomManager &Instance();
    bool Init();
    
    bool RegisterMessageMap();

    // 创建房间， 创建房间成功，创建房间失败
    void OnNewCreateRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnCreateRoomSucG2S(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

//    OnMsgClearRoom
//    void OnCreateRoomFailG2S(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    // 进入房间,进入房间成功，进入房间失败
    void OnNewTryEnterRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
//    OnMsgClearRoom
// 进入房间成功发送CreateRoomSucess消息
    
    // 离开房间, 
    // FromClientToGroup
    // OnMsgClearRoom
    // FromGroupToClient

    // 房主转让
    // 透传

    // 踢人
    // 透传

    // 修改玩家状态


    // 修改玩家位置


    

    // 清除房间信息，然后透传客户端
    void OnMsgClearRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer); //
    // 透传group到client的消息
    void FromGroupToClient(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    // 透传client 到 group.
    void FromClientToGroup(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

public:


};



#endif // __GAMESERVER_NEWROOMMANAGER_H__



