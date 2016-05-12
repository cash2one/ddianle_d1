#include "WAttributeManager.h"
#include "../../socket/SlotPeer.h"
#include "../../socket/GameMsg_Map.h"
#include "../group/GroupServerCirculator.h"
#include "../group/Global.h"
#include "../share/ServerMsgDef.h"
#include "../Couple/CoupleProcessor.h"

#include "../player/GameMsg_S2G_UpdateAttrInfo.h"
#include "../player/GameMsg_S2G_UpdateRoomInfo.h"
#include "../player/GameMsg_S2G_PlayerInfo.h"
#include "../player/GameMsg_G2S_PlayerInfo.h"
#include "../Couple/GameMsg_G2S_Couple.h"

#include "../room/GroupRoomPrecessor.h"


CWAttributeManager::CWAttributeManager():
GameMsg_Processor(true, true)
{
}

CWAttributeManager::~CWAttributeManager()
{
}

CWAttributeManager& CWAttributeManager::Instance()
{
	static CWAttributeManager stcAttrMgr;
	return stcAttrMgr;
}

void CWAttributeManager::Initialize()
{
	RegisterMessage();
	RegisterMessageProcessor();
}

void CWAttributeManager::RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateAttrInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateRoomInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateRoleLocation);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateRoleName);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateLevel);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateMoney);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetPlayerInfo_Forward);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetPlayerInfo_Backward);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateRoleAge);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetOfflinePlayerInfo);
}

void CWAttributeManager::RegisterMessageProcessor()
{
	RegMsgProcessor(MSG_S2G_UpdateAttrInfo, &CWAttributeManager::OnUpdateRoleInfo);
	RegMsgProcessor(MSG_S2G_UpdateRoomInfo, &CWAttributeManager::OnUpdateRoomInfo);
	RegMsgProcessor(MSG_S2G_UpdateRoleLocation, &CWAttributeManager::OnUpdateRoleLocation);
	RegMsgProcessor(MSG_S2G_UpdateRoleName, &CWAttributeManager::OnUpdateRoleName);
	RegMsgProcessor(MSG_S2G_UpdateRoleLevel, &CWAttributeManager::OnUpdateRoleLevel);
	RegMsgProcessor(MSG_S2G_UpdateRoleMoney, &CWAttributeManager::OnUpdateRoleMoney);
	RegMsgProcessor(MSG_S2G_GetPlayerInfo_Forward, &CWAttributeManager::OnGetPlayerInfoRequest);
	RegMsgProcessor(MSG_S2G_GetPlayerInfo_Backward, &CWAttributeManager::OnGetPlayerInfoReply);
    RegMsgProcessor(MSG_S2G_UpdateRoleAge, &CWAttributeManager::OnUpdateRoleAge);
    RegMsgProcessor(MSG_S2G_GetOfflinePlayerInfo, &CWAttributeManager::GetOfflinePlayerInfoBack);
}

void CWAttributeManager::OnUpdateRoleInfo(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_UpdateAttrInfo *pMsg = (GameMsg_S2G_UpdateAttrInfo *)&msg;

	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_RoleUpdateAttrInfo.m_nRoleID);
		if (pRole != NULL)
		{
			pRole->UpdateRoleInfo(pMsg->m_RoleUpdateAttrInfo);
		}
	}
}

void CWAttributeManager::OnUpdateRoomInfo(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_UpdateRoomInfo *pMsg = (GameMsg_S2G_UpdateRoomInfo *)&msg;

	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL)
		{
			pRole->UpdateRoomInfo(pMsg->m_RoomUpdateInfo);
		}
	}	
}

void CWAttributeManager::OnUpdateRoleName(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_UpdateRoleName	*pMsg = (GameMsg_S2G_UpdateRoleName *)&msg;
	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL)
		{
			pRole->SetRoleName(pMsg->m_szName);
			CGroupRoleEntity *pCouple = CGroupPlayerManager::Instance().SearchRoleByID(pRole->CoupleID());
			if (pCouple != NULL) 
			{
				std::string strName = pMsg->m_szName;
				pCouple->SetCoupleName(strName);

				GameMsg_G2S_CoupleNameChanged msgNotify;
				msgNotify.m_strCoupleName = pMsg->m_szName;
				pCouple->SendPlayerMsg(&msgNotify);
			}

			// sync couple
			std::string strNickname = pMsg->m_szName;
			CCoupleProcessor::Instance().SyncChangeNickname( pRole->PairID(), pMsg->m_nRoleID, strNickname );

            GroupRoomProcessor::Instance().OnRoleNameChanged(pRole, strNickname);
        }
	}
}

void CWAttributeManager::OnUpdateRoleLevel(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_UpdateLevel	*pMsg = (GameMsg_S2G_UpdateLevel *)&msg;
	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL)
		{
			pRole->SetLevel(pMsg->m_nLevel);
		}
	}
}

void CWAttributeManager::OnUpdateRoleMoney(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_UpdateMoney	*pMsg = (GameMsg_S2G_UpdateMoney *)&msg;
	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL)
		{
			pRole->SetMoney(pMsg->m_nMoney);
		}
	}
}


void CWAttributeManager::OnUpdateRoleLocation(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_UpdateRoleLocation	*pMsg = (GameMsg_S2G_UpdateRoleLocation *)&msg;
	if (pMsg != NULL)
	{
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
		if (pRole != NULL)
		{
			pRole->SetRoleLocation(pMsg->m_nRoleLocation);
		}
	}
}

void CWAttributeManager::OnGetPlayerInfoRequest(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_GetPlayerInfo_Forward *pMsg = (GameMsg_S2G_GetPlayerInfo_Forward *)&msg;	

	if (pMsg != NULL && pMsg->m_nSrcID > 0 && pMsg->m_nQueryID > 0) {
		CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nSrcID);
		if (pRole != NULL) {
			CGroupRoleEntity *pQuery = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nQueryID);
			if (pQuery != NULL) {
				GameMsg_G2S_GetPlayerInfo_Forward forwardMsg(pMsg->m_nSrcID, pMsg->m_nQueryID); 
				pQuery->SendPlayerMsg(&forwardMsg);
			} else {
				GameMsg_G2S_GetPlayerInfo_Backward backMsg;
				pRole->SendPlayerMsg(&backMsg);
			}
		}
	}
}

void CWAttributeManager::OnGetPlayerInfoReply(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_GetPlayerInfo_Backward *pMsg = (GameMsg_S2G_GetPlayerInfo_Backward *)&msg;

	CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nSrcID);
	if (pRole != NULL) {
		GameMsg_G2S_GetPlayerInfo_Backward backMsg(pMsg->m_playerInfo);
		pRole->SendPlayerMsg(&backMsg);
	}
}

void CWAttributeManager::OnUpdateRoleAge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_UpdateRoleAge *pMsg = (GameMsg_S2G_UpdateRoleAge *)&msg;

    if (NULL != pMsg)
    {
        CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);

        if (NULL != pRole)
        {
            pRole->SetRoleAge(pMsg->m_nAge);
            pRole->SetRoleConstellation(pMsg->m_nConstellation);
        }
    }
}

void CWAttributeManager::GetOfflinePlayerInfo(unsigned int nOnlineRoleID, unsigned int nGainRoleID, EOfflinePlayerDataFrom eFrom)
{
    CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( nOnlineRoleID );
    if (pRole != NULL)
    {
        GameMsg_G2S_GetOfflinePlayerInfo g2sOnlineMsg;
        g2sOnlineMsg.m_eFrom = eFrom;
        g2sOnlineMsg.m_nSelfRoleID = nGainRoleID;

        pRole->SendPlayerMsg( &g2sOnlineMsg );
    }
}

void CWAttributeManager::GetOfflinePlayerInfoBack(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetOfflinePlayerInfo & s2gmsg = (GameMsg_S2G_GetOfflinePlayerInfo&)msg;

    switch(s2gmsg.m_eFrom)
    {
    case EOfflinePlayerDataFrom_Couple:
    case EOfflinePlayerDataFrom_Divorce:
        {
            CCoupleProcessor::Instance().OnGetOnlinePersonInfo(msg, slotPeer);
        }
        break;
    case EOfflinePlayerDataFrom_Logout:
        {
            CGroupOffLineRoleEntity * pOfflineRole = CGroupOffLinePlayerManager::Instance().GetOfflineRoleImageData(s2gmsg.m_nRoleID);
            if (pOfflineRole != NULL)
            {
                pOfflineRole->UpdateOfflineData(s2gmsg.m_OffLineData);
            }
        }
        break;
    case EOfflinePlayerDataFrom_None:
    default: break;
    }
}

// end of file

