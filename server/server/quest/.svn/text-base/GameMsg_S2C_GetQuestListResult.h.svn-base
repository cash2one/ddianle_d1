#ifndef		__GAMEMSG_S2C_GETLISTRESULT_H__
#define		__GAMEMSG_S2C_GETLISTRESULT_H__ 

#include <list>
#include "../../socket/GameMsg_Base.h"
class CQuestListEntry;

class CParamPool;
using namespace std;

class GameMsg_S2C_GetQuestListResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetQuestListResult();
	~GameMsg_S2C_GetQuestListResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	list<CQuestListEntry> m_listQuestInfo;
};

#endif
// end of file


