#ifndef __WORLDATTRIBUTE_H__
#define __WORLDATTRIBUTE_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../socket/SlotPeer.h"
#include "../../datastructure/DataStruct_DB.h"

class CWAttributeManager :
	public GameMsg_Processor
{
protected:
	CWAttributeManager();	
	virtual ~CWAttributeManager();

public:
	static CWAttributeManager& Instance();
	void Initialize();

private:
	void RegisterMessage();
	void RegisterMessageProcessor();

private:
	void OnUpdateRoleInfo(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnUpdateRoomInfo(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnUpdateRoleName(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnUpdateRoleLevel(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnUpdateRoleMoney(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnUpdateRoleLocation(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnGetPlayerInfoRequest(GameMsg_Base &msg, CSlotPeer & slotPeer);
	void OnGetPlayerInfoReply(GameMsg_Base &msg, CSlotPeer & slotPeer);
    void OnUpdateRoleAge(GameMsg_Base &msg, CSlotPeer &slotPeer);
	void UpdateRoleInfo(CWorldRoleUpdateAttrInfo &updateInfo);

public:
    // 获取离线数据
    void GetOfflinePlayerInfo(unsigned int nOnlineRoleID, unsigned int nGainRoleID, EOfflinePlayerDataFrom eFrom);

    // 
    void GetOfflinePlayerInfoBack(GameMsg_Base &msg, CSlotPeer &slotPeer);
};






#endif

// end of file


