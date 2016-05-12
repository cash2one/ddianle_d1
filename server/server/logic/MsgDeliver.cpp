#include "MsgDeliver.h"
#include "PlayerManager.h"
#include "../../socket/Log.h"

MsgDeliver::MsgDeliver()
    : GameMsg_Processor(true, false)
{

}
MsgDeliver::~MsgDeliver()
{

}

MsgDeliver &MsgDeliver::Instance()
{
    static MsgDeliver s;
    return s;
}

bool MsgDeliver::Init()
{
    // do nothing.
    return true;
}


void MsgDeliver::FromGroupToClient(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);

    pRole->SendPlayerMsg(&rMsg);

    return;
}

void MsgDeliver::FromClientToGroup(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);

    pRole->SendMsg2GroupServer(&rMsg);

    return;
}
