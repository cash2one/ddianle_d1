#include "SlotPeer.h"
#include "SocketMgr.h"
#include "ClientSocketMgr.h"
using namespace ZeroSocket;
CSlotPeer::CSlotPeer(void)
{
	m_pSocketMgr = NULL;
	m_pClientSocketMgr = NULL;
	m_nSlot=0; 
}
CSlotPeer::CSlotPeer(const CSlotPeer &refSlotPeer)
{
	m_pSocketMgr = refSlotPeer.m_pSocketMgr;	
	m_pClientSocketMgr = refSlotPeer.m_pClientSocketMgr;
	m_nSlot = refSlotPeer.m_nSlot;
}

CSlotPeer::~CSlotPeer(void)
{
}
bool CSlotPeer::SendGameMsg( GameMsg_Base * pMsg )
{
	if( m_pSocketMgr )
	{
		return m_pSocketMgr->SendGameMsg(pMsg,m_nSlot); 
	}
	else
	{
		return m_pClientSocketMgr->SendGameMsg(pMsg);
	}
}



//file end

