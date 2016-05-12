#include "GameMsg_TimeEggMsg.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"

GameMsg_C2S_QueryTimeEggReward::GameMsg_C2S_QueryTimeEggReward()
:GameMsg_Base(MSG_C2S_QueryTimeEggReward)
{

}

GameMsg_C2S_QueryTimeEggReward::~GameMsg_C2S_QueryTimeEggReward()
{

}

bool GameMsg_C2S_QueryTimeEggReward::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_S2C_QueryTimeEggReward::GameMsg_S2C_QueryTimeEggReward()
:GameMsg_Base(MSG_S2C_QueryTimeEggReward)
{

}

GameMsg_S2C_QueryTimeEggReward::~GameMsg_S2C_QueryTimeEggReward()
{

}

bool GameMsg_S2C_QueryTimeEggReward::doEncode( CParamPool &IOBuff )
{
	::doEncode(IOBuff, m_listReward);
	::doEncode(IOBuff, m_listTogetherReward);
	IOBuff.AddUInt(m_nFixedMoneyReward);
	IOBuff.AddUInt(m_nExtraMoneyReward);
	return true;
}

GameMsg_S2C_SynRoleTimeEggInfo::GameMsg_S2C_SynRoleTimeEggInfo()
:GameMsg_Base(MSG_S2C_SynRoleTimeEggInfo)
{
}

GameMsg_S2C_SynRoleTimeEggInfo::~GameMsg_S2C_SynRoleTimeEggInfo()
{

}

bool GameMsg_S2C_SynRoleTimeEggInfo::doEncode( CParamPool &IOBuff )
{
	unsigned int nNow = (unsigned int)time(NULL);
	IOBuff.AddUShort((unsigned short)m_eggs.size());
	for (std::vector<TimeEggEntry*>::iterator it = m_eggs.begin();
		it != m_eggs.end(); ++it)
	{
		TimeEggEntry* pEgg = (*it);
		IOBuff.AddUInt(pEgg->m_nEggID);
		IOBuff.AddUInt(pEgg->m_nSceneID);
		IOBuff.AddInt(pEgg->m_nEndTime - nNow);//倒计时，秒，可能是负值
		IOBuff.AddUInt(pEgg->m_nRoleID);
		IOBuff.AddStringW(pEgg->m_strRoleName);
	}
	IOBuff.AddStringW(m_strHelpTitle.c_str());
	IOBuff.AddStringW(m_strHelpContent.c_str());

	return true;
}

