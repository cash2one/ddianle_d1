#include "AgentMsg_Announce.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

AgentMsg_Announce::AgentMsg_Announce(void)
:_baseClass(MSG_AGENT_Announce)
{
	szContent="";
}

AgentMsg_Announce::~AgentMsg_Announce(void)
{
}
bool AgentMsg_Announce::doDecode( CParamPool &IoBuff )
{
	szContent = IoBuff.GetString();
	return true;
}
bool AgentMsg_Announce::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddString(szContent.c_str());
	return true;
}


//file end

