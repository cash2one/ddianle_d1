#ifndef __AgentMsg_ANNOUNCE_H__
#define __AgentMsg_ANNOUNCE_H__
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Agent.h"

#include <string>
using namespace std;
class AgentMsg_Announce :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_Announce);
	AgentMsg_Announce(void);
	~AgentMsg_Announce(void);

	string szContent;

public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );
};
#endif


//file end

