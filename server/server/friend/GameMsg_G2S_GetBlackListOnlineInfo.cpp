#include "GameMsg_G2S_GetBlackListOnlineInfo.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_GetBlackListOnlineInfo::GameMsg_G2S_GetBlackListOnlineInfo()
:GameMsg_Base(MSG_G2S_GetBlackListOnlineInfo)
{

}

GameMsg_G2S_GetBlackListOnlineInfo::~GameMsg_G2S_GetBlackListOnlineInfo()
{

}

bool GameMsg_G2S_GetBlackListOnlineInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_listBlackOnlineInfo.size());
	for (list<CFriendOnlineInfo>::iterator itr = m_listBlackOnlineInfo.begin(); itr != m_listBlackOnlineInfo.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	return true;
}

bool GameMsg_G2S_GetBlackListOnlineInfo::doDecode(CParamPool &IOBuff)
{
	int nCount = IOBuff.GetUShort();

	while ( nCount-- > 0 ) {
		CFriendOnlineInfo info;
		info.doDecode(IOBuff);
		m_listBlackOnlineInfo.push_back(info);
	}

	return true;
}






