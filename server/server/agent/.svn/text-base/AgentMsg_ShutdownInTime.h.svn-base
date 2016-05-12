#ifndef __AgentMsg_SHUTDOWNINTIME_H__
#define __AgentMsg_SHUTDOWNINTIME_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"

class AgentMsg_ShutdownInTime :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_ShutdownInTime);

	AgentMsg_ShutdownInTime(void);
	~AgentMsg_ShutdownInTime(void);

	unsigned int nSeconds;

public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );
};
#endif



//file end

