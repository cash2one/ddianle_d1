#ifndef __AgentMsg_STAT_H__
#define __AgentMsg_STAT_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"

class AgentMsg_Stat :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_Stat);
	AgentMsg_Stat(void);
	~AgentMsg_Stat(void);

	int pid;
	STAT_BASE stat;

public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );
};
#endif


//file end

