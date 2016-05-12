#include "Chat.h"
//#include "../../datastructure/serverRoleEntity.h"
#include "../logic/PlayerManager.h"

//发送消息给GroupServer
extern void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CChatManager g_ChatManager;

CChatManager::CChatManager()
{
}

CChatManager::~CChatManager()
{
}

//只处理来自GroupServer的聊天消息
//void CChatManager::OnGroupMessage(GameMsg_Base* pMsg)
//{
	//switch(pMsg->nMsg) {
	//case MSG_G2S_ChatForward:
	//	((GameMsg_G2S_ChatForward*)pMsg)->Parse();
	//	ProcessChat(((GameMsg_G2S_ChatForward*)pMsg)->pChatMsg,((GameMsg_G2S_ChatForward*)pMsg)->nDestRoleIndex);
	//	break;
	//case MSG_G2S_ChatBackward:
	//	((GameMsg_G2S_ChatBackward*)pMsg)->Parse();
	//	ProcessChatResult(((GameMsg_G2S_ChatBackward*)pMsg)->nResult, ((GameMsg_G2S_ChatBackward*)pMsg)->pChatMsg,((GameMsg_G2S_ChatBackward*)pMsg)->nDestRoleIndex);
	//	break;
	//}
//}


//void CChatManager::ProcessChat(GameMsg_CHAT* pMsg,unsigned short nIndexDestRole)
//{
//	eChatChannel nChannel = (eChatChannel)pMsg->nChannel;
//	
//	//私聊
//	if(nChannel == eChatPrivate)
//	{
//		CServerRoleEntity* pPlayer = NULL;
//		if (nIndexDestRole > 0)
//		{
//			g_pPlayerManager->GetPlayer(nIndexDestRole,pMsg->nEntityIDReceiver);
//		}
//		if (pPlayer == NULL)
//		{
//			pPlayer = g_pPlayerManager->GetEntityByRoleID(pMsg->nEntityIDReceiver);
//		}
//		if(pPlayer&&pPlayer->IsValid())
//		{
//			int nReuslt;
//			if(pPlayer->HasBlackList(ROLEINFO_BASE(pMsg->nEntityID)))
//				nReuslt = ChatResultType_Squelched;
//			else
//			{
//				//聊天改造
//				GameMsg_S2C_Chat_Priv *pChatMsg2Client = NULL;
//				pChatMsg2Client = GameMsg_S2C_Chat_Priv::Create(pMsg->szSenderName,pMsg->nEntityID,pMsg->szContext,pMsg->dwColor[0],pMsg->dwColor[1]);
//				if (pChatMsg2Client)
//				{
//					SendGameMsg(pChatMsg2Client, &pPlayer->m_RoleTag);
//				}
//				delete pChatMsg2Client;
//
//				//发消息到接收者
//				//SendGameMsg(pMsg, &pPlayer->m_RoleTag);
//
//				//设置返回groupserver的代码
//				nReuslt = ChatResultType_Success;
//			}
//
//			GameMsg_S2G_ChatBackward* pBackMsg;
//			pBackMsg = GameMsg_S2G_ChatBackward::Create(nReuslt, pMsg);
//			SendMsgToGroupServer(pBackMsg);
//			delete pBackMsg;
//		}
//		else
//		{
//			GameMsg_S2G_ChatBackward* pBackMsg;
//			pBackMsg = GameMsg_S2G_ChatBackward::Create(ChatResultType_PlayerNotFound, pMsg);
//			SendMsgToGroupServer(pBackMsg);
//			delete pBackMsg;
//		}
//	}
//
//	//家族聊天
//	//if(nChannel == eChatFamily)
//	//{
//	//	//收到GroupServer发过来的家族聊天消息
//
//
//
//	//}
//
//	//世界频道 或者是 全区公告  
//	if(nChannel == eChatWorld || nChannel == eChatAdmin)
//	{
//		//聊天改造
//		GameMsg_S2C_Chat *pChatMsg2Client = NULL;
//		pChatMsg2Client = GameMsg_S2C_Chat::Create(pMsg->szSenderName,pMsg->nEntityID,(eChatChannel)pMsg->nChannel,pMsg->szContext,pMsg->dwColor[0],pMsg->dwColor[1]);
//		if (pChatMsg2Client&&g_pPlayerManager)
//		{
//			g_pPlayerManager->NotifyAllPlayerMsg(pChatMsg2Client);
//			g_pPlayerManager->SavaChatContent(pMsg->szSenderName,pMsg->nEntityID,(eChatChannel)pMsg->nChannel,pMsg->szContext,pMsg->dwColor[0],pMsg->dwColor[1]);
//
//		}
//		delete pChatMsg2Client;
//
//		//收到 GroupServer发过来的世界频道聊天消息，只需在本线广播执行即可，合法性判断在先前已经做过了
//		//g_pPlayerManager->NotifyAllPlayerMsg(pMsg);
//	}
//}

//void CChatManager::ProcessChatResult(int nResult, GameMsg_CHAT* pMsg,unsigned short nIndexDestRole)
//{
//	eChatChannel nChannel = (eChatChannel)pMsg->nChannel;
//
//	if(nChannel == eChatPrivate)
//	{
//		CServerRoleEntity* pPlayer = NULL;
//		if (nIndexDestRole > 0)
//		{
//			g_pPlayerManager->GetPlayer(nIndexDestRole,pMsg->nEntityID);
//		}
//		if (pPlayer == NULL)
//		{
//			pPlayer = g_pPlayerManager->GetEntityByRoleID(pMsg->nEntityID);
//		}
//		if(pPlayer&&pPlayer->IsValid())
//		{
//			switch(nResult) {
//			case ChatResultType_Success:
//				{
//					//聊天改造
//					GameMsg_S2C_Chat_PrivSelf *pChatMsg2Self = NULL;
//					pChatMsg2Self = GameMsg_S2C_Chat_PrivSelf::Create(pMsg->szReceiverName,pMsg->szContext,pMsg->dwColor[0],pMsg->dwColor[1]);
//					if (pChatMsg2Self)
//					{
//						SendGameMsg(pChatMsg2Self, &pPlayer->m_RoleTag);
//					}
//					delete pChatMsg2Self;
//
//
//					//SendGameMsg(pMsg, &pPlayer->m_RoleTag);
//				}	
//				break;
//			case ChatResultType_ChatShutOff:
//				pPlayer->SysChat2("玩家 %s 目前处于不接受私聊状态。", pMsg->szReceiverName);
//				break;
//			case ChatResultType_Squelched:
//				pPlayer->SysChat2("您被 %s 列于黑名单中，无法向其发送私聊信息。", pMsg->szReceiverName);
//				break;
//			case ChatResultType_PlayerNotFound:
//				pPlayer->SysChat2("该玩家不在线或不存在，不能发送私聊消息。");
//				break;
//			}
//		}
//	}
//}
//



