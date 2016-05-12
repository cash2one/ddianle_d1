#include "../group/GroupServerCirculator.h"
#include "GroupPrivateChat.h"
#include "../chat/GameMsg_S2G_Chat.h"
#include "../chat/GameMsg_G2S_Chat.h"
#include "../group/Global.h"

CGroupPrivateChat::CGroupPrivateChat()
{
}

CGroupPrivateChat::~CGroupPrivateChat()
{
}

void CGroupPrivateChat::OnGroupChatMessage(GameMsg_Base *pMsg)
{
} 

void CGroupPrivateChat::OnChat(GameMsg_Base *pMsg)
{
	if (pMsg != NULL){
		GameMsg_S2G_Chat *pChatMsg = (GameMsg_S2G_Chat *)pMsg;
		if (pChatMsg != NULL && pChatMsg->m_nSenderRoleID > 0 && pChatMsg->m_nReceiverRoleID > 0){
			
			unsigned char nPhase = pChatMsg->m_nPhase;
			unsigned char nResult = pChatMsg->m_nResult;
			int nLine = -1;
			__int64 nSessionID = pChatMsg->SessionID;
			int nIndex = pChatMsg->nIndex;
			string strRcvName = "";
			unsigned char nclickSex = 0;

			CGroupRoleEntity * pSenderRole = CGroupPlayerManager::Instance().SearchRoleByID(pChatMsg->m_nSenderRoleID);
			CGroupRoleEntity * pReceiverRole = CGroupPlayerManager::Instance().SearchRoleByID(pChatMsg->m_nReceiverRoleID);
			if (pChatMsg->m_nPhase == eChatPhrase_Forward){
				if (pReceiverRole == NULL) {
					nPhase = eChatPhrase_Backward;
					nResult = Chat_ePlayerNotExisted;
					if (pSenderRole != NULL){
						nLine = pSenderRole->m_nLine;
					}
				} else {
					nLine = pReceiverRole->m_nLine;
					nSessionID = pReceiverRole->m_nSession;
					nIndex = pReceiverRole->m_nIndex;
					strRcvName = pReceiverRole->GetName();
					if (pSenderRole != NULL) {
						nclickSex = pSenderRole->GetSex();
					}
				}
			} else {
				CGroupRoleEntity * pSenderRole = CGroupPlayerManager::Instance().SearchRoleByID(pChatMsg->m_nSenderRoleID);
				strRcvName = pChatMsg->m_strReceiverName;
				if (pSenderRole != NULL){
					nLine = pSenderRole->m_nLine;
					nSessionID = pSenderRole->m_nSession;
					nIndex = pSenderRole->m_nIndex;
					if (pReceiverRole != NULL) {
						nclickSex = pReceiverRole->GetSex();
					}
				}
			}
			
			GameMsg_G2S_Chat g2sMsg(nPhase, nResult, pChatMsg->m_nChannel, pChatMsg->m_nColor,
				pChatMsg->m_nSenderRoleID, pChatMsg->m_nReceiverRoleID,
				pChatMsg->m_strSenderName.c_str(), strRcvName.c_str(),
				pChatMsg->m_strText.c_str(), nclickSex, EChatExtraType_Default,
				pChatMsg->m_bIsVIP, pChatMsg->m_nVIPLevel, pChatMsg->m_bVoiceChat,pChatMsg->m_nWorldChatFlag);

			g2sMsg.SessionID = nSessionID;
            g2sMsg.nIndex = (unsigned short)nIndex;
			if (nLine != -1){
				SendMsg2GameServer(&g2sMsg, nLine);
			}
		}
	}
}


//file end

