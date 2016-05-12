#ifndef __GRPFRIEND_H__
#define __GRPFRIEND_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../socket/SlotPeer.h"
#include "../group/Global.h"
#include "GameMsg_S2G_FindPlayerFromID.h"
#include "GameMsg_G2S_FindPlayerFromID.h"

class CWFriendManager :
	public GameMsg_Processor
{
protected:
	CWFriendManager();	
	virtual ~CWFriendManager();

public:
	static CWFriendManager& Instance()
	{
		static CWFriendManager stcFriendMgr;
		return stcFriendMgr;
	}
	void Initialize()
	{
		RegisterMessage();
		RegisterMessageProcessor();
	}

public:
	void OnPlayerLogin(CGroupRoleEntity & role);
	void OnPlayerLogout(CGroupRoleEntity & role);

private:
	void RegisterMessage();
	void RegisterMessageProcessor();

	void OnAddBlackFriend(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnGetBlackListOnlineInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnGetTempListOnlineInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnRemoveFriend(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnFindPlayer(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnAddBlackList(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnRemoveBlackList(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnGetFriendOnlineInfo(GameMsg_Base  &msg, CSlotPeer &slotPeer);
	void OnGetAllFriendOnlineInfo(GameMsg_Base  &msg, CSlotPeer &slotPeer);

	void OnPushOnlineInfoToAllFriends(GameMsg_Base  &msg, CSlotPeer &slotPeer);
	void OnUpdateAttributeToAllFriends(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnFriendApplication(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnFriendLogin(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnNotifyHasBeenAccepted(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void OnNotifyHasBeenDeletedFriend(GameMsg_Base &msg, CSlotPeer &slotPeer);

	void OnNotifyFriendLogin(CGroupRoleEntity &role);
	void SendOnlineInfoToAllFriends(CGroupRoleEntity &role);

	void SetPlayerOnlineInfo(CGroupRoleEntity &role, CFriendOnlineInfo & Info);
	void SetFindPlayerInfo(CGroupRoleEntity &role, CFindPlayerInfo & info);

	void FindPlayerByRoleID(std::list<CFindPlayerInfo> & playerList, unsigned int nRoleID);
	void FindPlayerByRoleName(GameMsg_G2S_FindPlayerResult &res, GameMsg_S2G_FindPlayer &msg);


    void OnLoadFriendOfflineTimeBack(QueryBase & rQuery);
};

#endif

// end of file

