#ifndef __AgentMsg_CLOSE_H__
#define __AgentMsg_CLOSE_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"
#include <string>

class AgentMsg_Close :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_Close);
	AgentMsg_Close(void);
	~AgentMsg_Close(void);

	unsigned char bCloseSvr[SVRTYPE_COUNT];

public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );

	void Serialize(std::string &strDestBuffer) const;
};
#endif


//file end

