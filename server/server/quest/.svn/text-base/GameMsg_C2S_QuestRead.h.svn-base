#ifndef		__GAMEMSG_C2S_QUESTREAD_H__
#define		__GAMEMSG_C2S_QUESTREAD_H__ 

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_QuestRead:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_QuestRead);
	GameMsg_C2S_QuestRead();
	~GameMsg_C2S_QuestRead();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nQuestID;
};

#endif
// end of file


