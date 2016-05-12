#include "GameMsg_G2S_FriendApplicationBack2Sender.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Utility.h"
#include "../../datastructure/datainfor/ItemType.h"


GameMsg_G2S_FriendApplicationBack2Sender::GameMsg_G2S_FriendApplicationBack2Sender()
:GameMsg_Base(MSG_G2S_FriendApplicationBack2Sender)
,m_nResult(0)
{
}

GameMsg_G2S_FriendApplicationBack2Sender::~GameMsg_G2S_FriendApplicationBack2Sender()
{
}

bool GameMsg_G2S_FriendApplicationBack2Sender::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddString(m_strFriendName.c_str());

	return true;
}

bool GameMsg_G2S_FriendApplicationBack2Sender::doDecode(CParamPool &IOBuff)
{
	m_nResult = IOBuff.GetUChar();
	m_strFriendName = IOBuff.GetString();

	return true;
}

// end of file

