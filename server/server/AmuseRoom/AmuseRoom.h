#ifndef __AMUSEROOM__H__
#define __AMUSEROOM__H__

#include "../../socket/Windefine.h"
#include <string>
#include <list>
#include <map>
#include <set>

class CAmuseRoomComponent;
class CAmuseRoomDevice;
class CAmuseAction_Base;
class CAmuseEvent_Base;
class GameMsg_Base;
class CAmuseRoomBriefInfo;
class CAmuseRoomWholeInfo;
class GMRoomInfo;


typedef std::list<unsigned int> AmusePlayerIDList;
typedef std::map<unsigned int, CAmuseRoomComponent*> AmusePlayerMap;	//key - player id
typedef std::map<int, CAmuseRoomDevice*> AmuseRoomDeviceMap;		//key - device id
typedef std::multimap<unsigned long, CAmuseEvent_Base*> AmuseRoomEventMap;	//key - start time


class CAmuseRoom
{
public:
	CAmuseRoom(unsigned int nRoomID, int nRoomTheme, int nRoomScene, const std::string& strRoomName, const std::string& strRoomPwd, unsigned long lCreateTime);
	~CAmuseRoom();

	void OnUpdate(unsigned long lMgrTimeNow, unsigned long lTimeElapsed);

public:
	unsigned int RoomID() const;
	const std::string& RoomName() const;
	const std::string& RoomPwd() const;
	void ChangeRoomInfo(const char* pRoomName, const char* pRoomPwd);

	int RoomTheme() const;
	int RoomScene() const;

	int RoomCapacity() const;
	void SetRoomCapacity(int nCapcity);

	bool HasPwd() const;
	bool PwdPassed(const char* pRoomPwd) const;

	unsigned long CreateTime() const;
	int ActionID();

	CAmuseRoomComponent* AllocHost();
	CAmuseRoomComponent* RoomHost();
	void AssignHost(CAmuseRoomComponent* pAmuseHost);

	int PlayerCount() const;
	CAmuseRoomComponent* GetAmusePlayer();
	CAmuseRoomComponent* GetAmusePlayer(unsigned int nPlayerID);

	CAmuseRoomDevice* GetRoomDevice(int nDeviceID);

	int MaleCount() const;
	int FemaleCount() const;

	int RoomColor() const;
	bool IsTopRoom() const;

	bool PlayerIn(CAmuseRoomComponent& rAmusePlayer, bool bHost);
	CAmuseRoomComponent* PlayerOut(unsigned int nPlayerID);

	void EncodeBriefInfo(CAmuseRoomBriefInfo& rBriefInfo);
	void EncodeWholeInfo(CAmuseRoomWholeInfo& rWholeInfo, bool bHost);

	void SendMsgToAll(GameMsg_Base* pMsg, CAmuseRoomComponent* pExceptPlayer = NULL);
	void SendMsgToAll(GameMsg_Base* pMsg, const std::set<CAmuseRoomComponent*>& setExceptPlayer);

	void UpdateAmuseRoomEvent(unsigned long lDelayTime, CAmuseEvent_Base* pAmuseEvent);

	bool IsConstellationRoom() const;

	void ToGMRoomInfo(GMRoomInfo& gmRoom);

    bool CheckIsInDevice(int RoleId);

protected:
	void _LoadAmuseRoomEvent();
	void _LoadAmuseRoomDevice();

protected:
	unsigned int m_nRoomID;
	std::string m_strRoomName;
	std::string m_strRoomPwd;

	int m_nRoomTheme;
	int m_nRoomScene;
	int m_nCapacity;

	unsigned long m_lCreateTime;
	unsigned long m_lInternalTime;
	unsigned long m_lEventStep;

	unsigned int m_nActionID;

	int m_nAmuseMaleCount;
	int m_nAmuseFemaleCount;

	CAmuseRoomComponent* m_pAmuseHost;

	AmusePlayerIDList m_AllAmusePlayerID;
	AmusePlayerMap m_AllAmusePlayers;

	AmuseRoomEventMap m_AllAmuseEvents;
	AmuseRoomDeviceMap m_AllAmuseDevices;
};

#endif
//file end

