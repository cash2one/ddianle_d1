#include "AgentMsg_Close.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include <sstream>

AgentMsg_Close::AgentMsg_Close(void)
:_baseClass(MSG_AGENT_Close)
{
}

AgentMsg_Close::~AgentMsg_Close(void)
{
}

bool AgentMsg_Close::doDecode( CParamPool &IoBuff )
{
	IoBuff.GetMemory(bCloseSvr,SVRTYPE_COUNT);
	return true;
}
bool AgentMsg_Close::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddMemory( bCloseSvr,sizeof(char)*SVRTYPE_COUNT );
	return true;
}

void AgentMsg_Close::Serialize(std::string &strDestBuffer) const
{
	std::stringstream ss;
	ss << "MSG_AGENT_Close: ";
	for (int i = SVRTYPE_AGENT; i < SVRTYPE_COUNT; i++)
	{
		ss << "Close[" << s_szSvrType[i] << "]=[" << (bCloseSvr[i] != 0 ? "true" : "false") << "] ";
	}

	strDestBuffer = ss.str();
}

//file end

