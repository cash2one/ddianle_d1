#ifndef __AMUSEROOM_MANAGER_H__
#define __AMUSEROOM_MANAGER_H__

#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Processor.h"
#include <map>
#include <set>
#include <vector>
#include <string>


class CAmuseRoom;
class CAmuseRoomComponent;
class GMRoomInfo;


typedef std::list<unsigned int> AmuseRoomIDList;
typedef std::map<unsigned int, CAmuseRoom*> AmuseRoomMap;
typedef std::multimap<unsigned long, CAmuseRoom*> AmuseRoomTimeMap;
typedef std::list<CAmuseRoom*> AmuseRoomList;
typedef std::set<unsigned int> AmuseRoomIDSet;
typedef std::map<unsigned int, int> AmuseRoomIDMap;


class CAmuseRoomManager
{
protected:
	CAmuseRoomManager();
	~CAmuseRoomManager();

public:
	static CAmuseRoomManager& Instance();

	CAmuseRoom* FindAmuseRoom(unsigned int nAmuseRoomID);
	

	void DestroyAmuseRoom(unsigned int nAmuseRoomID);

	bool AddAmuseRoom(CAmuseRoom* pAmuseRoom);
	CAmuseRoom* RemoveAmuseRoom(unsigned int nAmuseRoomID);

	void OnUpdate(const unsigned long& lTimeElapsed);

	

public:
	void OnAmuseGetRoomList(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);

	void OnAmuseCreateRoom(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);
	void OnAmuseQuitRoom(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);

	void OnAmuseTryEnter(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);
	void OnAmuseEnterPlayer(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);

	void OnAmuseKickPlayer(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);

	void OnAmuseInvitePlayer(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);
	void OnAmuseInviteeResponse(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);

	void OnAmuseChangeRoomInfo(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);
	void OnAmusePromoteHost(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);

	void OnDoAmuseAction(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);
	void OnAmusePlayerLogout(CAmuseRoomComponent& rRoleAmuseRoomCOM);

	void OnAmuseApplyMatch(const GameMsg_Base& rMsg, CAmuseRoomComponent& rRoleAmuseRoomCOM);

	void CreateTestAmuseRoom();
	void CreateTestAmuseRoom(int nSceneID, int nRoomCount, CAmuseRoomComponent *pHost);
 
public:
	bool IsConstellationRoom(unsigned int nRoomID) const;
	bool CreateConstellationRoom(CAmuseRoomComponent& rFirstRoleCOM, CAmuseRoomComponent& rSecondRoleCOM, int nSceneID);
	void OnEnterConstellationRoom(CAmuseRoomComponent &rComponent);

    CAmuseRoom* FindMeetAmuseRoom(unsigned int nAmuseRoomID);
	std::string  CreateMeetAmuseRoom( int nSceneID, int & nAmuseID, CAmuseRoomComponent& rRoleAmuseRoomCOM );
	bool IsMeetAmuseSceneID( int nRoomID );
	void EnterMeetAmuse( int nRoomID,  CAmuseRoomComponent& rRoleAmuseRoomCOM );
	bool AddMeetAmuseRoom( CAmuseRoom* pAmuseRoom );
	void DestroyMeetAmuseRoom( unsigned int nAmuseRoomID );

	void GetRoomListByGM(std::vector<GMRoomInfo>& roomList);

protected:
	unsigned int _GenerateAmuseRoomID();
	void _ReleaseAmuseRoomID(unsigned int nRoomID);

	void _HandleAmuseTopRoom(CAmuseRoom* pAmuseRoom);

	void _AmuseEnterPlayer(CAmuseRoomComponent& rRoleRoomCOM, CAmuseRoom *pAmuseRoom, const std::string& strRoomPwd);

	bool _PlayerInAmuseRoom(CAmuseRoomComponent& rRoleAmuseRoomCOM, CAmuseRoom* pAmuseRoom, bool bHost, int nInType);
	void _PlayerOutAmuseRoom(CAmuseRoomComponent& rOuterAmuseRoomCOM, CAmuseRoomComponent* pKickerAmuseRoomCOM, int nOutType, int nQuitTo);

	bool _ApplyMatchRoom(CAmuseRoomComponent& rRoleAmuseRoomCOM, bool bCreateRoom, const std::string& strRoomName, char chRoomTheme, char chRoomScene);

	bool _IsInConstellationRoomEndingList(unsigned int nRoomID) const;
	void _AddConstellationRoomEndingList(unsigned int nRoomID, int nTime);
	void _RemoveConstellationRoomEndingList(unsigned int nRoomID);

	void LogRoleEnterRoom(unsigned int nRoleID, const std::string &strRoleName, int nSceneID, const std::string &strSceneName);

protected:
	unsigned long		m_lMgrNowTime;
	AmuseRoomIDList		m_IdleID;
	AmuseRoomIDList		m_StoreID;

	AmuseRoomMap		m_AllAmuseRooms;
	AmuseRoomIDSet		m_TopAmuseRoomID;
	AmuseRoomList		m_TopAmuseRooms;

	AmuseRoomIDSet		m_ConstellationRoomID;
	AmuseRoomTimeMap	m_EndConstellationRooms;
	AmuseRoomIDMap		m_ConstellationRoomEndID;

	AmuseRoomMap		m_MeetAmuseRooms;               // 邂逅创建房间	
};

#endif

//file end

