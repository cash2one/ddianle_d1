#include "Chat.h"
//#include "../../datastructure/serverRoleEntity.h"
#include "../logic/PlayerManager.h"

//������Ϣ��GroupServer
extern void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CChatManager g_ChatManager;

CChatManager::CChatManager()
{
}

CChatManager::~CChatManager()
{
}

//ֻ��������GroupServer��������Ϣ
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
//	//˽��
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
//				//�������
//				GameMsg_S2C_Chat_Priv *pChatMsg2Client = NULL;
//				pChatMsg2Client = GameMsg_S2C_Chat_Priv::Create(pMsg->szSenderName,pMsg->nEntityID,pMsg->szContext,pMsg->dwColor[0],pMsg->dwColor[1]);
//				if (pChatMsg2Client)
//				{
//					SendGameMsg(pChatMsg2Client, &pPlayer->m_RoleTag);
//				}
//				delete pChatMsg2Client;
//
//				//����Ϣ��������
//				//SendGameMsg(pMsg, &pPlayer->m_RoleTag);
//
//				//���÷���groupserver�Ĵ���
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
//	//��������
//	//if(nChannel == eChatFamily)
//	//{
//	//	//�յ�GroupServer�������ļ���������Ϣ
//
//
//
//	//}
//
//	//����Ƶ�� ������ ȫ������  
//	if(nChannel == eChatWorld || nChannel == eChatAdmin)
//	{
//		//�������
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
//		//�յ� GroupServer������������Ƶ��������Ϣ��ֻ���ڱ��߹㲥ִ�м��ɣ��Ϸ����ж�����ǰ�Ѿ�������
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
//					//�������
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
//				pPlayer->SysChat2("��� %s Ŀǰ���ڲ�����˽��״̬��", pMsg->szReceiverName);
//				break;
//			case ChatResultType_Squelched:
//				pPlayer->SysChat2("���� %s ���ں������У��޷����䷢��˽����Ϣ��", pMsg->szReceiverName);
//				break;
//			case ChatResultType_PlayerNotFound:
//				pPlayer->SysChat2("����Ҳ����߻򲻴��ڣ����ܷ���˽����Ϣ��");
//				break;
//			}
//		}
//	}
//}
//



