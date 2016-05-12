#include <string>
#include "EntityCoupleComponent.h"
#include "../room/RoomComponent.h"
#include "WeddingRoomManager.h"
#include "GameMsg_C2S_Couple.h"
#include "GameMsg_S2C_Couple.h"
#include "GameMsg_S2G_Couple.h"
#include "GameMsg_G2S_Couple.h"
#include "../item/EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../../datastructure/Localization.h"
#include "../mail/MailMgr.h"
#include "../Location/LocationMgr.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../socket/GameMsg_Map.h"
#include "CoupleMsgDef.h"
#include "../../datastructure/datainfor/ServerConfig.h"
#include "../logic/EntityComponentEventID.h"
#include "../dancegroup/EntityDanceGroupComponent.h"

void SendMsg2GroupServer(GameMsg_Base *pMsg);

extern int g_nMinIntimacyMarriageRequired;

CWeddingRoomManager::CWeddingRoomManager()
:GameMsg_Processor(true, true)
{

}

CWeddingRoomManager::~CWeddingRoomManager()
{

}

CWeddingRoomManager & CWeddingRoomManager::Instance()
{
    static CWeddingRoomManager s_weddingRoomManager;

    return s_weddingRoomManager;
}

void CWeddingRoomManager::Init()
{
    RegisterMessage();

    for (int i = EWeddingRoomRange_Common_Begin; i < EWeddingRoomRange_Common_End; ++i) {
        m_listSpawnedID.push_back(i);
    }
}

void CWeddingRoomManager::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_BlessingCouple);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_AddRedEnvelopeBlessingValue);


    RegMsgProcessor(MSG_G2S_BlessingCouple, &CWeddingRoomManager::OnBlessingCouple);
    RegMsgProcessor(MSG_G2S_AddRedEnvelopeBlessingValue, &CWeddingRoomManager::OnAddRedEnvelpeBlessingValue);
}

void CWeddingRoomManager::OnUpdate(const unsigned long &lTimeElapsed)
{
    unsigned int nTimeNow = (unsigned int)time(NULL);
    for (TimeWeddingRoomMap_t::iterator itr = m_mapTimeStartNofication.begin(); itr != m_mapTimeStartNofication.end();) {
        if (itr->second != NULL) {
            if (itr->first <= nTimeNow) {
                CWeddingRoom *pRoom = itr->second;
                m_mapTimeStartNofication.erase(itr++);

                if (pRoom->Valid()) {
                    pRoom->OnRoomStartingNofiication();
                }
            }
            else {
                break;
            }
        }
    }

    for (TimeWeddingRoomMap_t::iterator itr = m_mapTimeExpiredRooms.begin(); itr != m_mapTimeExpiredRooms.end();)
    {
        if (itr->second != NULL)
        {
            if (itr->first <= nTimeNow)
            {
                CWeddingRoom *pRoom = itr->second;
                m_mapTimeExpiredRooms.erase(itr++);

                if (pRoom->Valid())
                {
                    pRoom->OnRoomAutoStart();
                }

                // 更新状态到group
                GameMsg_S2G_UpdateWeddingRecordState s2gMsg;
                s2gMsg.m_nDoType = 2;
                s2gMsg.m_nPairID = pRoom->PairID();
                s2gMsg.m_bIsRunning = false;
                ::SendMsg2GroupServer(&s2gMsg);

                // 销毁通知
                pRoom->OnRoomEndNofiication();

                // 发放红包
                pRoom->SendGiftToBlessinger();

                // 销毁房间
                DestroyRoom(pRoom->RoomID(), CRoleCoupleLog::ECoupleDestoryCase_Update);
            }
            else
            {
                break;
            }
        }
    }
}

EWeddingRoomType CWeddingRoomManager::RoomIDIndex(unsigned int nRoomID)
{
    if (nRoomID >= EWeddingRoomRange_Common_Begin && nRoomID <= EWeddingRoomRange_Common_End) {
        return EWeddingRoom_PRIMARY;
    }

    return EWeddingRoom_None;
}

void CWeddingRoomManager::OnCoupleSeparated(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    const GameMsg_C2S_CoupleSeparateRequest & msgSeparate = (const GameMsg_C2S_CoupleSeparateRequest &)rMsg;

    std::string strError = "";
    CWeddingRoom *pRoom = FindWeddingRoomByPairID(rRoleCouple.PairID());
    if (rRoleCouple.IsSingle())
    {
        strError = "ECoupleSeparateRequest_YouAreSingle";
    }
    else if (msgSeparate.m_nTargetID != rRoleCouple.m_coupleInfo.m_nCoupleID)
    {
        strError = "ECoupleSeparateRequest_NotYouCoupleYet";
    }
    else
    {
        if (rRoleCouple.RoleAttr() != NULL)
        {
            unsigned int nPairID = rRoleCouple.PairID();
            g_pQueryMgr->AddQuery(QUERY_Couple_DelCouple, rRoleCouple.PairID());
            rRoleCouple.ResetCoupleData();
            rRoleCouple.CheckCoupleQuest();

            CRoleEntity *pCouple = CPlayerManager::Instance().GetEntityByRoleID(msgSeparate.m_nTargetID);
            if (pCouple != NULL)
                pCouple->SendEvent(eComponentEvent_CoupleHasBeenSeparated, NULL);

            GameMsg_S2C_CoupleSeparateRequestSuc msgSuc;
            rRoleCouple.SendPlayerMsg(&msgSuc);

            GameMsg_S2G_CoupleSeprate msgNotifyGroup;
            msgNotifyGroup.m_nPairID = nPairID;
            msgNotifyGroup.m_nSeparateID = rRoleCouple.RoleAttr()->GetRoleID();
            msgNotifyGroup.m_nToBeSeparateID = msgSeparate.m_nTargetID;
            msgNotifyGroup.m_bNeedNotify = pCouple == NULL;
            ::SendMsg2GroupServer(&msgNotifyGroup);

            std::string strMailTitle = CLocalization::Instance().GetString("Couple_Separated_Mail_Title");
            std::string strMailBoby = CLocalization::Instance().GetString("Couple_Separated_Mail_Content");
            CMailMgr::Instance().SendSystemMailToPlayer( msgSeparate.m_nTargetID, EMailType_CoupleSeparate, strMailTitle, strMailBoby );
        }

        if (pRoom != NULL)
        {
            pRoom->SetValid(false);
            pRoom->OnStateChange(false);

            pRoom->SendGiftToBlessinger();
            RemoveAllPlayer(pRoom);
            DestroyRoom(pRoom->RoomID(), CRoleCoupleLog::ECoupleDestoryCase_OutCoupleSeparated);
        }
    }

    if (strError != "") {
        GameMsg_S2C_CoupleSeparateRequestFail msgFail;
        msgFail.m_strError = strError;
        rRoleCouple.SendPlayerMsg(&msgFail);
    }
    else
    {
        // log
        unsigned int nMaleID = 0;
        unsigned int nFemaleID = 0;
        if (rRoleCouple.RoleAttr()->GetSex() == ESexType_Male)
        {
            nMaleID = rRoleCouple.RoleAttr()->GetRoleID();
            nFemaleID = msgSeparate.m_nTargetID;
        }
        else
        {
            nFemaleID = rRoleCouple.RoleAttr()->GetRoleID();
            nMaleID = msgSeparate.m_nTargetID;
        }

        rRoleCouple.RoleCoupleLog(nMaleID, nFemaleID, CRoleCoupleLog::ERoleCoupleCause_OutLover, 0, 0);
    }
}

void CWeddingRoomManager::OnGetWeddingRoomList(const GameMsg_Base & rMsg, CEntityCoupleComponent & rCoupleComponent)
{
    const GameMsg_C2S_GetWeddingRoomList& rMsgRequest = (const GameMsg_C2S_GetWeddingRoomList&)rMsg;

    unsigned short nRoomCount = 0;
    unsigned short nRoomPage = 0;

    int nObtainCount = 0;
    const CWeddingRoom* obtainRooms[EWeddingRoomRange_Common_End] = { NULL };

    //检查是否开启WeddingRoom
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_WiddingRoom))
    {
        return;
    }

    if (rMsgRequest.m_nPageShowNum <= 0)
    {
        return;
    }

    for (WeddingRoomMap::iterator itr = m_AllRooms[EWeddingRoom_None].begin(); itr != m_AllRooms[EWeddingRoom_None].end(); ++itr) {
        CWeddingRoom * pRoom = itr->second;
        if (pRoom != NULL) {
            nRoomPage = nRoomCount / rMsgRequest.m_nPageShowNum;
            ++nRoomCount;
            obtainRooms[nObtainCount++] = pRoom;
        }
    }

    GameMsg_S2C_GetWeddingRoomListRes msgRespond;
    msgRespond.m_nTotalCount = (unsigned short)m_AllRooms[EWeddingRoom_None].size();

    int nCurCount = 0;
    int nPage = (nRoomPage < rMsgRequest.m_nCurPage ? nRoomPage : rMsgRequest.m_nCurPage);

    for (int i = rMsgRequest.m_nPageShowNum * nPage; i < nObtainCount && nCurCount < rMsgRequest.m_nPageShowNum; ++i)
    {
        ++nCurCount;
        CWeddingRoomBriefInfo roomInfo;
        obtainRooms[i]->SetRoomBriefInfo(roomInfo);

        msgRespond.m_listRoomInfo.push_back(roomInfo);
    }

    msgRespond.m_nCurPage = (unsigned short)nPage;
    msgRespond.m_nTotalCount = nRoomCount;
    rCoupleComponent.SendPlayerMsg(&msgRespond);
}

int CWeddingRoomManager::OnCreateWeddingRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    ECreateWeddingRoomRes eRes = ECreateWeddingRoom_Suc;
    std::string strError = "";
    int	nNeedItemCount = 0;

    const GameMsg_C2S_CreateWeddingRoom & rMsgCreate = (const GameMsg_C2S_CreateWeddingRoom&)rMsg;

    // 物品消耗
    itemtype_t nCostItemID = 0;
    unsigned short nCostItemCount = 0;
    ConfigManager::Instance().GetWeddingRoomConfigMgr().GetWeddingCostItemInfo((unsigned short)rMsgCreate.m_nWeddingType, nCostItemID, nCostItemCount);
    if (nCostItemID == 0 || nCostItemCount == 0)
    {
        eRes = CreateWeddingRoom_WeddingConfigErr;
        strError = "CreateWeddingRoom_WeddingConfigErr";
    }
    else if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_WiddingRoom))
    {
        eRes = CreateWeddingRoom_NotHasCouple;
        strError = "CreateWeddingRoom_NotHasCouple";
    }
    else if (rRoleCouple.PairID() == 0)
    {
        eRes = CreateWeddingRoom_NotHasCouple;
        strError = "CreateWeddingRoom_NotHasCouple";
    }
    else if (rRoleCouple.GetDivorceAgreement().GetAgreementState() == eDivorceState_Running)
    {
        eRes = CreateWeddingRoom_InDivorce;
        strError = "CreateWeddingRoom_InDivorce";
    }
    else if ((int)rRoleCouple.Intimacy() < g_nMinIntimacyMarriageRequired)
    {
        eRes = CreateWeddingRoom_LackOfIntimacy;
        strError = "CreateWeddingRoom_LackOfIntimacy";
    }
    else if (ExistsSameCoupleRoom(rRoleCouple.m_coupleInfo.m_nPairID))
    {
        eRes = CreateWeddingRoom_HasTheSameCoupleRoom;
        strError = "CreateWeddingRoom_HasTheSameCoupleRoom";
    }
    else if (rRoleCouple.RoleItem() == NULL || rRoleCouple.RoleItem()->GetItemCount(nCostItemID, false, false) < nCostItemCount)
    {
        eRes = CreateWeddingRoom_LackOfRequiredItem;
        strError = "CreateWeddingRoom_LackOfRequiredItem";
        nNeedItemCount = nCostItemCount - rRoleCouple.RoleItem()->GetItemCount(nCostItemID, false, false);
    }
    else if (rRoleCouple.WeddingRoom() != NULL)
    {
        eRes = CreateWeddingRoom_InRoom;
        strError = "CreateWeddingRoom_InRoom";
    }
    else if (rMsgCreate.m_strRoomName.empty())
    {
        eRes = CreateWeddingRoom_EmptyName;
        strError = "CreateWeddingRoom_EmptyName";
    }
    else if (rMsgCreate.m_strRoomName.length() > (size_t)g_ServerConfig.m_MaxWeddingRoomLimit)
    {
        eRes = CreateWeddingRoom_NameTooLong;
        strError = "CreateWeddingRoom_NameTooLong";
    }
    else if ((int)rMsgCreate.m_strPassword.length() > C_MAX_WEDDING_ROOM_PASSWD)
    {
        eRes = CreateWeddingRoom_PwdTooLong;
        strError = "CreateWeddingRoom_PwdTooLong";
    }
    else if (!(rMsgCreate.m_nWeddingType > EWeddingType_None && rMsgCreate.m_nWeddingType < EWeddingType_Max))
    {
        eRes = CreateWeddingRoom_IncorrectWeddingType;
        strError = "CreateWeddingRoom_IncorrectWeddingType";
    }
    else if (rRoleCouple.RoleStageRoom()->IsInRoom())
    {
        eRes = CreateWeddingRoom_InDanceRoom;
        strError = "CreateWeddingRoom_InDanceRoom";
    }
    else if (CLocationMgr::Instance().IsInZone(Zone_Lobby, rRoleCouple.RoleID()))
    {
        eRes = CreateWeddingRoom_InDanceLobby;
        strError = "CreateWeddingRoom_InDanceLobby";
    }
    else if (rRoleCouple.RolePhotoRoom()->PhotoRoom() != NULL)
    {
        eRes = CreateWeddingRoom_InPhotoRoom;
        strError = "CreateWeddingRoom_InPhotoRoom";
    }
    else if (rRoleCouple.RoleAmuseRoom()->AmuseRoom() != NULL)
    {
        eRes = CreateWeddingRoom_InAmuseRoom;
        strError = "CreateWeddingRoom_InAmuseRoom";
    }
    else if (rRoleCouple.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        eRes = CreateWeddingRoom_InCeremonyRoom;
        strError = "CreateWeddingRoom_InCeremonyRoom";
    }
    else
    {
        if (rRoleCouple.RoleItem() != NULL && rRoleCouple.RoleItem()->RemoveItemByType(nCostItemID, nCostItemCount, EItemAction_Del_CreateWeddingRoom))
        {
            unsigned int nRoomID = _GenerateWeddingRoomID();
            if (nRoomID == 0)
            {
                eRes = CreateWeddingRoom_RoomFull;
                strError = "CreateWeddingRoom_RoomFull";
            }
            else
            {
                CWeddingRoom* pRoom = new CWeddingRoom;
                if (pRoom != NULL && rRoleCouple.RoleAttr() != NULL)
                {
                    pRoom->m_eWeddingType = (EWeddingRoomType)rMsgCreate.m_nWeddingType;
                    pRoom->m_nRoomID = nRoomID;
                    pRoom->m_nPairID = rRoleCouple.PairID();
                    pRoom->m_nHostID = rRoleCouple.RoleAttr()->GetRoleID();
                    pRoom->m_strRoomName = rMsgCreate.m_strRoomName;
                    pRoom->m_strPassword = rMsgCreate.m_strPassword;

                    const CCoupleInfo & coupleinfo = rRoleCouple.CoupleInfo();
                    if (coupleinfo.m_nSmallRoleID > 0)
                    {
                        pRoom->m_nGroomID = rRoleCouple.CoupleSmallRoleID() == rRoleCouple.RoleAttr()->GetRoleID() ? rRoleCouple.CoupleID() : rRoleCouple.RoleAttr()->GetRoleID();
                        pRoom->m_nBrideID = rRoleCouple.CoupleSmallRoleID() == rRoleCouple.RoleAttr()->GetRoleID() ? rRoleCouple.RoleAttr()->GetRoleID() : rRoleCouple.CoupleID();
                        pRoom->m_strGroomName = rRoleCouple.CoupleSmallRoleID() == rRoleCouple.RoleAttr()->GetRoleID() ? rRoleCouple.CoupleName() : rRoleCouple.RoleAttr()->GetRoleName();
                        pRoom->m_strBrideName = rRoleCouple.CoupleSmallRoleID() == rRoleCouple.RoleAttr()->GetRoleID() ? rRoleCouple.RoleAttr()->GetRoleName() : rRoleCouple.CoupleName();
                    }
                    else
                    {
                        pRoom->m_nGroomID = rRoleCouple.RoleAttr()->GetSex() == ESexType_Male ? rRoleCouple.RoleAttr()->GetRoleID() : rRoleCouple.CoupleID();
                        pRoom->m_strGroomName = rRoleCouple.RoleAttr()->GetSex() == ESexType_Male ? rRoleCouple.RoleAttr()->GetRoleName() : rRoleCouple.CoupleName();
                        pRoom->m_nBrideID = rRoleCouple.RoleAttr()->GetSex() == ESexType_Female ? rRoleCouple.RoleAttr()->GetRoleID() : rRoleCouple.CoupleID();
                        pRoom->m_strBrideName = rRoleCouple.RoleAttr()->GetSex() == ESexType_Female ? rRoleCouple.RoleAttr()->GetRoleName() : rRoleCouple.CoupleName();
                    }

                    pRoom->m_nCreateTime = (unsigned int)time(NULL);
                    pRoom->m_bValid = true;

                    if (_PlayerInWeddingRoom(rRoleCouple, pRoom, true) < 0)
                    {
                        eRes = CreateWeddingRoom_Unknow;
                        strError = "CreateWeddingRoom_Unknow";

                        _ReleaseWeddingRoomID(pRoom->RoomID());
                        delete pRoom, pRoom = NULL;
                    }
                    else
                    {
                        _AddWeddingRoom(pRoom);
                    }
                }
                else
                {
                    eRes = CreateWeddingRoom_Unknow;
                    strError = "CreateWeddingRoom_Unknow";
                }
            }
        }
        else
        {
            eRes = CreateWeddingRoom_LackOfRequiredItem;
            strError = "CreateWeddingRoom_LackOfRequiredItem";
        }
    }

    if (eRes != ECreateWeddingRoom_Suc)
    {
        GameMsg_S2C_CreateWeddingRoomFail msgResponse;
        msgResponse.m_nError = (unsigned char)eRes;
        msgResponse.m_strError = strError;
        msgResponse.m_nNeedCount = nNeedItemCount;
        rRoleCouple.SendPlayerMsg(&msgResponse);
    }

    return eRes;
}


void CWeddingRoomManager::OnTryEnterWeddingRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleRoomCOM)
{
    const GameMsg_C2S_TryEnterWeddingRoom & rMsgRequest = (const GameMsg_C2S_TryEnterWeddingRoom&)rMsg;
    int nRes = EEnterWeddingRoom_Suc;
    std::string strError = "";

    //检查是否开启WeddingRoom
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_WiddingRoom))
    {
        return;
    }

    CWeddingRoom* pRoom = _FindWeddingRoom(rMsgRequest.m_nRoomID);
    if (pRoom == NULL) {
        nRes = EEnterWeddingRoom_RoomNotExists;
        strError = "EEnterWeddingRoom_RoomNotExists";
    }
    else {
        if (pRoom->HasPassword()) {
            nRes = EEnterWeddingRoom_HasPassword;
            strError = "EEnterWeddingRoom_HasPassword";
        }
        else {
            _EnterWeddingRoom(rRoleRoomCOM, pRoom, "", nRes, strError);
        }
    }

    if (nRes != EEnterWeddingRoom_Suc) {
        GameMsg_S2C_TryEnterWeddingRoomFail msgFail;
        msgFail.m_nRoomID = rMsgRequest.m_nRoomID;
        msgFail.m_nError = (unsigned char)nRes;
        msgFail.m_strError = strError;

        rRoleRoomCOM.SendPlayerMsg(&msgFail);
    }
}

void CWeddingRoomManager::OnEnterWeddingRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    const GameMsg_C2S_EnterWeddingRoom& rMsgRequest = (const GameMsg_C2S_EnterWeddingRoom &)rMsg;

    int nRes = 0;
    std::string strError;
    CWeddingRoom* pRoom = _FindWeddingRoom(rMsgRequest.m_nRoomID);
    if (!_EnterWeddingRoom(rRoleCouple, pRoom, rMsgRequest.m_strRoomPassword, nRes, strError)) {
        GameMsg_S2C_EnterWeddingRoomFail msgFail;
        msgFail.m_nError = (unsigned char)nRes;
        msgFail.m_strError = strError;
        rRoleCouple.SendPlayerMsg(&msgFail);
    }
}

void CWeddingRoomManager::OnEnterMyWeddingRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    //const GameMsg_C2S_EnterMyWeddingRoom & msgRequest = (GameMsg_C2S_EnterMyWeddingRoom &)rMsg;

    //检查是否开启WeddingRoom
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_WiddingRoom))
    {
        return;
    }

    int nRes = EEnterMyWeddingRoom_Suc;
    std::string strError;
    if (rRoleCouple.PairID() == 0)
    {	// 没有情侣
        nRes = EEnterMyWeddingRoom_NotHasCouple;
        strError = "EEnterMyWeddingRoom_NotHasCouple";
    }
    // 	if (rRoleCouple.IsMarried()) 
    // 	{	// 已婚
    // 		nRes = EEnterMyWeddingRoom_HasMarried;
    // 		strError = "EEnterMyWeddingRoom_HasMarried";
    // 	}
    // 	else 
    if (rRoleCouple.WeddingRoom() != NULL)
    {	// 已经在房间
        nRes = EEnterMyWeddingRoom_AlreadyInRoom;
        strError = "EEnterMyWeddingRoom_AlreadyInRoom";
    }
    else
    {
        CWeddingRoom *pRoom = FindWeddingRoomByPairID(rRoleCouple.PairID());
        if (pRoom == NULL) {
            nRes = EEnterMyWeddingRoom_RoomNotExistsOrNotOnCurLine;
            strError = "EEnterMyWeddingRoom_RoomNotExistsOrNotOnCurLine";
        }
        else if (rRoleCouple.RoleAttr() != NULL && (rRoleCouple.RoleAttr()->GetRoleID() == pRoom->GroomID() || rRoleCouple.RoleAttr()->GetRoleID() == pRoom->BrideID())) {
            _EnterWeddingRoom(rRoleCouple, pRoom, pRoom->Password(), nRes, strError);
        }
    }

    if (nRes != EEnterMyWeddingRoom_Suc) {
        GameMsg_S2C_EnterMyWeddingRoomFail msgFail;
        msgFail.m_nError = (unsigned char)nRes;
        msgFail.m_strError = strError;
        rRoleCouple.SendPlayerMsg(&msgFail);
    }
}

void CWeddingRoomManager::OnStartWedding(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    //const GameMsg_C2S_RequestMarry & msgRequestMarry = (GameMsg_C2S_RequestMarry &)rMsg;
    int nRes = ERequestMarry_Suc;
    std::string strError = "";

    //检查是否开启WeddingRoom
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_WiddingRoom))
    {
        return;
    }

    if (!rRoleCouple.StartWedding(nRes, strError))
    {
        GameMsg_S2C_RequestMarryFail msgFail;
        msgFail.m_nError = (unsigned char)nRes;
        msgFail.m_strError = strError;
        rRoleCouple.SendPlayerMsg(&msgFail);
    }

    if (nRes == ERequestMarry_Suc && rRoleCouple.WeddingRoom() != NULL)
    {
        // 销毁房间
        DestroyRoom(rRoleCouple.WeddingRoom()->RoomID(), CRoleCoupleLog::ECoupleDestoryCase_Marriage);
    }
}

void CWeddingRoomManager::OnExitRoom(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    EExitWeddingRoomRes eRes = EExitWeddingRoom_Suc;
    std::string strError;

    CWeddingRoom* pRoom = rRoleCouple.WeddingRoom();
    if (pRoom == NULL)
    {
        eRes = EExitWeddingRoom_NotInRoom;
        strError = "EExitWeddingRoom_NotInRoom";
    }
    else
    {
        GameMsg_C2S_ExitWeddingRoom & exitmsg = (GameMsg_C2S_ExitWeddingRoom&)rMsg;
        _PlayerOutWeddingRoom(rRoleCouple, (EWeddingRoomQuitTo)exitmsg.m_nQuitTo);
    }

    if (eRes != EExitWeddingRoom_Suc) {
        GameMsg_S2C_ExitWeddingRoomFail msgFail;
        msgFail.m_nError = (unsigned char)eRes;
        msgFail.m_strError = strError;
        rRoleCouple.SendPlayerMsg(&msgFail);
    }
}

void CWeddingRoomManager::OnKickWeddingRoomPlayer(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    const GameMsg_C2S_KickWeddingRoomPlayer &msgRequest = (GameMsg_C2S_KickWeddingRoomPlayer &)rMsg;
    ERequestKickWeddingRoomPlayer	eRes = ERequestKickWeddingRoomPlayer_Suc;
    std::string strError;

    if (rRoleCouple.WeddingRoom() == NULL) {
        eRes = ERequestKickWeddingRoomPlayer_NotInRoom;
        strError = "ERequestKickWeddingRoomPlayer_NotInRoom";
    }
    else if (!rRoleCouple.IsHost()) {
        eRes = ERequestKickWeddingRoomPlayer_NotHost;
        strError = "ERequestKickWeddingRoomPlayer_NotHost";
    }
    else if (rRoleCouple.RoleAttr() == NULL || msgRequest.m_nTargetID == rRoleCouple.RoleAttr()->GetRoleID()) {
        eRes = ERequestKickWeddingRoomPlayer_CanNotKickSelf;
        strError = "ERequestKickWeddingRoomPlayer_CanNotKickSelf";
    }
    else {
        CWeddingRoom *pRoom = _FindWeddingRoom(rRoleCouple.WeddingRoom()->RoomID());
        if (pRoom == NULL) {
            eRes = ERequestKickWeddingRoomPlayer_RoomNotExists;
            strError = "ERequestKickWeddingRoomPlayer_RoomNotExists";
        }
        else {
            CEntityCoupleComponent *pTarget = pRoom->RemoveWeddingPlayer(msgRequest.m_nTargetID);
            if (pTarget == NULL) {
                eRes = ERequestKickWeddingRoomPlayer_TargetNotInRoom;
                strError = "ERequestKickWeddingRoomPlayer_TargetNotInRoom";
            }
            else {
                pTarget->OutWeddingRoom();

                GameMsg_S2C_KickWeddingPlayerSuc msgSuc;
                msgSuc.m_nTargetID = msgRequest.m_nTargetID;
                rRoleCouple.SendPlayerMsg(&msgSuc);

                GameMsg_S2C_PlayerExitedRoom msgPlayerExit;
                msgPlayerExit.m_nExitID = msgRequest.m_nTargetID;
                pRoom->SendMsgToAllBut(&msgPlayerExit, &rRoleCouple);

                GameMsg_S2C_ExitWeddingRoomSuc msgExitSuc;
                msgExitSuc.m_nQuitTo = (char)EWeddingRoomQuitTo_Wait;
                pTarget->SendPlayerMsg(&msgExitSuc);
            }
        }
    }

    if (eRes != ERequestKickWeddingRoomPlayer_Suc) {
        GameMsg_S2C_KickWeddingPlayerFail msgFail;
        msgFail.m_nError = (unsigned char)eRes;
        msgFail.m_strError = strError;
        rRoleCouple.SendPlayerMsg(&msgFail);
    }
}

void CWeddingRoomManager::OnSetWeddingPlayerType(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    GameMsg_C2S_SetWeddingPlayerType &msgSetting = (GameMsg_C2S_SetWeddingPlayerType &)rMsg;
    ESetWeddingPlayerType eRes = ESetWeddingPlayerType_Suc;
    std::string strError;
    if (!(msgSetting.m_nRoomRoleType >= EWeddingRoomRoleType_BestMan && msgSetting.m_nRoomRoleType <= EWeddingRoomRoleType_Guest)) {
        eRes = ESetWeddingPlayerType_InvalidType;
        strError = "ESetWeddingPlayerType_InvalidType";
    }
    else if (rRoleCouple.WeddingRoom() == NULL) {
        eRes = ESetWeddingPlayerType_NotInRoomOrWeddingBegan;
        strError = "ESetWeddingPlayerType_NotInRoomOrWeddingBegan";
    }
    else if ((msgSetting.m_nRoomRoleType == EWeddingRoomRoleType_BestMan && rRoleCouple.RoomRoleType() != EWeddingRoomRoleType_Groom)
        || ((msgSetting.m_nRoomRoleType == EWeddingRoomRoleType_Bridesmaid && rRoleCouple.RoomRoleType() != EWeddingRoomRoleType_Bride)
        || (msgSetting.m_nRoomRoleType == EWeddingRoomRoleType_Administer && !rRoleCouple.IsHost()))
        ) {
        eRes = ESetWeddingPlayerType_NoAuthority;
        strError = "ESetWeddingPlayerType_NoAuthority";
    }
    else {
        CWeddingRoom *pRoom = _FindWeddingRoom(rRoleCouple.WeddingRoom()->RoomID());
        if (pRoom == NULL) {
            eRes = ESetWeddingPlayerType_RoomNotExists;
            strError = "ESetWeddingPlayerType_RoomNotExists";
        }
        else if (pRoom->Valid()) {
            if (msgSetting.m_nTargetID == pRoom->GroomID()) {
                eRes = ESetWeddingPlayerType_CanNotSetGroomType;
                strError = "ESetWeddingPlayerType_CanNotSetGroomType";
            }
            else if (msgSetting.m_nTargetID == pRoom->BrideID()) {
                eRes = ESetWeddingPlayerType_CanNotSetBrideType;
                strError = "ESetWeddingPlayerType_CanNotSetBrideType";
            }
            else {
                CEntityCoupleComponent *pTarget = pRoom->GetWeddingPlayer(msgSetting.m_nTargetID);
                if (pTarget == NULL) {
                    eRes = ESetWeddingPlayerType_TargetNotInRoom;
                    strError = "ESetWeddingPlayerType_TargetNotInRoom";
                }
                else if (pTarget->RoleAttr() == NULL || (msgSetting.m_nRoomRoleType == EWeddingRoomRoleType_BestMan && pTarget->RoleAttr()->GetSex() == ESexType_Female)) {
                    eRes = ESetWeddingPlayerType_BestmanSexError;
                    strError = "ESetWeddingPlayerType_BestmanSexError";
                }
                else if (msgSetting.m_nRoomRoleType == EWeddingRoomRoleType_Bridesmaid && pTarget->RoleAttr()->GetSex() == ESexType_Male) {
                    eRes = ESetWeddingPlayerType_BridesmaildSexError;
                    strError = "ESetWeddingPlayerType_BridesmaildSexError";
                }
                else  {
                    CEntityCoupleComponent *pLastRole = pRoom->GetWeddingPlayer((EWeddingRoomRoleType)msgSetting.m_nRoomRoleType);
                    if (pLastRole != NULL) {
                        pLastRole->SetRoomRoleType(EWeddingRoomRoleType_Guest);
                        GameMsg_S2C_NotifyChangeWeddingPlayerType msgNotify;
                        msgNotify.m_nTargetID = pLastRole->m_pRoleAttr != NULL ? pLastRole->m_pRoleAttr->GetRoleID() : 0;
                        msgNotify.m_nRoomRoleType = EWeddingRoomRoleType_Guest;
                        pRoom->SendMsgToAll(&msgNotify);
                    }
                    pTarget->SetRoomRoleType((EWeddingRoomRoleType)msgSetting.m_nRoomRoleType);
                    GameMsg_S2C_SetWeddingPlayerTypeSuc msgSuc;
                    msgSuc.m_nTargetID = msgSetting.m_nTargetID;
                    msgSuc.m_RoomRoleType = msgSetting.m_nRoomRoleType;
                    rRoleCouple.SendPlayerMsg(&msgSuc);

                    GameMsg_S2C_NotifyChangeWeddingPlayerType msgNotify;
                    msgNotify.m_nTargetID = msgSetting.m_nTargetID;
                    msgNotify.m_nRoomRoleType = msgSetting.m_nRoomRoleType;
                    pRoom->SendMsgToAllBut(&msgNotify, &rRoleCouple);
                }
            }
        }
    }

    if (eRes != ESetWeddingPlayerType_Suc)
    {
        GameMsg_S2C_SetWeddingPlayerTypeFail msgFail;
        msgFail.m_strError = strError;
        rRoleCouple.SendPlayerMsg(&msgFail);
    }
}

void CWeddingRoomManager::OnChangeWeddingRoomPwd(const GameMsg_Base & rMsg, CEntityCoupleComponent & rRoleCouple)
{
    const GameMsg_C2S_ChangeWeddingRoomPwd & msgRequest = (GameMsg_C2S_ChangeWeddingRoomPwd &)rMsg;
    EChangeWeddingRoomPwdRes eRes = EChangeWeddingRoomPwd_Suc;
    std::string strError;
    if ((int)msgRequest.m_strNewPwd.size() > C_MAX_WEDDING_ROOM_PASSWD) {
        eRes = EChangeWeddingRoomPwd_PwdTooLong;
        strError = "EChangeWeddingRoomPwd_PwdTooLong";
    }
    else if (rRoleCouple.WeddingRoom() == NULL) {
        eRes = EChangeWeddingRoomPwd_NotInRoom;
        strError = "EChangeWeddingRoomPwd_NotInRoom";
    }
    else {
        CWeddingRoom *pRoom = _FindWeddingRoom(rRoleCouple.WeddingRoom()->RoomID());
        if (pRoom == NULL) {
            eRes = EChangeWeddingRoomPwd_RoomNotExists;
            strError = "EChangeWeddingRoomPwd_RoomNotExists";
        }
        else {
            if (!rRoleCouple.IsHost()) {
                eRes = EChangeWeddingRoomPwd_NoAutority;
                strError = "EChangeWeddingRoomPwd_NoAutority";
            }
            else {
                pRoom->SetPassword(msgRequest.m_strNewPwd);
                GameMsg_S2C_ChangeWeddingRoomPwdSuc msgSuc;
                msgSuc.m_strNewPwd = msgRequest.m_strNewPwd;
                rRoleCouple.SendPlayerMsg(&msgSuc);
            }
        }
    }

    if (eRes != EChangeWeddingRoomPwd_Suc) {
        GameMsg_S2C_ChangeWeddingRoomPwdFail msgFail;
        msgFail.m_strError = strError;
        rRoleCouple.SendPlayerMsg(&msgFail);
    }
}

void CWeddingRoomManager::OnBlessingCouple(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_G2S_BlessingCouple & g2sMsg = (GameMsg_G2S_BlessingCouple &)msg;

    CWeddingRoom * pRoom = CWeddingRoomManager::Instance().FindWeddingRoomByPairID(g2sMsg.m_nPairID);
    if (pRoom != NULL && pRoom->Valid())
    {
        pRoom->BlessingCouple(g2sMsg.m_nRoleID, g2sMsg.m_nSex);
    }
    else
    {
        CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(g2sMsg.m_nRoleID);
        if (pRoleEntity != NULL)
        {
            GameMsg_S2C_BlessingError errormsg;
            errormsg.m_strError = "ECoupleMsgError_BlessingPairNot";
            pRoleEntity->SendPlayerMsg(&errormsg);
        }
    }
}

void CWeddingRoomManager::OnAddRedEnvelpeBlessingValue(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_AddRedEnvelopeBlessingValue & g2sMsg = (GameMsg_G2S_AddRedEnvelopeBlessingValue &)msg;

    CWeddingRoom * pRoom = CWeddingRoomManager::Instance().FindWeddingRoomByPairID(g2sMsg.m_nPairID);
    if (pRoom != NULL && pRoom->Valid())
    {
        pRoom->RedEnvelope(g2sMsg.m_nRoleID, g2sMsg.m_strRoleName, g2sMsg.m_nRedEnvelopeCount);
    }
    else
    {
        CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(g2sMsg.m_nRoleID);
        if (pRoleEntity != NULL)
        {
            GameMsg_S2C_BlessingError errormsg;
            errormsg.m_strError = "ECoupleMsgError_BlessingPairNot";
            pRoleEntity->SendPlayerMsg(&errormsg);
        }
    }
}

CWeddingRoom * CWeddingRoomManager::RemoveRoom(unsigned int nRoomID)
{
    CWeddingRoom *pRoom = NULL;
    WeddingRoomMap::iterator itr = m_AllRooms[EWeddingRoom_None].find(nRoomID);
    if (itr != m_AllRooms[EWeddingRoom_None].end() && itr->second != NULL) {
        pRoom = itr->second;
        m_AllRooms[EWeddingRoom_None].erase(itr);
        _OutTimeExpiredRoomMap(pRoom);
        _OutTimeStartNoficationRoomMap(pRoom);
        if (pRoom != NULL && pRoom->RoomType() > EWeddingRoom_None && pRoom->RoomType() < EWeddingRoom_Max) {
            m_AllRooms[pRoom->RoomType()].erase(nRoomID);
            _ReleaseWeddingRoomID(nRoomID);
        }
    }

    return pRoom;
}

void CWeddingRoomManager::DestroyRoom(unsigned int nRoomID, CRoleCoupleLog::ECoupleDestoryCase eAction)
{
    CWeddingRoom *pRoom = RemoveRoom(nRoomID);//不用发送玩家退出房间消息，房间关闭客户端自己做处理
    if (pRoom != NULL)
    {
        unsigned int nBrideRoleID = pRoom->BrideID();
        unsigned int nGroomRoleID = pRoom->GroomID();
        for (WeddingRoomPlayerMap_t::const_iterator itr = pRoom->WeddingRoomPlayerMap().begin(); itr != pRoom->WeddingRoomPlayerMap().end(); ++itr)
        {
            if (itr->second != NULL)
            {
                itr->second->OutWeddingRoom();
            }
        }
        delete pRoom, pRoom = NULL;

        // 销毁日志
        CRoleCoupleLog * pLog = new CRoleCoupleLog();
        pLog->m_nMaleRoleID = nGroomRoleID;
        pLog->m_nFemaleRoleID = nBrideRoleID;
        pLog->m_eCoupleCause = CRoleCoupleLog::ERoleCoupleCause_DestoryWeddingRoom;
        pLog->m_nIntimacy = (int)eAction;
        g_pQueryMgr->AddQuery(QUERY_LogRoleCouple, 0, pLog);
    }
}

void CWeddingRoomManager::PlayerLogout(CEntityCoupleComponent *pRoleCouple)
{
    if (pRoleCouple != NULL)
    {
        _PlayerOutWeddingRoom(*pRoleCouple);
    }
}


unsigned int CWeddingRoomManager::_GenerateWeddingRoomID()
{
    unsigned int nRoomID = 0;
    if (!m_listIdleID.empty()) {
        WeddingRoomIDList::iterator it = m_listIdleID.begin();
        nRoomID = *it;
        m_listIdleID.erase(it);
    }
    else if (!m_listSpawnedID.empty()) {
        WeddingRoomIDList::iterator it = m_listSpawnedID.begin();
        nRoomID = *it;
        m_listSpawnedID.erase(it);
    }

    return nRoomID;
}

void CWeddingRoomManager::_ReleaseWeddingRoomID(unsigned int nRoomID)
{
    if (RoomIDIndex(nRoomID) == EWeddingRoom_PRIMARY)
    {
        m_listIdleID.push_back(nRoomID);
    }
}

bool CWeddingRoomManager::ExistsSameCoupleRoom(unsigned int nPairID)
{
    for (WeddingRoomMap::iterator itr = m_AllRooms[EWeddingRoom_None].begin(); itr != m_AllRooms[EWeddingRoom_None].end(); ++itr) {
        if (itr->second != NULL) {
            return itr->second->PairID() == nPairID;
        }
    }

    return false;
}

bool CWeddingRoomManager::_AddWeddingRoom(CWeddingRoom *pRoom)
{
    if (pRoom != NULL) {
        if (_FindWeddingRoom(pRoom->RoomID()) == NULL) {
            m_AllRooms[EWeddingRoom_None].insert(make_pair(pRoom->RoomID(), pRoom));
            _InTimeExpiredRoomMap(pRoom);
            _InTimeStartNoficationRoomMap(pRoom);
            if (pRoom->RoomType() > EWeddingRoom_None && RoomIDIndex(pRoom->RoomID() < EWeddingRoom_Max)) {
                m_AllRooms[pRoom->RoomType()].insert(make_pair(pRoom->RoomID(), pRoom));
            }
            else {
                WriteLog(LOGLEVEL_ERROR, "WeddingRoomManager Error:_AddWeddingRoom:wedding room type error,type : [%d]", pRoom->RoomType());
            }
            return true;
        }
        else {
            WriteLog(LOGLEVEL_ERROR, "WeddingRoomManager Error:_AddWeddingRoom: Wedding room ID[%d] is used", pRoom->RoomID());
        }
    }
    else {
        WriteLog(LOGLEVEL_ERROR, "WeddingRoomManager Error:_AddWeddingRoom: pRoom is NULL");
    }

    return false;
}

void CWeddingRoomManager::_InTimeExpiredRoomMap(CWeddingRoom *pRoom)
{
    if (pRoom != NULL) {
        unsigned int nExpiredTime = (unsigned int)pRoom->ExpiredTime();
        m_mapTimeExpiredRooms.insert(std::make_pair(nExpiredTime, pRoom));
    }
}

void CWeddingRoomManager::_OutTimeExpiredRoomMap(CWeddingRoom *pRoom)
{
    if (pRoom != NULL) {
        for (TimeWeddingRoomMap_t::iterator itr = m_mapTimeExpiredRooms.begin(); itr != m_mapTimeExpiredRooms.end(); ++itr) {
            if (itr->second != NULL && itr->second->RoomID() == pRoom->RoomID()) {
                m_mapTimeExpiredRooms.erase(itr);
                break;
            }
        }
    }
}

void CWeddingRoomManager::_InTimeStartNoficationRoomMap(CWeddingRoom *pRoom)
{
    if (pRoom != NULL) {
        unsigned int nNotificationTime = (unsigned int)pRoom->StartingNotificatioiTime();
        m_mapTimeStartNofication.insert(std::make_pair(nNotificationTime, pRoom));
    }
}

void CWeddingRoomManager::_OutTimeStartNoficationRoomMap(CWeddingRoom *pRoom)
{
    if (pRoom != NULL) {
        for (TimeWeddingRoomMap_t::iterator itr = m_mapTimeStartNofication.begin(); itr != m_mapTimeStartNofication.end(); ++itr) {
            if (itr->second != NULL && itr->second->RoomID() == pRoom->RoomID()) {
                m_mapTimeStartNofication.erase(itr);
                break;
            }
        }
    }
}

CWeddingRoom *CWeddingRoomManager::_FindWeddingRoom(unsigned int nRoomID)
{
    WeddingRoomMap::iterator itr = m_AllRooms[EWeddingRoom_None].find(nRoomID);
    if (itr != m_AllRooms[EWeddingRoom_None].end() && itr->second != NULL) {
        return itr->second;
    }

    return NULL;
}

CWeddingRoom * CWeddingRoomManager::FindWeddingRoomByPairID(unsigned int nPairID)
{
    for (WeddingRoomMap::iterator itr = m_AllRooms[EWeddingRoom_None].begin(); itr != m_AllRooms[EWeddingRoom_None].end(); ++itr) {
        if (itr->second != NULL && itr->second->PairID() == nPairID) {
            return itr->second;
        }
    }

    return NULL;
}


int CWeddingRoomManager::_PlayerInWeddingRoom(CEntityCoupleComponent &rCoupleComponent, CWeddingRoom *pRoom, bool bCreate)
{
    int nRes = -1;
    if (pRoom != NULL) {
        if (rCoupleComponent.WeddingRoom() != NULL) {
            WriteLog(LOGLEVEL_WARNING, "Player in wedding room error, already in a wedding room");
            return nRes;
        }

        nRes = pRoom->AddWeddingPlayer(&rCoupleComponent);
        if (nRes != -1) {
            rCoupleComponent.InWeddingRoom(pRoom, rCoupleComponent.RoleAttr()->GetRoleID() == pRoom->HostID());
            // notice self
            GameMsg_S2C_EnterWeddingRoomSuc msgSelfResponse;
            pRoom->SetWholeInfo(msgSelfResponse.m_roomWholeInfo, rCoupleComponent.RoleAttr()->GetRoleID() == pRoom->HostID());
            rCoupleComponent.SendPlayerMsg(&msgSelfResponse);

            // notice others, bCreate is true only when player is creating room, there is no others in that case
            if (!bCreate) {
                GameMsg_S2C_EnterWeddingPlayer msgOtherResponse;
                rCoupleComponent.SetRoomPlayerInfo(msgOtherResponse.m_roomPlayer);
                pRoom->SendMsgToAllBut(&msgOtherResponse, &rCoupleComponent);
            }
        }
    }

    return nRes;
}

void CWeddingRoomManager::_PlayerOutWeddingRoom(CEntityCoupleComponent &rRoleCouple, EWeddingRoomQuitTo eQuitTo)
{
    CWeddingRoom *pWeddingRoom = rRoleCouple.WeddingRoom();
    if (pWeddingRoom != NULL && rRoleCouple.RoleAttr() != NULL)
    {
        // exit room
        pWeddingRoom->RemoveWeddingPlayer(rRoleCouple.RoleAttr()->GetRoleID());
        // exit component
        rRoleCouple.OutWeddingRoom();

        GameMsg_S2C_ExitWeddingRoomSuc msgExitSuc;
        msgExitSuc.m_nQuitTo = (char)eQuitTo;
        rRoleCouple.SendPlayerMsg(&msgExitSuc);

        GameMsg_S2C_PlayerExitedRoom msgNotify;
        msgNotify.m_nExitID = rRoleCouple.RoleAttr()->GetRoleID();
        pWeddingRoom->SendMsgToAllBut(&msgNotify, &rRoleCouple);
    }
}

void CWeddingRoomManager::RemoveAllPlayer(CWeddingRoom *  pWeddingRoom)
{
    if (pWeddingRoom == NULL)
    {
        return;
    }

    const WeddingRoomPlayerMap_t & players = pWeddingRoom->WeddingRoomPlayerMap();
    const WeddingRoomPlayerMap_t tempplayers = players;
    WeddingRoomPlayerMap_t::const_iterator it = tempplayers.begin();
    for (; it != tempplayers.end(); ++it)
    {
        CEntityCoupleComponent * pRoleCouple = it->second;
        if (pRoleCouple != NULL)
        {
            _PlayerOutWeddingRoom(*pRoleCouple);
        }
    }
}

bool CWeddingRoomManager::_EnterWeddingRoom(CEntityCoupleComponent &rRoleCouple, CWeddingRoom *pRoom, const string & strPassword, int & nError, std::string & strError)
{
    nError = EEnterWeddingRoom_Suc;

    if (rRoleCouple.WeddingRoom() != NULL) {
        nError = EEnterWeddingRoom_InRoom;
        strError = "EEnterWeddingRoom_InRoom";
    }
    else if (pRoom == NULL) {
        nError = EEnterWeddingRoom_RoomNotExists;
        strError = "EEnterWeddingRoom_RoomNotExists";
    }
    else if (pRoom->HasPassword() && pRoom->Password().compare(strPassword) != 0) {
        nError = EEnterWeddingRoom_IncorrectPassword;
        strError = "EEnterWeddingRoom_IncorrectPassword";
    }
    else if (rRoleCouple.RoleStageRoom()->IsInRoom()) {
        nError = EEnterWeddingRoom_InDanceRoom;
        strError = "EEnterWeddingRoom_InDanceRoom";
    }
    else if (CLocationMgr::Instance().IsInZone(Zone_Lobby, rRoleCouple.RoleID())) {
        nError = EEnterWeddingRoom_InDanceLobby;
        strError = "EEnterWeddingRoom_InDanceLobby";
    }
    else if (rRoleCouple.RolePhotoRoom()->PhotoRoom() != NULL) {
        nError = EEnterWeddingRoom_InPhotoRoom;
        strError = "EEnterWeddingRoom_InPhotoRoom";
    }
    else if (rRoleCouple.RoleAmuseRoom()->AmuseRoom() != NULL) {
        nError = EEnterWeddingRoom_InAmuseRoom;
        strError = "EEnterWeddingRoom_InAmuseRoom";
    }
    else if (rRoleCouple.RoleDanceGroup()->GetCeremonyRoom() != NULL)
    {
        nError = EEnterWeddingRoom_InCeremonyRoom;
        strError = "EEnterWeddingRoom_InCeremonyRoom";
    }
    else {
        if (_PlayerInWeddingRoom(rRoleCouple, pRoom, false) < 0) {
            nError = EEnterWeddingRoom_RoomFull;
            strError = "EEnterWeddingRoom_RoomFull";
        }
    }

    return nError == EEnterWeddingRoom_Suc;
}

