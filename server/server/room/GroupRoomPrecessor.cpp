#include "GroupRoomPrecessor.h"
#include "RoomGroupCommon.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/SlotPeer.h"
#include "../../socket/Log.h"
#include "../../socket/Formatter.h"
#include "../../socket/Md5.h"

#include "../group/Global.h"

#include "../roomLib/GameMsg_C2S_RoomMsg.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/RoomDanceInfo.h"
#include "../roomLib/RoomPosManager.h"
#include "../roomLib/RoomLibBase.h"
#include "../roomLib/GameMsgNewRoom.h"
#include "../roomLib/RoomSettingMgr.h"
#include "../roomLib/GameMsgRoomBroadcast.h"
#include "../player/GameMsg_PlayerSetting.h"

#include "../Medal/MedalCfgMgr.h"
#include "../roomLib/GameMsgRoomBroadcast.h"

#include "GroupFairyLandRoomManager.h"


#define GetRoomInfoByRoleID(eRT, nRoleID, pRoleEnt, pRoomBase, cmd)\
CGroupRoleEntity *pRoleEnt = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);\
ENSURE_WITH_LOG_CMD(NULL != pRoleEnt, cmd, "miss group on line role[%u] entity.", nRoleID);\
ENSURE_WITH_LOG_CMD(pRoleEnt->GetGroupRoomType() == eRT, cmd, "error role[%u] room type[%d,%d] on GetRoleRoom", nRoleID, (int)pRoleEnt->GetGroupRoomType(), (int)eRT);\
LWRoomManagerPtr roomPtr = GetRoomManager(eRT);\
ENSURE_WITH_LOG_CMD(NULL != roomPtr, cmd, "miss room manage type[%d] ", (int)eRT);\
LWRoomBase* pRoomBase = roomPtr->GetRoom(pEnt->GetGroupRoomID()()) 

#define GetRoomInfoByAccount(eRT, nAccount, pRoleEnt, pRoomBase, cmd)\
CGroupRoleEntity *pRoleEnt = CGroupPlayerManager::Instance().SearchRoleByAccountID(nAccount);\
ENSURE_WITH_LOG_CMD(NULL != pRoleEnt, cmd, "miss group on line account[%u] entity.", nAccount);\
ENSURE_WITH_LOG_CMD(pRoleEnt->GetGroupRoomType() == eRT, cmd, "error account[%u] room type[%d,%d] on GetRoleRoom", nAccount, (int)pRoleEnt->GetGroupRoomType(), (int)eRT);\
LWRoomManagerPtr roomPtr = GetRoomManager(eRT);\
ENSURE_WITH_LOG_CMD(NULL != roomPtr, cmd, "miss room manage type[%d] ", (int)eRT);\
LWRoomBase* pRoomBase = roomPtr->GetRoom(pRoleEnt->GetGroupRoomID())


#define GetRoomInfoByAccountNoRoomType(nAccount, pRoleEnt, pRoomBase, cmd)\
CGroupRoleEntity *pRoleEnt = CGroupPlayerManager::Instance().SearchRoleByAccountID(nAccount);\
ENSURE_WITH_LOG_CMD(NULL != pRoleEnt, cmd, "miss group on line account[%u] entity.", nAccount);\
LWRoomManagerPtr roomPtr = GetRoomManager(pRoleEnt->GetGroupRoomType());\
ENSURE_WITH_LOG_CMD(NULL != roomPtr, cmd, "miss room manage type[%d] ", (int)pRoleEnt->GetGroupRoomType());\
LWRoomBase* pRoomBase = roomPtr->GetRoom(pRoleEnt->GetGroupRoomID())

GroupRoomProcessor::GroupRoomProcessor()
    : GameMsg_Processor(true, false)
{
    memset(m_roomManagerArry, 0, eRoomType_Max*sizeof(LWRoomManagerPtr));

    m_roomManagerArry[eRoomType_FairyLandRoom] = new GroupFairyLandRoomManager(); // 创建一个秘境的房间管理器
}

GroupRoomProcessor::~GroupRoomProcessor()
{
    ClearRoomManager(); 
}

GroupRoomProcessor& GroupRoomProcessor::Instance()
{
    static GroupRoomProcessor s;
    return s;
}

// 初始化
bool GroupRoomProcessor::Init()
{
    RegisterMessage();
    for (int i = 0; i < (int)ARRY_LENGTH(m_roomManagerArry); ++i)
    {
        if (NULL != m_roomManagerArry[i])
        {
            m_roomManagerArry[i]->Init();
        }
    }

    return true;
}

// 更新
void GroupRoomProcessor::OnUpdate(time_t tNow)
{ 
    for (int i = 0; i < (int)ARRY_LENGTH(m_roomManagerArry); ++i)
    {
        if (NULL != m_roomManagerArry[i])
        {
            m_roomManagerArry[i]->OnUpdate(tNow);
        } 
    }
}

void GroupRoomProcessor::RegisterMessage()
{
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewCreateRoom, &GroupRoomProcessor::OnCreateRoom);
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewQuitRoom, &GroupRoomProcessor::OnQuitRoom);
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewTryEnterRoom, &GroupRoomProcessor::OnTryEnterRoom);
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewKickPlayer, &GroupRoomProcessor::OnKickPlayer);
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewChangeRoleRoomState, &GroupRoomProcessor::OnChangeRoleRoomState);
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewChangeRoomPosState, &GroupRoomProcessor::OnChangeRoomPosState);
    RegMsgCreatorAndProcessor(GameMsg_C2S_NewPromoteRoomHost, &GroupRoomProcessor::OnPromoteRoomHost);

    // 跳舞消息：
    RegMsgCreatorAndProcessor(GameMsg_C2S_StartRoom, &GroupRoomProcessor::OnStartRoom);
    RegMsgCreatorAndProcessor(GameMsg_C2S_LoadingStartRoomProgress, &GroupRoomProcessor::OnLoadingStartGameProgress);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PlayerDownloading, &GroupRoomProcessor::OnSyncPlayerDownloading);

    RegMsgCreatorAndProcessor(GameMsg_C2S_ReadyRoom, &GroupRoomProcessor::OnReadyRoom);
    RegMsgCreatorAndProcessor(GameMsg_C2S_ReportRoundMark, &GroupRoomProcessor::OnReportRoundMark);
    RegMsgCreatorAndProcessor(GameMsg_C2S_ReportEffectChange, &GroupRoomProcessor::OnReportEffectChange);
    RegMsgCreatorAndProcessor(GameMsg_C2S_EndRoom, &GroupRoomProcessor::OnEndRoom);


    // 玩家可视数据更新消息：
    RegMsgCreatorAndProcessor(GameMsg_S2G_TransFromIDUpdate, &GroupRoomProcessor::OnTransFromIDUpdate);
    RegMsgCreatorAndProcessor(GameMsg_S2G_OnEquipNotify, &GroupRoomProcessor::OnOnEquipNotify);
    RegMsgCreatorAndProcessor(GameMsg_S2G_UnEquipNotify, &GroupRoomProcessor::OnUnEquipNotify);
    RegMsgCreatorAndProcessor(GameMsg_S2G_EffectChangeNotify, &GroupRoomProcessor::OnEffectChangeNotify);
    RegMsgCreatorAndProcessor(GameMsg_S2G_RefreshSevenColor, &GroupRoomProcessor::OnRefreshSevenColor);
    RegMsgCreatorAndProcessor(GameMsg_S2G_RefreshGeneNotify, &GroupRoomProcessor::OnRefreshGeneNotify);
    RegMsgCreatorAndProcessor(GameMsg_S2G_AddMedals, &GroupRoomProcessor::OnAddMedals);
    RegMsgCreatorAndProcessor(GameMsg_S2C_SyncHorseData, &GroupRoomProcessor::OnSyncHorseData);

    return;
}

// get room manager pointer for room type.
LWRoomManager* GroupRoomProcessor::GetRoomManager(ERoomType eRT)
{
    if (eRT < eRoomType_Max)
        return m_roomManagerArry[eRT];

    return NULL;
}

// use room type and role id to get role room potiner.
LWRoomBase* GroupRoomProcessor::GetRoleRoom(ERoomType eRT, unsigned int nRoleID)
{
    CGroupRoleEntity *pEnt = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != pEnt, return NULL, "miss group on line role[%u] entity.", nRoleID);

    ENSURE_WITH_LOG_CMD(pEnt->GetGroupRoomType() == eRT, return NULL, "error role[%u] room type[%d,%d] on GetRoleRoom", nRoleID, (int)pEnt->GetGroupRoomType(), (int)eRT);

    LWRoomManagerPtr roomPtr = GetRoomManager(eRT);
    ENSURE_WITH_LOG_CMD(NULL != roomPtr, return NULL, "miss room manage type[%d] ", (int)eRT);

    return roomPtr->GetRoom(pEnt->GetGroupRoomID()); // 取得这个房间
}

LWRoomBase* GroupRoomProcessor::GetRoleRoom(CGroupRoleEntity *pRoleEntity)
{
    if (NULL == pRoleEntity)
        return NULL;

    if (pRoleEntity->GetGroupRoomType() == eRoomType_None)
        return NULL;

    LWRoomManager *pRoomMgr = GetRoomManager(pRoleEntity->GetGroupRoomType());
    if (NULL == pRoomMgr)
        return NULL;
    
    return pRoomMgr->GetRoom(pRoleEntity->GetGroupRoomID());
}

void GroupRoomProcessor::OnRoleLogOut(CGroupRoleEntity *pGroupRole)
{
    if (NULL != pGroupRole && pGroupRole->GetGroupRoomType() != eRoomType_None)
    {
        LWRoomManager *pManager = GetRoomManager(pGroupRole->GetGroupRoomType());
        ENSURE_WITH_LOG_CMD(NULL != pManager, return, "error to get room manager[%d]", (int)pGroupRole->GetGroupRoomType());

        LWRoomBase *pRoom = pManager->GetRoom(pGroupRole->GetGroupRoomID());
        if (NULL != pRoom)
            pManager->OnRoleLogout(pRoom, pGroupRole->GetRoleID());

        pGroupRole->ClearGroupRoom(); // 清理这个人的房间
    }

}

void GroupRoomProcessor::OnRoleQuitFromDanceGroup(CGroupRoleEntity *pGroupRole)
{
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get null group role entity.");
    if (pGroupRole->GetGroupRoomID() == 0)
        return; // 不在房间中
    LWRoomManager *pRoomMgr = GetRoomManager(pGroupRole->GetGroupRoomType());
    if (NULL == pRoomMgr)
        return; // 不在房间中

    if (pGroupRole->GetGroupRoomType() == eRoomType_FairyLandRoom)
    { // 秘境房间有舞团要求：
        LWRoomBase *pRoom = pRoomMgr->GetRoom(pGroupRole->GetGroupRoomID());
        pRoomMgr->OutRole(pRoom, pGroupRole->GetRoleID(), RoomQuitTo_World, RoomQuitType_OutDanceGroup); // 
    }

    return;
}

void GroupRoomProcessor::OnDanceGroupDisband(unsigned int nGroupID)
{

    { // 秘境房间有舞团要求：
        GroupFairyLandRoomManager *pFairyLandRoomMgr = (GroupFairyLandRoomManager *)GetRoomManager(eRoomType_FairyLandRoom);
        pFairyLandRoomMgr->OnGroupDisband(nGroupID);
    }



    return;
}



void GroupRoomProcessor::OnCreateRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_NewCreateRoom &msg = (GameMsg_C2S_NewCreateRoom &)rMsg;
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", msg.nAccount);

    // GameServer 会锁定玩家申请进入的状态，因此，不管成功与否都要返回玩家状态
    ECreateRoomRes createRes = CreateRoom_Unknow;
    // 做通用的检测：
    do 
    {
        LWRoomManagerPtr pRoomMgr = GetRoomManager((ERoomType)msg.m_chRoomType);
        if (NULL == pRoomMgr)
            break;

        createRes = pRoomMgr->CreateRoom(pGroupRole->GetRoleID(), msg.m_strRoomName, msg.m_strRoomPwd, msg.m_chPhoneOS);
        if (CreateRoom_Success != createRes)
            break;

    } while (0);

    if (CreateRoom_Success != createRes)
    { // 如果失败，必须返回消息，解除GameServer中玩家的房间锁定状态.
        GameMsg_S2C_CreateRoomFail msg;
        msg.m_strError = RoomCommonFunc::CreateRoomErrorString(createRes);
        pGroupRole->SendPlayerMsg(&msg);
    }
    
    return;
}

void GroupRoomProcessor::OnQuitRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_NewQuitRoom& msg = (GameMsg_C2S_NewQuitRoom&)rMsg;
    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupRole, pRoomBase, return);
    
    EQuitRoomRes eRes = QuitRoom_NoRoom;
    do 
    {
        LWRoomManager* pRoomManger = GetRoomManager(pGroupRole->GetGroupRoomType());
        if (NULL == pRoomManger)
            break;
        
        if (NULL == pRoomBase)
        {
            eRes = QuitRoom_NoRoom; // 没有房间
            break;
        }

        eRes = pRoomManger->QuitFromRoom(pRoomBase, pGroupRole->GetRoleID(), msg.m_chQuitTo);
    } while (0);

    if (QuitRoom_Success != eRes)
    {
        GameMsg_S2C_QuitRoomFail quitMsg;
        quitMsg.m_strError = RoomCommonFunc::QuitRoomErrorString(eRes);
        pGroupRole->SendPlayerMsg(&quitMsg); // 
    }

    return;
}

// void GroupRoomProcessor::OnGetRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
// {
//     return;
// }

void GroupRoomProcessor::OnTryEnterRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_NewTryEnterRoom &msg = (GameMsg_C2S_NewTryEnterRoom &)rMsg;
    
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role ent by acount[%u].", rMsg.nAccount);

    EEnterRoomRes enterRes = EnterRoom_Unknow;

    do 
    {
        LWRoomManager *pRoomManager = GetRoomManager((ERoomType)msg.m_nRoomType);
        if (NULL == pRoomManager)
            break;

        LWRoomBase *pRoom = pRoomManager->GetRoom(pRoomManager->RoomSerialToID(msg.m_nRoomID)); // 找到这个房间
        if (NULL == pRoom)
        {
            enterRes = EnterRoom_NoRoom;
            break;
        }

        enterRes = pRoom->OnTryEnterRoom(pGroupRole->GetRoleID(), msg.m_nRoleRoomType, msg.m_chPhoneOS);

    } while (0);

    if (enterRes != EnterRoom_Success)
    {
        GameMsg_S2C_EnterRoomFail msg;
        msg.m_strError = RoomCommonFunc::EnterRoomErrorString(enterRes);
        pGroupRole->SendPlayerMsg(&msg);
    }

    return;
}


void GroupRoomProcessor::OnKickPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_NewKickPlayer &msg = (GameMsg_C2S_NewKickPlayer &)rMsg;
    
    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupHostRole, pRoom, return);
    EKickPlayerRes eRes = KickPlayer_Success;

    do 
    {
        if (NULL == pRoom)
        {
            eRes = KickPlayer_NoRoom;
            break;
        }

        // 直接调用房间踢人函数
        eRes = pRoom->OnKickPlayer(pGroupHostRole->GetRoleID(), msg.m_chRoleRoomType, msg.m_chRoleRoomPos, msg.m_nRoleID);
    } while (0);
    
    if (eRes != KickPlayer_Success)
    {
        GameMsg_S2C_KickPlayerFail retMsg;
        retMsg.m_strError = RoomCommonFunc::KickPlayerErrorString(eRes);
        pGroupHostRole->SendPlayerMsg(&retMsg);
    }

    return;
}


// void GroupRoomProcessor::OnInEquip(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
// {
// 
//     return;
// }

// void GroupRoomProcessor::OnOutEquip(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
// {
// 
//     return;
// }
// 

void GroupRoomProcessor::OnChangeRoomInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{ 
    // 暂时不需要
    return;
}

void GroupRoomProcessor::OnChangeMusicInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    // 暂时不需要
    return;
}


void GroupRoomProcessor::OnChangeRoleRoomType(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    // 暂时不需要
    return;
}

void GroupRoomProcessor::OnChangeRoleRoomState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_NewChangeRoleRoomState &msg = (GameMsg_C2S_NewChangeRoleRoomState &)rMsg;

    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupHostRole, pRoom, return);
    EChangeRoleRoomStateRes eRes = ChangeRoleRoomState_Unknown;

    do
    {
        if (NULL == pRoom || pRoom->GetType() != msg.m_chRoomType)
        {
            eRes = ChangeRoleRoomState_NoRoom;
            break;
        }

        eRes = pRoom->OnChangeRoleRoomState(pGroupHostRole->GetRoleID(), msg.m_chRoleRoomState);
    } while (0);

    if (eRes != ChangeRoleRoomState_Success)
    {
        GameMsg_S2C_ChangeRoleRoomStateFail retMsg;
        retMsg.m_strError = RoomCommonFunc::ChangeRoleRoomStateErrorString(eRes);
        pGroupHostRole->SendPlayerMsg(&retMsg);
    }

    return;
}


void GroupRoomProcessor::OnChangeRoomPosState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_NewChangeRoomPosState &msg = (GameMsg_C2S_NewChangeRoomPosState &)rMsg;

    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupHostRole, pRoom, return);
    EChangeRoomPosStateRes eRes = ChangeRoomPosState_NoRoom;

    do
    {
        if (NULL == pRoom)
        {
            eRes = ChangeRoomPosState_NoRoom;
            break;
        }

        eRes = pRoom->OnChangeRoomPosState(pGroupHostRole->m_nRoleID, (ERoleRoomType)msg.m_chRoleRoomType, msg.m_chRoleRoomPos, msg.m_chRoomPosState);
    } while (0);

    if (eRes != ChangeRoomPosState_Success)
    {
        GameMsg_S2C_ChangeRoomPosStateFail retMsg;
        retMsg.m_strError = RoomCommonFunc::ChangeRoomPosStateErrorString(eRes);
        pGroupHostRole->SendPlayerMsg(&retMsg);
    }

    return;
}

void GroupRoomProcessor::OnPromoteRoomHost(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_NewPromoteRoomHost &msg = (GameMsg_C2S_NewPromoteRoomHost &)rMsg;

    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupHostRole, pRoom, return);
    EPromoteRoomHost eRes = PromoteRoomHost_UnKnow;

    do
    {
        if (NULL == pRoom)
        {
            eRes = PromoteRoomHost_OldHostNotInRoom;
            break;
        }

        eRes = pRoom->OnPromoteRoomHost(pGroupHostRole->GetRoleID(), msg.m_chNewHostRoleRoomType, msg.m_chNewHostRoleRoomPos);
    } while (0);

    if (eRes != PromoteRoomHost_Sucess)
    {
        GameMsg_S2C_PromoteRoomHostFail retMsg;
        retMsg.m_strError = RoomCommonFunc::PromoteRoomHostErrorString(eRes);
        pGroupHostRole->SendPlayerMsg(&retMsg);
    }

    return;
}

void GroupRoomProcessor::OnQuitMarkAward(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{

    return;
}


void GroupRoomProcessor::OnStartRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_StartRoom &msg = (GameMsg_C2S_StartRoom &)rMsg;

    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    if (NULL == pGroupRole)
        return;

    EStartRoomRes eRet = StartRoom_Success;
    do 
    { // 持有room和entity.

        LWRoomManager *pRoomMgr = GetRoomManager((ERoomType)msg.m_chRoomType);
        if (NULL == pRoomMgr)
        {
            eRet = StartRoom_Unknown;
            break;
        }

        LWRoomBase *pRoom = pRoomMgr->GetRoom(pGroupRole->GetGroupRoomID());
        if (NULL == pRoom)
        {
            eRet = StartRoom_NoRoom;
            break;
        }

        LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
        if (NULL == pDanceInfo)
        { // 不支持跳舞的房间
            eRet = StartRoom_NoDanceRoom;
            break;
        }

        // 是否房主 StartRoom_NotHost
        if (pRoom->HostRoleID() != pGroupRole->GetRoleID())
        {
            eRet = StartRoom_NotHost;
            break; // 不是房主
        }

        // StartRoom_HasStart
        if (pDanceInfo->GetState() != eRoomState_Idle)
        {
            eRet = StartRoom_HasStart; // 已经开始了。
            break;
        }

        // 特殊条件要检测 (歌曲，活动时间,设置)
        if (StartRoom_Success != (eRet = pRoom->StartRoomDanceSpecialTest(pGroupRole->GetRoleID())))
        {
            break;
        }

        if (StartRoom_Success != (eRet = pDanceInfo->StartRoomDance(pGroupRole->GetRoleID())))
        {
            break;
        }

        std::vector<char> vecAndroidStage;
        std::vector<char> vecIosStage;
        std::vector<char> vecWinPhoneStage;
        if (!pDanceInfo->LoadStage(vecAndroidStage, vecIosStage, vecWinPhoneStage))
        {
            pRoomMgr->DestroyRoom(pRoom->GetOriRoomID()); // 
            return;// 房间相关数据已经被销毁，必须返回
        }

        std::vector<std::pair<unsigned int, char> > vecListener;
        pRoom->ListenerRole(vecListener); //

        GameMsg_S2C_PrepareRoom msgResponse;
        msgResponse.m_chScene = (char)pDanceInfo->GetPlayScene();
        msgResponse.m_nMusic = (short)pDanceInfo->GetPlayMusic();
        msgResponse.m_chMode = (char)pDanceInfo->GetPlayMusicMode();
        msgResponse.m_chLevel = (char)pDanceInfo->GetPlayMusicLevel();
        msgResponse.m_strCheckKey = pDanceInfo->GetCheckCode();

        for (size_t i = 0; i < vecListener.size(); ++i)
        { // 广播房间玩家，prepar over.
            CGroupRoleEntity* pGroupRoleBrod = CGroupPlayerManager::Instance().SearchRoleByID(vecListener[i].first);
            ENSURE_WITH_LOG_CMD(NULL != pGroupRoleBrod, continue, "group role[%u] entity ptr should not null on broad room prepar info...", vecListener[i].first);
            
            msgResponse.m_vecStage.clear();
            if (vecListener[i].second == OS_Ios)
            {
                msgResponse.m_vecStage.assign(vecIosStage.begin(), vecIosStage.end());
            }
            else if (vecListener[i].second == OS_WP8)
            {
                msgResponse.m_vecStage.assign(vecWinPhoneStage.begin(), vecWinPhoneStage.end());
            }
            else
            { // OS_Android
                msgResponse.m_vecStage.assign(vecAndroidStage.begin(), vecAndroidStage.end());
            }
            
            // 广播玩家信息
            pGroupRoleBrod->SendPlayerMsg(&msgResponse); 
        }

    } while (0);

    if (StartRoom_Success != eRet)
    { // start room failed. // 失败的情形
        GameMsg_S2C_StartRoomFail msg;
        msg.m_strError = RoomCommonFunc::StartRoomErrorString(eRet);
        pGroupRole->SendPlayerMsg(&msg);
    }

    return;
}

void GroupRoomProcessor::OnLoadingStartGameProgress(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    GameMsg_C2S_LoadingStartRoomProgress &msg = (GameMsg_C2S_LoadingStartRoomProgress &)rMsg;

    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupRole, pRoom, return);
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error to get role[%u] room on group loading progress.", pGroupRole->GetRoleID());
    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
    ENSURE_WITH_LOG_CMD(NULL != pDanceInfo, return, "not find dance info in group loading progress");

    if (pDanceInfo->GetState() == eRoomState_Preparing || pDanceInfo->GetState() == eRoomState_Waiting)
    { 
        pDanceInfo->OnLoadingStartGameProgress(pGroupRole->GetRoleID(), msg.m_nRate);
        GameMsg_S2C_LoadingStartRoomProgress msgLoadingSend;
        msgLoadingSend.m_nRoleID = pGroupRole->GetRoleID();
        msgLoadingSend.m_nRate = msg.m_nRate >= 100 ? 100 : msg.m_nRate;
        // 这个特殊，是广播所有人
        RoomGroupCommon::SendMsgToAll(pRoom, msgLoadingSend, 0);
    }

    return; 
}

void GroupRoomProcessor::OnSyncPlayerDownloading(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    GameMsg_C2S_PlayerDownloading &msg = (GameMsg_C2S_PlayerDownloading &)rMsg;

    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupRole, pRoom, return);
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error to get role[%u] room on group loading progress.", pGroupRole->GetRoleID());
    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
    ENSURE_WITH_LOG_CMD(NULL != pDanceInfo, return, "not find dance info in group loading progress");

    if (pDanceInfo->GetState() == eRoomState_Preparing || pDanceInfo->GetState() == eRoomState_Waiting)
    {
        GameMsg_S2C_PlayerDownloading msgDownLoading;
        msgDownLoading.m_nRoleID = pGroupRole->GetRoleID();
        msgDownLoading.m_nProgress = msg.m_nProgress;
        // 这个特殊，是广播所有人
        RoomGroupCommon::SendMsgToAll(pRoom, msgDownLoading, 0);
    }

    return;
}

void GroupRoomProcessor::OnReadyRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_ReadyRoom &msg = (GameMsg_C2S_ReadyRoom &)rMsg; 

    GetRoomInfoByAccount(((ERoomType)msg.m_chRoomType), rMsg.nAccount, pGroupRole, pRoom, return);
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error to get role[%u] room on group Ready room.", pGroupRole->GetRoleID());
    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
    ENSURE_WITH_LOG_CMD(NULL != pDanceInfo, return, "not find dance info in Group ready room");

    if (pDanceInfo->GetState() == eRoomState_Preparing || pDanceInfo->GetState() == eRoomState_Waiting) // 
    {
        pDanceInfo->OnLoadComplete(pGroupRole->GetRoleID());

        // test room if all dancer is ready.
        if (pDanceInfo->CheckAllRoleState(RoleRoomState_ToStart))
        { // in case all dancer is ready.
            // 移出等待开启列表
            GetRoomManager(pGroupRole->GetGroupRoomType())->RemoveFromWaitStartList(pRoom->GetOriRoomID());
            pRoom->OnRoomDanceStartCallBack();
            // 开启
            GameMsg_S2C_StartRoomSuc msgResponse;
            RoomGroupCommon::SendMsgToAll(pRoom, msgResponse, 0);
        }
        else
        { // 添加到消息处理器中
            GetRoomManager(pGroupRole->GetGroupRoomType())->AddToWaitStartList(RoomSettingMgr::Instance().RoomStartWait() + time(NULL), pRoom->GetOriRoomID());
        }
    }

    return; // 等待结束
}

void GroupRoomProcessor::OnReportRoundMark(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_ReportRoundMark &msg = (GameMsg_C2S_ReportRoundMark &)rMsg;

    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupRole, pRoom, return);
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error to get role[%u] room on report round mark.", pGroupRole->GetRoleID());
    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
    ENSURE_WITH_LOG_CMD(NULL != pDanceInfo, return, "not find dance info on report round mark");
    LWDanceRoleInfo *pRoleDanceInfo = pDanceInfo->GetDanceRole(pGroupRole->GetRoleID());
    ENSURE_WITH_LOG_CMD(NULL != pRoleDanceInfo, return, "not find dance role[%u] info on report round mark", pGroupRole->GetRoleID());
    LWRoomPosManager *pPosManager = pRoom->GetPosManager();
    ENSURE_WITH_LOG_CMD(NULL != pPosManager, return, "not find dance pos manager in group report RoundMark.");
    if (!pDanceInfo->IsPlaying())
        return; // 

    ERoleRoomType eRt;
    unsigned int pos;
    if (!pPosManager->GetRolePos(pGroupRole->GetRoleID(), eRt, pos))
        return;

    pDanceInfo->OnReportRoundMark(pGroupRole->GetRoleID(), msg.m_strMD5Code, msg.m_nMark, msg.m_chKeyRank, msg.m_nRound, msg.m_vecRoundRank);

    GameMsg_S2C_SyncRoundMark msgResponse;
    msgResponse.m_chDancerPos = (char)pos;
    msgResponse.m_chRound = (char)pRoleDanceInfo->GetMarkInfo().Round();
    msgResponse.m_chRank = (char)pRoleDanceInfo->GetMarkInfo().LastRank();//for model3 ,this is AllComboFlag; > 1 = AllCombo;
    msgResponse.m_nMark = pRoleDanceInfo->GetMarkInfo().Mark();
    msgResponse.m_nSpecial = pRoleDanceInfo->GetMarkInfo().LastSpecialCount();
    RoomGroupCommon::SendMsgToAll(pRoom, msgResponse, pGroupRole->GetRoleID());

    return;
}

void GroupRoomProcessor::OnReportEffectChange(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_ReportEffectChange &msg = (GameMsg_C2S_ReportEffectChange &)rMsg;

    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupRole, pRoom, return);
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error to get role[%u] room on report effect change.", pGroupRole->GetRoleID());
    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
    ENSURE_WITH_LOG_CMD(NULL != pDanceInfo, return, "not find dance info on report effect change");
    LWDanceRoleInfo *pRoleDanceInfo = pDanceInfo->GetDanceRole(pGroupRole->GetRoleID());
    ENSURE_WITH_LOG_CMD(NULL != pRoleDanceInfo, return, "not find dance role[%u] info on report effect change", pGroupRole->GetRoleID());
    LWRoomPosManager *pPosManager = pRoom->GetPosManager();
    ENSURE_WITH_LOG_CMD(NULL != pPosManager, return, "not find dance pos manager in group report RoundMark.");

    if (!pDanceInfo->IsPlaying())
        return;

    ERoleRoomType eRt;
    unsigned int pos;
    if (pPosManager->GetRolePos(pGroupRole->GetRoleID(), eRt, pos))
        return;

    std::string strSrcCode;
    Formatter(strSrcCode) << pDanceInfo->GetCheckCode() << msg.m_nMark
        << msg.m_nSpecail << (int)msg.m_chRound;

    unsigned char szMD5Code[16] = { 0 };
    MDString(strSrcCode.c_str(), szMD5Code);

    std::string strCheckCode = BinaryData2String(szMD5Code, sizeof(szMD5Code));
    if (msg.m_strMD5Code.compare(strCheckCode) == 0)
    { // md5 compare sucess .
        GameMsg_S2C_SyncEffectState msgResponse;
        msgResponse.m_nMark = msg.m_nMark;
        msgResponse.m_nSpecial = msg.m_nSpecail;
        msgResponse.m_chRound = msg.m_chRound;
        msgResponse.m_nDancerPos = (char)pos;

        RoomGroupCommon::SendMsgToAll(pRoom, msgResponse, pGroupRole->GetRoleID());
    }

    return;
}

void GroupRoomProcessor::OnEndRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
//    GameMsg_C2S_EndRoom &msg = (GameMsg_C2S_EndRoom &)rMsg;

    GetRoomInfoByAccountNoRoomType(rMsg.nAccount, pGroupRole, pRoom, return);
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error to get role[%u] room on group Ready room.", pGroupRole->GetRoleID());
    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
    ENSURE_WITH_LOG_CMD(NULL != pDanceInfo, return, "not find dance info in Group ready room");
    LWDanceRoleInfo *pRoleDanceInfo = pDanceInfo->GetDanceRole(pGroupRole->GetRoleID());
    ENSURE_WITH_LOG_CMD(NULL != pRoleDanceInfo, return, "not find dance role[%u] info on report round mark", pGroupRole->GetRoleID());

    if (!pDanceInfo->IsPlaying() && pDanceInfo->GetState() != eRoomState_Ending)
        return;

    LWRoomManagerPtr mgr = GetRoomManager(pGroupRole->GetGroupRoomType());
    ENSURE_WITH_LOG_CMD(NULL != mgr, return, "not find dance mgr");

    LWRoomDanceInfo::EEndRoomResult eRet = pDanceInfo->EndRoom(pGroupRole->GetRoleID());
    if (eRet == LWRoomDanceInfo::EEndRoomResult_TooFast)
    { // kick the dancer if EndRoom to fast.
        mgr->OutRole(pRoom, pGroupRole->GetRoleID(), RoomQuitTo_World, RoomQuitType_EndKick); // 是否要检测房间关闭
        
        return; // 房间可能已经被删除，不能再做后续处理
    }
    else if (eRet == LWRoomDanceInfo::EEndRoomResult_Sucess)
    { // 
        if (pDanceInfo->CheckAllRoleState(RoleRoomState_ToEnd))
        { // 所有人都跳完了
            mgr->RemoveFromWaitEndList(pRoom->GetOriRoomID());

            pRoom->OnRoomDanceEndCallBack(); // 所有人跳舞结束
        }
        else
        { 
            mgr->AddToWaitEndList(RoomSettingMgr::Instance().RoomEndWait() + time(NULL), pRoom->GetOriRoomID()); // 关闭房间监控
        }
    }

    return;
}


void GroupRoomProcessor::OnTransFromIDUpdate(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsg.nAccount);

    GameMsg_S2G_TransFromIDUpdate &msg = (GameMsg_S2G_TransFromIDUpdate &)rMsg;
    
    pGroupRole->SetTransformID((unsigned short)msg.m_nTransformId);

    return; // 这个特殊，不用广播
}

void GroupRoomProcessor::OnOnEquipNotify(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsg.nAccount);

    GameMsg_S2G_OnEquipNotify &msg = (GameMsg_S2G_OnEquipNotify &)rMsg;
    pGroupRole->m_roomImageData.OnEquip(msg.m_dbEquip, msg.m_nEffectID);

    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);
    if (NULL == pRoom)
        return;

    // 广播穿装备的消息
    GameMsg_S2C_RefreshItemForOther retMsg;
    retMsg.nRoleID = pGroupRole->GetRoleID();
    retMsg.nItemType = msg.m_dbEquip.m_nTypeID;
    retMsg.nColumn = msg.m_dbEquip.m_nColumn;
    retMsg.nIndex = msg.m_dbEquip.m_nIndex;

    retMsg.itemEquip.InitFromDBItem(msg.m_dbEquip);
    retMsg.itemEquip.OnEquip();
    retMsg.itemEquip.SetEffectID(msg.m_nEffectID);
    
    RoomGroupCommon::SendMsgToAll(pRoom, retMsg, pGroupRole->GetRoleID());

    return;
}

void GroupRoomProcessor::OnUnEquipNotify(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsg.nAccount);

    GameMsg_S2G_UnEquipNotify &msg = (GameMsg_S2G_UnEquipNotify &)rMsg;
    pGroupRole->m_roomImageData.UnEquip(msg.m_nItemID); 

    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);
    if (NULL == pRoom)
        return;

    // 广播脱装备的消息
    GameMsg_S2C_RefreshItemForOther retMsg;
    retMsg.nRoleID = pGroupRole->GetRoleID();
    retMsg.nItemType = 0;
    retMsg.nColumn = msg.m_nColumn;
    retMsg.nIndex = msg.m_nIndex;

    RoomGroupCommon::SendMsgToAll(pRoom, retMsg, pGroupRole->GetRoleID());
}

void GroupRoomProcessor::OnEffectChangeNotify(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsg.nAccount);

    GameMsg_S2G_EffectChangeNotify &msg = (GameMsg_S2G_EffectChangeNotify &)rMsg;
    pGroupRole->m_roomImageData.EffectChange(msg.m_nItemID, msg.m_nEffectID);

    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);
    if (NULL == pRoom)
        return;

    DBItem dbItem;
    if (pGroupRole->m_roomImageData.GetDbItem(msg.m_nItemID, dbItem))
    {
        GameMsg_S2C_RefreshEffectChangeToOthers refreshmsg;
        refreshmsg.m_nItemType = dbItem.m_nTypeID;
        refreshmsg.m_nEffectID = msg.m_nEffectID;
        RoomGroupCommon::SendMsgToAll(pRoom, refreshmsg, pGroupRole->GetRoleID());
    }

    return;
}

void GroupRoomProcessor::OnRefreshSevenColor(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsg.nAccount);

    GameMsg_S2G_RefreshSevenColor &msg = (GameMsg_S2G_RefreshSevenColor &)rMsg;
    pGroupRole->m_roomImageData.AddSevenColor(msg.m_nSuitID, msg.m_nColor);

    // 
    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);
    if (NULL == pRoom)
        return;

    GameMsg_S2C_RefreshColorProgressToOthers othersmsg;
    othersmsg.m_nSuitID = msg.m_nSuitID;
    othersmsg.m_nColor = msg.m_nColor;
    RoomGroupCommon::SendMsgToAll(pRoom, othersmsg, pGroupRole->GetRoleID());

    return;
}

void GroupRoomProcessor::OnRefreshGeneNotify(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsg.nAccount);

    GameMsg_S2G_RefreshGeneNotify &msg = (GameMsg_S2G_RefreshGeneNotify &)rMsg;
    pGroupRole->m_roomImageData.UpdateGeneMap(msg.m_eAction, msg.m_GeneData);


    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);
    if (NULL == pRoom)
        return;

    switch (msg.m_eAction)
    {
    case EGeneNotifyAction_Add:
        {
            GameMsg_S2C_AddGene addnotify;
            addnotify.m_nRoleID = pGroupRole->GetRoleID();
            addnotify.m_GeneData = msg.m_GeneData;
            RoomGroupCommon::SendMsgToAll(pRoom, addnotify, pGroupRole->GetRoleID());
        }
        break;
    case EGeneNotifyAction_Del:
        {
            GameMsg_S2C_DelGene delnotify;
            delnotify.m_nRoleID = pGroupRole->GetRoleID();
            delnotify.m_nGeneID = msg.m_GeneData.nGeneId;
            RoomGroupCommon::SendMsgToAll(pRoom, delnotify, pGroupRole->GetRoleID());
        }
        break;
    case EGeneNotifyAction_Reset:
        {
            GameMsg_S2C_ResetGene resetmsg;
            resetmsg.m_nRoleID = pGroupRole->GetRoleID();
            resetmsg.m_nGeneID = msg.m_GeneData.nGeneId;
            resetmsg.m_nParam2 = msg.m_GeneData.nParam2;
            RoomGroupCommon::SendMsgToAll(pRoom, resetmsg, pGroupRole->GetRoleID());
        }
        break;
    case EGeneNotifyAction_None:
    default:
        break;
    }
    
    return;
}

void GroupRoomProcessor::OnAddMedals(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsg.nAccount);

    GameMsg_S2G_AddMedals &msg = (GameMsg_S2G_AddMedals &)rMsg;
    pGroupRole->m_roomImageData.AddMedals(msg.m_nMedalID);

    // 
    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);
    if (NULL == pRoom)
        return;

    // 广播基因变更到客户端,把这个消息直接再发送出去
    GameMsg_S2C_SyncMedalInfoToOthers syncMsg;
    syncMsg.m_nSceneID = msg.m_nSceneID;
    syncMsg.m_nRoleID = pGroupRole->GetRoleID();
    syncMsg.m_displayInfo.m_nType = CMedalCfgMgr::Instance().GetMedalType(msg.m_nMedalID);
    CMedalCfgMgr::Instance().GetMedalIcon(msg.m_nMedalID, syncMsg.m_displayInfo.m_strIcon);
 
    RoomGroupCommon::SendMsgToAll(pRoom, syncMsg, pGroupRole->GetRoleID());
}

void GroupRoomProcessor::OnSyncHorseData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error to get group role entity by account[%u]", rMsg.nAccount);

    // group 收的是这个S2C消息，然后会把他在广播出去
    GameMsg_S2C_SyncHorseData &msg = (GameMsg_S2C_SyncHorseData &)rMsg;
    pGroupRole->m_roomImageData.SyncHorseData(msg.m_nHorseTypeId, msg.m_nHorseOwnerId, msg.m_nHorseSeatIdx);

    // 
    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);
    if (NULL == pRoom)
        return;

    // 广播基因变更到客户端,把这个消息直接再发送出去
    RoomGroupCommon::SendMsgToAll(pRoom, rMsg, 0);

    return;
}


void GroupRoomProcessor::OnRoleNameChanged(CGroupRoleEntity *pGroupRole, const std::string &strName)
{
    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);

    if (NULL != pRoom)
    {
        GameMsg_S2C_ChangeRoleNameResult msg(CHANGEROLENAME_SUCCESS);
        msg.m_nRoleID = pGroupRole->GetRoleID();
        msg.m_strNewName = strName;

        RoomGroupCommon::SendMsgToAll(pRoom, msg, pGroupRole->GetRoleID());
    }

    return;

}

void GroupRoomProcessor::OnRoleVipChanged(CGroupRoleEntity *pGroupRole, bool bIsVip, unsigned short nVipLev)
{
    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);

    if (NULL != pRoom)
    {
        GameMsg_S2C_NotifyRoleVIPInfoChanged msgNotify;
        msgNotify.m_nRoleID = pGroupRole->GetRoleID();
        msgNotify.m_bIsVIP = bIsVip;
        msgNotify.m_nVIPLevel = nVipLev;

        RoomGroupCommon::SendMsgToAll(pRoom, msgNotify, pGroupRole->GetRoleID());
    }

    return;
}

void GroupRoomProcessor::OnUpdateDanceGroupName(CGroupRoleEntity *pGroupRole, std::string strDanceGroupName)
{
    if (NULL == pGroupRole)
        return;
    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);

    if (NULL != pRoom)
    {
        GameMsg_S2C_UpdateDanceGroupName updateMsg;
        updateMsg.m_nRoleID = pGroupRole->GetRoleID();
        updateMsg.m_strNewName = strDanceGroupName;

        RoomGroupCommon::SendMsgToAll(pRoom, updateMsg, pGroupRole->GetRoleID());
    }

    return;
}

void GroupRoomProcessor::OnUpdateDanceGroupTitle(CGroupRoleEntity *pGroupRole, unsigned char nTitle)
{
    if (NULL == pGroupRole)
        return;

    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);

    if (NULL != pRoom)
    {
        GameMsg_S2C_UpdateDanceGroupTitle updateMsg;
        updateMsg.m_nRoleID = pGroupRole->GetRoleID();
        updateMsg.m_nTitle = nTitle;

        RoomGroupCommon::SendMsgToAll(pRoom, updateMsg, pGroupRole->GetRoleID());
    }

    return;
}

void GroupRoomProcessor::OnUpdateDanceGroupBadgeOrEffect(CGroupRoleEntity *pGroupRole, unsigned short nBadge, unsigned short nEffect)
{
    if (NULL == pGroupRole)
        return;
    LWRoomBase *pRoom = GetRoleRoom(pGroupRole);

    if (NULL != pRoom)
    {
        GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew changeMsg;
        changeMsg.m_nErrorCode = EDanceGroupBadgeResult_Notify;
        changeMsg.m_nRoleID = pGroupRole->GetRoleID();
        changeMsg.m_nBadge = nBadge;
        changeMsg.m_nEffect = nEffect;

        RoomGroupCommon::SendMsgToAll(pRoom, changeMsg, pGroupRole->GetRoleID());
    }

    return;
}

void GroupRoomProcessor::ClearRoomManager()
{
    for (int i = 0; i < eRoomType_Max; ++i)
    {
        if (NULL != m_roomManagerArry[i])
        {
            delete m_roomManagerArry[i];
            m_roomManagerArry[i] = NULL;
        }
    }
}
