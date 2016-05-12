#ifndef __AMUSEROOM_DEVICE__H__
#define __AMUSEROOM_DEVICE__H__

#include <set>
#include <vector>
#include <stdlib.h>

class CAmuseRoomDeviceInfo;
class CAmuseDeviceData;
class CAmuseRoom;
class CAmuseRoomComponent;
class GameMsg_Base;

typedef std::set<unsigned int> DeviceUserSet;

enum AmuseRoomDeviceType
{
	AmuseRoomDeviceType_Normal,
	AmuseRoomDeviceType_TimeEgg,
};

class CAmuseRoomDevice
{
public:
	CAmuseRoomDevice(CAmuseRoom* pAmuseRoom);
	~CAmuseRoomDevice();

public:
	int DeviceID() const;
	int InviteType() const;
	bool ReachCapacity() const;

	bool HasUser() const;
	bool HasUser(unsigned int nUserID) const;

	void UserIn(unsigned int nPlayerID);
	void UserOut(unsigned int nPlayerID);

	void EncodeDeviceInfo(CAmuseRoomDeviceInfo& rDeviceInfo);
	void DecodeDeviceInfo(CAmuseDeviceData& rDeviceData);

	void GetPlayers(std::vector<CAmuseRoomComponent*>& players);
	void GetPlayers(std::vector<unsigned int>& players);
	CAmuseRoomComponent* GetOtherPlayer(unsigned int nRoleID);//取得对方玩家

	AmuseRoomDeviceType GetDeviceType();
	void SetDeviceType(AmuseRoomDeviceType type);

	void SendMsgToAll(GameMsg_Base* pMsg, CAmuseRoomComponent* pExceptPlayer = NULL);
    /// <summary>
    /// 检测是否在坐骑上
    /// <summary>
    bool IsInDevice(int nRoleId);
protected:
	CAmuseRoom* m_pOwnerRoom;

	int m_nDeviceID;
	int m_nDeviceCapacity;
	int m_nInviteType;

	DeviceUserSet m_setDeviceUsers;

	AmuseRoomDeviceType m_nDeviceType;
};

#endif
//file end

