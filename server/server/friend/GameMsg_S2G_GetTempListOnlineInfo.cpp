#include "GameMsg_S2G_GetTempListOnlineInfo.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_GetTempListOnlineInfo::GameMsg_S2G_GetTempListOnlineInfo()
:GameMsg_Base(MSG_S2G_GetTempListOnlineInfo)
,m_nRequestRoleID(0)
,m_nListType(0)
{

}

GameMsg_S2G_GetTempListOnlineInfo::~GameMsg_S2G_GetTempListOnlineInfo()
{

}

bool GameMsg_S2G_GetTempListOnlineInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRequestRoleID);
	IOBuff.AddUChar(m_nListType);

	IOBuff.AddUShort((unsigned short)m_listMate.size());
	for (list<unsigned int>::iterator itr = m_listMate.begin(); itr != m_listMate.end(); ++itr) {
		IOBuff.AddUInt(*itr);
	}

	return true;
}

bool GameMsg_S2G_GetTempListOnlineInfo::doDecode(CParamPool &IOBuff)
{
	m_nRequestRoleID = IOBuff.GetUInt();
	m_nListType = IOBuff.GetUChar();

	int nCount = (int)IOBuff.GetUShort();
	while ( nCount-- > 0 ) {
		unsigned int nRoleID = IOBuff.GetUInt();
		m_listMate.push_back(nRoleID);
	}

	return true;
}





