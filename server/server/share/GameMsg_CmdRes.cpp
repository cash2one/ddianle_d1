#include "GameMsg_CmdRes.h"
#include "../../socket/ParamPool.h"
#include "ServerMsgDef.h"

GameMsg_CmdRes::GameMsg_CmdRes():_baseClass(MSG_SERVER_Command)
{

}
GameMsg_CmdRes::~GameMsg_CmdRes()
{

}

bool GameMsg_CmdRes::doEncode( CParamPool & IoBuff )
{
	IoBuff.AddString(strRes.c_str());
	return true;
}


//file end

