#include "GameMsg_G2S_NotifyGameHasBeenDeletedFriend.h"
#include "../share/ServerMsgDef.h"


GameMsg_G2S_NotifyGameHasBeenDeletedFriend::GameMsg_G2S_NotifyGameHasBeenDeletedFriend(unsigned int nRoleID, unsigned int nRemovedID):
GameMsg_Base(MSG_G2S_NotifyGameHasBeenDeletedFriend),
m_nRoleID(nRoleID)
{
}

GameMsg_G2S_NotifyGameHasBeenDeletedFriend::~GameMsg_G2S_NotifyGameHasBeenDeletedFriend()
{

}

bool GameMsg_G2S_NotifyGameHasBeenDeletedFriend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);

	return true;
}

bool GameMsg_G2S_NotifyGameHasBeenDeletedFriend::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();

	return true;
}




// end of file

