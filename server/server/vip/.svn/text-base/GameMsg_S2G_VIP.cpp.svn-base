#include "GameMsg_S2G_VIP.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_UpdateVIPInfo::GameMsg_S2G_UpdateVIPInfo(  )
:GameMsg_Base(MSG_S2G_UpdateVIPInfo)
,m_nRoleID(0)
,m_bIsVIP(false)
,m_nVIPLevel(0)
{

}

GameMsg_S2G_UpdateVIPInfo::~GameMsg_S2G_UpdateVIPInfo(  )
{

}

bool GameMsg_S2G_UpdateVIPInfo::doDecode( CParamPool& IOBuff )
{
	m_nRoleID = IOBuff.GetUInt();
	m_bIsVIP = IOBuff.GetBool();
	m_nVIPLevel = IOBuff.GetUShort();

	return true;
}

bool GameMsg_S2G_UpdateVIPInfo::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddBool(m_bIsVIP);
	IOBuff.AddUShort(m_nVIPLevel);

	return true;
}

