#ifndef __GameMsg_Server_LOGINRESULT_H__
#define __GameMsg_Server_LOGINRESULT_H__
#include "../../socket/GameMsg_Base.h"

class GameMsg_Server_LoginResult :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_Server_LoginResult(void);
	~GameMsg_Server_LoginResult(void);

	short nErrCode;
public:
	virtual bool doEncode( CParamPool& IoBuff );
};
#endif


//file end

