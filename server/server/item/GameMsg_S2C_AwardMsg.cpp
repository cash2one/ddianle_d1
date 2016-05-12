#include "GameMsg_S2C_AwardMsg.h"
#include "../share/ServerMsgDef.h"


///////////////////////////////////////////////////////
GameMsg_S2C_ItemAwardSuc::GameMsg_S2C_ItemAwardSuc(void)
:GameMsg_Base(MSG_S2C_ItemAwardSuc)
,m_nItemID(0)
,m_nItemCount(0)
{
}

GameMsg_S2C_ItemAwardSuc::~GameMsg_S2C_ItemAwardSuc(void)
{
}

bool GameMsg_S2C_ItemAwardSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt( m_nItemID );
	IOBuff.AddInt( m_nItemCount );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ItemAwardFail::GameMsg_S2C_ItemAwardFail(void)
:GameMsg_Base(MSG_S2C_ItemAwardFail)
{
}

GameMsg_S2C_ItemAwardFail::~GameMsg_S2C_ItemAwardFail(void)
{
}

bool GameMsg_S2C_ItemAwardFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}

//end

