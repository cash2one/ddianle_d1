#include "GameMsg_S2G_NotifyHasBeenDeletedFriend.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_NotifyHasBeenDeletedFriend::GameMsg_S2G_NotifyHasBeenDeletedFriend(unsigned int nRoleID, unsigned int nRemovedID):
GameMsg_Base(MSG_S2G_NotifyHasBeenDeletedFriend),
m_nRoleID(nRoleID),
m_nRemovedID(nRemovedID)
{
}

GameMsg_S2G_NotifyHasBeenDeletedFriend::~GameMsg_S2G_NotifyHasBeenDeletedFriend()
{

}

bool GameMsg_S2G_NotifyHasBeenDeletedFriend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nRemovedID);

	return true;
}

bool GameMsg_S2G_NotifyHasBeenDeletedFriend::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nRemovedID = IOBuff.GetUInt();

	return 0;
}




