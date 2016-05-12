#ifndef __TIME_EGG_MGR_H__
#define __TIME_EGG_MGR_H__

#include <set>
#include <map>
#include <vector>
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/DataStruct_Base.h"
using namespace std;

struct TimeEggEntry;

typedef std::map<unsigned int, TimeEggEntry*> EggMap;

class CTimeEggMgr : public GameMsg_Processor
{
public:
	CTimeEggMgr();
	~CTimeEggMgr();

	static CTimeEggMgr& Instance();

	void Init();

	void OnUpdate(const unsigned long& lTimeElapsed);
	EggMap& GetEggMap(){return m_mapEgg;}
private:
	void OnCreateTimeEgg( GameMsg_Base & Msg ,CSlotPeer & SlotPeer );
	void OnOpenTimeEgg( GameMsg_Base & Msg ,CSlotPeer & SlotPeer );
	void OnTimeEggSetting( GameMsg_Base & Msg ,CSlotPeer & SlotPeer );
	void OnDBAutoOpenEgg(QueryBase &pQuery);
	void OnDBGetTimeEggs(QueryBase &pQuery);
	void RegisterMessageProcessor();
private:
	EggMap m_mapEgg;
	unsigned long m_lTimeElapsed;
	TimeEggSetting m_setting;
	bool m_bRecvSetting;
};

#endif

