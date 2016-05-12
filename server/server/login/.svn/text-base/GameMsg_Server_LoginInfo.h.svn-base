/*
* 账号登陆返回gateserver记录账号的信息
*
*/
#ifndef __GameMsg_Server_LOGINFO_H__
#define __GameMsg_Server_LOGINFO_H__

#include "../../socket/GameMsg_Base.h"

class GameMsg_Server_LoginInfo :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Server_LoginInfo);
	GameMsg_Server_LoginInfo(void);
	~GameMsg_Server_LoginInfo(void);

	unsigned int nAccount;
public:
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
};

#endif

//file end

