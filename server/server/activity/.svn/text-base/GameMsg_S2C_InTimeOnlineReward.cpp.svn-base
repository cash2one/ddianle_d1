#include "GameMsg_S2C_InTimeOnlineReward.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_InTimeOnlineReward::GameMsg_S2C_InTimeOnlineReward(unsigned char nHour, unsigned char nMinute, std::list< CItem > & rRewardList, int nRewardMoney)
: GameMsg_Base(MSG_S2C_InTimeOnlineReward)
{
	m_nRewardHour = nHour;
	m_nRewardMinute = nMinute;
	m_RewardList = rRewardList;
	m_nRewardMoney = nRewardMoney;
}

GameMsg_S2C_InTimeOnlineReward::~GameMsg_S2C_InTimeOnlineReward()
{
}

bool GameMsg_S2C_InTimeOnlineReward::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nRewardHour);
	IOBuff.AddUChar(m_nRewardMinute);
	IOBuff.AddInt(m_nRewardMoney);

	IOBuff.AddUShort((unsigned short)m_RewardList.size());
	if (!m_RewardList.empty())
	{
		std::list< CItem >::iterator itr = m_RewardList.begin();
		for (; itr != m_RewardList.end(); ++itr)
		{
			(*itr).doEncode(IOBuff);
		}
	}
	return true;
}

// end of file




