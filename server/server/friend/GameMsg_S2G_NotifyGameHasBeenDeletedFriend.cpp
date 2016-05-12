#include "GameMsg_S2G_NotifyGameHasBeenDeletedFriend.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_NotifyGameHasBeenDeletedFriend::GameMsg_S2G_NotifyGameHasBeenDeletedFriend(unsigned int nRoleID, unsigned int nRemovedID):
GameMsg_Base(MSG_S2G_NotifyGameHasBeenDeletedFriend),
m_nRoleID(nRoleID),
m_nRemovedID(nRemovedID)
{
}

GameMsg_S2G_NotifyGameHasBeenDeletedFriend::~GameMsg_S2G_NotifyGameHasBeenDeletedFriend()
{

}

bool GameMsg_S2G_NotifyGameHasBeenDeletedFriend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nRemovedID);

	return true;
}

bool GameMsg_S2G_NotifyGameHasBeenDeletedFriend::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nRemovedID = IOBuff.GetUInt();

	return true;
}




// end of file

