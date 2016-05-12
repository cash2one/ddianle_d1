#include "GameMsg_S2C_GetQuestListResult.h"
#include "../share/ServerMsgDef.h"
#include "QuestListEntry.h"


GameMsg_S2C_GetQuestListResult::GameMsg_S2C_GetQuestListResult():
GameMsg_Base(MSG_S2C_GetQuestListResult)
{
}

GameMsg_S2C_GetQuestListResult::~GameMsg_S2C_GetQuestListResult()
{
}

bool GameMsg_S2C_GetQuestListResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_listQuestInfo.size());
	if (m_listQuestInfo.size() > 0)
	{
		for (list<CQuestListEntry>::iterator it = m_listQuestInfo.begin(); it != m_listQuestInfo.end(); it++)
		{
			it->doEncode(IOBuff);
		}
	}

	return true;
}



// end of file


