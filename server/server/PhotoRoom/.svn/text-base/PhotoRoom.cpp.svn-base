#include <algorithm> 
#include "../chat/EntityChatComponent.h"
#include "PhotoRoom.h"
#include "EntityPhotoRoomComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../../datastructure/datainfor/PhotoRoomConfig.h"
#include "../../datastructure/datainfor/PhotoRoomConfigManager.h"
#include "PhotoRoomDataStruct.h"
#include "PhotoRoomLuaDataManager.h"
#include "GameMsg_S2C_PhotoRoom.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/datainfor/GMDataInfo.h"
#include "../../socket/Formatter.h"

CPhotoRoom::CPhotoRoom()
:m_nRoomID(0)
,m_strRoomName("")
,m_nHostID(0)
,m_nCreatedTime(0)
,m_nExistTime(0)
,m_eRoomState(EPhotoRoomState_None)
,m_eRoomTheme(EPhotoRoomTheme_None)
,m_nSelectedBG(0)	
,m_nSelectedAction(0)
{

}

CPhotoRoom::~CPhotoRoom()
{

}

void CPhotoRoom::SetRoomID(unsigned int nRoomID)
{
	m_nRoomID = nRoomID;
}

unsigned int CPhotoRoom::RoomID() const
{
	return m_nRoomID;
}

void CPhotoRoom::SetRoomName(std::string & strRoomName)
{
	m_strRoomName = strRoomName;
}

const std::string & CPhotoRoom::RoomName() const
{
	return m_strRoomName;
}

void CPhotoRoom::SetHostID(unsigned int nHostID)
{
	m_nHostID = nHostID;
}

unsigned int CPhotoRoom::HostID() const
{
	return m_nHostID;
}

void CPhotoRoom::SetCreateTime(unsigned int nCreatedTime)
{
	m_nCreatedTime = nCreatedTime;
}

unsigned int CPhotoRoom::CreateTime() const
{
	return m_nCreatedTime;
}

void CPhotoRoom::SetExistTime(unsigned int nExistTime)
{
	m_nExistTime = nExistTime;
}

unsigned int CPhotoRoom::ExistTime()
{
	return m_nExistTime;
}

int CPhotoRoom::RemainTime()
{
	int nTimeRemain = (int)m_nCreatedTime + m_nExistTime - (int)time(NULL) ;

	return nTimeRemain > 0 ? nTimeRemain : 0;
}

void CPhotoRoom::SetRoomTheme(EPhotoRoomTheme nRoomTheme )
{
	m_eRoomTheme = nRoomTheme;
}

EPhotoRoomTheme CPhotoRoom::RoomTheme() const
{
	return m_eRoomTheme;
}

void CPhotoRoom::SetRoomState(EPhotoRoomState eState)
{
	if (eState > EPhotoRoomState_None && eState < EPhotoRoomState_Max) {
		m_eRoomState = eState;
	}
}

EPhotoRoomState CPhotoRoom::RoomState() const
{
	return m_eRoomState;	
}

void CPhotoRoom::SetSelectedBG(unsigned int nSelectedBGID)
{
	m_nSelectedBG = nSelectedBGID;
}

unsigned int CPhotoRoom::SelectedBG()
{
	return m_nSelectedBG;
}

void CPhotoRoom::SetSelectedAction(unsigned int nActionID)
{
	m_nSelectedAction = nActionID;
}

unsigned int CPhotoRoom::SelectedAction()
{
	return m_nSelectedAction;
}

void CPhotoRoom::AddRoomBG(const PhotoRoomConfig *pBGInfo)
{
	if (pBGInfo != NULL) {
		m_mapRoomPhotoBG.insert(std::make_pair(pBGInfo->m_nID, pBGInfo));
	}
}

bool CPhotoRoom::IsInRoomBGList(unsigned int nBGID) const
{
	return m_mapRoomPhotoBG.find(nBGID) != m_mapRoomPhotoBG.end() ? true : false;
}

void CPhotoRoom::AddRoomAction(const PhotoRoomGroupAction *pActionInfo)
{
	if (pActionInfo != NULL) {
		m_mapRoomAction.insert(std::make_pair(pActionInfo->m_nID, pActionInfo));
	}
}

bool CPhotoRoom::IsInRoomActionList(unsigned int nActionID) const
{
	return m_mapRoomAction.find(nActionID) != m_mapRoomAction.end() ? true : false;
}

int CPhotoRoom::PlayerCount()
{
	return (int)m_listPlayers.size();
}

void CPhotoRoom::EncodeRoomInfo(CPhotoRoomWholeInfo & rWholeRoomInfo, bool bHost)
{
	rWholeRoomInfo.m_nRoomID = m_nRoomID;
	rWholeRoomInfo.m_strRoomName = m_strRoomName;
	rWholeRoomInfo.m_nRoomType = m_eRoomTheme;
	__int64 nRemainTime = m_nCreatedTime + m_nExistTime - (int)time(NULL);
	rWholeRoomInfo.m_nPhotoRoomTimeRemaind = nRemainTime > 0 ? (int)nRemainTime : 0;
	rWholeRoomInfo.m_nSelectedBGID = (unsigned short)m_nSelectedBG;
	for (std::map<unsigned int, const PhotoRoomConfig *>::iterator itr = m_mapRoomPhotoBG.begin(); itr != m_mapRoomPhotoBG.end(); ++itr) {
		if (itr->second != NULL) {
			rWholeRoomInfo.m_listOwnedBG.push_back((unsigned char)(itr->second->m_nID));
		}
	}

	for (std::map<unsigned int, const PhotoRoomGroupAction *>::iterator itr = m_mapRoomAction.begin(); itr != m_mapRoomAction.end(); ++itr) {
		if (itr->second != NULL) {
			rWholeRoomInfo.m_listOwnedGroupAction.push_back((unsigned char)(itr->second->m_nID));
		}
	}

	for (PhotoRoomPlayerList_t::const_iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
		if (*itr != NULL) {
			 CPhotoRoomPlayerInfo photoRoomPlayer;
			(*itr)->EncodePhotoRoomPlayerInfo(photoRoomPlayer);
			rWholeRoomInfo.m_listPlayers.push_back(photoRoomPlayer);
		}
	}

	const std::vector<CPhotoRoomNPCInfo>* pVecPhotoRoomNPCInfo = CPhotoRoomLuaDataManager::Instance().GetPhotoRoomNPCInfo(RoomTheme());
	if (pVecPhotoRoomNPCInfo)
	{
		rWholeRoomInfo.m_listNPCs.insert(rWholeRoomInfo.m_listNPCs.begin(), pVecPhotoRoomNPCInfo->begin(), pVecPhotoRoomNPCInfo->end());
	}
}

const CPhotoRoom::PhotoRoomPlayerList_t & CPhotoRoom::PlayerList()
{
	return m_listPlayers;
}

bool CPhotoRoom::AddPhotoRoomPlayer(CEntityPhotoRoomComponent *pRolePhotoStudio)
{
	bool bRes = false;
	const PhotoRoomTheme *pPhotoTheme = PhotoRoomConfigManager::Instance().GetPhotoRoomTheme(m_eRoomTheme);
	if (pPhotoTheme != NULL) {
		if (pRolePhotoStudio != NULL && pRolePhotoStudio->RoleAttr() != NULL && m_listPlayers.size() < pPhotoTheme->MaxPlayerCount()) {
			int nCount = 0;
			for (PhotoRoomPlayerList_t::iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
				if (*itr != NULL && (*itr)->RoleAttr()->GetRoleID() != m_nHostID) {
					++nCount;
				}
			}

			if (nCount < pPhotoTheme->MaxPlayerCount() - 1) { // not include host
				bRes = true;
			} else if (pRolePhotoStudio->RoleAttr()->GetRoleID() == m_nHostID) {
				bRes = true;
			}
			if (bRes) {
				if (pRolePhotoStudio->RoleAttr()->GetRoleID() == m_nHostID) {
					pRolePhotoStudio->SetHost(true);
					m_listPlayers.push_front(pRolePhotoStudio);
				} else {
					m_listPlayers.push_back(pRolePhotoStudio);
				}
			}
		}
	}


	return bRes;
}

CEntityPhotoRoomComponent *CPhotoRoom::RemovePhotoRoomPlayer(unsigned int nRoleID)
{
	for (PhotoRoomPlayerList_t::iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
		if (*itr != NULL && (*itr)->RoleAttr() != NULL) {
			if ((*itr)->RoleAttr()->GetRoleID() == nRoleID) {
				 CEntityPhotoRoomComponent *pRole = *itr;
				m_listPlayers.erase(itr);
				return pRole;
			}
		}
	}

	return NULL;
}

CEntityPhotoRoomComponent * CPhotoRoom::GetPlayer(unsigned int nRoleID) const
{
	for (PhotoRoomPlayerList_t::const_iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
		if (*itr != NULL && (*itr)->RoleAttr() != NULL) {
			if ((*itr)->RoleAttr()->GetRoleID() == nRoleID) {
				return *itr;
			}
		}
	}

	return NULL;
}

const CPhotoRoom::PhotoRoomPlayerList_t & CPhotoRoom::PhotoRoomPlayerMap() const
{
	return m_listPlayers;
}

void CPhotoRoom::OnRoomExpiredNotification(int nLevel)
{
	const CPhotoRoomNotifiationTimer * pTimer = CPhotoRoomLuaDataManager::Instance().GetPhotoRoomNotificationTimer(nLevel);
	if (pTimer != NULL) {
		GameMsg_S2C_PhotoRoomExpiredNotification msgNotification;
		msgNotification.m_nTimeRemain = pTimer->m_nTimeBeforeExpired;
		msgNotification.m_eNotifyType = (ERoomExpiredNotiyType)pTimer->m_nNotifcationType;
		SendMsgToAll(&msgNotification);

		std::string strContent;
		SafeFormat(strContent, CLocalization::Instance().GetString("Photo_Room_Expired_Notification"), pTimer->m_nTimeBeforeExpired / (60));
		for (PhotoRoomPlayerList_t::iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
			if (*itr != NULL) {
				if ((*itr)->RoleChat() != NULL) {
					(*itr)->RoleChat()->SysChat(strContent.c_str());
				}
			}
		}
		WriteLog(LOGLEVEL_DEBUG, "Send photo room notification time[%d], notifytype[%d]",pTimer->m_nTimeBeforeExpired, pTimer->m_nNotifcationType);
	}
}

void CPhotoRoom::DestroyRoom()
{
	for (PhotoRoomPlayerList_t::iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ) {
		CEntityPhotoRoomComponent *pRolePhotoStudio = *itr;	
		m_listPlayers.erase(itr++);
		if (pRolePhotoStudio != NULL) {
			pRolePhotoStudio->OutPhotoRoom();
		}
	}
}

void CPhotoRoom::SendMsgToAll(GameMsg_Base *pMsg)
{
	if (pMsg != NULL) {
		for (PhotoRoomPlayerList_t::iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
			if (*itr != NULL) {
				(*itr)->SendPlayerMsg(pMsg);
			}
		}
	}
}

void CPhotoRoom::SendMsgToAllBut(GameMsg_Base *pMsg, CEntityPhotoRoomComponent *pExceptRole)
{
	if (pMsg != NULL) {
		for (PhotoRoomPlayerList_t::iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
			if (*itr != NULL && *itr != pExceptRole) {
				(*itr)->SendPlayerMsg(pMsg);
			}
		}
	}
}

void CPhotoRoom::ToGMRoomInfo(GMRoomInfo& gmRoomInfo)
{
	gmRoomInfo.m_RoomID = (unsigned short)RoomID();
	gmRoomInfo.m_RoomName = RoomName();

	PhotoRoomPlayerList_t::iterator it = m_listPlayers.begin();
	for (; it != m_listPlayers.end() ; it++)
	{
		GMRoomPlayerInfo roomPlayerInfo;
		CEntityPhotoRoomComponent *pRoomCom = *it;
		roomPlayerInfo.m_playerID = pRoomCom->RoleAttr()->m_nRoleID;
		roomPlayerInfo.m_playerName = pRoomCom->RoleAttr()->m_szRoleName;
		gmRoomInfo.AddPlayerInfo(roomPlayerInfo);
	}
}


