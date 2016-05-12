#include "AgentMsg_Start.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include <sstream>

AgentMsg_Start::AgentMsg_Start(void)
:_baseClass(MSG_AGENT_Start)
{
}

AgentMsg_Start::~AgentMsg_Start(void)
{
}
bool AgentMsg_Start::doDecode( CParamPool &IoBuff )
{
	IoBuff.GetMemory(bStartSvr,SVRTYPE_COUNT);
	return true;
}
bool AgentMsg_Start::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddMemory( bStartSvr,sizeof(unsigned char)*SVRTYPE_COUNT );
	return true;
}

void AgentMsg_Start::Serialize(std::string &strDestBuffer) const
{
	std::stringstream ss;
	ss << "MSG_AGENT_Start: ";
	for (int i = SVRTYPE_AGENT; i < SVRTYPE_COUNT; i++)
	{
		ss << "Start[" << s_szSvrType[i] << "]=[" << (bStartSvr[i] != 0 ? "true" : "false") << "] ";
	}

	strDestBuffer = ss.str();
}

//file end

