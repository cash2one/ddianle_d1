#include "AgentMsg_Execute.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

AgentMsg_Execute::AgentMsg_Execute(void)
:_baseClass(MSG_AGENT_Execute)
{
}

AgentMsg_Execute::~AgentMsg_Execute(void)
{
}
bool AgentMsg_Execute::doDecode( CParamPool &IoBuff )
{
	strCmdLine = IoBuff.GetString();
	return true;
}
bool AgentMsg_Execute::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddString(strCmdLine.c_str());
	return true;
}


//file end

