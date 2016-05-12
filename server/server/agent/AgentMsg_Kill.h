#ifndef __AgentMsg_KILL_H__
#define __AgentMsg_KILL_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"

class AgentMsg_Kill :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_Kill);

	AgentMsg_Kill(void);
	~AgentMsg_Kill(void);

	unsigned char bSvr[SVRTYPE_COUNT];

public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );

	void Serialize(std::string &strDestBuffer) const;
};
#endif


//file end

