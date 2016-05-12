#include "PhotoRoomManager.h"
#include "PhotoRoomLuaDataManager.h"
#include "PhotoRoom.h"
#include "../attribute/EntityAttributeComponent.h"
#include "EntityPhotoRoomComponent.h"
#include "../item/EntityItemComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../room/RoomComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../room/RoomManager.h"
#include "../../datastructure/Localization.h"
#include "../PhotoRoom/PhotoRoomLuaDataManager.h"
#include "../logic/PlayerManager.h"
#include "../../datastructure/datainfor/PhotoRoomConfig.h"
#include "../../datastructure/datainfor/PhotoRoomConfigManager.h"
#include "../../socket/Formatter.h"
#include "GameMsg_C2S_PhotoRoom.h"
#include "GameMsg_S2C_PhotoRoom.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../dancegroup/EntityDanceGroupComponent.h"



CPhotoRoomManager::CPhotoRoomManager()
{

}

CPhotoRoomManager::~CPhotoRoomManager()
{

}

CPhotoRoomManager & CPhotoRoomManager::Instance()
{
    static CPhotoRoomManager s_PhotoRoomManager;

    return s_PhotoRoomManager;
}

void CPhotoRoomManager::Init()
{
    for (int i = EPhotoRoomRange_Common_Begin ; i < EPhotoRoomRange_Common_End; ++i) {
        m_listSpawnedID.push_back(i);
    }

    const std::map<int, CPhotoRoomNotifiationTimer *> & rNotificationTimer = CPhotoRoomLuaDataManager::Instance().PhotoRoomNotificationTimerMap();
    TimePhotoRoomMap_t timePhotoMap;
    m_vecTimeExpiredNoficationMap.assign(rNotificationTimer.size(), timePhotoMap);
    const std::map<unsigned int, PhotoRoomTheme *> & themeMap = PhotoRoomConfigManager::Instance().PhotoRoomThemeMap();
    for (std::map<unsigned int, PhotoRoomTheme *>::const_iterator cit = themeMap.begin(); cit != themeMap.end(); ++cit)
    {
        if (m_mapAlThemelRoom.find(cit->first) == m_mapAlThemelRoom.end())
        {
            PhotoRoomMap m;
            m_mapAlThemelRoom.insert(make_pair(cit->first, m));
        }
    }
}

void CPhotoRoomManager::OnUpdate(const unsigned long &lTimeElapsed)
{
    int nTimeNow = (int)time(NULL);
    for (TimePhotoRoomMap_t::iterator itr = m_mapTimeExpiredRooms.begin(); itr != m_mapTimeExpiredRooms.end();) {
        if (itr->second != NULL) {
            if (itr->first <= nTimeNow) {
                CPhotoRoom *pRoom = itr->second;
                m_mapTimeExpiredRooms.erase(itr++);
                GameMsg_S2C_PhotoRoomCloseNotification msgCloseNotification;
                msgCloseNotification.m_eRoomClosedCause = ERoomClosedCause_TimeExpired; 
                CRoleEntity*pHost = CPlayerManager::Instance().GetEntityByRoleID(pRoom->HostID());
                if (pHost != NULL) {
                    pHost->SendPlayerMsg(&msgCloseNotification);
                }
                pRoom->SendMsgToAllBut(&msgCloseNotification, (pHost == NULL) ? NULL : pHost->GetComponent<CRolePhotoRoom>());
                _DestroyPhotoRoom(pRoom->RoomID());
            } else {
                break;
            }
        }
    }

    for (int i = 0; i < (int)m_vecTimeExpiredNoficationMap.size(); ++i) {
        TimePhotoRoomMap_t & timePhotoRoomMap = m_vecTimeExpiredNoficationMap[i];
        for (TimePhotoRoomMap_t::iterator itrRoom = timePhotoRoomMap.begin(); itrRoom != timePhotoRoomMap.end(); ) {
            if (itrRoom->first <= nTimeNow) {
                CPhotoRoom * pRoom = itrRoom->second;
                timePhotoRoomMap.erase(itrRoom++);
                if (pRoom != NULL) {
                    pRoom->OnRoomExpiredNotification(i+1);
                }
            } else {
                break;
            }
        }
    }
}

void CPhotoRoomManager::PlayerLogout(CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleAttr() != NULL) {
        CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
        if (pRoom != NULL) {
            _PlayerOutPhotoRoom(rRolePhotoRoom);
            GameMsg_S2C_PlayerQuitPhotoRoom msgNotification;
            msgNotification.m_nRoleID = rRolePhotoRoom.RoleAttr()->GetRoleID();
            pRoom->SendMsgToAll(&msgNotification);
        }
    }
}

void CPhotoRoomManager::OnGetMyPhotoRoomInfo(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    EGetMyPhotoRoomInfoError eError = EGetMyPhotoRoomInfoError_Success;
    if (rRolePhotoRoom.RoleAttr() == NULL) {
        eError = EGetMyPhotoRoomInfoError_unknown;	
        GameMsg_S2C_GetMyPhotoRoomInfoFail msgFail;
        msgFail.m_eError = eError;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
        return;
    }

    GameMsg_S2C_GetMyPhotoRoomInfoSuc msgSuc;		
    CPhotoRoom *pRoom = _FindHostRoom(rRolePhotoRoom.RoleAttr()->GetRoleID());
    if (pRoom != NULL) {
        msgSuc.m_nRoomID = pRoom->RoomID();		
        msgSuc.m_eThemeID = pRoom->RoomTheme();
        msgSuc.m_nTimeRemain = pRoom->RemainTime();
    }

    rRolePhotoRoom.SendPlayerMsg(&msgSuc);
}

void CPhotoRoomManager::OnCreatePhotoRoom( GameMsg_Base &rMsg, CEntityPhotoRoomComponent & rRolePhotoRoom )
{
    if (rRolePhotoRoom.RoleAttr() == NULL || rRolePhotoRoom.RoleRoom() == NULL || rRolePhotoRoom.RoleCouple() == NULL || rRolePhotoRoom.RoleItem() == NULL ) {
        return;
    }

    GameMsg_C2S_CreatePhotoRoom & rMsgCreate = (GameMsg_C2S_CreatePhotoRoom &)rMsg;

    ECreatePhotoRoomError nError = _CanCreatePhotoRoom(rRolePhotoRoom, rMsgCreate.m_nRoomTheme);
    if (nError != ECreatePhotoRoomError_Success) {
        GameMsg_S2C_CreatePhotoRoomFail msgFail;
        msgFail.m_eErrno = nError;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
        return;
    }

    //get photo room id
    unsigned int nPhotoRoomID = _GeneratePhotoRoomID();
    if (nPhotoRoomID <= 0) 
    {
        GameMsg_S2C_CreatePhotoRoomFail msgFail;
        msgFail.m_eErrno = ECreatePhotoRoomError_RoomsLimited;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
        return;
    }

    //construct room
    const PhotoRoomTheme * pPhotoTheme = PhotoRoomConfigManager::Instance().GetPhotoRoomTheme(rMsgCreate.m_nRoomTheme);
    if (pPhotoTheme != NULL) 
    {
        ECreatePhotoRoomError eError = ECreatePhotoRoomError_Success ;
        if (pPhotoTheme->RequiredItemType() > 0) {
            if (!rRolePhotoRoom.RoleItem()->RemoveItemByType(pPhotoTheme->RequiredItemType(), pPhotoTheme->RequiredItemCount(), EItemAction_Del_CreatePhotoRoom)) {
                eError = ECreatePhotoRoomError_Unkonwn;			
                WriteLog(LOGLEVEL_ERROR, "error player[%d] create photo room, use item failed", rRolePhotoRoom.RoleAttr()->GetRoleID());
            }
        }

        if (eError == ECreatePhotoRoomError_Success) 
        {
            CPhotoRoom *pPhotoRoom = new CPhotoRoom;
            pPhotoRoom->SetRoomID(nPhotoRoomID);
            pPhotoRoom->SetRoomTheme((EPhotoRoomTheme)rMsgCreate.m_nRoomTheme);
            pPhotoRoom->SetHostID(rRolePhotoRoom.RoleAttr()->GetRoleID());
            pPhotoRoom->SetCreateTime((unsigned int)time(NULL));
            pPhotoRoom->SetRoomState(EPhotoRoomState_Free);
            pPhotoRoom->SetSelectedBG(pPhotoTheme->DefaultThemeBG());
            pPhotoRoom->SetExistTime(pPhotoTheme->DefaultExistedTime());
            std::string strName;
            SafeFormat(strName, CLocalization::Instance().GetString("Photo_Room_Name"), rRolePhotoRoom.RoleAttr()->GetRoleName());
            pPhotoRoom->SetRoomName(strName);
            //enter room
            if (!_PlayerInPhotoRoom(rRolePhotoRoom, pPhotoRoom)) {
                _ReleasePhotoRoomID(nPhotoRoomID);
                delete pPhotoRoom, pPhotoRoom = NULL;

                GameMsg_S2C_CreatePhotoRoomFail msgFail;
                msgFail.m_eErrno = ECreatePhotoRoomError_Unkonwn;
                rRolePhotoRoom.SendPlayerMsg(&msgFail);
                return;
            }
            //add room to manager
            _AddPhotoRoom(pPhotoRoom);
            WriteLog(LOGLEVEL_DEBUG, "plaer[%d] created photo room[%u] at time[%u]", rRolePhotoRoom.RoleAttr()->GetRoleID(), pPhotoRoom->RoomID(), pPhotoRoom->CreateTime() );
        }
    }
}

void CPhotoRoomManager::OnQuitPhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleAttr() == NULL) {
        return;
    }

    CPhotoRoom * pPhotoRoom = rRolePhotoRoom.PhotoRoom();
    if (pPhotoRoom == NULL) 
    {
        GameMsg_S2C_QuitPhotoRoomFail msgFail;
        msgFail.m_eErrno = EQuitPhotoRoomError_NotInRoom;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
        return;
    }

    CEntityPhotoRoomComponent *pRole = _PlayerOutPhotoRoom(rRolePhotoRoom);
    if (pRole->IsHost()) {
        pPhotoRoom->SetRoomState(EPhotoRoomState_Free);
        GameMsg_S2C_PhotoRoomStateChangedNotification msgNotification;
        msgNotification.m_eRoomState = pPhotoRoom->RoomState();
        if (pPhotoRoom->PlayerCount() > 0) 
        {
            pPhotoRoom->SendMsgToAll(&msgNotification);
        }
    }

    GameMsg_S2C_QuitPhotoRoomSuc msgSuc;
    rRolePhotoRoom.SendPlayerMsg(&msgSuc);

    GameMsg_S2C_PlayerQuitPhotoRoom msgPlayerQuit;
    msgPlayerQuit.m_nRoleID = rRolePhotoRoom.RoleAttr()->GetRoleID();
    pPhotoRoom->SendMsgToAllBut(&msgPlayerQuit, &rRolePhotoRoom);
}

void CPhotoRoomManager::OnClosePhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleAttr() == NULL) 
    {
        return;
    }

    CPhotoRoom * pPhotoRoom = rRolePhotoRoom.PhotoRoom();
    if (pPhotoRoom == NULL) {
        GameMsg_S2C_ClosePhotoRoomFail msgFail;
        msgFail.m_eErrno = EClosePhotoRoomError_NotInRoom;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
        return;
    }

    if (pPhotoRoom->HostID() != rRolePhotoRoom.RoleAttr()->GetRoleID()) 
    {
        GameMsg_S2C_ClosePhotoRoomFail msgFail;
        msgFail.m_eErrno = EClosePhotoRoomError_NotHost;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
        return;
    }

    GameMsg_S2C_PhotoRoomCloseNotification msgCloseNotification;
    msgCloseNotification.m_eRoomClosedCause = ERoomClosedCause_HostClosed;
    pPhotoRoom->SendMsgToAll(&msgCloseNotification);

    WriteLog(LOGLEVEL_DEBUG, "player[%u] close photo room:id[%u], create time[%u]", rRolePhotoRoom.RoleAttr()->GetRoleID(), pPhotoRoom->RoomID(), pPhotoRoom->CreateTime());
    _DestroyPhotoRoom(pPhotoRoom->RoomID());
}

void CPhotoRoomManager::OnInvitedPlayer(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    GameMsg_C2S_InvitePhotoRoomPlayer & rMsgInvite = (GameMsg_C2S_InvitePhotoRoomPlayer &)rMsg;

    if (rRolePhotoRoom.RoleAttr() == NULL || rRolePhotoRoom.RoleFriend() == NULL || rMsgInvite.m_listInvitees.size() == 0) 
    {
        return;
    }

    EInvitePhotoRoomError eErrorInvite =  _CanInvitePhotoRoomPlayer(rRolePhotoRoom, rMsgInvite.m_listInvitees);
    if (eErrorInvite == EInvitePhotoRoomError_Success ) {
        CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
        const PhotoRoomTheme *pRoomTheme = PhotoRoomConfigManager::Instance().GetPhotoRoomTheme(pRoom->RoomTheme());
        if (pRoomTheme != NULL) 
        {
            if (pRoom != NULL ) 
            {
                GameMsg_S2C_PhotoRoomInviteeNotification msgNotification;
                msgNotification.m_nRoomID = pRoom->RoomID();
                msgNotification.m_nRoomTheme = (unsigned char)pRoom->RoomTheme();
                msgNotification.m_nInviterID = rRolePhotoRoom.RoleAttr()->GetRoleID();
                msgNotification.m_strInviterName = rRolePhotoRoom.RoleAttr()->GetRoleName();

                if (pRoomTheme->InviteType() == EPhotoRoomInviteType_InviteFriend) 
                {
                    for (list<unsigned int>::iterator itr = rMsgInvite.m_listInvitees.begin(); itr != rMsgInvite.m_listInvitees.end(); ++itr) {
                        if (rRolePhotoRoom.RoleFriend()->IsFriend(*itr)) 
                        {
                            CRoleEntity *pInvitee =  CPlayerManager::Instance().GetEntityByRoleID(*itr);
                            if (pInvitee != NULL && pInvitee->TryBeInvitedToPhotoRoom(pRoom->RoomID(), pRoom->CreateTime())) 
                            {
                                pInvitee->SendPlayerMsg(&msgNotification);
                            }
                        }
                    }
                } 
                else if(pRoomTheme->InviteType() == EPhotoRoomInviteType_Couple) 
                {
                    CRoleEntity *pInvitee =  CPlayerManager::Instance().GetEntityByRoleID(*rMsgInvite.m_listInvitees.begin());
                    if (pInvitee != NULL && pInvitee->TryBeInvitedToPhotoRoom(pRoom->RoomID(), pRoom->CreateTime())) 
                    {
                        pInvitee->SendPlayerMsg(&msgNotification);
                        pInvitee->SetLastInvitedPhotoRoomPlayerTime((int)time(NULL));
                    }
                }
            }
            GameMsg_S2C_InvitePhotoRoomPlayerSuc msgSuc;
            rRolePhotoRoom.SendPlayerMsg(&msgSuc);
        }
    }

    if (eErrorInvite != EInvitePhotoRoomError_Success) 
    {
        GameMsg_S2C_InvitePhotoRoomPlayerFail msgFail;
        msgFail.m_eError = eErrorInvite;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
    }
}

void CPhotoRoomManager::OnInviteeResponse(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleCouple() == NULL || rRolePhotoRoom.RoleRoom() == NULL || rRolePhotoRoom.RoleAttr() == NULL) {
        return;
    }

    GameMsg_C2S_PhotoRoomInviteeResponse & rMsgResponse = (GameMsg_C2S_PhotoRoomInviteeResponse &)rMsg;
    if (rMsgResponse.m_bAccept) 
    {
        PhotoRoomInviteeResponseError eRes = _CanBeInvitedToPhotoRoom(rRolePhotoRoom, rMsgResponse.m_nRoomID);
        if (eRes == PhotoRoomInviteeResponseError_Success) 
        {
            CPhotoRoom *pPhotoRoom = _FindPhotoRoom(rMsgResponse.m_nRoomID);
            if (pPhotoRoom != NULL) 
            {
                if (_PlayerInPhotoRoom(rRolePhotoRoom, pPhotoRoom)) 
                {
                    rRolePhotoRoom.InvitedFinished();
                    CRoomComponent* pPlayer = CRoomManager::Instance().GetLobbyPlayer(rRolePhotoRoom.RoleAttr()->GetRoleID());//退出大厅
                    if (pPlayer != NULL) 
                    {
                        CRoomManager::Instance().ExitLobby(*pPlayer);
                    }
                } 
                else 
                {
                    eRes = PhotoRoomInviteeResponseError_InvalidData;
                }
            }
        }

        if (eRes != PhotoRoomInviteeResponseError_Success) 
        {
            GameMsg_S2C_PhotoRoomInviteeResponseFail msgFail;
            msgFail.m_eError = eRes;
            rRolePhotoRoom.SendPlayerMsg(&msgFail);
        }
    }

    rRolePhotoRoom.InvitedOver(rMsgResponse.m_nRoomID);
}

void CPhotoRoomManager::OnTryEnterPhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleAttr() == NULL || rRolePhotoRoom.RoleRoom() == NULL || rRolePhotoRoom.RoleCouple() == NULL) {
        return;
    }

    GameMsg_C2S_TryEnterMyPhotoRoom & rMsgTryEnter = (GameMsg_C2S_TryEnterMyPhotoRoom &)rMsg;

    ETryEnterMyPhotoRoomError eError = _CanEnterMyPhotoRoom(rRolePhotoRoom, (EPhotoRoomTheme)rMsgTryEnter.m_nRoomType );
    if (eError != ETryEnterMyPhotoRoomError_Success) {
        GameMsg_S2C_TryEnterMyPhotoRoomFail msgFail;
        msgFail.m_eError = eError;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
        return;
    }

    CPhotoRoom *pPhotoRoom =_FindHostRoom(rRolePhotoRoom.RoleAttr()->GetRoleID());
    if (pPhotoRoom != NULL) {
        if (!_PlayerInPhotoRoom(rRolePhotoRoom, pPhotoRoom)) {
            GameMsg_S2C_TryEnterMyPhotoRoomFail msgFail;
            msgFail.m_eError = ETryEnterMyPhotoRoomError_Unkonwn;
            rRolePhotoRoom.SendPlayerMsg(&msgFail);
        }
    }
}

void CPhotoRoomManager::OnKickPhotoRoomPlayer(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleAttr() == NULL) {
        return;
    }

    GameMsg_C2S_KickPhotoRoomPlayer & rMsgKickPlayer = (GameMsg_C2S_KickPhotoRoomPlayer &)rMsg;
    EKickPhotoRoomPlayerError eError = _CanKickPhotoRoomPlayer(rRolePhotoRoom, rMsgKickPlayer.m_nBeKickedID);

    CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
    if (pRoom != NULL) {
        CEntityPhotoRoomComponent *pBeKickedRole = pRoom->GetPlayer(rMsgKickPlayer.m_nBeKickedID);
        if (pBeKickedRole != NULL) {
            CEntityPhotoRoomComponent *pRoleKicked = _PlayerOutPhotoRoom(*pBeKickedRole);
            if (pRoleKicked != NULL) {
                GameMsg_S2C_KickPhotoRoomPlayerSuc msgSuc;
                msgSuc.m_nKickedID = rMsgKickPlayer.m_nBeKickedID;
                rRolePhotoRoom.SendPlayerMsg(&msgSuc);

                GameMsg_S2C_QuitPhotoRoomSuc msgQuitSuc;
                msgQuitSuc.m_nKickedBy = rRolePhotoRoom.RoleAttr()->GetRoleID();
                pRoleKicked->SendPlayerMsg(&msgQuitSuc);

                GameMsg_S2C_PlayerQuitPhotoRoom msgNoticeAll;
                msgNoticeAll.m_nRoleID = rMsgKickPlayer.m_nBeKickedID;
                pRoom->SendMsgToAllBut(&msgNoticeAll, pBeKickedRole);
            }
        }
    }

    if (eError != EKickPhotoRoomPlayerError_Success) {
        GameMsg_S2C_KickPhotoRoomPlayerFail msgFail;
        msgFail.m_eError = eError;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
    }
}

void CPhotoRoomManager::OnChangePhotoRoomPlayerState(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleAttr() == NULL) {
        return;
    }

    GameMsg_C2S_ChangePhotoRoomPlayerState & rMsgChangeState = (GameMsg_C2S_ChangePhotoRoomPlayerState &)rMsg;
    EPhotoRoomPlayerState eChangeState = (EPhotoRoomPlayerState)rMsgChangeState.m_nState;

    EPhotoRoomPlayerChangeStateError eError = _CanChangePhotoRoleState(rRolePhotoRoom, eChangeState);

    if (eError == EPhotoRoomPlayerChangeStateError_Success) {
        CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
        if (pRoom != NULL) {
            rRolePhotoRoom.SetPlayerState(eChangeState);
            GameMsg_S2C_ChangeRoomPlayerStateSuc msgSuc;
            msgSuc.m_eState = eChangeState;
            rRolePhotoRoom.SendPlayerMsg(&msgSuc);

            GameMsg_S2C_PhotoRoomPlayerChangeStateNotification msgNotification;
            msgNotification.m_eState = eChangeState;
            msgNotification.m_nRoleID = rRolePhotoRoom.RoleAttr()->GetRoleID();
            pRoom->SendMsgToAllBut(&msgNotification, &rRolePhotoRoom);
        } else {
            eError = EPhotoRoomPlayerChangeStateError_NotInPhotoRoom;
        }
    }

    if (eError != EPhotoRoomPlayerChangeStateError_Success) {
        GameMsg_S2C_ChangeRoomPlayerStateFail msgFail;
        msgFail.m_eError = (EPhotoRoomPlayerChangeStateError)rMsgChangeState.m_nState;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);	
    }
}

void CPhotoRoomManager::OnChangePhotoRoomState(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    GameMsg_C2S_ChangePhotoRoomState & rMsgChange = (GameMsg_C2S_ChangePhotoRoomState &)rMsg;
    EPhotoRoomState eRoomState = (EPhotoRoomState)rMsgChange.m_nRoomState;

    EChangePhotoRoomStateError eError = _CanChangePhotoRoomState(rRolePhotoRoom, eRoomState);

    if (eError == EChangePhotoRoomStateError_Success) {
        CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
        if (pRoom != NULL) {
            pRoom->SetRoomState(eRoomState);
            GameMsg_S2C_ChangePhotoRoomStateSuc msgSuc;
            msgSuc.m_eRoomState = eRoomState;
            rRolePhotoRoom.SendPlayerMsg(&msgSuc);

            GameMsg_S2C_PhotoRoomStateChangedNotification msgNotification;
            msgNotification.m_eRoomState = eRoomState;
            pRoom->SendMsgToAllBut(&msgNotification, &rRolePhotoRoom);
        } else {
            eError = EChangePhotoRoomStateError_NotInPhotoRoom;
        }
    }

    if (eError != EChangePhotoRoomStateError_Success) {
        GameMsg_S2C_ChangePhotoRoomStateFail msgFail;
        msgFail.m_eError = eError;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
    }
}

void CPhotoRoomManager::OnPlayerChangeRoomAction(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleItem() == NULL) {
        return;
    }

    GameMsg_C2S_ChangePhotoRoomAction & rMsgChange = (GameMsg_C2S_ChangePhotoRoomAction &)rMsg;
    EChangePhotoRoomActionError eError = _CanChangePhtotoRoomAction(rRolePhotoRoom, rMsgChange.m_nActionID);
    if (eError == EChangePhotoRoomActionError_Success) {
        CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
        if (pRoom != NULL) {
            const PhotoRoomGroupAction *pGroupAction = PhotoRoomConfigManager::Instance().GroupActionInfo(rMsgChange.m_nActionID);
            if (pGroupAction != NULL) {
                if (!pRoom->IsInRoomActionList(rMsgChange.m_nActionID)) {
                    if (pGroupAction->m_nRequiredItemType > 0) {
                        if (rRolePhotoRoom.RoleItem()->RemoveItemByType(pGroupAction->m_nRequiredItemType, pGroupAction->m_nRequiredItemCount, EItemAction_Del_ChangePhotoRoomAction)) {
                            pRoom->AddRoomAction(pGroupAction);
                        } else {
                            eError = EChangePhotoRoomActionError_Unkonwn;
                        }
                    }
                }
                if (eError == EChangePhotoRoomActionError_Success) {
                    pRoom->SetSelectedAction(rMsgChange.m_nActionID);

                    GameMsg_S2C_ChangePhotoRoomActionSuc msgSuc;
                    msgSuc.m_nActionID = rMsgChange.m_nActionID;
                    rRolePhotoRoom.SendPlayerMsg(&msgSuc);

                    GameMsg_S2C_PhotoRoomActionChangedNotification msgNotification;
                    msgNotification.m_nActionID = rMsgChange.m_nActionID;
                    pRoom->SendMsgToAllBut(&msgNotification, &rRolePhotoRoom);
                }
            } else {
                eError = EChangePhotoRoomActionError_NotSuchAction;
            }
        } else {
            eError = EChangePhotoRoomActionError_NotInPhotoRoom;
        }
    }

    if (eError != EChangePhotoRoomActionError_Success) {
        GameMsg_S2C_ChangePhotoRoomActionFail msgFail;
        msgFail.m_eError = eError;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
    }
}

void CPhotoRoomManager::OnPlayerChangeRoomBG(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleItem() == NULL) {
        return;
    }

    GameMsg_C2S_ChangePhotoRoomBG & rMsgChange = (GameMsg_C2S_ChangePhotoRoomBG &)rMsg;
    EChangePhotoRoomBGError eError = _CanChangePhotoRoomBG(rRolePhotoRoom, rMsgChange.m_nBGID);
    if (eError == EChangePhotoRoomBGError_Success) {
        CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
        if (pRoom != NULL) {
            const PhotoRoomConfig *pBGInfo = PhotoRoomConfigManager::Instance().PhotoRoomBG(rMsgChange.m_nBGID);
            if (pBGInfo != NULL) {
                if (!pRoom->IsInRoomBGList(rMsgChange.m_nBGID)) {
                    if (pBGInfo->m_nRequiredItemType > 0) {
                        if (rRolePhotoRoom.RoleItem()->RemoveItemByType(pBGInfo->m_nRequiredItemType, pBGInfo->m_nRequiredItemCount, EItemAction_Del_ChangePhotoRoomBG)) {
                            pRoom->AddRoomBG(pBGInfo);
                        } else {
                            eError = EChangePhotoRoomBGError_Unknown;
                        }
                    }
                }
                if (eError == EChangePhotoRoomBGError_Success) {
                    pRoom->SetSelectedBG(rMsgChange.m_nBGID);
                    GameMsg_S2C_ChangePhotoRoomBGSuc msgSuc;
                    msgSuc.m_nBGID = rMsgChange.m_nBGID;
                    rRolePhotoRoom.SendPlayerMsg(&msgSuc);

                    GameMsg_S2C_PhotoRoomBGChangedNotification msgNotification;
                    msgNotification.m_nBGID = rMsgChange.m_nBGID;
                    pRoom->SendMsgToAllBut(&msgNotification, &rRolePhotoRoom);
                }
            } else {
                eError = EChangePhotoRoomBGError_RoomNotSupport;
            }
        } else {
            eError = EChangePhotoRoomBGError_NotInPhotoRoom;
        }
    }

    if (eError != EChangePhotoRoomBGError_Success) {
        GameMsg_S2C_ChangePhotoRoomBGFail msgFail;
        msgFail.m_eError = eError;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
    }	
}

void CPhotoRoomManager::OnRenewPhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleItem() == NULL) {
        return;
    }

    ERenewPhotoRoomError eError = _CanRenewPhotoRoom(rRolePhotoRoom);	
    if (eError == ERenewPhotoRoomError_Success) {
        CPhotoRoom *pRoom = _FindHostRoom(rRolePhotoRoom.RoleAttr()->GetRoleID());	
        if (pRoom != NULL) {
            const PhotoRoomTheme *pTheme = PhotoRoomConfigManager::Instance().GetPhotoRoomTheme(pRoom->RoomTheme());
            if (pTheme != NULL) {
                if (pTheme->RenewItemType() > 0 && pTheme->RenewItemCount() > 0) {
                    if (rRolePhotoRoom.RoleItem()->RemoveItemByType(pTheme->RenewItemType(), pTheme->RenewItemCount(), EItemAction_Del_RenewPhotoRoom)) {
                        pRoom->SetExistTime(pRoom->ExistTime() + pTheme->RenewTime());
                        _OutTimeExpiredPhotoRoomContainer(pRoom->RoomID());
                        _OutTimeExpiredNotificationContainer(pRoom->RoomID());
                        _InTimeExpiredPhotoRoomContainer(pRoom);
                        _InTimeExpiredNotificationContainer(pRoom);

                        GameMsg_S2C_RenewPhotoRoomSuc msgSuc;
                        msgSuc.m_nTime = pRoom->RemainTime();
                        pRoom->SendMsgToAll(&msgSuc);
                    } else {
                        eError = ERenewPhotoRoomError_Unknown;		
                    }
                }
            }
        }
    }

    if (eError != ERenewPhotoRoomError_Success) {
        GameMsg_S2C_RenewPhotoRoomFail msgFail;
        msgFail.m_eError = eError;
        rRolePhotoRoom.SendPlayerMsg(&msgFail);
    }
}

ECreatePhotoRoomError CPhotoRoomManager::_CanCreatePhotoRoom(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nRoomTheme)
{
    unsigned int nNowTime = (unsigned int)time(NULL);
    unsigned int nOpenTime = CSystemSettingMgr::Instance().PhotoRoomStartTime();
    if (nOpenTime > nNowTime)
    {
        return ECreatePhotoRoomError_NotOpen;
    }
    const PhotoRoomTheme * pRoomTheme =  PhotoRoomConfigManager::Instance().GetPhotoRoomTheme(nRoomTheme);
    if (pRoomTheme == NULL) {
        return ECreatePhotoRoomError_RoomTypeError;
    }
    PhotoRoomMapMap::iterator it = m_mapAlThemelRoom.find(nRoomTheme);
    if (it == m_mapAlThemelRoom.end())
    {
        return ECreatePhotoRoomError_RoomTypeError;
    }

    //level
    if (rRolePhotoRoom.RoleAttr() == NULL || rRolePhotoRoom.RoleAttr()->GetLevel() < pRoomTheme->RequiredLevel()) {
        return ECreatePhotoRoomError_LevelNotEnough;
    }

    //has item
    if (pRoomTheme->RequiredItemType() > 0) {
        if (rRolePhotoRoom.RoleItem() == NULL || rRolePhotoRoom.RoleItem()->GetItemCount(pRoomTheme->RequiredItemType(), false, false) < pRoomTheme->RequiredItemCount()) {
            return ECreatePhotoRoomError_NotHasRequireItem;
        }
    }

    //not created room yet
    CPhotoRoom * pRoom = _FindHostRoom(rRolePhotoRoom.RoleAttr()->GetRoleID());
    if (pRoom != NULL) {
        return ECreatePhotoRoomError_AlreadyHasOwnRoomCurLine;
    }

    //not in photoroom
    if (rRolePhotoRoom.PhotoRoom() != NULL) {
        return ECreatePhotoRoomError_InPhotoRoom;
    }

    //not in dance room
    if (rRolePhotoRoom.RoleRoom()->IsInRoom()) {
        return ECreatePhotoRoomError_InDanceRoom;
    }

    //not in wedding room
    if (rRolePhotoRoom.RoleCouple()->WeddingRoom() != NULL) {
        return ECreatePhotoRoomError_InWeddingRoom;
    }

    //not in amuse room
    if (rRolePhotoRoom.RoleAmuseRoom()->AmuseRoom() != NULL) {
        return ECreatePhotoRoomError_InAmuseRoom;
    }

    if (rRolePhotoRoom.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        return ECreatePhotoRoomError_InCeremonyRoom;
    }


    //not in lobby
    if (CRoomManager::Instance().GetLobbyPlayer(rRolePhotoRoom.RoleAttr()->GetRoleID()) != NULL) {
        return ECreatePhotoRoomError_InLobby;
    }

    return ECreatePhotoRoomError_Success;
}

EInvitePhotoRoomError CPhotoRoomManager::_CanInvitePhotoRoomPlayer(CEntityPhotoRoomComponent &rRolePhotoRoom, std::list<unsigned int> &listInvitees)
{
    if (rRolePhotoRoom.RoleAttr() == NULL) {
        return EInvitePhotoRoomError_InvalidData;
    }

    CPhotoRoom *pPhotoRoom = rRolePhotoRoom.PhotoRoom();
    if (pPhotoRoom == NULL) {
        return EInvitePhotoRoomError_NotInRoom;
    }

    const PhotoRoomTheme * pTheme = PhotoRoomConfigManager::Instance().GetPhotoRoomTheme(pPhotoRoom->RoomTheme());

    if (pTheme == NULL || pTheme->MaxPlayerCount() < 2 || pTheme->InviteType() == EPhotoRoomInviteType_None) {
        return EInvitePhotoRoomError_RoomNotSupport;
    }

    if (pPhotoRoom->HostID() != rRolePhotoRoom.RoleAttr()->GetRoleID()) {
        return EInvitePhotoRoomError_NotHost;
    }

    if (pTheme->InviteType() == EPhotoRoomInviteType_Couple) {
        if (listInvitees.size() > 0 && listInvitees.size() < 2) {
            CRoleEntity *pInvitee =  CPlayerManager::Instance().GetEntityByRoleID(*listInvitees.begin());
            if (pInvitee != NULL && pInvitee->PairID() != rRolePhotoRoom.RoleCouple()->PairID()) {
                return EInvitePhotoRoomError_NotCouple;
            }
        } else {
            return EInvitePhotoRoomError_TooMany;	
        }
    }

    if (pTheme->InviteType() == EPhotoRoomInviteType_InviteFriend) {
        if ( listInvitees.size() > (size_t)CPhotoRoomLuaDataManager::Instance().MaxInvitePlayerCount() ) {
            return EInvitePhotoRoomError_TooMany;
        }
    }

    for (std::list<unsigned int>::iterator itr = listInvitees.begin(); itr != listInvitees.end(); ++itr) {
        CRoleEntity *pInvitee =  CPlayerManager::Instance().GetEntityByRoleID(*itr);
        if (pInvitee != NULL) {
            if (pInvitee->GetRoomID() > 0 || pInvitee->WeddingRoom() != NULL || pInvitee->AmuseRoom() != NULL
                || pInvitee->GetCeremonyRoom() != NULL) { //在大厅可被邀请
                return EInvitePhotoRoomError_InOtherPlace;
            }

            if (pInvitee->PhotoRoom() != NULL) {
                return EInvitePhotoRoomError_AlreadyInPhotoRoom;
            }

            if (pInvitee->GetRoleID() == rRolePhotoRoom.RoleAttr()->GetRoleID()) {
                return EInvitePhotoRoomError_InviteSelf;
            }
        }
    }

    if (rRolePhotoRoom.LastInvitedTime() + CPhotoRoomLuaDataManager::Instance().InvitePlayerInterval() > time(NULL)) {
        return EInvitePhotoRoomError_TooFrequent;
    }

    return EInvitePhotoRoomError_Success;
}

PhotoRoomInviteeResponseError CPhotoRoomManager::_CanBeInvitedToPhotoRoom(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nRoomID)
{
    if (rRolePhotoRoom.RoleRoom() == NULL || rRolePhotoRoom.RoleCouple() == NULL) {
        return PhotoRoomInviteeResponseError_InvalidData;
    }

    if (rRolePhotoRoom.RoleRoom()->IsInRoom()) {
        return PhotoRoomInviteeResponseError_InDancingRoom;
    }

    if (rRolePhotoRoom.RoleCouple()->WeddingRoom() != NULL) {
        return PhotoRoomInviteeResponseError_InWeddingRoom;
    }

    if (rRolePhotoRoom.RoleAmuseRoom()->AmuseRoom() != NULL) {
        return PhotoRoomInviteeResponseError_InAmuseRoom;
    }

    if (rRolePhotoRoom.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        return PhotoRoomInviteeResponseError_InCeremonyRoom;
    }

    CPhotoRoom *pPhotoRoom =_FindPhotoRoom(nRoomID);
    if (pPhotoRoom == NULL) {
        return PhotoRoomInviteeResponseError_NoPhotoRoom;
    }

    const PhotoRoomTheme * pThemeRoom = PhotoRoomConfigManager::Instance().GetPhotoRoomTheme(pPhotoRoom->RoomTheme());
    if (pThemeRoom == NULL || pThemeRoom->InviteType() == EPhotoRoomInviteType_None) {
        return PhotoRoomInviteeResponseError_NotSupportInvite;	
    }

    if (pPhotoRoom->PlayerCount() == pThemeRoom->MaxPlayerCount()) {
        return PhotoRoomInviteeResponseError_RoomFull;
    }

    if (!rRolePhotoRoom.IsInvited(pPhotoRoom->RoomID(), pPhotoRoom->CreateTime())) {
        return PhotoRoomInviteeResponseError_NoInvitation;
    }

    if (pPhotoRoom->RoomState() == EPhotoRoomState_Start) {
        return PhotoRoomInviteeResponseError_Started;
    }

    return PhotoRoomInviteeResponseError_Success;
}

ETryEnterMyPhotoRoomError CPhotoRoomManager::_CanEnterMyPhotoRoom(CEntityPhotoRoomComponent &rRolePhotoRoom, EPhotoRoomTheme eRoomType)
{
    if (rRolePhotoRoom.RoleAttr() == NULL || rRolePhotoRoom.RoleRoom() == NULL || rRolePhotoRoom.RoleCouple() == NULL) {
        return ETryEnterMyPhotoRoomError_Unkonwn;
    }

    CPhotoRoom *pRoom =_FindHostRoom(rRolePhotoRoom.RoleAttr()->GetRoleID());
    if (pRoom == NULL) {
        return ETryEnterMyPhotoRoomError_NotCreateSuchRoom;
    }

    if (pRoom->RoomTheme() != eRoomType) {
        return ETryEnterMyPhotoRoomError_HasAnotherRoom;
    }

    if (rRolePhotoRoom.PhotoRoom() != NULL) {
        return ETryEnterMyPhotoRoomError_InPhotoRoom;
    }

    if (rRolePhotoRoom.RoleRoom()->IsInRoom()) {
        return ETryEnterMyPhotoRoomError_InDanceRoom;
    }

    if (rRolePhotoRoom.RoleCouple()->WeddingRoom() != NULL) {
        return ETryEnterMyPhotoRoomError_InWeddingRoom;
    }

    if (rRolePhotoRoom.RoleAmuseRoom()->AmuseRoom() != NULL) 
    {
        return ETryEnterMyPhotoRoomError_InAmuseRoom;
    }

    if (rRolePhotoRoom.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        return ETryEnterMyPhotoRoomError_InCeremonyRoom;
    }

    if (CRoomManager::Instance().GetLobbyPlayer(rRolePhotoRoom.RoleAttr()->GetRoleID()) != NULL) {
        return ETryEnterMyPhotoRoomError_Unkonwn;
    }

    if (pRoom->RoomState() == EPhotoRoomState_Destroy)
    {
        return ETryEnterMyPhotoRoomError_Unkonwn;
    }

    return ETryEnterMyPhotoRoomError_Success;		
}

EKickPhotoRoomPlayerError CPhotoRoomManager::_CanKickPhotoRoomPlayer(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nBeKickedID)
{
    if (rRolePhotoRoom.RoleAttr() == NULL) {
        return EKickPhotoRoomPlayerError_UnKnown;
    }

    if (rRolePhotoRoom.RoleAttr()->GetRoleID() == nBeKickedID) {
        return EKickPhotoRoomPlayerError_KickSelf;
    }

    CPhotoRoom *pPhotoRoom = rRolePhotoRoom.PhotoRoom();
    if (pPhotoRoom == NULL) {
        return EKickPhotoRoomPlayerError_NotInRoom;
    }

    if (pPhotoRoom->HostID() != rRolePhotoRoom.RoleAttr()->GetRoleID()) {
        return EKickPhotoRoomPlayerError_NotHost;
    }

    if (pPhotoRoom->GetPlayer(nBeKickedID) == NULL) {
        return EKickPhotoRoomPlayerError_BeKickedPlayerNotInRoom;
    }

    return EKickPhotoRoomPlayerError_Success;
}

EPhotoRoomPlayerChangeStateError CPhotoRoomManager::_CanChangePhotoRoleState(CEntityPhotoRoomComponent &rRolePhotoRoom, EPhotoRoomPlayerState ePlayerState)
{
    CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
    if (pRoom == NULL) {
        return EPhotoRoomPlayerChangeStateError_NotInPhotoRoom;
    }

    if (pRoom->RoomState() == EPhotoRoomState_Start) {
        return EPhotoRoomPlayerChangeStateError_RoomStart;
    }

    if (!(ePlayerState >= EPhotoRoomPlayerState_Free && ePlayerState <= EPhotoRoomPlayerState_Ready) || rRolePhotoRoom.PlayerState() == ePlayerState) {
        return EPhotoRoomPlayerChangeStateError_InvalidData;
    }

    return EPhotoRoomPlayerChangeStateError_Success;
}

EChangePhotoRoomStateError CPhotoRoomManager::_CanChangePhotoRoomState(CEntityPhotoRoomComponent &rRolePhotoRoom, EPhotoRoomState eRoomState)
{
    if (rRolePhotoRoom.RoleAttr() == NULL) {
        return EChangePhotoRoomStateError_InvalidData;
    }

    CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
    if (pRoom == NULL) {
        return EChangePhotoRoomStateError_NotInPhotoRoom;
    }

    if (pRoom->HostID() != rRolePhotoRoom.RoleAttr()->GetRoleID()) {
        return EChangePhotoRoomStateError_NotHost;
    }

    const CPhotoRoom::PhotoRoomPlayerList_t & playerList = pRoom->PhotoRoomPlayerMap();
    for (CPhotoRoom::PhotoRoomPlayerList_t::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr) {
        if (*itr != NULL) {
            if (*itr != &rRolePhotoRoom && (*itr)->PlayerState() != EPhotoRoomPlayerState_Ready) {
                return EChangePhotoRoomStateError_NotAllPlayerReady;
            }
        }
    }

    if (!(eRoomState > EPhotoRoomState_None && eRoomState < EPhotoRoomState_Max) ) {
        return EChangePhotoRoomStateError_RoomStateNotExist;
    }

    if (eRoomState == pRoom->RoomState()) {
        return EChangePhotoRoomStateError_NotChange;
    }

    return EChangePhotoRoomStateError_Success;
}

EChangePhotoRoomActionError CPhotoRoomManager::_CanChangePhtotoRoomAction(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nActionID)
{
    if (rRolePhotoRoom.RoleItem() == NULL || rRolePhotoRoom.RoleAttr() == NULL) {
        return EChangePhotoRoomActionError_Unkonwn;
    }

    CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
    if (pRoom == NULL) {
        return EChangePhotoRoomActionError_NotInPhotoRoom;
    }

    if (pRoom->HostID() != rRolePhotoRoom.RoleAttr()->GetRoleID()) {
        return EChangePhotoRoomActionError_NotHost;
    }

    if (pRoom->RoomState() != EPhotoRoomState_Start) {
        return EChangePhotoRoomActionError_RoomNotStarted;
    }

    const PhotoRoomGroupAction * pActionInfo = PhotoRoomConfigManager::Instance().GroupActionInfo(nActionID);
    if (pActionInfo == NULL) {
        return EChangePhotoRoomActionError_NotSuchAction;
    }

    if (!PhotoRoomConfigManager::Instance().ThemeHasGroupAction(nActionID)) {
        return EChangePhotoRoomActionError_RoomNotSupport;
    }

    if (!pRoom->IsInRoomActionList(nActionID)) {
        if (pActionInfo->m_nRequiredItemType > 0) {
            if (rRolePhotoRoom.RoleItem()->GetItemCount(pActionInfo->m_nRequiredItemType, false, false) < pActionInfo->m_nRequiredItemCount) {
                return EChangePhotoRoomActionError_NotEnoughItemCount;
            }
        }
    }

    return EChangePhotoRoomActionError_Success;
}

EChangePhotoRoomBGError CPhotoRoomManager::_CanChangePhotoRoomBG(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nBGID)
{
    if (rRolePhotoRoom.RoleItem() == NULL || rRolePhotoRoom.RoleAttr() == NULL) {
        return EChangePhotoRoomBGError_Unknown;
    }

    CPhotoRoom *pRoom = rRolePhotoRoom.PhotoRoom();
    if (pRoom == NULL) {
        return EChangePhotoRoomBGError_NotInPhotoRoom;
    }

    if (pRoom->HostID() != rRolePhotoRoom.RoleAttr()->GetRoleID()) {
        return EChangePhotoRoomBGError_NotHost;
    }

    //if (pRoom->RoomState() == EPhotoRoomState_Start) {
    //	return EChangePhotoRoomBGError_RoomStarted;
    //}

    const PhotoRoomConfig * pBGInfo = PhotoRoomConfigManager::Instance().PhotoRoomBG(nBGID);
    if (pBGInfo == NULL) {
        return EChangePhotoRoomBGError_NotSuchBG;
    }

    if (!PhotoRoomConfigManager::Instance().ThemeHasBG(nBGID)) {
        return EChangePhotoRoomBGError_RoomNotSupport;
    }

    if (!pRoom->IsInRoomActionList(nBGID)) {
        if (pBGInfo->m_nRequiredItemType > 0) {
            if (rRolePhotoRoom.RoleItem()->GetItemCount(pBGInfo->m_nRequiredItemType, false, false) < pBGInfo->m_nRequiredItemCount) {
                return EChangePhotoRoomBGError_NotEnoughtItemCount;
            }
        }
    }

    return EChangePhotoRoomBGError_Success;
}

ERenewPhotoRoomError CPhotoRoomManager::_CanRenewPhotoRoom(CEntityPhotoRoomComponent &rRolePhotoRoom)
{
    if (rRolePhotoRoom.RoleAttr() == NULL || rRolePhotoRoom.RoleItem() == NULL) {
        return ERenewPhotoRoomError_Unknown;
    }

    CPhotoRoom *pRoom = _FindHostRoom(rRolePhotoRoom.RoleAttr()->GetRoleID());	

    if (pRoom == NULL) {
        return ERenewPhotoRoomError_NoRoom;
    }

    CEntityPhotoRoomComponent *pRole = pRoom->GetPlayer(rRolePhotoRoom.RoleAttr()->GetRoleID());	
    if (pRole == NULL) {
        return ERenewPhotoRoomError_NotInRoom;
    }

    const PhotoRoomTheme *pTheme = PhotoRoomConfigManager::Instance().GetPhotoRoomTheme(pRoom->RoomTheme());

    if (pTheme == NULL) {
        return ERenewPhotoRoomError_Unknown;
    }

    if (!pTheme->CanRenew()) {
        return ERenewPhotoRoomError_RoomNotSupport;
    }

    if (pTheme->RenewItemType() > 0 && pTheme->RenewItemCount() > 0) 
    {
        if (rRolePhotoRoom.RoleItem()->GetItemCount(pTheme->RenewItemType(), false, false) < pTheme->RenewItemCount()) 
        {
            return ERenewPhotoRoomError_NotEnoughItem;
        }
    }


    return ERenewPhotoRoomError_Success;
}

unsigned int CPhotoRoomManager::_GeneratePhotoRoomID()
{
    unsigned int nRoomID = 0;
    if ( !m_listIdleID.empty() ) 
    {
        PhotoRoomIDList::iterator itr = m_listIdleID.begin();
        nRoomID = *itr;
        m_listIdleID.erase( itr );
    }
    else if ( !m_listSpawnedID.empty() ) 
    {
        PhotoRoomIDList::iterator it = m_listSpawnedID.begin();
        nRoomID = *it;
        m_listSpawnedID.erase( it );
    }

    return nRoomID;
}

void CPhotoRoomManager::_ReleasePhotoRoomID(unsigned int nRoomID)
{
    m_listIdleID.push_back(nRoomID);
}

void CPhotoRoomManager::_AddPhotoRoom(CPhotoRoom *pPhotoRoom)
{
    if (pPhotoRoom != NULL) {
        if (_FindPhotoRoom(pPhotoRoom->RoomID()) == NULL) 
        {
            if (pPhotoRoom->RoomTheme()) 
            {
                m_mapAlThemelRoom[EPhotoRoomTheme_None].insert(std::make_pair(pPhotoRoom->RoomID(), pPhotoRoom));
                m_mapAlThemelRoom[pPhotoRoom->RoomTheme()].insert(std::make_pair(pPhotoRoom->RoomID(), pPhotoRoom));
                _InTimeExpiredPhotoRoomContainer(pPhotoRoom);
                _InTimeExpiredNotificationContainer(pPhotoRoom);
            }
        }
    }
}

CPhotoRoom *CPhotoRoomManager::_RemovePhotoRoom(unsigned int nRoomID)
{
    if (nRoomID > 0) 
    {
        CPhotoRoom *pPhotoRoom = _FindPhotoRoom(nRoomID);
        if (pPhotoRoom != NULL) 
        {
            m_mapAlThemelRoom[EPhotoRoomTheme_None].erase(nRoomID);
            m_mapAlThemelRoom[pPhotoRoom->RoomTheme()].erase(nRoomID);
            _OutTimeExpiredPhotoRoomContainer(nRoomID);
            _OutTimeExpiredNotificationContainer(nRoomID);
            _ReleasePhotoRoomID(nRoomID);
            return pPhotoRoom;
        }
    }

    return NULL;
}


void CPhotoRoomManager::_InTimeExpiredPhotoRoomContainer(CPhotoRoom *pPhotoRoom)
{
    if (pPhotoRoom != NULL) {
        int nExpiredTime = pPhotoRoom->CreateTime() + pPhotoRoom->ExistTime();
        m_mapTimeExpiredRooms.insert(make_pair( nExpiredTime,pPhotoRoom));
    }
}

void CPhotoRoomManager::_OutTimeExpiredPhotoRoomContainer(unsigned int nRoomID)
{
    for (TimePhotoRoomMap_t::iterator itr = m_mapTimeExpiredRooms.begin(); itr != m_mapTimeExpiredRooms.end(); ++itr) {
        if (itr->second != NULL) {
            if (itr->second->RoomID() == nRoomID) {
                m_mapTimeExpiredRooms.erase(itr);
                return;
            }
        }
    }
}

void CPhotoRoomManager::_InTimeExpiredNotificationContainer(CPhotoRoom *pPhotoRoom)
{
    if (pPhotoRoom != NULL) {
        int nTimeExpired = 0;
        for (size_t i = 0; i < m_vecTimeExpiredNoficationMap.size(); ++i) {
            const CPhotoRoomNotifiationTimer *pTimer = CPhotoRoomLuaDataManager::Instance().GetPhotoRoomNotificationTimer(i + 1);
            if (pTimer != NULL) {
                if (pPhotoRoom->ExistTime() > (unsigned int)pTimer->m_nTimeBeforeExpired) {
                    int nTimerNotification = pPhotoRoom->CreateTime() + pPhotoRoom->ExistTime() - pTimer->m_nTimeBeforeExpired;
                    m_vecTimeExpiredNoficationMap[i].insert(make_pair(nTimerNotification, pPhotoRoom));
                } else {
                    WriteLog(LOGLEVEL_ERROR, "Photo Room Exist Time is less than expired notification time[%d]\n", nTimeExpired);
                }	
            }
        }
    }
}

void CPhotoRoomManager::_OutTimeExpiredNotificationContainer(unsigned int nRoomID)
{
    if (nRoomID > 0) {
        for (size_t i = 0; i < m_vecTimeExpiredNoficationMap.size(); ++i) {
            for (TimePhotoRoomMap_t::iterator itr = m_vecTimeExpiredNoficationMap[i].begin(); itr != m_vecTimeExpiredNoficationMap[i].end(); ++itr) {
                if (itr->second != NULL && itr->second->RoomID() == nRoomID) {
                    m_vecTimeExpiredNoficationMap[i].erase(itr);
                    break;
                }
            }
        }
    }	
}

CPhotoRoom * CPhotoRoomManager::_FindHostRoom( unsigned int nRoleID )
{
    for (PhotoRoomMap::iterator itr = m_mapAlThemelRoom[EPhotoRoomTheme_None].begin(); itr != m_mapAlThemelRoom[EPhotoRoomTheme_None].end(); ++itr) {
        if (itr->second != NULL) {
            if (itr->second->HostID() == nRoleID) {
                return itr->second;
            }
        }
    }

    return NULL;
}

CPhotoRoom * CPhotoRoomManager::_FindPhotoRoom(unsigned int nRoomID)
{
    if (nRoomID > 0) {
        PhotoRoomMap::iterator itr = m_mapAlThemelRoom[EPhotoRoomTheme_None].find(nRoomID);
        if (itr != m_mapAlThemelRoom[EPhotoRoomTheme_None].end() && itr->second != NULL) {
            return itr->second;
        }
    }

    return NULL;
}

bool CPhotoRoomManager::_PlayerInPhotoRoom(CEntityPhotoRoomComponent & rRolePhotoRoom, CPhotoRoom * pRoom)
{
    if (pRoom != NULL && rRolePhotoRoom.RoleAttr() != NULL) 
    {
        if (rRolePhotoRoom.PhotoRoom() == NULL) 
        {
            if (pRoom->AddPhotoRoomPlayer(&rRolePhotoRoom)) 
            {
                rRolePhotoRoom.InPhotoRoom(pRoom);
                GameMsg_S2C_EnterPhotoRoomSuc msgSuc;
                pRoom->EncodeRoomInfo(msgSuc.m_wholeRoomInfo, rRolePhotoRoom.RoleAttr()->GetRoleID() == pRoom->HostID());
                rRolePhotoRoom.SendPlayerMsg(&msgSuc);

                if (pRoom->PlayerList().size() > 1) 
                {
                    GameMsg_S2C_EnterPhotoRoomPlayer msgEnterPlayer;
                    rRolePhotoRoom.EncodePhotoRoomPlayerInfo(msgEnterPlayer.m_photoRoomPlayerInfo);
                    pRoom->SendMsgToAllBut(&msgEnterPlayer, &rRolePhotoRoom);
                }
            }

            // log
            rRolePhotoRoom.InsertActionLog(CRolePhotoRoomLog::ERolePhotoRoomAction_Enter);
            return true;
        }
    }

    return false;
}

CEntityPhotoRoomComponent * CPhotoRoomManager::_PlayerOutPhotoRoom(CEntityPhotoRoomComponent & rRolePhotoRoom)
{
    CPhotoRoom *pPhotoRoom = rRolePhotoRoom.PhotoRoom();
    if ( pPhotoRoom != NULL) {
        if (rRolePhotoRoom.RoleAttr() != NULL) {
            CEntityPhotoRoomComponent *pRole = pPhotoRoom->RemovePhotoRoomPlayer(rRolePhotoRoom.RoleAttr()->GetRoleID());
            if (pRole != NULL) {
                pRole->OutPhotoRoom();
            }
            if (pRole->RoleAttr()->GetRoleID() == pPhotoRoom->HostID()) {
                pPhotoRoom->SetRoomState(EPhotoRoomState_Free);
                GameMsg_S2C_PhotoRoomStateChangedNotification msgNotification;
                msgNotification.m_eRoomState = pPhotoRoom->RoomState();
                pPhotoRoom->SendMsgToAll(&msgNotification);
            }

            //明星房间特殊处理
            if (pPhotoRoom->RoomTheme() == EPhotoRoomTheme_Star && pPhotoRoom->PlayerCount() == 0)
            {
                pPhotoRoom->SetRoomState(EPhotoRoomState_Destroy);
                _OutTimeExpiredPhotoRoomContainer(pPhotoRoom->RoomID());
                m_mapTimeExpiredRooms.insert(make_pair( 0, pPhotoRoom ));
            }
            return pRole;
        }
    }

    return NULL;
}

void CPhotoRoomManager::_DestroyPhotoRoom(unsigned int nRoomID)
{
    CPhotoRoom *pRoom =  _RemovePhotoRoom(nRoomID);
    if (pRoom != NULL) {
        pRoom->DestroyRoom();
    }

    WriteLog(LOGLEVEL_DEBUG, "destory photoro roomid[%u], created time[%u]", pRoom->RoomID(), pRoom->CreateTime());

    delete pRoom, pRoom = NULL;
}

void CPhotoRoomManager::GetRoomListByGM(std::vector<GMRoomInfo>& roomList)
{
    PhotoRoomMap::iterator it = m_mapAlThemelRoom[EPhotoRoomTheme_None].begin();
    for ( ; it != m_mapAlThemelRoom[EPhotoRoomTheme_None].end() ; it++ )
    {
        GMRoomInfo roomInfo;
        it->second->ToGMRoomInfo(roomInfo);
        roomList.push_back(roomInfo);
    }
}


