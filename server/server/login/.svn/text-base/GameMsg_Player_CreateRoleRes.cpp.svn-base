#include "GameMsg_Player_CreateRoleRes.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/Entity.h"
extern int  g_nLine;

GameMsg_S2C_CreateRoleSuc::GameMsg_S2C_CreateRoleSuc() : GameMsg_Base(MSG_S2C_CreateRoleSuc)
,m_pEntity(NULL)
{

}

GameMsg_S2C_CreateRoleSuc::~GameMsg_S2C_CreateRoleSuc()
{

}

bool GameMsg_S2C_CreateRoleSuc::doEncode(CParamPool &IOBuff)
{
	if( m_pEntity != NULL )
	{
		//IOBuff.AddInt(g_nLine);
		m_pEntity->SerializeAllComponent( IOBuff );
		m_pEntity->SerializeExtraData( IOBuff );
	}

	return true;
}


void GameMsg_S2C_CreateRoleSuc::AttachEntity(CEntity* pEntity )
{
	m_pEntity = pEntity ;
}


GameMsg_Player_CreateRoleFail::GameMsg_Player_CreateRoleFail() : GameMsg_Base(MSG_S2C_CreateRoleFail)
{
	m_strResultMsg = "";
}

GameMsg_Player_CreateRoleFail::~GameMsg_Player_CreateRoleFail()
{
}

bool GameMsg_Player_CreateRoleFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strResultMsg.c_str());
	return true;
}


