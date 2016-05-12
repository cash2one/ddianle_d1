#ifndef __ZERO_SOCKET_PEER_H__
#define __ZERO_SOCKET_PEER_H__


class GameMsg_Base;
namespace ZeroSocket
{
class CClientSocketMgr;
class CSocketMgr;
class CSlotPeer
{
public:
	CSlotPeer();
	CSlotPeer(const CSlotPeer &refSlotPeer);
	~CSlotPeer();
	bool SendGameMsg( GameMsg_Base * pMsg );
	void SetSocketMgr(CClientSocketMgr * pClientSocketMgr)
	{
		m_pClientSocketMgr = pClientSocketMgr;
	}
	void SetSocketMgr(CSocketMgr * pSocketMgr)
	{
		m_pSocketMgr = pSocketMgr;
	}

	void SetSlot(unsigned short nSlot){m_nSlot = nSlot;};
	unsigned short GetSlot(){return m_nSlot;};

private:
	CSocketMgr *m_pSocketMgr;
	CClientSocketMgr * m_pClientSocketMgr;
	unsigned short m_nSlot;
};
}
#endif


//file end

