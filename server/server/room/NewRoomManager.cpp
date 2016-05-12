#include "NewRoomManager.h"
#include "../../datastructure/RoleEntity.h"
#include "../../socket/Log.h"
#include "../../socket/GameMsg_Map.h"
#include "../logic/PlayerManager.h"
#include "../roomLib/RoomLibBase.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/GameMsgNewRoom.h"
#include "../roomLib/GameMsgRoomBroadcast.h"
#include "../player/GameMsg_PlayerSetting.h"
#include "../DanceGroupFairyLand/GameMsg_FairyLand.h"
#include "RoomComponent.h"


NewRoomManager::NewRoomManager()
    : GameMsg_Processor(true, false)
{

}

NewRoomManager::~NewRoomManager()
{

}


NewRoomManager &NewRoomManager::Instance()
{
    static NewRoomManager s;
    return s;
}

bool NewRoomManager::Init()
{
    return RegisterMessageMap();
}

bool NewRoomManager::RegisterMessageMap()
{
    // 创建房间
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewCreateRoom, &NewRoomManager::OnNewCreateRoom);
    RegMsgCreatorAndProcessor(GameMsg_S2C_NewCreateRoomSuc, &NewRoomManager::OnCreateRoomSucG2S);
    RegMsgCreatorAndProcessor(GameMsg_S2C_CreateRoomFail, &NewRoomManager::OnMsgClearRoom);

    // 进入房间成功会受到CreateRoomSucess消息
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewTryEnterRoom, &NewRoomManager::OnNewTryEnterRoom);
    RegMsgCreatorAndProcessor(GameMsg_S2C_EnterRoomFail, &NewRoomManager::OnMsgClearRoom);
    RegMsgCreatorAndProcessor(GameMsg_S2C_EnterPlayer, &NewRoomManager::FromGroupToClient);

    // 退出房间
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewQuitRoom, &NewRoomManager::FromClientToGroup);
    RegMsgCreatorAndProcessor(GameMsg_S2C_QuitRoomSuc, &NewRoomManager::OnMsgClearRoom);
    RegMsgCreatorAndProcessor(GameMsg_S2C_QuitRoomFail, &NewRoomManager::FromGroupToClient);

    // 房主转让
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewPromoteRoomHost, &NewRoomManager::FromClientToGroup);
    RegMsgCreatorAndProcessor(GameMsg_S2C_PromoteRoomHostSuc, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_PromoteRoomHostFail, &NewRoomManager::FromGroupToClient);

    // 踢人：
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewKickPlayer, &NewRoomManager::FromClientToGroup);
    RegMsgCreatorAndProcessor(GameMsg_S2C_QuitPlayer, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_KickPlayerFail, &NewRoomManager::FromGroupToClient);

    // 修改玩家状态：
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewChangeRoleRoomState, &NewRoomManager::FromClientToGroup);
    RegMsgCreatorAndProcessor(GameMsg_S2C_ChangeRoleRoomStateSuc, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_ChangeRoleRoomStateFail, &NewRoomManager::FromGroupToClient);

    // 开启/关闭房间中的位置
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewChangeRoomPosState, &NewRoomManager::FromClientToGroup);
    RegMsgCreatorAndProcessor(GameMsg_S2C_ChangeRoomPosStateSuc, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_ChangeRoomPosStateFail, &NewRoomManager::FromGroupToClient);

    // others.
    RegMsgCreatorAndProcessor(GameMsg_S2C_QuitMany, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetFairyLandRoomList, &NewRoomManager::FromClientToGroup);
    RegMsgCreatorAndProcessor(GameMsg_S2C_GetFairyLandRoomListResult, &NewRoomManager::FromGroupToClient);

    // startRoom:
    RegMsgCreatorAndProcessor(GameMsg_S2C_StartRoomFail, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_PrepareRoom, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_LoadingStartRoomProgress, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_PlayerDownloading, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_StartRoomSuc, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_SyncRoundMark, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_SyncEffectState, &NewRoomManager::FromGroupToClient);



    // sync player show info.
    RegMsgCreatorAndProcessor(GameMsg_S2C_RefreshItemForOther, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_RefreshEffectChangeToOthers, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_RefreshColorProgressToOthers, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_ChangeRoleNameResult, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_NotifyRoleVIPInfoChanged, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_UpdateDanceGroupName, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_UpdateDanceGroupTitle, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_SyncMedalInfoToOthers, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_SyncHorseData, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_AddGene, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_DelGene, &NewRoomManager::FromGroupToClient);
    RegMsgCreatorAndProcessor(GameMsg_S2C_ResetGene, &NewRoomManager::FromGroupToClient);



    return true;
}

void NewRoomManager::OnNewCreateRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);
    
    CRoleRoom *pRoom = pRole->GetComponent<CRoleRoom>();
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "can't find role[%u] room component", pRole->GetRoleID());

    ECreateRoomRes eRes = CreateRoom_Success;
    GameMsg_C2S_NewCreateRoom &msg = (GameMsg_C2S_NewCreateRoom &)rMsg;
    do 
    {
        if (pRole->InSomeRoom())
        {
            eRes = CreateRoom_InRoom;
            break;
        }

        // 锁定玩家房间：
        pRoom->GetGroupRoomInfo().LockRoom((ERoomType)msg.m_chRoomType);

        pRole->SendMsg2GroupServer(&msg);
    } while (0);


    if (eRes != CreateRoom_Success)
    {
        GameMsg_S2C_CreateRoomFail msgResponse;
        msgResponse.m_strError = RoomCommonFunc::CreateRoomErrorString(eRes);
        pRole->SendPlayerMsg(&msgResponse);
    }

    return;
}


void NewRoomManager::OnCreateRoomSucG2S(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);

    CRoleRoom *pRoom = pRole->GetComponent<CRoleRoom>();
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "can't find role[%u] room component", pRole->GetRoleID());
    ENSURE_WITH_LOG_CMD(pRoom->GetGroupRoomInfo().m_nRoomID == 0, return, "role[%u] group room id should 0");
    ENSURE_WITH_LOG_CMD(pRoom->GetGroupRoomInfo().m_roomType != eRoomType_None, return, "role[%u] group room type should not None");

    GameMsg_S2C_NewCreateRoomSuc &msg = (GameMsg_S2C_NewCreateRoomSuc &)rMsg;
    pRoom->GetGroupRoomInfo().m_nRoomID = msg.m_RoomInfo.m_nRoomID;
    
    pRole->SendPlayerMsg(&msg);
    return;
}

void NewRoomManager::OnNewTryEnterRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);

    CRoleRoom *pRoom = pRole->GetComponent<CRoleRoom>();
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "can't find role[%u] room component", pRole->GetRoleID());

    EEnterRoomRes eRes = EnterRoom_Success;
    GameMsg_C2S_NewTryEnterRoom &msg = (GameMsg_C2S_NewTryEnterRoom &)rMsg;

    do
    {
        if (pRole->InSomeRoom())
        {
            eRes = EnterRoom_InRoom;
            break;
        }

        // 锁定玩家房间：
        pRoom->GetGroupRoomInfo().LockRoom((ERoomType)msg.m_nRoomType);

        pRole->SendMsg2GroupServer(&msg);
    } while (0);

    if (eRes != EnterRoom_Success)
    {
        GameMsg_S2C_EnterRoomFail msgResponse;
        msgResponse.m_strError = RoomCommonFunc::EnterRoomErrorString(eRes);
        pRole->SendPlayerMsg(&msgResponse);
    }

}

// void NewRoomManager::OnEnterPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
// {
//     CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
//     ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);
// 
//     CRoleRoom *pRoom = pRole->GetComponent<CRoleRoom>();
//     ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "can't find role[%u] room component", pRole->GetRoleID());
//     ENSURE_WITH_LOG_CMD(pRoom->GetGroupRoomInfo().m_nRoomID == 0, return, "role[%u] group room id should 0");
//     ENSURE_WITH_LOG_CMD(pRoom->GetGroupRoomInfo().m_roomType != eRoomType_None, return, "role[%u] group room type should not None");
// 
//     GameMsg_S2C_EnterPlayer &msg = (GameMsg_S2C_EnterPlayer &)rMsg;
//     pRoom->GetGroupRoomInfo().m_nRoomID = ;
// 
//     pRole->SendPlayerMsg(&msg);
// 
//     return;
// }

void NewRoomManager::OnMsgClearRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);

    CRoleRoom *pRoom = pRole->GetComponent<CRoleRoom>();
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "can't find role[%u] room component", pRole->GetRoleID());

    pRoom->GetGroupRoomInfo().ClearRoom(); // 清空等待，创建失败

    pRole->SendPlayerMsg(&rMsg);

    return;
}


void NewRoomManager::FromGroupToClient(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);

    pRole->SendPlayerMsg(&rMsg);

    return;
}

void NewRoomManager::FromClientToGroup(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "can't find role entity by account[%u]", rMsg.nAccount);

    pRole->SendMsg2GroupServer(&rMsg);

    return;
}
