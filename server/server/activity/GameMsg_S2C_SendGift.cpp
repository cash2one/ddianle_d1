#include "GameMsg_S2C_SendGift.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_SendGift::GameMsg_S2C_SendGift(int nGiftID, std::list< CItem > & rRewardList, int nRewardMoney, std::string & rstrMessage)
: GameMsg_Base(MSG_S2C_SendGift)
{
	m_nGiftID = nGiftID;
	m_RewardList = rRewardList;
	m_nRewardMoney = nRewardMoney;
	m_strMessage = rstrMessage;
}

GameMsg_S2C_SendGift::~GameMsg_S2C_SendGift()
{
}
bool GameMsg_S2C_SendGift::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nGiftID);
	IOBuff.AddInt(m_nRewardMoney);
	IOBuff.AddStringW(m_strMessage.c_str());

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




