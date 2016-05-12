#include "RoomManager.h"
#include "RoomComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../player/PlayerInfoComponent.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../grade/GradeInfoMgr.h"
#include "../Location/LocationMgr.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/DanceGroupDataMgr.h"
#include "../../socket/Md5.h"
#include "../../socket/Formatter.h"
#include "../util/Split.h"
#include "../../socket/Md5.h"
#include "../logic/LogicCirculator.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../music/MusicStageFileManager.h"

#include "../roomLib/GameMsg_C2S_RoomMsg.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../roomLib/RoomSettingMgr.h"
#include "../player/GameMsg_S2G_UpdateRoomInfo.h"

#include "../EntityNetComponent/EntityNetComponent.h"
#include "../Pet/PetComponent.h"
#include "../Pet/PetConfig/PetConfigManager.h"

#include "RoomTimer.h"
#include "../util/TimerMgr.h"
#include "../util/NameDeny.h"

#include <stdio.h>
#include <fstream>
#include <algorithm>

extern CLogicCirculator* g_pCirculator;
extern string g_sPath;


#define	ROOMCOUNT_PER_PAGE		6		//客户端每一页显示的房间个数
#define INVITEELIST_INTERVAL	3000	//获取邀请列表时间间隔:ms
#define INVITEPLAYER_INTERVAL	3000	//邀请玩家时间间隔:ms
#define INVITEPLAYER_COUNT		3		//邀请玩家数量限制
#define MAX_INVITEE_ARRAY_NUM   2048	//最大邀请数组	
#define MAX_INVITEE_COUNT       20      //最大邀请数

extern void SendMsg2GroupServer(LPGameMsg_Base pMsg);


CRoomManager::CRoomManager()
:m_nMgrNowTime(0)
, m_nMatchTime(0)
{
    for (unsigned int i = ERoomRange_Common_Begin; i <= ERoomRange_Common_End; i++)
    {
        m_StoreID.push_back(i);
    }
    for (unsigned int i = ERoomRange_NewPlayer_Begin; i <= ERoomRange_NewPlayer_End; i++)
    {
        m_NewPlayerStoreID.push_back(i);
    }
}

CRoomManager::~CRoomManager()
{
    for (AllRoomMap::iterator it = m_AllRooms[MusicMode_None].begin(); it != m_AllRooms[MusicMode_None].end(); ++it)
    {
        delete it->second;
    }

    for (int nMode = MusicMode_None; nMode < MusicMode_Max; ++nMode)
    {
        m_AllRooms[nMode].clear();
    }

    for (int nMode = MusicMode_None; nMode < MusicMode_Max; ++nMode)
    {
        m_TopRooms[nMode].clear();
    }
}

CRoomManager& CRoomManager::Instance()
{
    static CRoomManager stcRoomMgr;
    return stcRoomMgr;
}
//根据房间号获取房间类型
ERoomType CRoomManager::RoomType(unsigned int nRoomID)
{
    if (nRoomID >= ERoomRange_Common_Begin && nRoomID <= ERoomRange_Common_End)
    {
        return eRoomType_Common;
    }
    else if (nRoomID >= ERoomRange_NewPlayer_Begin && nRoomID <= ERoomRange_NewPlayer_End)
    {
        return eRoomType_NewPlayer;
    }
    else
    {
        return eRoomType_Unknown;
    }
}

CRoom* CRoomManager::FindRoom(unsigned int nRoomID)
{
    AllRoomMap::iterator it = m_AllRooms[MusicMode_None].find(nRoomID);
    if (it != m_AllRooms[MusicMode_None].end() && it->second != NULL)
    {
        return it->second;
    }

    return NULL;
}

void CRoomManager::DestroyRoom(bool bOldTopState, unsigned int nRoomID, int nFlag, int nErr)
{
    CRoom* pRoom = RemoveRoom(nRoomID);
    char chQuitType = RoomQuitType_NoRoom;
    if (nErr != 0)
    {
        chQuitType = (char)nErr;
    }

    if (pRoom != NULL)
    {
        if (pRoom->RoomTop())
        {
            WriteLog(LOGLEVEL_ERROR, "Error: CRoomManager::DestroyRoom. It will cause server crash. ReasonID=%d, DancerCount=%d, AudienceCount=%d, m_szDancer(%p,%p,%p,%p), m_szAudience(%p,%p,%p,%p)",
                nFlag, pRoom->DancerCount(), pRoom->AudienceCount(),
                pRoom->Player(RoleRoomType_Dancer, 0), pRoom->Player(RoleRoomType_Dancer, 1), pRoom->Player(RoleRoomType_Dancer, 2), pRoom->Player(RoleRoomType_Dancer, 3),
                pRoom->Player(RoleRoomType_Audience, 0), pRoom->Player(RoleRoomType_Audience, 1), pRoom->Player(RoleRoomType_Audience, 2), pRoom->Player(RoleRoomType_Audience, 3));
        }
        for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
        {
            CRoomComponent* pRoomCOM = pRoom->PlayerOut(RoleRoomType_Dancer, nPos);
            if (pRoomCOM != NULL)
            {
                pRoomCOM->OutRoom();

                if (pRoomCOM->AttrCOM() != NULL)
                {
                    m_LobbyPlayer.insert(std::make_pair(pRoomCOM->AttrCOM()->GetRoleID(), pRoomCOM));
                    CLocationMgr::Instance().EnterZone(Zone_Lobby, pRoomCOM->AttrCOM()->GetRoleID());
                    _NoticeGroupState(pRoomCOM->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None);
                    if (pRoomCOM->FriendCOM() != NULL) {
                        pRoomCOM->FriendCOM()->PushOnlineInfoToAllFriends();
                    }
                }

                // notice self
                GameMsg_S2C_QuitRoomSuc msgSelfResponse;
                msgSelfResponse.m_chQuitType = chQuitType;
                msgSelfResponse.m_chQuitTo = (char)RoomQuitTo_Lobby;
                pRoomCOM->SendPlayerMsg(&msgSelfResponse);
            }
        }

        for (int nPos = 0; nPos <= pRoom->MaxAudiencePos(); ++nPos)
        {
            CRoomComponent* pRoomCOM = pRoom->PlayerOut(RoleRoomType_Audience, nPos);
            if (pRoomCOM != NULL)
            {
                pRoomCOM->OutRoom();

                if (pRoomCOM->AttrCOM() != NULL)
                {
                    m_LobbyPlayer.insert(std::make_pair(pRoomCOM->AttrCOM()->GetRoleID(), pRoomCOM));
                    CLocationMgr::Instance().EnterZone(Zone_Lobby, pRoomCOM->AttrCOM()->GetRoleID());
                    _NoticeGroupState(pRoomCOM->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None);
                    if (pRoomCOM->FriendCOM() != NULL) {
                        pRoomCOM->FriendCOM()->PushOnlineInfoToAllFriends();
                    }
                }

                // notice self
                GameMsg_S2C_QuitRoomSuc msgSelfResponse;
                msgSelfResponse.m_chQuitType = chQuitType;
                msgSelfResponse.m_chQuitTo = (char)RoomQuitTo_Lobby;
                pRoomCOM->SendPlayerMsg(&msgSelfResponse);
            }
        }
        bool bNewState = pRoom->RoomTop();
        HandleTopRoom(bOldTopState, pRoom);
        {
            // For debug
            TopRoomList::iterator it = find(m_TopRooms[MusicMode_None].begin(), m_TopRooms[MusicMode_None].end(), pRoom);
            if (it != m_TopRooms[MusicMode_None].end())
            {
                WriteLog(LOGLEVEL_ERROR, "Error: CRoomManager::DestroyRoom. Have not delete the top room. ReasonID=%d, OldState=%d, NewState=%d, DancerCount=%d, AudienceCount=%d, m_szDancer(%p,%p,%p,%p), m_szAudience(%p,%p,%p,%p)",
                    nFlag, bOldTopState ? 1 : 0, bNewState ? 1 : 0, pRoom->DancerCount(), pRoom->AudienceCount(),
                    pRoom->Player(RoleRoomType_Dancer, 0), pRoom->Player(RoleRoomType_Dancer, 1), pRoom->Player(RoleRoomType_Dancer, 2), pRoom->Player(RoleRoomType_Dancer, 3),
                    pRoom->Player(RoleRoomType_Audience, 0), pRoom->Player(RoleRoomType_Audience, 1), pRoom->Player(RoleRoomType_Audience, 2), pRoom->Player(RoleRoomType_Audience, 3));
            }

            //临时保护一下
            if (pRoom->RoomTop())
            {
                RemoveTopRoom(pRoom);
            }
            else
            {
                if (!bOldTopState)
                {
                    RemoveTopRoom(pRoom);
                }
            }
        }

        delete pRoom;
        pRoom = NULL;
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "Error: CRoomManager::DestroyRoom. pRoom == NULL");
    }
}

bool CRoomManager::AddRoom(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        if (FindRoom(pRoom->RoomID()) == NULL)
        {
            m_AllRooms[MusicMode_None][pRoom->RoomID()] = pRoom;
            WriteLog(LOGLEVEL_DEBUG, "CRoomManager::AddRoom. Add to all room list. RoomID=%d", pRoom->RoomID());
            m_AllRooms[pRoom->ChoosedMusicMode()][pRoom->RoomID()] = pRoom;

            HandleTopRoom(false, pRoom);

            return true;
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "Error: CRoomManager::AddRoom. The room ID has been used. RoomID=%d", pRoom->RoomID());
        }
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "Error: CRoomManager::AddRoom. pRoom == NULL");
    }

    return false;
}

CRoom* CRoomManager::RemoveRoom(unsigned int nRoomID)
{
    CRoom* pRoom = NULL;
    for (int i = MusicMode_None; i < MusicMode_Max; ++i)
    {
        AllRoomMap::iterator it_Mode = m_AllRooms[i].find(nRoomID);
        if (it_Mode != m_AllRooms[i].end())
        {
            pRoom = it_Mode->second;
            m_AllRooms[i].erase(it_Mode);
            WriteLog(LOGLEVEL_DEBUG, "CRoomManager::RemoveRoom. Remove to all room list. RoomID=%d", pRoom->RoomID());
        }
    }

    if (pRoom != NULL)
    {
        _ReleaseRoomID(pRoom->RoomID());
    }

    return pRoom;
}

void CRoomManager::RemoveTopRoom(CRoom* pRoom)
{
    for (int i = MusicMode_None; i < MusicMode_Max; ++i)
    {
        TopRoomList::iterator it = find(m_TopRooms[i].begin(), m_TopRooms[i].end(), pRoom);
        if (it != m_TopRooms[i].end())
        {
            m_TopRooms[i].erase(it);
        }
    }
}

void CRoomManager::HandleTopRoom(bool bOldTopState, CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        if (pRoom->RoomTop())
        {
            if (!bOldTopState)
            {
                TopRoomList::iterator it = find(m_TopRooms[MusicMode_None].begin(), m_TopRooms[MusicMode_None].end(), pRoom);
                if (it == m_TopRooms[MusicMode_None].end())
                {
                    m_TopRooms[MusicMode_None].push_back(pRoom);
                    WriteLog(LOGLEVEL_DEBUG, "CRoomManager::HandleTopRoom. Add to top room list. RoomID=%d", pRoom->RoomID());

                    if (pRoom->ChoosedMusicMode() != MusicMode_None)
                    {
                        m_TopRooms[pRoom->ChoosedMusicMode()].push_back(pRoom);
                    }
                }
            }
        }
        else
        {
            if (bOldTopState)
            {
                TopRoomList::iterator it = find(m_TopRooms[MusicMode_None].begin(), m_TopRooms[MusicMode_None].end(), pRoom);
                if (it != m_TopRooms[MusicMode_None].end())
                {
                    m_TopRooms[MusicMode_None].erase(it);
                    WriteLog(LOGLEVEL_DEBUG, "CRoomManager::HandleTopRoom. Remove from top room list. RoomID=%d", pRoom->RoomID());

                    TopRoomList::iterator itrMode = find(m_TopRooms[pRoom->ChoosedMusicMode()].begin(), m_TopRooms[pRoom->ChoosedMusicMode()].end(), pRoom);
                    if (itrMode != m_TopRooms[pRoom->ChoosedMusicMode()].end()) {
                        m_TopRooms[pRoom->ChoosedMusicMode()].erase(itrMode);
                    }
                }
            }
        }
    }
}

unsigned long CRoomManager::GetMgrTime()
{
    return m_nMgrNowTime;
}

const CRoomManager::AllRoomMap& CRoomManager::GetRooms(unsigned char nMode)
{
    return m_AllRooms[nMode];
}

const CRoomManager::TopRoomList& CRoomManager::GetTopRooms(unsigned char nMode)
{
    return m_TopRooms[nMode];
}

void CRoomManager::OnUpdate(const unsigned long& lTimeElapsed)
{
    m_nMgrNowTime += lTimeElapsed;

    for (TimeRoomMap::iterator it = m_NeedStartRooms.begin(); it != m_NeedStartRooms.end();)
    {
        if (it->first <= m_nMgrNowTime)
        {
            CRoom* pRoom = it->second;
            m_NeedStartRooms.erase(it++);

            _AutoStart(pRoom);
        }
        else
        {
            break;
        }
    }

    for (TimeRoomMap::iterator it = m_NeedEndRooms.begin(); it != m_NeedEndRooms.end();)
    {
        if (it->first <= m_nMgrNowTime)
        {
            CRoom* pRoom = it->second;
            m_NeedEndRooms.erase(it++);

            _AutoEnd(pRoom);
        }
        else
        {
            break;
        }
    }

    m_nMatchTime += lTimeElapsed;

    if (m_nMatchTime >= (unsigned long)RoomSettingMgr::Instance().AutoMatchInterval())
    {
        m_nMatchTime -= (unsigned long)RoomSettingMgr::Instance().AutoMatchInterval();

        _SpecificModeMatch();
        _RandomModeMatch();
    }
}

void CRoomManager::GetLobbyPlayersID(std::vector<unsigned int> &rPlayersID) const
{
    rPlayersID.clear();

    for (PlayerMap::const_iterator itr = m_LobbyPlayer.begin(); itr != m_LobbyPlayer.end(); ++itr)
    {
        rPlayersID.push_back(itr->first);
    }
}

void CRoomManager::OnEnterLobby(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    if (rRoleRoomCOM.AttrCOM() != NULL)
    {
        m_LobbyPlayer.insert(std::make_pair(rRoleRoomCOM.AttrCOM()->GetRoleID(), &rRoleRoomCOM));
        CLocationMgr::Instance().EnterZone(Zone_Lobby, rRoleRoomCOM.AttrCOM()->GetRoleID());
    }
}

void CRoomManager::OnExitLobby(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    _PlayerOutLobby(rRoleRoomCOM);
}

void CRoomManager::OnGetRoomList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_GetRoomList& rMsgRequest = (const GameMsg_C2S_GetRoomList&)rMsg;
    if (rMsgRequest.m_chMode >= MusicMode_None && rMsgRequest.m_chMode < MusicMode_Max)
    {
        int nObtainCount = 0;
        const CRoom* obtainRooms[ERoomRange_Common_End] = { NULL };

        unsigned short nRoomCount = 0;
        unsigned short nRoomPage = 0;

        const TopRoomList& rTopRooms = GetTopRooms(rMsgRequest.m_chMode);
        for (TopRoomList::const_iterator it = rTopRooms.begin(); it != rTopRooms.end(); ++it)
        {
            CRoom * pRoom = *it;
            if (pRoom)
            {
                if (rMsgRequest.m_bGetAll || !pRoom->HasStart())
                {
                    if (pRoom->RoomType() != eRoomType_NewPlayer)
                    {
                        nRoomPage = nRoomCount / ROOMCOUNT_PER_PAGE;
                        ++nRoomCount;

                        obtainRooms[nObtainCount++] = pRoom;
                    }
                }
            }
        }

        const AllRoomMap& rRooms = GetRooms(rMsgRequest.m_chMode);
        for (AllRoomMap::const_iterator it = rRooms.begin(); it != rRooms.end(); ++it)
        {
            CRoom * pRoom = it->second;
            if (pRoom != NULL)
            {
                if ((rMsgRequest.m_bGetAll || !pRoom->HasStart()) && !pRoom->RoomTop())
                {
                    if (pRoom->RoomType() != eRoomType_NewPlayer)
                    {
                        nRoomPage = nRoomCount / ROOMCOUNT_PER_PAGE;
                        ++nRoomCount;

                        obtainRooms[nObtainCount++] = pRoom;
                    }
                }
            }
        }

        GameMsg_S2C_GetRoomListResult msgResponse;

        int nCurCount = 0;
        int nPage = (nRoomPage < rMsgRequest.m_nPage ? nRoomPage : rMsgRequest.m_nPage);

        for (int i = ROOMCOUNT_PER_PAGE * nPage; i < nObtainCount && nCurCount < ROOMCOUNT_PER_PAGE; ++i)
        {
            ++nCurCount;

            RoomBriefInfo roomInfo;
            obtainRooms[i]->EncodeBriefInfo(roomInfo);

            msgResponse.m_vecRoomInfo.push_back(roomInfo);
        }

        msgResponse.m_nCurPage = (unsigned short)nPage;
        msgResponse.m_nRoomTotalNum = nRoomCount;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}


void CRoomManager::OnGetInviteeList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EGetInviteeListRes eRes = GetInviteeList_Success;
    std::string strError;

    if (rRoleRoomCOM.Room() == NULL)
    {
        eRes = GetInviteeList_NoRoom;
        strError.assign("GetInviteeList_NoRoom");
    }
    else if (!rRoleRoomCOM.IsHost())
    {
        eRes = GetInviteeList_NotHost;
        strError.assign("GetInviteeList_NotHost");
    }
    else
    {
        unsigned long lNowTime = m_nMgrNowTime;
        if (rRoleRoomCOM.GetInviteeTime() > 0 && lNowTime - rRoleRoomCOM.GetInviteeTime() < INVITEELIST_INTERVAL)
        {
            eRes = GetInviteeList_TooFrequent;
            strError.assign("GetInviteeList_TooFrequent");
        }
        else
        {
            int nInviteeCount = 0;
            CRoomComponent * InviteeArray[MAX_INVITEE_ARRAY_NUM] = { NULL };

            rRoleRoomCOM.SetGetInviteeTime(lNowTime);
            GameMsg_S2C_GetInviteeListSuc msgResponse;
            for (PlayerMap::iterator it = m_LobbyPlayer.begin(); it != m_LobbyPlayer.end() && nInviteeCount < MAX_INVITEE_ARRAY_NUM; ++it)
            {
                if (it->second != NULL && it->second->PlayerInfoCOM() != NULL && it->second->PlayerInfoCOM()->AcceptRoomInvitation()) {
                    InviteeArray[nInviteeCount++] = it->second;
                }
            }

            int nCount = nInviteeCount;

            if (nInviteeCount > MAX_INVITEE_COUNT)
            {
                nCount = MAX_INVITEE_COUNT;
                for (int i = 0; i < MAX_INVITEE_COUNT; ++i)
                {
                    int nSwapPos = Random(0, nInviteeCount - 1);
                    if (nSwapPos != i)
                    {
                        swap(InviteeArray[i], InviteeArray[nSwapPos]);
                    }
                }
            }

            for (int i = 0; i < nCount; ++i) {
                InivteePlayerInfo playerInfo;
                InviteeArray[i]->EncodeInviteeInfo(playerInfo);
                msgResponse.m_vecInviteeInfo.push_back(playerInfo);
            }

            rRoleRoomCOM.SendPlayerMsg(&msgResponse);
        }
    }

    if (eRes != GetInviteeList_Success)
    {
        GameMsg_S2C_GetInviteeListFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnCreateRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    ECreateRoomRes eRes = CreateRoom_Success;
    std::string strError;

    const GameMsg_C2S_CreateRoom& rMsgRequest = (const GameMsg_C2S_CreateRoom&)rMsg;
    rRoleRoomCOM.SetPhoneOS(rMsgRequest.m_chPhoneOS);

    if (rRoleRoomCOM.IsInRoom())
    {
        eRes = CreateRoom_InRoom;
        strError.assign("CreateRoom_InRoom");
    }
    else if (rMsgRequest.m_strRoomName.empty())
    {
        eRes = CreateRoom_EmptyName;
        strError.assign("CreateRoom_EmptyName");
    }
    else if (rMsgRequest.m_strRoomName.length() > (size_t)g_ServerConfig.m_nMaxRoomNameLength)
    {
        eRes = CreateRoom_NameTooLong;
        strError.assign("CreateRoom_NameTooLong");
    }
    else if (rMsgRequest.m_strRoomPwd.length() > MAX_ROOM_PASSWD)
    {
        eRes = CreateRoom_PwdTooLong;
        strError.assign("CreateRoom_PwdTooLong");
    }
    else if (rRoleRoomCOM.RoleCouple()->WeddingRoom() != NULL)
    {
        eRes = CreateRoom_InWeddingRoom;
        strError.assign("CreateRoom_InWeddingRoom");
    }
    else if (rRoleRoomCOM.RolePhotoRoom()->PhotoRoom() != NULL)
    {
        eRes = CreateRoom_InPhotoRoom;
        strError.assign("CreateRoom_InPhotoRoom");
    }
    else if (rRoleRoomCOM.RoleAmuseRoom()->AmuseRoom() != NULL)
    {
        eRes = CreateRoom_InAmuseRoom;
        strError.assign("CreateRoom_InAmuseRoom");
    }
    else if (rRoleRoomCOM.DanceGroupCOM()->GetCeremonyRoom() != NULL)
    {
        eRes = CreateRoom_InCeremonyRoom;
        strError.assign("CreateRoom_InCeremonyRoom");
    }
    else
    {
        unsigned int nRoomID = _GenerateRoomID(rMsgRequest.m_chRoomType);
        if (nRoomID == 0)
        {
            eRes = CreateRoom_RoomEnough;
            strError.assign("CreateRoom_RoomEnough");
        }
        else
        {
            std::string strRoomName(rMsgRequest.m_strRoomName);
            NameDeny::Instance().FilterInvalidText(strRoomName);

            CRoom* pRoom = new CRoom((ERoomType)rMsgRequest.m_chRoomType, nRoomID);
            pRoom->ChangeRoomInfo(strRoomName.c_str(), rMsgRequest.m_strRoomPwd.c_str());

            if (_PlayerInRoom(rRoleRoomCOM, pRoom, RoleRoomType_Dancer, RoleRoomState_Ready, true, RoomEnterType_Create) < 0)
            {
                eRes = CreateRoom_Unknow;
                strError.assign("CreateRoom_Unknow");

                _ReleaseRoomID(pRoom->RoomID());
                delete pRoom;
                pRoom = NULL;
            }
            else
            {
                _QuitMatchList(rRoleRoomCOM);
                AddRoom(pRoom);
            }
        }
    }

    if (eRes != CreateRoom_Success)
    {
        GameMsg_S2C_CreateRoomFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnQuitRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EQuitRoomRes eRes = QuitRoom_Success;
    std::string strError;

    CRoom* pRoom = rRoleRoomCOM.Room();
    if (pRoom == NULL)
    {
        eRes = QuitRoom_NoRoom;
        strError.assign("QuitRoom_NoRoom");
    }
    else if (!rRoleRoomCOM.IsHost() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer
        && rRoleRoomCOM.RoleRoomState() == RoleRoomState_Start)
    {
        eRes = QuitRoom_HasStart;
        strError.assign("QuitRoom_HasStart");
    }
    else if (pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
    {
        eRes = QuitRoom_HasStart;
        strError.assign("QuitRoom_HasStart");
    }
    else
    {
        GameMsg_C2S_QuitRoom& rQuitMsg = (GameMsg_C2S_QuitRoom&)rMsg;
        _PlayerOutRoom(rRoleRoomCOM, RoomQuitType_Active, rQuitMsg.m_chQuitTo, NULL);
    }

    if (eRes != QuitRoom_Success)
    {
        GameMsg_S2C_QuitRoomFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnStartRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EStartRoomRes eRes = StartRoom_Success;
    std::string strError;

    CRoom* pRoom = rRoleRoomCOM.Room();
    if (pRoom == NULL)
    {
        eRes = StartRoom_NoRoom;
        strError.assign("StartRoom_NoRoom");
    }
    else if (!rRoleRoomCOM.IsHost())
    {
        eRes = StartRoom_NotHost;
        strError.assign("StartRoom_NotHost");
    }
    else if (pRoom->HasStart())
    {
        eRes = StartRoom_HasStart;
        strError.assign("StartRoom_HasStart");
    }
    else if (!pRoom->IsStartReady())
    {
        eRes = StartRoom_NotReady;
        strError.assign("StartRoom_NotReady");
    }
    else if (pRoom->TeamMode() != EDanceRoomTeamMode_None && !pRoom->IsTeamMatched())
    {
        eRes = StartRoom_NotMatchTeamMode;
        strError.assign("StartRoom_NotMatchTeamMode");
    }
    else if (pRoom->ChoosedMusicMode() == MusicMode_Sweethearts &&
        ((pRoom->MaleDancerCount() != 2) || (pRoom->FemaleDancerCount() != 2)))
    {
        eRes = StartRoom_NotMatchPlayerCount;
        strError.assign("StartRoom_NotMatchPlayerCount");
    }
    else
    {
        pRoom->RoomPrepare();

        std::vector<char> vecAndroidStage;
        std::vector<char> vecIosStage;
        std::vector<char> vecWinPhoneStage;
        const std::string& strStageFile = pRoom->PlayMusicStage();

        if (!strStageFile.empty() && CMusicStageFileManager::LoadStage(strStageFile, vecAndroidStage, vecIosStage, vecWinPhoneStage))
        {
            int nStageTime = CMusicStageFileManager::CalcuStageTime(pRoom->PlayMusicMode(), vecAndroidStage, vecIosStage, vecWinPhoneStage);
            pRoom->SetCheckTime(nStageTime);

            for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
            {
                CRoomComponent* pRoomCOM = pRoom->Player(RoleRoomType_Dancer, nPos);
                if (pRoomCOM != NULL)
                {
                    GameMsg_S2C_PrepareRoom msgResponse;
                    msgResponse.m_chScene = (char)pRoom->PlayScene();
                    msgResponse.m_nMusic = (short)pRoom->PlayMusicID();
                    msgResponse.m_chMode = (char)pRoom->PlayMusicMode();
                    msgResponse.m_chLevel = (char)pRoom->PlayMusicLevel();
                    msgResponse.m_strCheckKey = pRoom->PlayCheckCode();

                    if (pRoomCOM->GetPhoneOS() == OS_Ios)
                    {
                        msgResponse.m_vecStage.assign(vecIosStage.begin(), vecIosStage.end());
                    }
                    else if (pRoomCOM->GetPhoneOS() == OS_WP8)
                    {
                        msgResponse.m_vecStage.assign(vecWinPhoneStage.begin(), vecWinPhoneStage.end());
                    }
                    else
                    {
                        msgResponse.m_vecStage.assign(vecAndroidStage.begin(), vecAndroidStage.end());
                    }

                    pRoomCOM->SendPlayerMsg(&msgResponse);
                }
            }

            for (int nPos = 0; nPos <= pRoom->MaxAudiencePos(); ++nPos)
            {
                CRoomComponent* pRoomCOM = pRoom->Player(RoleRoomType_Audience, nPos);
                if (pRoomCOM != NULL)
                {
                    GameMsg_S2C_PrepareRoom msgResponse;
                    msgResponse.m_chScene = (char)pRoom->PlayScene();
                    msgResponse.m_nMusic = (short)pRoom->PlayMusicID();
                    msgResponse.m_chMode = (char)pRoom->PlayMusicMode();
                    msgResponse.m_chLevel = (char)pRoom->PlayMusicLevel();

                    if (pRoomCOM->GetPhoneOS() == OS_Ios)
                    {
                        msgResponse.m_vecStage.assign(vecIosStage.begin(), vecIosStage.end());
                    }
                    else if (pRoomCOM->GetPhoneOS() == OS_WP8)
                    {
                        msgResponse.m_vecStage.assign(vecWinPhoneStage.begin(), vecWinPhoneStage.end());
                    }
                    else
                    {
                        msgResponse.m_vecStage.assign(vecAndroidStage.begin(), vecAndroidStage.end());
                    }

                    pRoomCOM->SendPlayerMsg(&msgResponse);
                }
            }
            if (pRoom->PlayMusicMode() == MusicMode_Sweethearts)
            {
                StartSweetheartsMode(pRoom, vecAndroidStage, vecIosStage);
            }
        }
        else
        {
            DestroyRoom(pRoom->RoomTop(), pRoom->RoomID(), 1, RoomQuitType_NoMusic);
        }
    }

    if (eRes != StartRoom_Success)
    {
        GameMsg_S2C_StartRoomFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnReadyRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    CRoom* pRoom = rRoleRoomCOM.Room();
    if (pRoom != NULL && pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
    {
        _PlayerChangeState(rRoleRoomCOM, RoleRoomState_ToStart, false);

        if (pRoom->IsPlayReady())
        {
            _OutStartingList(pRoom);
            _RoomStart(pRoom);
        }
        else
        {
            if (pRoom->RoomState() == eRoomState_Preparing)
            {
                pRoom->SetState(eRoomState_Waiting);
                _InStartingList(pRoom);
            }
        }
    }
}

void CRoomManager::OnEndRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    CRoom* pRoom = rRoleRoomCOM.Room();
    if (pRoom != NULL && pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
    {
        int nNowTime = time(NULL);
        int nLegalTime = pRoom->StartTime() + pRoom->GetCheckTime();

        if (nNowTime >= nLegalTime)
        {
            _PlayerChangeState(rRoleRoomCOM, RoleRoomState_ToEnd, false);

            if (pRoom->IsEndReady())
            {
                _OutEndingList(pRoom);
                _RoomEnd(pRoom);
            }
            else
            {
                if (pRoom->RoomState() == eRoomState_Playing)
                {
                    pRoom->SetState(eRoomState_Ending);
                    _InEndingList(pRoom);
                }
            }
        }
        else
        {
            _PlayerOutRoom(rRoleRoomCOM, RoomQuitType_EndKick, RoomQuitTo_Lobby, NULL);
        }
    }
}

void CRoomManager::OnTryEnterPlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_TryEnterRoom& rMsgRequest = (const GameMsg_C2S_TryEnterRoom&)rMsg;
    rRoleRoomCOM.SetPhoneOS(rMsgRequest.m_chPhoneOS);

    CRoom* pRoom = FindRoom(rMsgRequest.m_nRoomID);
    if (pRoom == NULL)
    {
        //EEnterRoomRes eRes = EnterRoom_NoRoom;
        GameMsg_S2C_CreateRoomFail msgResponse;
        msgResponse.m_strError = "EnterRoom_NoRoom";
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
    else if (pRoom->RoomType() == eRoomType_NewPlayer)
    {
        GameMsg_S2C_CreateRoomFail msgResponse;
        msgResponse.m_strError = "EnterRoom_NoRoom";
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
    else if (pRoom->HasStart())
    {
        //EEnterRoomRes eRes = EnterRoom_RoomStart;
        GameMsg_S2C_CreateRoomFail msgResponse;
        msgResponse.m_strError = "EnterRoom_RoomStart";
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
    else
    {
        if (pRoom->HasPwd())
        {
            GameMsg_S2C_RoomRequriePwd reqMsg;
            reqMsg.m_nRoomID = pRoom->RoomID();

            rRoleRoomCOM.SendPlayerMsg(&reqMsg);
        }
        else
        {
            _EnterPlayer(rRoleRoomCOM, pRoom, "", rMsgRequest.m_bAudience);
        }
    }
}

void CRoomManager::OnEnterPlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_EnterRoom& rMsgRequest = (const GameMsg_C2S_EnterRoom&)rMsg;

    CRoom* pRoom = FindRoom(rMsgRequest.m_nRoomID);
    _EnterPlayer(rRoleRoomCOM, pRoom, rMsgRequest.m_strPwd, rMsgRequest.m_bAudience);
}

void CRoomManager::OnKickPlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EKickPlayerRes eRes = KickPlayer_Success;
    std::string strError;

    const GameMsg_C2S_KickPlayer& rMsgRequest = (const GameMsg_C2S_KickPlayer&)rMsg;
    CRoom* pRoom = rRoleRoomCOM.Room();

    if (pRoom == NULL)
    {
        eRes = KickPlayer_NoRoom;
        strError.assign("KickPlayer_NoRoom");
    }
    else if (!rRoleRoomCOM.IsHost() && !rRoleRoomCOM.VipCOM()->IsVIP())
    {
        eRes = KickPlayer_NoAuthority;
        strError.assign("KickPlayer_NoAuthority");
    }
    else if (rRoleRoomCOM.RoleRoomType() == RoleRoomType_Audience)
    {
        eRes = KickPlayer_AudienceCanNotKickPlayer;
        strError.assign("KickPlayer_AudienceCanNotKickPlayer");
    }
    else if (pRoom->HasStart())
    {
        eRes = KickPlayer_HasStart;
        strError.assign("KickPlayer_HasStart");
    }
    else
    {
        CRoomComponent* pPlayer = pRoom->Player(rMsgRequest.m_chRoleRoomType, rMsgRequest.m_chRoleRoomPos);
        if (pPlayer == NULL)
        {
            eRes = KickPlayer_NoPlayer;
            strError.assign("KickPlayer_NoPlayer");
        }
        else if (pPlayer->RoleRoomType() == rRoleRoomCOM.RoleRoomType() && pPlayer->RoleRoomPos() == rRoleRoomCOM.RoleRoomPos())
        {
            eRes = KickPlayer_IsSelf;
            strError.assign("KickPlayer_IsSelf");
        }
        else if (pPlayer == pRoom->RoomHost())
        {
            eRes = KickPlayer_CanNotKickHost;
            strError.assign("KickPlayer_CanNotKickHost");
        }
        else
        {
            if (!rRoleRoomCOM.IsHost())
            {
                if (pPlayer->VipCOM()->IsVIP())
                {
                    if (rRoleRoomCOM.VipCOM()->VIPLevel() <= pPlayer->VipCOM()->VIPLevel())
                    {
                        eRes = KickPlayer_PoorVip;
                        strError.assign("KickPlayer_PoorVip");
                    }
                }
                else
                {
                    if (!pPlayer->AttrCOM()->CanBeVIPKick())
                    {
                        eRes = KickPlayer_CanNotKickNewPlayer;
                        strError.assign("KickPlayer_CanNotKickNewPlayer");
                    }
                }
            }


            if (eRes == KickPlayer_Success)
            {
                ERoomQuitType eQuitType = rRoleRoomCOM.IsHost() ? RoomQuitType_HostKick : RoomQuitType_KickedByVIP;
                _PlayerOutRoom(*pPlayer, eQuitType, RoomQuitTo_Lobby, &rRoleRoomCOM);
            }
        }
    }

    if (eRes != KickPlayer_Success)
    {
        GameMsg_S2C_KickPlayerFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnApplyMatch(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_ApplyMatch& rMsgRequest = (const GameMsg_C2S_ApplyMatch&)rMsg;
    if (rMsgRequest.m_chMode == MusicMode_Sweethearts)
    {
        return;
    }
    rRoleRoomCOM.SetPhoneOS(rMsgRequest.m_chPhoneOS);

    if (rMsgRequest.m_chMode >= MusicMode_None && rMsgRequest.m_chMode < MusicMode_Max)
    {
        MatchList& matchPlayers = m_AllMatchs[(int)rMsgRequest.m_chMode];

        if (rRoleRoomCOM.Room() == NULL && !rRoleRoomCOM.IsMatching() && !rRoleRoomCOM.IsBeingInvited())
        {
            rRoleRoomCOM.AutoMatch(true, rMsgRequest.m_chMode);
            matchPlayers.push_back(&rRoleRoomCOM);
        }
    }
}

void CRoomManager::OnCancelMatch(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    _QuitMatchList(rRoleRoomCOM);
}

void CRoomManager::OnInvitePlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EInvitePlayerRes eRes = InvitePlayer_Success;
    std::string strError;

    CRoom* pRoom = rRoleRoomCOM.Room();
    if (pRoom == NULL)
    {
        eRes = InvitePlayer_NoRoom;
        strError.assign("InvitePlayer_NoRoom");
    }
    else if (pRoom->RoomType() == eRoomType_NewPlayer)
    {
        eRes = InvitePlayer_NewPlayer;
        strError.assign("InvitePlayer_NewPlayer");
    }
    else if (!rRoleRoomCOM.IsHost())
    {
        eRes = InvitePlayer_NotHost;
        strError.assign("InvitePlayer_NotHost");
    }
    else if (rRoleRoomCOM.RoleCouple()->WeddingRoom() != NULL)
    {
        eRes = InvitePlayer_InWeddingRoom;
        strError.assign("InvitePlayer_InWeddingRoom");
    }
    else if (rRoleRoomCOM.RolePhotoRoom()->PhotoRoom() != NULL)
    {
        eRes = InvitePlayer_InPhotoRoom;
        strError.assign("InvitePlayer_InPhotoRoom");
    }
    else if (rRoleRoomCOM.RoleAmuseRoom()->AmuseRoom() != NULL)
    {
        eRes = InvitePlayer_InAmuseRoom;
        strError.assign("InvitePlayer_InAmuseRoom");
    }
    else if (rRoleRoomCOM.DanceGroupCOM()->GetCeremonyRoom() != NULL)
    {
        eRes = InvitePlayer_InCeremonyRoom;
        strError.assign("InvitePlayer_InCeremonyRoom");
    }
    else
    {
        const GameMsg_C2S_InvitePlayer& rMsgRequest = (const GameMsg_C2S_InvitePlayer&)rMsg;

        if (rMsgRequest.m_vecInviteID.size() > INVITEPLAYER_COUNT)
        {
            eRes = InvitePlayer_TooMany;
            strError.assign("InvitePlayer_TooMany");
        }
        else
        {
            unsigned long lNowTime = m_nMgrNowTime;
            if (rRoleRoomCOM.LastInviteTime() > 0 && lNowTime - rRoleRoomCOM.LastInviteTime() < INVITEPLAYER_INTERVAL)
            {
                eRes = InvitePlayer_TooFrequent;
                strError.assign("InvitePlayer_TooFrequent");
            }
            else
            {
                rRoleRoomCOM.SetLastInviteTime(lNowTime);

                // notice self
                GameMsg_S2C_InvitePlayerSuc msgSelfResponse;
                rRoleRoomCOM.SendPlayerMsg(&msgSelfResponse);

                // notice others
                GameMsg_S2C_InviteeNotice msgOtherResponse;
                msgOtherResponse.m_nRoomID = pRoom->RoomID();
                msgOtherResponse.m_chMusicMode = (char)pRoom->ChoosedMusicMode();

                if (rRoleRoomCOM.AttrCOM() != NULL)
                {
                    msgOtherResponse.m_strInviterName = rRoleRoomCOM.AttrCOM()->GetRoleName();
                    msgOtherResponse.m_nInviterRoleID = rRoleRoomCOM.AttrCOM()->GetRoleID();
                }

                if (rRoleRoomCOM.VipCOM() != NULL)
                {
                    msgOtherResponse.m_bIsVIP = rRoleRoomCOM.VipCOM()->IsVIP();
                    msgOtherResponse.m_nVIPLevel = (unsigned short)rRoleRoomCOM.VipCOM()->VIPLevel();
                }

                for (size_t i = 0; i < rMsgRequest.m_vecInviteID.size(); ++i)
                {
                    if (rMsgRequest.m_nInviteType == Invited_Lobby) {
                        CRoomComponent* pPlayerRoomCOM = GetLobbyPlayer(rMsgRequest.m_vecInviteID[i]);

                        if (pPlayerRoomCOM != NULL && pPlayerRoomCOM->AcceptRoomInvitation() && pPlayerRoomCOM->TryBeInvited(pRoom->RoomID(), pRoom->CreateTime()))
                        {
                            pPlayerRoomCOM->SendPlayerMsg(&msgOtherResponse);
                        }
                    }
                    else if (rMsgRequest.m_nInviteType == Invited_Friend)
                    {
                        if (rRoleRoomCOM.FriendCOM() != NULL && rRoleRoomCOM.FriendCOM()->IsFriend(rMsgRequest.m_vecInviteID[i]))
                        {
                            CRoleEntity *pRoleInvited = CPlayerManager::Instance().GetEntityByRoleID(rMsgRequest.m_vecInviteID[i]);
                            if (pRoleInvited != NULL)
                            {
                                CRoleRoom *pRoleRoom = pRoleInvited->GetComponent<CRoleRoom>();
                                if (pRoleRoom != NULL && pRoleRoom->AcceptRoomInvitation() && pRoleRoom->TryBeInvited(pRoom->RoomID(), pRoom->CreateTime()))
                                {
                                    pRoleInvited->SendPlayerMsg(&msgOtherResponse);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (eRes != InvitePlayer_Success)
    {
        GameMsg_S2C_InvitePlayerFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnInviteeResponse(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_InviteeResponse& msgRequest = (const GameMsg_C2S_InviteeResponse&)rMsg;
    rRoleRoomCOM.SetPhoneOS(msgRequest.m_chPhoneOS);

    if (msgRequest.m_bAccept)
    {
        EInviteeResponseRes eRes = InviteeResponse_Success;
        std::string strError;

        if (rRoleRoomCOM.Room() != NULL)
        {
            eRes = InviteeResponse_InRoom;
            strError.assign("InviteeResponse_InRoom");
        }
        else
        {
            CRoom* pRoom = FindRoom(msgRequest.m_nRoomID);
            if (pRoom == NULL)
            {

                eRes = InviteeResponse_NoRoom;
                strError.assign("InviteeResponse_NoRoom");
            }
            else if (pRoom->RoomType() == eRoomType_NewPlayer)
            {
                eRes = InviteeResponse_NewPlayer;
                strError.assign("InviteeResponse_NewPlayer");
            }
            else if (rRoleRoomCOM.RoleCouple()->WeddingRoom() != NULL)
            {
                eRes = InviteeResponse_InWeddingRoom;
                strError.assign("InviteeResponse_InWeddingRoom");
            }
            else if (rRoleRoomCOM.RolePhotoRoom()->PhotoRoom() != NULL)
            {
                eRes = InviteeResponse_InPhotoRoom;
                strError.assign("InviteeResponse_InPhotoRoom");
            }
            else if (rRoleRoomCOM.RoleAmuseRoom()->AmuseRoom() != NULL)
            {
                eRes = InviteeResponse_InAmuseRoom;
                strError.assign("InviteeResponse_InAmuseRoom");
            }
            else if (rRoleRoomCOM.DanceGroupCOM()->GetCeremonyRoom() != NULL)
            {
                eRes = InviteeResponse_InCeremonyRoom;
                strError.assign("InviteeResponse_InCeremonyRoom");
            }
            else
            {
                if (!rRoleRoomCOM.IsBeingInvited(pRoom->RoomID(), pRoom->CreateTime()))
                {
                    eRes = InviteeResponse_NoAuth;
                    strError.assign("InviteeResponse_NoAuth");
                }
                else
                {
                    if (pRoom->HasStart())
                    {
                        eRes = InviteeResponse_HasStart;
                        strError.assign("InviteeResponse_HasStart");
                    }
                    else
                    {
                        if (_PlayerInRoom(rRoleRoomCOM, pRoom, RoleRoomType_Dancer, rRoleRoomCOM.RoleDefaultState(), false, RoomEnterType_Invited) < 0)
                        {
                            eRes = InviteeResponse_NoPos;
                            strError.assign("InviteeResponse_NoPos");
                        }
                        else
                        {
                            _QuitMatchList(rRoleRoomCOM);
                        }
                    }
                }
            }
        }

        if (eRes != InviteeResponse_Success)
        {
            GameMsg_S2C_InviteeFail msgResponse;
            msgResponse.m_strError = strError;
            rRoleRoomCOM.SendPlayerMsg(&msgResponse);
        }
    }

    rRoleRoomCOM.InvitedOver(msgRequest.m_nRoomID);
}

void CRoomManager::OnInEquip(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    if (rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer/* && !rRoleRoomCOM.IsHost()*/)
    {
        CRoom * pRoom = rRoleRoomCOM.Room();
        if (pRoom != NULL && !pRoom->HasStart())
        {
            if (rRoleRoomCOM.RoleRoomState() == RoleRoomState_Wait || rRoleRoomCOM.RoleRoomState() == RoleRoomState_Ready)
            {
                _PlayerChangeState(rRoleRoomCOM, RoleRoomState_Equip, true);
            }
        }
    }
}

void CRoomManager::OnOutEquip(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    if (rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
    {
        CRoom* pRoom = rRoleRoomCOM.Room();
        if (pRoom != NULL && !pRoom->HasStart())
        {
            if (rRoleRoomCOM.RoleRoomState() == RoleRoomState_Equip)
            {
                if (rRoleRoomCOM.IsHost())
                {
                    _PlayerChangeState(rRoleRoomCOM, RoleRoomState_Ready, true);
                }
                else
                {
                    _PlayerChangeState(rRoleRoomCOM, RoleRoomState_Wait, true);
                }
            }
        }
    }
}

void CRoomManager::OnChangeRoomInfo(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EChangeRoomInfoRes eRes = ChangeRoomInfo_Success;
    std::string strError;

    const GameMsg_C2S_ChangeRoomInfo& rMsgRequest = (GameMsg_C2S_ChangeRoomInfo&)rMsg;
    CRoom* pRoom = rRoleRoomCOM.Room();

    if (pRoom == NULL)
    {
        eRes = ChangeRoomInfo_NoRoom;
        strError.assign("ChangeRoomInfo_NoRoom");
    }

    else if (pRoom->HasStart())
    {
        eRes = ChangeRoomInfo_HasStart;
        strError.assign("ChangeRoomInfo_HasStart");
    }
    else if (!rRoleRoomCOM.IsHost())
    {
        eRes = ChangeRoomInfo_NotHost;
        strError.assign("ChangeRoomInfo_NotHost");
    }
    else if (rMsgRequest.m_strRoomName.empty())
    {
        eRes = ChangeRoomInfo_EmptyName;
        strError.assign("ChangeRoomInfo_EmptyName");
    }
    else if (rMsgRequest.m_strRoomName.length() > (size_t)g_ServerConfig.m_nMaxRoomNameLength)
    {
        eRes = ChangeRoomInfo_NameTooLong;
        strError.assign("ChangeRoomInfo_NameTooLong");
    }
    else if (rMsgRequest.m_strRoomPwd.length() > MAX_ROOM_PASSWD)
    {
        eRes = ChangeRoomInfo_PwdTooLong;
        strError.assign("ChangeRoomInfo_PwdTooLong");
    }
    else
    {
        std::string strRoomName(rMsgRequest.m_strRoomName);
        NameDeny::Instance().FilterInvalidText(strRoomName);

        pRoom->ChangeRoomInfo(strRoomName.c_str(), rMsgRequest.m_strRoomPwd.c_str());

        // notice host
        GameMsg_S2C_ChangeRoomInfoSuc msgSelfResponse;
        msgSelfResponse.m_strRoomName.assign(pRoom->RoomName());
        msgSelfResponse.m_strRoomPwd.assign(pRoom->RoomPwd());
        msgSelfResponse.m_bHasPwd = pRoom->HasPwd();
        rRoleRoomCOM.SendPlayerMsg(&msgSelfResponse);

        // notice others
        GameMsg_S2C_ChangeRoomInfoSuc msgOtherResponse;
        msgOtherResponse.m_strRoomName.assign(pRoom->RoomName());
        msgOtherResponse.m_strRoomPwd.assign("");
        msgOtherResponse.m_bHasPwd = pRoom->HasPwd();
        pRoom->SendMsgToAll(&msgOtherResponse, &rRoleRoomCOM);
    }

    if (eRes != ChangeRoomInfo_Success)
    {
        GameMsg_S2C_ChangeRoomInfoFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnChangeMusicInfo(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EChangeMusicInfoRes eRes = ChangeMusicInfo_Success;
    std::string strError;

    const GameMsg_C2S_ChangeMusicInfo& rMsgRequest = (GameMsg_C2S_ChangeMusicInfo&)rMsg;
    CRoom* pRoom = rRoleRoomCOM.Room();

    if (pRoom == NULL)
    {
        eRes = ChangeMusicInfo_NoRoom;
        strError.assign("ChangeMusicInfo_NoRoom");
    }
    else if (pRoom->HasStart())
    {
        eRes = ChangeMusicInfo_HasStart;
        strError.assign("ChangeMusicInfo_HasStart");
    }
    else if (!rRoleRoomCOM.IsHost())
    {
        eRes = ChangeMusicInfo_NotHost;
        strError.assign("ChangeMusicInfo_NotHost");
    }
    else
    {
        if (rMsgRequest.m_chScene != pRoom->ChoosedScene()
            && ConfigManager::Instance().GetSceneModeManager().GetSceneInfo(rMsgRequest.m_chScene) == NULL)
        {
            eRes = ChangeMusicInfo_NoScene;
            strError.assign("ChangeMusicInfo_NoScene");
        }
        else if (rMsgRequest.m_chMode < MusicMode_None || rMsgRequest.m_chMode >= MusicMode_Max)
        {
            eRes = ChangeMusicInfo_NoMode;
            strError.assign("ChangeMusicInfo_NoMode");
        }
        else if (rMsgRequest.m_chLevel < MusicLevel_None || rMsgRequest.m_chLevel >= MusicLevel_Max)
        {
            eRes = ChangeMusicInfo_NoLevel;
            strError.assign("ChangeMusicInfo_NoLevel");
        }
        else
        {

            if (rMsgRequest.m_nMusic != RAN_MUSIC_ID)
            {
                MusicConfig* pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetByID(rMsgRequest.m_nMusic);
                if (pMusicInfo == NULL)
                {
                    eRes = ChangeMusicInfo_NoMusic;
                    strError.assign("ChangeMusicInfo_NoMusic");
                }
                else if (pMusicInfo != NULL && pMusicInfo->m_bIsFresher == false && g_pCirculator->GetServerInstallerID() == INSTALLER_FRESHER)
                {
                    eRes = ChangeMusicInfo_InvalidInstaller;
                    strError.assign("ChangeMusicInfo_InvalidInstaller");
                }
                else if (pMusicInfo != NULL && rMsgRequest.m_chMode != MusicMode_None &&  rMsgRequest.m_chLevel != MusicLevel_None)
                {
                    if (!ConfigManager::Instance().GetMusicStageManager().ExistMusic(rMsgRequest.m_nMusic, rMsgRequest.m_chMode, rMsgRequest.m_chLevel))
                    {
                        eRes = ChangeMusicInfo_NoMusic;
                        strError.assign("ChangeMusicInfo_NoMusic");
                    }
                    else
                    {
                        int nOldMode = pRoom->ChoosedMusicMode();
                        pRoom->ChangeChoosedScene(rMsgRequest.m_chScene);
                        pRoom->ChangeChoosedMusic(rMsgRequest.m_nMusic, rMsgRequest.m_chMode, rMsgRequest.m_chLevel);
                        _RoomSwitch(nOldMode, pRoom);

                        GameMsg_S2C_ChangeMusicInfoSuc msgResponse;
                        msgResponse.m_chScene = (char)pRoom->ChoosedScene();
                        msgResponse.m_nMusic = (short)pRoom->ChoosedMusicID();
                        msgResponse.m_chMode = (char)pRoom->ChoosedMusicMode();
                        msgResponse.m_chLevel = (char)pRoom->ChoosedMusicLevel();
                        pRoom->SendMsgToAll(&msgResponse);
                    }
                }
                else if (pMusicInfo != NULL)
                {
                    int nOldMode = pRoom->ChoosedMusicMode();
                    pRoom->ChangeChoosedScene(rMsgRequest.m_chScene);
                    pRoom->ChangeChoosedMusic(rMsgRequest.m_nMusic, rMsgRequest.m_chMode, rMsgRequest.m_chLevel);
                    _RoomSwitch(nOldMode, pRoom);

                    GameMsg_S2C_ChangeMusicInfoSuc msgResponse;
                    msgResponse.m_chScene = (char)pRoom->ChoosedScene();
                    msgResponse.m_nMusic = (short)pRoom->ChoosedMusicID();
                    msgResponse.m_chMode = (char)pRoom->ChoosedMusicMode();
                    msgResponse.m_chLevel = (char)pRoom->ChoosedMusicLevel();
                    pRoom->SendMsgToAll(&msgResponse);
                }
            }
            else
            {
                int nOldMode = pRoom->ChoosedMusicMode();
                pRoom->ChangeChoosedScene(rMsgRequest.m_chScene);
                pRoom->ChangeChoosedMusic(rMsgRequest.m_nMusic, rMsgRequest.m_chMode, rMsgRequest.m_chLevel);
                _RoomSwitch(nOldMode, pRoom);

                GameMsg_S2C_ChangeMusicInfoSuc msgResponse;
                msgResponse.m_chScene = (char)pRoom->ChoosedScene();
                msgResponse.m_nMusic = (short)pRoom->ChoosedMusicID();
                msgResponse.m_chMode = (char)pRoom->ChoosedMusicMode();
                msgResponse.m_chLevel = (char)pRoom->ChoosedMusicLevel();
                pRoom->SendMsgToAll(&msgResponse);
            }
        }
    }

    if (eRes != ChangeMusicInfo_Success)
    {
        GameMsg_S2C_ChangeMusicInfoFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnChangeRoleRoomType(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EChangeRoleRoomTypeRes eRes = ChangeRoleRoomType_Success;
    std::string strError;

    const GameMsg_C2S_ChangeRoleRoomType& rMsgRequest = (const GameMsg_C2S_ChangeRoleRoomType&)rMsg;
    CRoom* pRoom = rRoleRoomCOM.Room();

    if (pRoom == NULL)
    {
        eRes = ChangeRoleRoomType_NoRoom;
        strError.assign("ChangeRoleRoomType_NoRoom");
    }
    else if (rRoleRoomCOM.IsHost())
    {
        eRes = ChangeRoleRoomType_IsHost;
        strError.assign("ChangeRoleRoomType_IsHost");
    }
    else if (pRoom->HasStart())
    {
        eRes = ChangeRoleRoomType_HasStart;
        strError.assign("ChangeRoleRoomType_HasStart");
    }
    else if (rMsgRequest.m_chRoleRoomType <= RoleRoomType_None || rMsgRequest.m_chRoleRoomType >= RoleRoomType_Max)
    {
        eRes = ChangeRoleRoomType_NoType;
        strError.assign("ChangeRoleRoomType_NoType");
    }
    else if (rRoleRoomCOM.RoleRoomType() == rMsgRequest.m_chRoleRoomType)
    {
        eRes = ChangeRoleRoomType_NoChange;
        strError.assign("ChangeRoleRoomType_NoChange");
    }
    else if (rRoleRoomCOM.RoleRoomState() == RoleRoomState_Ready && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
    {
        eRes = ChangeRoleRoomType_IsReady;
        strError.assign("ChangeRoleRoomType_IsReady");
    }
    else
    {
        if (_PlayerChangeType(rRoleRoomCOM, rMsgRequest.m_chRoleRoomType) < 0)
        {
            eRes = ChangeRoleRoomType_NoPos;
            strError.assign("ChangeRoleRoomType_NoPos");
        }
    }

    if (eRes != ChangeRoleRoomType_Success)
    {
        GameMsg_S2C_ChangeRoleRoomTypeFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnChangeRoleRoomState(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EChangeRoleRoomStateRes eRes = ChangeRoleRoomState_Success;
    std::string strError;

    const GameMsg_C2S_ChangeRoleRoomState& rMsgRequest = (const GameMsg_C2S_ChangeRoleRoomState&)rMsg;
    CRoom* pRoom = rRoleRoomCOM.Room();

    if (pRoom == NULL)
    {
        eRes = ChangeRoleRoomState_NoRoom;
        strError.assign("ChangeRoleRoomState_NoRoom");
    }
    else if (rRoleRoomCOM.IsHost())
    {
        eRes = ChangeRoleRoomState_IsHost;
        strError.assign("ChangeRoleRoomState_IsHost");
    }
    else if (rRoleRoomCOM.RoleRoomType() != RoleRoomType_Dancer)
    {
        eRes = ChangeRoleRoomState_NotDancer;
        strError.assign("ChangeRoleRoomState_NotDancer");
    }
    else if (pRoom->HasStart())
    {
        eRes = ChangeRoleRoomState_HasStart;
        strError.assign("ChangeRoleRoomState_HasStart");
    }
    else if (rMsgRequest.m_chRoleRoomState <= RoleRoomState_None || rMsgRequest.m_chRoleRoomState >= RoleRoomState_ToStart)
    {
        eRes = ChangeRoleRoomState_NoState;
        strError.assign("ChangeRoleRoomState_NoState");
    }
    else if (rRoleRoomCOM.RoleRoomState() == rMsgRequest.m_chRoleRoomState)
    {
        eRes = ChangeRoleRoomState_NoChange;
        strError.assign("ChangeRoleRoomState_NoChange");
    }
    else
    {
        _PlayerChangeState(rRoleRoomCOM, rMsgRequest.m_chRoleRoomState, true);
    }

    if (eRes != ChangeRoleRoomState_Success)
    {
        GameMsg_S2C_ChangeRoleRoomStateFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnChangeRoomPosState(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    EChangeRoomPosStateRes eRes = ChangeRoomPosState_Success;
    std::string strError;

    const GameMsg_C2S_ChangeRoomPosState& rMsgRequest = (const GameMsg_C2S_ChangeRoomPosState&)rMsg;
    CRoom* pRoom = rRoleRoomCOM.Room();

    if (pRoom == NULL)
    {
        eRes = ChangeRoomPosState_NoRoom;
        strError.assign("ChangeRoomPosState_NoRoom");
    }
    else if (!rRoleRoomCOM.IsHost())
    {
        eRes = ChangeRoomPosState_NotHost;
        strError.assign("ChangeRoomPosState_NotHost");
    }
    else if (pRoom->HasStart())
    {
        eRes = ChangeRoomPosState_HasStart;
        strError.assign("ChangeRoomPosState_HasStart");
    }
    else if (pRoom->Player(rMsgRequest.m_chRoleRoomType, rMsgRequest.m_chRoleRoomPos) != NULL)
    {
        eRes = ChangeRoomPosState_Occupied;
        strError.assign("ChangeRoomPosState_Occupied");
    }
    else if (rMsgRequest.m_chRoomPosState < RoomPos_Open || rMsgRequest.m_chRoomPosState > RoomPos_Close)
    {
        eRes = ChangeRoomPosState_NoState;
        strError.assign("ChangeRoomPosState_NoState");
    }
    else
    {
        if (!_ChangeRoomPosState(pRoom, rMsgRequest.m_chRoleRoomType, rMsgRequest.m_chRoleRoomPos, rMsgRequest.m_chRoomPosState))
        {
            eRes = ChangeRoomPosState_NoChange;
            strError.assign("ChangeRoomPosState_NoChange");
        }
    }

    if (eRes != ChangeRoomPosState_Success)
    {
        GameMsg_S2C_ChangeRoomPosStateFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}

void CRoomManager::OnPromoteRoomHost(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    GameMsg_C2S_PromoteRoomHost & rPromoteMsg = (GameMsg_C2S_PromoteRoomHost &)rMsg;

    EPromoteRoomHost eRes = PromoteRoomHost_None;
    string strError;

    CRoom *pRoom = rRoleRoomCOM.Room();
    if (pRoom == NULL)
    {
        eRes = PromoteRoomHost_RoomNotExist;
        strError = "PromoteRoomHost_RoomNotExist";
    }
    else if (!rRoleRoomCOM.IsHost())
    {
        eRes = PromoteRoomHost_NoAuthority;
        strError = "PromoteRoomHost_NoAuthority";
    }
    else if (rPromoteMsg.m_chNewHostRoleRoomType != RoleRoomType_Dancer)
    {
        eRes = PromoteRoomHost_UnKnow;
        strError = "PromoteRoomHost_UnKnow";
    }
    else
    {
        bool bRoomTopState = pRoom->RoomTop();
        CRoomComponent *pNewHost = pRoom->ReassignHost(RoleRoomType_Dancer, rPromoteMsg.m_chNewHostRoleRoomPos);
        if (pNewHost == NULL)
        {
            eRes = PromoteRoomHost_NewHostNotInRoom;
            strError = "PromoteRoomOwner_NotInRoom";
        }
        else
        {
            rRoleRoomCOM.AsHost(false);
            pNewHost->AsHost(true);

            // self
            GameMsg_S2C_PromoteRoomHostSuc sucMsg;
            sucMsg.m_chOldHostRoomType = (char)rRoleRoomCOM.RoleRoomType();
            sucMsg.m_chOldHostRoomPos = (char)rRoleRoomCOM.RoleRoomPos();
            sucMsg.m_chOldHostState = (char)rRoleRoomCOM.RoleRoomState();
            sucMsg.m_chNewHostRoomType = (char)pNewHost->RoleRoomType();
            sucMsg.m_chNewHostRoomPos = (char)pNewHost->RoleRoomPos();
            sucMsg.m_chNewHostRoomState = (char)pNewHost->RoleRoomState();
            sucMsg.m_strRoomPwd = pRoom->RoomPwd();
            pNewHost->SendPlayerMsg(&sucMsg);

            // others
            sucMsg.m_strRoomPwd = "";
            pRoom->SendMsgToAll(&sucMsg, pNewHost);

            GameMsg_S2C_ChangeRoomColor colorMsg;
            colorMsg.m_chColor = (unsigned char)pRoom->RoomColor();
            pRoom->SendMsgToAll(&colorMsg);
        }

        HandleTopRoom(bRoomTopState, pRoom);
    }

    if (eRes != PromoteRoomHost_None)
    {
        GameMsg_S2C_PromoteRoomHostFail failMsg;
        failMsg.m_strError = strError;

        rRoleRoomCOM.SendPlayerMsg(&failMsg);
    }

}

void CRoomManager::OnLoadingStartGameProgress(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_LoadingStartRoomProgress & rMsgLoaddingRcv = (const GameMsg_C2S_LoadingStartRoomProgress &)rMsg;
    CRoom *pRoom = rRoleRoomCOM.Room();
    if (pRoom != NULL && (rRoleRoomCOM.RoleRoomState() == RoleRoomState_Ready || rRoleRoomCOM.RoleRoomState() == RoleRoomState_ToStart))
    {
        GameMsg_S2C_LoadingStartRoomProgress msgLoadingSend;
        if (rRoleRoomCOM.AttrCOM() != NULL) {
            msgLoadingSend.m_nRoleID = rRoleRoomCOM.AttrCOM()->GetRoleID();
            msgLoadingSend.m_nRate = rMsgLoaddingRcv.m_nRate >= 100 ? 100 : rMsgLoaddingRcv.m_nRate;
            pRoom->SendMsgToAll(&msgLoadingSend);
        }
    }
}

void CRoomManager::OnSwitchDanceRoomMode(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    CRoom *pRoom = rRoleRoomCOM.Room();
    if (pRoom != NULL && pRoom->RoomState() == eRoomState_Idle)
    {
        GameMsg_C2S_SwitchDanceRoomTeamMode & msgSwitch = (GameMsg_C2S_SwitchDanceRoomTeamMode &)rMsg;
        ESwitchDanceRoomTeamModeRes eRes = ESwitchDanceRoomTeamModeRes_Suc;
        std::string strError;

        if (!_CanSwitchRoomTeamMode(rRoleRoomCOM, (EDanceRoomTeamMode)msgSwitch.m_nDanceRoomTeamMode, (EDanceColorTeam)msgSwitch.m_nDanceRoomColorTeam, eRes, strError)) {
            GameMsg_S2C_SwitchTeamModeFail msgFail;
            msgFail.m_eRes = eRes;
            msgFail.m_strError.assign(strError);
            rRoleRoomCOM.SendPlayerMsg(&msgFail);
        }
        else
        {
            pRoom->SwitchTeamMode((EDanceRoomTeamMode)msgSwitch.m_nDanceRoomTeamMode, (EDanceColorTeam)msgSwitch.m_nDanceRoomColorTeam);
            GameMsg_S2C_UpdateWholeDanceRoomTeamInfo msgWholeRoomTeamInfo;
            pRoom->EncodeWholeRoomTeamInfo(msgWholeRoomTeamInfo.m_WholeRoomTeamInfo);
            pRoom->SendMsgToAll(&msgWholeRoomTeamInfo);
        }
    }
}

void CRoomManager::OnJoinDanceRoomTeam(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    CRoom *pRoom = rRoleRoomCOM.Room();
    if (pRoom != NULL && pRoom->RoomState() == eRoomState_Idle)
    {
        GameMsg_C2S_JoinDanceRoomTeam &rMsgJoin = (GameMsg_C2S_JoinDanceRoomTeam &)rMsg;
        EJoinDanceRoomTeamRes eRes = EJoinDanceRoomTeamRes_Suc;
        std::string strError;

        if (!_CanJoinTeam(rRoleRoomCOM, (EDanceColorTeam)rMsgJoin.m_nRoomColorTeam, eRes, strError)) {
            GameMsg_S2C_JoinDanceRoomTeamFail msgFail;
            msgFail.m_eError = eRes;
            msgFail.m_strEroor.assign(strError);
            rRoleRoomCOM.SendPlayerMsg(&msgFail);
            return;
        }

        pRoom->PlayerJoinTeam(rRoleRoomCOM, (EDanceColorTeam)rMsgJoin.m_nRoomColorTeam);
        GameMsg_S2C_UpdateRoleDanceRoomTeamInfo msgUpdate;
        msgUpdate.m_SelfRoomTeamInfo.m_nRoleID = rRoleRoomCOM.AttrCOM() == NULL ? 0 : rRoleRoomCOM.AttrCOM()->GetRoleID();
        msgUpdate.m_SelfRoomTeamInfo.m_eRoomTeamMode = pRoom->TeamMode();
        msgUpdate.m_SelfRoomTeamInfo.m_eRoomColorTeam = (EDanceColorTeam)rMsgJoin.m_nRoomColorTeam;
        pRoom->SendMsgToAll(&msgUpdate);
    }
}

void CRoomManager::OnReportRoundMark(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_ReportRoundMark& rMsgRequest = (const GameMsg_C2S_ReportRoundMark&)rMsg;
    if (rMsgRequest.m_chKeyRank > RoomRoundRank_None && rMsgRequest.m_chKeyRank < RoomRoundRank_Max && rMsgRequest.m_nRound > 0)
    {
        CRoom* pRoom = rRoleRoomCOM.Room();

        if (pRoom != NULL && pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
        {
            std::string strSrcCode;
            Formatter(strSrcCode) << pRoom->PlayCheckCode() << (int)rMsgRequest.m_nRound
                << (int)rMsgRequest.m_chKeyRank << rMsgRequest.m_nMark;

            for (size_t i = 0; i < rMsgRequest.m_vecRoundRank.size(); ++i)
            {
                std::string strRank;
                Formatter(strRank) << (int)rMsgRequest.m_vecRoundRank[i];
                strSrcCode.append(strRank);
            }

            unsigned char szMD5Code[16] = { 0 };
            MDString(strSrcCode.c_str(), szMD5Code);

            std::string strCheckCode = BinaryData2String(szMD5Code, sizeof(szMD5Code));
            if (rMsgRequest.m_strMD5Code.compare(strCheckCode) == 0)
            {
                bool bIsSweetheartsMode = false;
                int nMusicMode = pRoom->PlayMusicMode();
                if (nMusicMode == MusicMode_Sweethearts)
                {
                    bIsSweetheartsMode = true;
                    nMusicMode = pRoom->GetMusicModeByRound(rMsgRequest.m_nRound);
                    if (nMusicMode == MusicMode_None)
                    {
                        //WriteLog("xindong OnReportRoundMark error round %d", (int)rMsgRequest.m_chRound);
                        return;
                    }
                }
                if (rRoleRoomCOM.RecvRoundMark(nMusicMode,
                    rMsgRequest.m_nRound, rMsgRequest.m_nMark, rMsgRequest.m_chKeyRank, rMsgRequest.m_vecRoundRank))
                {
                    if (pRoom->TeamMode() != EDanceRoomTeamMode_None) {
                        pRoom->AddTeamMarkInfo(rRoleRoomCOM.ColorTeam(), rMsgRequest.m_nMark);
                    }
                    GameMsg_S2C_SyncRoundMark msgResponse;
                    msgResponse.m_chDancerPos = (char)rRoleRoomCOM.RoleRoomPos();
                    msgResponse.m_chRound = (char)rRoleRoomCOM.RoleMarkInfo().Round();
                    msgResponse.m_chRank = (char)rRoleRoomCOM.RoleMarkInfo().LastRank();//for model3 ,this is AllComboFlag; > 1 = AllCombo;
                    msgResponse.m_nMark = rRoleRoomCOM.RoleMarkInfo().Mark();
                    msgResponse.m_nSpecial = rRoleRoomCOM.RoleMarkInfo().LastSpecialCount();
                    pRoom->SendMsgToAll(&msgResponse, &rRoleRoomCOM);
                    if (bIsSweetheartsMode)
                    {
                        pRoom->UpdateSweethearsInfo();
                    }

                    //fufeng to do: award, achievement, quest
                    rRoleRoomCOM.AchieveCOM()->RoomPerRoundScoreReport(rRoleRoomCOM, rMsgRequest.m_nMark, rMsgRequest.m_chKeyRank);
                }
            }
        }
    }
}

void CRoomManager::OnReportEffectChange(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    const GameMsg_C2S_ReportEffectChange rMsgRequest = (const GameMsg_C2S_ReportEffectChange&)rMsg;
    CRoom* pRoom = rRoleRoomCOM.Room();
    if (pRoom != NULL && pRoom->HasStart() && rRoleRoomCOM.RoleRoomType() == RoleRoomType_Dancer)
    {
        std::string strSrcCode;
        Formatter(strSrcCode) << pRoom->PlayCheckCode() << rMsgRequest.m_nMark
            << rMsgRequest.m_nSpecail << (int)rMsgRequest.m_chRound;

        unsigned char szMD5Code[16] = { 0 };
        MDString(strSrcCode.c_str(), szMD5Code);

        std::string strCheckCode = BinaryData2String(szMD5Code, sizeof(szMD5Code));
        if (rMsgRequest.m_strMD5Code.compare(strCheckCode) == 0)
        {
            GameMsg_S2C_SyncEffectState msgResponse;
            msgResponse.m_nMark = rMsgRequest.m_nMark;
            msgResponse.m_nSpecial = rMsgRequest.m_nSpecail;
            msgResponse.m_chRound = rMsgRequest.m_chRound;
            msgResponse.m_nDancerPos = (char)rRoleRoomCOM.RoleRoomPos();

            pRoom->SendMsgToAll(&msgResponse, &rRoleRoomCOM);
        }
    }
}
void CRoomManager::OnQuitMarkAward(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM)
{
    CRoom* pRoom = rRoleRoomCOM.Room();

    if (pRoom != NULL && rRoleRoomCOM.RoleRoomState() == RoleRoomState_End)
    {
        int nState = (rRoleRoomCOM.IsHost() ? RoleRoomState_Ready : rRoleRoomCOM.RoleDefaultState());
        _PlayerChangeState(rRoleRoomCOM, nState, true);
    }
}

void CRoomManager::OnRoleLogout(CRoomComponent& rRoleRoomCOM)
{
    _QuitMatchList(rRoleRoomCOM);
    _PlayerOutLobby(rRoleRoomCOM);
    _PlayerOutRoom(rRoleRoomCOM, RoomQuitType_Logout, RoomQuitTo_World, NULL);
}

void CRoomManager::SendMsgInLobby(GameMsg_Base* pMsg)
{
    for (PlayerMap::iterator it = m_LobbyPlayer.begin(); it != m_LobbyPlayer.end(); ++it)
    {
        if (it->second != NULL)
        {
            it->second->SendPlayerMsg(pMsg);
        }
    }
}

void CRoomManager::SendLobbyChatMsg(unsigned int nSenderID, GameMsg_Base *pMsg)
{
    for (PlayerMap::iterator it = m_LobbyPlayer.begin(); it != m_LobbyPlayer.end(); ++it)
    {
        if (it->second != NULL)
        {
            if (it->second->FriendCOM() != NULL && !it->second->FriendCOM()->IsInBlackList(nSenderID)) {
                it->second->SendPlayerMsg(pMsg);
            }
        }
    }
}

void CRoomManager::ExitLobby(CRoomComponent& rRoleRoomCOM)
{
    _PlayerOutLobby(rRoleRoomCOM);
}

unsigned int CRoomManager::_GenerateRoomID(unsigned char nRoomType)
{
    unsigned int nRoomID = 0;

    switch (nRoomType)
    {
    case eRoomType_Common:
    {
                             nRoomID = _GenerateCommonRoomID();
    }
        break;
    case eRoomType_NewPlayer:
    {
                                nRoomID = _GenerateNewPlayerRoomID();
    }
        break;
    default:
    {
    }
        break;
    }

    return nRoomID;
}
unsigned int CRoomManager::_GenerateCommonRoomID()
{
    unsigned int nRoomID = 0;
    if (!m_IdleID.empty())
    {
        RoomIDList::iterator it = m_IdleID.begin();

        nRoomID = *it;
        m_IdleID.erase(it);
    }
    else if (!m_StoreID.empty())
    {
        RoomIDList::iterator it = m_StoreID.begin();

        nRoomID = *it;
        m_StoreID.erase(it);
    }
    return nRoomID;
}

unsigned int CRoomManager::_GenerateNewPlayerRoomID()
{
    unsigned int nRoomID = 0;
    if (!m_NewPlayerIdleID.empty())
    {
        RoomIDList::iterator it = m_NewPlayerIdleID.begin();

        nRoomID = *it;
        m_NewPlayerIdleID.erase(it);
    }
    else if (!m_NewPlayerStoreID.empty())
    {
        RoomIDList::iterator it = m_NewPlayerStoreID.begin();

        nRoomID = *it;
        m_NewPlayerStoreID.erase(it);
    }
    return nRoomID;
}

void CRoomManager::_ReleaseRoomID(unsigned int nRoomId)
{
    if (CRoomManager::RoomType(nRoomId) == eRoomType_Common)
    {
        m_IdleID.push_back(nRoomId);
    }
    else if (CRoomManager::RoomType(nRoomId) == eRoomType_NewPlayer)
    {
        m_NewPlayerIdleID.push_back(nRoomId);
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "Error: CRoomManager::_ReleaseRoomID. Release an unknown roomid(%u)", nRoomId);
    }
}

void CRoomManager::_EnterPlayer(CRoomComponent& rRoleRoomCOM, CRoom *pRoom, const std::string& strPwd, bool bAudience)
{
    EEnterRoomRes eRes = EnterRoom_Success;
    std::string strError;

    if (rRoleRoomCOM.IsInRoom())
    {
        eRes = EnterRoom_InRoom;
        strError.assign("EnterRoom_InRoom");
    }
    else if (pRoom == NULL)
    {
        eRes = EnterRoom_NoRoom;
        strError.assign("EnterRoom_NoRoom");
    }
    else if (pRoom->RoomType() == eRoomType_NewPlayer)
    {
        eRes = EnterRoom_NewPlayer;
        strError.assign("EnterRoom_NewPlayer");
    }
    else if (pRoom->HasStart())
    {
        eRes = EnterRoom_RoomStart;
        strError.assign("EnterRoom_RoomStart");
    }
    else if (pRoom->HasPwd() && pRoom->RoomPwd().compare(strPwd) != 0)
    {
        eRes = EnterRoom_IncorrectPwd;
        strError.assign("EnterRoom_WrongPwd");
    }
    else if (rRoleRoomCOM.RoleCouple()->WeddingRoom() != NULL)
    {
        eRes = EnterRoom_InWeddingRoom;
        strError.assign("EnterRoom_InWeddingRoom");
    }
    else if (rRoleRoomCOM.RolePhotoRoom()->PhotoRoom() != NULL)
    {
        eRes = EnterRoom_InPhotoRoom;
        strError.assign("EnterRoom_InPhotoRoom");
    }
    else if (rRoleRoomCOM.RoleAmuseRoom()->AmuseRoom() != NULL)
    {
        eRes = EnterRoom_InAmuseRoom;
        strError.assign("EnterRoom_InAmuseRoom");
    }
    else if (rRoleRoomCOM.DanceGroupCOM()->GetCeremonyRoom() != NULL)
    {
        eRes = EnterRoom_InCeremonyRoom;
        strError.assign("EnterRoom_InCeremonyRoom");
    }
    else
    {
        int nRoleRoomType = (bAudience ? RoleRoomType_Audience : RoleRoomType_Dancer);

        if (_PlayerInRoom(rRoleRoomCOM, pRoom, nRoleRoomType, rRoleRoomCOM.RoleDefaultState(), false, RoomEnterType_Join) < 0)
        {
            eRes = (bAudience ? EnterRoom_AudienceFull : EnterRoom_DancerFull);
            strError.assign((bAudience ? "EnterRoom_AudienceFull" : "EnterRoom_DancerFull"));
        }
        else
        {
            _QuitMatchList(rRoleRoomCOM);
        }
    }

    if (eRes != EnterRoom_Success)
    {
        GameMsg_S2C_CreateRoomFail msgResponse;
        msgResponse.m_strError = strError;
        rRoleRoomCOM.SendPlayerMsg(&msgResponse);
    }
}


int CRoomManager::_PlayerInRoom(CRoomComponent& rRoleRoomCOM, CRoom* pRoom, int nRoleType, int nRoleState, bool bHost, int nInType)
{
    int nPos = -1;

    if (pRoom != NULL)
    {
        if (rRoleRoomCOM.Room() != NULL)
        {
            WriteLog(LOGLEVEL_ERROR, "Player in room error, already in a room, in type: %d", nInType);
        }

        EDanceColorTeam eColorTeam = (pRoom->TeamMode() == EDanceRoomTeamMode_None) ? EDanceColorTeam_None : pRoom->GetAutoSelectedColorTeam();

        nPos = pRoom->PlayerIn(rRoleRoomCOM, nRoleType, nRoleState, bHost);

        if (nPos >= 0)
        {
            rRoleRoomCOM.InRoom(pRoom, eColorTeam, nRoleType, nPos, nRoleState, bHost);

            if (rRoleRoomCOM.AttrCOM() != NULL)
            {
                m_LobbyPlayer.erase(rRoleRoomCOM.AttrCOM()->GetRoleID());
                CLocationMgr::Instance().RemoveEntity(Zone_Lobby, rRoleRoomCOM.AttrCOM()->GetRoleID());
                _NoticeGroupState(rRoleRoomCOM.AttrCOM()->GetRoleID(), pRoom->RoomID(), nRoleType, nRoleState);
                if (rRoleRoomCOM.FriendCOM()) {
                    rRoleRoomCOM.FriendCOM()->PushOnlineInfoToAllFriends();
                }
            }

            if (nInType != RoomEnterType_Match)
            {
                // notice self
                GameMsg_S2C_CreateRoomSuc msgSelfResponse;
                pRoom->EncodeWholeInfo(msgSelfResponse.m_RoomInfo, bHost);
                rRoleRoomCOM.SendPlayerMsg(&msgSelfResponse);

                // notice others, bHost is true only when player is creating room, there is no others in that case
                if (!bHost)
                {
                    GameMsg_S2C_EnterPlayer msgOtherResponse;
                    rRoleRoomCOM.EncodePlayerInfo(msgOtherResponse.m_PlayerInfo);
                    pRoom->SendMsgToAll(&msgOtherResponse, &rRoleRoomCOM);
                }
            }
        }
    }

    return nPos;
}

void CRoomManager::_PlayerOutRoom(CRoomComponent& rOutRoleRoomCOM, int nOutType, int nOutTo, CRoomComponent * pKickedPlayerRoomCOM)
{
    CRoom* pRoom = rOutRoleRoomCOM.Room();
    if (pRoom != NULL && pRoom->IsNormalRoom())
    {
        bool bRoomTopState = pRoom->RoomTop();

        int nRoleRoomType = rOutRoleRoomCOM.RoleRoomType();
        int nRoleRoomPos = rOutRoleRoomCOM.RoleRoomPos();
        CRoomComponent* pPlayer = pRoom->PlayerOut(nRoleRoomType, nRoleRoomPos);

        if (pPlayer != NULL)
        {
            _LogRoleDancing(pPlayer->AttrCOM()->GetRoleID(), CRoleDancingLog::EDancingAction_End,
                pRoom->PlayScene(), pRoom->TeamMode(), pRoom->PlayMusicID(), pRoom->PlayMusicMode(), pRoom->PlayMusicLevel(),
                pRoom->DancerCount(), -1, 0);
            pPlayer->OutRoom();

            // notice player
            if (nOutType != RoomQuitType_Logout)
            {
                if (pPlayer->AttrCOM() != NULL)
                {
                    m_LobbyPlayer.insert(std::make_pair(pPlayer->AttrCOM()->GetRoleID(), pPlayer));
                    CLocationMgr::Instance().EnterZone(Zone_Lobby, pPlayer->AttrCOM()->GetRoleID());
                    _NoticeGroupState(pPlayer->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None);

                    if (pPlayer->FriendCOM() != NULL) {
                        pPlayer->FriendCOM()->PushOnlineInfoToAllFriends();
                    }
                }

                GameMsg_S2C_QuitRoomSuc msgPlayerResponse;
                msgPlayerResponse.m_chQuitType = (char)nOutType;
                msgPlayerResponse.m_chQuitTo = (char)nOutTo;
                if (nOutType == RoomQuitType_KickedByVIP && pKickedPlayerRoomCOM != NULL) {
                    msgPlayerResponse.m_nRoleIDKicked = pKickedPlayerRoomCOM->AttrCOM()->GetRoleID();
                }
                pPlayer->SendPlayerMsg(&msgPlayerResponse);
            }


            if (nOutTo == (int)RoomQuitTo_World)
            {
                _PlayerOutLobby(rOutRoleRoomCOM);
            }

            // check whether to destroy room
            if (pRoom->HasDancer())
            {
                int nHostRoomType = RoleRoomType_None;
                int nHostRoomPos = -1;
                int nHostRoomState = RoleRoomState_None;

                //check whether to reassign host
                CRoomComponent* pHost = NULL;
                if (pRoom->RoomHost() == NULL)
                {
                    pHost = pRoom->AssignHost();
                    if (pHost != NULL)
                    {
                        pHost->AsHost(true);
                        nHostRoomType = pHost->RoleRoomType();
                        nHostRoomPos = pHost->RoleRoomPos();
                        nHostRoomState = pHost->RoleRoomState();

                        GameMsg_S2C_QuitPlayer msgHostResponse;
                        msgHostResponse.m_chQuitType = (char)nOutType;
                        msgHostResponse.m_chRoleRoomType = (char)nRoleRoomType;
                        msgHostResponse.m_chRoleRoomPos = (char)nRoleRoomPos;
                        msgHostResponse.m_chHostRoomType = (char)nHostRoomType;
                        msgHostResponse.m_chHostRoomPos = (char)nHostRoomPos;
                        msgHostResponse.m_chHostRoomState = (char)nHostRoomState;
                        msgHostResponse.m_strRoomPwd.assign(pRoom->RoomPwd());
                        pHost->SendPlayerMsg(&msgHostResponse);

                        GameMsg_S2C_ChangeRoomColor colorMsg;
                        colorMsg.m_chColor = (unsigned char)pRoom->RoomColor();
                        pHost->SendPlayerMsg(&colorMsg);
                    }

                    HandleTopRoom(bRoomTopState, pRoom);
                }

                // notice others
                GameMsg_S2C_QuitPlayer msgOtherResponse;
                msgOtherResponse.m_chQuitType = (char)nOutType;
                msgOtherResponse.m_chRoleRoomType = (char)nRoleRoomType;
                msgOtherResponse.m_chRoleRoomPos = (char)nRoleRoomPos;
                msgOtherResponse.m_chHostRoomType = (char)nHostRoomType;
                msgOtherResponse.m_chHostRoomPos = (char)nHostRoomPos;
                msgOtherResponse.m_chHostRoomState = (char)nHostRoomState;
                msgOtherResponse.m_strRoomPwd.assign("");

                pRoom->SendMsgToAll(&msgOtherResponse, pHost);

                if (pRoom->RoomState() == eRoomState_Waiting && pRoom->IsPlayReady())
                {
                    _OutStartingList(pRoom);
                    _RoomStart(pRoom);
                }

                if (pRoom->RoomState() == eRoomState_Ending && pRoom->IsEndReady())
                {
                    _OutEndingList(pRoom);
                    _RoomEnd(pRoom);
                }
            }
            else
            {
                if (pRoom->RoomState() == eRoomState_Waiting)
                {
                    _OutStartingList(pRoom);
                }
                else if (pRoom->RoomState() == eRoomState_Ending)
                {
                    _OutEndingList(pRoom);
                }

                DestroyRoom(bRoomTopState, pRoom->RoomID(), 2);
            }
        }
        else
        {
            CEntityAttributeComponent* pAttrCOM = rOutRoleRoomCOM.AttrCOM();
            if (pAttrCOM != NULL)
            {
                WriteLog(LOGLEVEL_ERROR, "Player quit room error, role id: %d, role name: %s, room id: %u, quit type: %d",
                    pAttrCOM->GetRoleID(), pAttrCOM->GetRoleName(), pRoom->RoomID(), nOutType);
            }
        }
    }
}

int CRoomManager::_PlayerChangeType(CRoomComponent& rRoleRoomCOM, int nRoleType)
{
    CRoom* pRoom = rRoleRoomCOM.Room();
    int nPos = -1;

    if (pRoom != NULL)
    {
        int nOriType = rRoleRoomCOM.RoleRoomType();
        int nOriPos = rRoleRoomCOM.RoleRoomPos();

        nPos = pRoom->PlayerChangeType(rRoleRoomCOM, nRoleType);
        if (nPos >= 0)
        {
            if (nRoleType == RoleRoomType_Dancer) {
                if (pRoom->TeamMode() != EDanceRoomTeamMode_None) {
                    EDanceColorTeam eColorTeam = pRoom->GetAutoSelectedColorTeam();
                    pRoom->PlayerJoinTeam(rRoleRoomCOM, eColorTeam);
                }
            }
            else if (nRoleType == RoleRoomType_Audience) {
                pRoom->PlayerExitTeam(rRoleRoomCOM);
            }

            int nState = (nRoleType == RoleRoomType_Dancer ? rRoleRoomCOM.RoleDefaultState() : RoleRoomState_Ready);
            rRoleRoomCOM.ChangeRoleType(nRoleType, nPos, nState);

            if (rRoleRoomCOM.AttrCOM() != NULL)
            {
                _NoticeGroupState(rRoleRoomCOM.AttrCOM()->GetRoleID(), pRoom->RoomID(), nRoleType, nState);
            }

            GameMsg_S2C_ChangeRoleRoomTypeSuc msgResponse;
            msgResponse.m_chSrcRoleRoomType = (char)nOriType;
            msgResponse.m_chSrcRoleRoomPos = (char)nOriPos;
            msgResponse.m_chDesRoleRoomType = (char)rRoleRoomCOM.RoleRoomType();
            msgResponse.m_chDesRoleRoomPos = (char)rRoleRoomCOM.RoleRoomPos();
            msgResponse.m_chRoleRoomState = (char)rRoleRoomCOM.RoleRoomState();
            msgResponse.m_eColorTeam = rRoleRoomCOM.ColorTeam();
            pRoom->SendMsgToAll(&msgResponse);
        }
    }

    return nPos;
}

void CRoomManager::_PlayerChangeState(CRoomComponent& rRoleRoomCOM, int nRoleState, bool bNotice)
{
    CRoom* pRoom = rRoleRoomCOM.Room();
    if (pRoom != NULL)
    {
        rRoleRoomCOM.ChangeRoleState(nRoleState);

        if (rRoleRoomCOM.AttrCOM() != NULL)
        {
            _NoticeGroupState(rRoleRoomCOM.AttrCOM()->GetRoleID(), pRoom->RoomID(), rRoleRoomCOM.RoleRoomType(), nRoleState);
        }

        if (bNotice)
        {
            GameMsg_S2C_ChangeRoleRoomStateSuc msgResponse;
            msgResponse.m_chRoleRoomType = (char)rRoleRoomCOM.RoleRoomType();
            msgResponse.m_chRoleRoomPos = (char)rRoleRoomCOM.RoleRoomPos();
            msgResponse.m_chRoleRoomState = (char)nRoleState;
            pRoom->SendMsgToAll(&msgResponse);
        }
    }
}

void CRoomManager::_SpecificModeMatch()
{
    MatchList& matchRandom = m_AllMatchs[MusicMode_None];

    for (int nMode = MusicMode_None + 1; nMode < MusicMode_Max; ++nMode)
    {
        bool bContinue = true;
        MatchList& matchMode = m_AllMatchs[nMode];

        while (bContinue && matchMode.size() >= (size_t)RoomSettingMgr::Instance().AutoMatchCount())
        {
            bool bIsFresherInstaller = (g_pCirculator->GetServerInstallerID() == INSTALLER_FRESHER ? true : false);
            int nScene = ConfigManager::Instance().GetSceneModeManager().RandomScene(bIsFresherInstaller);

            CRoom* pRoom = _CreateMatchRoom(nScene, nMode, MusicLevel_Easy);
            if (pRoom != NULL)
            {
                int dancerCount = matchMode.size();
                if (dancerCount > MAX_ROOM_PLAYER_COUNT)
                {
                    dancerCount = MAX_ROOM_PLAYER_COUNT;
                }

                for (int i = 0; i < dancerCount; ++i)
                {
                    CRoomComponent* pPlayerRoomCOM = matchMode.front();
                    matchMode.pop_front();

                    if (pPlayerRoomCOM != NULL && pPlayerRoomCOM->Room() == NULL)
                    {
                        pPlayerRoomCOM->AutoMatch(false, MusicMode_None);
                        _PlayerInRoom(*pPlayerRoomCOM, pRoom, RoleRoomType_Dancer, RoleRoomState_Ready, false, RoomEnterType_Match);
                    }
                }

                _PrepareMatchRoom(pRoom);
            }
            else
            {
                bContinue = false;
            }
        }

        if (bContinue && !matchMode.empty() && matchRandom.size() + matchMode.size() >= (size_t)RoomSettingMgr::Instance().AutoMatchCount())
        {
            bool bIsFresherInstaller = (g_pCirculator->GetServerInstallerID() == INSTALLER_FRESHER ? true : false);
            int nScene = ConfigManager::Instance().GetSceneModeManager().RandomScene(bIsFresherInstaller);

            CRoom* pRoom = _CreateMatchRoom(nScene, nMode, MusicLevel_Easy);
            if (pRoom != NULL)
            {
                int dancerCount = matchRandom.size() + matchMode.size();
                if (dancerCount > MAX_ROOM_PLAYER_COUNT)
                {
                    dancerCount = MAX_ROOM_PLAYER_COUNT;
                }

                for (int i = 0; i < dancerCount; ++i)
                {
                    CRoomComponent* pPlayerRoomCOM = NULL;
                    if (!matchMode.empty())
                    {
                        pPlayerRoomCOM = matchMode.front();
                        matchMode.pop_front();
                    }
                    else
                    {
                        pPlayerRoomCOM = matchRandom.front();
                        matchRandom.pop_front();
                    }

                    if (pPlayerRoomCOM != NULL && pPlayerRoomCOM->Room() == NULL)
                    {
                        pPlayerRoomCOM->AutoMatch(false, MusicMode_None);
                        _PlayerInRoom(*pPlayerRoomCOM, pRoom, RoleRoomType_Dancer, RoleRoomState_Ready, false, RoomEnterType_Match);
                    }
                }

                _PrepareMatchRoom(pRoom);
            }
        }
    }
}

void CRoomManager::_RandomModeMatch()
{
    bool bContinue = true;
    MatchList& matchRandom = m_AllMatchs[MusicMode_None];

    while (bContinue && matchRandom.size() >= (size_t)RoomSettingMgr::Instance().AutoMatchCount())
    {
        // 4月线上特殊处理
        //CRoom* pRoom = _CreateMatchRoom( RAN_SCENE_ID, MusicMode_None, MusicLevel_Easy );
        int nMode = (int)ConfigManager::Instance().GetSceneModeManager().RandomMode(MusicMode_Sweethearts);
        bool bIsFresherInstaller = (g_pCirculator->GetServerInstallerID() == INSTALLER_FRESHER ? true : false);
        int nScene = ConfigManager::Instance().GetSceneModeManager().RandomScene(bIsFresherInstaller);
        CRoom* pRoom = _CreateMatchRoom(nScene, nMode, MusicLevel_Easy);
        if (pRoom != NULL)
        {
            int dancerCount = matchRandom.size();
            if (dancerCount > MAX_ROOM_PLAYER_COUNT)
            {
                dancerCount = MAX_ROOM_PLAYER_COUNT;
            }

            for (int i = 0; i < dancerCount; ++i)
            {
                CRoomComponent* pPlayerRoomCOM = matchRandom.front();
                matchRandom.pop_front();

                if (pPlayerRoomCOM != NULL && pPlayerRoomCOM->Room() == NULL)
                {
                    pPlayerRoomCOM->AutoMatch(false, MusicMode_None);
                    _PlayerInRoom(*pPlayerRoomCOM, pRoom, RoleRoomType_Dancer, RoleRoomState_Ready, false, RoomEnterType_Match);
                }
            }

            _PrepareMatchRoom(pRoom);
        }
        else
        {
            bContinue = false;
        }
    }
}

CRoom* CRoomManager::_CreateMatchRoom(int nScene, int nMode, int nLevel)
{
    CRoom* pRoom = NULL;
    unsigned int nRoomID = _GenerateRoomID(eRoomType_Common);

    if (nRoomID > 0)
    {
        pRoom = new CRoom(eRoomType_Common, nRoomID);
        pRoom->ChangeRoomInfo(CLocalization::Instance().GetString("DefaultRoomName").c_str(), "");
        pRoom->SetMatch();

        pRoom->ChangeChoosedScene(nScene);
        pRoom->ChangeChoosedMusic(RAN_MUSIC_ID, nMode, nLevel);
    }

    return pRoom;
}

void CRoomManager::_PrepareMatchRoom(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        CRoomComponent* pHost = pRoom->AssignHost();
        if (pHost != NULL)
        {
            pHost->AsHost(true);

            AddRoom(pRoom);
            pRoom->RoomPrepare();

            std::vector<char> vecAndroidStage;
            std::vector<char> vecIosStage;
            std::vector<char> vecWinPhoneStage;
            const std::string& strStageFile = pRoom->PlayMusicStage();

            if (!strStageFile.empty() && CMusicStageFileManager::LoadStage(strStageFile, vecAndroidStage, vecIosStage, vecWinPhoneStage))
            {
                int nStageTime = CMusicStageFileManager::CalcuStageTime(pRoom->PlayMusicMode(), vecAndroidStage, vecIosStage, vecWinPhoneStage);
                pRoom->SetCheckTime(nStageTime);

                for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
                {
                    CRoomComponent* pRoomCOM = pRoom->Player(RoleRoomType_Dancer, nPos);
                    if (pRoomCOM != NULL)
                    {
                        GameMsg_S2C_PrepareMatch msgResponse;
                        pRoom->EncodeWholeInfo(msgResponse.m_RoomInfo, pRoomCOM->IsHost());
                        msgResponse.m_chPlayScene = (char)pRoom->PlayScene();
                        msgResponse.m_nPlayMusic = (short)pRoom->PlayMusicID();
                        msgResponse.m_chPlayMode = (char)pRoom->PlayMusicMode();
                        msgResponse.m_chPlayLevel = (char)pRoom->PlayMusicLevel();
                        msgResponse.m_strCheckKey = pRoom->PlayCheckCode();

                        if (pRoomCOM->GetPhoneOS() == OS_Ios)
                        {
                            msgResponse.m_vecStage.assign(vecIosStage.begin(), vecIosStage.end());
                        }
                        else if (pRoomCOM->GetPhoneOS() == OS_WP8)
                        {
                            msgResponse.m_vecStage.assign(vecWinPhoneStage.begin(), vecWinPhoneStage.end());
                        }
                        else
                        {
                            msgResponse.m_vecStage.assign(vecAndroidStage.begin(), vecAndroidStage.end());
                        }

                        pRoomCOM->SendPlayerMsg(&msgResponse);
                    }
                }
                if (pRoom->PlayMusicMode() == MusicMode_Sweethearts)
                {
                    StartSweetheartsMode(pRoom, vecAndroidStage, vecIosStage);
                }
            }
            else
            {
                DestroyRoom(pRoom->RoomTop(), pRoom->RoomID(), 3);
            }
        }
        else
        {
            DestroyRoom(pRoom->RoomTop(), pRoom->RoomID(), 4);
        }
    }
}

bool CRoomManager::_ChangeRoomPosState(CRoom* pRoom, int nRoleRoomType, int nRoleRoomPos, int nRoomPosState)
{
    if (pRoom != NULL)
    {
        if (pRoom->ChangePosState(nRoleRoomType, nRoleRoomPos, nRoomPosState))
        {
            GameMsg_S2C_ChangeRoomPosStateSuc msgResponse;
            msgResponse.m_chRoleRoomType = (char)nRoleRoomType;
            msgResponse.m_chRoleRoomPos = (char)nRoleRoomPos;
            msgResponse.m_chRoomPosState = (char)pRoom->PosState(nRoleRoomType, nRoleRoomPos);
            pRoom->SendMsgToAll(&msgResponse);

            return true;
        }
    }

    return false;
}

void CRoomManager::_QuitMatchList(CRoomComponent& rRoleRoomCOM)
{
    MatchList& matchPlayers = m_AllMatchs[rRoleRoomCOM.MatchMode()];
    for (MatchList::iterator it = matchPlayers.begin(); it != matchPlayers.end(); ++it)
    {
        if (*it == &rRoleRoomCOM)
        {
            (*it)->AutoMatch(false, MusicMode_None);
            matchPlayers.erase(it);
            return;
        }
    }
}

void CRoomManager::_PlayerOutLobby(CRoomComponent& rRoleRoomCOM)
{
    if (rRoleRoomCOM.AttrCOM() != NULL)
    {
        m_LobbyPlayer.erase(rRoleRoomCOM.AttrCOM()->GetRoleID());
        CLocationMgr::Instance().RemoveEntity(Zone_Lobby, rRoleRoomCOM.AttrCOM()->GetRoleID());
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "Player out lobby error, attribute component invalid -----");
    }

    // net delay
    rRoleRoomCOM.InvitedOver();
}

CRoomComponent* CRoomManager::GetLobbyPlayer(unsigned int nRoleID)
{
    PlayerMap::iterator it = m_LobbyPlayer.find(nRoleID);
    if (it != m_LobbyPlayer.end())
    {
        return it->second;
    }

    return NULL;
}

void CRoomManager::_InStartingList(CRoom* pRoom)
{
    unsigned long lNowTime = m_nMgrNowTime + RoomSettingMgr::Instance().RoomStartWait();
    m_NeedStartRooms.insert(std::make_pair(lNowTime, pRoom));
}

void CRoomManager::_OutStartingList(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        for (TimeRoomMap::iterator it = m_NeedStartRooms.begin(); it != m_NeedStartRooms.end(); ++it)
        {
            if (it->second != NULL && it->second->RoomID() == pRoom->RoomID())
            {
                m_NeedStartRooms.erase(it);
                break;
            }
        }
    }
}

void CRoomManager::_InEndingList(CRoom* pRoom)
{
    unsigned long lNowTime = m_nMgrNowTime + RoomSettingMgr::Instance().RoomEndWait();
    m_NeedEndRooms.insert(std::make_pair(lNowTime, pRoom));
}

void CRoomManager::_OutEndingList(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        for (TimeRoomMap::iterator it = m_NeedEndRooms.begin(); it != m_NeedEndRooms.end(); ++it)
        {
            if (it->second != NULL && it->second->RoomID() == pRoom->RoomID())
            {
                m_NeedEndRooms.erase(it);
                break;
            }
        }
    }
}

void CRoomManager::_RoomStart(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        pRoom->RoomStart();

        for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
        {
            CRoomComponent* pRoomCOM = pRoom->Player(RoleRoomType_Dancer, nPos);
            if (pRoomCOM != NULL)
            {
                _PlayerChangeState(*pRoomCOM, RoleRoomState_Start, false);

                pRoomCOM->AchieveCOM()->RoomStart(*pRoomCOM);
                _LogRoleDancing(pRoomCOM->AttrCOM()->GetRoleID(), CRoleDancingLog::EDancingAction_Start,
                    pRoom->PlayScene(), pRoom->TeamMode(), pRoom->PlayMusicID(), pRoom->PlayMusicMode(), pRoom->PlayMusicLevel(),
                    pRoom->DancerCount(), -1, 0);

                for (int nSlot = 0; nSlot <= pRoom->MaxDancerPos(); ++nSlot)
                {	// add recently playmate
                    CRoomComponent *pPlayMateCOM = pRoom->Player(RoleRoomType_Dancer, nSlot);
                    if (nSlot != nPos &&  pRoomCOM->FriendCOM() != NULL && pPlayMateCOM != NULL && pPlayMateCOM->AttrCOM() != NULL
                        && !pRoomCOM->FriendCOM()->IsInBlackList(pPlayMateCOM->AttrCOM()->GetRoleID())) {
                        pRoomCOM->FriendCOM()->AddRecentlyPlayMate(pPlayMateCOM->AttrCOM()->GetRoleID(), pPlayMateCOM->AttrCOM()->GetRoleName());
                    }
                }
            }
        }

        GameMsg_S2C_StartRoomSuc msgResponse;
        pRoom->SendMsgToAll(&msgResponse);

        //fufeng to do: award, achievement, quest
    }
}

void CRoomManager::_RoomEnd(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        if (pRoom->RoomType() == eRoomType_NewPlayer)
        {
            bool bRoomTopState = pRoom->RoomTop();

            // kick player
            GameMsg_S2C_EndRoomSuc msgEndResponse;
            pRoom->GetSweethearsInfo(msgEndResponse.m_vecSweetheartsInfo);
            int nKickerType = RoleRoomType_Dancer;

            for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
            {
                CRoomComponent* pPlayer = pRoom->Player(nKickerType, nPos);
                if (pPlayer != NULL)
                {
                    pRoom->PlayerOut(nKickerType, nPos);
                    pPlayer->OutRoom();

                    if (pPlayer->AttrCOM() != NULL)
                    {
                        m_LobbyPlayer.insert(std::make_pair(pPlayer->AttrCOM()->GetRoleID(), pPlayer));
                        CLocationMgr::Instance().EnterZone(Zone_Lobby, pPlayer->AttrCOM()->GetRoleID());
                        _NoticeGroupState(pPlayer->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None);
                    }

                    // 任务成就
                    QUESTUPDATEINFO info;
                    pPlayer->RoleQuestNew()->OnMusicEnd(&info);


                    // notice player
                    GameMsg_S2C_QuitRoomSuc msgQuitResponse;
                    msgQuitResponse.m_chQuitType = (char)RoomQuitType_ProcKick;
                    msgQuitResponse.m_chQuitTo = (char)RoomQuitTo_Lobby;
                    pPlayer->SendPlayerMsg(&msgQuitResponse);
                    pPlayer->SendPlayerMsg(&msgEndResponse);
                }
            }

            DestroyRoom(bRoomTopState, pRoom->RoomID(), 7);
        }
        else if (pRoom->RoomType() == eRoomType_Common)
        {
            _RoomRanking(pRoom);
            _RoomReward(pRoom);

            GameMsg_S2C_EndRoomSuc msgResponse;
            pRoom->GetSweethearsInfo(msgResponse.m_vecSweetheartsInfo);
            for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
            {
                CRoomComponent* pRoomCOM = pRoom->Player(RoleRoomType_Dancer, nPos);
                if (pRoomCOM != NULL)
                {
                    _PlayerChangeState(*pRoomCOM, RoleRoomState_End, false);
                    _LogRoleDancing(pRoomCOM->AttrCOM()->GetRoleID(), CRoleDancingLog::EDancingAction_End,
                        pRoom->PlayScene(), pRoom->TeamMode(), pRoom->PlayMusicID(), pRoom->PlayMusicMode(), pRoom->PlayMusicLevel(),
                        pRoom->DancerCount(), pRoomCOM->RoleMarkInfo().Place(), pRoomCOM->RoleMarkInfo().Mark());

                    msgResponse.m_szDancerPlace[nPos] = (char)pRoomCOM->RoleMarkInfo().Place();
                    msgResponse.m_szDancerGrade[nPos] = (char)pRoomCOM->RoleMarkInfo().Grade();
                    msgResponse.m_szDancerMark[nPos] = pRoomCOM->RoleMarkInfo().Mark();
                    msgResponse.m_szDancerExp[nPos] = pRoomCOM->RoleMarkInfo().Exp();
                    msgResponse.m_szDancerPerfect[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Perfect);
                    msgResponse.m_szDancerCool[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Cool);
                    msgResponse.m_szDancerGood[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Good);
                    msgResponse.m_szDancerBad[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Bad);
                    msgResponse.m_szDancerMiss[nPos] = pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss);
                    msgResponse.m_szDancerSpecial[nPos] = pRoomCOM->RoleMarkInfo().MaxSpecialCount();

                    // 任务成就
                    QUESTUPDATEINFO info;
                    _CreateMusicUpdateInfo(pRoomCOM, info);
                    pRoomCOM->RoleQuestNew()->OnMusicEnd(&info);

                    pRoomCOM->QuestCOM()->MusicEnd(*pRoomCOM);
                    pRoomCOM->AchieveCOM()->MusicEnd(*pRoomCOM);

                    if (pRoomCOM->RoleMarkInfo().IsLegalMark(pRoom->PlayMusicMode()))
                    {
                        // 如果是自动匹配房间，则同时通知观察者
                        if (pRoom->IsMatch())
                        {
                            CommonParam param;
                            param.SetParam(std::string("activeness"), std::string("match_room"));
                            pRoomCOM->NotifyAll(param);
                        }

                        // 通知观察者跳舞的模式
                        CommonParam param;
                        param.SetParam(std::string("activeness"), std::string("dance_mode"));
                        param.SetParam(std::string("dance_mode"), pRoom->PlayMusicMode());
                        pRoomCOM->NotifyAll(param);
                    }

                    pRoomCOM->RoleMarkInfo().ClearMark();
                }
            }

            for (int i = EDanceColorTeam_Red; i < EDanceColorTeam_Max; ++i) {
                CDanceTeam *pTeam = pRoom->GetDanceTeam((EDanceColorTeam)i);
                if (pTeam != NULL) {
                    if (pRoom->TeamMemberCount((EDanceColorTeam)i) > 0) {
                        CDanceTeamMarkSend teamMark;
                        teamMark.m_eColorTeam = (EDanceColorTeam)i;
                        teamMark.m_nTotalMark = pTeam->TotalTeamMark();
                        msgResponse.m_listTeamMark.push_back(teamMark);
                    }
                }
            }

            pRoom->SendMsgToAll(&msgResponse);
            pRoom->RoomEnd();
        }
    }
}

void CRoomManager::_CreateMusicUpdateInfo(CRoomComponent* pRoomCOM, QUESTUPDATEINFO& info)
{
    CRoom* pRoom = pRoomCOM->Room();
    CRoomMarkInfo& markInfo = pRoomCOM->RoleMarkInfo();
    info.m_nMusicID = pRoom->PlayMusicID();
    info.m_nModeID = pRoom->PlayMusicMode();
    info.m_nSceneID = pRoom->PlayScene();
    info.m_nMusicDifficulty = pRoom->PlayMusicLevel();
    info.m_nPlayerNum = pRoom->StartRoomDancersCount();
    info.m_nRank = markInfo.Place();
    info.m_nScore = markInfo.Mark();
    info.m_nPerfectNum = markInfo.TotalRankCount(RoomRoundRank_Perfect);
    info.m_nFailNum = markInfo.TotalRankCount(RoomRoundRank_Miss);
    info.m_nConPerfectNum = markInfo.ContinuousRankCount(RoomRoundRank_Perfect);
    info.m_nComboNum = markInfo.MaxSpecialCount();
    info.m_nGrade = markInfo.Grade();
    info.m_bIsMatch = pRoom->IsMatch() && pRoom->IsNormalRoom();
    CEntityDanceGroupComponent* pDanceGroup = pRoomCOM->DanceGroupCOM();
    if (pDanceGroup != NULL)
    {
        info.m_GroupNum = pRoom->GetDanceGroupPlayerCount(pDanceGroup->GetDanceGroupID());
    }
    CEntityAttributeComponent* pAttr = pRoomCOM->AttrCOM();
    if (pAttr != NULL)
    {
        info.m_FriendNum = pRoom->GetFriendCount(pAttr->m_nRoleID);
        if (pRoom->HasLover(pAttr->m_nRoleID))
        {
            info.m_LoverNum = 1;
        }
        else
        {
            info.m_LoverNum = 0;
        }
    }
}

void CRoomManager::_AutoStart(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        bool bRoomTopState = pRoom->RoomTop();

        // kick player
        int nKickerType = RoleRoomType_Dancer;
        std::vector<char> vecKickerPos;

        for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
        {
            CRoomComponent* pPlayer = pRoom->Player(nKickerType, nPos);
            if (pPlayer != NULL && pPlayer->RoleRoomState() != RoleRoomState_ToStart)
            {

                vecKickerPos.push_back((char)nPos);

                pRoom->PlayerOut(nKickerType, nPos);
                pPlayer->OutRoom();

                if (pPlayer->AttrCOM() != NULL)
                {
                    m_LobbyPlayer.insert(std::make_pair(pPlayer->AttrCOM()->GetRoleID(), pPlayer));
                    CLocationMgr::Instance().EnterZone(Zone_Lobby, pPlayer->AttrCOM()->GetRoleID());
                    _NoticeGroupState(pPlayer->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None);
                }

                // notice player
                GameMsg_S2C_QuitRoomSuc msgQuitResponse;
                msgQuitResponse.m_chQuitType = (char)RoomQuitType_StartKick;
                msgQuitResponse.m_chQuitTo = (char)RoomQuitTo_Lobby;
                pPlayer->SendPlayerMsg(&msgQuitResponse);
            }
        }

        // check whether to destroy room
        if (pRoom->HasDancer())
        {
            int nHostRoomType = RoleRoomType_None;
            int nHostRoomPos = -1;
            int nHostRoomState = RoleRoomState_None;

            //check whether to reassign host
            CRoomComponent* pHost = NULL;
            if (pRoom->RoomHost() == NULL)
            {
                pHost = pRoom->AssignHost();
                if (pHost != NULL)
                {
                    pHost->AsHost(true);
                    nHostRoomType = pHost->RoleRoomType();
                    nHostRoomPos = pHost->RoleRoomPos();
                    nHostRoomState = pHost->RoleRoomState();

                    GameMsg_S2C_QuitMany msgHostResponse;
                    msgHostResponse.m_chQuitType = (char)RoomQuitType_StartKick;
                    msgHostResponse.m_chRoleRoomType = (char)nKickerType;
                    msgHostResponse.m_vecRoleRoomPos.assign(vecKickerPos.begin(), vecKickerPos.end());
                    msgHostResponse.m_chHostRoomType = (char)nHostRoomType;
                    msgHostResponse.m_chHostRoomPos = (char)nHostRoomPos;
                    msgHostResponse.m_chHostRoomState = (char)nHostRoomState;
                    msgHostResponse.m_strRoomPwd.assign(pRoom->RoomPwd());
                    pHost->SendPlayerMsg(&msgHostResponse);
                }

                HandleTopRoom(bRoomTopState, pRoom);
            }

            // notice others
            GameMsg_S2C_QuitMany msgOtherResponse;
            msgOtherResponse.m_chQuitType = (char)RoomQuitType_StartKick;
            msgOtherResponse.m_chRoleRoomType = (char)nKickerType;
            msgOtherResponse.m_vecRoleRoomPos.assign(vecKickerPos.begin(), vecKickerPos.end());
            msgOtherResponse.m_chHostRoomType = (char)nHostRoomType;
            msgOtherResponse.m_chHostRoomPos = (char)nHostRoomPos;
            msgOtherResponse.m_chHostRoomState = (char)nHostRoomState;
            msgOtherResponse.m_strRoomPwd.assign("");
            pRoom->SendMsgToAll(&msgOtherResponse, pHost);

            _RoomStart(pRoom);
        }
        else
        {
            DestroyRoom(bRoomTopState, pRoom->RoomID(), 5);
        }
    }
}

void CRoomManager::_AutoEnd(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        bool bRoomTopState = pRoom->RoomTop();

        // kick player
        GameMsg_S2C_EndRoomSuc msgEndResponse;
        pRoom->GetSweethearsInfo(msgEndResponse.m_vecSweetheartsInfo);
        int nKickerType = RoleRoomType_Dancer;
        std::vector<char> vecKickerPos;

        for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
        {
            CRoomComponent* pPlayer = pRoom->Player(nKickerType, nPos);
            if (pPlayer != NULL && pPlayer->RoleRoomState() != RoleRoomState_ToEnd)
            {
                vecKickerPos.push_back((char)nPos);

                pRoom->PlayerOut(nKickerType, nPos);
                pPlayer->OutRoom();

                if (pPlayer->AttrCOM() != NULL)
                {
                    m_LobbyPlayer.insert(std::make_pair(pPlayer->AttrCOM()->GetRoleID(), pPlayer));
                    CLocationMgr::Instance().EnterZone(Zone_Lobby, pPlayer->AttrCOM()->GetRoleID());
                    _NoticeGroupState(pPlayer->AttrCOM()->GetRoleID(), 0, RoleRoomType_None, RoleRoomState_None);
                }

                // notice player
                GameMsg_S2C_QuitRoomSuc msgQuitResponse;
                msgQuitResponse.m_chQuitType = (char)RoomQuitType_EndKick;
                msgQuitResponse.m_chQuitTo = (char)RoomQuitTo_Lobby;
                pPlayer->SendPlayerMsg(&msgQuitResponse);
                pPlayer->SendPlayerMsg(&msgEndResponse);
            }
        }

        // check whether to destroy room
        if (pRoom->HasDancer())
        {
            int nHostRoomType = RoleRoomType_None;
            int nHostRoomPos = -1;
            int nHostRoomState = RoleRoomState_None;

            //check whether to reassign host
            CRoomComponent* pHost = NULL;
            if (pRoom->RoomHost() == NULL)
            {
                pHost = pRoom->AssignHost();
                if (pHost != NULL)
                {
                    pHost->AsHost(true);
                    nHostRoomType = pHost->RoleRoomType();
                    nHostRoomPos = pHost->RoleRoomPos();
                    nHostRoomState = pHost->RoleRoomState();

                    GameMsg_S2C_QuitMany msgHostResponse;
                    msgHostResponse.m_chQuitType = (char)RoomQuitType_EndKick;
                    msgHostResponse.m_chRoleRoomType = (char)nKickerType;
                    msgHostResponse.m_vecRoleRoomPos.assign(vecKickerPos.begin(), vecKickerPos.end());
                    msgHostResponse.m_chHostRoomType = (char)nHostRoomType;
                    msgHostResponse.m_chHostRoomPos = (char)nHostRoomPos;
                    msgHostResponse.m_chHostRoomState = (char)nHostRoomState;
                    msgHostResponse.m_strRoomPwd.assign(pRoom->RoomPwd());
                    pHost->SendPlayerMsg(&msgHostResponse);
                }

                HandleTopRoom(bRoomTopState, pRoom);
            }

            // notice others
            GameMsg_S2C_QuitMany msgOtherResponse;
            msgOtherResponse.m_chQuitType = (char)RoomQuitType_EndKick;
            msgOtherResponse.m_chRoleRoomType = (char)nKickerType;
            msgOtherResponse.m_vecRoleRoomPos.assign(vecKickerPos.begin(), vecKickerPos.end());
            msgOtherResponse.m_chHostRoomType = (char)nHostRoomType;
            msgOtherResponse.m_chHostRoomPos = (char)nHostRoomPos;
            msgOtherResponse.m_chHostRoomState = (char)nHostRoomState;
            msgOtherResponse.m_strRoomPwd.assign("");
            pRoom->SendMsgToAll(&msgOtherResponse, pHost);

            _RoomEnd(pRoom);
        }
        else
        {
            DestroyRoom(bRoomTopState, pRoom->RoomID(), 6);
        }
    }
}

void CRoomManager::_RoomSwitch(int nOldMode, CRoom* pRoom)
{
    if (pRoom != NULL && nOldMode != pRoom->ChoosedMusicMode())
    {
        if (nOldMode != MusicMode_None)
        {
            m_AllRooms[nOldMode].erase(pRoom->RoomID());
        }
        m_AllRooms[pRoom->ChoosedMusicMode()][pRoom->RoomID()] = pRoom;

        if (pRoom->RoomTop())
        {
            TopRoomList::iterator it = find(m_TopRooms[nOldMode].begin(), m_TopRooms[nOldMode].end(), pRoom);
            if (it != m_TopRooms[nOldMode].end())
            {
                if (nOldMode != MusicMode_None)
                {
                    m_TopRooms[nOldMode].erase(it);
                }

                if (pRoom->ChoosedMusicMode() != MusicMode_None)
                {
                    it = find(m_TopRooms[pRoom->ChoosedMusicMode()].begin(), m_TopRooms[pRoom->ChoosedMusicMode()].end(), pRoom);
                    if (it == m_TopRooms[pRoom->ChoosedMusicMode()].end())
                    {
                        m_TopRooms[pRoom->ChoosedMusicMode()].push_back(pRoom);
                    }
                }
            }
        }
    }
}

void CRoomManager::_RoomRanking(CRoom* pRoom)
{
    if (pRoom != NULL)
    {
        if (pRoom->PlayMusicMode() == MusicMode_Sweethearts)
        {
            pRoom->CalcSweetRank();
        }
        std::list<CRoomComponent*> lstRanking;

        for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
        {
            CRoomComponent* pRoomCOM = pRoom->Player(RoleRoomType_Dancer, nPos);
            if (pRoomCOM != NULL)
            {
                int nGrade = _CalcuGrade(*pRoomCOM);
                pRoomCOM->RoleMarkInfo().WinGrade(nGrade);

                if (lstRanking.empty())
                {
                    lstRanking.push_back(pRoomCOM);
                }
                else
                {
                    for (std::list<CRoomComponent*>::iterator it = lstRanking.begin(); it != lstRanking.end();)
                    {
                        if (pRoomCOM->RoleMarkInfo().BeckoningValue() > (*it)->RoleMarkInfo().BeckoningValue())
                        {
                            lstRanking.insert(it, pRoomCOM);
                            break;
                        }
                        else if (pRoomCOM->RoleMarkInfo().BeckoningValue() == (*it)->RoleMarkInfo().BeckoningValue())
                        {
                            if (pRoomCOM->RoleMarkInfo().Mark() > (*it)->RoleMarkInfo().Mark())
                            {
                                lstRanking.insert(it, pRoomCOM);
                                break;
                            }
                            else if (pRoomCOM->RoleMarkInfo().Mark() == (*it)->RoleMarkInfo().Mark())
                            {
                                if (pRoomCOM->RoleMarkInfo().Grade() > (*it)->RoleMarkInfo().Grade())
                                {
                                    lstRanking.insert(it, pRoomCOM);
                                    break;
                                }
                                else if (pRoomCOM->RoleMarkInfo().Grade() == (*it)->RoleMarkInfo().Grade())
                                {
                                    if (pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Perfect) > (*it)->RoleMarkInfo().TotalRankCount(RoomRoundRank_Perfect))
                                    {
                                        lstRanking.insert(it, pRoomCOM);
                                        break;
                                    }
                                    else if (pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Perfect) == (*it)->RoleMarkInfo().TotalRankCount(RoomRoundRank_Perfect))
                                    {
                                        if (pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Good) > (*it)->RoleMarkInfo().TotalRankCount(RoomRoundRank_Good))
                                        {
                                            lstRanking.insert(it, pRoomCOM);
                                            break;
                                        }
                                        else if (pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Good) == (*it)->RoleMarkInfo().TotalRankCount(RoomRoundRank_Good))
                                        {
                                            if (pRoomCOM->RoleMarkInfo().TotalRankCount(RoomRoundRank_Cool) > (*it)->RoleMarkInfo().TotalRankCount(RoomRoundRank_Cool))
                                            {
                                                lstRanking.insert(it, pRoomCOM);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        ++it;
                        if (it == lstRanking.end())
                        {
                            lstRanking.push_back(pRoomCOM);
                        }
                    }
                }
            }
        }

        if (pRoom->TeamMode() != EDanceRoomTeamMode_None)
        {
            _TeamRank(pRoom);
            _CalcuTeamExp(pRoom);
        }

        int nPlace = 1;
        for (std::list<CRoomComponent*>::iterator it = lstRanking.begin(); it != lstRanking.end(); ++it)
        {
            (*it)->RoleMarkInfo().WinPlace(nPlace);
            ++nPlace;

            int nExp = 0;
            if ((*it)->ColorTeam() == EDanceColorTeam_None)
            {
                nExp = _CalcuExp(*(*it), lstRanking.size(), pRoom->PlayMusicMode(), pRoom->PlayMusicLevel());
            }
            else
            {
                nExp = pRoom->TeamExp((*it)->ColorTeam());
            }

            (*it)->RoleMarkInfo().WinExp(nExp);
        }
    }
}

void CRoomManager::_RoomReward(CRoom* pRoom)
{
    if (pRoom == NULL)
        return;

    bool bItemReward = (pRoom->StartRoomDancersCount() >= CSystemSettingMgr::Instance().GetMinRoomRewardPlayerNum() ? true : false);
    bool bGroupReward = (pRoom->DancerCount() >= CDanceGroupDataMgr::Instance().MinRoomDancersGroupHonorBonusNeed() ? true : false);

    for (int nPos = 0; nPos <= pRoom->MaxDancerPos(); ++nPos)
    {
        CRoomComponent* pDancerRoomCOM = pRoom->Player(RoleRoomType_Dancer, nPos);
        if (pDancerRoomCOM != NULL)
        {
            if (pDancerRoomCOM->RoleMarkInfo().IsLegalMark(pRoom->PlayMusicMode()))
            {
                //经验奖励
                int nAddExp = pDancerRoomCOM->SendExpReward();
                pDancerRoomCOM->RoleMarkInfo().WinExp(nAddExp);

                //物品奖励
                if (bItemReward)
                {
                    pDancerRoomCOM->SendRoomItemReward(pRoom->PlayMusicMode());
                }

                // 金券奖励(宠物技能)
                pDancerRoomCOM->SendRoomCurrencyReward();

                //情侣亲密度奖励
                if (pDancerRoomCOM->CoupleCOM() != NULL && pDancerRoomCOM->CoupleCOM()->PairID() != 0)
                {
                    for (int i = nPos + 1; i <= pRoom->MaxDancerPos(); ++i)
                    {
                        CRoomComponent* pCoupleRoomCOM = pRoom->Player(RoleRoomType_Dancer, i);
                        if (pCoupleRoomCOM != NULL && pCoupleRoomCOM->RoleMarkInfo().IsLegalMark(pRoom->PlayMusicMode()))
                        {
                            if (pCoupleRoomCOM->CoupleCOM() != NULL && pDancerRoomCOM->CoupleCOM()->PairID() == pCoupleRoomCOM->CoupleCOM()->PairID())
                            {
                                int nPlace = MIN(pDancerRoomCOM->RoleMarkInfo().Place(), pCoupleRoomCOM->RoleMarkInfo().Place());
                                pDancerRoomCOM->CoupleReward(pDancerRoomCOM, pCoupleRoomCOM, nPlace);
                            }
                        }
                    }
                }

                //舞团荣誉奖励
                if (bGroupReward && pDancerRoomCOM->DanceGroupCOM() != NULL)
                {
                    if (pDancerRoomCOM->RoleMarkInfo().Place() == 1)
                    {
                        int nHonorBonus = CDanceGroupDataMgr::Instance().MemberMatchGroupHonorBonusLeve1();
                        if (pDancerRoomCOM->GeneCOM() != NULL)
                        {
                            GeneData* pGeneData = pDancerRoomCOM->GeneCOM()->FindGene(CDanceGroupDataMgr::Instance().RoomGroupHonorBonusGeneID());
                            if (pGeneData != NULL)
                            {
                                nHonorBonus += pGeneData->nParam1;
                            }
                        }

                        pDancerRoomCOM->DanceGroupCOM()->ChangeDanceGroupHonor(nHonorBonus, EChangeDanceGroupHonorCause_Match, pDancerRoomCOM->AttrCOM()->GetRoleID());
                    }
                }
            }
            else
            {
                pDancerRoomCOM->RoleMarkInfo().WinExp(0);
            }
        }
    }
}

int CRoomManager::_CalcuGrade(CRoomComponent& rRoleRoomCOM)
{
    return rRoleRoomCOM.CalcuGrade();
}

unsigned int CRoomManager::_CalcuExp(CRoomComponent& rRoleRoomCOM, int nDancerCount, int nMusicMode, int nMusicLevel)
{
    return (unsigned int)rRoleRoomCOM.CalcExp(nDancerCount, nMusicMode, nMusicLevel);
}

void CRoomManager::_TeamRank(CRoom *pRoom)
{
    if (pRoom != NULL) {
        if (pRoom->TeamMode() != EDanceRoomTeamMode_None) {
            std::vector<CDanceTeam> vecDanceTeam;
            for (int i = EDanceColorTeam_Red; i < EDanceColorTeam_Max; ++i) {
                CDanceTeam  *pDanceTeam = pRoom->GetDanceTeam((EDanceColorTeam)i);
                if (pDanceTeam != NULL) {
                    if (pRoom->TeamMemberCount((EDanceColorTeam)i) > 0) {
                        vecDanceTeam.push_back(*pDanceTeam);
                    }
                }
            }
            WriteLog(LOGLEVEL_DEBUG, "---dance team count [%d]\n", vecDanceTeam.size());

            DanceTeamRankCompare danceTeamCompare;
            std::stable_sort(vecDanceTeam.begin(), vecDanceTeam.end(), danceTeamCompare);//如果分数相同，按顺序排名
            WriteLog(LOGLEVEL_DEBUG, "---team ranking---");
            for (size_t i = 0; i < vecDanceTeam.size(); ++i) {
                CDanceTeam *pTeam = pRoom->GetDanceTeam((EDanceColorTeam)vecDanceTeam[i].TeamColor());
                if (pTeam != NULL) {
                    pTeam->SetRank(i + 1);
                    WriteLog(LOGLEVEL_DEBUG, "---team color[%d], rank[%d]---\n", pTeam->TeamColor(), pTeam->Rank());
                }
            }
        }
    }
}

void CRoomManager::_CalcuTeamExp(CRoom *pRoom)
{
    if (pRoom != NULL) {
        WriteLog(LOGLEVEL_DEBUG, "---team exp---\n");
        for (int i = EDanceColorTeam_Red; i < EDanceColorTeam_Max; ++i) {
            EDanceColorTeam eColorTeam = (EDanceColorTeam)i;
            CDanceTeam *pColorTeam = pRoom->GetDanceTeam(eColorTeam);
            if (pColorTeam != NULL) {
                if (pRoom->TeamMode() != EDanceRoomTeamMode_None) {
                    int nGrade = RoomDanceGrade_None;
                    CRoomComponent *pCandidatePlayer = NULL;
                    for (int j = 0; j < MAX_ROOM_PLAYER_COUNT; ++j) {
                        CRoomComponent *pPlayer = pRoom->Player(RoleRoomType_Dancer, j);
                        if (pPlayer != NULL) {
                            if (pPlayer->ColorTeam() == eColorTeam) {
                                if (pPlayer->RoleMarkInfo().Grade() > nGrade) {
                                    nGrade = pPlayer->RoleMarkInfo().Grade();
                                    pCandidatePlayer = pPlayer;
                                    WriteLog(LOGLEVEL_DEBUG, "team[%d] best grade[%d]\n", i, nGrade);
                                }
                            }
                        }
                    }
                    if (pCandidatePlayer != NULL) {
                        //同一队伍之中最高的操作经验值
                        int nGradeExp = CGradeInfoManager::Instance().GetGradeExp(pRoom->PlayMusicLevel(), pCandidatePlayer->RoleMarkInfo().Grade());
                        pColorTeam->AddTeamExp(nGradeExp);
                        WriteLog(LOGLEVEL_DEBUG, "Grade exp:team[%d],exp[%d]---\n", eColorTeam, nGradeExp);
                    }

                    int nMemberCountPerTeam = pRoom->StartRoomDancersCount() / pRoom->StartTeamsCount();
                    int nPlaceExp = CGradeInfoManager::Instance().GetPlaceExp((nMemberCountPerTeam * (pColorTeam->Rank() - 1) + 1), pRoom->StartRoomDancersCount());
                    pColorTeam->AddTeamExp(nPlaceExp);
                    WriteLog(LOGLEVEL_DEBUG, "place exp:team[%d],exp[%d]---\n", eColorTeam, nPlaceExp);
                }
            }
        }
    }
}

void CRoomManager::_NoticeGroupState(unsigned int nRoleID, unsigned int nRoomID, int nRoleRoomType, int nRoleRoomState)
{
    GameMsg_S2G_UpdateRoomInfo msgGroup;
    msgGroup.m_nRoleID = nRoleID;
    msgGroup.m_RoomUpdateInfo.m_nRoomID = nRoomID;
    msgGroup.m_RoomUpdateInfo.m_nRoleRoomType = nRoleRoomType;
    msgGroup.m_RoomUpdateInfo.m_nRoleRoomState = nRoleRoomState;

    SendMsg2GroupServer(&msgGroup);
}

bool CRoomManager::_CanSwitchRoomTeamMode(CRoomComponent& rRoleRoomCOM, EDanceRoomTeamMode eTeamMode, EDanceColorTeam eColorTeam, ESwitchDanceRoomTeamModeRes &eRes, std::string & strError)
{
    CRoom * pRoom = rRoleRoomCOM.Room();
    if (pRoom == NULL) {
        eRes = ESwitchDanceRoomTeamModeRes_NotInRoom;
        strError.assign("ESwitchDanceRoomTeamModeRes_NotInRoom");
        return false;
    }

    if (!rRoleRoomCOM.IsHost()) {
        eRes = ESwitchDanceRoomTeamModeRes_NotRoomHost;
        strError.assign("ESwitchDanceRoomTeamModeRes_NotRoomHost");
        return false;
    }

    if (!(eTeamMode >= EDanceRoomTeamMode_None && eTeamMode <= EDanceRoomTeamMode_Team)) {
        eRes = ESwitchDanceRoomTeamModeRes_InvalidTeamMode;
        strError.assign("ESwitchDanceRoomTeamModeRes_InvalidTeamMode");
        return false;
    }

    if (eTeamMode != EDanceRoomTeamMode_None && !(eColorTeam >= EDanceColorTeam_Red && eColorTeam < EDanceColorTeam_Max)) {
        eRes = ESwitchDanceRoomTeamModeRes_InvalidTeamColor;
        strError.assign("ESwitchDanceRoomTeamModeRes_InvalidTeamColor");
        return false;
    }

    if (pRoom->TeamMode() == eTeamMode) {
        eRes = ESwitchDanceRoomTeamModeRes_TeamModeNotChange;
        strError.assign("ESwitchDanceRoomTeamModeRes_TeamModeNotChange");
        return false;
    }

    return true;
}

bool CRoomManager::_CanJoinTeam(CRoomComponent& rRoleRoomCOM, EDanceColorTeam eColorTeam, EJoinDanceRoomTeamRes &eRes, std::string & strError)
{
    CRoom *pRoom = rRoleRoomCOM.Room();
    if (pRoom == NULL) {
        eRes = EJoinDanceRoomTeamRes_NotInRoom;
        strError.assign("EJoinDanceRoomTeamRes_NotInRoom");
        return false;
    }

    if (!rRoleRoomCOM.IsHost() && rRoleRoomCOM.RoleRoomState() != RoleRoomState_Wait) {
        eRes = EJoinDanceRoomTeamRes_InReadyState;
        strError.assign("EJoinDanceRoomTeamRes_InReadyState");
        return false;
    }

    if (!(eColorTeam > EDanceColorTeam_None && eColorTeam < EDanceColorTeam_Max)) {
        eRes = EJoinDanceRoomTeamRes_InvalidColorTeam;
        strError.assign("EJoinDanceRoomTeamRes_InvalidColorTeam");
        return false;
    }

    if (rRoleRoomCOM.ColorTeam() == eColorTeam) {
        eRes = EJoinDanceRoomTeamRes_NotChange;
        strError = "EJoinDanceRoomTeamRes_NotChange";
        return false;
    }

    if (rRoleRoomCOM.RoleRoomType() != RoleRoomType_Dancer) {
        eRes = EJoinDanceRoomTeamRes_NotDancer;
        strError.assign("EJoinDanceRoomTeamRes_NotDancer");
        return false;
    }

    return true;
}

//记录角色跳舞信息
void CRoomManager::_LogRoleDancing(unsigned int nRoleID, CRoleDancingLog::EDancingAction eDancingAction,
    int nScene, int nTeamMode, int nMusicID, int nMusicMode, int nMusicLevel, int nPlayerCount, int nRank, int nScore)
{
    CRoleDancingLog * pLog = new CRoleDancingLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_eDancingAction = eDancingAction;
    pLog->m_nScene = nScene;
    pLog->m_nMusicID = nMusicID;
    pLog->m_nMusicMode = nMusicMode;
    pLog->m_nMusicLevel = nMusicLevel;
    pLog->m_nPlayerCount = nPlayerCount;
    pLog->m_nRank = nRank;
    pLog->m_nScore = nScore;
    pLog->m_nTeamMode = nTeamMode;
    g_pQueryMgr->AddQuery(QUERY_LogRoleDancing, 0, pLog);
}

void CRoomManager::StartSweetheartsMode(CRoom* pRoom, std::vector<char>& vecAndroidData, std::vector<char>& vecIosData)
{
    float fBPM = 0.f;
    int nBeatN = 0;
    int nBeatD = 0;
    float fRoundTime = 0.f;
    int nBeginRound = 0;
    int nEndRound = 0;
    std::string tmpString;
    unsigned int nRoomId = pRoom->RoomID();

    std::string text(vecAndroidData.begin(), vecAndroidData.end());
    std::vector<std::string> lines;
    Split(text, "\r\n", lines);

    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        const std::string& line = *it;
        if (!line.empty())
        {
            if (line[0] != '#')
            {
                std::vector<int> vecInt;
                if (BeginWithString(line, "BPM:"))
                {
                    tmpString = "BPM:";
                    ToType(line.substr(tmpString.size(), line.size() - tmpString.size()), fBPM);
                }
                else if (BeginWithString(line, "MEASURE"))
                {
                    tmpString = "MEASURE:";
                    Split(line.substr(tmpString.size(), line.size() - tmpString.size()), "/", vecInt);

                    nBeatN = vecInt[0];
                    nBeatD = vecInt[1];

                    fRoundTime = (60 * 4 * nBeatN) / (nBeatD * fBPM);//计算每个小节消耗的时间
                    //WriteLog("xindong nBeatN=%d, nBeatD=%d, fBPM=%f, fRoundTime=%f", nBeatN, nBeatD, fBPM, fRoundTime);
                }
                else if (BeginWithString(line, "@tradition:"))
                {
                    tmpString = "@tradition:";
                    Split(line.substr(tmpString.size(), line.size() - tmpString.size()), ":", vecInt);
                    nBeginRound = vecInt[0];
                    nEndRound = vecInt[1];
                    pRoom->SetRoundRange(MusicMode_Tradition, nBeginRound, nEndRound);
                    //WriteLog("xindong @tradition:nBeginRound=%d, nMaxRound=%u", nBeginRound, nEndRound);
                }
                else if (BeginWithString(line, "@taigu:"))
                {
                    tmpString = "@taigu:";
                    Split(line.substr(tmpString.size(), line.size() - tmpString.size()), ":", vecInt);
                    nBeginRound = vecInt[0];
                    nEndRound = vecInt[1];
                    pRoom->SetRoundRange(MusicMode_Taiko, nBeginRound, nEndRound);
                    //WriteLog("xindong @taigu:nBeginRound=%d, nMaxRound=%u", nBeginRound, nEndRound);
                }
                else if (BeginWithString(line, "@select:"))
                {
                    tmpString = "@select:";
                    Split(line.substr(tmpString.size(), line.size() - tmpString.size()), ":", vecInt);
                    int nBeginRound = vecInt[0];
                    int nEndRound = vecInt[1];
                    unsigned int nBeginTime = (unsigned int)(nBeginRound * fRoundTime);
                    unsigned int nEndTime = (unsigned int)((nEndRound - 1) * fRoundTime);
                    ITimer* pTimer = new CBeginToSelectPartnerTimer;
                    pTimer->m_nTimerID = nRoomId;
                    pTimer->m_nTriggerTime = nBeginTime;
                    pRoom->AddTimer(pTimer);
                    pTimer = new CEndToSelectPartnerTimer;
                    pTimer->m_nTimerID = nRoomId;
                    pTimer->m_nTriggerTime = nEndTime;
                    pRoom->AddTimer(pTimer);
                    //WriteLog("xindong @select:nBeginRound=%d, nBeginTime=%u, nEndRound=%d, nEndTime=%u",
                    //	nBeginRound, nBeginTime, nEndRound, nEndTime);
                }
            }
        }
    }
}

void CRoomManager::CreateTestRoom(int nRoomCount, CRoomComponent *pHost)
{
    std::string strRoomName;
    for (int i = 0; i < nRoomCount; ++i)
    {
        int nRoomID = _GenerateRoomID(eRoomType_Common);
        if (nRoomID > 0)
        {
            Formatter::Format(strRoomName, "TestRoom%03d-%d", nRoomCount, nRoomID);

            CRoom* pRoom = new CRoom(eRoomType_Common, nRoomID);
            pRoom->ChangeRoomInfo(strRoomName.c_str(), "123456");
            pRoom->ChangeChoosedMusic(RAN_MUSIC_ID, MusicMode_Taiko + i % 2, MusicLevel_Easy + i % 3);

            AddRoom(pRoom);
        }
    }

    WriteLog(LOGLEVEL_DEBUG, "此次共创建测试房间[%d]个\n", nRoomCount);
}

void CRoomManager::GetRoomListByGM(std::vector<GMRoomInfo>& roomList)
{
    AllRoomMap::iterator it = m_AllRooms[MusicMode_None].begin();
    for (; it != m_AllRooms[MusicMode_None].end(); it++)
    {
        GMRoomInfo roomInfo;
        it->second->ToGMRoomInfo(roomInfo);
        roomList.push_back(roomInfo);
    }
}


