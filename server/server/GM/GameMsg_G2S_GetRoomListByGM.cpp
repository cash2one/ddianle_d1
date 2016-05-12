#include "GameMsg_G2S_GetRoomListByGM.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_GetRoomListByGM::GameMsg_G2S_GetRoomListByGM()
	: GameMsg_Base(MSG_G2S_GetRoomListByGM)
{
	m_nSlot = 0;
	m_nGMAskRoomType = 0;
}

GameMsg_G2S_GetRoomListByGM::~GameMsg_G2S_GetRoomListByGM()
{

}

bool GameMsg_G2S_GetRoomListByGM::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);
	IOBuff.AddUShort(m_nGMAskRoomType);
	return true;
}

bool GameMsg_G2S_GetRoomListByGM::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	m_nGMAskRoomType = IOBuff.GetUShort();
	return true;
}

GameMsg_S2G_GetRoomListByGMRes::GameMsg_S2G_GetRoomListByGMRes()
	: GameMsg_Base(MSG_S2G_GetRoomListByGMRes)
{
	m_nSlot = 0;
}

GameMsg_S2G_GetRoomListByGMRes::~GameMsg_S2G_GetRoomListByGMRes()
{

}

bool GameMsg_S2G_GetRoomListByGMRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nSlot);
	IOBuff.AddUShort((unsigned short)m_RoomList.size());
	std::vector<GMRoomInfo>::iterator it = m_RoomList.begin();
	for (; it != m_RoomList.end() ; it++ )
	{
		it->doEncode(IOBuff);
	}
	return true;
}

bool GameMsg_S2G_GetRoomListByGMRes::doDecode(CParamPool &IOBuff)
{
	m_nSlot = IOBuff.GetUShort();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		GMRoomInfo roomInfo;
		roomInfo.doDecode(IOBuff);
		m_RoomList.push_back(roomInfo);
	}
	return true;
}
