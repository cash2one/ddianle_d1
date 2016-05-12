#include <list>
#include "PlayerInfoComponent.h"
#include "../../socket/GameMsg_Map.h"
#include "../SystemSetting/SystemSettingMgr.h"

#include "GameMsg_C2S_GetRoleInfo.h"
#include "GameMsg_S2C_GetRoleInfoResult.h"
#include "GameMsg_S2G_PlayerInfo.h"
#include "GameMsg_G2S_PlayerInfo.h"
#include "../../datastructure/DataStruct_DB.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../room/RoomComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../item/EntityItemComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "GameMsg_Announcement.h"
#include "../announcement/AnnouncementMgr.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../Couple/CoupleDataStructrue.h"
#include "GameMsg_PlayerSetting.h"
#include "../item/ItemProcess.h"
#include "PlayerSettingDefine.h"
#include "../../datastructure/Localization.h"
#include "../Couple/WeddingRoom.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "../Couple/GameMsg_G2S_Couple.h"
//#include "../CoupleMsgDef.h"
#include "../logic/LogicCirculator.h"
#include "../../datastructure/datainfor/ServerConfig.h"
#include "../group/GameMsg_LBS.h"
#include "../AmuseRoom/GameMsg_C2S_AmuseMsg.h"
#include "../AmuseRoom/GameMsg_S2C_AmuseMsg.h"
#include "../share/GameMsg_Forward.h"
#include "../Pet/PetComponent.h"
#include "../Pet/PetService.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../CeremonyRoom/ceremonyRoom.h"
#include "../Medal/EntityMedalComponent.h"
#include "../mall/EntityMallComponent.h"

extern int g_nLine;
extern CLogicCirculator* g_pCirculator;

CPlayerInfoComponent::CPlayerInfoComponent()
    : CEntityComponent(true, true)
    , m_pNet(NULL)
    , m_pFriend(NULL)
    , m_pRoleAttr(NULL)
    , m_pRoleItem(NULL)
    , m_pAch(NULL)
    , m_pRoleCouple(NULL)
    , m_pRoleVIP(NULL)
    , m_pRoleAmuseRoom(NULL)
    , m_pRoleRoom(NULL)
    , m_pRolePet(NULL)
    , m_pRoleMall(NULL)
{
    m_position.m_nMoveType = CSystemSettingMgr::Instance().DefaultMoveType();
}

CPlayerInfoComponent::~CPlayerInfoComponent()
{
}

void CPlayerInfoComponent::Start()
{
    m_pRoleAttr = GetComponent<CRoleAttribute>();
    m_pFriend = GetComponent<CRoleFriend>();
    m_pNet = GetComponent<CRoleNet>();
    m_pRoleItem = GetComponent<CRoleItem>();
    m_pAch = GetComponent<CRoleAchievement>();
    m_pRoleCouple = GetComponent<CRoleCouple>();
    m_pRoleVIP = GetComponent<CRoleVIP>();
    m_pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
    m_pRoleRoom = GetComponent<CRoleRoom>();
    m_pRolePet = GetComponent<CRolePet>();
    m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
    m_pRoleMall = GetComponent<CRoleMall>();
}

void CPlayerInfoComponent::RegComponentNetMsgMap()
{
    // message creator
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetRoleInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetPlayerInfo_Forward);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetPlayerInfo_Backward);
    GAMEMSG_REGISTERCREATOR(GameMsg_Forward);

    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetAnnouncement);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_OptionSetting);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PlayerMoveTo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PlayerMoveType);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PlayMakerSync);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PlayerMotion);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_PlayerDownloading);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetOfflinePlayerInfo);

    // message processor

    RegMsgProcessor(MSG_G2S_GetPlayerInfo_Forward, &CPlayerInfoComponent::OnGetPlayerInfoForwardByGroup);
    RegMsgProcessor(MSG_G2S_GetPlayerInfo_Backward, &CPlayerInfoComponent::OnGetRoleInfoBackwardByGroup);
    RegMsgProcessor(MSG_G2S_GetOfflinePlayerInfo, &CPlayerInfoComponent::GetOfflinePlayerInfo);
    RegMsgProcessor(MSG_S2S_Forward, &CPlayerInfoComponent::_OnForward);


    RegMsgProcessor(MSG_C2S_GetRoleInfo, &CPlayerInfoComponent::OnGetRoleInfo);
    RegMsgProcessor(MSG_C2S_GetAnnoucement, &CPlayerInfoComponent::OnGetAnnouncement);
    RegMsgProcessor(MSG_C2S_OptionSetting, &CPlayerInfoComponent::_OnOptionSetting);
    RegMsgProcessor(MSG_C2S_PlayerMoveTo, &CPlayerInfoComponent::_OnPlayerMoveTo);
    RegMsgProcessor(MSG_C2S_PlayerMoveType, &CPlayerInfoComponent::_OnPlayerMoveType);
    RegMsgProcessor(MSG_C2S_PlayMakerSync, &CPlayerInfoComponent::_OnPlayMakerSync);
    RegMsgProcessor(MSG_C2S_PlayerMotion, &CPlayerInfoComponent::OnSyncPlayerMotion);
    RegMsgProcessor(MSG_C2S_PlayerDownloading, &CPlayerInfoComponent::OnSyncPlayerDownloading);
}

void CPlayerInfoComponent::OnGetRoleInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetRoleInfo *pGetMsg = (GameMsg_C2S_GetRoleInfo *)&msg;
    unsigned int nQueryRoleID = pGetMsg->m_nRoleID;

    if (nQueryRoleID <= 0)
    {
        return;
    }

    CRoleEntity *pQueryRole = CPlayerManager::Instance().GetEntityByRoleID(nQueryRoleID);
    if (pQueryRole == NULL)
    {
        GameMsg_S2G_GetPlayerInfo_Forward requireMsg(m_pRoleAttr->GetRoleID(), nQueryRoleID);
        SendGroupServerMsg(&requireMsg);
    }
    else // 如果玩家在同一条线
    {
        GameMsg_S2C_GetRoleInfoResult resultMsg;
        SetPlayerInfo(resultMsg.m_playerInfo, *pQueryRole);
        SendPlayerMsg(&resultMsg);
    }
}

void CPlayerInfoComponent::OnGetRoleInfoBackwardByGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetPlayerInfo_Backward *pMsg = (GameMsg_G2S_GetPlayerInfo_Backward *)&msg;
    if (pMsg != NULL)
    {
        GameMsg_S2C_GetRoleInfoResult resultMsg;
        resultMsg.m_playerInfo = pMsg->m_playerInfo;
        SendPlayerMsg(&resultMsg);
    }
}

void CPlayerInfoComponent::GetOfflinePlayerInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetOfflinePlayerInfo & g2smsg = (GameMsg_G2S_GetOfflinePlayerInfo&)msg;
    SyncPlayerDataToGroup(g2smsg.m_nSelfRoleID, g2smsg.m_eFrom);
}

void CPlayerInfoComponent::OnGetPlayerInfoForwardByGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetPlayerInfo_Forward *pMsg = (GameMsg_G2S_GetPlayerInfo_Forward *)&msg;
    GameMsg_S2G_GetPlayerInfo_Backward backwardMsg;

    backwardMsg.m_nSrcID= pMsg->m_nSrcID;
    if (pMsg != NULL && m_pRoleAttr != NULL && pMsg->m_nQueryID > 0)
    {
        CRoleEntity *pQueryRole = (CRoleEntity *)Entity();
        if (pQueryRole != NULL)
        {
            SetPlayerInfo(backwardMsg.m_playerInfo, *pQueryRole);
        }
    }
    SendGroupServerMsg(&backwardMsg);
}

void CPlayerInfoComponent::OnGetAnnouncement(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetAnnouncement *pMsg = (GameMsg_C2S_GetAnnouncement *)&msg;
    if (pMsg != NULL)
    {
        if (pMsg->m_nVersion < AnnouncementMgr::Instance().Version())
        {
            GameMsg_S2C_UPdateAnnouncement announceMsg;
            announceMsg.m_nVersion = AnnouncementMgr::Instance().Version();
            for (vector<CAnnouncement>::iterator itr = AnnouncementMgr::Instance().m_vecAnnouncement.begin();
                itr != AnnouncementMgr::Instance().m_vecAnnouncement.end(); ++itr)
            {
                if (itr->m_nPID == PID_NONE || itr->m_nPID == m_pRoleAttr->PID())
                {
                    announceMsg.m_vecAnnoucement.push_back(*itr);
                }
            }
            SendPlayerMsg(&announceMsg);
        }
        else
        {
            GameMsg_S2C_AnnouncementUpdated updatedMsg;
            SendPlayerMsg(&updatedMsg);
        }
    }
}

void CPlayerInfoComponent::_OnOptionSetting(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_OptionSetting & msgSetting = (GameMsg_C2S_OptionSetting &)msg;
    if (msgSetting.m_nOption <= EPlayerSetting_None || msgSetting.m_nOption >= EPlayerSetting_Max)
    {
        std::string strError = CLocalization::Instance().GetString("Player_Setting_Error_Para");
        GameMsg_S2C_OptionSettingFail msgFail;
        msgFail.m_nOption = msgSetting.m_nOption;
        msgFail.m_strError = strError;

        SendPlayerMsg(&msgFail);
    }
    else
    {
        _SetOptionSetting((EPlayerSetting)msgSetting.m_nOption, msgSetting.m_nState);

        GameMsg_S2C_OptionSettingSuc msgSuc;
        msgSuc.m_nOption = msgSetting.m_nOption;
        msgSuc.m_nState = msgSetting.m_nState;
        SendPlayerMsg(&msgSuc);
    }
}

void CPlayerInfoComponent::_OnPlayerMoveTo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_PlayerMoveTo & msgMoveTo = (GameMsg_C2S_PlayerMoveTo &)msg;

    switch (msgMoveTo.m_nSceneID)
    {
    case ESceneMove_WeddingOut:
        {
            if (m_pRoleCouple != NULL && m_pRoleCouple->WeddingRoom() != NULL)
            {
                GameMsg_S2C_PlayerMoveTo msgNotification;
                msgNotification.m_nSceneID = msgMoveTo.m_nSceneID;
                msgNotification.m_nRoleID = ( m_pRoleAttr == NULL ? 0 : m_pRoleAttr->GetRoleID() );
                msgNotification.m_fDirectionX = msgMoveTo.m_fDirectionX;
                msgNotification.m_fDirectionZ = msgMoveTo.m_fDirectionZ;
                msgNotification.m_fPosX = msgMoveTo.m_fPosX;
                msgNotification.m_fPosY = msgMoveTo.m_fPosY;
                msgNotification.m_fPosZ = msgMoveTo.m_fPosZ;

                m_position.m_ESceneID = ESceneMove_WeddingOut;
                m_position.m_fOrient = msgMoveTo.m_fOrient;
                m_position.m_fPosX = msgMoveTo.m_fPosX;
                m_position.m_fPosY = msgMoveTo.m_fPosY;
                m_position.m_fPosZ = msgMoveTo.m_fPosZ;
                m_pRoleCouple->WeddingRoom()->SendMsgToAllBut(&msgNotification, m_pRoleCouple);
            }
        }
        break;
    case ESceneMove_AmuseRoom:
        {
            if ( m_pRoleAmuseRoom != NULL && m_pRoleAmuseRoom->AmuseRoom() != NULL )
            {
                GameMsg_S2C_PlayerMoveTo msgNotification;
                msgNotification.m_nSceneID = msgMoveTo.m_nSceneID;
                msgNotification.m_nRoleID = ( m_pRoleAttr == NULL ? 0 : m_pRoleAttr->GetRoleID() );
                msgNotification.m_fDirectionX = msgMoveTo.m_fDirectionX;
                msgNotification.m_fDirectionZ = msgMoveTo.m_fDirectionZ;
                msgNotification.m_fPosX = msgMoveTo.m_fPosX;
                msgNotification.m_fPosY = msgMoveTo.m_fPosY;
                msgNotification.m_fPosZ = msgMoveTo.m_fPosZ;

                m_position.m_ESceneID = ESceneMove_WeddingOut;
                m_position.m_fOrient = msgMoveTo.m_fOrient;
                m_position.m_fPosX = msgMoveTo.m_fPosX;
                m_position.m_fPosY = msgMoveTo.m_fPosY;
                m_position.m_fPosZ = msgMoveTo.m_fPosZ;
                m_pRoleAmuseRoom->AmuseRoom()->SendMsgToAll( &msgNotification, m_pRoleAmuseRoom );
            }
        }
        break;
    case ESceneMove_DanceGroupCeremonyRoom:
        if ( m_pRoleDanceGroup != NULL && m_pRoleDanceGroup->GetCeremonyRoom() != NULL )
        {
            GameMsg_S2C_PlayerMoveTo msgNotification;
            msgNotification.m_nSceneID = msgMoveTo.m_nSceneID;
            msgNotification.m_nRoleID = ( m_pRoleAttr == NULL ? 0 : m_pRoleAttr->GetRoleID() );
            msgNotification.m_fDirectionX = msgMoveTo.m_fDirectionX;
            msgNotification.m_fDirectionZ = msgMoveTo.m_fDirectionZ;
            msgNotification.m_fPosX = msgMoveTo.m_fPosX;
            msgNotification.m_fPosY = msgMoveTo.m_fPosY;
            msgNotification.m_fPosZ = msgMoveTo.m_fPosZ;

            m_position.m_ESceneID = ESceneMove_DanceGroupCeremonyRoom;
            m_position.m_fOrient = msgMoveTo.m_fOrient;
            m_position.m_fPosX = msgMoveTo.m_fPosX;
            m_position.m_fPosY = msgMoveTo.m_fPosY;
            m_position.m_fPosZ = msgMoveTo.m_fPosZ;
            m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcast(msgNotification);
        }
        break;
    default:
        break;
    }
}

void CPlayerInfoComponent::OnSyncPlayerMotion(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_PlayerMotion &request = (GameMsg_C2S_PlayerMotion &)msg;

    GameMsg_S2C_PlayerMotion response;
    response.m_nRoleID = m_pRoleAttr->GetRoleID();
    response.m_nTransFormID = request.m_nTransFormID;

    SendMsgToAll(response);
}

void CPlayerInfoComponent::OnSyncPlayerDownloading(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_PlayerDownloading &request = (GameMsg_C2S_PlayerDownloading &)msg;

    if (m_pRoleRoom->IsInGroupRoom())
    { // 
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&msg); // 转发给 group. 其他不管
    }

    GameMsg_S2C_PlayerDownloading response;
    response.m_nRoleID = m_pRoleAttr->GetRoleID();
    response.m_nProgress = request.m_nProgress;

    SendMsgToAll(response, true);
}

void CPlayerInfoComponent::SendMsgToAll(GameMsg_Base &msg, bool bToSelf)
{
    if (m_pRoleRoom->Room() != NULL)
    {
        m_pRoleRoom->Room()->SendMsgToAll(&msg, bToSelf ? NULL : m_pRoleRoom);
    }
    else if (m_pRoleAmuseRoom->AmuseRoom() != NULL)
    {
        m_pRoleAmuseRoom->AmuseRoom()->SendMsgToAll(&msg, bToSelf ? NULL : m_pRoleAmuseRoom);
    }
    else if (m_pRoleCouple->WeddingRoom() != NULL)
    {
        m_pRoleCouple->WeddingRoom()->SendMsgToAllBut(&msg, bToSelf ? NULL : m_pRoleCouple);
    }
    else if (NULL != m_pRoleDanceGroup && m_pRoleDanceGroup->GetCeremonyRoom() != NULL)
    {
        m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcastExcept((bToSelf ? 0 : m_pRoleAttr->GetRoleID()), msg);
    }
}

void CPlayerInfoComponent::_OnPlayerMoveType(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_PlayerMoveType & msgMoveType = (GameMsg_C2S_PlayerMoveType &)msg;

    switch (msgMoveType.m_nSceneID)
    {
    case ESceneMove_WeddingOut:
        {
            if (m_pRoleCouple != NULL && m_pRoleCouple->WeddingRoom() != NULL)
            {
                GameMsg_S2C_PlayerMoveType msgNotification;
                msgNotification.m_nSceneID = msgMoveType.m_nSceneID;
                msgNotification.m_nRoleID = ( m_pRoleAttr == NULL ? 0 : m_pRoleAttr->GetRoleID() );
                msgNotification.m_nMoveType = msgMoveType.m_nMoveType;

                m_position.m_ESceneID = ESceneMove_WeddingOut;
                m_position.m_nMoveType = msgMoveType.m_nMoveType;
                m_pRoleCouple->WeddingRoom()->SendMsgToAllBut(&msgNotification, m_pRoleCouple);
            }
        }
        break;
    case ESceneMove_AmuseRoom:
        {
            if ( m_pRoleAmuseRoom != NULL && m_pRoleAmuseRoom->AmuseRoom() != NULL )
            {
                GameMsg_S2C_PlayerMoveType msgNotification;
                msgNotification.m_nSceneID = msgMoveType.m_nSceneID;
                msgNotification.m_nRoleID = ( m_pRoleAttr == NULL ? 0 : m_pRoleAttr->GetRoleID() );
                msgNotification.m_nMoveType = msgMoveType.m_nMoveType;

                m_position.m_ESceneID = ESceneMove_WeddingOut;
                m_position.m_nMoveType = msgMoveType.m_nMoveType;
                m_pRoleAmuseRoom->AmuseRoom()->SendMsgToAll( &msgNotification, m_pRoleAmuseRoom );
            }
        }
        break;
    case ESceneMove_WaitingRoom:
        {
            if ( m_pRoleRoom != NULL && m_pRoleRoom->Room() != NULL )
            {
                GameMsg_S2C_PlayerMoveType msgNotification;
                msgNotification.m_nSceneID = msgMoveType.m_nSceneID;
                msgNotification.m_nRoleID = ( m_pRoleAttr == NULL ? 0 : m_pRoleAttr->GetRoleID() );
                msgNotification.m_nMoveType = msgMoveType.m_nMoveType;

                m_position.m_ESceneID = ESceneMove_WeddingOut;
                m_position.m_nMoveType = msgMoveType.m_nMoveType;
                m_pRoleRoom->Room()->SendMsgToAll( &msgNotification, m_pRoleRoom );
            }
        }
        break;
    case ESceneMove_DanceGroupCeremonyRoom:
        {
            if ( m_pRoleDanceGroup != NULL && m_pRoleDanceGroup->GetCeremonyRoom() != NULL )
            {
                GameMsg_S2C_PlayerMoveType msgNotification;
                msgNotification.m_nSceneID = msgMoveType.m_nSceneID;
                msgNotification.m_nRoleID = ( m_pRoleAttr == NULL ? 0 : m_pRoleAttr->GetRoleID() );
                msgNotification.m_nMoveType = msgMoveType.m_nMoveType;

                m_position.m_ESceneID = ESceneMove_DanceGroupCeremonyRoom;
                m_position.m_nMoveType = msgMoveType.m_nMoveType;
                m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcast(msgNotification);
            }
        }
        break;
    default:
        break;
    }
}

void CPlayerInfoComponent::_OnPlayMakerSync(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_PlayMakerSync &request = (GameMsg_C2S_PlayMakerSync &)msg;

    GameMsg_C2S_PlayMakerSync::eReceiverType receiverType 
        = (GameMsg_C2S_PlayMakerSync::eReceiverType)request.m_nReceiverType;
    GameMsg_C2S_PlayMakerSync::eSenceLocationType locationType 
        = (GameMsg_C2S_PlayMakerSync::eSenceLocationType)request.m_nLocationType;

    GameMsg_S2C_PlayMakerBroadcast broadcastMsg;
    broadcastMsg.m_playerMakerInfo = request.m_playerMakerInfo;

    std::list<unsigned int> roleList;
    switch (receiverType)
    {
    case GameMsg_C2S_PlayMakerSync::eReceiverType_All:
        if (locationType == GameMsg_C2S_PlayMakerSync::eSenceLocationType_AmuseRoom)
        {
            CAmuseRoom *pRoom = m_pRoleAmuseRoom->AmuseRoom();
            if (pRoom != NULL && pRoom->RoomID() == (unsigned int)request.m_nRoomID)
            {
                pRoom->SendMsgToAll(&broadcastMsg, m_pRoleAmuseRoom);
            }
        }
        else if (locationType == GameMsg_C2S_PlayMakerSync::eSenceLocationType_WeddingRoom)
        {
            CWeddingRoom *pRoom = m_pRoleCouple->WeddingRoom();
            if (pRoom != NULL && pRoom->RoomID() == (unsigned int)request.m_nRoomID)
            {
                pRoom->SendMsgToAllBut(&broadcastMsg, m_pRoleCouple);
            }
        }
        else
        {
            WriteLog(LOGLEVEL_WARNING, "OnPlayMakerSync: unsupported location type: %d", (int)locationType);
        }
        return;
    case GameMsg_C2S_PlayMakerSync::eReceiverType_Lover:
        roleList.push_back(m_pRoleCouple->CoupleID());

        //TODO: check in same room
        break;
    case GameMsg_C2S_PlayMakerSync::eReceiverType_SpecificConsignee:
        roleList.push_back(request.m_nSpecificID);

        //TODO: check in same room
        break;
    case GameMsg_C2S_PlayMakerSync::eReceiverType_Unknown:
    default:
        WriteLog(LOGLEVEL_WARNING, "OnPlayMakerSync: unsupported sync type: %d", (int)receiverType);
        return;
    }

    SendMsgToPlayerList(roleList, broadcastMsg);
}

void CPlayerInfoComponent::_SetOptionSetting(EPlayerSetting eOption, int nState)
{
    m_playerSetting.m_mapOption[eOption] = nState;

    CRoleEntity* pRole = (CRoleEntity*)Entity();
    if (eOption == EPlayerSetting_LBS && 0 == nState)//关闭LBS
    {
        GameMsg_S2G_CloseLBS msg;
        msg.m_nRoleID = pRole->GetRoleID();
        SendMsg2GroupServer(&msg);
    }
    if (m_pRoleAttr != NULL)
    {
        CPlayerSetting *pPlayerSetting = new CPlayerSetting(m_playerSetting);
        AddQuery(QUERY_Player_SavePlayerSetting, m_pRoleAttr->GetRoleID(), pPlayerSetting, eOption);
    }
}

bool CPlayerInfoComponent::AcceptRoomInvitation()
{
    return m_playerSetting.m_mapOption[EPlayerSetting_ReceiveRoomInvitation] == 1;
}

bool CPlayerInfoComponent::IsOpenLBS()
{
    return m_playerSetting.m_mapOption[EPlayerSetting_LBS] == 1;
}

void CPlayerInfoComponent::ResetRolePosition()
{
    m_position.Reset();
}

void CPlayerInfoComponent::SendMsgToPlayerList(std::list<unsigned int> roleIDList, GameMsg_Base &msg, bool bToOtherLine)
{
    std::list<unsigned int> forwardRoleIDList;
    for (std::list<unsigned int>::iterator it = roleIDList.begin(); it != roleIDList.end(); ++it)
    {
        CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(*it);
        if (pRole != NULL)
        {
            pRole->SendPlayerMsg(&msg);
        }
        else
        {
            if (bToOtherLine)
            {
                forwardRoleIDList.push_back(*it);
            }
        }
    }

    if (bToOtherLine && !forwardRoleIDList.empty())
    {
        GameMsg_Forward forwardMsg;
        forwardMsg.m_RoleIDList = forwardRoleIDList;
        forwardMsg.m_nForwardMsgID = msg.nMsg;
        forwardMsg.m_forwardMsg = &msg;

        SendMsg2GroupServer(&forwardMsg);
    }
}

void CPlayerInfoComponent::_OnForward(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_Forward &forwardMsg = (GameMsg_Forward &)msg;
    SendPlayerMsg(forwardMsg.m_forwardMsg);
}

void CPlayerInfoComponent::SetPlayerInfo(PlayerInfo &playerInfo,  CRoleEntity & role)
{
    CRoleAttribute *pRoleAttr = role.GetComponent<CRoleAttribute>();
    CEntityItemComponent *pRoleItem = role.GetComponent<CRoleItem>();
    CEntityVIPComponent *pRoleVIP = role.GetComponent<CRoleVIP>();
    CEntityAchievementComponent *pRoleAch = role.GetComponent<CRoleAchievement>();
    CEntityGeneComponent* pRoleGene = role.GetComponent<CRoleGene>();
    CRoomComponent *pRoleRoom = role.GetComponent<CRoleRoom>();
    CEntityDanceGroupComponent * pDanceGroupCom = role.GetComponent< CRoleDanceGroup >();
    CEntityCoupleComponent * pRoleCouple = role.GetComponent<CRoleCouple>();
    PetComponent *pPetComponent = role.GetComponent<CRolePet>();
    CEntityMedalComponent *pRoleMedal = role.GetComponent<CRoleMedal>();
    CEntityMallComponent * pRoleMall = role.GetComponent<CRoleMall>();

    if (pRoleAttr == NULL || pRoleItem == NULL || pRoleAch == NULL 
        || pRoleGene == NULL || pRoleRoom == NULL || pDanceGroupCom == NULL 
        || pRoleCouple == NULL || pRoleVIP == NULL) 
    {
        return;
    }

    playerInfo.m_nRoleID = pRoleAttr->GetRoleID();
    playerInfo.SetRoleName(pRoleAttr->m_szRoleName);
    playerInfo.m_nSex = (unsigned char)pRoleAttr->m_nSex;
    playerInfo.SetBirthday(pRoleAttr->GetBirthday());
    playerInfo.m_nConstellation = pRoleAttr->m_nConstellation;
    playerInfo.SetSignature(pRoleAttr->m_szSignature);
    playerInfo.m_nSkinColor = pRoleAttr->m_nSkinColor;
    playerInfo.m_bVIP = pRoleVIP->IsVIP();
    playerInfo.m_nLevel = pRoleAttr->m_nLevel;
    playerInfo.m_nHot = pRoleAttr->m_nHot;

    playerInfo.m_nExp = pRoleAttr->m_nExp;
    playerInfo.m_nCurLine = g_nLine;
    playerInfo.SetCurLineName(g_pCirculator->GetServerName());
    playerInfo.m_nRoomID = (unsigned short)pRoleRoom->RoomID();
    if (playerInfo.m_nRoomID > 0) 
    {
        playerInfo.m_nRoomState = pRoleRoom->RoleRoomState() == RoleRoomState_Start ? 0 : 1;
    }

    std::list<itemtype_t> listItemType;
    pRoleItem->GetEquipedCloth(listItemType);
    playerInfo.m_vecEquipedItem.insert(playerInfo.m_vecEquipedItem.end(), listItemType.begin(), listItemType.end());

    listItemType.clear();
    pRoleMall->GetDesireList(listItemType);
    playerInfo.m_vecWishItem.insert(playerInfo.m_vecWishItem.end(), listItemType.begin(), listItemType.end());

    playerInfo.m_nVIPLevel = (unsigned char)pRoleVIP->VIPLevel();
    playerInfo.m_nAchScore = pRoleAch->m_AchiScore();
    pRoleAch->m_listAchievement.sort();
    for (list<CAchievementInfo>::const_iterator itr = pRoleAch->m_listAchievement.begin(); itr != pRoleAch->m_listAchievement.end(); itr++) {
        playerInfo.m_vecAch.push_back(itr->m_nAchievementID);
    }

    CParamPool IOItemBuff(playerInfo.m_szItemBuff, 65536);
    pRoleItem->PackItemForOther(IOItemBuff);
    playerInfo.m_nItemBuffLen = IOItemBuff.GetValidDataSize();

    CParamPool IOGeneBuff(playerInfo.m_szGeneBuff, 10240);
    pRoleGene->PackGeneForOther(IOGeneBuff);
    playerInfo.m_nGeneBuffLen = IOGeneBuff.GetValidDataSize();
    CDanceGroupInfo * pDGInfo = pDanceGroupCom->GetDanceGroupInfo();
    if (pDGInfo != NULL)
    {
        playerInfo.m_nDanceGroupID = pDGInfo->m_DGBaseInfo.m_nGroupID;
        playerInfo.SetDanceGroupName(pDGInfo->m_DGBaseInfo.m_strGroupName);
    }

    if (pRoleCouple != NULL)
    {
        playerInfo.m_nCoupleID = pRoleCouple->CoupleID();
        playerInfo.SetCoupleName(pRoleCouple->CoupleName() );
        playerInfo.m_nCoupleDate = pRoleCouple->CoupleDate();
        playerInfo.m_nMarriageDate = pRoleCouple->MarriageDate();
        playerInfo.m_nSmallRoleID = pRoleCouple->CoupleSmallRoleID();
    }

    playerInfo.m_bHasPet = false;
    if (pPetComponent != NULL && pPetComponent->HasCarriedPet())
    {
        Pet *pPet = pPetComponent->GetCarriedPet();
        if (pPet != NULL)
        {
            playerInfo.m_bHasPet = true;
            PetService::Instance().EncodePetBrief(*pPet, playerInfo.m_playerPet);
            PetService::Instance().EncodePetSkill(*pPet, playerInfo.m_playerPetSkill);
        }
    }

    if ( pRoleMedal != NULL )
    {
        pRoleMedal->EncodePlayerActivityMedals( playerInfo.m_listActivityMedals );
        pRoleMedal->EncodePlayerShowMedals(playerInfo.m_listShowMedals);
        playerInfo.m_nMedalScore = pRoleMedal->EncodePlayerMedalScore();
    }

    if ( pRoleItem != NULL )
    {
        playerInfo.m_nPerpetualEquipCount = pRoleItem->GetAllPerpetualEquipCountNoRing();
        playerInfo.m_nPerpetualEffectScore = pRoleItem->GetPerpetualEffectEquipScore();
    }
}

void CPlayerInfoComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (pMsg != NULL && m_pNet != NULL)
    {
        m_pNet->SendPlayerMsg(pMsg);
    }
}

void CPlayerInfoComponent::SendGroupServerMsg(GameMsg_Base *pMsg)
{
    if (pMsg != NULL && m_pNet != NULL)
    {
        m_pNet->Send2GroupServer(pMsg);
    }
}

void CPlayerInfoComponent::OnLogout()
{
    SyncPlayerDataToGroup(m_pRoleAttr->GetRoleID(), EOfflinePlayerDataFrom_Logout);
}

void CPlayerInfoComponent::OnLogin()
{
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_LBS))
    {
        m_playerSetting.m_mapOption.erase(EPlayerSetting_LBS);
    }

    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_WifiDownload))
    {
        m_playerSetting.m_mapOption[EPlayerSetting_WifiDownload] = 0;
    }

    GameMSG_S2C_SendPlayerSetting msgSuc;
    msgSuc.m_playerSetting = m_playerSetting;
    SendPlayerMsg(&msgSuc);

    // 结婚纪念日奖励
    if (!m_pRoleCouple->IsMarried())
    {
        return ;
    }

    time_t nGiftDate           = m_pRoleCouple->GetMarriageGiftDate();
    time_t nMarriageDate       = m_pRoleCouple->MarriageDate();
    time_t now                 = time(NULL);

    tm * pTemp                  = localtime( &now );
    tm tmNow(*pTemp);

    pTemp                       = localtime( &nGiftDate );
    tm tmGiftDate(*pTemp);

    pTemp                       = localtime( &nMarriageDate );
    tm tmMarriageDate(*pTemp);

    if ( nGiftDate == 0)
    {
        // 是否过了一个月
        int nDiffMon = ( tmNow.tm_year*12 + tmNow.tm_mon ) - ( tmMarriageDate.tm_year*12 + tmMarriageDate.tm_mon );
        if ( (nDiffMon == 1 && tmNow.tm_mday >= tmMarriageDate.tm_mday)  ||  nDiffMon > 1 )
        {
            // 发放奖励
            m_pRoleCouple->SendMarriageDateGift( now );
        }
    }
    else
    {
        int nDiffMon = ( tmNow.tm_year*12 + tmNow.tm_mon ) - ( tmGiftDate.tm_year*12 + tmGiftDate.tm_mon );
        // 时间在一个月内
        if ( nDiffMon == 0 )
        {
            if ( tmGiftDate.tm_mday < tmMarriageDate.tm_mday && tmNow.tm_mday >= tmMarriageDate.tm_mday )
            {
                // 发放奖励
                m_pRoleCouple->SendMarriageDateGift( now );
            }
        }
        else if ( nDiffMon == 1 )
        {
            if ( tmGiftDate.tm_mday < tmMarriageDate.tm_mday )
            {
                // 发放奖励
                m_pRoleCouple->SendMarriageDateGift( now );
            }
            else if ( tmGiftDate.tm_mday >= tmMarriageDate.tm_mday && tmNow.tm_mday >= tmMarriageDate.tm_mday )
            {
                // 发放奖励
                m_pRoleCouple->SendMarriageDateGift( now );
            }
        }
        else if ( nDiffMon > 1 )
        {
            // 发放奖励
            m_pRoleCouple->SendMarriageDateGift( now );
        }
    }
}

bool CPlayerInfoComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    m_playerSetting = pRoleInforDB->m_playerSetting;

    return true;
}

bool CPlayerInfoComponent::PacketToCache( ROLEINFO_DB* pRoleInforDB ) const
{
    if ( pRoleInforDB == NULL )
        return false;

    pRoleInforDB->m_playerSetting = m_playerSetting;

    return true;
}

const char * CPlayerInfoComponent::SerializeName(void)
{
    return "playerInfo";
}

void CPlayerInfoComponent::SyncPlayerDataToGroup(unsigned int nGainRoleID, EOfflinePlayerDataFrom eFrom)
{
    GameMsg_S2G_GetOfflinePlayerInfo s2gMsg;
    s2gMsg.m_eFrom = eFrom;
    s2gMsg.m_nRoleID = nGainRoleID;

    // 人物基本信息
    s2gMsg.m_OffLineData.m_nRoleID = m_pRoleAttr->GetRoleID();
    s2gMsg.m_OffLineData.m_strRoleName  = m_pRoleAttr->GetRoleName();
    s2gMsg.m_OffLineData.m_nSex = m_pRoleAttr->GetSex();
    s2gMsg.m_OffLineData.m_nSkin = m_pRoleAttr->GetColor();
    s2gMsg.m_OffLineData.m_nLevel = m_pRoleAttr->GetLevel();
    s2gMsg.m_OffLineData.m_bVIP = m_pRoleVIP->IsVIP();
    s2gMsg.m_OffLineData.m_nVIPLevel = (unsigned short)m_pRoleVIP->VIPLevel();

    // 物品信息
    m_pRoleItem->PackOnlinePlayerInfo( s2gMsg.m_OffLineData );

    // 宠物信息
    s2gMsg.m_OffLineData.m_bHasCarriedPet = false;
    if (m_pRolePet != NULL && m_pRolePet->GetCarriedPet() != NULL)
    {
        const Pet *pPet = m_pRolePet->GetCarriedPet();

        s2gMsg.m_OffLineData.m_bHasCarriedPet = true;
        PetService::Instance().EncodePetBrief( *pPet, s2gMsg.m_OffLineData.m_playerPet );
        PetService::Instance().EncodePetSkill( *pPet, s2gMsg.m_OffLineData.m_playerPetSkill );
    }

    SendGroupServerMsg( &s2gMsg );
}
