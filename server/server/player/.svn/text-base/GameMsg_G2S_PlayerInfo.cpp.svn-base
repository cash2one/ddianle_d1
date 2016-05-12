#include "../share/ServerMsgDef.h"
#include "GameMsg_G2S_PlayerInfo.h"


GameMsg_G2S_GetPlayerInfo_Forward::GameMsg_G2S_GetPlayerInfo_Forward(unsigned int nSrcID, unsigned int nQueryID):
GameMsg_Base(MSG_G2S_GetPlayerInfo_Forward),
m_nSrcID(nSrcID),
m_nQueryID(nQueryID)
{
}

GameMsg_G2S_GetPlayerInfo_Forward::~GameMsg_G2S_GetPlayerInfo_Forward()
{
}

bool GameMsg_G2S_GetPlayerInfo_Forward::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSrcID);
	IOBuff.AddUInt(m_nQueryID);
	return true;
}

bool GameMsg_G2S_GetPlayerInfo_Forward::doDecode(CParamPool &IOBuff)
{
	m_nSrcID = IOBuff.GetUInt();
	m_nQueryID = IOBuff.GetUInt();
	return true;
}

GameMsg_G2S_GetPlayerInfo_Backward::GameMsg_G2S_GetPlayerInfo_Backward():
GameMsg_Base(MSG_G2S_GetPlayerInfo_Backward)
{
}

GameMsg_G2S_GetPlayerInfo_Backward::GameMsg_G2S_GetPlayerInfo_Backward(PlayerInfo &playerInfo):
GameMsg_Base(MSG_G2S_GetPlayerInfo_Backward),
m_playerInfo(playerInfo)
{
}


GameMsg_G2S_GetPlayerInfo_Backward::~GameMsg_G2S_GetPlayerInfo_Backward()
{
}

bool GameMsg_G2S_GetPlayerInfo_Backward::doEncode(CParamPool &IOBuff)
{
	m_playerInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_G2S_GetPlayerInfo_Backward::doDecode(CParamPool &IOBuff)
{
	m_playerInfo.doDecode(IOBuff);

	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_G2S_GetOfflinePlayerInfo::GameMsg_G2S_GetOfflinePlayerInfo()
    : GameMsg_Base(MSG_G2S_GetOfflinePlayerInfo)
    ,m_eFrom(EOfflinePlayerDataFrom_None)
    ,m_nSelfRoleID(0)
{
}

GameMsg_G2S_GetOfflinePlayerInfo::~GameMsg_G2S_GetOfflinePlayerInfo()
{

}

bool GameMsg_G2S_GetOfflinePlayerInfo::doDecode(CParamPool &IOBuff)
{
    m_eFrom = (EOfflinePlayerDataFrom)IOBuff.GetUChar();
    m_nSelfRoleID = IOBuff.GetUInt();
    return true;
}

bool GameMsg_G2S_GetOfflinePlayerInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar( (unsigned char)m_eFrom );
    IOBuff.AddUInt( m_nSelfRoleID );
    return true;
}









