// 任务如果发生变化，如：推送新任务，任务达到完成条件
// 服务端将主动发送该消息
#ifndef		__GAMEMSG_S2C_QUESTSTATECHANGE_H__
#define		__GAMEMSG_S2C_QUESTSTATECHANGE_H__ 

#include "../../socket/GameMsg_Base.h"
#include "QuestListEntry.h"
#include "QuestTargetSend.h"

class CParamPool; 

class GameMsg_S2C_QuestStateChange:
	public GameMsg_Base
{
public:
	GameMsg_S2C_QuestStateChange(unsigned short nQuestID = 0, unsigned char nState = 0);
	~GameMsg_S2C_QuestStateChange();
public:
	bool doEncode(CParamPool &IOBuff);
	void SetListEntry(CQuestListEntry& listEntry);
public:
	unsigned short m_nQuestID;
	unsigned char m_nState;
	list<CQuestTargetSend> m_QuestTargetSendList;
    std::string m_strJsonProgress; // json格式传输的进度
};

#endif
// end of file


