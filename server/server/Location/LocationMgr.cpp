#include "../share/ServerMsgDef.h"
#include "../../socket/GameMsg_Map.h"
#include "LocationMgr.h"
#include "GameMsg_Location.h"
#include "../../datastructure/RoleEntity.h"
#include "../logic/PlayerManager.h"
#include "../friend/EntityFriendComponent.h"


CLocationMgr::CLocationMgr()
:GameMsg_Processor(true, true)
{

}

CLocationMgr::~CLocationMgr()
{

}

CLocationMgr & CLocationMgr::Instance()
{
	static CLocationMgr stcLocationMgr;
	return stcLocationMgr;
}


void CLocationMgr::AddEntity(int nLocation, CRoleEntity *pEntity)
{
	if (nLocation > Zone_None && nLocation < Zone_Max) {
		if (pEntity != NULL) {
			m_mapZone[nLocation].insert(make_pair(pEntity->GetRoleID(), pEntity));	
		}
	}
}

CRoleEntity * CLocationMgr::RemoveEntity(int nLocation, unsigned int nRoleID)
{
	CRoleEntity *pEntity = NULL;

	if (nLocation > Zone_None && nLocation < Zone_Max) {
		TMapZone::iterator itr = m_mapZone[nLocation].find(nRoleID);
		if (itr != m_mapZone[nLocation].end()) {
			pEntity = itr->second;	
			m_mapZone[nLocation].erase(itr);
		}
	}

	return pEntity;
}

void CLocationMgr::Initialize()
{
	RegisterMessage();
	RegisterMessageProcessor();
}

void CLocationMgr::PlayerLogout(int nLocation, unsigned int nRoleID)
{
	if (nLocation > Zone_None && nLocation < Zone_Max) {
		RemoveEntity(nLocation, nRoleID);
	}
}

void CLocationMgr::RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_EnterZone);
}

void CLocationMgr::RegisterMessageProcessor()
{
	RegMsgProcessor(MSG_C2S_EnterZone, &CLocationMgr::OnEnterZone);
}

void CLocationMgr::OnEnterZone(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_C2S_EnterZone & msgEnter = (GameMsg_C2S_EnterZone &)msg;

	if (msgEnter.m_nLocation < Zone_Max) {
		EnterZone(msgEnter.Location(), msgEnter.nIndex);
	}
}

void CLocationMgr::EnterZone(int nLocation, unsigned int nRoleID)
{
	if (nLocation > Zone_None && nLocation < Zone_Max) {
		CRoleEntity *pEnter = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
		if (pEnter != NULL) {
			RemoveEntity(pEnter->GetLocation(), pEnter->GetRoleID());
			pEnter->SetLocation(nLocation);
			AddEntity(nLocation, pEnter);
		}
	}
}

void CLocationMgr::EnterZone(int nLocation, unsigned short nIndex)
{
	if (nLocation > Zone_None && nLocation < Zone_Max) {
		CRoleEntity *pEnter = CPlayerManager::Instance().GetPlayer(nIndex);
		if (pEnter != NULL) {
			RemoveEntity(pEnter->GetLocation(), pEnter->GetRoleID());
			pEnter->SetLocation(nLocation);
			AddEntity(nLocation, pEnter);
		}
	}
}

bool CLocationMgr::IsInZone(int nLocation, unsigned int nRoleID)
{
	if (nLocation > Zone_None && nLocation < Zone_Max) {
		return m_mapZone[nLocation].find(nRoleID) != m_mapZone[nLocation].end();
	}

	return false;
}

void CLocationMgr::SendLocationChatMsg(int nLocation, unsigned int nSenderID, GameMsg_Base *pMsg)
{
	if (nLocation > Zone_None && nLocation < Zone_Max) {
		for (TMapZone::iterator itr = m_mapZone[nLocation].begin(); itr != m_mapZone[nLocation].end(); ++itr) {
			if (itr->second != NULL && !itr->second->IsInBlackList(nSenderID)) {
				itr->second->SendPlayerMsg(pMsg);
			}
		}
	}
}






