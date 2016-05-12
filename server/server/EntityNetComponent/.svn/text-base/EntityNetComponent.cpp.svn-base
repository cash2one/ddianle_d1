#include "EntityNetComponent.h"
#include "../logic/LogicCirculator.h"

//int SendGameMsg(LPGameMsg_Base pMsg, const PLAYERTAG* pTag);

CEntityNetComponent::CEntityNetComponent(void)
:CEntityComponent(false,false),m_pTag(NULL)
{
}

CEntityNetComponent::~CEntityNetComponent(void)
{
}

void CEntityNetComponent::Start()
{
	
}
void CEntityNetComponent::RegComponentNetMsgMap()
{

}
void CEntityNetComponent::OnLogin()	
{
}
void CEntityNetComponent::OnLogout()
{
}

bool CEntityNetComponent::SendPlayerMsg(GameMsg_Base * pMsg )
{
	int bResult = false;
	if( m_pTag && pMsg )
	{
		bResult = SendGameMsg( pMsg , m_pTag );
	}

	return (bResult == 0) ? true : false;
}
void CEntityNetComponent::SetPlayerTag( PLAYERTAG * pTag )
{
	m_pTag = pTag;
}

void CEntityNetComponent::Send2GroupServer(GameMsg_Base *pMsg)
{
	if (pMsg != NULL && NULL != m_pTag)

	{
		pMsg->nIndex = GetPlayerLogicIndex();
		pMsg->SessionID = GetSessionID();
		pMsg->nAccount = m_pTag->m_nAccount;
		SendMsg2GroupServer(pMsg);
	}
}

PLAYERTAG * CEntityNetComponent::GetPlayerTag()
{
	return m_pTag;
}


__int64 CEntityNetComponent::GetSessionID()
{
	if (Entity()->IsValid())
	{
		return m_pTag != NULL ? m_pTag->m_nSessionID:0;
	}
	return 0;
}

unsigned short CEntityNetComponent::GetPlayerSlot()
{ 
	return m_pTag != NULL ? m_pTag->m_nSlot:0;
}

unsigned short CEntityNetComponent::GetPlayerLogicIndex()
{
	return m_pTag != NULL ? m_pTag->m_nIndexLogic:0;
}//end

/*
unsigned short CEntityNetComponent::GetIndex()
{ 
	return m_pTag != NULL ? m_pTag->m_nIndexLogic:0;
}
*/
int CEntityNetComponent::GetVerType() const
{
	return Version_Free;
}


//bool CEntityNetComponent::Packet(GameMsg_Role_CreateRole *pCreateMsg)
//{
//	if(pCreateMsg == NULL || m_pTag == NULL)
//	{
//		return false;
//	}
//
//	pCreateMsg->nIndex = m_pTag->m_nIndexLogic;
//
//	return true;
//}





//end file


