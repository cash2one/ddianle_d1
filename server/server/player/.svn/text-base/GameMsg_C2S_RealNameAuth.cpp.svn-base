#include "GameMsg_C2S_RealNameAuth.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_SynAdult::GameMsg_C2S_SynAdult()
:GameMsg_Base(MSG_C2S_SynAdult)
{
	m_nAdult = 0;
}

GameMsg_C2S_SynAdult::~GameMsg_C2S_SynAdult()
{

}

bool GameMsg_C2S_SynAdult::doDecode( CParamPool &IOBuff )
{
	m_nAdult = IOBuff.GetUChar();
	return true;
}

