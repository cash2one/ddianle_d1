#ifndef __WORLDMARRIAGEMANAGER_H__
#define __WORLDMARRIAGEMANAGER_H__

#include <map>
#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/DataStruct_Couple.h"

class CWCoupleManager :
	public GameMsg_Processor
{
private:
	CWCoupleManager();	
	virtual ~CWCoupleManager();
public:
	static CWCoupleManager & Instance();
	void Init();
private:
	void _RegisterMessage();

	void _OnAcceptCoupleRequest(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnCoupleSeparated(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnGetMarried(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnDivorced(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnWeddingRoomStartNofication(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnAutoStartWeddingRoomFail(GameMsg_Base & msg, CSlotPeer & slotPeer);
	void _OnUpdateCoupleIntimacy(GameMsg_Base & msg, CSlotPeer & slotPeer);
private:

};




#endif

