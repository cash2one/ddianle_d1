#include "GameMsg_Server_State.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include <string.h>

GameMsg_Server_State::GameMsg_Server_State(void):_baseClass(MSG_SERVER_State)
{
	memset(aState,0,sizeof(unsigned short)*LINES_IN_GROUP);
}

GameMsg_Server_State::~GameMsg_Server_State(void)
{
}

bool GameMsg_Server_State::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddUInt(LINES_IN_GROUP);
	for(int i = 0 ;i< LINES_IN_GROUP ; i++ )
	{
		IoBuff.AddUShort(aState[i]);
	}
	return true;
}


//end file


