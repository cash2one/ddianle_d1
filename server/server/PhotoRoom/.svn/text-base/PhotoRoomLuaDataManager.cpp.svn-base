#include "PhotoRoomLuaDataManager.h"
#include "PhotoRoomDataStruct.h"
#include "../../datastructure/datainfor/PhotoRoomConfig.h"

CPhotoRoomLuaDataManager::CPhotoRoomLuaDataManager()
:m_nMaxInvitePlayerCount(3)
,m_nInvitePlayerInterval(30)
{

}

CPhotoRoomLuaDataManager::~CPhotoRoomLuaDataManager()
{

}

CPhotoRoomLuaDataManager & CPhotoRoomLuaDataManager::Instance()
{
	static CPhotoRoomLuaDataManager s_photoStudioDataManager;

	return s_photoStudioDataManager;
}

void CPhotoRoomLuaDataManager::AddPhotoStudioData( int nMaxInvitePlayerCount, int nInvitePlayerInterval, int nInviteeResponseExpiredTime )
{
	m_nMaxInvitePlayerCount = nMaxInvitePlayerCount;
	m_nInvitePlayerInterval = nInvitePlayerInterval;
	m_nInviteeResponseExpiredTime = nInviteeResponseExpiredTime;
}

void CPhotoRoomLuaDataManager::AddNotificationTimer(CPhotoRoomNotifiationTimer *pTimer)
{
	if (pTimer != NULL) {
		m_mapPhotoRoomNotificationTimer.insert(std::make_pair(pTimer->m_nLevel, pTimer));
	}
}

const CPhotoRoomNotifiationTimer * CPhotoRoomLuaDataManager::GetPhotoRoomNotificationTimer(int nLevel) const
{
	std::map<int, CPhotoRoomNotifiationTimer *>::const_iterator itr = m_mapPhotoRoomNotificationTimer.find(nLevel);
	if (itr != m_mapPhotoRoomNotificationTimer.end() && itr->second != NULL) {
		return itr->second;	
	}

	return NULL;
}

const std::map<int, CPhotoRoomNotifiationTimer *> & CPhotoRoomLuaDataManager::PhotoRoomNotificationTimerMap() const
{
	return m_mapPhotoRoomNotificationTimer;
}

int CPhotoRoomLuaDataManager::MaxInvitePlayerCount()
{
	return m_nMaxInvitePlayerCount;
}

int CPhotoRoomLuaDataManager::InvitePlayerInterval()
{
	return m_nInvitePlayerInterval;
}

int CPhotoRoomLuaDataManager::InviteeResponseExpiredTime()
{
	return m_nInviteeResponseExpiredTime;
}

void CPhotoRoomLuaDataManager::AddNPCInfo( int nThemeID, const CPhotoRoomNPCInfo& npcInfo )
{
	m_mapNPCs[nThemeID].push_back(npcInfo);
}

const std::vector<CPhotoRoomNPCInfo>* CPhotoRoomLuaDataManager::GetPhotoRoomNPCInfo( int nThemeID ) const
{
	std::map<int, std::vector<CPhotoRoomNPCInfo> >::const_iterator it = m_mapNPCs.find(nThemeID);
	if (it != m_mapNPCs.end())
	{
		return &(it->second);
	}
	return NULL;
}

