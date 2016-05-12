#include "GameMsg_S2G_FindPlayerFromID.h"
#include "../share/ServerMsgDef.h"



GameMsg_S2G_FindPlayer::GameMsg_S2G_FindPlayer()
:GameMsg_Base(MSG_S2G_FindPlayer)
{
	m_nFindFriendTypeIndex = 0;
	m_nFindRoleID = 0;
	m_strFindRoleName = "";
	m_nFindLevelIndex = 0;
	m_nFindSexIndex = 0;
	m_nFindAgeIndex = 0;
	m_nFindConstellationIndex = 0;
}

GameMsg_S2G_FindPlayer::~GameMsg_S2G_FindPlayer()
{
}

bool GameMsg_S2G_FindPlayer::doEncode(CParamPool &ioBuff)
{
	ioBuff.AddUChar(m_nFindFriendTypeIndex);
	ioBuff.AddUInt(m_nFindRoleID);
	ioBuff.AddStringW(m_strFindRoleName.c_str());
	ioBuff.AddUChar(m_nFindLevelIndex);
	ioBuff.AddUChar(m_nFindSexIndex);
	ioBuff.AddUChar(m_nFindAgeIndex);
	ioBuff.AddUChar(m_nFindConstellationIndex);

	return true;
}

bool GameMsg_S2G_FindPlayer::doDecode(CParamPool &ioBuff)
{
	m_nFindFriendTypeIndex = ioBuff.GetUChar();
	m_nFindRoleID = ioBuff.GetUInt();
	ioBuff.GetStringW(m_strFindRoleName);
	m_nFindLevelIndex = ioBuff.GetUChar();
	m_nFindSexIndex = ioBuff.GetUChar();
	m_nFindAgeIndex = ioBuff.GetUChar();
	m_nFindConstellationIndex = ioBuff.GetUChar();

	return true;
}
