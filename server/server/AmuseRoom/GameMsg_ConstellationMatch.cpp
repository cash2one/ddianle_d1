#include "GameMsg_ConstellationMatch.h"
#include "../../datastructure/Macro_Define.h"
#include "../../server/share/ServerMsgDef.h"


///////////////////////////////////////////////////////
GameMsg_S2C_GetConstellationMatchInfoRes::GameMsg_S2C_GetConstellationMatchInfoRes(void)
:GameMsg_Base(MSG_S2C_GetConstellationMatchInfoRes)
{
	m_nMinRoleLevel = 0;
	m_bIsOpen = false;
	m_nLeftOpenTime = 0;
}

GameMsg_S2C_GetConstellationMatchInfoRes::~GameMsg_S2C_GetConstellationMatchInfoRes(void)
{
}

bool GameMsg_S2C_GetConstellationMatchInfoRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nMinRoleLevel);
	IOBuff.AddBool(m_bIsOpen);
	IOBuff.AddInt(m_nLeftOpenTime);
	return true;
}



//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_EnterConstellationLobby::GameMsg_C2S_EnterConstellationLobby( void )
:GameMsg_Base(MSG_C2S_EnterConstellationLobby)
{
}

GameMsg_C2S_EnterConstellationLobby::~GameMsg_C2S_EnterConstellationLobby( void )
{

}

bool GameMsg_C2S_EnterConstellationLobby::doDecode( CParamPool& IOBuff )
{
	return true;
}


GameMsg_S2C_EnterConstellationLobbySuc::GameMsg_S2C_EnterConstellationLobbySuc(void)
:GameMsg_Base(MSG_S2C_EnterConstellationLobbySuc)
{
	m_nMaxUseCount = 0;
	m_nUseCount = 0;
	m_bFirstEnter = false;
}

GameMsg_S2C_EnterConstellationLobbySuc::~GameMsg_S2C_EnterConstellationLobbySuc(void)
{
}

bool GameMsg_S2C_EnterConstellationLobbySuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddBool(m_bFirstEnter);
	IOBuff.AddStringW(m_strFirstEnterTip.c_str());

	IOBuff.AddUShort(m_nMaxUseCount);
	IOBuff.AddUShort(m_nUseCount);

	IOBuff.AddUShort((unsigned short)m_listCard.size());
	for (std::list<SConstellationCardInfo>::iterator it = m_listCard.begin();
		it != m_listCard.end(); ++it)
	{
		it->doEncode(IOBuff);
	}

	IOBuff.AddStringW(m_strRule.c_str());

	return true;
}


GameMsg_S2C_EnterConstellationLobbyFail::GameMsg_S2C_EnterConstellationLobbyFail(void)
:GameMsg_Base(MSG_S2C_EnterConstellationLobbyFail)
{
	m_nUseCount = 0;
	m_nWillOpenTime = 0;
}

GameMsg_S2C_EnterConstellationLobbyFail::~GameMsg_S2C_EnterConstellationLobbyFail(void)
{
}

bool GameMsg_S2C_EnterConstellationLobbyFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	IOBuff.AddUShort(m_nUseCount);
	IOBuff.AddInt(m_nWillOpenTime);
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_QuitConstellationLobby::GameMsg_C2S_QuitConstellationLobby( void )
:GameMsg_Base(MSG_C2S_QuitConstellationLobby)
{
}

GameMsg_C2S_QuitConstellationLobby::~GameMsg_C2S_QuitConstellationLobby( void )
{

}

bool GameMsg_C2S_QuitConstellationLobby::doDecode( CParamPool& IOBuff )
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_ConstellationMatch::GameMsg_C2S_ConstellationMatch( void )
:GameMsg_Base(MSG_C2S_ConstellationMatch)
{
	m_nCardIndex = 0;
}

GameMsg_C2S_ConstellationMatch::~GameMsg_C2S_ConstellationMatch( void )
{

}

bool GameMsg_C2S_ConstellationMatch::doDecode( CParamPool& IOBuff )
{
	m_nCardIndex = IOBuff.GetUShort();
	return true;
}

GameMsg_S2C_ConstellationMatchSuc::GameMsg_S2C_ConstellationMatchSuc(void)
:GameMsg_Base(MSG_S2C_ConstellationMatchSuc)
{
	m_nRoleID = 0;
	m_nLevel = 0;
	m_nConstellation = 0;
	m_nMatchRate = 0;
	m_nSceneID = 0;
}

GameMsg_S2C_ConstellationMatchSuc::~GameMsg_S2C_ConstellationMatchSuc(void)
{
}

bool GameMsg_S2C_ConstellationMatchSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddStringW(m_strRoleName.c_str());
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddUChar(m_nConstellation);
	IOBuff.AddUChar(m_nMatchRate);
	IOBuff.AddStringW(m_strWishWords.c_str());
	IOBuff.AddUShort(m_nSceneID);

	unsigned short nQuestionCount = (unsigned short)m_vecQuestion.size();
	IOBuff.AddUShort(nQuestionCount);
	for (unsigned short i = 0; i < nQuestionCount; i++)
	{
		m_vecQuestion[i].doEncode(IOBuff);
	}

	return true;
}


GameMsg_S2C_ConstellationMatchFail::GameMsg_S2C_ConstellationMatchFail(void)
:GameMsg_Base(MSG_S2C_ConstellationMatchFail)
{
}

GameMsg_S2C_ConstellationMatchFail::~GameMsg_S2C_ConstellationMatchFail(void)
{
}

bool GameMsg_S2C_ConstellationMatchFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_EnterConstellationRoom::GameMsg_C2S_EnterConstellationRoom( void )
:GameMsg_Base(MSG_C2S_EnterConstellationRoom)
{
}

GameMsg_C2S_EnterConstellationRoom::~GameMsg_C2S_EnterConstellationRoom( void )
{

}

bool GameMsg_C2S_EnterConstellationRoom::doDecode( CParamPool& IOBuff )
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_SendQuestionAnswer::GameMsg_C2S_SendQuestionAnswer( void )
:GameMsg_Base(MSG_C2S_SendQuestionAnswer)
{
}

GameMsg_C2S_SendQuestionAnswer::~GameMsg_C2S_SendQuestionAnswer( void )
{

}

bool GameMsg_C2S_SendQuestionAnswer::doDecode( CParamPool& IOBuff )
{
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0; i < nCount; i++)
	{
		unsigned char nAnswer = IOBuff.GetUChar();
		m_listAnswer.push_back(nAnswer);
	}

	return true;
}

