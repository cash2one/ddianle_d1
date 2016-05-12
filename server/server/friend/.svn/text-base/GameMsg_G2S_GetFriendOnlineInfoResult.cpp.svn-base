#include "GameMsg_G2S_GetFriendOnlineInfoResult.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_GetFriendOnlineInfoResult::GameMsg_G2S_GetFriendOnlineInfoResult():
GameMsg_Base(MSG_G2S_GetFriendOnlineInfoResult)
{
}

GameMsg_G2S_GetFriendOnlineInfoResult::~GameMsg_G2S_GetFriendOnlineInfoResult()
{
}

bool GameMsg_G2S_GetFriendOnlineInfoResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_listFriendOnlineInfo.size());
	if (m_listFriendOnlineInfo.size() > 0)
	{
		for (list<CFriendOnlineInfo>::iterator itr = m_listFriendOnlineInfo.begin(); itr != m_listFriendOnlineInfo.end(); itr++)
		{
			itr->doEncode(IOBuff);
		}
	}
	
	return true;
}

bool GameMsg_G2S_GetFriendOnlineInfoResult::doDecode(CParamPool &IOBuff)
{
	unsigned short nSize = (unsigned short)IOBuff.GetUShort();

	while (nSize-- > 0)
	{
		CFriendOnlineInfo	info;
		info.doDecode(IOBuff);

		m_listFriendOnlineInfo.push_back(info);
	}

	return true;
}


// end of file

