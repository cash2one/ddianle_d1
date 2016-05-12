#include "RoomLibBase.h"
#include "../../socket/Log.h"

#include "RoomDanceInfo.h"

#define INIT_ROOM_COUNT     256
#define RESERVE_ROOM_COUNT  128


LWRoomBase::LWRoomBase(ERoomType eType, unsigned int nRoomID)
    : m_eType(eType)
    , m_nRoomID(nRoomID)
{
    // empty;
}

LWRoomBase::~LWRoomBase()
{
    // empty;
}



unsigned int LWRoomBase::HostRoleID()
{
    return m_nHostRoleID;
}

void LWRoomBase::SetHost(unsigned int nHostID)
{
    m_nHostRoleID = nHostID;
}


EQuitRoomRes LWRoomBase::OnQuitRoom(unsigned int nRoleID, char quitTo)
{
    return QuitRoom_NoRoom;
}

// void LWRoomBase::OnGetRoomList(GameMsg_C2S_GetRoomList &msg)
// {
//     WriteLog(LOG_ERROR, "Roomtype[%d] error call LWRoomBase::OnGetRoomList", (int)GetType());
// }

EEnterRoomRes LWRoomBase::OnTryEnterRoom(unsigned int nRoleID, char nRoomRoleType, char nPhoneOs)
{
    return EnterRoom_Unknow;
}

// EEnterRoomRes LWRoomBase::OnEnterPlayer(GameMsg_C2S_EnterRoom &msg)
// {
//     WriteLog(LOG_ERROR, "Roomtype[%d] error call LWRoomBase::OnEnterPlayer", (int)GetType());
// }

EKickPlayerRes LWRoomBase::OnKickPlayer(unsigned int nKickRole, char nKickedRoleRoomType, char nKickedRolePos, unsigned int nKickedRole)
{
    WriteLog(LOGLEVEL_ERROR, "Roomtype[%d] error call LWRoomBase::OnKickPlayer", (int)GetType());
    return KickPlayer_NoRoom;
}
// 
// EChangeRoomInfoRes LWRoomBase::OnChangeRoomInfo(GameMsg_C2S_ChangeRoomInfo& msg)
// {
//     WriteLog(LOG_ERROR, "Roomtype[%d] error call LWRoomBase::OnChangeRoomInfo", (int)GetType());
// }

EChangeRoleRoomStateRes LWRoomBase::OnChangeRoleRoomState(unsigned int nRoleID, char toState)
{
    WriteLog(LOGLEVEL_ERROR, "Roomtype[%d] error call LWRoomBase::OnChangeRoleRoomState", (int)GetType());
    return ChangeRoleRoomState_Unknown;
}

EChangeRoomPosStateRes LWRoomBase::OnChangeRoomPosState(unsigned int nRoleID, ERoleRoomType eRoleType, char roomPos, char roomState)
{
    WriteLog(LOGLEVEL_ERROR, "Roomtype[%d] error call LWRoomBase::OnChangeRoomPosState", (int)GetType());
    return ChangeRoomPosState_NoRoom; // 
}

EPromoteRoomHost LWRoomBase::OnPromoteRoomHost(unsigned int nRoleID, char nNewHostRoomType, char nNewHostPos)
{
    WriteLog(LOGLEVEL_ERROR, "Roomtype[%d] error call LWRoomBase::OnPromoteRoomHost", (int)GetType());
    return PromoteRoomHost_UnKnow;
}

void LWRoomBase::ListenerRole(std::vector<std::pair<unsigned int, char> > &vecListner)
{
    WriteLog(LOGLEVEL_ERROR, "Roomtype[%d] error call LWRoomBase::ListenerRole", (int)GetType());
}

void LWRoomBase::OnRoomDanceAutoStart()
{
    // do nothing.
}
void LWRoomBase::OnRoomDanceAutoEnd()
{
    // do nothing.
}
void LWRoomBase::OnRoomDanceStartCallBack()
{
    // do nothing.
}
void LWRoomBase::OnRoomDanceEndCallBack()
{
    // do nothing.
}

void LWRoomBase::OnRoomDestoryCallBack()
{
    // do nothing.
}

unsigned int LWRoomBase::RoomSerial() const
{
    return GetOriRoomID();
}

unsigned int LWRoomBase::GetOriRoomID() const
{
    return m_nRoomID;
}


//////////////////////////////////////////////////////////////////////////
// LWRoomManager.
LWRoomManager::LWRoomManager(ERoomType eRt)
    : m_nPreUsedRoomID(0)
    , m_eRoomType(eRt)
{
//    ResizeRoomVector(RESERVE_ROOM_COUNT); //
}

LWRoomManager::~LWRoomManager()
{
    for (std::vector<LWRoomBase *>::iterator vecItr = m_vectRooms.begin();
        vecItr != m_vectRooms.end(); ++vecItr)
    {
        SAFE_DELETE(*vecItr);
    }
    m_vectRooms.clear();

    while (!m_listIdleRooms.empty())
    {
        SAFE_DELETE(*m_listIdleRooms.begin());
        m_listIdleRooms.pop_front();
    }
}

// 统一管理房间的内存
LWRoomBase* LWRoomManager::CreateNewRoom()
{
    LWRoomBase *pRoom = NULL;

    if (!m_listIdleRooms.empty())
    {
        pRoom = *m_listIdleRooms.begin();
        m_listIdleRooms.pop_front();
    }

    if (pRoom == NULL)
        pRoom = RoomFactory(++m_nPreUsedRoomID);

    MakeSureRoomSize(pRoom->GetOriRoomID()); // 

    m_vectRooms[pRoom->GetOriRoomID()] = pRoom;

    return pRoom; // 
}

ECreateRoomRes LWRoomManager::CreateRoom(unsigned int nCreateRole, const std::string &strRoomName, const std::string strRoomPwd, const char phoneOs)
{
    return CreateRoom_Unknow; // 错误的创建房间
}


EQuitRoomRes LWRoomManager::QuitFromRoom(LWRoomBase *room, unsigned int nRoleID, char quitTo)
{
    return QuitRoom_NoRoom;
}

void LWRoomManager::OnRoleLogout(LWRoomBase *room, unsigned int nRoleID)
{
    return ;
}

void LWRoomManager::OutRole(LWRoomBase *room, unsigned int nRoleID, char quitTo, ERoomQuitType quitType)
{
    return ;
}

void LWRoomManager::OnDestroyRoom(unsigned int nRoomID)
{
    return; // do nothing.
}


LWRoomBase* LWRoomManager::GetRoom(unsigned int nRoomID)
{
    if (nRoomID < m_vectRooms.size())
        return m_vectRooms[nRoomID];

    return NULL;
}

void LWRoomManager::DestroyRoom(unsigned int nRoomID)
{
    ENSURE_WITH_LOG_CMD(nRoomID < m_vectRooms.size(), return, "unvalid room id[%u], room type[%d]", nRoomID, (int)m_eRoomType);
    LWRoomBase *pRoom = m_vectRooms[nRoomID];
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error get null room[%u], room type[%d]", nRoomID, (int)m_eRoomType);

    // Manage on destroy room
    OnDestroyRoom(nRoomID);
    // Room on destroy room;
    pRoom->OnRoomDestoryCallBack();

    // Manage 回收这个房间
    m_listIdleRooms.push_back(pRoom);
    m_vectRooms[nRoomID] = NULL;

    return;
}

void LWRoomManager::AddToWaitStartList(time_t tillTime, unsigned int nRoomID)
{
    AddToWaitList(tillTime, nRoomID, m_waitStartRoomList, m_waitStartRoomMap);
    return; // 
}

void LWRoomManager::RemoveFromWaitStartList(unsigned int nRoomID)
{
    RemoveFromWaitList(nRoomID, m_waitStartRoomList, m_waitStartRoomMap);
    return;
}

void LWRoomManager::AddToWaitEndList(time_t tillTime, unsigned int nRoomID)
{
    AddToWaitList(tillTime, nRoomID, m_waitEndRoomList, m_waitEndRoomMap);
    return;
}

void LWRoomManager::RemoveFromWaitEndList(unsigned int nRoomID)
{
    RemoveFromWaitList(nRoomID, m_waitEndRoomList, m_waitEndRoomMap);
    return;
}

void LWRoomManager::AddToWaitList(time_t tillTime, unsigned int nRoomID, TimeRoomList &roomList, TimeRoomMap &roomMap)
{
    TimeRoomMap::iterator mapIter = roomMap.find(nRoomID);
    if (mapIter != roomMap.end())
        return; // 已经在里边了

    TimeRoom timeRoom;
    timeRoom.tillTime = tillTime;
    timeRoom.nRoomID = nRoomID;
    roomList.push_back(timeRoom); //

    TimeRoomList::iterator iter = (--roomList.end());

    roomMap.insert(std::make_pair(nRoomID, iter));
    return;
}

void LWRoomManager::RemoveFromWaitList(unsigned int nRoomID, TimeRoomList &roomList, TimeRoomMap &roomMap)
{
    TimeRoomMap::iterator mapIter = roomMap.find(nRoomID);
    if (mapIter == roomMap.end())
        return; // 没有这个

    roomList.erase(mapIter->second); // 删除掉
    roomMap.erase(mapIter); // 

    return;
}


void LWRoomManager::OnUpdate(time_t now)
{
    // 检测所有到时间的房间
    while (!m_waitStartRoomList.empty())
    {
        if (m_waitStartRoomList.front().tillTime < now)
            break; // first room not over time.
        
        LWRoomBase* pRoom = GetRoom(m_waitStartRoomList.front().nRoomID);

        // 这个必须要清理
        m_waitStartRoomMap.erase(m_waitStartRoomList.front().nRoomID);
        m_waitStartRoomList.erase(m_waitStartRoomList.begin());

        if (NULL != pRoom)
            OnMgrRoomDanceAutoStart(pRoom);
    }

    // 检测所有到时间的房间
    while (!m_waitEndRoomList.empty())
    {
        if (m_waitEndRoomList.front().tillTime < now)
            break; // first room not over time.

        LWRoomBase* pRoom = GetRoom(m_waitEndRoomList.front().nRoomID);

        m_waitEndRoomMap.erase(m_waitEndRoomList.front().nRoomID); 
        m_waitEndRoomList.erase(m_waitEndRoomList.begin());

        if (NULL != pRoom)
            OnMgrRoomDanceAutoEnd(pRoom);
    }
}

void LWRoomManager::OnMgrRoomDanceAutoStart(LWRoomBase* pRoom)
{
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "room ptr should not null on room auto start, room type[%d].", (int)m_eRoomType);

    pRoom->OnRoomDanceAutoStart();
    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();

    if (NULL != pDanceInfo && !pDanceInfo->HasDancer())
    { // 关闭房间
        DestroyRoom(pRoom->GetOriRoomID()); // 关闭这个房间
    }
}

void LWRoomManager::OnMgrRoomDanceAutoEnd(LWRoomBase* pRoom)
{
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "room ptr should not null on room auto end, room type[%d].", (int)m_eRoomType);

    pRoom->OnRoomDanceAutoEnd();
    LWRoomDanceInfo *pDanceInfo = pRoom->GetRoomDanceInfo();
    if (NULL != pDanceInfo && !pDanceInfo->HasDancer())
    { // 关闭房间
        DestroyRoom(pRoom->GetOriRoomID());
    }

    return;
}

unsigned int LWRoomManager::RoomSerialToID(unsigned int nRoomSerial) const
{
    return nRoomSerial;
}

void LWRoomManager::MakeSureRoomSize(unsigned int nRoomID)
{
    if (nRoomID >= m_vectRooms.size())
    {
        unsigned int newCount = RESERVE_ROOM_COUNT;
        while (nRoomID >= m_vectRooms.size() + newCount)
        {
            newCount += RESERVE_ROOM_COUNT;
        }
        m_vectRooms.insert(m_vectRooms.end(), newCount, NULL);
    }
}


namespace RoomCommonFunc
{
    std::string CreateRoomErrorString(ECreateRoomRes eRes)
    {
        switch (eRes)
        {
        case CreateRoom_Unknow:
            return "CreateRoom_Unknow";
        case CreateRoom_InRoom:
            return "CreateRoom_InRoom";
        case CreateRoom_EmptyName:
            return "CreateRoom_EmptyName";
        case CreateRoom_NameTooLong:
            return "CreateRoom_NameTooLong";
        case CreateRoom_PwdTooLong:
            return "CreateRoom_PwdTooLong";
        case CreateRoom_RoomEnough:
            return "CreateRoom_RoomEnough";
        case CreateRoom_InvalidRoomType:
            return "CreateRoom_Unknow";
        case CreateRoom_DancerNotExist:
            return "CreateRoom_Unknow";
        case CreateRoom_InWeddingRoom:
            return "CreateRoom_InWeddingRoom";
        case CreateRoom_InPhotoRoom:
            return "CreateRoom_InPhotoRoom";
        case CreateRoom_InAmuseRoom:
            return "CreateRoom_InAmuseRoom";
        case CreateRoom_InCeremonyRoom:
            return "CreateRoom_InCeremonyRoom";
        case CreateRoom_NotHasDanceGroup:
            return "CreateRoom_NotHasDanceGroup";
        case CreateRoom_NoMoreFailyLandRoomStage:
            return "CreateRoom_NoMoreFailyLandRoomStage";
        case  CreateRoom_NotInOpenTime:
            return "CreateRoom_NotInOpenTime";
        case CreateRoom_NoPriv:
            return "CreateRoom_NoPriv";
        case CreateRoom_NotXiaoMengXin:
            return "CreateRoom_NotXiaoMengXin";
        case CreateRoom_Success:
        default:
            return "";
        }
    }

    std::string EnterRoomErrorString(EEnterRoomRes eRes)
    {
        switch (eRes)
        {
        case EnterRoom_Unknow:
            return "EnterRoom_Unknow";
        case EnterRoom_InRoom:
            return "EnterRoom_InRoom";
        case EnterRoom_NoRoom:
            return "EnterRoom_NoRoom";
        case EnterRoom_IncorrectPwd:
            return "EnterRoom_WrongPwd";
        case EnterRoom_RoomStart:
            return "EnterRoom_RoomStart";
        case EnterRoom_RoomEnd:
            return "EnterLiveRoom_RoomEnd";
        case EnterRoom_AudienceFull:
            return "EnterRoom_AudienceFull";
        case EnterRoom_DancerFull:
            return "EnterRoom_DancerFull";
        case EnterRoom_RequirePwd:
            return "EnterRoom_WrongPwd";
            //     case EnterRoom_Success:
            //         return "CreateRoom_InPhotoRoom";
        case EnterRoom_NewPlayer:
            return "EnterRoom_NewPlayer";
        case EnterRoom_InWeddingRoom:
            return "EnterRoom_InWeddingRoom";
        case EnterRoom_InPhotoRoom:
            return "EnterRoom_InPhotoRoom";
        case EnterRoom_InAmuseRoom:
            return "EnterRoom_InAmuseRoom";
        case EnterRoom_RoomTypeNotMatch:
            return "EnterLiveRoom_RoomTypeNotMatch";
        case EnterRoom_InCeremonyRoom:
            return "EnterRoom_InCeremonyRoom";
        case EnterRoom_NotSameDanceGroup:
            return "EnterRoom_NotSameDanceGroup";
        case EnterRoom_NotInOpenTime:
            return "EnterRoom_NotInOpenTime";
        case EnterRoom_NoMoreFailyLandRoomStage:
            return "EnterRoom_NoMoreFailyLandRoomStage";
        case EnterRoom_NotXiaoMengXin:
            return "EnterRoom_NotXiaoMengXin";
        case EnterRoom_Success:
        default:
            return "";
        }
    }

    std::string QuitRoomErrorString(EQuitRoomRes eRes)
    {
        switch (eRes)
        {
        case QuitRoom_NoRoom:
            return "QuitRoom_NoRoom";
        case QuitRoom_HasReady:
            return "QuitRoom_HasReady";
        case QuitRoom_HasStart:
            return "QuitRoom_HasStart";
        case QuitRoom_Success:
        default:
            return "";
        }
    }

    std::string KickPlayerErrorString(EKickPlayerRes eRes)
    {
        switch (eRes)
        {
        case KickPlayer_NoRoom:
            return "KickPlayer_NoRoom";
        case KickPlayer_NotHost:
            return "KickPlayer_NotHost";
        case KickPlayer_HasStart:
            return "KickPlayer_HasStart";
        case KickPlayer_NoPlayer:
            return "KickPlayer_NoPlayer";
        case KickPlayer_IsSelf:
            return "KickPlayer_IsSelf";
        case KickPlayer_PoorVip:
            return "KickPlayer_PoorVip";
        case KickPlayer_NoAuthority:
            return "KickPlayer_NoAuthority";
        case KickPlayer_CanNotKickHost:
            return "KickPlayer_CanNotKickHost";
        case KickPlayer_CanNotKickNewPlayer:
            return "KickPlayer_CanNotKickNewPlayer";
        case KickPlayer_AudienceCanNotKickPlayer:
            return "KickPlayer_AudienceCanNotKickPlayer";
        case KickPlayer_CanNotKickGM:
            return "KickPlayer_CanNotKickGM"; // new add.
        case KickPlayer_Success:
        default:
            return "";
        }
    }

    std::string ChangeRoleRoomStateErrorString(EChangeRoleRoomStateRes eRes)
    {
        switch (eRes)
        {
        case ChangeRoleRoomState_NoRoom:
            return "ChangeRoleRoomState_NoRoom";
        case ChangeRoleRoomState_IsHost:
            return "ChangeRoleRoomState_IsHost";
        case ChangeRoleRoomState_NotDancer:
            return "ChangeRoleRoomState_NotDancer";
        case ChangeRoleRoomState_HasStart:
            return "ChangeRoleRoomState_HasStart";
        case ChangeRoleRoomState_NoState:
            return "ChangeRoleRoomState_NoState";
        case ChangeRoleRoomState_NoChange:
            return "ChangeRoleRoomState_NoChange";
        case ChangeRoleRoomState_Unknown:
            return "ChangeRoleRoomState_Unknown";
        case ChangeRoleRoomState_NoChanllageTime:
            return "ChangeRoleRoomState_NoChanllageTime";
        case ChangeRoleRoomState_Success:
        default:
            return "";
        }
    }

    std::string ChangeRoomPosStateErrorString(EChangeRoomPosStateRes eRes)
    {
        switch (eRes)
        {
        case ChangeRoomPosState_NoRoom:
            return "ChangeRoomPosState_NoRoom";
        case ChangeRoomPosState_NotHost:
            return "ChangeRoomPosState_NotHost";
        case ChangeRoomPosState_HasStart:
            return "ChangeRoomPosState_HasStart";
        case ChangeRoomPosState_Occupied:
            return "ChangeRoomPosState_Occupied";
        case ChangeRoomPosState_NoState:
            return "ChangeRoomPosState_NoState";
        case ChangeRoomPosState_NoChange:
            return "ChangeRoomPosState_NoChange";
        case ChangeRoomPosState_Success:
        default:
            return "";
        }
    }

    std::string PromoteRoomHostErrorString(EPromoteRoomHost eRes)
    {
        switch (eRes)
        {
        case PromoteRoomHost_OldHostNotInRoom:
            return "PromoteRoomHost_OldHostNotInRoom";
        case PromoteRoomHost_RoomNotExist:
            return "PromoteRoomHost_RoomNotExist";
        case PromoteRoomHost_NewHostNotInRoom:
            return "PromoteRoomHost_NewHostNotInRoom";
        case PromoteRoomHost_NoAuthority:
            return "PromoteRoomHost_NoAuthority";
        case PromoteRoomHost_UnKnow:
            return "PromoteRoomHost_UnKnow";
        case PromoteRoomHost_Sucess:
        default:
            return "";
        }
    }

    std::string StartRoomErrorString(EStartRoomRes eRes)
    {
        switch (eRes)
        {
        case StartRoom_NoRoom:
            return "StartRoom_NoRoom";
        case StartRoom_NotHost:
            return "StartRoom_NotHost";
        case StartRoom_HasStart:
            return "StartRoom_HasStart";
        case StartRoom_NotReady:
            return "StartRoom_NotReady";
        case StartRoom_NotMatchTeamMode:
            return "StartRoom_NotMatchTeamMode";
        case StartRoom_NotMatchPlayerCount:
            return "StartRoom_NotMatchPlayerCount";
        case StartLiveRoom_NoPriv:
            return "StartLiveRoom_NoPriv";
        case StartLiveRoom_NoDancer:
            return "StartLiveRoom_NoDancer";
        case StartLiveRoom_HasEnd:
            return "StartLiveRoom_HasEnd";
        case StartRoom_NoMusic:
            return "StartRoom_NoMusic";
        case StartRoom_NoDanceRoom:
            return "StartRoom_NoDanceRoom";
        case StartRoom_NotInOpenTime:
            return "StartRoom_NotInOpenTime";
        case StartRoom_Unknown:
            return "StartRoom_Unknown";
        case StartRoom_NoFailyLandRoomTimes:
            return "StartRoom_NoFailyLandRoomTimes";
        case StartRoom_NoMoreFailyLandRoomStage:
            return "StartRoom_NoMoreFailyLandRoomStage";
        case StartRoom_Success:
        default:
            return "";
        }
    }
}

