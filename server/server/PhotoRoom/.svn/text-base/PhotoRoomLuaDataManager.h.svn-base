#ifndef		__PHOTOROOMDATAMANAGER_H__
#define		__PHOTOROOMDATAMANAGER_H__ 

#include <map>
#include <vector>
#include "PhotoRoomMacroDefine.h"
#include "PhotoRoomDataStruct.h"

class CPhotoRoomNotifiationTimer;

class CPhotoRoomLuaDataManager
{
private:
	CPhotoRoomLuaDataManager();
	~CPhotoRoomLuaDataManager();
public:
	static CPhotoRoomLuaDataManager & Instance();

public:
	void AddPhotoStudioData(int nMaxInvitePlayerCount, int nInvitePlayerInterval, int nInviteeResponseExpiredTime);

	void AddNotificationTimer(CPhotoRoomNotifiationTimer *pTimer);

	void AddNPCInfo(int nThemeID, const CPhotoRoomNPCInfo& npcInfo);

	const CPhotoRoomNotifiationTimer * GetPhotoRoomNotificationTimer(int nLevel) const;
	
	const std::map<int, CPhotoRoomNotifiationTimer *> & PhotoRoomNotificationTimerMap() const;

	const std::vector<CPhotoRoomNPCInfo>* GetPhotoRoomNPCInfo(int nThemeID) const;

	int MaxInvitePlayerCount();
	int InvitePlayerInterval();
	int InviteeResponseExpiredTime();
private:
	int m_nMaxInvitePlayerCount;			//default:3
	int m_nInvitePlayerInterval;			//default:3s
	int m_nInviteeResponseExpiredTime;		//default:60s

	std::map<int, CPhotoRoomNotifiationTimer *> m_mapPhotoRoomNotificationTimer;
	std::map<int, std::vector<CPhotoRoomNPCInfo> > m_mapNPCs;//key: photo room's theme id
};







#endif


