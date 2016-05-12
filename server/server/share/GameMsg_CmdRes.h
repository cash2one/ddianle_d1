#ifndef __GameMsg_CmdRes_H__
#define __GameMsg_CmdRes_H__

#include "../../socket/GameMsg_Base.h"
#include <string>

using namespace std;
class CParamPool;
class GameMsg_CmdRes :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_CmdRes(void);
	~GameMsg_CmdRes(void);
	string strRes;
public:
	virtual bool doEncode( CParamPool & IoBuff );

};
#endif

//file end

