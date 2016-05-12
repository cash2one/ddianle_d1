#ifndef __GAMEMSG_G2S_FRIENDAPPLICATIONBACK2SENDER_H__
#define __GAMEMSG_G2S_FRIENDAPPLICATIONBACK2SENDER_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>

using namespace std;
class CParamPool;

enum
{
	APPLICATION_SENT,
	PLAYER_OFFLINE,
	PLAYER_PUTINBLACKLIST,
	PLAYER_NOTSAMELINE,
};

class GameMsg_G2S_FriendApplicationBack2Sender:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_FriendApplicationBack2Sender);
	GameMsg_G2S_FriendApplicationBack2Sender();
	~GameMsg_G2S_FriendApplicationBack2Sender();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nResult; // 0表示成功 1表示失败
	string m_strFriendName;
};





#endif

// end of file

