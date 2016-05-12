#include "EntityFriendComponent.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "../logic/EntityComponentEventID.h"
#include "../logic/PlayerManager.h"
#include "../login/GameMsg_Player_CreateRoleRes.h"
#include "FriendDefine.h"
#include "../lua/InitLua.h"

#include "GameMsg_C2S_Friend.h"
#include "GameMsg_S2C_Friend.h"

//#include "GameMsg_S2G_AddFriend.h"
#include "GameMsg_S2G_RemoveFriend.h"
#include "GameMsg_S2G_AddBlackList.h"
#include "GameMsg_S2G_RemoveBlackList.h"
#include "GameMsg_S2G_GetFriendOnlineInfo.h"
#include "GameMsg_S2G_AddMuteList.h"
#include "GameMsg_S2G_AddMuteFriend.h"
#include "GameMsg_S2G_AddBlackFriend.h"
#include "GameMsg_S2G_RemoveMuteList.h"
#include "GameMsg_S2G_FriendApplication.h"
#include "GameMsg_S2G_NotifyGameRemoveFollowedFriend.h"
#include "GameMsg_S2G_FriendLogin.h"
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
#include "GameMsg_G2S_FriendApplicationForward2Receiver.h"
#include "GameMsg_G2S_FriendApplicationBack2Sender.h"
#include "GameMsg_G2S_NotifyGameRemoveFollowedFriend.h"
#include "GameMsg_G2S_FriendLogin.h"
#include "GameMsg_G2S_NotifyGameRemoveFriend.h"
#include "GameMsg_G2S_NotifyHasBeenAccepted.h"
#include "GameMsg_G2S_BecomeFriends.h"
#include "GameMsg_G2S_NotifyGameHasBeenDeletedFriend.h"
#include "GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist.h"
#include "GameMsg_G2S_GetBlackListOnlineInfo.h"
#include "GameMsg_G2S_GetTempListOnlineInfo.h"
#include "GameMsg_G2S_PushOnlineInfoToAllFriends.h"
#include "GameMsg_G2S_UpdateAttributeToAllFriend.h"
#include "GameMsg_G2S_FindPlayerFromID.h"

#include "../logic/EntityComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../room/RoomComponent.h"
#include "../room/RoomManager.h"
#include "../mail/EntityMailComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../vip/VIPDataMgr.h"
#include "RelationInfo.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/FriendDataManager.h"


const unsigned int BLACK_INTERVAL = 3;
const unsigned int PLAYMATE_INTERVAL = 3;
const unsigned int CHATMATE_INTERVAL = 3;



void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CEntityFriendComponent::CEntityFriendComponent()
    : CEntityComponent(true, true)
    , m_pNetComponent(NULL)
    , m_pChatComponent(NULL)
    , m_pRoleAttr(NULL)
    , m_pRoleRoom(NULL)
    , m_pRoleMail(NULL)
    , m_pRoleQuest(NULL)
    , m_pRoleVIP(NULL)
    , m_nBlackListInterval(0)
    , m_nPlayMateInterval(0)
    , m_nChatMateInterval(0)
    , m_nLastFindPlayerTime(0)
    , m_nLastGetRecommendTime(0)
    , m_nRecommendRefreshCount(0)
    , m_nLastRecommendResetTime(0)
{
}

CEntityFriendComponent::~CEntityFriendComponent()
{
	for (map< unsigned int, FriendInfo * >::iterator it = m_mapFriend.begin();
		it != m_mapFriend.end(); ++it)
	{
		delete it->second;
	}
	m_mapFriend.clear();
	for (map< unsigned int, FriendInfo * >::iterator it = m_mapBlackList.begin();
		it != m_mapBlackList.end(); ++it)
	{
		delete it->second;
	}
	m_mapBlackList.clear();
}

void CEntityFriendComponent::Start()
{
	m_pNetComponent = GetComponent<CRoleNet>();
	m_pChatComponent = GetComponent<CRoleChat>();
	m_pRoleAttr = GetComponent<CRoleAttribute>();
	m_pRoleRoom = GetComponent<CRoleRoom>();
	m_pRoleMail = GetComponent<CRoleMail>();
	m_pRoleQuest = GetComponent<CRoleQuest>();
	m_pRoleVIP = GetComponent<CRoleVIP>();
}

void CEntityFriendComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_FriendApplication);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AcceptApplication);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AddBlackFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RefuseApplication);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AddBlackList);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RemoveFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RemoveBlackList);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetBlackListOnlineInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetTempListPlayerOnlineInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_FindPlayer);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ReworkFrindNotes);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetRecommendFriends);

	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetFriendOnlineInfoResult);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_FriendApplicationForward2Receiver);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_FriendApplicationBack2Sender);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_NotifyGameRemoveFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_FriendLogin);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_NotifyHasBeenAccpted);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_BecomeFriends);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_NotifyGameHasBeenDeletedFriend);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetBlackListOnlineInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetTempListOnlineInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_PushOnlineInfoToAllFriends);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_UpdateAttributeToAllFriends);
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_FindPlayerResult);


	RegMsgProcessor(MSG_C2S_FriendApplication, &CEntityFriendComponent::OnFriendApplication);	
	RegMsgProcessor(MSG_C2S_AcceptApplication, &CEntityFriendComponent::OnAcceptApplication);
	RegMsgProcessor(MSG_C2S_AddBlackFriend, &CEntityFriendComponent::OnAddBlackFriend);
	RegMsgProcessor(MSG_C2S_RefuseApplication, &CEntityFriendComponent::OnRefuseApplication);
	RegMsgProcessor(MSG_C2S_AddBlackList, &CEntityFriendComponent::OnAddBlackList);
	RegMsgProcessor(MSG_C2S_RemoveBlackList, &CEntityFriendComponent::OnRemoveBlackList);
	RegMsgProcessor(MSG_C2S_RemoveFriend, &CEntityFriendComponent::OnRemoveFriend);
	RegMsgProcessor(MSG_C2S_GetBlackListOnlineInfo, &CEntityFriendComponent::OnGetBlackListOnlineInfo);
	RegMsgProcessor(MSG_C2S_GetTempListPlayerOnlineInfo, &CEntityFriendComponent::OnGetTempListOnlineInfo);
	RegMsgProcessor(MSG_C2S_FindPlayer, &CEntityFriendComponent::OnFindPlayer);
	RegMsgProcessor(MSG_C2S_ReworkFriendNotes, &CEntityFriendComponent::OnReworkFriendNotes);
    RegMsgProcessor(MSG_C2S_GetRecommendFriends, &CEntityFriendComponent::OnGetRecommendFriends);

	RegMsgProcessor(MSG_G2S_GetFriendOnlineInfoResult, &CEntityFriendComponent::_OnGetFriendList);
	RegMsgProcessor(MSG_G2S_FriendApplicationBack2Sender, &CEntityFriendComponent::OnFriendApplicationGSBack2Sender);
	RegMsgProcessor(MSG_G2S_FriendApplicationForward2Receiver, &CEntityFriendComponent::OnFriendApplicationGSForward2Receiver);
	RegMsgProcessor(MSG_G2S_NotifyGameRemoveFriend, &CEntityFriendComponent::OnNotifyGameHasBeenRemovedFriend);
	RegMsgProcessor(MSG_G2S_NotifiedHasBeenAccepted, &CEntityFriendComponent::OnNotifiedHasBeenAccepted);
	RegMsgProcessor(MSG_G2S_NotifyGameHasBeenDeletedFriend, &CEntityFriendComponent::OnNotifyGameHasBeenRemovedFriend);
	RegMsgProcessor(MSG_G2S_NotifyGameHasBeenDeletedBlacklist, &CEntityFriendComponent::OnNotifyGameHasBeenDeletedBlacklist);
	RegMsgProcessor(MSG_G2S_GetBlackListOnlineInfo, &CEntityFriendComponent::OnGetBlackListOnlineInfoFromWorld);
	RegMsgProcessor(MSG_G2S_GetTempListOnlineInfo, &CEntityFriendComponent::OnGetTempListOnlineInfoFromWorld);
	RegMsgProcessor(MSG_G2S_PushOnlineInfoToAllFriends, &CEntityFriendComponent::OnPushOnlineInfoToAllFriendsFromWorld);
	RegMsgProcessor(MSG_G2S_UpdateAttrbuteToAllFriends, &CEntityFriendComponent::OnUpdateFriendAttributeFromWorld);
	RegMsgProcessor(MSG_G2S_FriendLogin, &CEntityFriendComponent::OnNotifiedFriendLogin);
	RegMsgProcessor(MSG_G2S_FindPlayerResult, &CEntityFriendComponent::OnFindPlayerResult);

	// DB
	RegQueryProcessor(QUERY_RemoveRoleFriend,&CEntityFriendComponent::RemoveFriendDBCallBack);
	RegQueryProcessor(QUERY_RemoveBlackList,&CEntityFriendComponent::RemoveBlackListDBCallBack);
	RegQueryProcessor(QUERY_BecomeFriend,&CEntityFriendComponent::BecomeFriendsCallBack);
	RegQueryProcessor(QUERY_AddBlackListFriend,&CEntityFriendComponent::AddBlackListFriendDBCallBack);
	RegQueryProcessor(QUERY_GetBeAppliedPlayerInfo ,&CEntityFriendComponent::GetBeAppliedPlayerInfoDBCallBack);
	
	RegQueryProcessor(QUERY_AddBlackList,&CEntityFriendComponent::AddBlackListCallBack);

}

void CEntityFriendComponent::OnEvent(CComponentEvent& refEvent)
{
	if (refEvent.nID == eComponentEvent_NormalLogin) 
    {
		if (GetRoleAttr() != NULL) 
        {
			GameMsg_S2G_FriendLogin msg = GameMsg_S2G_FriendLogin(GetRoleAttr()->GetRoleID());
			SendMsg2GroupServer(&msg);
		}
	}
	//RequestAllFriendOnlineInfo();
}

bool CEntityFriendComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
    if (pRoleInfoDB == NULL)
        return false;

    for (list<FriendInfo>::iterator itr = pRoleInfoDB->listFriend.begin(); itr != pRoleInfoDB->listFriend.end(); ++itr)
    {
        FriendInfo *pFriend = new FriendInfo(*itr);	
        m_mapFriend.insert(make_pair(pFriend->m_nRoleID, pFriend) );
    }

    for (list<FriendInfo>::iterator itr = pRoleInfoDB->listBlackList.begin(); itr != pRoleInfoDB->listBlackList.end(); ++itr)
    {
        FriendInfo *pBlack = new FriendInfo(*itr);
        m_mapBlackList.insert(make_pair(pBlack->m_nRoleID, pBlack) );
    }

    m_nRecommendRefreshCount = pRoleInfoDB->m_nRecommendRefreshCount;
    m_nLastRecommendResetTime = pRoleInfoDB->m_nLastRecommendResetTime;

    return true;
}

bool CEntityFriendComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const 
{
	return true;
}

void CEntityFriendComponent::SerializeComponent(CParamPool &IOBuff)
{
}

void CEntityFriendComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
	if (m_pNetComponent != NULL) {
		m_pNetComponent->SendPlayerMsg(pMsg);
	}
}

void CEntityFriendComponent::SendOnlinePlayersMsg(GameMsg_Base *pMsg)
{
	map<unsigned int,FriendInfo *>::iterator it = m_mapFriend.begin();
	for ( ; it != m_mapFriend.end(); ++it )
	{
        CRoleEntity *pFriendRole =  CPlayerManager::Instance().GetEntityByRoleID( it->second->m_nRoleID );
		if ( pFriendRole != NULL)
		{
			pFriendRole->SendPlayerMsg( pMsg );
		}
	}
}

void CEntityFriendComponent::OnLogin()	
{
	SendBlackList();
	SendFindPlayerSetting();

    // 更新自己好友在线状态
    UpdateFriendOnlineState();

    // 通知好友更新自己在线状态
    NoticeFriendLogin();
}

void CEntityFriendComponent::GetFriendList( std::vector<unsigned int> &rolelist )
{
	map<unsigned int,FriendInfo *>::iterator it = m_mapFriend.begin();
	for ( ; it != m_mapFriend.end(); ++it )
	{
		rolelist.push_back( it->second->m_nRoleID );
	}
}

void CEntityFriendComponent::SendBlackList()
{
	GameMsg_S2C_BlackList msg;
	msg.m_mapBlackList = m_mapBlackList;
	SendPlayerMsg(&msg);
}

void CEntityFriendComponent::SendFindPlayerSetting()
{
	GameMsg_S2C_FindPlayerSetting msg;
	msg.m_FindFriendType = CFriendDataManager::Instance().m_FindFriendType;
	msg.m_FindFriendLevel = CFriendDataManager::Instance().m_FindFriendLevel;
	msg.m_FindFriendSex = CFriendDataManager::Instance().m_FindFriendSex;
	msg.m_FindFriendAge = CFriendDataManager::Instance().m_FindFriendAge;
	msg.m_FindFriendConstellation = CFriendDataManager::Instance().m_FindFriendConstellation;
	SendPlayerMsg(&msg);
}

void CEntityFriendComponent::OnLogout()
{
    std::map<unsigned int,FriendInfo*>::iterator it = m_mapFriend.begin();
    for (; it != m_mapFriend.end(); ++it)
    {
        CRoleEntity * pFriendEntity = CPlayerManager::Instance().GetEntityByRoleID(it->first);
        if (pFriendEntity != NULL)
        {
            pFriendEntity->GetComponent<CRoleFriend>()->FriendLogout(m_pRoleAttr->GetRoleID());
        }
    }
}

void CEntityFriendComponent::SendExtraStaticData()
{
}

bool CEntityFriendComponent::IsInBlackList(unsigned int nRoleID)
{
	return m_mapBlackList.end() != m_mapBlackList.find(nRoleID);
}

void CEntityFriendComponent::OnFriendApplication(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	if (m_pRoleAttr != NULL && m_pChatComponent != NULL)
	{
		GameMsg_C2S_FriendApplication *pAddMsg = (GameMsg_C2S_FriendApplication *)&msg;


		string strError = "";
		if (m_pRoleAttr->GetRoleID() == pAddMsg->m_nFriendRoleID)
		{
			strError = "Friend_Can_Not_Add_Self";
		}
		else if (IsFriend(pAddMsg->m_nFriendRoleID))
		{	
			strError = "Friend_Have_Added";
		}
		else if (FriendCount() >= FriendsLimitedCount())
		{
			strError = "Friend_Count_Limit";
		}
		else
		{
			CRoleEntity *friendEntity = CPlayerManager::Instance().GetEntityByRoleID(pAddMsg->m_nFriendRoleID);
			if (friendEntity != NULL)
			{
				CEntityFriendComponent *friendComp = friendEntity->GetComponent<CRoleFriend>();
				if (friendComp != NULL && friendComp->FriendCount() >= friendComp->FriendsLimitedCount())
				{
					strError = "Friend_Friend_Count_Limit";
				}
				else
				{
					FriendInfo *pBlackFriend = GetBlack(pAddMsg->m_nFriendRoleID);
					if (pBlackFriend != NULL && pBlackFriend->m_nRelationType == 0)
					{
						GameMsg_S2G_FriendApplication applyMsg = GameMsg_S2G_FriendApplication(m_pRoleAttr->GetRoleID(), 
							pAddMsg->m_nFriendRoleID, pAddMsg->m_nFriendCallBy, pAddMsg->m_nFriendCallTo, pAddMsg->m_bInsertMsgBox);
						SendMsg2GroupServer(&applyMsg);
					}
					else
					{
						FriendInfo *pFriendInfo = new FriendInfo;
						pFriendInfo->m_nRoleID = pAddMsg->m_nFriendRoleID;
						pFriendInfo->m_nCallBy = pAddMsg->m_nFriendCallBy;
						pFriendInfo->m_nCallTo = pAddMsg->m_nFriendCallTo;
						CDelHelper delHelper(pFriendInfo);
						AddQuery(QUERY_GetBeAppliedPlayerInfo, pAddMsg->m_bInsertMsgBox ? 1 : 0, pFriendInfo, 0, NULL, &delHelper);	
					}
				}
			}
			else
			{
				FriendInfo *pFriendInfo = new FriendInfo;
				pFriendInfo->m_nRoleID = pAddMsg->m_nFriendRoleID;
				pFriendInfo->m_nCallBy = pAddMsg->m_nFriendCallBy;
				pFriendInfo->m_nCallTo = pAddMsg->m_nFriendCallTo;
				CDelHelper delHelper(pFriendInfo);
				AddQuery(QUERY_GetBeAppliedPlayerInfo, pAddMsg->m_bInsertMsgBox ? 1 : 0, pFriendInfo, 0, NULL, &delHelper);	
			}
		}

		if (strError != "")
		{
			GameMsg_S2C_FriendApplicationFail failMsg;
			failMsg.m_strError = strError;
			SendPlayerMsg(&failMsg);
		}
	}
}

void CEntityFriendComponent::OnAddBlackFriend(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_C2S_AddBlackFriend *pMsg = (GameMsg_C2S_AddBlackFriend *)&msg;
	string strError = "";
	if (m_pRoleAttr != NULL) {
		if (m_pRoleAttr->GetRoleID() == pMsg->m_nRoleID) {
			strError = "Friend_Can_Not_Add_Self";
		} else if (IsFriend(pMsg->m_nRoleID)) {	
			strError = "Friend_Have_Added";
		} else if (FriendCount() >= FriendsLimitedCount()) {
			strError = "Friend_Count_Limit";
		} else {
			FriendInfo *pBlackFriend = RemoveBlackEntry(pMsg->m_nRoleID);
			if (pBlackFriend == NULL) {
				strError = "Friend_Not_In_Black_List";
			} else {
				AddFriend(pBlackFriend);

				GameMsg_S2C_AcceptApplicationSuc resultMsg;
				resultMsg.m_friendInfo = *pBlackFriend;
				SendPlayerMsg(&resultMsg);

				GameMsg_S2G_AddBlackListFriend addMsg;
				addMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
				addMsg.m_FriendInfo = *pBlackFriend;
				SendMsg2GroupServer(&addMsg);

				FriendInfo *pFriend = new FriendInfo(*pBlackFriend);
				CDelHelper delHelper(pFriend);
				AddQuery(QUERY_AddBlackListFriend, m_pRoleAttr->GetRoleID(), pFriend, 0, NULL, &delHelper);
			}
		}
	}
	
	if (strError != "") {
		GameMsg_S2C_AddBlackFriendFail failMsg;
		failMsg.m_strError = strError;
		SendPlayerMsg(&failMsg);
	}

}

void CEntityFriendComponent::AddBlackListCallBack(QueryBase &query)
{
    unsigned int nRoleID = (unsigned int)query.m_nAttachment;
    FriendInfo *pBlack = (FriendInfo *)query.m_pAttachment;

    if ( query.m_nResult != Query_Res_OK || pBlack == NULL )
    {
        delete pBlack, pBlack = NULL;

        return;
    }

    unsigned int nBlackRoleID = pBlack->m_nRoleID;
    FriendInfo *pEntity = RemoveFriendEntity( nBlackRoleID );

    GameMsg_S2C_AddBlackListSuc sendMsg;
    sendMsg.m_friendInfo.m_nRoleID = nBlackRoleID;
    sendMsg.m_friendInfo.SetName( pBlack->GetName() );
    sendMsg.m_friendInfo.m_strNotes = pBlack->m_strNotes;
    sendMsg.m_friendInfo.m_nSex = pBlack->m_nSex;
    sendMsg.m_friendInfo.m_nLevel = pBlack->m_nLevel;

    if ( pEntity != NULL )
    {
        InsertIntoBlackMap(pEntity);
        sendMsg.m_friendInfo.m_nRelationType = pEntity->m_nRelationType;
        sendMsg.m_friendInfo.m_nCallBy = pEntity->m_nCallBy;
        sendMsg.m_friendInfo.m_nCallTo = pEntity->m_nCallTo;

        delete pBlack, pBlack = NULL;
    }
    else
    {
        InsertIntoBlackMap( pBlack );
    }

    SendPlayerMsg( &sendMsg );

    GameMsg_S2G_AddBlackList notifyMsg( nRoleID, nBlackRoleID );
    SendMsg2GroupServer( &notifyMsg );

    LogRoleFriends( CRoleFriendsLog::EFriendsAction_Delete, CRoleFriendsLog::EFriendsMode_Del_ToBlack, nBlackRoleID );
}

void CEntityFriendComponent::OnAcceptApplication(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	if (m_pRoleAttr != NULL) {
		GameMsg_C2S_AcceptApplication *pAcceptMsg = (GameMsg_C2S_AcceptApplication *)&msg;
		FriendInfo friendInfo;
		friendInfo.m_nRoleID = pAcceptMsg->m_nFriendRoleID;
		friendInfo.m_nCallBy = pAcceptMsg->m_nFriendCallBy;
		friendInfo.m_nCallTo = pAcceptMsg->m_nFriendCallTo;

		string strError = "";
		/*if (IsFriend(pAcceptMsg->m_nFriendRoleID)) {
			strError = "Friend_Have_Added";//已经是好友时，继续走下面的加好友流程，防止对方没有加上好友
		} else */
		if (find(m_listApplication.begin(),m_listApplication.end(),friendInfo) == m_listApplication.end()) {
			strError = "Friend_Not_Exist_Or_Not_In_Application_List";
		} else {
			list<FriendInfo>::iterator itr = find( m_listApplication.begin(),m_listApplication.end(),friendInfo);
			FriendInfo *pFriendInfo = new FriendInfo(*itr);
			CDelHelper delHelper(pFriendInfo);
			AddQuery(QUERY_BecomeFriend, m_pRoleAttr->GetRoleID(), pFriendInfo, 0, NULL, &delHelper);
		}

		if (strError != "") {
			GameMsg_S2C_AcceptApplicationFail failMsg;
			failMsg.m_strError = strError;
			SendPlayerMsg(&failMsg);
		}
	}
}

bool CEntityFriendComponent::MakeOnlineFriend(unsigned int nFriendID, unsigned char nCallBy, unsigned char nCallTo)
{
    CRoleEntity *friendEntity = CPlayerManager::Instance().GetEntityByRoleID( nFriendID );

    if ( m_pRoleAttr->GetRoleID() == nFriendID )
    {
        return false;
    }
    else if ( IsFriend( nFriendID ) )
    {
        return true;
    }
    else if ( FriendCount() >= FriendsLimitedCount() )
    {
        return false;
    }
    else if ( friendEntity == NULL )
    {
        return false;
    }
    else
    {
        CEntityFriendComponent *friendComp = friendEntity->GetComponent<CRoleFriend>();
        if ( friendComp->FriendCount() >= friendComp->FriendsLimitedCount() )
        {
            return false;
        }

        unsigned char nRelationType = (unsigned char)CFriendRelationInfoManager::Instance().GetRelation( nCallBy, nCallTo );

        FriendInfo *pSelfFriendInfo = new FriendInfo;
        pSelfFriendInfo->m_nRoleID = nFriendID;
        pSelfFriendInfo->m_nRelationType = nRelationType;
        pSelfFriendInfo->m_nCallBy = nCallBy;
        pSelfFriendInfo->m_nCallTo = nCallTo;

        CDelHelper delHelper( pSelfFriendInfo );
        AddQuery( QUERY_BecomeFriend, m_pRoleAttr->GetRoleID(), pSelfFriendInfo, 0, NULL, &delHelper );

        FriendInfo *pFriendFriendInfo = new FriendInfo;
        pFriendFriendInfo->m_nRoleID = m_pRoleAttr->GetRoleID();
        pFriendFriendInfo->m_nRelationType = nRelationType;
        pFriendFriendInfo->m_nCallBy = nCallTo;
        pFriendFriendInfo->m_nCallTo = nCallBy;
        pFriendFriendInfo->m_bIsVIP = m_pRoleVIP->IsVIP();
        pFriendFriendInfo->m_nVIPLevel = m_pRoleVIP->VIPLevel();
        pFriendFriendInfo->SetName( m_pRoleAttr->GetRoleName() );
        pFriendFriendInfo->m_nLevel = m_pRoleAttr->GetLevel();
        pFriendFriendInfo->m_nSex = m_pRoleAttr->GetSex();

        FriendInfo *pBlack = friendComp->RemoveBlackEntry( pFriendFriendInfo->m_nRoleID );
        if ( pBlack != NULL )
        {
            delete pBlack, pBlack = NULL;
        }

        friendComp->AddFriend( pFriendFriendInfo );

        return true;
    }
}

void CEntityFriendComponent::OnRefuseApplication(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_RefuseApplication *pRefuseMsg = (GameMsg_C2S_RefuseApplication *)&msg;
	FriendInfo friendInfo;
	friendInfo.m_nRoleID = pRefuseMsg->m_nFriendRoleID;
	friendInfo.m_nCallBy = pRefuseMsg->m_nFriendCallBy;
	friendInfo.m_nCallTo = pRefuseMsg->m_nFriendCallTo;
	
	list<FriendInfo>::iterator itr = find(m_listApplication.begin(), m_listApplication.end(), friendInfo);
	if (itr != m_listApplication.end()) {
		m_listApplication.erase(itr);
	}

	GameMsg_S2C_RefuseApplicaionSuc sucMsg;
	SendPlayerMsg(&sucMsg);
}

void CEntityFriendComponent::OnRemoveFriend(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	if (m_pRoleAttr != NULL) {
		GameMsg_C2S_RemoveFriend *pMsg = (GameMsg_C2S_RemoveFriend *)&msg;

		string strError = "";
		if (!IsFriend(pMsg->m_nFriendRoleID)) {
			strError = "Friend_Is_Not_Your_Friend";	
		} else {
			AddQuery(QUERY_RemoveRoleFriend, m_pRoleAttr->GetRoleID(), NULL, pMsg->m_nFriendRoleID);
		}

		if (strError != "") {
			GameMsg_S2C_RemoveFriendFail failMsg;
			failMsg.m_strError = strError;
			SendPlayerMsg(&failMsg);
		}
	}
}

void CEntityFriendComponent::OnRemoveBlackList(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_RemoveBlackList *pMsg = (GameMsg_C2S_RemoveBlackList *)&msg;

	string strError = "";
	if (pMsg != NULL && m_pRoleAttr != NULL) {
		if (!IsInBlackList(pMsg->m_nBlackRoleID)) {
			strError = "Friend_Not_In_Black_List";	
		} else {
			AddQuery(QUERY_RemoveBlackList, m_pRoleAttr->GetRoleID(), NULL, pMsg->m_nBlackRoleID);
		}
	} 
		
	if (strError != "") {
		GameMsg_S2C_RemoveFriendFail failMsg;
		failMsg.m_strError = strError;
		SendPlayerMsg(&failMsg);
	}
}

void CEntityFriendComponent::OnGetBlackListOnlineInfo(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_GetBlackListOnlineInfo *pMsg = (GameMsg_C2S_GetBlackListOnlineInfo *)&msg;
	if (pMsg != NULL) 
    {
		unsigned int nTimeNow = (unsigned int)time(NULL);
		if (m_nBlackListInterval != 0 && nTimeNow - m_nBlackListInterval < BLACK_INTERVAL ) 
        {
			GameMsg_S2C_GetBlackListOnlineInfoFail failMsg;
			failMsg.m_strError = "Friend_Get_Black_List_Frequently";
			SendPlayerMsg(&failMsg);
		} 
        else 
        {
            GameMsg_S2G_GetBlackListOnlineInfo forwardMsg;
            forwardMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
            SendMsg2GroupServer(&forwardMsg);
		}
	}
}

void CEntityFriendComponent::OnGetBlackListOnlineInfoFromWorld(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_G2S_GetBlackListOnlineInfo *pMsg = (GameMsg_G2S_GetBlackListOnlineInfo *)&msg;

    if ( pMsg != NULL )
    {
        GameMsg_S2C_GetBlackListOnlineInfoSuc sucMsg;
        sucMsg.m_listBlakList = pMsg->m_listBlackOnlineInfo;

        for ( list<CFriendOnlineInfo>::iterator itr = sucMsg.m_listBlakList.begin(); itr != sucMsg.m_listBlakList.end(); ++itr )
        {
            FriendInfo *pBlack = GetBlack( itr->m_nRoleID );

            if ( pBlack != NULL )
            {
                itr->m_strName = pBlack->GetName();
                itr->m_strNotes = pBlack->m_strNotes;
                itr->m_nRelationType = pBlack->m_nRelationType;
                itr->m_nCallBy = pBlack->m_nCallBy;
                itr->m_nCallTo = pBlack->m_nCallTo;
            }
        }

        m_nBlackListInterval = (unsigned int)time( NULL );

        SendPlayerMsg( &sucMsg );
    }
}

void CEntityFriendComponent::OnGetTempListOnlineInfoFromWorld(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_G2S_GetTempListOnlineInfo & g2smsg = (GameMsg_G2S_GetTempListOnlineInfo &)msg;

    GameMsg_S2C_GetTempListPlayerOnlineInfoSuc resMsg;
    resMsg.m_nListType = g2smsg.m_nListType;
    resMsg.m_listOnlineInfo = g2smsg.m_listMate;

    for ( list<CFriendOnlineInfo>::iterator itr = resMsg.m_listOnlineInfo.begin(); 
        itr != resMsg.m_listOnlineInfo.end(); ++itr )
    {
        map<unsigned int, FriendInfo *>::iterator itrf = m_mapFriend.find( itr->m_nRoleID );
        if ( itrf != m_mapFriend.end() )
        {
            itr->m_nRelationType = itrf->second->m_nRelationType;
            itr->m_nCallBy = itrf->second->m_nCallBy;
            itr->m_nCallTo = itrf->second->m_nCallTo;
        }
        else
        {
            itrf = m_mapBlackList.find( itr->m_nRoleID );
            if ( itrf != m_mapBlackList.end() )
            {
                itr->m_nRelationType = itrf->second->m_nRelationType;
                itr->m_nCallBy = itrf->second->m_nCallBy;
                itr->m_nCallTo = itrf->second->m_nCallTo;
            }
        }

        if (itr->m_nLine == -1)
        {
            list<FriendInfo> listMate;
            if ( g2smsg.m_nListType == TempPlayerList_RecentlyPlayMate )
            {
                m_nPlayMateInterval = (unsigned int)time( NULL );
                listMate = m_listRecentlyPlayMate;
            }
            else
            {
                m_nChatMateInterval = (unsigned int)time( NULL );
                listMate = m_listRecentlyChatMate;
            }

            FriendInfo info;
            info.m_nRoleID = itr->m_nRoleID;
            list<FriendInfo>::iterator itrMate = find( listMate.begin(), listMate.end(), info );
            if ( itrMate != listMate.end() )
            {
                itr->m_strName = itrMate->GetName();
                itr->m_nLevel = itrMate->m_nLevel;
            }
        }
    }

    SendPlayerMsg( &resMsg );
}

void CEntityFriendComponent::OnGetTempListOnlineInfo(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_GetTempListPlayerOnlineInfo *pMsg = (GameMsg_C2S_GetTempListPlayerOnlineInfo *)&msg;

	if (pMsg != NULL) {
		string strError = "";
		list<unsigned int> listMate;
		unsigned int nTimeNow = (unsigned int)time(NULL);
		if (pMsg->m_nListType == TempPlayerList_RecentlyPlayMate) 
        {
			if (m_nPlayMateInterval != 0 && nTimeNow - m_nPlayMateInterval < PLAYMATE_INTERVAL) 
            {
				strError = "Friend_Get_List_Frequently";
			} 
            else 
            {
				for(list<FriendInfo>::iterator itr = m_listRecentlyPlayMate.begin(); itr != m_listRecentlyPlayMate.end(); ++itr) 
                {
					listMate.push_back(itr->m_nRoleID);
				}
			}
		} 
        else if (pMsg->m_nListType == TempPlayerList_RecentlyChatMate) 
        {
			if (m_nChatMateInterval != 0 && nTimeNow - m_nChatMateInterval < CHATMATE_INTERVAL) 
            {
				strError = "Friend_Get_List_Frequently";
			} 
            else 
            {
				for(list<FriendInfo>::iterator itr = m_listRecentlyChatMate.begin(); itr != m_listRecentlyChatMate.end(); ++itr) 
                {
					listMate.push_back(itr->m_nRoleID);
				}
			}
		}

		if (strError == "" && m_pRoleAttr != NULL) 
        {
			GameMsg_S2G_GetTempListOnlineInfo forwardMsg;
			forwardMsg.m_nRequestRoleID = m_pRoleAttr->GetRoleID();
			forwardMsg.m_nListType = pMsg->m_nListType;
			forwardMsg.m_listMate = listMate;
			SendMsg2GroupServer(&forwardMsg);
		} 
        else 
        {
			GameMsg_S2C_GetTempListPlayerOnlineInfoFail failMsg;
			failMsg.m_nListType = pMsg->m_nListType;
			failMsg.m_strError = strError;
			SendPlayerMsg(&failMsg);
		}
	}
}

void CEntityFriendComponent::_OnGetFriendList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetFriendOnlineInfoResult *pMsg = (GameMsg_G2S_GetFriendOnlineInfoResult *)&msg;

    GameMsg_S2C_GetFriendOnlineInfoResult   replyMsg;
    list<CFriendOnlineInfo>::iterator listItr = pMsg->m_listFriendOnlineInfo.begin();
    for (; listItr != pMsg->m_listFriendOnlineInfo.end(); ++listItr) 
    {
        map<unsigned int, FriendInfo *>::iterator mapItr= m_mapFriend.find(listItr->m_nRoleID);
        if ( mapItr != m_mapFriend.end() )
        {
            CFriendOnlineInfo info = *listItr;

            if (info.m_nLine != -1)
            {
                UpdateFriendOnlineInfo( mapItr->second, info );
            }

            info.m_strName = mapItr->second->GetName();
            info.m_strNotes = mapItr->second->m_strNotes;
            info.m_nSex = mapItr->second->m_nSex;
            info.m_nLevel = mapItr->second->m_nLevel;
            info.m_nRelationType = mapItr->second->m_nRelationType;
            info.m_nCallBy = mapItr->second->m_nCallBy;
            info.m_nCallTo = mapItr->second->m_nCallTo;

            replyMsg.m_listFriend.push_back( info );
        }
    }

    SendPlayerMsg( &replyMsg );
}

void CEntityFriendComponent::OnPushOnlineInfoToAllFriendsFromWorld(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_PushOnlineInfoToAllFriends & g2smsg = (GameMsg_G2S_PushOnlineInfoToAllFriends &)msg;

    map<unsigned int, FriendInfo *>::iterator it = m_mapFriend.find( g2smsg.m_roleOnlineInfo.m_nRoleID );
    if (it == m_mapFriend.end() || it->second == NULL) 
    {
        return;
    }

    //更新好友信息
    if(g2smsg.m_roleOnlineInfo.m_nLine != -1)
    {
        UpdateFriendOnlineInfo(it->second, g2smsg.m_roleOnlineInfo);
    }

    GameMsg_S2C_UpdateFriendInfo replyMsg;
    replyMsg.m_friendOnlineInfo = g2smsg.m_roleOnlineInfo;
    replyMsg.m_friendOnlineInfo.m_strName = it->second->m_strName;
    replyMsg.m_friendOnlineInfo.m_strNotes = it->second->m_strNotes;
    replyMsg.m_friendOnlineInfo.m_nSex = it->second->m_nSex;
    replyMsg.m_friendOnlineInfo.m_nLevel = it->second->m_nLevel;
    replyMsg.m_friendOnlineInfo.m_nRelationType = it->second->m_nRelationType;
    replyMsg.m_friendOnlineInfo.m_nCallBy = it->second->m_nCallBy;
    replyMsg.m_friendOnlineInfo.m_nCallTo = it->second->m_nCallTo;
    replyMsg.m_friendOnlineInfo.m_bIsVIP = it->second->m_bIsVIP;
    replyMsg.m_friendOnlineInfo.m_nVIPLevel = (unsigned short)it->second->m_nVIPLevel;
    SendPlayerMsg( &replyMsg );
}

void CEntityFriendComponent::UpdateFriendOnlineInfo(FriendInfo* pFriend, CFriendOnlineInfo& friendInfo)
{
    pFriend->m_bIsVIP = friendInfo.m_bIsVIP;
    pFriend->m_nLevel = friendInfo.m_nLevel;
    pFriend->m_nVIPLevel = friendInfo.m_nVIPLevel;
    pFriend->SetName( friendInfo.m_strName );
}

void CEntityFriendComponent::OnUpdateFriendAttributeFromWorld(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_UpdateAttributeToAllFriends *pMsgUpdate = (GameMsg_G2S_UpdateAttributeToAllFriends *)&msg;

    if ( pMsgUpdate != NULL )
    {
        FriendInfo *pFriend = GetFriend( pMsgUpdate->m_nRoleID );

        if ( pFriend != NULL )
        {
            pFriend->SetName( pMsgUpdate->m_strName );
            pFriend->m_nLevel = pMsgUpdate->m_nLevel;
            pFriend->m_bIsVIP = pMsgUpdate->m_bIsVIP;
            pFriend->m_nVIPLevel = pMsgUpdate->m_nVIPLevel;
        }
    }
}

void CEntityFriendComponent::OnFindPlayerResult(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_G2S_FindPlayerResult *pMsg = (GameMsg_G2S_FindPlayerResult *)&msg;

	if (pMsg->m_strError.empty())
	{
		if (!pMsg->m_PlayerList.empty())
		{
			GameMsg_S2C_FindPlayerResult resultMsg;
			resultMsg.m_PlayerList = pMsg->m_PlayerList;

			SendPlayerMsg(&resultMsg);
		}
		else
		{
			GameMsg_S2C_FindPlayerFail fail;
			fail.m_strError = "FindPlayer_NoPlayer";
			SendPlayerMsg(&fail);
		}
	}
	else
	{
		GameMsg_S2C_FindPlayerFail fail;
		fail.m_strError = pMsg->m_strError;
		SendPlayerMsg(&fail);
	}
}

void CEntityFriendComponent::OnFindPlayer(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	int nFindPlayerInterval = CFriendDataManager::Instance().m_nFindFriendCoolDown;
	unsigned int nNow = (unsigned int)time(NULL);
	if (m_nLastFindPlayerTime + nFindPlayerInterval <= nNow)
	{
		GameMsg_C2S_FindPlayer *pMsg = (GameMsg_C2S_FindPlayer *)&msg;
		if (pMsg != NULL && (pMsg->m_strFindRoleName.length() > 0 || pMsg->m_nFindRoleID > 0 ))
		{
			GameMsg_S2G_FindPlayer gMsg;
			gMsg.m_nFindFriendTypeIndex = pMsg->m_nFindFriendTypeIndex;
			gMsg.m_nFindRoleID = pMsg->m_nFindRoleID;
			gMsg.m_strFindRoleName = pMsg->m_strFindRoleName;
			gMsg.m_nFindLevelIndex = pMsg->m_nFindLevelIndex;
			gMsg.m_nFindSexIndex = pMsg->m_nFindSexIndex;
			gMsg.m_nFindAgeIndex = pMsg->m_nFindAgeIndex;
			gMsg.m_nFindConstellationIndex = pMsg->m_nFindConstellationIndex;

			m_pNetComponent->Send2GroupServer(&gMsg);
		}
		m_nLastFindPlayerTime = nNow;
	}
	else
	{
		GameMsg_S2C_FindPlayerFail fail;
		fail.m_strError = "FindPlayer_CoolDown";
		fail.m_nCoolDownTime = nFindPlayerInterval - (nNow - m_nLastFindPlayerTime);
		SendPlayerMsg(&fail);
	}
}

void CEntityFriendComponent::OnAddBlackList(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    if ( m_pRoleAttr != NULL )
    {
        GameMsg_C2S_AddBlackList *pAddMsg = (GameMsg_C2S_AddBlackList *)&msg;

        string strError = "";
        if ( pAddMsg->m_nRoleID == m_pRoleAttr->GetRoleID() )
        {
            strError = "Friend_Can_Not_Add_Self";
        }
        else if ( IsInBlackList( pAddMsg->m_nRoleID ) )
        {
            strError = "Friend_Have_In_Black_List";
        } 
        else if ( BlackListCount() >= BlackListLimitedCount() )
        {
            strError = "BlackList_Count_Limit";
        }
        else
        {
            FriendInfo *pBlackRole = new FriendInfo;
            pBlackRole->m_nRoleID = pAddMsg->m_nRoleID;

            FriendInfo * pFriend = GetFriend( pAddMsg->m_nRoleID );
            if ( pFriend != NULL )
            {
                pBlackRole->m_strNotes = pFriend->m_strNotes;
                pBlackRole->SetName( pFriend->GetName() );
                pBlackRole->m_nVIPLevel  = pFriend->m_nVIPLevel;
                pBlackRole->m_bIsVIP  = pFriend->m_bIsVIP;
                pBlackRole->m_nSex  = pFriend->m_nSex;
                pBlackRole->m_nLevel = pFriend->m_nLevel;
            }

            CDelHelper delHelper( pBlackRole );
            AddQuery( QUERY_AddBlackList, m_pRoleAttr->GetRoleID(), pBlackRole, 0, NULL, &delHelper );
        }

        if ( strError != "" )
        {
            GameMsg_S2C_AddBlackListFail failMsg;
            failMsg.m_strError = strError;

            SendPlayerMsg( &failMsg );
        }
    }
}

void CEntityFriendComponent::RemoveFriendDBCallBack(QueryBase &query)
{
	if (query.m_nResult == 0) 
    {
		if (m_pRoleAttr != NULL) 
        {
			unsigned int nFriendRoleID = (unsigned int)query.m_nTag;
			FriendInfo *pEntity = RemoveFriendEntity(nFriendRoleID);

			if (pEntity != NULL) 
            {
				GameMsg_S2C_RemoveFriendSuc sucMsg;
				sucMsg.m_nRoleID = nFriendRoleID;
				SendPlayerMsg(&sucMsg);

				GameMsg_S2G_RemoveFriend removeMsg;
				removeMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
				removeMsg.m_nTargetID = nFriendRoleID;
				SendMsg2GroupServer(&removeMsg);

				delete pEntity, pEntity = NULL;

                LogRoleFriends(CRoleFriendsLog::EFriendsAction_Delete, CRoleFriendsLog::EFriendsMode_Del_Normal, nFriendRoleID);
			}
		}
	}
}

void CEntityFriendComponent::BecomeFriendsCallBack(QueryBase &query)
{
	if (m_pRoleAttr != NULL){
		if (query.m_nResult == 0){
			FriendInfo *pFriend = (FriendInfo *)query.m_pAttachment;
			FriendInfo self(m_pRoleAttr->GetRoleID(), m_pRoleAttr->GetRoleName(), "", m_pRoleAttr->GetSex(), m_pRoleAttr->GetLevel(),
							pFriend->m_nRelationType, pFriend->m_nCallTo, pFriend->m_nCallBy, m_pRoleVIP->IsVIP(), (unsigned short)m_pRoleVIP->VIPLevel());

			if (pFriend != NULL){
				FriendInfo *pBlack = RemoveBlackEntry(pFriend->m_nRoleID);
				if (pBlack != NULL) {
					delete pBlack, pBlack = NULL;
				}

				AddFriend(pFriend);
				GameMsg_S2G_NotifyHasBeenAccpted notifyMsg;
				notifyMsg.m_nRoleID = pFriend->m_nRoleID;
				notifyMsg.m_friendInfo = self;
				SendMsg2GroupServer(&notifyMsg);

				GameMsg_S2C_AcceptApplicationSuc resultMsg;
				resultMsg.m_friendInfo = *pFriend;
				SendPlayerMsg(&resultMsg);

				UpdateFriendInfo(pFriend->m_nRoleID);

				RemoveApplication(pFriend->m_nRoleID);
			}
		}
	}
}

void CEntityFriendComponent::AddBlackListFriendDBCallBack(QueryBase &query)
{
	FriendInfo *pFriend = (FriendInfo *)query.m_pAttachment;	
	if (pFriend != NULL) {
		UpdateFriendInfo(pFriend->m_nRoleID);

        LogRoleFriends(CRoleFriendsLog::EFriendsAction_Add, CRoleFriendsLog::EFriendsMode_Add_Black, pFriend->m_nRoleID);
	}

	delete pFriend, pFriend = NULL;
}

void CEntityFriendComponent::GetBeAppliedPlayerInfoDBCallBack(QueryBase &query)
{
    FriendInfo *pFriendInfo = (FriendInfo *)query.m_pAttachment;

    if ( query.m_nResult == 0 )
    {
        string strError = "";

        if ( pFriendInfo != NULL )
        {
            if ( pFriendInfo->GetName() == "" )
            {
                strError = "Friend_Not_Exist";
            }
            else if ( !CheckFriendRelation( pFriendInfo->m_nCallBy, pFriendInfo->m_nCallTo, pFriendInfo->m_nSex, GetRoleAttr()->GetSex() ) )
            {
                strError = "Friend_Name_Error";
            }
            else
            {
                bool bInsertMsgBox = ( query.m_nAttachment == 1 ? true : false );

                GameMsg_S2G_FriendApplication applyMsg(m_pRoleAttr->GetRoleID(), pFriendInfo->m_nRoleID, 
                    pFriendInfo->m_nCallBy, pFriendInfo->m_nCallTo, bInsertMsgBox);

                SendMsg2GroupServer( &applyMsg );
            }
        }

        if ( strError != "" )
        {
            GameMsg_S2C_FriendApplicationFail failMsg;
            failMsg.m_strError = strError;

            SendPlayerMsg( &failMsg );
        }
    }

    delete pFriendInfo, pFriendInfo = NULL;
}

void CEntityFriendComponent::RemoveBlackListDBCallBack(QueryBase &query)
{
	if (query.m_nResult == 0) {
		unsigned int nRoleID = (unsigned int)query.m_nAttachment;
		unsigned int nDestID = (unsigned int)query.m_nTag;

		FriendInfo *pBlcak = RemoveBlackEntry(nDestID);
		if (pBlcak != NULL) {
			delete pBlcak, pBlcak = NULL;
		}

		GameMsg_S2C_RemoveBlackListSuc sucMsg;
		sucMsg.m_nRoleID = nDestID;
		SendPlayerMsg(&sucMsg);

		GameMsg_S2G_RemoveBlackList removeMsg(nRoleID, nDestID);
		SendMsg2GroupServer(&removeMsg);

        LogRoleFriends(CRoleFriendsLog::EFriendsAction_Delete, CRoleFriendsLog::EFriendsMode_Del_FromBlack, nDestID);
	}
}


bool CEntityFriendComponent::IsFriend(unsigned int nRoleID)
{
	return m_mapFriend.end() != m_mapFriend.find(nRoleID);
}

unsigned short CEntityFriendComponent::FriendCount()
{
	return (unsigned short)m_mapFriend.size();
}

unsigned short CEntityFriendComponent::BlackListCount()
{
	return (unsigned short)m_mapBlackList.size();
}

bool CEntityFriendComponent::CheckFriendRelation(int nLeftNameCode, int nRightNameCode, int nLeftGender, int nRightGender)
{
	return CFriendRelationInfoManager::Instance().RelationExists(nLeftNameCode, nRightNameCode, nLeftGender, nRightGender); 
}

void CEntityFriendComponent::AddFriend(FriendInfo *pFriendInfo)
{
	if (pFriendInfo != NULL) 
    {
		InsertIntoFriendMap(pFriendInfo);
		if (m_pRoleQuest != NULL)
        {
			m_pRoleQuest->OnRoleAttrChange();
		}
	}
}

void CEntityFriendComponent::InsertIntoFriendMap(FriendInfo * pFriendInfo)
{
	if (pFriendInfo != NULL && pFriendInfo->m_nRoleID > 0) {
		m_mapFriend.insert(make_pair(pFriendInfo->m_nRoleID, pFriendInfo));
	}
}

FriendInfo * CEntityFriendComponent::RemoveFriendEntity(unsigned int nRoleID)
{
	FriendInfo *pEntity = NULL;

	if (nRoleID > 0) {
		map<unsigned int, FriendInfo* >::iterator itr = m_mapFriend.find(nRoleID);
		if (itr != m_mapFriend.end()) {
			pEntity = itr->second;
			m_mapFriend.erase(itr);
		}
	}

	return pEntity;
}
void CEntityFriendComponent::InsertIntoBlackMap(FriendInfo * pFriendInfo)
{
	if (pFriendInfo != NULL && pFriendInfo->m_nRoleID > 0) {
		m_mapBlackList.insert(make_pair(pFriendInfo->m_nRoleID, pFriendInfo));
	}
}

FriendInfo * CEntityFriendComponent::RemoveBlackEntry(unsigned int nRoleID)
{
	FriendInfo *pEntity = NULL;
	if (nRoleID > 0) {
		map< unsigned int, FriendInfo * >::iterator itr = m_mapBlackList.find(nRoleID);
		if (itr != m_mapBlackList.end()) {
			pEntity = itr->second;
			m_mapBlackList.erase(itr);
		}
	}

	return pEntity;
}

void CEntityFriendComponent::AddApplication(const FriendInfo & friendInfo)
{
	m_listApplication.push_back(friendInfo);
}

void CEntityFriendComponent::RemoveApplication(unsigned int nRoleID)
{
	list<FriendInfo>::iterator itr = m_listApplication.begin();
	while (itr != m_listApplication.end()) {
		if (itr->m_nRoleID == nRoleID) {
			m_listApplication.erase(itr++);
		} else {
			++itr;
		}
	}
}

bool CEntityFriendComponent::IsInApplicationList(const FriendInfo & friendInfo)
{
	return 	find(m_listApplication.begin(), m_listApplication.end(), friendInfo) != m_listApplication.end();
}

void CEntityFriendComponent::NotifyGroupAddBlackListFriend(unsigned int nRoleID, FriendInfo &friendInfo)
{
	if (nRoleID > 0 && friendInfo.m_nRoleID > 0) {
		GameMsg_S2G_AddBlackListFriend addMsg;
		addMsg.m_nRoleID = nRoleID;
		addMsg.m_FriendInfo = friendInfo;
		SendMsg2GroupServer(&addMsg);
	}
}

void CEntityFriendComponent::OnFriendApplicationGSBack2Sender(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_G2S_FriendApplicationBack2Sender *pMsg = (GameMsg_G2S_FriendApplicationBack2Sender *)&msg;
	
	if (GetRoleChat() != NULL) {
		if (pMsg->m_nResult == APPLICATION_SENT) {
			GameMsg_S2C_FriendApplicationSuc sucMsg;
			sucMsg.m_strFriendName = pMsg->m_strFriendName;
			SendPlayerMsg(&sucMsg);
		} else if (pMsg->m_nResult == PLAYER_OFFLINE) {
			GameMsg_S2C_FriendApplicationFail failMsg;
			failMsg.m_strError = "Friend_Not_On_Line";
			SendPlayerMsg(&failMsg);
		} else if (pMsg->m_nResult == PLAYER_PUTINBLACKLIST) {

		} else if (pMsg->m_nResult == PLAYER_NOTSAMELINE) {
			GameMsg_S2C_FriendApplicationFail failMsg;
			failMsg.m_strError = "Friend_Not_On_Same_Line";
			SendPlayerMsg(&failMsg);
		}
	}
}

void CEntityFriendComponent::OnFriendApplicationGSForward2Receiver(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_FriendApplicationForward2Receiver *pMsg = (GameMsg_G2S_FriendApplicationForward2Receiver  *)&msg;

    if ( pMsg != NULL )
    {
        unsigned char nRelationType = (unsigned char)CFriendRelationInfoManager::Instance().GetRelation( pMsg->m_nCallBy, pMsg->m_nCallTo );

        FriendInfo friendInfo( pMsg->m_nSenderID, pMsg->m_szRoleName, "", 
            pMsg->m_nSex, pMsg->m_nLevel, nRelationType, pMsg->m_nCallBy, 
            pMsg->m_nCallTo, pMsg->m_bIsVIP, pMsg->m_nVIPLevel );
        AddApplication(friendInfo);

        GameMsg_S2C_FriendApplication applyMsg;
        applyMsg.m_friendInfo.m_nRoleID = pMsg->m_nSenderID;
        applyMsg.m_friendInfo.SetName( pMsg->m_szRoleName );
        applyMsg.m_friendInfo.m_nSex = pMsg->m_nSex;
        applyMsg.m_friendInfo.m_nLevel = pMsg->m_nLevel;
        applyMsg.m_friendInfo.m_nCallBy = pMsg->m_nCallBy;
        applyMsg.m_friendInfo.m_nCallTo = pMsg->m_nCallTo;
        applyMsg.m_friendInfo.m_bIsVIP = pMsg->m_bIsVIP;
        applyMsg.m_friendInfo.m_nVIPLevel = pMsg->m_nVIPLevel;
        applyMsg.m_bInsertMsgBox = pMsg->m_bInsertMsgBox;

        SendPlayerMsg(&applyMsg);
    }
}


void CEntityFriendComponent::NotifyGroupBecomeFriends(unsigned int nRoleID, const FriendInfo &friendInfo)
{
	GameMsg_S2G_BecomeFriends gameMsg;
	gameMsg.m_nRoleID = nRoleID;
	gameMsg.m_friendInfo = friendInfo;

	SendMsg2GroupServer(&gameMsg);
}


void CEntityFriendComponent::OnNotifiedHasBeenAccepted(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_G2S_NotifyHasBeenAccpted *pMsg = (GameMsg_G2S_NotifyHasBeenAccpted *)&msg;
	if (pMsg != NULL && m_pChatComponent != NULL && m_pRoleAttr) {
		FriendInfo *pBlack = RemoveBlackEntry(pMsg->m_friendInfo.m_nRoleID);
		if (pBlack != NULL) {
			delete pBlack, pBlack = NULL;
		}

		FriendInfo *pFriend = new FriendInfo(pMsg->m_friendInfo);
		AddFriend(pFriend);

		GameMsg_S2C_NotifyHasBeenAccepted sendMsg;
		sendMsg.m_friendInfo = pMsg->m_friendInfo;
		SendPlayerMsg(&sendMsg);

        std::set<unsigned int>::iterator itr = m_setRecommendFriends.find(pMsg->m_friendInfo.m_nRoleID);

        if (itr == m_setRecommendFriends.end() )
        {
            LogRoleFriends(CRoleFriendsLog::EFriendsAction_Add, CRoleFriendsLog::EFriendsMode_Add_Normal, pMsg->m_friendInfo.m_nRoleID);
        }
        else
        {
            LogRoleFriends(CRoleFriendsLog::EFriendsAction_Add, CRoleFriendsLog::EFriendsMode_Add_Recommend, pMsg->m_friendInfo.m_nRoleID);

            m_setRecommendFriends.erase(itr);
        }
	}
}

void CEntityFriendComponent::OnNotifyGameHasBeenRemovedFriend(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_G2S_NotifyGameHasBeenDeletedFriend *pMsg = (GameMsg_G2S_NotifyGameHasBeenDeletedFriend *)&msg;
	if (pMsg != NULL) {
		if (IsFriend(pMsg->m_nRoleID)) {
			FriendInfo *pFriend =  RemoveFriendEntity(pMsg->m_nRoleID);
			if (pFriend != NULL) {
				delete pFriend, pFriend = NULL;
			}
		} else {
			FriendInfo *pBlackInfo = GetBlack(pMsg->m_nRoleID);	
			if (pBlackInfo != NULL) {
				pBlackInfo->m_nRelationType = 0;
				pBlackInfo->m_nCallTo = 0;
				pBlackInfo->m_nCallBy = 0;
			}
		}
		GameMsg_S2C_HasBeenRemovedFriend resMsg;
		resMsg.m_nSrcID = pMsg->m_nRoleID;
		SendPlayerMsg(&resMsg);
	}
}

void CEntityFriendComponent::OnNotifyGameHasBeenDeletedBlacklist(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist *pMsg = (GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist *)&msg;	
	if (pMsg != NULL) {
		FriendInfo *pEntity = NULL;
		if ( (pEntity = RemoveFriendEntity(pMsg->m_nSrcRoleID)) == NULL ) {
			FriendInfo *pBlack = GetBlack(pMsg->m_nSrcRoleID);
			if (pBlack != NULL) {
				pBlack->m_nRelationType = 0;
				pBlack->m_nCallBy = 0;
				pBlack->m_nCallTo = 0;
			}
		} else {
			delete pEntity, pEntity = NULL;
		}

		GameMsg_S2C_HasBeenRemovedBlackList noticeMsg;
		noticeMsg.m_nSrcID = pMsg->m_nSrcRoleID;
		SendPlayerMsg(&noticeMsg);
	}
}

void CEntityFriendComponent::OnNotifiedFriendLogin(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_FriendLogin &msgFriendLogin = (GameMsg_G2S_FriendLogin &)msg;

    FriendInfo *pFriend = GetFriend( msgFriendLogin.m_FriendOnlineInfo.m_nRoleID );

    if ( pFriend != NULL )
    {
        std::string strCallingName;
        CFriendRelationInfoManager::Instance().GetCallingName( pFriend->m_nCallTo, strCallingName );

        if ( m_pChatComponent != NULL )
        {
            std::string strContent;
            SafeFormat(strContent, CLocalization::Instance().GetString("Friend_Login_Notify"), strCallingName.c_str(),
                pFriend->m_strNotes.empty() ? pFriend->GetName().c_str() : pFriend->m_strNotes.c_str() );

            m_pChatComponent->SysChat( strContent.c_str() );
        }
    }
}

void CEntityFriendComponent::OnReworkFriendNotes( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
	GameMsg_C2S_ReworkFrindNotes & c2sMsg = ( GameMsg_C2S_ReworkFrindNotes&)msg;
	FriendInfo  * pFriendInfo = GetFriend( c2sMsg.m_nSelectRoleID );
	if ( pFriendInfo != NULL )
	{
		if ( pFriendInfo->m_strNotes != c2sMsg.m_strNotes )
		{
			pFriendInfo->m_strNotes = c2sMsg.m_strNotes;
			FriendInfo * pTempFriendInfo = new FriendInfo( *pFriendInfo );
			if ( pTempFriendInfo != NULL )
			{
				AddQuery( QUERY_ReworkFriendNotes, m_pRoleAttr->GetRoleID(), pTempFriendInfo );
			}
		}
	}
}

void CEntityFriendComponent::OnGetRecommendFriends(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    unsigned int nNow = (unsigned int)time(NULL);
    unsigned int nCD = CFriendDataManager::Instance().GetRecommendCD();

    if (m_pRoleRoom != NULL && m_pRoleRoom->Room() != NULL)
        return;

    CheckRecommendResetTime(nNow);

    GameMsg_C2S_GetRecommendFriends &rGetRecommendMsg = dynamic_cast<GameMsg_C2S_GetRecommendFriends &>(msg);
    GameMsg_S2C_GetRecommendFriendsResult resultMsg;
    resultMsg.m_nRefreshCount = (unsigned short)m_nRecommendRefreshCount;

    if (m_nRecommendRefreshCount == 0)
    {
        resultMsg.m_nError = ERecommendFriendsError_NotReset;
        resultMsg.m_fCD = (float)(m_nLastRecommendResetTime + CFriendDataManager::Instance().GetRecommendResetSeconds() - nNow);
    }
    else if (nNow - m_nLastGetRecommendTime < nCD)
    {
        resultMsg.m_nError = ERecommendFriendsError_InCD;
        resultMsg.m_fCD = (float)(m_nLastGetRecommendTime + nCD - nNow);
    }
    else if (rGetRecommendMsg.m_nRecommendType >= ERecommendFriendsType_Max)
    {
        resultMsg.m_nError = ERecommendFriendsError_InvalidType;
        resultMsg.m_fCD = (float)nCD;
    }

    if (resultMsg.m_nError != ERecommendFriendsError_Success)
    {
        SendPlayerMsg(&resultMsg);

        return;
    }

    unsigned char oppositeSex = ESexType_No;

    if (rGetRecommendMsg.m_nRecommendType == ERecommendFriendsType_Male)
        oppositeSex = ESexType_Female;
    else if (rGetRecommendMsg.m_nRecommendType == ERecommendFriendsType_Female)
        oppositeSex = ESexType_Male;

    RandomRecommendFriends(resultMsg.m_listPlayers, oppositeSex, rGetRecommendMsg.m_nMaxCount);

    m_nLastGetRecommendTime = nNow;
    --m_nRecommendRefreshCount;
    if (m_pRoleAttr != NULL)
        AddQuery(QUERY_Recommend_UpdateCount, m_pRoleAttr->GetRoleID(), NULL, (__int64)m_nRecommendRefreshCount);

    resultMsg.m_nRefreshCount = (unsigned short)m_nRecommendRefreshCount;
    resultMsg.m_fCD = (float)nCD;

    if (m_nRecommendRefreshCount == 0)
        resultMsg.m_fCD = (float)(m_nLastRecommendResetTime + CFriendDataManager::Instance().GetRecommendResetSeconds() - nNow);

    SendPlayerMsg(&resultMsg);
}

unsigned short CEntityFriendComponent::FriendsLimitedCount()
{
    int nCount = 0;
    if (CInitLua::GetInstance().DoString("FriendLimitedCount", enumSCRIPT_RETURN_NUMBER, 1, DOSTRING_PARAM_END) != -1)
    {
        CInitLua::GetInstance().GetRetNum(nCount, 0);
    }

    if (m_pRoleVIP != NULL && m_pRoleVIP->IsVIP())
    {
        int nVIPLevel = m_pRoleVIP->VIPLevel();
        const CVIPLevelPrivInfo *pInfo = CVIPDataMgr::Instance().GetVIPPrivInfo(nVIPLevel);
        if (pInfo != NULL)
        {
            nCount = pInfo->m_nMaxFriendCount;
        }
    }

    return (unsigned short)nCount;
}

unsigned short CEntityFriendComponent::BlackListLimitedCount()
{
    int nCount = 0;
    if (CInitLua::GetInstance().DoString("BlackListLimitedCount", enumSCRIPT_RETURN_NUMBER, 1, DOSTRING_PARAM_END) != -1)
    {
        CInitLua::GetInstance().GetRetNum(nCount, 0);
    }

    if (m_pRoleVIP != NULL && m_pRoleVIP->IsVIP())
    {
        int nVIPLevel = m_pRoleVIP->VIPLevel();
        const CVIPLevelPrivInfo *pInfo = CVIPDataMgr::Instance().GetVIPPrivInfo(nVIPLevel);
        if (pInfo != NULL)
        {
            nCount = pInfo->m_nMaxBlackListCount;
        }
    }

    return (unsigned short)nCount;
}

FriendInfo * CEntityFriendComponent::GetBlack(unsigned int nRoleID)
{
	for (map< unsigned int, FriendInfo * >::iterator itr = m_mapBlackList.begin(); itr != m_mapBlackList.end(); ++itr) {
		if (itr->second->m_nRoleID == nRoleID) {
			return itr->second;
		}
	}

	return NULL;
}

FriendInfo * CEntityFriendComponent::GetFriend(unsigned int nRoleID)
{
	for (map< unsigned int, FriendInfo * >::iterator itr = m_mapFriend.begin(); itr != m_mapFriend.end(); ++itr) {
		if (itr->second->m_nRoleID == nRoleID) 
        {
			return itr->second;
		}
	}

	return NULL;
}

void CEntityFriendComponent::UpdateAttributeToAllFriends()
{
	if (m_pRoleAttr != NULL) {
		GameMsg_S2G_UpdateAttributeToAllFriends msgUpdateAttr;
		msgUpdateAttr.m_nRoleID = m_pRoleAttr->GetRoleID();
		msgUpdateAttr.m_strName = m_pRoleAttr->GetRoleName();
		msgUpdateAttr.m_nLevel = m_pRoleAttr->GetLevel();
		if (m_pRoleVIP != NULL) {
			msgUpdateAttr.m_bIsVIP = m_pRoleVIP->IsVIP();
			msgUpdateAttr.m_nVIPLevel = (unsigned short)m_pRoleVIP->VIPLevel();
		}

		SendMsg2GroupServer(&msgUpdateAttr);
	}
}

void CEntityFriendComponent::UpdateFriendInfo(unsigned int nFriendID)
{
	if (nFriendID > 0 && m_pRoleAttr != NULL) 
    {
		GameMsg_S2G_RequestFriendOnlineInfo requestMsg;
		requestMsg.m_nSrcRoleID = m_pRoleAttr->GetRoleID();
		requestMsg.m_nDstRoleID = nFriendID;
		SendMsg2GroupServer(&requestMsg);
	}
}

void CEntityFriendComponent::PushOnlineInfoToAllFriends()
{
	if (m_pRoleAttr != NULL) {
		GameMsg_S2G_PushOnlineInfoToAllFriends requestMsg;
		requestMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
		SendMsg2GroupServer(&requestMsg);
	}
}

void CEntityFriendComponent::RequestAllFriendOnlineInfo()
{
    if ( m_pRoleAttr != NULL )
    {
        GameMsg_S2G_GetFriendOnlineInfo sendMsg( GetRoleAttr()->GetRoleID() );
        SendMsg2GroupServer( &sendMsg );
    }

	if (m_pRoleAttr != NULL) 
    {
		GameMsg_S2G_GetFriendOnlineInfo sendMsg = GameMsg_S2G_GetFriendOnlineInfo(GetRoleAttr()->GetRoleID());
		SendMsg2GroupServer(&sendMsg);
	}
}

void CEntityFriendComponent::AddRecentlyPlayMate(unsigned int nRoleID, const std::string& rName)
{
    if ( nRoleID == 0 || rName.empty() )
        return;

    FriendInfo frindInfo;
    frindInfo.m_nRoleID = nRoleID;
    frindInfo.SetName( rName );

    if ( find( m_listRecentlyPlayMate.begin(), m_listRecentlyPlayMate.end(), frindInfo ) == m_listRecentlyPlayMate.end() )
    {
        if ( (int)m_listRecentlyPlayMate.size() >= MAX_RECENTLY_PLAYMATE )
        {
            m_listRecentlyPlayMate.pop_front();
        }

        m_listRecentlyPlayMate.push_back( frindInfo );
    }
}

void CEntityFriendComponent::AddRecentlyChatMate(unsigned int nRoleID, const std::string& rName)
{
    if ( nRoleID == 0 || rName.empty() )
        return;

    FriendInfo frindInfo;
    frindInfo.m_nRoleID = nRoleID;
    frindInfo.SetName( rName );

    if ( find( m_listRecentlyChatMate.begin(), m_listRecentlyChatMate.end(), frindInfo ) == m_listRecentlyChatMate.end() )
    {
        if ( (int)m_listRecentlyChatMate.size() >= MAX_RECENTLY_CHATMATE )
        {
            m_listRecentlyChatMate.pop_front();
        }

        m_listRecentlyChatMate.push_back( frindInfo );
    }
}

void CEntityFriendComponent::CheckRecommendResetTime(unsigned int nNow)
{
    unsigned int nResetTime = m_nLastRecommendResetTime + CFriendDataManager::Instance().GetRecommendResetSeconds();

    if (nNow >= nResetTime)
    {
        m_nRecommendRefreshCount = CFriendDataManager::Instance().GetRecommendRefreshCount();
        m_nLastRecommendResetTime = nNow;

        if (m_pRoleAttr != NULL)
            AddQuery(QUERY_Recommend_UpdateResetTime, m_pRoleAttr->GetRoleID(), NULL, (__int64)m_nRecommendRefreshCount);
    }
}

void CEntityFriendComponent::RandomRecommendFriends(std::list<CFindPlayerInfo> &rFriends, unsigned char oppositeSex, unsigned int nMaxCount)
{
    rFriends.clear();

    std::vector<unsigned int> vectAllPlayersID;
    CRoomManager::Instance().GetLobbyPlayersID(vectAllPlayersID);

    if (nMaxCount == 0 || vectAllPlayersID.size() == 0 || m_pRoleAttr == NULL)
        return;

    CRoleEntity *pRoleEntity = NULL;
    CEntityAttributeComponent *pFriendAttrCom = NULL;
    std::vector<CRoleEntity *> vectPlayerRoleEntity;

    unsigned int i = 0;
    unsigned int nRoleID = 0;

    // find all players who meet the conditions
    for (i = 0; i < vectAllPlayersID.size(); ++i)
    {
        nRoleID = vectAllPlayersID[i];

        pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);

        if (pRoleEntity == NULL)
            continue;

        pFriendAttrCom = pRoleEntity->GetComponent<CRoleAttribute>();

        if (pFriendAttrCom != NULL && pFriendAttrCom->GetSex() != oppositeSex && 
            nRoleID != m_pRoleAttr->GetRoleID() && 
            m_mapFriend.find(nRoleID) == m_mapFriend.end() && 
            m_mapBlackList.find(nRoleID) == m_mapBlackList.end() )
        {
            vectPlayerRoleEntity.push_back(pRoleEntity);
        }
    }

    CEntityVIPComponent *pFriendVIPCom = NULL;
    unsigned int nSwapPos = 0;
    unsigned int nCount = 0;

    // get the player info by random
    for (i = 0, nCount = 0; nCount < nMaxCount && i < vectPlayerRoleEntity.size(); ++i)
    {
        nSwapPos = (unsigned int)Random(i, vectPlayerRoleEntity.size() - 1); // random one

        if (nSwapPos < vectPlayerRoleEntity.size() && nSwapPos != i)
        {
            std::swap(vectPlayerRoleEntity[i], vectPlayerRoleEntity[nSwapPos]);
        }

        if (vectPlayerRoleEntity[i] != NULL)
        {
            pFriendAttrCom = vectPlayerRoleEntity[i]->GetComponent<CRoleAttribute>();
            pFriendVIPCom = vectPlayerRoleEntity[i]->GetComponent<CRoleVIP>();

            if (pFriendAttrCom != NULL && pFriendVIPCom != NULL)
            {
                CFindPlayerInfo playerInfo;
                playerInfo.m_nRoleID = pFriendAttrCom->GetRoleID();
                playerInfo.m_strRoleName = pFriendAttrCom->GetRoleName();
                playerInfo.m_nLevel = pFriendAttrCom->GetLevel();
                playerInfo.m_nSex = pFriendAttrCom->GetSex();
                playerInfo.m_nConstellation = pFriendAttrCom->m_nConstellation;
                playerInfo.m_bIsVip = pFriendVIPCom->IsVIP();
                playerInfo.m_nVipLevel = (unsigned short)pFriendVIPCom->VIPLevel();

                rFriends.push_back(playerInfo);

                // 记录推荐过的好友
                if (m_setRecommendFriends.find(playerInfo.m_nRoleID) == m_setRecommendFriends.end() )
                    m_setRecommendFriends.insert(playerInfo.m_nRoleID);

                ++nCount;
            }
        }
    }
}

void CEntityFriendComponent::LogRoleFriends(unsigned char eAction, unsigned char eMode, unsigned int nFriendID)
{
    if (m_pRoleAttr == NULL || eAction >= CRoleFriendsLog::EFriendsAction_Max)
        return;

    CRoleFriendsLog *pLog = new CRoleFriendsLog;
    if (pLog != NULL)
    {
        pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
        pLog->m_nFriendID = nFriendID;
        pLog->m_eAction = (CRoleFriendsLog::EFriendsAction)eAction;
        pLog->m_eMode = CRoleFriendsLog::EFriendsMode_None;

        if (pLog->m_eAction == CRoleFriendsLog::EFriendsAction_Add)
        {
            switch (eMode)
            {
            case CRoleFriendsLog::EFriendsMode_Add_Normal:
            case CRoleFriendsLog::EFriendsMode_Add_Recommend:
            case CRoleFriendsLog::EFriendsMode_Add_Black:
                pLog->m_eMode = (CRoleFriendsLog::EFriendsMode)eMode;
                break;
            }
        }
        else if (pLog->m_eAction == CRoleFriendsLog::EFriendsAction_Delete)
        {
            switch (eMode)
            {
            case CRoleFriendsLog::EFriendsMode_Del_Normal:
            case CRoleFriendsLog::EFriendsMode_Del_ToBlack:
            case CRoleFriendsLog::EFriendsMode_Del_FromBlack:
                pLog->m_eMode = (CRoleFriendsLog::EFriendsMode)eMode;
                break;
            }
        }

        AddQuery(QUERY_LogRoleAddFriends, 0, pLog);
    }
}

void CEntityFriendComponent::FriendLogout(unsigned int nFriendRoleID)
{
    std::map<unsigned int,FriendInfo*>::iterator it = m_mapFriend.find(nFriendRoleID);
    if (it != m_mapFriend.end())
    {
        it->second->m_bIsOnline = false;
        it->second->m_nOfflineTime = (unsigned int)time(NULL);
    }
}

void CEntityFriendComponent::FriendLogin(unsigned int nFriendRoleID)
{
    std::map<unsigned int,FriendInfo*>::iterator it = m_mapFriend.find(nFriendRoleID);
    if (it != m_mapFriend.end())
    {
        it->second->m_bIsOnline = true;
        it->second->m_nOfflineTime = 0;
    }
}

void CEntityFriendComponent::UpdateFriendOnlineState()
{
    std::map<unsigned int,FriendInfo*>::iterator it = m_mapFriend.begin();
    for (; it != m_mapFriend.end(); ++it)
    {
        CRoleEntity * pFriendEntity = CPlayerManager::Instance().GetEntityByRoleID(it->first);
        if (pFriendEntity != NULL)
        {
            it->second->m_bIsOnline = true;
        }
    }
}

void CEntityFriendComponent::NoticeFriendLogin()
{
    std::map<unsigned int,FriendInfo*>::iterator it = m_mapFriend.begin();
    for (; it != m_mapFriend.end(); ++it)
    {
        CRoleEntity * pFriendEntity = CPlayerManager::Instance().GetEntityByRoleID(it->first);
        if (pFriendEntity != NULL)
        {
            pFriendEntity->GetComponent<CRoleFriend>()->FriendLogin(m_pRoleAttr->GetRoleID());
        }
    }
}
