#include "AgentMsg_ShutdownInTime.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

AgentMsg_ShutdownInTime::AgentMsg_ShutdownInTime(void)
:_baseClass(MSG_AGENT_ShutdownInTime)
{

}

AgentMsg_ShutdownInTime::~AgentMsg_ShutdownInTime(void)
{

}

bool AgentMsg_ShutdownInTime::doDecode( CParamPool &IoBuff )
{
	nSeconds = IoBuff.GetUInt();
	return true;
}

bool AgentMsg_ShutdownInTime::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddUInt(nSeconds);
	return true;
}


//file end

