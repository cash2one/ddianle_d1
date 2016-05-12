#include "GroupFairyLandRoom.h"
#include "RoomGroupCommon.h"
#include "../Macros/Macros.h"
#include "../../socket/Log.h"
#include "../group/Global.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/GameMsgNewRoom.h"
#include "../roomLib/RoomDanceInfo.h"
#include "../roomLib/RoomPosManager.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../dancegroup/DanceGroupProcessor.h"
#include "../DanceGroupFairyLand/DanceGroupFairyLandProcessor.h"

GroupFairyLandRoom::GroupFairyLandRoom(unsigned int nRoomID)
    : LWRoomBase(eRoomType_FairyLandRoom, nRoomID)
{
    m_pRoomDanceInfo = new LWRoomDanceInfo;
    m_pRoomPosManager = new LWRoomPosManager(MAX_ROOM_PLAYER_COUNT, 0);
}

GroupFairyLandRoom::~GroupFairyLandRoom()
{
    SAFE_DELETE(m_pRoomDanceInfo);
    SAFE_DELETE(m_pRoomPosManager);
}


EQuitRoomRes GroupFairyLandRoom::OnQuitRoom(unsigned int nRoleID, char quitTo)
{
    // room state test
    if (m_pRoomDanceInfo->HasStart())
    {
        return QuitRoom_HasStart; // 
    }

    LWDanceRoleInfo* pDancer = m_pRoomDanceInfo->GetDanceRole(nRoleID);
    if (NULL == pDancer)
        return QuitRoom_NoRoom; // 

    // role state test: player who is not Host, and has ready, can not quit.
    if (pDancer->GetState() == RoleRoomState_Ready && HostRoleID() != nRoleID)
        return QuitRoom_HasReady; // 
    
    // 退出玩家
    RoomGroupCommon::OutDancerRole(this, m_pRoomDanceInfo, m_pRoomPosManager, nRoleID, quitTo, RoomQuitType_Active, 0);
    return QuitRoom_Success;
}

EEnterRoomRes GroupFairyLandRoom::OnTryEnterRoom(unsigned int nRoleID, char nRoomRoleType, char nPhoneOs)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return EnterRoom_Unknow, "miss role[%u] GroupRoleEnity", nRoleID); //
    // 不支持观众
    LWDanceRoomPosInfo* posInfo = m_pRoomPosManager->GetRoomPosInfo(RoleRoomType_Dancer);
    ENSURE_WITH_LOG_CMD(NULL != posInfo, return EnterRoom_Unknow, "role[%u] enter room error, FairyLandRoom do not has dance pos info.", pGroupRole->GetRoleID());

    if (pGroupRole->GetDanceGroupID() != m_nDanceGroup)
        return EnterRoom_NotSameDanceGroup;

    const CDanceGroupInfo *pDanceGroupInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo(pGroupRole->GetDanceGroupID());
    if (NULL == pDanceGroupInfo)
        return EnterRoom_NotSameDanceGroup;

    const CDanceGroupMember *pMember = pDanceGroupInfo->GetGroupMember(pGroupRole->GetRoleID());
    if (NULL == pMember)
        return EnterRoom_NotSameDanceGroup;

    if (pMember->m_nProperty == EMemberProperty_MengXin)
        return EnterRoom_NotXiaoMengXin;

    if (pGroupRole->GetGroupRoomID() != 0)
        return EnterRoom_InRoom; // 已经在房间中

    if (m_pRoomDanceInfo->HasStart())
        return EnterRoom_RoomStart;

    if (posInfo->HasFull() || !posInfo->AddDancer(PhoneRole(nRoleID, Phone_OS(nPhoneOs))))
        return EnterRoom_DancerFull; // 人已经满了

//     if (!CDanceGroupFairyLandProcessor::Instance().CheckAllowStart())
//         return EnterRoom_NotInOpenTime;
// 
//     if (CDanceGroupFairyLandProcessor::Instance().CheckIsFinishedAllChapter(m_nDanceGroup))
//         return EnterRoom_NoMoreFailyLandRoomStage;

    m_pRoomDanceInfo->AddRole(nRoleID, RoleRoomState_Wait);
    pGroupRole->SetNewGroupRoom(this->GetType(), GetOriRoomID()); // 记录
    int enterPos = posInfo->GetRolePos(nRoleID);

    GameMsg_S2C_NewCreateRoomSuc msgSelfResponse;
    msgSelfResponse.m_nRoomType = (unsigned char)this->GetType();
    RoomGroupCommon::EncodeWholeRoomInfo(this, m_pRoomDanceInfo, m_pRoomPosManager, msgSelfResponse.m_RoomInfo, false);

    pGroupRole->SendPlayerMsg(&msgSelfResponse); // 回复加入成功消息

    GameMsg_S2C_EnterPlayer msgOtherResponse;
    RoomGroupCommon::EncodePlayerInfo(pGroupRole->GetRoleID(), false, RoleRoomType_Dancer, (char)enterPos, RoleRoomState_Wait, EDanceColorTeam_None, msgOtherResponse.m_PlayerInfo);
    RoomGroupCommon::SendMsgToAll(this, msgOtherResponse, pGroupRole->GetRoleID());

    return EnterRoom_Success; //
}

EKickPlayerRes GroupFairyLandRoom::OnKickPlayer(unsigned int nKickRole, char nKickedRoleRoomType, char nKickedRolePos, unsigned int nKickedRole)
{
    CGroupRoleEntity *pPlayerOperator = CGroupPlayerManager::Instance().SearchRoleByID(nKickRole);
    CGroupRoleEntity *pPlayerKicked = CGroupPlayerManager::Instance().SearchRoleByID(nKickedRole);
    ENSURE_WITH_LOG_CMD(NULL != pPlayerOperator, return KickPlayer_NoPlayer, "miss role[%u](operator) group role entity on kick dancer.", nKickRole);
    ENSURE_WITH_LOG_CMD(NULL != pPlayerKicked, return KickPlayer_NoPlayer, "miss role[%u](kicked) group role entity on kick dancer.", nKickedRole);

    ERoleRoomType eHostRoomType;
    unsigned int nHostPos;
    ERoleRoomState eHostRoomState;
    {
        ERoleRoomType eTempKickedRoleRoomType;
        unsigned int nTempKickedRolePos;
        bool ret = m_pRoomPosManager->GetRolePos(pPlayerKicked->GetRoleID(), eTempKickedRoleRoomType, nTempKickedRolePos);
        ENSURE_WITH_LOG_CMD(ret && (nKickedRoleRoomType == eTempKickedRoleRoomType) && (nKickedRolePos == (char)nTempKickedRolePos), 
            return KickPlayer_NoPlayer, "not find kicked role[%u] pos info in kicked player", pPlayerKicked->GetRoleID());

        ret = m_pRoomPosManager->GetRolePos(nKickRole, eHostRoomType, nHostPos);
        ENSURE_WITH_LOG_CMD(ret, return KickPlayer_NoAuthority, "not find host[%u] pos info in kicked player", HostRoleID());

        LWDanceRoleInfo *pDancer = m_pRoomDanceInfo->GetDanceRole(HostRoleID());
        ENSURE_WITH_LOG_CMD(ret, return KickPlayer_NoAuthority, "not find host[%u] dancer info in kicked player", HostRoleID());
        eHostRoomState = pDancer->GetState();
    }

    // this player should be host.
    if (HostRoleID() != pPlayerOperator->GetRoleID())
        return KickPlayer_NoAuthority; //

    // room should not in start state.
    if (m_pRoomDanceInfo->HasStart())
        return KickPlayer_HasStart;

    // can't kick self.
    if (pPlayerOperator->GetRoleID() == pPlayerKicked->GetRoleID())
        return KickPlayer_IsSelf;

    RoomGroupCommon::OutDancerRole(this, GetRoomDanceInfo(), GetPosManager(), nKickedRole, RoomQuitTo_World, RoomQuitType_HostKick, pPlayerOperator->GetRoleID());

    return KickPlayer_Success; // 成功
}

EChangeRoleRoomStateRes GroupFairyLandRoom::OnChangeRoleRoomState(unsigned int nRoleID, char toState)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return ChangeRoleRoomState_Unknown, "can't find role[%u] group role entity on change role state.", nRoleID);

    LWDanceRoleInfo *pDancer = m_pRoomDanceInfo->GetDanceRole(nRoleID);
    ENSURE_WITH_LOG_CMD(NULL != pGroupRole, return ChangeRoleRoomState_Unknown, "can't find role[%u] role danceinfo.", nRoleID);

    ERoleRoomType eRoleRoomType;
    unsigned int nRolePos;
    ENSURE_WITH_LOG_CMD(m_pRoomPosManager->GetRolePos(pGroupRole->GetRoleID(), eRoleRoomType, nRolePos), return ChangeRoleRoomState_Unknown,
        "can't find role[%u] pos info on change role room state", pGroupRole->GetRoleID());

    // 只能变为这三个状态
    if (toState != RoleRoomState_Equip && toState != RoleRoomState_Ready && toState != RoleRoomState_Wait)
        return ChangeRoleRoomState_NoState;

    if (pDancer->GetState() == toState)
        return ChangeRoleRoomState_NoChange;

    // 需要检测ready状态，是否有次数
    if (toState == RoleRoomState_Ready && !CDanceGroupFairyLandProcessor::Instance().CheckAllowReadyInFairyLandRoom(m_nDanceGroup, pGroupRole->GetRoleID()))
    {
        return ChangeRoleRoomState_NoChanllageTime;
    }

    // 房主不能切换为等待状态
    if (HostRoleID() == pGroupRole->GetRoleID() && toState == RoleRoomState_Wait)
        return ChangeRoleRoomState_IsHost;

    if (m_pRoomDanceInfo->HasStart())
        return ChangeRoleRoomState_HasStart;

    pDancer->SetState(ERoleRoomState(toState));

    GameMsg_S2C_ChangeRoleRoomStateSuc msgResponse;
    msgResponse.m_chRoleRoomType = (char)eRoleRoomType;
    msgResponse.m_chRoleRoomPos = (char)nRolePos;
    msgResponse.m_chRoleRoomState = (char)pDancer->GetState();
    RoomGroupCommon::SendMsgToAll(this, msgResponse, 0);

    return ChangeRoleRoomState_Success;
}

EChangeRoomPosStateRes GroupFairyLandRoom::OnChangeRoomPosState(unsigned int nRoleID, ERoleRoomType eRoleType, char roomPos, char roomState)
{
    if (HostRoleID() != nRoleID)
        return ChangeRoomPosState_NotHost;

    if (m_pRoomDanceInfo->HasStart())
        return ChangeRoomPosState_HasStart;

    if (m_pRoomPosManager->HasPlayerInPostion(eRoleType, roomPos))
        return ChangeRoomPosState_Occupied;

    ERoomPosState newState = (roomState == RoomPos_Open ? RoomPos_Open : RoomPos_Close);
    
    // 修改状态：
    m_pRoomPosManager->SetPostionState(eRoleType, roomPos, newState); // 设置状态

    GameMsg_S2C_ChangeRoomPosStateSuc msgResponse;
    msgResponse.m_chRoleRoomType = (char)eRoleType;
    msgResponse.m_chRoleRoomPos = (char)roomPos;
    msgResponse.m_chRoomPosState = (char)newState;
    RoomGroupCommon::SendMsgToAll(this, msgResponse, 0);

    return ChangeRoomPosState_Success;
}

EPromoteRoomHost GroupFairyLandRoom::OnPromoteRoomHost(unsigned int nRoleID, char nNewHostRoomType, char nNewHostPos)
{
    if (HostRoleID() != nRoleID)
        return PromoteRoomHost_NoAuthority;

    if (nNewHostRoomType != RoleRoomType_Dancer)
        return PromoteRoomHost_UnKnow; //

    unsigned int nNewHostRoleID = m_pRoomPosManager->GetPostionRole(RoleRoomType_Dancer, nNewHostPos);
    if (0 == nNewHostRoleID)
        return PromoteRoomHost_NewHostNotInRoom; // 

    ERoleRoomType eOldHostType;
    unsigned int nOldHostPos;
    if (!m_pRoomPosManager->GetRolePos(nRoleID, eOldHostType, nOldHostPos))
        return PromoteRoomHost_UnKnow;
    LWDanceRoleInfo *pOldHost = m_pRoomDanceInfo->GetDanceRole(nRoleID);
    LWDanceRoleInfo *pNewHost = m_pRoomDanceInfo->GetDanceRole(nNewHostRoleID);
    if (NULL == pNewHost || NULL == pOldHost)
        return PromoteRoomHost_UnKnow;

    SetHost(nNewHostRoleID); // 设置新的房主
    m_pRoomDanceInfo->SetDancerState(nNewHostRoleID, RoleRoomState_Ready);
    m_pRoomDanceInfo->SetDancerState(nRoleID, RoleRoomState_Wait);

    GameMsg_S2C_PromoteRoomHostSuc sucMsg;
    sucMsg.m_chOldHostRoomType = (char)eOldHostType;
    sucMsg.m_chOldHostRoomPos = (char)nOldHostPos;
    sucMsg.m_chOldHostState = (char)pOldHost->GetState();
    sucMsg.m_chNewHostRoomType = (char)nNewHostRoomType;
    sucMsg.m_chNewHostRoomPos = (char)nNewHostPos;
    sucMsg.m_chNewHostRoomState = (char)pNewHost->GetState();

    RoomGroupCommon::SendMsgToAll(this, sucMsg, 0);

    return PromoteRoomHost_Sucess;
}

EStartRoomRes GroupFairyLandRoom::StartRoomDanceSpecialTest(unsigned int nRoleID)
{
    // 是否在活动时间
    if (!CDanceGroupFairyLandProcessor::Instance().CheckAllowStart())
        return StartRoom_NotInOpenTime;

    // 检测是不是全部打完了
    if (CDanceGroupFairyLandProcessor::Instance().CheckIsFinishedAllChapter(m_nDanceGroup))
    {
        return StartRoom_NoMoreFailyLandRoomStage;
    }

    // 每个人的次数
    LWDanceRoomPosInfo* pPosInfo = m_pRoomPosManager->GetRoomPosInfo(RoleRoomType_Dancer);
    ENSURE_WITH_LOG_CMD(NULL != pPosInfo, return StartRoom_Unknown, "error get fairy land room posinfo ptr");
    std::vector<std::pair<unsigned int, char> > vecAllRole;
    pPosInfo->GetAllPlayer(vecAllRole);
    for (size_t i = 0; i < vecAllRole.size(); ++i)
    {
        if (!CDanceGroupFairyLandProcessor::Instance().CheckAllowReadyInFairyLandRoom(m_nDanceGroup, vecAllRole[i].first))
            return StartRoom_NoFailyLandRoomTimes;
    }

    // 记录当前轮次
    m_nChapterID = CDanceGroupFairyLandProcessor::Instance().GetCurChapterLvl(m_nDanceGroup); // 记录当前轮次

    // 填充歌曲信息：
    m_pRoomDanceInfo->FillMusicInfo(CDanceGroupFairyLandCfgMgr::Instance().GetChapterSceneId(m_nChapterID),
        CDanceGroupFairyLandCfgMgr::Instance().GetChapterMusicId(m_nChapterID),
        (unsigned char)CDanceGroupFairyLandCfgMgr::Instance().GetChapterDanceType(m_nChapterID),
        (unsigned char)CDanceGroupFairyLandCfgMgr::Instance().GetChapterDifficultyLevel(m_nChapterID));

    return StartRoom_Success;
}

LWRoomDanceInfo* GroupFairyLandRoom::GetRoomDanceInfo() 
{ 
    return m_pRoomDanceInfo; 
}

LWRoomPosManager* GroupFairyLandRoom::GetPosManager()
{
    return m_pRoomPosManager;
}


// 收听房间广播的玩家,roleID, phone type.
void GroupFairyLandRoom::ListenerRole(std::vector<std::pair<unsigned int, char> > &vecListner)
{ // 
    LWDanceRoomPosInfo *pDancerManager = m_pRoomPosManager->GetRoomPosInfo(RoleRoomType_Dancer);
    if (NULL == pDancerManager)
        return;

    pDancerManager->GetAllPlayer(vecListner);
    return;
}

void GroupFairyLandRoom::OnRoomDanceAutoStart()
{
    RoomGroupCommon::OnRoomDanceAutoStart(this, GetRoomDanceInfo(), GetPosManager());
}
void GroupFairyLandRoom::OnRoomDanceAutoEnd()
{
    RoomGroupCommon::OnRoomDanceAutoEnd(this, GetRoomDanceInfo(), GetPosManager());
}

void GroupFairyLandRoom::OnRoomDanceStartCallBack()
{ 
    m_pRoomDanceInfo->OnDanceStartCallBack();
}

void GroupFairyLandRoom::OnRoomDanceEndCallBack()
{ 
    m_pRoomDanceInfo->OnDanceEndCallBack();
    m_pRoomDanceInfo->SetDancerState(HostRoleID(), RoleRoomState_Ready); // 房主变为ready状态

    LWDanceRoomPosInfo *pPosInfo = m_pRoomPosManager->GetRoomPosInfo(RoleRoomType_Dancer);
    if (NULL == pPosInfo)
        return;

    LWDanceRoomPosInfo::RoleVec roles;
    pPosInfo->GetAllPlayer(roles);
    std::list<SDancerScore> listScore;
    for (size_t i = 0; i < roles.size(); ++i)
    {
        SDancerScore sd;
        sd.m_nRoleId = roles[i].first;
        LWDanceRoleInfo *pRoleInfo = m_pRoomDanceInfo->GetDanceRole(sd.m_nRoleId);
        if (NULL != pRoleInfo)
        {
            sd.m_nScore = pRoleInfo->GetTotalScore();
            sd.m_nGrade = pRoleInfo->GetMarkInfo().Grade();
            listScore.push_back(sd);
        }
    }

    CDanceGroupFairyLandProcessor::Instance().DanceOverScoreProcess(m_nDanceGroup, m_nChapterID, listScore);

    return;
}

void GroupFairyLandRoom::OnRoomDestoryCallBack()
{
    RoomGroupCommon::QuitAllDanceRoomRole(this, GetRoomDanceInfo(), GetPosManager());
}


unsigned int GroupFairyLandRoom::RoomSerial() const
{
    return FAIRY_LAND_ROOM_SERIAL_START + GetOriRoomID();
}


void GroupFairyLandRoom::SetDanceGroup(unsigned int nDanceGroup)
{
    m_nDanceGroup = nDanceGroup;
}

unsigned int GroupFairyLandRoom::GetDanceGroup() const
{
    return m_nDanceGroup;
}

