#include "../share/ServerMsgDef.h"
#include "GameMsg_Security.h"

GameMsg_C2S_SetSecurityQuiz::GameMsg_C2S_SetSecurityQuiz()
: GameMsg_Base(MSG_C2S_SetSecurityQuiz)
{
}

GameMsg_C2S_SetSecurityQuiz::~GameMsg_C2S_SetSecurityQuiz()
{
}

bool GameMsg_C2S_SetSecurityQuiz::doDecode(CParamPool &IOBuff)
{
	unsigned short nQuizCount = IOBuff.GetUShort();
	for (unsigned short i = 0; i < nQuizCount; i++)
	{
		SecurityQuiz quiz;
		quiz.m_nQuizID = IOBuff.GetUChar();
		IOBuff.GetStringW(quiz.m_strAnswer);
		m_quizList.push_back(quiz);
	}
	return true;
}


GameMsg_S2C_SetSecurityQuizSuc::GameMsg_S2C_SetSecurityQuizSuc()
: GameMsg_Base(MSG_S2C_SetSecurityQuizSuc)
{
	m_nMoney = 0;
}

GameMsg_S2C_SetSecurityQuizSuc::~GameMsg_S2C_SetSecurityQuizSuc()
{
}

bool GameMsg_S2C_SetSecurityQuizSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_rewardItem.size());
	for (std::list<CItem>::iterator ir = m_rewardItem.begin(); ir != m_rewardItem.end(); ++ir)
	{
		ir->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nMoney);
	return true;
}

GameMsg_S2C_SetSecurityQuizFail::GameMsg_S2C_SetSecurityQuizFail()
: GameMsg_Base(MSG_S2C_SetSecurityQuizFail)
{
}

GameMsg_S2C_SetSecurityQuizFail::~GameMsg_S2C_SetSecurityQuizFail()
{
}

bool GameMsg_S2C_SetSecurityQuizFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

