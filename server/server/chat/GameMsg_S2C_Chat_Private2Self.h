#ifndef	__GAMEMSG_S2C_CHAT_PRIVATE2SELF_H__
#define	__GAMEMSG_S2C_CHAT_PRIVATE2SELF_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>
//服务器发往客户端的除私聊以外的聊天消息
using namespace std;
class CParamPool;

class GameMsg_S2C_Chat_Private2Self:
	public GameMsg_Base
{
public:
	GameMsg_S2C_Chat_Private2Self();
	GameMsg_S2C_Chat_Private2Self(unsigned int nReceiverRoleID, const char *szReceiverName, unsigned char nColor, const char *szText, unsigned char nClickSex, bool bVoiceChat,unsigned char cWorldChatFlag, bool bReceiverIsVIP, unsigned short nReceiverVIPLevel);
	~GameMsg_S2C_Chat_Private2Self();
public:
	unsigned int m_nReceiverRoleID;
	string m_strReceiverName;
	unsigned char m_nColor;
	string m_strText;
	unsigned char m_nClickSex;
	bool m_bVoiceChat;
	unsigned char m_WorldChatFlag;
	bool m_bReceiverIsVIP;
	unsigned short m_nReceiverVipLevel;
public:
	bool doEncode(CParamPool &IOBuff);
};


#endif


//end file


