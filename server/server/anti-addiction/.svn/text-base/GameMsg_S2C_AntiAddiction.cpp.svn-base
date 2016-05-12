#include "GameMsg_S2C_AntiAddiction.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_AntiAddictionTip::GameMsg_S2C_AntiAddictionTip( void )
:GameMsg_Base(MSG_S2C_AntiAddictionTip)
{

}

GameMsg_S2C_AntiAddictionTip::~GameMsg_S2C_AntiAddictionTip( void )
{

}

bool GameMsg_S2C_AntiAddictionTip::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddStringW(m_strTipText.c_str());
	return true;
}


