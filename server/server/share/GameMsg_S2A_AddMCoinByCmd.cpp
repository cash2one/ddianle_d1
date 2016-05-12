#include "GameMsg_S2A_AddMCoinByCmd.h"
#include "../../socket/ParamPool.h"
#include "ServerMsgDef.h"

GameMsg_S2A_AddMCoinByCmd::GameMsg_S2A_AddMCoinByCmd(void)
:_baseClass(MSG_S2A_AddMCoinByCmd)
{
	nAccount = 0;
	nAmount = 0;
}

GameMsg_S2A_AddMCoinByCmd::~GameMsg_S2A_AddMCoinByCmd(void)
{
}

bool GameMsg_S2A_AddMCoinByCmd::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt(nAccount);
	IOBuff.AddUInt(nAmount);

	return true;
}

bool GameMsg_S2A_AddMCoinByCmd::doDecode( CParamPool& IoBuff )
{
	nAccount = IoBuff.GetUInt();
	nAmount = IoBuff.GetUInt();
	return true;
}




GameMsg_S2A_AddMCoinByGm::GameMsg_S2A_AddMCoinByGm(void)
:_baseClass(MSG_S2A_AddMCoinByGm)
{
	nPid = 0;
	nAmount = 0;
	nBindAmount = 0;
	m_nCurrency = 0;
}

GameMsg_S2A_AddMCoinByGm::~GameMsg_S2A_AddMCoinByGm(void)
{
}

bool GameMsg_S2A_AddMCoinByGm::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort(nPid);
	IOBuff.AddString(strPUID.c_str());
	IOBuff.AddUInt(nAmount);
	IOBuff.AddUInt(nBindAmount);
	IOBuff.AddString(strTransID.c_str());
	IOBuff.AddUInt(m_nCurrency);

	return true;
}

bool GameMsg_S2A_AddMCoinByGm::doDecode( CParamPool& IoBuff )
{
	nPid = IoBuff.GetUShort();
	strPUID = IoBuff.GetString();
	nAmount = IoBuff.GetUInt();
	nBindAmount = IoBuff.GetUInt();
	strTransID = IoBuff.GetString();
	m_nCurrency = IoBuff.GetUInt();
	return true;
}

