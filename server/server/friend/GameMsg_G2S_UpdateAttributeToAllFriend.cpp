#include "GameMsg_G2S_UpdateAttributeToAllFriend.h"
#include "../share/ServerMsgDef.h"



GameMsg_G2S_UpdateAttributeToAllFriends::GameMsg_G2S_UpdateAttributeToAllFriends()
: GameMsg_Base(MSG_G2S_UpdateAttrbuteToAllFriends)
,m_nRoleID(0)
,m_strName("")
,m_nLevel(0)
,m_bIsVIP(false)
,m_nVIPLevel(0)
{

}

GameMsg_G2S_UpdateAttributeToAllFriends::~GameMsg_G2S_UpdateAttributeToAllFriends()
{

}

bool GameMsg_G2S_UpdateAttributeToAllFriends::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddStringW(m_strName.c_str());
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddBool(m_bIsVIP);
	IOBuff.AddUShort(m_nVIPLevel);

	return true;
}

bool GameMsg_G2S_UpdateAttributeToAllFriends::doDecode( CParamPool &IOBuff )
{
	m_nRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strName);
	m_nLevel = IOBuff.GetUShort();
	m_bIsVIP = IOBuff.GetBool();
	m_nVIPLevel = IOBuff.GetUShort();

	return true;
}


