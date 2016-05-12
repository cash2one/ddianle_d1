#include "AgentMsg_SysInfo.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

AgentMsg_SysInfo::AgentMsg_SysInfo(void):_baseClass(MSG_AGENT_SysInfo)
{
}

AgentMsg_SysInfo::~AgentMsg_SysInfo(void)
{
}
bool AgentMsg_SysInfo::doDecode( CParamPool &IoBuff )
{
	stat.nCPU[0] = IoBuff.GetUShort();
	stat.nCPU[1] = IoBuff.GetUShort();
	stat.nCPU[2] = IoBuff.GetUShort();

	stat.nMemLeft = IoBuff.GetUShort();
	stat.nMemUsed = IoBuff.GetUShort();

	return true;
}
bool AgentMsg_SysInfo::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddUShort(stat.nCPU[0]);
	IoBuff.AddUShort(stat.nCPU[1]);
	IoBuff.AddUShort(stat.nCPU[2]);

	IoBuff.AddUShort( stat.nMemLeft );
	IoBuff.AddUShort( stat.nMemUsed );

	return true;
}


//file end

