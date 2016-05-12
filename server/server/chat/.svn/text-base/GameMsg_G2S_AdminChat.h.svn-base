#ifndef		__GAMEMSG_G2S_ADMINCHAT_H__
#define		__GAMEMSG_G2S_ADMINCHAT_H__ 

#include <string>
#include "../../socket/GameMsg_Base.h"

class CParamPool;

using namespace std;

class GameMsg_G2S_AdminChat:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_AdminChat);
	GameMsg_G2S_AdminChat(unsigned int nSenderRoleID = 0, const char *szSenderName = "", const char *szText = "");
	~GameMsg_G2S_AdminChat();
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int	m_nSenderRoleID;		
	string			m_strSenderName;	
	string			m_strText;
};


#endif

