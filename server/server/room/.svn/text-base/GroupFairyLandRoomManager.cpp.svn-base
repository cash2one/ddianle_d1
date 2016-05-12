#include "GroupFairyLandRoomManager.h"
#include "RoomGroupCommon.h"
#include "../group/Global.h"
#include "../../socket/Log.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../socket/GameMsg_Map.h"
#include "GroupFairyLandRoom.h"
#include "../roomLib/RoomDanceInfo.h"
#include "../roomLib/RoomPosManager.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/GameMsgNewRoom.h"
#include "../DanceGroupFairyLand/DanceGroupFairyLandProcessor.h"
#include "../DanceGroupFairyLand/GameMsg_FairyLand.h"
#include "../dancegroup/DanceGroupProcessor.h"


GroupFairyLandRoomManager::GroupFairyLandRoomManager()
    : GameMsg_Processor(true, false)
    , LWRoomManager(eRoomType_FairyLandRoom)
{

}

GroupFairyLandRoomManager::~GroupFairyLandRoomManager()
{

}

bool GroupFairyLandRoomManager::Init()
{
    RegisterMessage();

    return true;
}

bool GroupFairyLandRoomManager::RegisterMessage()
{
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetFairyLandRoomList, &GroupFairyLandRoomManager::OnGetRoomList);

    return true; 
}


void GroupFairyLandRoomManager::OnGetRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    GameMsg_C2S_GetFairyLandRoomList &msg = (GameMsg_C2S_GetFairyLandRoomList &)rMsg;

    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(rMsg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return, "error get account[%u] group role entity.", rMsg.nAccount);

    if (pGroupRole->GetDanceGroupID() == 0)
        return ; // 没有舞团的人才加不了

    GroupRoomMap::iterator iter = m_dancegroupRoomMap.find(pGroupRole->GetDanceGroupID());

    GameMsg_S2C_GetFairyLandRoomListResult retMsg;
    retMsg.m_nCurPage = 0;
    retMsg.m_nRoomNum = 0;
    if (iter != m_dancegroupRoomMap.end() && NULL != iter->second && !iter->second->empty())
    { // 不存在房间，
        std::vector<unsigned int> *roomVec = iter->second;

        const unsigned int nNumPerPage = 4; // 每页的房间个数
        unsigned int nStartIndex = msg.m_nPage * nNumPerPage;
        
        retMsg.m_nCurPage = msg.m_nPage;
        retMsg.m_nRoomNum = roomVec->size();

        if (nStartIndex >= roomVec->size())
        {
            nStartIndex = (roomVec->size() >= nNumPerPage ? roomVec->size() - nNumPerPage : 0);
            retMsg.m_nCurPage = roomVec->size() / nNumPerPage;
        }
        
        unsigned int nEndIndex = nStartIndex + nNumPerPage;
        for (int i = nStartIndex; i < (int)roomVec->size() && i < (int)nEndIndex; ++i)
        { //
            FairyLandRoomBriefInfo f;
            GroupFairyLandRoom *pRoom = (GroupFairyLandRoom *)GetRoom((*roomVec)[i]);

            ENSURE_WITH_LOG_CMD(NULL != pRoom, continue, "error to get NULL room[%u] ptr", (*roomVec)[i]);
            
            LWRoomDanceInfo *pRoomDanceInfo = pRoom->GetRoomDanceInfo();
            ENSURE_WITH_LOG_CMD(NULL != pRoomDanceInfo, continue, "error to get NULL room[%u] danceinof ptr", (*roomVec)[i]);
            f.m_bHasStart = (pRoomDanceInfo->GetState() != eRoomState_Idle);
            f.m_nRoomIndex = pRoom->RoomSerial();

            LWRoomPosManager *pPosManager = pRoom->GetPosManager();
            ENSURE_WITH_LOG_CMD(NULL != pPosManager, continue, "error to get NULL room[%u] POS manager ptr", (*roomVec)[i]);
            LWDanceRoomPosInfo *pPosInfo = pPosManager->GetRoomPosInfo(RoleRoomType_Dancer);
            ENSURE_WITH_LOG_CMD(NULL != pPosInfo, continue, "error to get NULL room[%u] pos info ptr", (*roomVec)[i]);

            LWDanceRoomPosInfo::RoleVec vecRole;
            pPosInfo->GetAllPlayer(vecRole);
            for (size_t i = 0; i < vecRole.size(); ++i)
            {
                CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(vecRole[i].first);
                ENSURE_WITH_LOG_CMD(NULL != pGroupRole, continue, "error to get role[%u] in room", vecRole[i].first);
                FairyLandRoomRole r;
                r.m_strRoleName = pGroupRole->GetName();
                r.m_nVip = (unsigned char)pGroupRole->VIPLevel();
                r.m_bIsVip = pGroupRole->IsVIP();
                f.m_vecRoles.push_back(r);
            }

            retMsg.m_vecRoomBriefInfo.push_back(f);
        }
    }

    pGroupRole->SendPlayerMsg(&retMsg);

    return ;
}

ECreateRoomRes GroupFairyLandRoomManager::CreateRoom(unsigned int nCreateRole, const
    std::string &strRoomName, const std::string strRoomPwd, const char phoneOs)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nCreateRole);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return CreateRoom_Unknow, "role[%u] miss group entity.", nCreateRole);

    if (pGroupRole->GetGroupRoomID() != 0 || pGroupRole->GetGroupRoomType() != 0)
        return CreateRoom_InRoom;

    // 创建检测
    if (0 == pGroupRole->GetDanceGroupID())
        return CreateRoom_NotHasDanceGroup;

    const CDanceGroupInfo *pDanceGroupInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo(pGroupRole->GetDanceGroupID());
    if (NULL == pDanceGroupInfo)
        return CreateRoom_NotHasDanceGroup;

    const CDanceGroupMember *pMember = pDanceGroupInfo->GetGroupMember(pGroupRole->GetRoleID());
    if (NULL == pMember)
        return CreateRoom_NotHasDanceGroup;

    if (pMember->m_nProperty == EMemberProperty_MengXin)
        return CreateRoom_NotXiaoMengXin;

    // CreateRoom. 做自己的房间管理
    GroupFairyLandRoom *pRoom = (GroupFairyLandRoom *)CreateNewRoom();
    pRoom->SetDanceGroup(pGroupRole->GetDanceGroupID());
    AddRoom(pGroupRole->GetDanceGroupID(), pRoom->GetOriRoomID());

    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
    LWRoomPosManager *pPosManager = pRoom->GetPosManager();
    ENSURE_WITH_LOG_CMD(NULL != pDanceInfo && NULL != pPosManager, return CreateRoom_Unknow, "creater fairyLand room error");
    pDanceInfo->Init();
    pPosManager->InitRoomPosInfo();

    pGroupRole->SetNewGroupRoom(m_eRoomType, pRoom->GetOriRoomID());
    pDanceInfo->AddRole(nCreateRole, RoleRoomState_Ready); 
    pPosManager->AddDancer(RoleRoomType_Dancer, PhoneRole(nCreateRole, (Phone_OS)phoneOs));
    pRoom->SetHost(nCreateRole); //over

    // 回复这个玩家成功进入房间的消息
    GameMsg_S2C_NewCreateRoomSuc msgSelfResponse;
    msgSelfResponse.m_nRoomType = (unsigned char)pRoom->GetType();
    RoomGroupCommon::EncodeWholeRoomInfo(pRoom, pRoom->GetRoomDanceInfo(), pRoom->GetPosManager(), msgSelfResponse.m_RoomInfo, true);
    pGroupRole->SendPlayerMsg(&msgSelfResponse); // 回复玩家房间创建成功消息,

    return CreateRoom_Success;
}

EQuitRoomRes GroupFairyLandRoomManager::QuitFromRoom(LWRoomBase *room, unsigned int nRoleID, char quitTo)
{
    ENSURE_WITH_LOG_CMD(NULL != room, return QuitRoom_NoRoom, "room should not null");
    EQuitRoomRes quitRes = room->OnQuitRoom(nRoleID, quitTo);

    if (quitRes == QuitRoom_Success)
    {
        if (NULL != room->GetRoomDanceInfo() && !room->GetRoomDanceInfo()->HasDancer())
        { // 房间没人了，关闭这个房间
            DestroyRoom(room->GetOriRoomID());
        }
    }

    return quitRes;
}

void GroupFairyLandRoomManager::OnRoleLogout(LWRoomBase *room, unsigned int nRoleID)
{
    OutRole(room, nRoleID, 0, RoomQuitType_Logout);

    return;
}


void GroupFairyLandRoomManager::OutRole(LWRoomBase *room, unsigned int nRoleID, char quitTo, ERoomQuitType quitType)
{
    // 
    GroupFairyLandRoom *pRoom = (GroupFairyLandRoom *)room;
    RoomGroupCommon::OutDancerRole(room, room->GetRoomDanceInfo(), room->GetPosManager(), nRoleID, quitTo, quitType, 0);

    LWRoomDanceInfo *pDancerInfo = pRoom->GetRoomDanceInfo();
    if (NULL != pDancerInfo)
    {
        bool bHasDancer = pDancerInfo->HasDancer();

        // 如果房间等待开启，触发开启
        if (pDancerInfo->GetState() == eRoomState_Waiting && pDancerInfo->CheckAllRoleState(RoleRoomState_ToStart))
        {
            RemoveFromWaitStartList(pRoom->GetOriRoomID());
            if (bHasDancer)
            {
                pRoom->OnRoomDanceStartCallBack(); // 启动
                GameMsg_S2C_StartRoomSuc msgResponse;
                RoomGroupCommon::SendMsgToAll(pRoom, msgResponse, 0);
            }
        }

        if (pDancerInfo->GetState() == eRoomState_Ending && pDancerInfo->CheckAllRoleState(RoleRoomState_ToEnd))
        {
            RemoveFromWaitEndList(pRoom->GetOriRoomID());
            if (bHasDancer)
            { // 处理结束：
                pRoom->OnRoomDanceEndCallBack(); 
            }
        }

        // 如果房间等待结束，触发结束
        if (!bHasDancer)
        {
            DestroyRoom(pRoom->GetOriRoomID()); // 销毁这个房间
        }
    }

    return;
}

void GroupFairyLandRoomManager::OnDestroyRoom(unsigned int nRoomID)
{
    RemoveRoom(nRoomID);

    return;
}

unsigned int GroupFairyLandRoomManager::RoomSerialToID(unsigned int nRoomSerial) const
{
    if (nRoomSerial < FAIRY_LAND_ROOM_SERIAL_START)
        return 0;
    else
    {
        return nRoomSerial - FAIRY_LAND_ROOM_SERIAL_START;
    }
}

void GroupFairyLandRoomManager::OnGroupDisband(unsigned int nDanceGroupID)
{
    GroupRoomMap::iterator iterRooms = m_dancegroupRoomMap.find(nDanceGroupID);
    if (m_dancegroupRoomMap.end() == iterRooms ||  NULL == iterRooms->second)
        return; // 这个舞团没有房间

    std::vector<unsigned int> roomVec(iterRooms->second->begin(), iterRooms->second->end());

    for (int i = 0; i < (int)roomVec.size(); ++i)
    {
        DestroyRoom(roomVec[i]);
    }

    return;
}


LWRoomBase *GroupFairyLandRoomManager::RoomFactory(unsigned int nRoomID)
{
    return new GroupFairyLandRoom(nRoomID);
}


void GroupFairyLandRoomManager::AddRoom(unsigned int nGroupID, unsigned int nRoomID)
{ 
    GroupRoomMap::iterator iter = m_dancegroupRoomMap.find(nGroupID);
    std::vector<unsigned int> *roomVec = NULL;
    if (iter == m_dancegroupRoomMap.end())
    {
        roomVec = new std::vector < unsigned int >() ;
        m_dancegroupRoomMap.insert(std::make_pair(nGroupID, roomVec));
    }
    else
    {
        roomVec = iter->second;
        ENSURE_WITH_LOG_CMD(NULL != roomVec, return, "get null FairyLandRoom vec");
    }

    roomVec->push_back(nRoomID);

    return;
}

void GroupFairyLandRoomManager::RemoveRoom(unsigned int nRoomID)
{
    GroupFairyLandRoom *pRoom = (GroupFairyLandRoom *)GetRoom(nRoomID);
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error to get room[%u] in fairylandroom ptr.", nRoomID);
    
    GroupRoomMap::iterator iterRooms = m_dancegroupRoomMap.find(pRoom->GetDanceGroup());
    ENSURE_WITH_LOG_CMD(iterRooms != m_dancegroupRoomMap.end(), return, "error to get room[%u] in fairylandroom map.", nRoomID);

    std::vector<unsigned int> *roomVec = iterRooms->second;
    ENSURE_WITH_LOG_CMD(NULL != roomVec, return, "get null room[%u] in fairyland room Manager.", nRoomID);
    roomVec->erase(std::remove(roomVec->begin(), roomVec->end(), nRoomID), roomVec->end());
    if (roomVec->empty())
    {
        SAFE_DELETE(roomVec);
        iterRooms->second = NULL;
        m_dancegroupRoomMap.erase(iterRooms); // 删除这个舞团的房间节点
    }

    return;
}

