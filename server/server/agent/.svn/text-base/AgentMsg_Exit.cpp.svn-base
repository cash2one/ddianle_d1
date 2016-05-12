#include "AgentMsg_Exit.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include <sstream>

AgentMsg_Exit::AgentMsg_Exit(void)
:_baseClass(MSG_AGENT_Exit)
{

}

AgentMsg_Exit::~AgentMsg_Exit(void)
{

}
bool AgentMsg_Exit::doDecode( CParamPool &IoBuff )
{
	IoBuff.GetMemory(bCloseSvr,SVRTYPE_COUNT);
	return true;
}
bool AgentMsg_Exit::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddMemory(bCloseSvr,SVRTYPE_COUNT);
	return true;
}

void AgentMsg_Exit::Serialize(std::string &strDestBuffer) const
{
	std::stringstream ss;
	ss << "MSG_AGENT_Exit: ";
	for (int i = SVRTYPE_AGENT; i < SVRTYPE_COUNT; i++)
	{
		ss << "Close[" << s_szSvrType[i] << "]=[" << (bCloseSvr[i] != 0 ? "true" : "false") << "] ";
	}

	strDestBuffer = ss.str();
}

//file end

