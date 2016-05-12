#ifndef	__GAMEMSG_S2G_SWITCHLINECOMPLETE_H__
#define	__GAMEMSG_S2G_SWITCHLINECOMPLETE_H__ 

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/datainfor/ItemType.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_SwitchLineComplete:
	public GameMsg_Base
{
public:
	GameMsg_S2G_SwitchLineComplete();
	~GameMsg_S2G_SwitchLineComplete();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	int m_nLine;
	string m_strErrorMsg;
};

#endif


