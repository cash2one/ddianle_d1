#include "../share/StdAfx.h"
#include "GameMsg_Server_DropClient.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/Macro_Define.h"


GameMsg_Server_DropClient::GameMsg_Server_DropClient(void)
	:GameMsg_Base(MSG_SERVER_DropClient)
	, m_nDropReason(DropPlayerCause_ReLogin)
{
	//nAccount = 0;
}

GameMsg_Server_DropClient::~GameMsg_Server_DropClient(void)
{
}
bool GameMsg_Server_DropClient::doEncode( CParamPool &IoBuff )
{
	//IoBuff.AddUInt(nAccount);
	IoBuff.AddUShort(m_nDropReason);
	return true;
}
bool GameMsg_Server_DropClient::doDecode( CParamPool &IoBuff )
{
	//nAccount = IoBuff.GetUInt();
	m_nDropReason = IoBuff.GetUShort();
	return true;
}

//end

