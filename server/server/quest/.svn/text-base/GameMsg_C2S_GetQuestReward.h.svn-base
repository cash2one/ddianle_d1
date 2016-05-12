#ifndef		__GAMEMSG_C2S_GETQUESTREWARD_H__
#define		__GAMEMSG_C2S_GETQUESTREWARD_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_GetQuestReward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetQuestReward);
	GameMsg_C2S_GetQuestReward();
	~GameMsg_C2S_GetQuestReward();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nQuestID;
	unsigned short m_ItemChoiceID;	// indexº¥ «ŒÔ∆∑ID
};

class GameMsg_C2S_GetFresherQuestReward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFresherQuestReward);
	GameMsg_C2S_GetFresherQuestReward();
	~GameMsg_C2S_GetFresherQuestReward();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nQuestID;
};

#endif
// end of file


