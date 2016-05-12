#include "DivorceAgreement.h"
//#include "../../datastructure/q"
#include "../libServerFrame/QueryManager.h"
#include "../Couple/GameMsg_S2C_Couple.h"
#include "../../datastructure/RoleEntity.h"
#include "../logic/PlayerManager.h"
#include "GameMsg_S2G_Couple.h"

extern CQueryManager* g_pQueryMgr;
extern void SendMsg2GroupServer(GameMsg_Base *pMsg);

void CDivorceAgreement::LoadDivorceAction( std::list<CDivorceAction> & actionlist )
{
	std::list<CDivorceAction>::iterator it = actionlist.begin();
	for ( ; it != actionlist.end(); ++it )
	{
		CDivorceActionIter itInsert = m_DivorceAction.insert( m_DivorceAction.end(), *it );
		if ( it->m_eType == eDivorceAction_Chat ||  it->m_eType == eDivorceAction_Gift)
		{
			m_DivorceChatIndex.push_front( itInsert );
		}
	}
}


void CDivorceAgreement::AddDivorceAction( const CDivorceAction & action, bool bIsSave, bool bMaleSendMsg, bool bFemaleSendMsg )
{
	CDivorceActionIter itInsert = m_DivorceAction.insert( m_DivorceAction.end(), action );

	if ( action.m_eType == eDivorceAction_Chat || action.m_eType == eDivorceAction_Gift )
	{
		int nIndexCount = (int)m_DivorceChatIndex.size();
		if ( nIndexCount >= MAX_CHAT_RECORD_COUNT )
		{
			CDivorceActionIter it = m_DivorceChatIndex[nIndexCount-1];
			if ( it != m_DivorceAction.end() )
			{
				m_DivorceAction.erase( it );
			}

			m_DivorceChatIndex.pop_back();
		}

		m_DivorceChatIndex.push_front( itInsert );
	}
		

	// DB
	if ( bIsSave )
	{
		CDivorceAction * pAction = new CDivorceAction( action );
		if ( pAction != NULL )
		{
			g_pQueryMgr->AddQuery(QUERY_Couple_AddDivorceAction, 0, pAction );
		}
	}
	
	// ·´À¡
	GameMsg_S2C_RefreshAction refreshmsg;
	refreshmsg.m_action = action;
	refreshmsg.m_DivorceAgreementInfo = m_DivorceAgreementInfo;

	if ( bMaleSendMsg )
	{
		CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID( m_DivorceAgreementInfo.m_nMaleRoleID );
		if ( pRoleEntity != NULL )
		{
			pRoleEntity->SendPlayerMsg( &refreshmsg);
		}
		else
		{
			GameMsg_S2G_RefreshDivorceAction s2gmsg;
			s2gmsg.m_nRoleID =  m_DivorceAgreementInfo.m_nMaleRoleID;
			s2gmsg.m_DivorceAgreementInfo = m_DivorceAgreementInfo;
			s2gmsg.m_action = action;
			::SendMsg2GroupServer( &s2gmsg );
		}
	}
	
	if ( bFemaleSendMsg )
	{
		CRoleEntity * pFemaleEntity = CPlayerManager::Instance().GetEntityByRoleID( m_DivorceAgreementInfo.m_nFemaleRoleID );
		if ( pFemaleEntity != NULL )
		{
			pFemaleEntity->SendPlayerMsg( &refreshmsg);
		}
		else
		{
			GameMsg_S2G_RefreshDivorceAction s2gmsg;
			s2gmsg.m_nRoleID =  m_DivorceAgreementInfo.m_nFemaleRoleID;
			s2gmsg.m_DivorceAgreementInfo = m_DivorceAgreementInfo;
			s2gmsg.m_action = action;
			::SendMsg2GroupServer( &s2gmsg );
		}
	}	
}

void CDivorceAgreement::Reset()
{
	m_DivorceAgreementInfo.clear();
	m_DivorceAction.clear();
	m_DivorceChatIndex.clear();
}

void CDivorceAgreement::UpdateDivorceAgreementState( eDivorceState state )
{
	m_DivorceAgreementInfo.m_eState = state;
}
