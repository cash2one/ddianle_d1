/*
* 角色创建成功	
*  
*/
#ifndef __GameMsg_SERVERACCOUNTLOGIN_H__
#define __GameMsg_SERVERACCOUNTLOGIN_H__
#include "../../socket/GameMsg_Base.h"
class CParamPool;
class GameMsg_Server_AccountLogin :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Server_AccountLogin);
	GameMsg_Server_AccountLogin(void);
	~GameMsg_Server_AccountLogin(void);
	unsigned int nAccount;
public:
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
};
#endif


//file end

