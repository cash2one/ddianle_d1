#ifndef		__PHOTOROOMMANAGER_H__
#define		__PHOTOROOMMANAGER_H__

#include <vector>
#include <list>
#include <map>
#include "PhotoRoomMacroDefine.h"
#include "GameMsg_S2C_PhotoRoom.h"
#include "PhotoRoomMacroDefine.h"

class CPhotoRoom;
class CEntityPhotoRoomComponent;
class GameMsg_Base;
class GMRoomInfo;

class CPhotoRoomManager
{
private:
	CPhotoRoomManager();
	~CPhotoRoomManager();
public:
	static CPhotoRoomManager & Instance();
	void Init();
	void OnUpdate(const unsigned long &lTimeElapsed);
	void PlayerLogout(CEntityPhotoRoomComponent &rRolePhotoRoom);

	void GetRoomListByGM(std::vector<GMRoomInfo>& roomList);

	void OnGetMyPhotoRoomInfo(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnCreatePhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnQuitPhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnClosePhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnInvitedPlayer(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnInviteeResponse(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnTryEnterPhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnKickPhotoRoomPlayer(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnChangePhotoRoomPlayerState(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnChangePhotoRoomState(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnPlayerChangeRoomAction(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnPlayerChangeRoomBG(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
	void OnRenewPhotoRoom(GameMsg_Base &rMsg, CEntityPhotoRoomComponent &rRolePhotoRoom);
private:
	ECreatePhotoRoomError _CanCreatePhotoRoom(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int  nRoomTheme);
	EInvitePhotoRoomError _CanInvitePhotoRoomPlayer(CEntityPhotoRoomComponent &rRolePhotoRoom, std::list<unsigned int> &listInvitees);
	PhotoRoomInviteeResponseError _CanBeInvitedToPhotoRoom(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nRoomID);
	ETryEnterMyPhotoRoomError _CanEnterMyPhotoRoom(CEntityPhotoRoomComponent &rRolePhotoRoom, EPhotoRoomTheme eRoomType);
	EKickPhotoRoomPlayerError _CanKickPhotoRoomPlayer(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nBeKickedID);
	EPhotoRoomPlayerChangeStateError _CanChangePhotoRoleState(CEntityPhotoRoomComponent &rRolePhotoRoom, EPhotoRoomPlayerState ePlayerState);
	EChangePhotoRoomStateError _CanChangePhotoRoomState(CEntityPhotoRoomComponent &rRolePhotoRoom, EPhotoRoomState eRoomState);
	EChangePhotoRoomActionError _CanChangePhtotoRoomAction(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nActionID);
	EChangePhotoRoomBGError _CanChangePhotoRoomBG(CEntityPhotoRoomComponent &rRolePhotoRoom, unsigned int nBGID);
	ERenewPhotoRoomError _CanRenewPhotoRoom(CEntityPhotoRoomComponent &rRolePhotoRoom);

	unsigned int _GeneratePhotoRoomID();
	void _ReleasePhotoRoomID(unsigned int nRoomID);

	void _AddPhotoRoom(CPhotoRoom *pPhotoRoom);
	CPhotoRoom *_RemovePhotoRoom(unsigned int nRoomID);

	void _InTimeExpiredPhotoRoomContainer(CPhotoRoom *pPhotoRoom);
	void _OutTimeExpiredPhotoRoomContainer(unsigned int nRoomID);

	void _InTimeExpiredNotificationContainer(CPhotoRoom *pPhotoRoom);
	void _OutTimeExpiredNotificationContainer(unsigned int nRoomID);

	CPhotoRoom * _FindHostRoom(unsigned int nRoleID);
	CPhotoRoom * _FindPhotoRoom(unsigned int nRoomID);

	bool _PlayerInPhotoRoom(CEntityPhotoRoomComponent & rRolePhotoRoom, CPhotoRoom * pRoom);
	CEntityPhotoRoomComponent * _PlayerOutPhotoRoom(CEntityPhotoRoomComponent & rRolePhotoRoom);

	void _DestroyPhotoRoom(unsigned int nRoomID);
public:
	typedef std::list<unsigned int> PhotoRoomIDList;							
	typedef std::map<unsigned int, CPhotoRoom*> PhotoRoomMap;				//	key - roomID
	typedef std::map<unsigned int, PhotoRoomMap> PhotoRoomMapMap;			//	key - themeID
	typedef std::multimap<int, CPhotoRoom*> TimePhotoRoomMap_t;				//	key - expiredtime 

	PhotoRoomIDList m_listIdleID;
	PhotoRoomIDList m_listSpawnedID;
	PhotoRoomMapMap m_mapAlThemelRoom;
	TimePhotoRoomMap_t m_mapTimeExpiredRooms;
	std::vector<TimePhotoRoomMap_t> m_vecTimeExpiredNoficationMap;
};




#endif



