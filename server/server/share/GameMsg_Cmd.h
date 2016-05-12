#ifndef __GameMsg_Cmd_H__
#define __GameMsg_Cmd_H__
#include "../../socket/GameMsg_Base.h"
#include <string>


using namespace std;
class CParamPool;
class GameMsg_Cmd :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Cmd);
	GameMsg_Cmd(void);
	~GameMsg_Cmd(void);
	string strCmd;
public:
	virtual bool doDecode(CParamPool & IoBuff);
};
#endif

//file end

