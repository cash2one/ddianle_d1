#include "GameMsg_S2C_UpdateGiftInfo.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_UpdateGiftInfo::GameMsg_S2C_UpdateGiftInfo(int nIndexID, int nGiftID, unsigned int nExpiredTime, std::list< CItem > & rRewardList, int nRewardMoney)
: GameMsg_Base(MSG_S2C_UpdateGiftInfo)
{
	m_nIndexID = nIndexID;
	m_nGiftID = nGiftID;
	m_nExpiredTime = nExpiredTime;
	m_RewardList = rRewardList;
	m_nRewardMoney = nRewardMoney;
}

GameMsg_S2C_UpdateGiftInfo::~GameMsg_S2C_UpdateGiftInfo()
{
}
bool GameMsg_S2C_UpdateGiftInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nIndexID);
	IOBuff.AddInt(m_nGiftID);
	IOBuff.AddUInt(m_nExpiredTime);
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




