#include "GameMsg_S2G_UpdateRoomInfo.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_UpdateRoomInfo::GameMsg_S2G_UpdateRoomInfo():
GameMsg_Base(MSG_S2G_UpdateRoomInfo)
,m_nRoleID(0)
{
}

GameMsg_S2G_UpdateRoomInfo::~GameMsg_S2G_UpdateRoomInfo()
{
}

bool GameMsg_S2G_UpdateRoomInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nRoleID );
	m_RoomUpdateInfo.doEncode( IOBuff );

	return true;
}

bool GameMsg_S2G_UpdateRoomInfo::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_RoomUpdateInfo.doDecode(IOBuff);

	return true;
}


// end of file



