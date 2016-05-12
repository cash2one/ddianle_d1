#ifndef		__GAMEMSG_G2S_WORLDCHAT_H__
#define		__GAMEMSG_G2S_WORLDCHAT_H__ 

#include <string>
#include "../../socket/GameMsg_Base.h"

class CParamPool;

using namespace std;

class GameMsg_G2S_WorldChat:
public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_WorldChat);
	GameMsg_G2S_WorldChat();
	GameMsg_G2S_WorldChat(unsigned int nSenderRoleID, const char *szSenderName, const char *szText, unsigned char nClickSex);
	~GameMsg_G2S_WorldChat();
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int	m_nSenderRoleID;		
	string			m_strSenderName;	
	string			m_strText;
	unsigned char	m_nClickSex;
};


#endif

