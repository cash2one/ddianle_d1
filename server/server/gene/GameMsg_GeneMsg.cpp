#include "GameMsg_GeneMsg.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/ParamPool.h"


GameMsg_S2C_AddGene::GameMsg_S2C_AddGene()
:GameMsg_Base(MSG_S2C_AddGene)
,m_nRoleID(0)
{
}

GameMsg_S2C_AddGene::~GameMsg_S2C_AddGene()
{
}

bool GameMsg_S2C_AddGene::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nRoleID );
	m_GeneData.doEncode( IOBuff );

	return true;
}


GameMsg_S2C_DelGene::GameMsg_S2C_DelGene()
:GameMsg_Base(MSG_S2C_DelGene)
,m_nRoleID(0)
,m_nGeneID(0)
{
}

GameMsg_S2C_DelGene::~GameMsg_S2C_DelGene()
{
}

bool GameMsg_S2C_DelGene::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUShort( m_nGeneID );

	return true;
}


GameMsg_S2C_ResetGene::GameMsg_S2C_ResetGene()
:GameMsg_Base(MSG_S2C_ResetGene)
,m_nRoleID(0)
,m_nGeneID(0)
,m_nParam2(0)
{
}

GameMsg_S2C_ResetGene::~GameMsg_S2C_ResetGene()
{
}

bool GameMsg_S2C_ResetGene::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUShort( m_nGeneID );
	IOBuff.AddInt( m_nParam2 );

	return true;
}




