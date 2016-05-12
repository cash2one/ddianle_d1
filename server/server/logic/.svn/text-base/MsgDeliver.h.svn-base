#ifndef __GAMESERVER_MSGDELIVER_H__
#define __GAMESERVER_MSGDELIVER_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../socket/GameMsg_Base.h"

// GameMsg 消息透传类

class MsgDeliver : public GameMsg_Processor
{
public:
    MsgDeliver();
    ~MsgDeliver();

    static MsgDeliver &Instance();
    bool Init();

    // 透传函数
    void FromGroupToClient(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void FromClientToGroup(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
};


#endif // __GAMESERVER_MSGDELIVER_H__

