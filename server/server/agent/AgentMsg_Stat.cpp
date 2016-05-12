#include "AgentMsg_Stat.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

AgentMsg_Stat::AgentMsg_Stat(void)
:_baseClass(MSG_AGENT_Stat)
{
	pid = 0;
}

AgentMsg_Stat::~AgentMsg_Stat(void)
{
}
bool AgentMsg_Stat::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddInt(pid);
	IoBuff.AddInt(stat.nType);
	IoBuff.AddInt(stat.nStat);
	IoBuff.AddInt(stat.nCltCnt);
	IoBuff.AddInt(stat.nVersion);
	IoBuff.AddUShort(stat.nPort);

	return true;
}

bool AgentMsg_Stat::doDecode( CParamPool &IoBuff )
{
	pid = IoBuff.GetInt();
	stat.nType = IoBuff.GetInt();
	stat.nStat = IoBuff.GetInt();
	stat.nCltCnt = IoBuff.GetInt();
	stat.nVersion = IoBuff.GetInt();
	stat.nPort = IoBuff.GetUShort();

	return true;
}


//file end

