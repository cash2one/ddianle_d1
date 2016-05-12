#include "AgentMsg_Setting.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

AgentMsg_Setting::AgentMsg_Setting(void)
:_baseClass(MSG_AGENT_Setting)
{
}

AgentMsg_Setting::~AgentMsg_Setting(void)
{
}
bool AgentMsg_Setting::doDecode( CParamPool &IoBuff )
{
	bRestore = IoBuff.GetUInt();
	for( int i = 0 ;i< SVRTYPE_COUNT ;i++ )
	{
		setting.abKeepServer[i] = IoBuff.GetBool();
	}
	setting.nCheckInterval = IoBuff.GetInt();
	setting.nSysInfoInterval = IoBuff.GetInt();
	return true;
}
bool AgentMsg_Setting::doEncode( CParamPool &IoBuff )
{
	IoBuff.AddUInt(bRestore);
	for( int i = 0 ;i< SVRTYPE_COUNT ;i++ )
	{
		IoBuff.AddBool(setting.abKeepServer[i]);
	}
	IoBuff.AddInt( setting.nCheckInterval );
	IoBuff.AddInt( setting.nSysInfoInterval );
	return true;
}


//file end

