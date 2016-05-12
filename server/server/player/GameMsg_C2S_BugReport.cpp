#include "GameMsg_C2S_BugReport.h"

#include "../share/ServerMsgDef.h"

GameMsg_C2S_BugReport::GameMsg_C2S_BugReport(void)
:GameMsg_Base(MSG_C2S_BugReport)
{
}

GameMsg_C2S_BugReport::~GameMsg_C2S_BugReport(void)
{
}


bool GameMsg_C2S_BugReport::doDecode(CParamPool &IOBuff)
{
	IOBuff.GetStringW(m_strBugDesc);
	return true;
}

