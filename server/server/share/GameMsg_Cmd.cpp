#include "GameMsg_Cmd.h"
#include "../../socket/ParamPool.h"
#include "ServerMsgDef.h"

GameMsg_Cmd::GameMsg_Cmd():_baseClass(MSG_SERVER_Command)
{

}
GameMsg_Cmd::~GameMsg_Cmd()
{

}
bool GameMsg_Cmd::doDecode(CParamPool & IoBuff)
{
	strCmd = IoBuff.GetString();
	return true;
}

//file end

