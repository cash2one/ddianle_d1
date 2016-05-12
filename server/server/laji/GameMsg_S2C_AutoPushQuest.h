#ifndef		__GAMEMSG_S2C_AUTOPUSHQUEST_H__
#define		__GAMEMSG_S2C_AUTOPUSHQUEST_H__ 

#include "../socket/GameMsg_Base.h"

class CParamPool;
class CQuestDoc;

class GameMsg_S2C_AutoPushQuest:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AutoPushQuest();
	~GameMsg_S2C_AutoPushQuest();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	//CQuestDoc m_stQuestInfo;
};

#endif
// end of file
