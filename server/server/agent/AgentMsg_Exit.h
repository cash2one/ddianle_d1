#ifndef __AgentMsg_EXIT_H__
#define __AgentMsg_EXIT_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"

class AgentMsg_Exit:
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_Exit);
	AgentMsg_Exit(void);
	~AgentMsg_Exit(void);

	unsigned char bCloseSvr[SVRTYPE_COUNT];

public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );

	void Serialize(std::string &strDestBuffer) const;
};
#endif


//file end

