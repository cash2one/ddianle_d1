#include "../share/ServerMsgDef.h"
#include "GameMsg_S2G_PlayerInfo.h"


GameMsg_S2G_GetPlayerInfo_Forward::GameMsg_S2G_GetPlayerInfo_Forward(unsigned int nSrcID, unsigned int nQueryID):
GameMsg_Base(MSG_S2G_GetPlayerInfo_Forward),
m_nSrcID(nSrcID),
m_nQueryID(nQueryID)
{
}


GameMsg_S2G_GetPlayerInfo_Forward::~GameMsg_S2G_GetPlayerInfo_Forward()
{
}

bool GameMsg_S2G_GetPlayerInfo_Forward::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSrcID);
	IOBuff.AddUInt(m_nQueryID);

	return true;
}

bool GameMsg_S2G_GetPlayerInfo_Forward::doDecode(CParamPool &IOBuff)
{
	m_nSrcID = IOBuff.GetUInt();
	m_nQueryID = IOBuff.GetUInt();

	return true;
}

GameMsg_S2G_GetPlayerInfo_Backward::GameMsg_S2G_GetPlayerInfo_Backward(unsigned int nSrcID):
GameMsg_Base(MSG_S2G_GetPlayerInfo_Backward),
m_nSrcID(nSrcID)
{
}

GameMsg_S2G_GetPlayerInfo_Backward::GameMsg_S2G_GetPlayerInfo_Backward(unsigned int nSrcID, PlayerInfo &playInfo):
GameMsg_Base(MSG_S2G_GetPlayerInfo_Backward),
m_nSrcID(nSrcID),
m_playerInfo(playInfo)
{
}

GameMsg_S2G_GetPlayerInfo_Backward::~GameMsg_S2G_GetPlayerInfo_Backward()
{
}

bool GameMsg_S2G_GetPlayerInfo_Backward::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSrcID);
	m_playerInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_S2G_GetPlayerInfo_Backward::doDecode(CParamPool &IOBuff)
{
	m_nSrcID = IOBuff.GetUInt();
	m_playerInfo.doDecode(IOBuff);

	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2G_GetOfflinePlayerInfo::GameMsg_S2G_GetOfflinePlayerInfo()
    : GameMsg_Base(MSG_S2G_GetOfflinePlayerInfo)
    ,m_nRoleID(0)
    ,m_eFrom(EOfflinePlayerDataFrom_None)
{
}

GameMsg_S2G_GetOfflinePlayerInfo::~GameMsg_S2G_GetOfflinePlayerInfo()
{
}

bool GameMsg_S2G_GetOfflinePlayerInfo::doDecode(CParamPool &IOBuff)
{
    m_eFrom = (EOfflinePlayerDataFrom)IOBuff.GetUChar();
    m_nRoleID = IOBuff.GetUInt();
    m_OffLineData.doDecode( IOBuff );
    return true;
}

bool GameMsg_S2G_GetOfflinePlayerInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar( (unsigned char)m_eFrom );
    IOBuff.AddUInt( m_nRoleID );
    m_OffLineData.doEncode(IOBuff);
    return true;
}




