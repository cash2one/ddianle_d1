#include "GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist::GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist(unsigned int nDesRoleID, unsigned int nRemovedRoleID):
GameMsg_Base(MSG_S2G_NotifyGameHasBeenDeletedBlacklist)
{
	m_nDesRoleID = nDesRoleID;
	m_nRemovedRoleID = nRemovedRoleID;
}

GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist::~GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist()
{

}

bool GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nDesRoleID);
	IOBuff.AddUInt(m_nRemovedRoleID);
	//IOBuff.AddUInt(m_friendInfo.nRoleID);
	//IOBuff.AddUChar(m_friendInfo.nSex);
	//IOBuff.AddString(m_friendInfo.szName);
	//IOBuff.AddUShort(m_friendInfo.nLevel);

	return true;
}

bool GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist::doDecode(CParamPool &IOBuff)
{
	m_nDesRoleID = IOBuff.GetUInt();
	m_nRemovedRoleID = IOBuff.GetUInt();
	//m_friendInfo.nRoleID = IOBuff.GetUInt();
	//m_friendInfo.nSex = IOBuff.GetUChar();
	//string str = IOBuff.GetString();
	//CopyString2Buffer(str, m_friendInfo.szName, MAX_NAME_LEN - 1);
	//m_friendInfo.nLevel = IOBuff.GetUShort();

	return true;
}


// end of file

