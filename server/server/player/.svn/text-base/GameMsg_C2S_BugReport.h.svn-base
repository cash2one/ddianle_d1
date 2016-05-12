#ifndef _GAMEMSG_C2S_BUGREPORT_H_
#define _GAMEMSG_C2S_BUGREPORT_H_

#include "../../socket/GameMsg_Base.h"

#include <string>

class CParamPool;

class GameMsg_C2S_BugReport :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_BugReport);
	GameMsg_C2S_BugReport(void);
	virtual ~GameMsg_C2S_BugReport(void);

public:
	bool doDecode(CParamPool &IOBuff);

public:
	std::string m_strBugDesc;
};

#endif //_GAMEMSG_C2S_BUGREPORT_H_

