#include "GameMsg_LBS.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_UpdateLBSPositon::GameMsg_C2S_UpdateLBSPositon():GameMsg_Base(MSG_C2S_UpdateLBSPositon)
{
	m_fX = 0.0f;
	m_fY = 0.0f;
}

GameMsg_C2S_UpdateLBSPositon::~GameMsg_C2S_UpdateLBSPositon()
{

}

bool GameMsg_C2S_UpdateLBSPositon::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_fX << m_fY;
	return true;
}

bool GameMsg_C2S_UpdateLBSPositon::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_fX >> m_fY;
	return true;
}

GameMsg_S2G_UpdateLBSPositon::GameMsg_S2G_UpdateLBSPositon():GameMsg_Base(MSG_S2G_UpdateLBSPositon)
{
	m_nRoleID = 0;
	m_fX = 0.0f;
	m_fY = 0.0f;
}

GameMsg_S2G_UpdateLBSPositon::~GameMsg_S2G_UpdateLBSPositon()
{

}

bool GameMsg_S2G_UpdateLBSPositon::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_nRoleID << m_fX << m_fY;
	return true;
}

bool GameMsg_S2G_UpdateLBSPositon::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nRoleID >> m_fX >> m_fY;
	return true;
}

NearbyPlayer::NearbyPlayer()
{
	m_nRoleID = 0;
	m_nLevel = 0;
	m_nSex = 0;
	m_nLine = 0;
	m_fDist = 0.0f;
	m_fX = 0.0f;
	m_fY = 0.0f;
	m_nCallTo = 0;
}

NearbyPlayer::~NearbyPlayer()
{

}

bool NearbyPlayer::doEncode( CParamPool &IOBuff ) const
{
	IOBuff << m_nRoleID << m_strRoleName << m_nLevel << m_nVIPLevel << m_nSex << m_nLine << m_fDist << m_fX << m_fY << m_nCallTo;
	return true;
}

bool NearbyPlayer::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nRoleID >> m_strRoleName >> m_nLevel >> m_nVIPLevel >> m_nSex >> m_nLine >> m_fDist >> m_fX >> m_fY >> m_nCallTo;
	return true;
}

GameMsg_G2S_NearbyPlayers::GameMsg_G2S_NearbyPlayers():GameMsg_Base(MSG_G2S_NearbyPlayers)
{
	m_nRoleID = 0;
	m_cFlag = 0;
}

GameMsg_G2S_NearbyPlayers::~GameMsg_G2S_NearbyPlayers()
{

}

bool GameMsg_G2S_NearbyPlayers::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_nRoleID << m_cFlag << m_listNearbyPlayer;
	return true;
}

bool GameMsg_G2S_NearbyPlayers::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nRoleID >> m_cFlag >> m_listNearbyPlayer;
	return true;
}

GameMsg_S2C_NearbyPlayers::GameMsg_S2C_NearbyPlayers():GameMsg_Base(MSG_S2C_NearbyPlayers)
{
	m_cFlag = 0;
}

GameMsg_S2C_NearbyPlayers::~GameMsg_S2C_NearbyPlayers()
{

}

bool GameMsg_S2C_NearbyPlayers::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_cFlag << m_listNearbyPlayer;
	return true;
}

bool GameMsg_S2C_NearbyPlayers::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_cFlag >> m_listNearbyPlayer;
	return true;
}

GameMsg_C2S_GetNearbyPlayers::GameMsg_C2S_GetNearbyPlayers():GameMsg_Base(MSG_C2S_GetNearbyPlayers)
{
	m_cFlag = 0;
}

GameMsg_C2S_GetNearbyPlayers::~GameMsg_C2S_GetNearbyPlayers()
{

}

bool GameMsg_C2S_GetNearbyPlayers::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_cFlag;
	return true;
}

bool GameMsg_C2S_GetNearbyPlayers::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_cFlag;
	return true;
}

GameMsg_S2G_GetNearbyPlayers::GameMsg_S2G_GetNearbyPlayers():GameMsg_Base(MSG_S2G_GetNearbyPlayers)
{
	m_nRoleID = 0;
	m_cFlag = 0;
}

GameMsg_S2G_GetNearbyPlayers::~GameMsg_S2G_GetNearbyPlayers()
{

}

bool GameMsg_S2G_GetNearbyPlayers::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_nRoleID << m_cFlag;
	return true;
}

bool GameMsg_S2G_GetNearbyPlayers::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nRoleID >> m_cFlag;
	return true;
}

GameMsg_S2G_CloseLBS::GameMsg_S2G_CloseLBS():GameMsg_Base(MSG_S2G_CloseLBS)
{
	m_nRoleID = 0;
}

GameMsg_S2G_CloseLBS::~GameMsg_S2G_CloseLBS()
{

}

bool GameMsg_S2G_CloseLBS::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_nRoleID;
	return true;
}

bool GameMsg_S2G_CloseLBS::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nRoleID;
	return true;
}

