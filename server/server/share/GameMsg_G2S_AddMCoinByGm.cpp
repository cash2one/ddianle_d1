#include "GameMsg_G2S_AddMCoinByGm.h"
#include "ServerMsgDef.h"

GameMsg_G2S_AddMCoinByGm::GameMsg_G2S_AddMCoinByGm():
GameMsg_Base(MSG_G2S_AddMCoinByGm)
{
	m_nPid = 0;
	m_nAmount = 0;
	m_nBindAmount = 0;
	m_nCurrency = 0;
}

GameMsg_G2S_AddMCoinByGm::~GameMsg_G2S_AddMCoinByGm()
{
}

bool GameMsg_G2S_AddMCoinByGm::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nPid);
	IOBuff.AddString(m_strPUID.c_str());
	IOBuff.AddUInt(m_nAmount);
	IOBuff.AddUInt(m_nBindAmount);
	IOBuff.AddString(m_strTransID.c_str());
	IOBuff.AddUInt(m_nCurrency);
	return true;
}

bool GameMsg_G2S_AddMCoinByGm::doDecode(CParamPool &IOBuff)
{
	m_nPid = IOBuff.GetUShort();
	m_strPUID = IOBuff.GetString();
	m_nAmount = IOBuff.GetUInt();
	m_nBindAmount = IOBuff.GetUInt();
	m_strTransID = IOBuff.GetString();
	m_nCurrency = IOBuff.GetUInt();
	return true;
}

