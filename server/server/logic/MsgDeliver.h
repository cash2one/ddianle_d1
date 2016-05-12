#ifndef __GAMESERVER_MSGDELIVER_H__
#define __GAMESERVER_MSGDELIVER_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../socket/GameMsg_Base.h"

// GameMsg ��Ϣ͸����

class MsgDeliver : public GameMsg_Processor
{
public:
    MsgDeliver();
    ~MsgDeliver();

    static MsgDeliver &Instance();
    bool Init();

    // ͸������
    void FromGroupToClient(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void FromClientToGroup(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
};


#endif // __GAMESERVER_MSGDELIVER_H__

