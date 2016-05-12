#ifndef __GameMsg_S2C_REQUIRECREATEROLE_H__
#define __GameMsg_S2C_REQUIRECREATEROLE_H__
#include "../../socket/GameMsg_Base.h"
#include "../share/ServerMsgDef.h"
#include <list>
using namespace std;
class CParamPool;

class GameMsg_S2C_RequireCreateRole :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequireCreateRole);
	GameMsg_S2C_RequireCreateRole();
	~GameMsg_S2C_RequireCreateRole();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::list<std::string> m_listLoadingAD;
};

#endif
//file end

