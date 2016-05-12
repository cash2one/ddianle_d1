#include "GameMsg_Buff.h"
#include "../share/ServerMsgDef.h"

bool BuffData::doEncode( CParamPool &IOBuff ) const
{
	IOBuff << m_nBuffType << m_nBuffSrcType << m_nGeneID << m_nPercent << m_nLeftTime << m_nItemType << m_bCanRemove;
	return true;
}

GameMsg_C2S_GetAllBuff::GameMsg_C2S_GetAllBuff():GameMsg_Base(MSG_C2S_GetAllBuff)
{

}

GameMsg_C2S_GetAllBuff::~GameMsg_C2S_GetAllBuff()
{

}

bool GameMsg_C2S_GetAllBuff::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_S2C_GetAllBuff::GameMsg_S2C_GetAllBuff():GameMsg_Base(MSG_S2C_GetAllBuff)
{

}

GameMsg_S2C_GetAllBuff::~GameMsg_S2C_GetAllBuff()
{

}

bool GameMsg_S2C_GetAllBuff::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_listBuffData;
	return true;
}

GameMsg_C2S_UnApplyBuff::GameMsg_C2S_UnApplyBuff():GameMsg_Base(MSG_C2S_UnApplyBuff)
{
	m_nGeneId = 0;
}

GameMsg_C2S_UnApplyBuff::~GameMsg_C2S_UnApplyBuff()
{

}

bool GameMsg_C2S_UnApplyBuff::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nGeneId;
	return true;
}

GameMsg_S2C_UnApplyBuff::GameMsg_S2C_UnApplyBuff():GameMsg_Base(MSG_S2C_UnApplyBuff)
{
	m_nGeneId = 0;
}

GameMsg_S2C_UnApplyBuff::~GameMsg_S2C_UnApplyBuff()
{

}

bool GameMsg_S2C_UnApplyBuff::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_nGeneId << m_strError;
	return true;
}


