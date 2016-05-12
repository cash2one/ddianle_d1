#include "GameMsg_G2S_GetTempListOnlineInfo.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_GetTempListOnlineInfo::GameMsg_G2S_GetTempListOnlineInfo()
:GameMsg_Base(MSG_G2S_GetTempListOnlineInfo)
,m_nListType(0)
{

}

GameMsg_G2S_GetTempListOnlineInfo::~GameMsg_G2S_GetTempListOnlineInfo()
{

}

bool GameMsg_G2S_GetTempListOnlineInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nListType);

	IOBuff.AddUShort((unsigned short)m_listMate.size());
	for (list<CFriendOnlineInfo>::iterator itr = m_listMate.begin(); itr != m_listMate.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	return true;	
}

bool GameMsg_G2S_GetTempListOnlineInfo::doDecode(CParamPool &IOBuff)
{
	m_nListType = IOBuff.GetUChar();

	int nCount = IOBuff.GetUShort();
	while(nCount-- > 0) {
		CFriendOnlineInfo info;
		info.doDecode(IOBuff);
		m_listMate.push_back(info);
	} 

	return true;
}




