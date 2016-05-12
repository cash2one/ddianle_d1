#include "GameMsg_S2C_NewCompensation.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_NewCompensation::GameMsg_S2C_NewCompensation()
: GameMsg_Base(MSG_S2C_CompensationData)
{
}

GameMsg_S2C_NewCompensation::~GameMsg_S2C_NewCompensation()
{
}

bool GameMsg_S2C_NewCompensation::doEncode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_C2S_GetCompensation::GameMsg_C2S_GetCompensation()
:GameMsg_Base(MSG_C2S_GetCompensation)
{

}

GameMsg_C2S_GetCompensation::~GameMsg_C2S_GetCompensation()
{

}

bool GameMsg_C2S_GetCompensation::doDecode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_S2C_GetCompensationSuc::GameMsg_S2C_GetCompensationSuc()
:GameMsg_Base(MSG_S2C_GetCompensationSuc)
,m_nMoney(0)
{

}

GameMsg_S2C_GetCompensationSuc::~GameMsg_S2C_GetCompensationSuc()
{

}

bool GameMsg_S2C_GetCompensationSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strTitle.c_str());
	IOBuff.AddStringW(m_strContent.c_str());

	IOBuff.AddInt(m_nMoney);

	IOBuff.AddShort((short)m_Item.size());
	for (std::list<CItem>::iterator it = m_Item.begin(); it != m_Item.end() ; it++)
	{
		IOBuff.AddUInt(it->m_nItemType);
		IOBuff.AddShort(it->m_nItemCount);
		IOBuff.AddInt(it->m_nValidTime);
	}

	return true;
}

GameMsg_S2C_GetCompensationFail::GameMsg_S2C_GetCompensationFail()
:GameMsg_Base(MSG_S2C_GetCompensationFail)
{

}

GameMsg_S2C_GetCompensationFail::~GameMsg_S2C_GetCompensationFail()
{

}

bool GameMsg_S2C_GetCompensationFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

