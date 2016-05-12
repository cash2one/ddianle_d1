#ifndef __GameMsg_S2C_Chat_Priv_H__
#define __GameMsg_S2C_Chat_Priv_H__

#include "../socket/GameMsg_Base.h"
#include "Macro_Define.h"
#include <string>

//服务器发往客户端私聊消息,此消息是私聊时，给对方的
using namespace std;
class CParamPool;
typedef class GameMsg_S2C_Chat_Priv :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_Chat_Priv);
	GameMsg_S2C_Chat_Priv(void);
	~GameMsg_S2C_Chat_Priv(void);

	unsigned int	nEntityID;//发送者id
	unsigned short  dwColor[CHATCOLORTYPE];
	string          strSenderName;//发送者名称
	string          strContext;//聊天的文字
	bool			m_bVoiceChat;

	static GameMsg_S2C_Chat_Priv* Create(const char* szSenderName,unsigned int _nEntityID,const char* szContext , unsigned short dwColor1 =0xffff,unsigned short dwColor2 =0x0000,
		bool bVoiceChat=false );

public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
}* LPGameMsg_S2C_Chat_Priv ;

#endif

