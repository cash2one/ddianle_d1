#ifndef __AgentMsg_SYSINFO_H__
#define __AgentMsg_SYSINFO_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"

class AgentMsg_SysInfo :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_SysInfo);
	AgentMsg_SysInfo(void);
	~AgentMsg_SysInfo(void);
	STAT_SYS stat;
public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );
};

#endif


//file end

