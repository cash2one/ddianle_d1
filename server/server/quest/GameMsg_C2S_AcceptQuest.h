#ifndef		__GAMEMSG_C2S_ACCEPTQUEST_H__
#define		__GAMEMSG_C2S_ACCEPTQUEST_H__ 

#include "../../socket/GameMsg_Base.h"
#include "../../nsytworld/QuestDef.h"

class CParamPool;

class GameMsg_C2S_AcceptQuest :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AcceptQuest);
	GameMsg_C2S_AcceptQuest();
	~GameMsg_C2S_AcceptQuest();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nQuestID;
};

class GameMsg_C2S_ClientCompleteQuest : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ClientCompleteQuest);
	GameMsg_C2S_ClientCompleteQuest();
	~GameMsg_C2S_ClientCompleteQuest();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nQuestID;
	EQuest_Complete_Type m_nCompleteType;
	int m_nParam1;
	unsigned short m_nParam2;
};

class GameMsg_C2S_GetQuestState : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetQuestState);
	GameMsg_C2S_GetQuestState();
	~GameMsg_C2S_GetQuestState();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	std::list<unsigned short> m_nQuestIDList;
};

class GameMsg_S2C_GetQuestStateRes :
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetQuestStateRes();
	~GameMsg_S2C_GetQuestStateRes();
public:
	bool doEncode(CParamPool &IOBuff);
	void AddQuestState(unsigned short nQuestID,unsigned short nQuestState);
public:
	std::map<unsigned short,unsigned short> m_QuestStateMap;
};

#endif
// end of file


