#include "GameMsg_G2S_FindPlayerFromID.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_FindPlayerResult::GameMsg_G2S_FindPlayerResult()
:GameMsg_Base(MSG_G2S_FindPlayerResult)
{
	m_strError = "";
}

GameMsg_G2S_FindPlayerResult::~GameMsg_G2S_FindPlayerResult()
{

}

bool GameMsg_G2S_FindPlayerResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddString(m_strError.c_str());

	IOBuff.AddUShort((unsigned short)m_PlayerList.size());

	std::list<CFindPlayerInfo>::iterator it;
	for (it = m_PlayerList.begin(); it != m_PlayerList.end(); ++it)
	{
		it->doEncode(IOBuff);
	}

	return true;
}

bool GameMsg_G2S_FindPlayerResult::doDecode(CParamPool &IOBuff)
{
	IOBuff.GetString(m_strError);

	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0; i < nCount; i++)
	{
		CFindPlayerInfo info;
		info.doDecode(IOBuff);
		m_PlayerList.push_back(info);
	}

	return true;
}

