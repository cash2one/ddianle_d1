#ifndef		__PHOTOROOM_H__
#define		__PHOTOROOM_H__

#include <list>
#include <map>
#include "PhotoRoomMacroDefine.h"

class CEntityPhotoRoomComponent;
class PhotoRoomConfig;
class PhotoRoomGroupAction;
class CPhotoRoomWholeInfo;
class GMRoomInfo;

class GameMsg_Base;

class CPhotoRoom
{
public:
	CPhotoRoom();
	~CPhotoRoom();
public:
	typedef std::list<CEntityPhotoRoomComponent *> PhotoRoomPlayerList_t;	// key - roleid
public:

	void SetRoomID(unsigned int nRoomID);
	unsigned int RoomID() const;

	void SetRoomName(std::string & strRoomName);
	const std::string & RoomName() const;

	void SetHostID(unsigned int nHostID);
	unsigned int HostID() const;

	void SetCreateTime(unsigned int nCreatedTime);
	unsigned int CreateTime() const;

	void SetExistTime(unsigned int nExistTime);
	unsigned int ExistTime();

	int RemainTime();

	void SetRoomTheme(EPhotoRoomTheme nRoomTheme);
	EPhotoRoomTheme RoomTheme() const;

	void SetRoomState(EPhotoRoomState eState);
	EPhotoRoomState RoomState() const;	

	void SetSelectedBG(unsigned int nSelectedBGID);
	unsigned int SelectedBG();

	void SetSelectedAction(unsigned int nActionID);
	unsigned int SelectedAction();

	void AddRoomBG(const PhotoRoomConfig *pBGInfo);
	bool IsInRoomBGList(unsigned int nBG) const;

	void AddRoomAction(const PhotoRoomGroupAction *pActionInfo);
	bool IsInRoomActionList(unsigned int nActionID) const;

	int PlayerCount();

	void EncodeRoomInfo(CPhotoRoomWholeInfo & rWholeRoomInfo, bool bHost);

	const PhotoRoomPlayerList_t & PlayerList();

	bool AddPhotoRoomPlayer(CEntityPhotoRoomComponent *pRolePhotoRoom);
	CEntityPhotoRoomComponent * RemovePhotoRoomPlayer(unsigned int nRoleID);

	CEntityPhotoRoomComponent * GetPlayer(unsigned int nRoleID) const;

	const PhotoRoomPlayerList_t & PhotoRoomPlayerMap() const;

	void OnRoomExpiredNotification(int nLevel);

	void DestroyRoom();

	void SendMsgToAll(GameMsg_Base *pMsg);
	void SendMsgToAllBut(GameMsg_Base *pMsg, CEntityPhotoRoomComponent *pExceptRole);

	void ToGMRoomInfo(GMRoomInfo& gmRoomInfo);
	void GetPlayerInfo(CEntityPhotoRoomComponent * player,unsigned int& playerID,std::string& playerName);

private:
	unsigned int m_nRoomID;
	std::string m_strRoomName;
	unsigned int m_nHostID;
	unsigned int m_nCreatedTime;
	unsigned int m_nExistTime;
	EPhotoRoomState m_eRoomState;
	EPhotoRoomTheme m_eRoomTheme;
	unsigned int m_nSelectedBG;
	unsigned int m_nSelectedAction;

	std::map<unsigned int, const PhotoRoomConfig *> m_mapRoomPhotoBG;			//key-CPhotoThemeBGInfo ID
	std::map<unsigned int, const PhotoRoomGroupAction *> m_mapRoomAction;	//key-CPhotoRoomThemeGroupAction ID
	PhotoRoomPlayerList_t m_listPlayers;
};






#endif











