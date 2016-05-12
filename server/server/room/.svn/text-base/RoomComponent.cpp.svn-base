#include "RoomComponent.h"
#include "RoomManager.h"
#include "LiveRoomManager.h"
#include "LiveRoomDataMgr.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../player/PlayerInfoComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../Pet/PetComponent.h"
#include "RoomRewardDataMgr.h"
#include "../item/ItemProcess.h"
#include "../logic/LogicCirculator.h"

#include "../../socket/GameMsg_Map.h"
#include "../../socket/GameMsg_Processor.h"
#include "../roomLib/GameMsg_C2S_RoomMsg.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "GameMsg_C2S_LiveRoomMsg.h"
#include "GameMsg_S2C_LiveRoomMsg.h"
#include "../item/ItemMsgDefine.h"
#include "../util/Split.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/Localization.h"
#include "../anti-addiction/EntityAntiAddictionComponent.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../Dungeon/DungeonMgr.h"
#include "../Dungeon/Dungeon.h"
#include "../grade/GradeInfoMgr.h"
#include "../Pet/PetConfig/PetConfigManager.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Horse/EntityHorseComponent.h"

#include <sstream>

#define ROOMINVITE_INTERVAL 45000


GroupRoomInfo::GroupRoomInfo()
{
    ClearRoom();
}

GroupRoomInfo::~GroupRoomInfo()
{
}
// player is in room.
bool GroupRoomInfo::IsInRoom() const
{
    return !(m_roomType == eRoomType_None && m_nRoomID == 0);
}

// 
void GroupRoomInfo::ClearRoom()
{
    m_roomType = eRoomType_None;
    m_nRoomID = 0; 
}

void GroupRoomInfo::LockRoom(ERoomType roomType)
{
    m_roomType = roomType;
}


CRoomComponent::CRoomComponent(void)
: CEntityComponent(true, true)
, m_bIsHost(false)
, m_nRoleRoomType(RoleRoomType_None)
, m_nRoleRoomPos(-1)
, m_nRoleRoomState(RoleRoomState_None)
, m_nRoleDefaultState(RoleRoomState_Wait)
, m_bIsMatching(false)
, m_nMatchMode(MusicMode_None)
, m_eColorTeam(EDanceColorTeam_None)
, m_lGetInviteeTime(0)
, m_lLastInviteTime(0)
, m_pRoom(NULL)
, m_nRoomColor(0)
, m_bRoomTop(false)
, m_nPhoneOS(OS_None)
, m_nHotValue(0)
, m_nAuditionScore(0)
, m_pRoleNetCOM(NULL)
, m_pRoleAttrCOM(NULL)
, m_pRoleVipCOM(NULL)
, m_pRoleQuestCOM(NULL)
, m_pRoleAchieveCOM(NULL)
, m_pItemCOM(NULL)
, m_pGeneCOM(NULL)
, m_pFriendCOM(NULL)
, m_pDanceGroupCOM(NULL)
, m_pRolePlayerInfo(NULL)
, m_pChatCOM(NULL)
, m_pCoupleCOM(NULL)
, m_pRolePhotoRoom(NULL)
, m_pRoleAmuseRoom(NULL)
, m_pRoleAntiAddiction(NULL)
, m_pRoleQuestNew(NULL)
, m_pRolePet(NULL)
, m_pRoleMedal(NULL)
, m_pRoleHorse(NULL)
{
}

CRoomComponent::~CRoomComponent(void)
{
}

void CRoomComponent::Start()
{
    m_pRoleNetCOM = GetComponent<CRoleNet>();
    m_pRoleAttrCOM = GetComponent<CRoleAttribute>();
    m_pRoleVipCOM = GetComponent<CRoleVIP>();
    m_pRoleQuestCOM = GetComponent<CRoleQuest>();
    m_pRoleAchieveCOM = GetComponent<CRoleAchievement>();
    m_pItemCOM = GetComponent<CRoleItem>();
    m_pGeneCOM = GetComponent<CRoleGene>();
    m_pFriendCOM = GetComponent<CRoleFriend>();
    m_pDanceGroupCOM = GetComponent< CRoleDanceGroup >();
    m_pRolePlayerInfo = GetComponent< CPlayerInfo >();
    m_pChatCOM = GetComponent< CRoleChat >();
    m_pCoupleCOM = GetComponent<CRoleCouple>();
    m_pRolePhotoRoom = GetComponent<CRolePhotoRoom>();
    m_pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
    m_pRoleAntiAddiction = GetComponent<CRoleAntiAddiction>();
    m_pRoleQuestNew = GetComponent<CRoleQuestNew>();
    m_pRolePet = GetComponent<CRolePet>();
    m_pRoleMedal = GetComponent<CRoleMedal>();
    m_pRoleHorse = GetComponent<CRoleHorse>();
}

void CRoomComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EnterLobby);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ExitLobby);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetRoomList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CreateRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QuitRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_StartRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ReadyRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EndRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ApplyMatch);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CancelMatch);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_TryEnterRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EnterRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_KickPlayer);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetInviteeList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_InvitePlayer);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_InviteeResponse);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_InEquip);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_OutEquip);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeRoomInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeMusicInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeRoleRoomType);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeRoleRoomState);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeRoomPosState);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ReportRoundMark);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QuitMarkAward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PromoteRoomHost);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ReportEffectChange);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_LoadingStartRoomProgress);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SwitchDanceRoomTeamMode);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_JoinDanceRoomTeam);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SelectPartner);

    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EnterLiveLobby);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ExitLiveLobby);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetLiveRoomList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CreateLiveRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EnterLiveRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QuitLiveRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CloseLiveRoom);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SetLiveRoomDefaultCloth);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetLiveRoomScore);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_UpdateLiveRoomDancer);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SetLiveRoomDancerToAudience);

    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_OpenVote);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetLiveRoomHotList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetLiveRoomAuditionScoreList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMyAuditionScore);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_Vote);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_LiveRoomJoinAudition);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_LiveRoomCancelAudition);

    RegMsgProcessor(MSG_C2S_EnterLobby, &CRoomComponent::OnEnterLobby);
    RegMsgProcessor(MSG_C2S_ExitLobby, &CRoomComponent::OnExitLobby);
    RegMsgProcessor(MSG_C2S_GetRoomList, &CRoomComponent::OnGetRoomList);
    RegMsgProcessor(MSG_C2S_CreateRoom, &CRoomComponent::OnCreateRoom);
    RegMsgProcessor(MSG_C2S_QuitRoom, &CRoomComponent::OnQuitRoom);
    RegMsgProcessor(MSG_C2S_StartRoom, &CRoomComponent::OnStartRoom);
    RegMsgProcessor(MSG_C2S_ReadyRoom, &CRoomComponent::OnReadyRoom);
    RegMsgProcessor(MSG_C2S_EndRoom, &CRoomComponent::OnEndRoom);
    RegMsgProcessor(MSG_C2S_ApplyMatch, &CRoomComponent::OnApplyMatch);
    RegMsgProcessor(MSG_C2S_CancelMatch, &CRoomComponent::OnCancelMatch);
    RegMsgProcessor(MSG_C2S_TryEnterRoom, &CRoomComponent::OnTryEnterPlayer);
    RegMsgProcessor(MSG_C2S_EnterRoom, &CRoomComponent::OnEnterPlayer);
    RegMsgProcessor(MSG_C2S_KickPlayer, &CRoomComponent::OnKickPlayer);
    RegMsgProcessor(MSG_C2S_GetInviteeList, &CRoomComponent::OnGetInviteeList);
    RegMsgProcessor(MSG_C2S_InvitePlayer, &CRoomComponent::OnInvitePlayer);
    RegMsgProcessor(MSG_C2S_InviteeResponse, &CRoomComponent::OnInviteeResponse);
    RegMsgProcessor(MSG_C2S_InEquip, &CRoomComponent::OnInEquip);
    RegMsgProcessor(MSG_C2S_OutEquip, &CRoomComponent::OnOutEquip);
    RegMsgProcessor(MSG_C2S_ChangeRoomInfo, &CRoomComponent::OnChangeRoomInfo);
    RegMsgProcessor(MSG_C2S_ChangeMusicInfo, &CRoomComponent::OnChangeMusicInfo);
    RegMsgProcessor(MSG_C2S_ChangeRoleRoomType, &CRoomComponent::OnChangeRoleRoomType);
    RegMsgProcessor(MSG_C2S_ChangeRoleRoomState, &CRoomComponent::OnChangeRoleRoomState);
    RegMsgProcessor(MSG_C2S_ChangeRoomPosState, &CRoomComponent::OnChangeRoomPosState);
    RegMsgProcessor(MSG_C2S_ReportRoundMark, &CRoomComponent::OnReportRoundMark);
    RegMsgProcessor(MSG_C2S_QuitMarkAward, &CRoomComponent::OnQuitMarkAward);
    RegMsgProcessor(MSG_C2S_PromoteRoomHost, &CRoomComponent::OnPromoteRoomHost);
    RegMsgProcessor(MSG_C2S_ReportEffectChange, &CRoomComponent::OnReportEffectChange);
    RegMsgProcessor(MSG_C2S_LoadingStartRoomProgress, &CRoomComponent::OnLoadingStartGameProgress);
    RegMsgProcessor(MSG_C2S_SwitchDanceRoomTeamMode, &CRoomComponent::OnSwitchDanceRoomMode);
    RegMsgProcessor(MSG_C2S_JoinDanceRoomTeam, &CRoomComponent::OnJoinDanceRoomTeam);
    RegMsgProcessor(MSG_C2S_SelectPartner, &CRoomComponent::OnSelectPartner);

    RegMsgProcessor(MSG_C2S_EnterLiveLobby, &CRoomComponent::OnEnterLiveLobby);
    RegMsgProcessor(MSG_C2S_ExitLiveLobby, &CRoomComponent::OnExitLiveLobby);
    RegMsgProcessor(MSG_C2S_CreateLiveRoom, &CRoomComponent::OnCreateLiveRoom);
    RegMsgProcessor(MSG_C2S_GetLiveRoomList, &CRoomComponent::OnGetLiveRoomList);
    RegMsgProcessor(MSG_C2S_EnterLiveRoom, &CRoomComponent::OnEnterLiveRoom);
    RegMsgProcessor(MSG_C2S_QuitLiveRoom, &CRoomComponent::OnQuitLiveRoom);
    RegMsgProcessor(MSG_C2S_CloseLiveRoom, &CRoomComponent::OnCloseLiveRoom);
    RegMsgProcessor(MSG_C2S_SetLiveRoomDefaultCloth, &CRoomComponent::OnSetLiveRoomDefaultCloth);
    RegMsgProcessor(MSG_C2S_GetLiveRoomScore, &CRoomComponent::OnGetLiveRoomScore);
    RegMsgProcessor(MSG_C2S_UpdateLiveRoomDancer, &CRoomComponent::OnUpdateLiveRoomDancer);
    RegMsgProcessor(MSG_C2S_SetLiveRoomDancerToAudience, &CRoomComponent::OnSetLiveRoomDancerToAudience);

    RegMsgProcessor(MSG_C2S_OpenVote, &CRoomComponent::OnOpenVote);
    RegMsgProcessor(MSG_C2S_GetLiveRoomHotList, &CRoomComponent::OnGetLiveRoomHotList);
    RegMsgProcessor(MSG_C2S_GetLiveRoomAuditionScoreList, &CRoomComponent::OnGetLiveRoomAuditionScoreList);
    RegMsgProcessor(MSG_C2S_GetMyAuditionScore, &CRoomComponent::OnGetMyAuditionScore);
    RegMsgProcessor(MSG_C2S_Vote, &CRoomComponent::OnVote);
    RegMsgProcessor(MSG_C2S_LiveRoomJoinAudition, &CRoomComponent::OnLiveRoomJoinAudition);
    RegMsgProcessor(MSG_C2S_LiveRoomCancelAudition, &CRoomComponent::OnLiveRoomCancelAudition);

    RegQueryProcessor(Query_LiveRoom_GetPointAndHot, &CRoomComponent::OnGetRolePointAndHot);
}

void CRoomComponent::SendPlayerMsg(GameMsg_Base* pMsg)
{
    if (m_pRoleNetCOM != NULL)
    {
        m_pRoleNetCOM->SendPlayerMsg(pMsg);
    }
}

CRoom* CRoomComponent::Room() const
{
    return m_pRoom;
}

unsigned int CRoomComponent::RoomID() const
{
    if (m_pRoom != NULL)
    {
        return m_pRoom->RoomID();
    }

    return 0;
}

bool CRoomComponent::IsInRoom() const
{
    return IsInGroupRoom() || (NULL != Room());
}


bool CRoomComponent::IsInGroupRoom() const
{
    return m_groupRoomInfo.IsInRoom(); 
}

GroupRoomInfo &CRoomComponent::GetGroupRoomInfo()
{
    return m_groupRoomInfo;
}


void CRoomComponent::InRoom(CRoom* pRoom, EDanceColorTeam eColorTeam, int nRoleType, int nRolePos, int nRoleState, bool bHost)
{
    m_bIsHost = bHost;
    m_nRoleRoomType = nRoleType;
    m_nRoleRoomPos = nRolePos;
    m_nRoleRoomState = nRoleState;
    m_eColorTeam = eColorTeam;

    m_pRoom = pRoom;

    InvitedOver();
}

void CRoomComponent::OutRoom()
{
    m_bIsHost = false;
    m_nRoleRoomType = RoleRoomType_None;
    m_nRoleRoomPos = -1;
    m_nRoleRoomState = RoleRoomState_None;
    m_eColorTeam = EDanceColorTeam_None;

    m_pRoom = NULL;

    m_pRoleHorse->LeaveRoomHorseTask();
}

void CRoomComponent::SetHost(bool bHost)
{
    m_bIsHost = bHost;
}

bool CRoomComponent::IsHost() const
{
    return m_bIsHost;
}

void CRoomComponent::AsHost(bool bIsHost)
{
    m_bIsHost = bIsHost;

    if (m_bIsHost)
    {
        if (m_nRoleRoomState == RoleRoomState_Wait || m_nRoleRoomState == RoleRoomState_Equip)
        {
            m_nRoleRoomState = RoleRoomState_Ready;
        }
    }
    else
    {
        m_nRoleRoomState = m_nRoleDefaultState;
    }
}

int CRoomComponent::RoleRoomType() const
{
    return m_nRoleRoomType;
}

int CRoomComponent::RoleRoomPos() const
{
    return m_nRoleRoomPos;
}

int CRoomComponent::RoleRoomState() const
{
    return m_nRoleRoomState;
}

void CRoomComponent::ChangeRoleState(int nRoleRoomState)
{
    m_nRoleRoomState = nRoleRoomState;
}

void CRoomComponent::ChangeRoleType(int nRoleRoomType, int nRoleRoomPos, int nRoleRoomState)
{
    m_nRoleRoomType = nRoleRoomType;
    m_nRoleRoomPos = nRoleRoomPos;
    m_nRoleRoomState = nRoleRoomState;
}

int CRoomComponent::RoleDefaultState() const
{
    return m_nRoleDefaultState;
}

CRoomMarkInfo& CRoomComponent::RoleMarkInfo()
{
    return m_MarkInfo;
}

void CRoomComponent::UpdateAuditionScore(int nScore)
{
    if (m_nAuditionScore < CLiveRoomDataManager::Instance().m_nBaseScore)
    {
        m_nAuditionScore = CLiveRoomDataManager::Instance().m_nBaseScore;
    }

    m_nAuditionScore += nScore;

    if (m_nAuditionScore < CLiveRoomDataManager::Instance().m_nBaseScore)
    {
        m_nAuditionScore = CLiveRoomDataManager::Instance().m_nBaseScore;
    }
}

bool CRoomComponent::RecvRoundMark(int nMode, int nRound, unsigned int nMark, char chKeyRank, const std::vector<char>& vecRoundRank)
{
    std::vector<int> vecRank;
    for (size_t i = 0; i < vecRoundRank.size(); ++i)
    {
        vecRank.push_back(vecRoundRank[i]);
    }

    bool bRes = m_MarkInfo.AddRoundMark(nMode, nRound, nMark, chKeyRank, vecRank);
    if (bRes && m_pRoom && m_pRoom->PlayMusicMode() == MusicMode_Sweethearts)
    {
        //WriteLog("xindong Role %u RecvRoundMark nMode=%d, nRound=%d, nMark=%u, nTotalMark=%u", 
        //	AttrCOM()->GetRoleID(), nMode, nRound, nMark, m_MarkInfo.Mark());
        m_MarkInfo.AddRoundRank(nRound, vecRank);
    }

    LogRoleCheckingMarkFailed();

    return bRes;
}

bool CRoomComponent::IsMatching() const
{
    return m_bIsMatching;
}

int CRoomComponent::MatchMode() const
{
    return m_nMatchMode;
}

void CRoomComponent::AutoMatch(bool bMatch, int nMode)
{
    m_bIsMatching = bMatch;
    m_nMatchMode = nMode;
}

unsigned long CRoomComponent::GetInviteeTime() const
{
    return m_lGetInviteeTime;
}

void CRoomComponent::SetGetInviteeTime(unsigned long lTime)
{
    m_lGetInviteeTime = lTime;
}

unsigned long CRoomComponent::LastInviteTime() const
{
    return m_lLastInviteTime;
}

void CRoomComponent::SetLastInviteTime(unsigned long lTime)
{
    m_lLastInviteTime = lTime;
}

bool CRoomComponent::IsBeingInvited() const
{
    if (m_mapInvitedRoom.size() > 0)
    {
        return true;
    }

    return false;
}

bool CRoomComponent::IsBeingInvited(unsigned int nRoomID, int nRoomTime) const
{
    std::map<unsigned int, SInvitedRoomInfo>::const_iterator it = m_mapInvitedRoom.find(nRoomID);
    if (it != m_mapInvitedRoom.end() && it->second.m_nRoomTime == nRoomTime)
    {
        return true;
    }

    return false;
}

void CRoomComponent::InvitedOver()
{
    m_mapInvitedRoom.clear();
}

void CRoomComponent::InvitedOver(unsigned int nRoomID)
{
    m_mapInvitedRoom.erase(nRoomID);
}

bool CRoomComponent::TryBeInvited(unsigned int nRoomID, int nRoomTime)
{
    std::map<unsigned int, SInvitedRoomInfo>::iterator it = m_mapInvitedRoom.find(nRoomID);
    if (it != m_mapInvitedRoom.end())
    {
        if (it->second.m_nRoomTime != nRoomTime)
        {
            it->second.m_nRoomTime = nRoomTime;
            it->second.m_nLeftTime = ROOMINVITE_INTERVAL;
        }
        else
        {
            return false;
        }
    }
    else
    {
        SInvitedRoomInfo invitedInfo(nRoomID, nRoomTime, ROOMINVITE_INTERVAL);
        m_mapInvitedRoom.insert(std::make_pair(nRoomID, invitedInfo));
    }

    return true;
}

void CRoomComponent::EncodePlayerInfo(RoomPlayerInfo& rPlayerInfo)
{
    if (m_pRoleAttrCOM != NULL && m_pRoleVipCOM != NULL)
    {
        rPlayerInfo.m_nRoleID = m_pRoleAttrCOM->GetRoleID();
        rPlayerInfo.m_strRoleName = m_pRoleAttrCOM->GetRoleName();
        rPlayerInfo.m_bIsHost = m_bIsHost;
        rPlayerInfo.m_chRoleType = (char)m_nRoleRoomType;
        rPlayerInfo.m_chRolePos = (char)m_nRoleRoomPos;
        rPlayerInfo.m_chRoleState = (char)m_nRoleRoomState;
        rPlayerInfo.m_eColorTeam = m_eColorTeam;
        rPlayerInfo.m_nRoleSex = m_pRoleAttrCOM->GetSex();
        rPlayerInfo.m_nRoleSkin = m_pRoleAttrCOM->GetColor();
        rPlayerInfo.m_nTransFormID = (unsigned short)m_pRoleAttrCOM->GetTransformId();

        CDanceGroupInfo * pDGInfo = m_pDanceGroupCOM->GetDanceGroupInfo();
        if (pDGInfo)
        {
            rPlayerInfo.m_strDanceGroupName = pDGInfo->m_DGBaseInfo.m_strGroupName;
            rPlayerInfo.m_nDanceGroupBadge = pDGInfo->m_DGBaseInfo.m_nBadge;
            rPlayerInfo.m_nDanceGroupEffect = pDGInfo->m_DGBaseInfo.m_nEffect;
            CDanceGroupMember * pDGMember = pDGInfo->GetGroupMember(m_pRoleAttrCOM->GetRoleID());
            if (pDGMember)
            {
                rPlayerInfo.m_nGroupTitle = (char)pDGMember->m_nTitle;
            }
        }
        rPlayerInfo.m_bIsVIP = m_pRoleVipCOM->IsVIP();
        rPlayerInfo.m_nVIPLevel = (unsigned short)m_pRoleVipCOM->VIPLevel();
        rPlayerInfo.m_nMoveType = (m_pRolePlayerInfo != NULL ? m_pRolePlayerInfo->m_position.m_nMoveType : 0);

        RoleRoomImageData rrid;
        ((CRoleEntity*)Entity())->PackRoleRoomImageData(rrid);
        rPlayerInfo.m_strData = rrid.EncodeToString();
    }
}

void CRoomComponent::EncodeInviteeInfo(InivteePlayerInfo& rPlayerInfo) const
{
    if (m_pRoleAttrCOM != NULL)
    {
        rPlayerInfo.m_nRoleID = m_pRoleAttrCOM->GetRoleID();
        rPlayerInfo.m_strRoleName = m_pRoleAttrCOM->GetRoleName();
        rPlayerInfo.m_chRoleLevel = (char)m_pRoleAttrCOM->GetLevel();
        rPlayerInfo.m_chRoleSex = m_pRoleAttrCOM->GetSex();
    }
    if (m_pRoleVipCOM != NULL)
    {
        rPlayerInfo.m_bIsVIP = m_pRoleVipCOM->IsVIP();
        rPlayerInfo.m_nVIPLevel = (unsigned short)m_pRoleVipCOM->VIPLevel();
    }
}

void CRoomComponent::EncodeAudienceInfo(LiveRoomAudienceInfo& rPlayerInfo) const
{
    if (m_pRoleAttrCOM != NULL)
    {
        rPlayerInfo.m_nRoleID = m_pRoleAttrCOM->GetRoleID();
        rPlayerInfo.m_strRoleName = m_pRoleAttrCOM->GetRoleName();
        rPlayerInfo.m_nRoleSex = m_pRoleAttrCOM->GetSex();
        rPlayerInfo.m_bIsVIP = m_pRoleVipCOM->IsVIP();
        rPlayerInfo.m_nVIPLevel = (unsigned short)m_pRoleVipCOM->VIPLevel();
    }
}

void CRoomComponent::SetRoomColor(int nColor)
{
    //if ( nColor >= RoomColor_None && nColor < RoomColor_Max )
    //{
    m_nRoomColor = nColor;
    if (m_pRoom && m_pRoom->RoomHost() == this)
    {
        GameMsg_S2C_ChangeRoomColor msg;
        msg.m_chColor = (unsigned char)nColor;
        m_pRoom->SendMsgToAll(&msg);
    }
    //}
}

int CRoomComponent::GetRoomColor()
{
    return m_nRoomColor;
}

void CRoomComponent::SetRoomTop(bool bTop)
{
    m_bRoomTop = bTop;
}

bool CRoomComponent::GetRoomTop()
{
    return m_bRoomTop;
}

void CRoomComponent::SetPhoneOS(int nPhoneOS)
{
    if (m_nPhoneOS == OS_None)
    {
        m_nPhoneOS = nPhoneOS;
    }
    else if (m_nPhoneOS != nPhoneOS)
    {
        WriteLog(LOGLEVEL_ERROR, "Error: Phone os changed!");
    }
}

int CRoomComponent::GetPhoneOS()
{
    return m_nPhoneOS;
}


void CRoomComponent::SetHotValue(int nHotValue)
{
    m_nHotValue = nHotValue;
}

int CRoomComponent::GetHotValue() const
{
    return m_nHotValue;
}

void CRoomComponent::SetAuditionScore(int nScore)
{
    m_nAuditionScore = nScore;
}

int CRoomComponent::AuditionScore() const
{
    return m_nAuditionScore;
}

void CRoomComponent::SetColorTeam(EDanceColorTeam eColor)
{
    m_eColorTeam = eColor;
}

EDanceColorTeam CRoomComponent::ColorTeam() const
{
    return m_eColorTeam;
}

bool CRoomComponent::AcceptRoomInvitation()
{
    if (m_pRolePlayerInfo != NULL) {
        return m_pRolePlayerInfo->AcceptRoomInvitation();
    }

    return true;
}
//发送关底奖励道具
void CRoomComponent::SendRoomItemReward(int nMusicMode)
{
    if (m_pRoleAttrCOM == NULL)
    {
        return;
    }
    bool bIsMale = m_pRoleAttrCOM->GetSex() == ESexType_Male;
    std::list< CItem > oRewardItemList;
    CRoomRewardDataMgr::Instance().GetCertainItem(bIsMale, oRewardItemList);

#ifndef SERVER_IOS
    int nCertainItemCount = (int)oRewardItemList.size();
    //防沉迷
    if (m_pRoleAntiAddiction != NULL && m_pRoleAntiAddiction->IsActive() && nCertainItemCount > 0)
    {
        if (m_pRoleAntiAddiction->ReCalcEffectData(eAddicEffect_FightItemDropCount, nCertainItemCount))
        {
            std::list< CItem >::iterator it = oRewardItemList.begin();
            std::advance(it, nCertainItemCount);
            oRewardItemList.erase(it, oRewardItemList.end());
        }
    }
#endif //SERVER_IOS

    int nRewardCount = CRoomRewardDataMgr::Instance().GetItemRewardCount();
    if (m_status.HasCastPetSkill())
    {
        int nDropRatio = m_status.GetSkillRewardItemRatio();
        if (nDropRatio > 0)
            nRewardCount += nRewardCount * nDropRatio;
    }

#ifndef SERVER_IOS
    //防沉迷
    if (m_pRoleAntiAddiction != NULL && m_pRoleAntiAddiction->IsActive() && nRewardCount > 0)
    {
        m_pRoleAntiAddiction->ReCalcEffectData(eAddicEffect_FightItemDropCount, nRewardCount);
    }
#endif //SERVER_IOS

    CRoomRewardDataMgr::Instance().GetRandomItem(nMusicMode, bIsMale, nRewardCount, oRewardItemList);

#ifndef SERVER_IOS
    //防沉迷
    if (m_pRoleAntiAddiction != NULL && m_pRoleAntiAddiction->IsActive() && !oRewardItemList.empty())
    {
        std::list< CItem >::iterator it = oRewardItemList.begin();
        for (; it != oRewardItemList.end();)
        {
            ItemConfig* pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_nItemType);
            if ((pItemInfo == NULL) || (pItemInfo->m_anType[0] == eItemClassType_Equip)) {
                it = oRewardItemList.erase(it);
            }
            else{
                ++it;
            }
        }
    }
#endif //SERVER_IOS

    if (!oRewardItemList.empty())
    {
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), oRewardItemList, EItemAction_Add_RoomItemReward, 0, true, EMailType_RoomItemReward,
            CRoomRewardDataMgr::Instance().GetMailTitle(), CRoomRewardDataMgr::Instance().GetMailContent());
        if (m_pChatCOM)
        {
            std::list< CItem >::iterator itBegin = oRewardItemList.begin();
            ItemConfig * pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID((*itBegin).m_nItemType);
            if (pItemInfo)
            {
                std::string strChatTips = CLocalization::Instance().GetString("Room_Reward_Tips");
                m_pChatCOM->SysChat(strChatTips.c_str(), pItemInfo->m_strName.c_str());
            }
        }
        GameMsg_S2C_RoomReward s2cMsg;
        s2cMsg.m_ItemList = oRewardItemList;
        SendPlayerMsg(&s2cMsg);
    }
}

int CRoomComponent::SendExpReward()
{
    int nExp = 0;
    if (m_pRoleAttrCOM != NULL)
        nExp = m_pRoleAttrCOM->AddRateExp(RoleMarkInfo().Exp(), CRoleExpLog::EAddExpCause_Match, 0);
    if (m_pRolePet != NULL)
        m_pRolePet->AddExp(m_pRolePet->GetCarriedPetID(), (unsigned int)m_status.GetBonusExp());
    return nExp;
}

void CRoomComponent::SendRoomCurrencyReward()
{
    if (m_status.HasCastPetSkill())
    {
        m_pRoleAttrCOM->ChangeMoney(m_status.GetSkillRewardMoney(), EChangeMoneyCause_Add_PetSkillRoomReward, 0);
    }
}

unsigned int CRoomComponent::SimulateDanceItemDrop(int nMusicMode, unsigned int nCount)
{
    unsigned int nTimesSuc = 0;
    GameMsg_S2C_SimulateRandomItem msgResult;
    if (m_pRoleAttrCOM != NULL) {
        while (nCount > 0) {
            bool bIsMale = m_pRoleAttrCOM->GetSex() == ESexType_Male;
            std::list< CItem > oRewardItemList;
            CRoomRewardDataMgr::Instance().GetCertainItem(bIsMale, oRewardItemList);
            int nRewardCount = CRoomRewardDataMgr::Instance().GetItemRewardCount();
            CRoomRewardDataMgr::Instance().GetRandomItem(nMusicMode, bIsMale, nRewardCount, oRewardItemList);

            if (!oRewardItemList.empty() && m_pChatCOM != NULL) {
                ++nTimesSuc;
                for (std::list<CItem>::iterator it_boxitem = oRewardItemList.begin(); it_boxitem != oRewardItemList.end(); ++it_boxitem) {
                    std::map<CItem, unsigned int>::iterator it_allitem = msgResult.allItem.find(*it_boxitem);
                    if (it_allitem != msgResult.allItem.end()) {
                        it_allitem->second += 1;
                    }
                    else {
                        msgResult.allItem.insert(std::make_pair(*it_boxitem, 1));
                    }
                }
            }
            --nCount;
        }
    }

    SendPlayerMsg(&msgResult);

    return nTimesSuc;
}

void CRoomComponent::CoupleReward(CRoomComponent* pDanceRoom, CRoomComponent* pCoupleRoom, int nRank)
{
    int nRatio = 1;
    if (m_pRoom && m_pRoom->PlayMusicMode() == MusicMode_Sweethearts)
    {
        nRatio = 2;
    }

    if (m_pCoupleCOM != NULL)
    {
        int nWeddingRingAdd = 0;
        if (pDanceRoom != NULL && pCoupleRoom != NULL)
        {
            if (pDanceRoom->ItemCOM()->GetPlayerWeddingRingItemType() == pCoupleRoom->ItemCOM()->GetPlayerWeddingRingItemType())
            {
                nWeddingRingAdd = pDanceRoom->ItemCOM()->GetCurWeddingRingGeneEffect();
            }
        }

        if (nRank == 1)
        {	//第一名
            m_pCoupleCOM->ChangeIntimacy(C_MATCH_INTIMACY_REWARD_LEVE1*nRatio + nWeddingRingAdd, EChangeIntimacyCause_Match, 0);
            //WriteLog("player[%d] gain %d intimacy from match\n", m_pRoleAttrCOM->GetRoleID(), C_MATCH_INTIMACY_REWARD_LEVE1);
        }
        else
        {
            m_pCoupleCOM->ChangeIntimacy(C_MATCH_INTIMACY_REWARD_LEVE2*nRatio + nWeddingRingAdd, EChangeIntimacyCause_Match, 0);
            //WriteLog("player[%d] gain %d intimacy from match\n", m_pRoleAttrCOM->GetRoleID(), C_MATCH_INTIMACY_REWARD_LEVE2);
        }
    }
}

void CRoomComponent::JoionTeam(EDanceColorTeam eColorTeam)
{
    if (eColorTeam > EDanceColorTeam_None && eColorTeam < EDanceColorTeam_Max) {
        m_eColorTeam = eColorTeam;
    }
}

void CRoomComponent::ExitTeam()
{
    m_eColorTeam = EDanceColorTeam_None;
}

void CRoomComponent::OnStartDance()
{
    m_status.Clear();
    if (m_pRoom != NULL)
    {
        bool bCostEndurance = m_pRolePet->CastPetDanceSkill(m_status.SkillMap());
        m_status.CostEndurance(bCostEndurance);
    }
}

int CRoomComponent::CalcuGrade()
{
    int nGrade = RoomDanceGrade_D;
    if (RoleMarkInfo().TotalRankBase() <= 0)
    {
        return nGrade;
    }

    int nAccuracy = CalcTotalMark() / RoleMarkInfo().TotalRankBase();
    if (nAccuracy >= 100)
    {
        nGrade = RoomDanceGrade_SS;
    }
    else if (nAccuracy >= 90)
    {
        if (RoleMarkInfo().TotalRankPercent(RoomRoundRank_Good) < 1
            && RoleMarkInfo().TotalRankPercent(RoomRoundRank_Bad) < 1
            && RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss) == 0)
        {
            nGrade = RoomDanceGrade_S;
        }
        else
        {
            nGrade = RoomDanceGrade_A;
        }
    }
    else if (nAccuracy >= 80)
    {
        if (RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss) == 0)
        {
            nGrade = RoomDanceGrade_A;
        }
        else
        {
            nGrade = RoomDanceGrade_B;
        }
    }
    else if (nAccuracy >= 70)
    {
        if (RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss) == 0)
        {
            nGrade = RoomDanceGrade_B;
        }
        else
        {
            nGrade = RoomDanceGrade_C;
        }
    }
    else if (nAccuracy >= 60)
    {
        nGrade = RoomDanceGrade_C;
    }

    return nGrade;
}

int CRoomComponent::CalcTotalMark()
{
    int nTotalMark = 0;

    int nMusicMode = MusicMode_None;
    int nMusicLevel = MusicLevel_None;
    if (m_pRoom != NULL)
    {
        nMusicMode = m_pRoom->PlayMusicMode();
        nMusicLevel = m_pRoom->PlayMusicLevel();
    }

    for (int nRank = RoomRoundRank_Miss; nRank < RoomRoundRank_Max; ++nRank)
    {
        int nSkillMark = 0;
        int nRankMark = CalcRankMark(nMusicMode, nMusicLevel, nRank, nSkillMark);

        // 将宠物技能额外增加的基础分加入总分中
        int nTotalSkillMark = RoleMarkInfo().TotalRankCount(nRank) * nSkillMark;
        RoleMarkInfo().AddMark(nTotalSkillMark);

        nTotalMark += RoleMarkInfo().TotalRankCount(nRank) * nRankMark;
    }

    if (m_status.HasCastPetSkill())
    {
        // 将宠物技能额外增加比例的总分加入总分中
        int nSkillMarkRatio = m_status.GetSkillDanceMarkRatio(nMusicMode);
        int nTotalSkillMark = RoleMarkInfo().Mark() * (int)((double)nSkillMarkRatio / 100);
        RoleMarkInfo().AddMark(nTotalSkillMark);

        nTotalMark += nTotalSkillMark;
    }

    return nTotalMark;
}

int CRoomComponent::CalcRankMark(int nMusicMode, int nMusicLevel, int nRank, int &nSkillMark)
{
    nSkillMark = 0;
    int nMark = CGradeInfoManager::Instance().GetGradePercent(nRank);

    // 计算宠物技能额外增加的分数（Perfect点击的基础分）
    if (m_status.HasCastPetSkill() && nRank == RoomRoundRank_Perfect)
    {
        nSkillMark = m_status.GetSkillRankMark(nMusicMode, nMusicLevel, nRank);

        nMark += nSkillMark;
    }

    return nMark;
}

int CRoomComponent::CalcExp(int nDancerCount, int nMusicMode, int nMusicLevel)
{
    int nPlaceExp = CGradeInfoManager::Instance().GetPlaceExp(RoleMarkInfo().Place(), nDancerCount);
    int nGradeExp = CGradeInfoManager::Instance().GetGradeExp(nMusicLevel, RoleMarkInfo().Grade());

    int nTotalExp = nPlaceExp + nGradeExp;

    if (m_pRolePet != NULL && m_pRolePet->HasCarriedPet())
    {
        int nExtraExpRatio = 0;
        if (m_status.HasCastPetSkill())
        {
            nExtraExpRatio = m_status.GetSkillRewardExpRatio(nMusicMode, nMusicLevel);
        }

        nTotalExp += nTotalExp * (int)((double)nExtraExpRatio / 100);

        int nPetDanceBaseExp = PetConfigManager::Instance().GetDanceBaseExp();
        m_status.SetBonusExp(nPetDanceBaseExp + nGradeExp);
    }

    return nTotalExp;
}

CEntityNetComponent* CRoomComponent::NetCOM() const
{
    return m_pRoleNetCOM;
}

void CRoomComponent::SetAttrCOM(CEntityAttributeComponent *pAttrCOM)
{
    m_pRoleAttrCOM = pAttrCOM;
}

CEntityAttributeComponent* CRoomComponent::AttrCOM() const
{
    return m_pRoleAttrCOM;
}

CEntityItemComponent* CRoomComponent::ItemCOM() const
{
    return m_pItemCOM;
}

CEntityVIPComponent* CRoomComponent::VipCOM() const
{
    return m_pRoleVipCOM;
}

CEntityQuestComponent* CRoomComponent::QuestCOM() const
{
    return m_pRoleQuestCOM;
}

CEntityAchievementComponent* CRoomComponent::AchieveCOM() const
{
    return m_pRoleAchieveCOM;
}

CEntityFriendComponent *CRoomComponent::FriendCOM()const
{
    return m_pFriendCOM;
}
CEntityDanceGroupComponent * CRoomComponent::DanceGroupCOM()const
{
    return m_pDanceGroupCOM;
}

CPlayerInfoComponent *CRoomComponent::PlayerInfoCOM()const
{
    return m_pRolePlayerInfo;
}
CEntityChatComponent *CRoomComponent::ChatCOM()const
{
    return m_pChatCOM;
}

CEntityCoupleComponent *CRoomComponent::CoupleCOM()const
{
    return m_pCoupleCOM;
}

CEntityGeneComponent *CRoomComponent::GeneCOM()const
{
    return m_pGeneCOM;
}

CEntityCoupleComponent *CRoomComponent::RoleCouple()const
{
    return m_pCoupleCOM;
}

CEntityPhotoRoomComponent * CRoomComponent::RolePhotoRoom() const
{
    return m_pRolePhotoRoom;
}

CAmuseRoomComponent * CRoomComponent::RoleAmuseRoom() const
{
    return m_pRoleAmuseRoom;
}

CEntityQuestNewComponent * CRoomComponent::RoleQuestNew() const
{
    return m_pRoleQuestNew;
}

void CRoomComponent::OnUpdate(const unsigned long& lTimeElapsed)
{
    for (std::map<unsigned int, SInvitedRoomInfo>::iterator it = m_mapInvitedRoom.begin(); it != m_mapInvitedRoom.end();)
    {
        it->second.m_nLeftTime -= lTimeElapsed;

        if (it->second.m_nLeftTime <= 0)
        {
            m_mapInvitedRoom.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

void CRoomComponent::OnLogin()
{
    CLiveRoomScoreDB *pScore = new CLiveRoomScoreDB;
    pScore->m_nRoleID = m_pRoleAttrCOM->GetRoleID();
    unsigned int nStartTime = (unsigned int)CLiveRoomDataManager::Instance().m_tLiveStartTime;
    unsigned int nEndTime = (unsigned int)CLiveRoomDataManager::Instance().m_tLiveEndTime;

    CDelHelper delHelper(pScore);
    AddQuery(Query_LiveRoom_GetPointAndHot, nStartTime, pScore, nEndTime, NULL, &delHelper);
}

void CRoomComponent::OnGetRolePointAndHot(QueryBase &rQuery)
{
    CLiveRoomScoreDB *pScore = (CLiveRoomScoreDB *)rQuery.m_pAttachment;
    if (pScore != NULL)
    {
        m_nAuditionScore = pScore->m_nPoint;
        m_nHotValue = pScore->m_nHot;
    }
    delete (CLiveRoomScoreDB *)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
}

void CRoomComponent::OnLogout()
{
    CRoomManager::Instance().OnRoleLogout(*this);
    CLiveRoomManager::Instance().OnRoleLogout(*this);
}

void CRoomComponent::OnEnterLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnEnterLobby(rMsg, *this);
}

void CRoomComponent::OnExitLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnExitLobby(rMsg, *this);
}

void CRoomComponent::OnGetRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnGetRoomList(rMsg, *this);
}

void CRoomComponent::OnGetInviteeList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnGetInviteeList(rMsg, *this);
}

void CRoomComponent::OnCreateRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnCreateRoom(rMsg, *this);
}

void CRoomComponent::OnQuitRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnQuitRoom(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnQuitRoom(rMsg, *this);
        }
    }
}

void CRoomComponent::OnStartRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnStartRoom(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnStartRoom(rMsg, *this);
        }
    }
    else if (IsInGroupRoom())
    {
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&rMsg);
    }
}

void CRoomComponent::OnReadyRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnReadyRoom(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnReadyRoom(rMsg, *this);
        }
    }
    else if (IsInGroupRoom())
    {
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&rMsg);
    }
    else
    {
        CommonParam param;
        param.SetParam("room_ready", 1);
        NotifyAll(param);
    }
}

void CRoomComponent::OnEndRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnEndRoom(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnEndRoom(rMsg, *this);
        }
        else if (m_pRoom->IsDungeonRoom())
        {
            CDungeonMgr::Instance().OnParcloseEnd(m_pRoom->RoomID(), m_pRoleAttrCOM->GetRoleID());
        }
    }
    else if (IsInGroupRoom())
    {
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&rMsg);
    }
    else
    {
        CommonParam param;
        param.SetParam("room_end", 1);
        NotifyAll(param);
    }
}

void CRoomComponent::OnTryEnterPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnTryEnterPlayer(rMsg, *this);
}

void CRoomComponent::OnEnterPlayer(GameMsg_Base & rMsg, CSlotPeer & rSlotPeer)
{
    CRoomManager::Instance().OnEnterPlayer(rMsg, *this);
}

void CRoomComponent::OnKickPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnKickPlayer(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnKickPlayer(rMsg, *this);
        }
    }
}

void CRoomComponent::OnApplyMatch(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnApplyMatch(rMsg, *this);
}

void CRoomComponent::OnCancelMatch(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnCancelMatch(rMsg, *this);
}

void CRoomComponent::OnInvitePlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnInvitePlayer(rMsg, *this);
        }
    }
}

void CRoomComponent::OnInviteeResponse(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnInviteeResponse(rMsg, *this);
}

void CRoomComponent::OnInEquip(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnInEquip(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnInEquip(rMsg, *this);
        }
    }
}

void CRoomComponent::OnOutEquip(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnOutEquip(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnOutEquip(rMsg, *this);
        }
    }
}

void CRoomComponent::OnChangeRoomInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnChangeRoomInfo(rMsg, *this);
        }
    }
}

void CRoomComponent::OnChangeMusicInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnChangeMusicInfo(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnChangeMusicInfo(rMsg, *this);
        }
    }
}

void CRoomComponent::OnChangeRoleRoomType(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnChangeRoleRoomType(rMsg, *this);
        }
    }
}

void CRoomComponent::OnChangeRoleRoomState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnChangeRoleRoomState(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnChangeRoleRoomState(rMsg, *this);
        }
    }
}

void CRoomComponent::OnChangeRoomPosState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CRoomManager::Instance().OnChangeRoomPosState(rMsg, *this);
}

void CRoomComponent::OnReportRoundMark(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnReportRoundMark(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnReportRoundMark(rMsg, *this);
        }
        else if (m_pRoom->IsDungeonRoom())
        {
            const GameMsg_C2S_ReportRoundMark& rMsgRequest = (const GameMsg_C2S_ReportRoundMark&)rMsg;
            CDungeonMgr::Instance().OnReportRoundMark((CDungeon*)m_pRoom, m_pRoleAttrCOM->GetRoleID(), rMsgRequest.m_strMD5Code,
                rMsgRequest.m_nMark, rMsgRequest.m_chKeyRank, rMsgRequest.m_nRound, rMsgRequest.m_vecRoundRank);
        }
    }
    else if (IsInGroupRoom())
    {
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&rMsg);
    }
    else
    {
        GameMsg_C2S_ReportRoundMark &rMsgRequest = (GameMsg_C2S_ReportRoundMark &)rMsg;
        std::string strRoundRank;
        ConcatToString(rMsgRequest.m_vecRoundRank, "|", strRoundRank);

        CommonParam param;
        param.SetParam("report_round_mark", 1);
        param.SetParam("MD5_code", rMsgRequest.m_strMD5Code);
        param.SetParam("mark", (int)rMsgRequest.m_nMark);
        param.SetParam("key_rank", (int)rMsgRequest.m_chKeyRank);
        param.SetParam("round", (int)rMsgRequest.m_nRound);
        param.SetParam("round_rank", strRoundRank);
        NotifyAll(param);
    }
}

void CRoomComponent::OnReportEffectChange(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnReportEffectChange(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnReportEffectChange(rMsg, *this);
        }
        else if (m_pRoom->IsDungeonRoom())
        {
            CDungeonMgr::Instance().OnReportEffectChange(rMsg, m_pRoom->RoomID(), m_pRoleAttrCOM->GetRoleID());
        }
    }
    else if (IsInGroupRoom())
    {
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&rMsg);
    }
}
void CRoomComponent::OnQuitMarkAward(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnQuitMarkAward(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnQuitMarkAward(rMsg, *this);
        }
    }
}

void CRoomComponent::OnPromoteRoomHost(GameMsg_Base& rMsg, CSlotPeer&)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnPromoteRoomHost(rMsg, *this);
        }
    }
}

void CRoomComponent::OnLoadingStartGameProgress(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnLoadingStartGameProgress(rMsg, *this);
        }
        else if (m_pRoom->IsLiveRoom() || m_pRoom->IsAuditionRoom())
        {
            CLiveRoomManager::Instance().OnLoadingStartGameProgress(rMsg, *this);
        }
        else if (m_pRoom->IsDungeonRoom())
        {
            CDungeonMgr::Instance().OnLoadingStartGameProgress(rMsg, m_pRoleAttrCOM->GetRoleID(), m_pRoom->RoomID());
        }
    }
    else if (IsInGroupRoom())
    {
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&rMsg);
    }
    else
    {
        GameMsg_C2S_LoadingStartRoomProgress &msg = (GameMsg_C2S_LoadingStartRoomProgress &)rMsg;
        CommonParam param;
        param.SetParam("room_loadingprogress", 1);
        param.SetParam("room_loadingprogress_rate", msg.m_nRate);
        NotifyAll(param);
    }
}

void CRoomComponent::OnSwitchDanceRoomMode(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnSwitchDanceRoomMode(rMsg, *this);
        }
    }
}

void CRoomComponent::OnJoinDanceRoomTeam(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsNormalRoom())
        {
            CRoomManager::Instance().OnJoinDanceRoomTeam(rMsg, *this);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////
void CRoomComponent::OnEnterLiveLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CLiveRoomManager::Instance().OnEnterLiveLobby(rMsg, *this);
}

void CRoomComponent::OnExitLiveLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    CLiveRoomManager::Instance().OnExitLiveLobby(rMsg, *this);
}

void CRoomComponent::OnGetLiveRoomList(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    CLiveRoomManager::Instance().OnGetLiveRoomList(rMsg, *this);
}

void CRoomComponent::OnCreateLiveRoom(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    CLiveRoomManager::Instance().OnCreateLiveRoom(rMsg, *this);
}

void CRoomComponent::OnEnterLiveRoom(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    CLiveRoomManager::Instance().OnEnterLiveRoom(rMsg, *this);
}

void CRoomComponent::OnQuitLiveRoom(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsLiveRoom())
        {
            CLiveRoomManager::Instance().OnQuitLiveRoom(rMsg, *this);
        }
    }
}

void CRoomComponent::OnCloseLiveRoom(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsLiveRoom())
        {
            CLiveRoomManager::Instance().OnCloseLiveRoom(rMsg, *this);
        }
    }
}

void CRoomComponent::OnSetLiveRoomDefaultCloth(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsLiveRoom())
        {
            CLiveRoomManager::Instance().OnSetLiveRoomDefaultCloth(rMsg, *this);
        }
    }
}

void CRoomComponent::OnGetLiveRoomScore(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsLiveRoom())
        {
            CLiveRoomManager::Instance().OnGetLiveRoomScore(rMsg, *this);
        }
    }
}

void CRoomComponent::OnUpdateLiveRoomDancer(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsLiveRoom())
        {
            CLiveRoomManager::Instance().OnUpdateLiveRoomDancer(rMsg, *this);
        }
    }
}

void CRoomComponent::OnSetLiveRoomDancerToAudience(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsLiveRoom())
        {
            CLiveRoomManager::Instance().OnSetLiveRoomDancerToAudience(rMsg, *this);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void CRoomComponent::OnOpenVote(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsLiveRoom())
        {
            CLiveRoomManager::Instance().OnOpenVote(rMsg, *this);
        }
    }
}
void CRoomComponent::OnGetLiveRoomHotList(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    CLiveRoomManager::Instance().OnGetLiveRoomHotList(rMsg, *this);
}
void CRoomComponent::OnGetLiveRoomAuditionScoreList(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    CLiveRoomManager::Instance().OnGetLiveRoomAuditionScoreList(rMsg, *this);
}
void CRoomComponent::OnVote(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    if (m_pRoom != NULL)
    {
        if (m_pRoom->IsLiveRoom())
        {
            CLiveRoomManager::Instance().OnVote(rMsg, *this);
        }
    }
}
void CRoomComponent::OnGetMyAuditionScore(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    CLiveRoomManager::Instance().OnGetMyAuditionScore(rMsg, *this);
}
void CRoomComponent::OnLiveRoomJoinAudition(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    CLiveRoomManager::Instance().OnLiveRoomJoinAudition(rMsg, *this);
}
void CRoomComponent::OnLiveRoomCancelAudition(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    CLiveRoomManager::Instance().OnLiveRoomCancelAudition(rMsg, *this);
}

////////////////////////////////////////////////////////////////////////////////////
void CRoomComponent::OnRoomTest(int nPara1, int nPara2, int nPara3)
{
    int nIndex = -1;
    nIndex = nPara1;
    CSlotPeer slotPeer;
    switch (nIndex)
    {
    case 0:
    {
              GameMsg_C2S_KickPlayer msgKickPlayer;
              msgKickPlayer.m_chRoleRoomType = (char)nPara2;	// 1 dancer 2 audience
              msgKickPlayer.m_chRoleRoomPos = (char)nPara3;

              CRoomManager::Instance().OnKickPlayer(msgKickPlayer, *this);
    }
        break;
    }
}

void CRoomComponent::AddLiveRoomAuditionScoreRankData(int nAddCount)
{
    int nBaseScore = CLiveRoomDataManager::Instance().m_nBaseScore;
    for (int i = 0; i < nAddCount; ++i)
    {
        std::stringstream ss;
        ss << i + 1;
        std::string strName = ss.str();

        // update database
        AddQuery(Query_LiveRoom_UpdateAuditionPoint,
            i + 1, (void *)CLiveRoomDataManager::Instance().m_tLiveStartTime,
            nBaseScore + i + 1, (void *)CLiveRoomDataManager::Instance().m_tLiveEndTime);

        GameMsg_S2G_UpdateDancerPoint groupMsg;
        groupMsg.m_nRoleID = i + 1;
        groupMsg.m_strRoleName = strName;
        groupMsg.m_nAdded = nBaseScore + i + 1;
        SendMsg2GroupServer(&groupMsg);
    }
}

void CRoomComponent::OnSelectPartner(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer)
{
    const GameMsg_C2S_SelectPartner *pMsg = (const GameMsg_C2S_SelectPartner *)&rMsg;

    CRoleEntity *pRole = (CRoleEntity *)(Entity());
    CRoleEntity *pTargetRole = CPlayerManager::Instance().GetEntityByRoleID(pMsg->m_nPartnerRoleID);
    if (pRole != NULL && pTargetRole != NULL)
    {
        CRoom* pRoom = Room();
        if (pRoom && pRoom->PlayMusicMode() == MusicMode_Sweethearts &&
            pRoom->RoomState() == eRoomState_SelectPartner)
        {
            std::string strRes;
            int nRes = pRoom->SelectPartner(pRole->GetRoleID(), pMsg->m_nPartnerRoleID, strRes);
            if (nRes == ESelectPartner_Success)
            {
                GameMsg_S2C_SelectPartner okMsg;
                okMsg.m_nSelectorRoleID = pRole->GetRoleID();
                okMsg.m_nPartnerRoleID = pMsg->m_nPartnerRoleID;
                pRoom->SendMsgToAll(&okMsg);
            }
            else
            {
                CRoleNet* pRoleNet = pRole->GetComponent<CRoleNet>();
                if (pRoleNet)
                {
                    GameMsg_S2C_SelectPartnerFail errMsg;
                    errMsg.m_strError = strRes;
                    errMsg.SessionID = pMsg->SessionID;
                    errMsg.nAccount = pMsg->nAccount;
                    pRoleNet->SendPlayerMsg(&errMsg);
                }
            }
        }
    }
}

void CRoomComponent::LogRoleCheckingMarkFailed()
{
    if (m_pRoom == NULL || m_pRoleAttrCOM == NULL)
        return;

    const CheckingMarkFailedInfo &rInfo = m_MarkInfo.GetCheckingMarkFailedInfo();

    if (!rInfo.HasFailed())
        return;

    CRoleCheckingMarkFailedLog *pLog = new CRoleCheckingMarkFailedLog;

    pLog->m_nRoleID = m_pRoleAttrCOM->GetRoleID();
    pLog->m_nRoomType = (short)m_pRoom->RoomType();
    pLog->m_nScene = m_pRoom->PlayScene();
    pLog->m_nTeamMode = (short)m_pRoom->TeamMode();
    pLog->m_nMusicID = m_pRoom->PlayMusicID();
    pLog->m_nMusicMode = m_pRoom->PlayMusicMode();
    pLog->m_nMusicLevel = m_pRoom->PlayMusicLevel();
    pLog->m_nPlayerCount = m_pRoom->DancerCount();
    pLog->m_nStartTime = m_pRoom->StartTime();
    pLog->m_nRound = rInfo.GetRound();
    pLog->m_nKeyRank = rInfo.GetKeyRank();
    pLog->m_nLastSpecial = rInfo.GetLastSpecial();
    pLog->m_strRoundRank = rInfo.GetRoundRank();
    pLog->m_nMark = rInfo.GetMark();
    pLog->m_nCheckingMark = rInfo.GetCheckingMark();

    AddQuery(QUERY_LogRoleCheckingMarkFailed, 0, pLog);
}

//file end

