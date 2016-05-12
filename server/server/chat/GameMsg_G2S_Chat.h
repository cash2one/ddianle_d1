#ifndef __GAMEMSG_G2S_CHAT_H__
#define __GAMEMSG_G2S_CHAT_H__

#include "../../socket/GameMsg_Base.h"
#include <string>
#include "../../datastructure/Macro_Define.h"
#include "../chatGrp/GroupChatHistory.h"

//客户端发往服务器的除私聊以外的聊天消息
using namespace std;
class CParamPool;

class CChatHistory;

class GameMsg_G2S_Chat :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_Chat);
	GameMsg_G2S_Chat(void);
	GameMsg_G2S_Chat(unsigned char nPhrase, unsigned char nResult, unsigned char nChannel, unsigned char nColor, 
					 unsigned int SenderRoleID, unsigned int nReceiverRoleID, const char * szSenderName, const char* szReceiverName, 
					 const char *szText, unsigned char nClickSex, EChatExtraType nExtraType,
					 bool bIsVIP, unsigned short nVIPLevel, bool bVoiceChat,unsigned char cWorldChatFlag);	
	~GameMsg_G2S_Chat(void);
public:
	unsigned char	m_nPhase;			//发送过程的状态
	unsigned char	m_nResult;			//涉及跨服交互的聊天需填充
	unsigned char	m_nChannel;			//频道
	unsigned char	m_nColor;
	unsigned int	m_nSenderRoleID;		
	unsigned int	m_nReceiverRoleID;	//如果是私聊，接收者的roleid
	string			m_strSenderName;	
	string			m_strReceiverName;	
	string          m_strText;			//聊天的文字
	unsigned char   m_nClickSex;
	EChatExtraType  m_nExtraType;
	bool			m_bIsVIP;
	unsigned short	m_nVIPLevel;
	bool			m_bVoiceChat;
	unsigned char   m_WorldChatFlag;
public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
};

class GameMsg_G2S_GroupDanceChatHistory : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_GroupDanceChatHistory);
    GameMsg_G2S_GroupDanceChatHistory(CChatHistory &rhistory);
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned int m_nSenderRoleID;
    unsigned char m_nColor;
    unsigned char m_nChannel;
    std::string m_strSenderName;
    std::string m_strText;
    unsigned char m_nClickSex;
    int m_nExtraType;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    bool m_bVoiceChat;
    unsigned char m_WorldChatFlag;
};


#endif


//file end

