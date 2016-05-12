#include "GameMsg_S2G_TimeEgg.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_CreateTimeEgg::GameMsg_S2G_CreateTimeEgg()
:GameMsg_Base(MSG_S2G_CreateTimeEgg)
{
	m_nEggID = 0;
}

GameMsg_S2G_CreateTimeEgg::~GameMsg_S2G_CreateTimeEgg()
{

}

bool GameMsg_S2G_CreateTimeEgg::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nEggID);
	return true;
}

bool GameMsg_S2G_CreateTimeEgg::doDecode( CParamPool &IOBuff )
{
	m_nEggID = IOBuff.GetUInt();
	return true;
}


GameMsg_S2G_OpenTimeEgg::GameMsg_S2G_OpenTimeEgg()
:GameMsg_Base(MSG_S2G_OpenTimeEgg)
{
	m_nEggID = 0;
}

GameMsg_S2G_OpenTimeEgg::~GameMsg_S2G_OpenTimeEgg()
{

}

bool GameMsg_S2G_OpenTimeEgg::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nEggID);
	return true;
}

bool GameMsg_S2G_OpenTimeEgg::doDecode( CParamPool &IOBuff )
{
	m_nEggID = IOBuff.GetUInt();
	return true;
}

GameMsg_S2G_TimeEggSetting::GameMsg_S2G_TimeEggSetting()
:GameMsg_Base(MSG_S2G_TimeEggSetting)
{

}

GameMsg_S2G_TimeEggSetting::~GameMsg_S2G_TimeEggSetting()
{

}

bool GameMsg_S2G_TimeEggSetting::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_setting;
	return true;
}

bool GameMsg_S2G_TimeEggSetting::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_setting;
	return true;
}

