#ifndef __AgentMsg_START_H__
#define __AgentMsg_START_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"

class AgentMsg_Start :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_Start);
	AgentMsg_Start(void);
	~AgentMsg_Start(void);

	unsigned char bStartSvr[SVRTYPE_COUNT];
public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );

	void Serialize(std::string &strDestBuffer) const;
};
#endif



//file end

