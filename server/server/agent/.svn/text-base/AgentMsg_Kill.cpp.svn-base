#include "AgentMsg_Kill.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include <sstream>

AgentMsg_Kill::AgentMsg_Kill(void)
:_baseClass(MSG_AGENT_Kill)
{
}

AgentMsg_Kill::~AgentMsg_Kill(void)
{
}

bool AgentMsg_Kill::doDecode( CParamPool &IoBuff )
{
	IoBuff.GetMemory(bSvr,SVRTYPE_COUNT);
	return true;
}
bool AgentMsg_Kill::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddMemory(bSvr,SVRTYPE_COUNT);
	return true;
}

void AgentMsg_Kill::Serialize(std::string &strDestBuffer) const
{
	std::stringstream ss;
	ss << "MSG_AGENT_Kill: ";
	for (int i = SVRTYPE_AGENT; i < SVRTYPE_COUNT; i++)
	{
		ss << "Kill[" << s_szSvrType[i] << "]=[" << (bSvr[i] != 0 ? "true" : "false") << "] ";
	}

	strDestBuffer = ss.str();
}

//file end

