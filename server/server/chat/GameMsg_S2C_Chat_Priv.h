#ifndef __GameMsg_S2C_Chat_Priv_H__
#define __GameMsg_S2C_Chat_Priv_H__

#include "../socket/GameMsg_Base.h"
#include "Macro_Define.h"
#include <string>

//�����������ͻ���˽����Ϣ,����Ϣ��˽��ʱ�����Է���
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

	unsigned int	nEntityID;//������id
	unsigned short  dwColor[CHATCOLORTYPE];
	string          strSenderName;//����������
	string          strContext;//���������
	bool			m_bVoiceChat;

	static GameMsg_S2C_Chat_Priv* Create(const char* szSenderName,unsigned int _nEntityID,const char* szContext , unsigned short dwColor1 =0xffff,unsigned short dwColor2 =0x0000,
		bool bVoiceChat=false );

public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
}* LPGameMsg_S2C_Chat_Priv ;

#endif

