#include "GameMsg_Player_SwitchLine.h"
#include "../account/AccountMsgDef.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"

GameMsg_C2S_SwitchLine::GameMsg_C2S_SwitchLine(int nLine, unsigned char nLocation):
_baseClass(MSG_C2S_SwitchLine),
m_nLine(nLine),
m_nLoaction(nLocation)
{
}

GameMsg_C2S_SwitchLine::~GameMsg_C2S_SwitchLine(void)
{
}

bool GameMsg_C2S_SwitchLine::doDecode( CParamPool& IoBuff )
{
	m_nLine = IoBuff.GetInt();
	m_nLoaction = IoBuff.GetUChar();

	return true;
}

GameMsg_S2C_SwitchLineRes::GameMsg_S2C_SwitchLineRes(int nDstLine, unsigned char nLocation, unsigned char nResult):
GameMsg_Base(MSG_S2C_SwitchLineRes),
m_nResult(nResult),
m_nDstLine(nDstLine),
m_nLocation(nLocation)
{
}

GameMsg_S2C_SwitchLineRes::~GameMsg_S2C_SwitchLineRes()
{
}

bool GameMsg_S2C_SwitchLineRes::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddInt(m_nDstLine);
	IOBuff.AddUChar(m_nLocation);

	return true;
}

GameMsg_S2A_PlayerSwithLine::GameMsg_S2A_PlayerSwithLine(int nSrcLine, int nDstLine):
GameMsg_Base(MSG_S2A_PlayerSwitchLine),
m_nSrcLine(nSrcLine),
m_nDstLine(nDstLine)
{
}

GameMsg_S2A_PlayerSwithLine::~GameMsg_S2A_PlayerSwithLine()
{
}

bool GameMsg_S2A_PlayerSwithLine::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddInt(m_nSrcLine);
	IOBuff.AddInt(m_nDstLine);

	return true;
}

bool GameMsg_S2A_PlayerSwithLine::doDecode(CParamPool & IOBuff)
{
	m_nSrcLine = IOBuff.GetInt();
	m_nDstLine = IOBuff.GetInt();

	return true;
}







