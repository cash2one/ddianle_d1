#ifndef __AgentMsg_EXECUTE_H__
#define __AgentMsg_EXECUTE_H__
#include "../../socket/GameMsg_Base.h"
#include <string>
using namespace std;
class AgentMsg_Execute :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(AgentMsg_Execute);
	AgentMsg_Execute(void);
	~AgentMsg_Execute(void);

	string strCmdLine;
public:
	virtual bool doDecode( CParamPool &IoBuff );
	virtual bool doEncode( CParamPool &IoBuff );
};
#endif


//file end

