#ifndef		__INFOCENTERMGR_H__
#define		__INFOCENTERMGR_H__

#include <map>
#include "../../socket/GameMsg_Base.h"
#include "../../socket/GameMsg_Processor.h"
#include "GMDataStruct.h"

class InfoCenterMgr:
	public GameMsg_Processor
{
private:
	InfoCenterMgr();	
	~InfoCenterMgr();
public:
	static InfoCenterMgr & Instance();
public:
	void Initialize();


private:
	void _RegisterMessage();
	void _RegisterMessageProcessor();


	void OnGMInfoCenterRecvClientGet(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMInfoCenterRecvClientAdd(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMInfoCenterRecvClientUpdate(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMInfoCenterRecvClientRemove(GameMsg_Base & msg, CSlotPeer & slotPeer);

	void OnGMInfoCenterRecvGroupGet(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMInfoCenterRecvGroupAdd(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMInfoCenterRecvGroupUpdate(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void OnGMInfoCenterRecvGroupRemove(GameMsg_Base & msg, CSlotPeer & slotPeer);

public:
	
};







#endif // __INFOCENTERMGR_H__



