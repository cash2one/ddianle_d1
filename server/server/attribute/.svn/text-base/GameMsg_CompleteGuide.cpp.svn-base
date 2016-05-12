#include "GameMsg_CompleteGuide.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_CompleteTeachGuide::GameMsg_C2S_CompleteTeachGuide()
:GameMsg_Base(MSG_C2S_CompletedTeachGuide)
{
}

GameMsg_C2S_CompleteTeachGuide::~GameMsg_C2S_CompleteTeachGuide()
{
}

bool GameMsg_C2S_CompleteTeachGuide::doDecode(CParamPool &IOBuff)
{
	return true;
}



GameMsg_S2C_CompleteGuideResult::GameMsg_S2C_CompleteGuideResult()
:GameMsg_Base(MSG_S2C_CompletedTeachGuideRes)
{
}

GameMsg_S2C_CompleteGuideResult::~GameMsg_S2C_CompleteGuideResult()
{
}

bool GameMsg_S2C_CompleteGuideResult::doEncode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_C2S_CompleteProcGuide::GameMsg_C2S_CompleteProcGuide()
:GameMsg_Base(MSG_C2S_CompletedProcGuide)
,m_chProcStep(0)
{

}

GameMsg_C2S_CompleteProcGuide::~GameMsg_C2S_CompleteProcGuide()
{

}

bool GameMsg_C2S_CompleteProcGuide::doDecode( CParamPool &IOBuff )
{
	m_chProcStep = IOBuff.GetUChar();

	return true;
}

GameMsg_S2C_CompleteProcGuideResult::GameMsg_S2C_CompleteProcGuideResult()
: GameMsg_Base(MSG_C2S_CompletedProcGuideRes)
{

}

GameMsg_S2C_CompleteProcGuideResult::~GameMsg_S2C_CompleteProcGuideResult()
{

}

bool GameMsg_S2C_CompleteProcGuideResult::doEncode(CParamPool &IOBuff)
{
	//IOBuff.AddUChar(m_chProcStep);
	return true;
}


GameMsg_C2S_SelectGuideFace::GameMsg_C2S_SelectGuideFace()
:GameMsg_Base(MSG_C2S_SelectGuideFace)
{
	m_Face = 0;
}

GameMsg_C2S_SelectGuideFace::~GameMsg_C2S_SelectGuideFace()
{

}

bool GameMsg_C2S_SelectGuideFace::doDecode(CParamPool &IOBuff)
{
	m_Face = IOBuff.GetUChar();
	return true;
}

GameMsg_S2C_SelectGuideFaceRet::GameMsg_S2C_SelectGuideFaceRet()
:GameMsg_Base(MSG_S2C_SelectGuideFaceRet)
{
	m_Result = 0;
}

GameMsg_S2C_SelectGuideFaceRet::~GameMsg_S2C_SelectGuideFaceRet()
{

}

bool GameMsg_S2C_SelectGuideFaceRet::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_Result);
	IOBuff.AddUShort((unsigned short)m_Items.size());
	std::list<CItem>::iterator it = m_Items.begin();
	for (;it != m_Items.end() ; it++)
	{
		it->doEncode(IOBuff);
	}
	return true;
}

void GameMsg_S2C_SelectGuideFaceRet::SetItems(std::list<CItem>& items)
{
	std::list<CItem>::iterator it = items.begin();
	for (;it != items.end() ; it++)
	{
		m_Items.push_back(*it);
	}
}

GameMsg_C2S_CompleteAnimation::GameMsg_C2S_CompleteAnimation()
:GameMsg_Base(MSG_C2S_CompleteAnimation)
{

}

GameMsg_C2S_CompleteAnimation::~GameMsg_C2S_CompleteAnimation()
{

}

bool GameMsg_C2S_CompleteAnimation::doDecode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_S2C_CompleteAnimationResult::GameMsg_S2C_CompleteAnimationResult()
: GameMsg_Base(MSG_S2C_CompleteAnimationRet)
{

}

GameMsg_S2C_CompleteAnimationResult::~GameMsg_S2C_CompleteAnimationResult()
{

}

bool GameMsg_S2C_CompleteAnimationResult::doEncode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_C2S_ReciveGuideExperienceCard::GameMsg_C2S_ReciveGuideExperienceCard()
	: GameMsg_Base(MSG_C2S_ReciveGuideExperienceCard)
{

}

GameMsg_C2S_ReciveGuideExperienceCard::~GameMsg_C2S_ReciveGuideExperienceCard()
{
}

bool GameMsg_C2S_ReciveGuideExperienceCard::doDecode(CParamPool &IOBuff)
{
	return true;
}


GameMsg_S2C_ReciveGuideExperienceCardRes::GameMsg_S2C_ReciveGuideExperienceCardRes()
	: GameMsg_Base(MSG_S2C_ReciveGuideExperienceCardRes)
	, m_nRes(0)
{

}

GameMsg_S2C_ReciveGuideExperienceCardRes::~GameMsg_S2C_ReciveGuideExperienceCardRes()
{

}

bool GameMsg_S2C_ReciveGuideExperienceCardRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nRes);

	return true;
}


