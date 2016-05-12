#include "GameMsg_C2S_AwardMsg.h"
#include "../share/ServerMsgDef.h"


///////////////////////////////////////////////////////
GameMsg_C2S_ItemAward::GameMsg_C2S_ItemAward(void)
:GameMsg_Base(MSG_C2S_ItemAward)
{
}

GameMsg_C2S_ItemAward::~GameMsg_C2S_ItemAward(void)
{
}

bool GameMsg_C2S_ItemAward::doDecode(CParamPool &IOBuff)
{
	IOBuff.GetStringW( m_strItemCode );

	return true;
}

//end

