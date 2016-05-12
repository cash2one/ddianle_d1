#ifndef	__COMPENSATION_PROCESSOR_H__
#define __COMPENSATION_PROCESSOR_H__

#include "../../socket/GameMsg_Processor.h"


class CCompensationProcessor : public GameMsg_Processor
{
protected:
	CCompensationProcessor();
	virtual ~CCompensationProcessor();

public:
	static CCompensationProcessor& Instance();

	void InitCompensation();

	void OnGameServerConnected(unsigned short nSlot);
	void OnGMUpdateCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer);

protected:
	void _RegisterMessage();
	void _RegisterQuery();

	void _LoadCompensation();
	void _SyncCompensation(int nSlot);

	void _OnLoadCompensation(QueryBase& query);
	void _OnInsertCompensation(QueryBase& query);
	
private:
	bool m_bHasLoad;
};

#endif


