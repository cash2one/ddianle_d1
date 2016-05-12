#ifndef __GameMsg_Server_STATE_H__
#define __GameMsg_Server_STATE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

//ÓÎÏ··þÎñÆ÷×´Ì¬
enum
{
	LineState_Good, 
	LineState_Normal, 
	LineState_Bad, 
	LineState_Disable , 
	LineState_Full
};
class CParamPool;
class GameMsg_Server_State :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_Server_State(void);
	~GameMsg_Server_State(void);
	unsigned short aState[LINES_IN_GROUP];
public:
	virtual bool doEncode( CParamPool& IoBuff );
};

#endif


//end file


