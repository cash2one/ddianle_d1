#ifndef __GAMEMSG_G2S_CHAT_ADMIN_H__
#define __GAMEMSG_G2S_CHAT_ADMIN_H__

#include "../../socket/GameMsg_Base.h"
#include <string>

using namespace std;

class GameMsg_G2S_Chat_Admin:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_Chat_Admin);
	GameMsg_G2S_Chat_Admin();
	GameMsg_G2S_Chat_Admin(unsigned char nColor, const char *szText);
	~GameMsg_G2S_Chat_Admin();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nColor;
	string m_strText;
};






#endif


//file end

