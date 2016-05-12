#include "WFriendManager.h"
#include "../../socket/SlotPeer.h"
#include "../../socket/GameMsg_Map.h"
#include "../group/GroupServerCirculator.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/FriendDataManager.h"

#include "GameMsg_S2G_AddFriend.h"
#include "GameMsg_S2G_RemoveFriend.h"
#include "GameMsg_S2G_AddBlackList.h"
#include "GameMsg_S2G_RemoveBlackList.h"
#include "GameMsg_S2G_GetFriendOnlineInfo.h"
#include "GameMsg_S2G_FriendApplication.h"
#include "GameMsg_S2G_NotifyGameRemoveFollowedFriend.h"
#include "GameMsg_S2G_FriendLogin.h"
#include "GameMsg_S2G_AddMuteList.h"
#include "GameMsg_S2G_RemoveMuteList.h"
#include "GameMsg_S2G_AddMuteFriend.h"
#include "GameMsg_S2G_AddBlackFriend.h"
#include "GameMsg_S2G_NotifyGameRemoveFriend.h"
#include "GameMsg_S2G_NotifyHasBeenAccepted.h"
#include "GameMsg_S2G_BecomeFriends.h"
#include "GameMsg_S2G_NotifyHasBeenDeletedFriend.h"
#include "GameMsg_S2G_NotifyGameHasBeenDeletedFriend.h"
#include "GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist.h"
#include "GameMsg_S2G_GetBlackListOnlineInfo.h"
#include "GameMsg_S2G_GetTempListOnlineInfo.h"
#include "GameMsg_S2G_RequestFriendOnlineInfo.h"
#include "GameMsg_S2G_PushOnlineInfoToAllFriends.h"
#include "GameMsg_S2G_UpdateAttributeToAllFriend.h"
#include "GameMsg_S2G_FindPlayerFromID.h"

#include "GameMsg_G2S_GetFriendOnlineInfoResult.h"
#include "GameMsg_G2S_FriendApplicationBack2Sender.h"
#include "GameMsg_G2S_FriendApplicationForward2Receiver.h"
#include "GameMsg_G2S_NotifyGameRemoveFriend.h"
#include "GameMsg_G2S_NotifyGameRemoveFollowedFriend.h"
#include "GameMsg_G2S_NotifyGameRemoveFriend.h"
#include "GameMsg_G2S_FriendLogin.h"
#include "GameMsg_G2S_NotifyHasBeenAccepted.h"
#include "GameMsg_G2S_BecomeFriends.h"
#include "GameMsg_G2S_NotifyGameHasBeenDeletedFriend.h"
#include "GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist.h"
#include "GameMsg_G2S_PushOnlineInfoToAllFriends.h"
#include "GameMsg_G2S_GetBlackListOnlineInfo.h"
#include "GameMsg_G2S_GetTempListOnlineInfo.h"
#include "GameMsg_G2S_UpdateAttributeToAllFriend.h"
#include "GameMsg_G2S_FindPlayerFromID.h"

#include "../libServerFrame/QueryManager.h"

extern CQueryManager * g_pQueryMgr;
CWFriendManager::CWFriendManager():
	GameMsg_Processor(true, true)
{
}

CWFriendManager::~CWFriendManager()
{
}

void CWFriendManager::RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AddBlackListFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_RemoveFriend)
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AddBlackList);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_RemoveBlackList);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_FriendApplication);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_NotifyGameRemoveFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_NotifyGameRemoveFollowedFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_FriendLogin);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetFriendOnlineInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_NotifyHasBeenAccpted);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_BecomeFriends);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_NotifyHasBeenDeletedFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_NotifyGameHasBeenDeletedFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetBlackListOnlineInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetTempListOnlineInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_RequestFriendOnlineInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_PushOnlineInfoToAllFriends);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateAttributeToAllFriends);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_FindPlayer);
}

void CWFriendManager::RegisterMessageProcessor()
{
	RegMsgProcessor(MSG_S2G_AddBlackListFriend, &CWFriendManager::OnAddBlackFriend);
	RegMsgProcessor(MSG_S2G_RemoveFriend, &CWFriendManager::OnRemoveFriend);
	RegMsgProcessor(MSG_S2G_AddBlackList, &CWFriendManager::OnAddBlackList);
	RegMsgProcessor(MSG_S2G_RemoveBlackList, &CWFriendManager::OnRemoveBlackList);
	RegMsgProcessor(MSG_S2G_FriendApplication, &CWFriendManager::OnFriendApplication);
	RegMsgProcessor(MSG_S2G_FriendLogin, &CWFriendManager::OnFriendLogin);
	RegMsgProcessor(MSG_S2G_GetFriendOnlineInfo, &CWFriendManager::OnGetAllFriendOnlineInfo);
	RegMsgProcessor(MSG_S2G_NotityHasBeenAccepted, &CWFriendManager::OnNotifyHasBeenAccepted);
	RegMsgProcessor(MSG_S2G_NotifyHasBeenDeletedFriend, &CWFriendManager::OnNotifyHasBeenDeletedFriend);
	RegMsgProcessor(MSG_S2G_GetBlackListOnlineInfo, &CWFriendManager::OnGetBlackListOnlineInfo);
	RegMsgProcessor(MSG_S2G_GetTempListOnlineInfo, &CWFriendManager::OnGetTempListOnlineInfo);
	RegMsgProcessor(MSG_S2G_RequestFriendOnlineInfo, &CWFriendManager::OnGetFriendOnlineInfo);
	RegMsgProcessor(MSG_S2G_PushOnlineInfoToAllFriends, &CWFriendManager::OnPushOnlineInfoToAllFriends);
	RegMsgProcessor(MSG_S2G_UpdateAttrbuteToAllFriends, &CWFriendManager::OnUpdateAttributeToAllFriends);
	RegMsgProcessor(MSG_S2G_FindPlayer, &CWFriendManager::OnFindPlayer)

    RegQueryProcessor(QUERY_LoadFriendOfflineTime, &CWFriendManager::OnLoadFriendOfflineTimeBack);
}

void CWFriendManager::OnFindPlayer(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_FindPlayer * pMsg = (GameMsg_S2G_FindPlayer *)&msg;
	CGroupRoleEntity *pMeEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(pMsg->nAccount);
	CFindFriendCondition *pCondition = CFriendDataManager::Instance().GetTypeCondition(pMsg->m_nFindFriendTypeIndex);

	if (pMeEntity != NULL && pCondition != NULL)
	{
		GameMsg_G2S_FindPlayerResult gMsg;
		if (pCondition->m_nConditionMin == eFindFriendType_RoleID)
		{	
			FindPlayerByRoleID(gMsg.m_PlayerList, pMsg->m_nFindRoleID);
		}
		else if (pCondition->m_nConditionMin == eFindFriendType_RoleName)
		{
			pMsg->m_nFindRoleID = pMeEntity->GetRoleID();//用来过滤自己
			FindPlayerByRoleName(gMsg, *pMsg);
		}
		else
		{
			gMsg.m_strError = "FindPlayer_InvalidParam";
		}
		pMeEntity->SendPlayerMsg(&gMsg);
	}
}

void CWFriendManager::OnPlayerLogin(CGroupRoleEntity & role)
{
	SendOnlineInfoToAllFriends(role);
	OnNotifyFriendLogin(role);

    std::map<unsigned int,unsigned int*> *pMapFriend = new std::map<unsigned int,unsigned int*>;
    for (std::list<unsigned int>::iterator itr = role.m_listFriend.begin(); itr != role.m_listFriend.end(); ++itr) 
    {
        if (CGroupOffLinePlayerManager::Instance().GetOfflineTime(*itr) == 0)
        {
            unsigned int * pOfflineTime = CGroupOffLinePlayerManager::Instance().GetOfflineTimePoint(*itr);
            pMapFriend->insert(std::make_pair(*itr, pOfflineTime));
        }
    }

    g_pQueryMgr->AddQuery(QUERY_LoadFriendOfflineTime, role.GetRoleID(), pMapFriend);
}

void CWFriendManager::OnPlayerLogout(CGroupRoleEntity & role)
{
	GameMsg_G2S_PushOnlineInfoToAllFriends updateMsg;		
	updateMsg.m_roleOnlineInfo.m_nRoleID = role.GetRoleID();

	for (list<unsigned int>::iterator itr = role.m_listFriend.begin(); itr != role.m_listFriend.end(); ++itr) 
    {
		CGroupRoleEntity*pFriend =  CGroupPlayerManager::Instance().SearchRoleByID(*itr);
		if (pFriend != NULL) 
        {
			pFriend->SendPlayerMsg(&updateMsg);
		}
	}
}

void CWFriendManager::OnAddBlackFriend(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_AddBlackListFriend *pMsg = (GameMsg_S2G_AddBlackListFriend *)&msg;
	if (pMsg != NULL) 
    {
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL) 
        {
			pRole->RemoveBlackList(pMsg->m_FriendInfo.m_nRoleID);
			pRole->AddFriend(pMsg->m_FriendInfo.m_nRoleID);
		}
	}
}

void CWFriendManager::OnGetBlackListOnlineInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_GetBlackListOnlineInfo *pMsg = (GameMsg_S2G_GetBlackListOnlineInfo *)&msg;
	if (pMsg != NULL) 
    {
		CGroupRoleEntity *pPlayer = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pPlayer != NULL) 
        {
			GameMsg_G2S_GetBlackListOnlineInfo backMsg;
			for (list<unsigned int>::iterator itr = pPlayer->m_listBlackList.begin(); itr != pPlayer->m_listBlackList.end(); ++itr) 
            {
				CGroupRoleEntity *pBlack = CGroupPlayerManager::Instance().SearchRoleByID(*itr);
				CFriendOnlineInfo info;	
				info.m_nRoleID = *itr;
				if (pBlack != NULL) 
                {
					info.m_nSex = pBlack->GetSex();
					info.m_nLine = pBlack->GetLine();
					info.m_strLineName = pBlack->GetLineName();
				}
				backMsg.m_listBlackOnlineInfo.push_back(info);
			}
			pPlayer->SendPlayerMsg(&backMsg);
		}
	}
}

void CWFriendManager::OnGetTempListOnlineInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_GetTempListOnlineInfo *pMsg = (GameMsg_S2G_GetTempListOnlineInfo *)&msg;
	if (pMsg != NULL) 
    {
		GameMsg_G2S_GetTempListOnlineInfo backMsg;
		backMsg.m_nListType = pMsg->m_nListType;
		for (list<unsigned int>::iterator itr = pMsg->m_listMate.begin(); itr != pMsg->m_listMate.end(); ++itr) 
        {
			CFriendOnlineInfo info;
			info.m_nRoleID = *itr;
			CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByID(*itr);
			if (pEntity != NULL) 
            {
				SetPlayerOnlineInfo(*pEntity, info);
			}
			backMsg.m_listMate.push_back(info);
		}

		CGroupRoleEntity *pSender = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRequestRoleID);
		if (pSender != NULL) 
        {
			pSender->SendPlayerMsg(&backMsg);
		}
	}
}

void CWFriendManager::OnRemoveFriend(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_RemoveFriend *pMsg = (GameMsg_S2G_RemoveFriend *)&msg;
	if (pMsg != NULL)
	{
		GameMsg_S2G_RemoveFriend *pRemoveMsg = (GameMsg_S2G_RemoveFriend *)pMsg;	
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pRemoveMsg->m_nRoleID);
		if (pRole != NULL)
		{
			pRole->RemoveFriend(pRemoveMsg->m_nTargetID);
		}

		CGroupRoleEntity *pRemoveRole = CGroupPlayerManager::Instance().SearchRoleByID(pRemoveMsg->m_nTargetID);
		if (pRemoveRole != NULL) 
        {
			pRemoveRole->RemoveFriend(pRemoveMsg->m_nRoleID);
			GameMsg_G2S_NotifyGameHasBeenDeletedFriend notifyMsg;
			notifyMsg.m_nRoleID = pRemoveMsg->m_nRoleID;
			pRemoveRole->SendPlayerMsg(&notifyMsg);
		}
	}
}

void CWFriendManager::OnAddBlackList(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_AddBlackList*pMsg = (GameMsg_S2G_AddBlackList *)&msg;	
	CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
	if (pRole != NULL)
	{
		pRole->RemoveFriend(pMsg->m_nBlackID);
		pRole->AddBlackList(pMsg->m_nBlackID);
	}
}

void CWFriendManager::OnRemoveBlackList(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_RemoveBlackList *pMsg =  (GameMsg_S2G_RemoveBlackList*)&msg;
	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nSrcRoleID);
		if (pRole != NULL)
		{
			pRole->RemoveBlackList(pMsg->m_nTargetID);
		}
		CGroupRoleEntity *pTargetRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nTargetID);
		if (pTargetRole != NULL) {
			pTargetRole->RemoveFriend(pMsg->m_nSrcRoleID);
			GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist forwardMsg(pMsg->m_nSrcRoleID);
			pTargetRole->SendPlayerMsg(&forwardMsg);
		}
	}
}

void CWFriendManager::OnGetFriendOnlineInfo(GameMsg_Base  &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_RequestFriendOnlineInfo *pMsg = (GameMsg_S2G_RequestFriendOnlineInfo *)&msg;
	if (pMsg != NULL) 
    {
		GameMsg_G2S_PushOnlineInfoToAllFriends noticeMsg;
		noticeMsg.m_roleOnlineInfo.m_nRoleID = pMsg->m_nDstRoleID;

		CGroupRoleEntity *pDstRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nDstRoleID);
		if ( pDstRole != NULL ) 
        {
			SetPlayerOnlineInfo(*pDstRole, noticeMsg.m_roleOnlineInfo);
		}

		CGroupRoleEntity *pSrcRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nSrcRoleID);
		if (pSrcRole != NULL) {
			pSrcRole->SendPlayerMsg(&noticeMsg);
		}
	}
}


void CWFriendManager::OnGetAllFriendOnlineInfo(GameMsg_Base  &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_GetFriendOnlineInfo *pMsg = (GameMsg_S2G_GetFriendOnlineInfo *)&msg;
	CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);

	if (pMsg != NULL && pRole != NULL)
	{
		GameMsg_G2S_GetFriendOnlineInfoResult replyMsg;

		list<unsigned int> listQueryFriend = pRole->m_listFriend;
		for (list<unsigned int>::const_iterator itr = listQueryFriend.begin(); itr != listQueryFriend.end(); itr++) 
        {
			CGroupRoleEntity *pFriend = CGroupPlayerManager::Instance().SearchRoleByID(*itr);
			CFriendOnlineInfo	info;
			info.m_nRoleID = *itr;
			if (pFriend != NULL) 
            {
				SetPlayerOnlineInfo(*pFriend, info);
			}
			replyMsg.m_listFriendOnlineInfo.push_back(info);
		}

		pRole->SendPlayerMsg(&replyMsg);
	}
}

void CWFriendManager::OnPushOnlineInfoToAllFriends(GameMsg_Base  &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_PushOnlineInfoToAllFriends *pMsg = (GameMsg_S2G_PushOnlineInfoToAllFriends *)&msg;

	if (pMsg != NULL) {
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL) {
			GameMsg_G2S_PushOnlineInfoToAllFriends noticeMsg;
			SetPlayerOnlineInfo(*pRole, noticeMsg.m_roleOnlineInfo);
			for (list<unsigned int>::iterator itr = pRole->m_listFriend.begin(); itr != pRole->m_listFriend.end(); ++itr) {
				CGroupRoleEntity *pFriend = CGroupPlayerManager::Instance().SearchRoleByID(*itr);
				if (pFriend != NULL) {
					pFriend->SendPlayerMsg(&noticeMsg);
				}
			}
		}
	}
}

void CWFriendManager::OnUpdateAttributeToAllFriends(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_UpdateAttributeToAllFriends *pMsg = (GameMsg_S2G_UpdateAttributeToAllFriends *)&msg;

	if (pMsg != NULL) 
    {
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL) 
        {
			GameMsg_G2S_UpdateAttributeToAllFriends msgUpdate;
			msgUpdate.m_nRoleID = pMsg->m_nRoleID;
			msgUpdate.m_strName = pMsg->m_strName;
			msgUpdate.m_nLevel = pMsg->m_nLevel;
			msgUpdate.m_bIsVIP = pMsg->m_bIsVIP;
			msgUpdate.m_nVIPLevel = pMsg->m_nVIPLevel;
			for (list<unsigned int>::iterator itr = pRole->m_listFriend.begin(); itr != pRole->m_listFriend.end(); ++itr) 
            {
				CGroupRoleEntity *pFriend = CGroupPlayerManager::Instance().SearchRoleByID(*itr);
				if (pFriend != NULL) 
                {
					pFriend->SendPlayerMsg(&msgUpdate);
				}
			}
		}
	}
}

void CWFriendManager::OnFriendApplication(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_FriendApplication *pMsg = (GameMsg_S2G_FriendApplication *)&msg;
	if (pMsg != NULL)
	{
		CGroupRoleEntity *pSendRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nSenderID);
		CGroupRoleEntity *pRecvRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nReceiverID);
		unsigned char nResult = PLAYER_OFFLINE;
		if (pRecvRole != NULL && pRecvRole->IsInBlackList(pMsg->m_nSenderID)) 
        {
			nResult = PLAYER_PUTINBLACKLIST;// 在黑名单后者屏蔽名单中就不发过去了
		}
        else if ( pSendRole != NULL && pRecvRole != NULL) 
        {
			if ( pSendRole->GetLine() != pRecvRole->GetLine() )
			{
				nResult = PLAYER_NOTSAMELINE;
			}
			else
			{
				GameMsg_G2S_FriendApplicationForward2Receiver forwardMsg;
				forwardMsg.m_nSenderID = pMsg->m_nSenderID;
				memcpy(forwardMsg.m_szRoleName, pSendRole->GetName(), MAX_NAME_LEN - 1);
				forwardMsg.m_nSex = pSendRole->GetSex();
				forwardMsg.m_nLevel = pSendRole->GetLevel();
				forwardMsg.m_nCallBy = pMsg->m_nCallBy;
				forwardMsg.m_nCallTo = pMsg->m_nCallTo;
				forwardMsg.m_bIsVIP = pSendRole->IsVIP();
				forwardMsg.m_nVIPLevel = pSendRole->VIPLevel();
				forwardMsg.m_bInsertMsgBox = pMsg->m_bInsertMsgBox;

				pRecvRole->SendPlayerMsg(&forwardMsg);
				nResult = APPLICATION_SENT;
			}
		}

		if (pSendRole != NULL) 
        {
			GameMsg_G2S_FriendApplicationBack2Sender backMsg;
			backMsg.m_nResult = nResult;
			if (pRecvRole != NULL) 
            {
				backMsg.m_strFriendName = pRecvRole->GetName();
			}

			pSendRole->SendPlayerMsg(&backMsg);	
		}
	}
}

void CWFriendManager::OnFriendLogin(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_FriendLogin *pMsg = (GameMsg_S2G_FriendLogin *)&msg;	

	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nLoginID);
		if (pRole != NULL){
			for (list<unsigned int>::iterator itr = pRole->m_listFriend.begin(); itr != pRole->m_listFriend.end(); ++itr){
				CGroupRoleEntity *pNotifyPlayer = CGroupPlayerManager::Instance().SearchRoleByID(*itr);
				if (pNotifyPlayer != NULL)
				{
					GameMsg_G2S_FriendLogin loginMsg;
					SetPlayerOnlineInfo(*pRole, loginMsg.m_FriendOnlineInfo);
					pNotifyPlayer->SendPlayerMsg(&loginMsg);
				}
			}
		}
	}
}

void CWFriendManager::OnNotifyHasBeenAccepted(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_NotifyHasBeenAccpted *pMsg = (GameMsg_S2G_NotifyHasBeenAccpted *)&msg;
	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL)
		{
			pRole->RemoveBlackList(pMsg->m_friendInfo.m_nRoleID);
			pRole->AddFriend(pMsg->m_friendInfo.m_nRoleID);
			GameMsg_G2S_NotifyHasBeenAccpted notifyMsg;
			notifyMsg.m_friendInfo = pMsg->m_friendInfo;
			pRole->SendPlayerMsg(&notifyMsg);
		}

		CGroupRoleEntity *pFriend = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_friendInfo.m_nRoleID);
		if (pFriend != NULL) {
			pFriend->AddFriend(pMsg->m_nRoleID);
			GameMsg_G2S_PushOnlineInfoToAllFriends noticeMsg;
			SetPlayerOnlineInfo(*pFriend, noticeMsg.m_roleOnlineInfo);
			if (pRole != NULL) {
				pRole->SendPlayerMsg(&noticeMsg);
			}
		}
	}
}

void CWFriendManager::OnNotifyHasBeenDeletedFriend(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2G_NotifyHasBeenDeletedFriend *pMsg = (GameMsg_S2G_NotifyHasBeenDeletedFriend *)&msg;
	if (pMsg != NULL) {
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRemovedID);
		if (pRole != NULL) {
			pRole->RemoveFriend(pMsg->m_nRoleID);	//	不管是否还是好友都走这一步
			if (pRole->HasBlackList(pMsg->m_nRoleID)) {
				pRole->RemoveBlackList(pMsg->m_nRoleID);
			}
		}
	}
}

void CWFriendManager::OnNotifyFriendLogin(CGroupRoleEntity &role)
{
    for (list<unsigned int>::iterator itr = role.m_listFriend.begin(); itr != role.m_listFriend.end(); ++itr) 
    {
        CGroupRoleEntity*pFriend =  CGroupPlayerManager::Instance().SearchRoleByID(*itr);
        if (pFriend != NULL)
        {
            GameMsg_G2S_FriendLogin loginMsg;
            SetPlayerOnlineInfo(role, loginMsg.m_FriendOnlineInfo);
            pFriend->SendPlayerMsg(&loginMsg);
        }
    }
}

void CWFriendManager::SendOnlineInfoToAllFriends(CGroupRoleEntity &role)
{
	GameMsg_G2S_PushOnlineInfoToAllFriends updateMsg;

	SetPlayerOnlineInfo(role, updateMsg.m_roleOnlineInfo);

	for (list<unsigned int>::iterator itr = role.m_listFriend.begin(); itr != role.m_listFriend.end(); ++itr) 
    {
		CGroupRoleEntity*pFriend =  CGroupPlayerManager::Instance().SearchRoleByID(*itr);
		if (pFriend != NULL)
        {
			pFriend->SendPlayerMsg(&updateMsg);
		}
	}
}

void CWFriendManager::SetPlayerOnlineInfo(CGroupRoleEntity &role, CFriendOnlineInfo & info)
{
	info.m_nRoleID = role.GetRoleID();
	info.m_strName.assign( role.GetName() );
	info.m_nLine = role.GetLine();
	info.m_strLineName = role.GetLineName();
	info.m_nRoomID = role.GetSyncRoomID();
	info.m_nLevel = role.GetLevel();
	info.m_nSex = role.GetSex();
	info.m_bIsVIP = role.IsVIP();
	info.m_nVIPLevel = role.VIPLevel();

    if (role.GetSyncRoomID() <= 0)
    {
		info.m_nState = FriendRoomState_Free;
	}
    else if (role.m_nRoleRoomType == RoleRoomType_Audience) 
    {
		info.m_nState = FriendRoomState_Watching;
	}
    else if (role.GetRoomState() > RoleRoomState_None && role.GetRoomState() < RoleRoomState_ToStart)
    {
		info.m_nState = FriendRoomState_Waiting;
	}
    else 
    {
		info.m_nState = FriendRoomState_Dancing;
	}
}

void CWFriendManager::SetFindPlayerInfo(CGroupRoleEntity &role, CFindPlayerInfo & info)
{
	info.m_nRoleID = role.GetRoleID();
	info.m_strRoleName = role.GetName();
	info.m_nLevel = role.GetLevel();
	info.m_nSex = role.GetSex();
	info.m_nConstellation = role.GetRoleConstellation();
	info.m_bIsVip = role.IsVIP();
	info.m_nVipLevel = role.VIPLevel();
}

void CWFriendManager::FindPlayerByRoleID(std::list<CFindPlayerInfo> & playerList, unsigned int nRoleID)
{
	CGroupRoleEntity *pEntity = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
	if (pEntity != NULL) 
	{
		CFindPlayerInfo info;
		SetFindPlayerInfo(*pEntity, info);
		playerList.push_back(info);
	}
}

void CWFriendManager::FindPlayerByRoleName(GameMsg_G2S_FindPlayerResult &res, GameMsg_S2G_FindPlayer &msg)
{
	if (msg.m_strFindRoleName.empty())
	{
		res.m_strError = "FindPlayer_InvalidParam";
		return;
	}

	std::string m_strFindRoleName = msg.m_strFindRoleName;
	int nFindLevelIndex = (int)msg.m_nFindLevelIndex;
	int nFindSexIndex = (int)msg.m_nFindSexIndex;
	int nFindAgeIndex = (int)msg.m_nFindAgeIndex;
	int nFindConstellationIndex = (int)msg.m_nFindConstellationIndex;

	CFindFriendCondition *pLevelCondition = CFriendDataManager::Instance().GetLevelCondition(nFindLevelIndex);
	CFindFriendCondition *pSexCondition = CFriendDataManager::Instance().GetSexCondition(nFindSexIndex);
	CFindFriendCondition *pAgeCondition = CFriendDataManager::Instance().GetAgeCondition(nFindAgeIndex);
	CFindFriendCondition *pConstellationCondition = CFriendDataManager::Instance().GetConstellationCondition(nFindConstellationIndex);
	if (pLevelCondition == NULL || pSexCondition == NULL || pAgeCondition == NULL || pConstellationCondition == NULL)
	{
		res.m_strError = "FindPlayer_InvalidParam";
		return;
	}

	int nCount = 0;
	std::multimap<unsigned int, CGroupRoleEntity> matchPlayerMap;

	int nMaxPlayerCount = CFriendDataManager::Instance().m_nFindFriendMaxCount;
	std::map<unsigned int, CGroupRoleEntity>::iterator it = g_mapRoleEntity.begin();
	while (it != g_mapRoleEntity.end() && nCount < nMaxPlayerCount)
	{
		CGroupRoleEntity &role = it->second;
		if (strstr(role.GetName(), m_strFindRoleName.c_str()) == NULL)
		{
			++it;
			continue;
		}

		if (pLevelCondition != NULL && pLevelCondition->m_nConditionMin >= 0)
		{
			if (role.GetLevel() < pLevelCondition->m_nConditionMin
				|| ( role.GetLevel() > pLevelCondition->m_nConditionMax
				&& pLevelCondition->m_nConditionMax >= 0))
			{
				++it;
				continue;
			}
		}

		if (pSexCondition != NULL && pSexCondition->m_nConditionMin > 0)
		{
			if (role.GetSex() != pSexCondition->m_nConditionMin)
			{
				++it;
				continue;
			}
		}

		if (pAgeCondition != NULL && pAgeCondition->m_nConditionMin >= 0)
		{
			if (role.GetRoleAge() < pAgeCondition->m_nConditionMin
				|| ( role.GetRoleAge() > pAgeCondition->m_nConditionMax
				&& pAgeCondition->m_nConditionMax >= 0))
			{
				++it;
				continue;
			}
		}

		if (pConstellationCondition != NULL && pConstellationCondition->m_nConditionMin >= 0)
		{
			if (role.GetRoleConstellation() != pConstellationCondition->m_nConditionMin)
			{
				++it;
				continue;
			}
		}

		if (role.GetRoleID() == msg.m_nFindRoleID) // 过滤自己
		{
			++it;
			continue;
		}

		matchPlayerMap.insert(std::make_pair(it->second.m_nLoginTimeOnGroup, it->second));
		nCount++;
		++it;
	}
	
	if (nCount > CFriendDataManager::Instance().m_nFindFriendMaxCount)
	{
		res.m_strError = "FindPlayer_TooManyPlayer";
		return;
	}

	std::multimap<unsigned int, CGroupRoleEntity>::reverse_iterator itMatch = matchPlayerMap.rbegin(); 
	for (; itMatch != matchPlayerMap.rend(); ++itMatch)
	{
		CFindPlayerInfo info;
		SetFindPlayerInfo(itMatch->second, info);

		res.m_PlayerList.push_back(info);
	}
}

void CWFriendManager::OnLoadFriendOfflineTimeBack(QueryBase & rQuery)
{
    // 下发好友信息
    unsigned int nRoleID = (unsigned int)rQuery.m_nAttachment;
    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    if (pRole != NULL)
    {
        GameMsg_G2S_GetFriendOnlineInfoResult replyMsg;

        std::list<unsigned int> listQueryFriend = pRole->m_listFriend;
        for (list<unsigned int>::const_iterator itr = listQueryFriend.begin(); itr != listQueryFriend.end(); itr++) 
        {
            CGroupRoleEntity * pFriend = CGroupPlayerManager::Instance().SearchRoleByID(*itr);
            CFriendOnlineInfo info;
            info.m_nRoleID = *itr;
            if (pFriend != NULL) 
            {
                SetPlayerOnlineInfo(*pFriend, info);
            }
            else
            {
                info.m_nOfflineTime = CGroupOffLinePlayerManager::Instance().GetOfflineTime(*itr);
            }
            replyMsg.m_listFriendOnlineInfo.push_back(info);
        }

        pRole->SendPlayerMsg(&replyMsg);
    }

    delete (std::map<unsigned int,unsigned int*>*)rQuery.m_pAttachment;
    rQuery.m_pAttachment = NULL;
}
