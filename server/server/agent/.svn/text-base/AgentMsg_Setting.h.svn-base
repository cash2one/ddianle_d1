#ifndef __AgentMsg_SETTING_H__
#define __AgentMsg_SETTING_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"

class AgentMsg_Setting :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_Setting);
	AgentMsg_Setting(void);
	~AgentMsg_Setting(void);

	unsigned int bRestore;//恢复到Agent自身原来的设定
	AGENT_SETTING setting;
public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );
};

#endif

//file end

