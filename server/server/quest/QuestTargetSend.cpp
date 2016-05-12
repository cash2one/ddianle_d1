#include "QuestTargetSend.h"
#include "../../socket/ParamPool.h"


CQuestTargetSend::CQuestTargetSend()
{
	m_nType = 0;	
	m_nIndex = 0;
	m_nRequiredCount = 0;
	m_nCompletedCount = 0;	
}

CQuestTargetSend::~CQuestTargetSend()
{
}

void CQuestTargetSend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nType);	
	IOBuff.AddUInt(m_nIndex);
	IOBuff.AddUInt(m_nRequiredCount);
	IOBuff.AddUInt(m_nCompletedCount);
}


// end of file


