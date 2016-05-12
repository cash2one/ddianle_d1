#ifndef __GAMEMSG_C2S_CHAT_PRIV_H__
#define __GAMEMSG_C2S_CHAT_PRIV_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>
//�ͻ��˷���������˽����Ϣ
using namespace std;
class CParamPool;

class GameMsg_C2S_Chat_Priv :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_Chat_Priv);
	GameMsg_C2S_Chat_Priv(void);
	~GameMsg_C2S_Chat_Priv(void);

	unsigned int	m_nReceiverRoleID;//˽�Ķ���roleID ��˽��ʱ���һ��Ҫ��

	static GameMsg_C2S_Chat_Priv * Create(const char* szContext , unsigned int nEntityIDReceiver, unsigned short dwColor1 =0xffff,unsigned short dwColor2 =0x0000 );
public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );

};
#endif

