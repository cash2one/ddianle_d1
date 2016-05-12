#ifndef __GameMsg_SERVERACCOUNTLOGOUT_H__
#define __GameMsg_SERVERACCOUNTLOGOUT_H__
#include "../../socket/GameMsg_Base.h"
class CParamPool;
class GameMsg_Server_AccountLogout :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Server_AccountLogout);
	GameMsg_Server_AccountLogout(unsigned int _nAccout = 0);
	~GameMsg_Server_AccountLogout(void);
	unsigned int nAccount;
public :
	virtual bool doEncode( CParamPool& IoBuff );
	virtual bool doDecode( CParamPool& IoBuff );
};
#endif


//file end

