#include "StdAfx.h"
#include "AddrMgr.h"
using namespace ZeroSocket;

// ÆÁ±Îµ×²ã±¨¾¯£¬²»ÐÞ¸Ä
#ifdef WIN32
#pragma warning(disable:4996)
#endif

CAddrMgr::CAddrMgr()
{
	m_nTimeElapse = 0;
}

void CAddrMgr::Deny(unsigned int nAddr, int nSecond)
{
	map< unsigned int , int >::iterator it = mapAddr.find(nAddr);
	if(it == mapAddr.end())
	{
		mapAddr.insert(pair< unsigned int, int >(nAddr, nSecond));
	}
	else
	{
		(*it).second += nSecond;
	}
}

void CAddrMgr::DenySection(const char* addr)
{
	listAddrSection.push_back(inet_addr(addr));
}

bool CAddrMgr::BeDenied(unsigned int nAddr)
{
	list< unsigned int >::iterator itList;
	unsigned int addr;
	for(itList = listAddrSection.begin(); itList != listAddrSection.end(); itList ++)
	{
		addr = (*itList);
		if( (addr & nAddr) == addr)
			return true;
	}

	if(mapAddr.find(nAddr) != mapAddr.end())
		return true;
	return false;
}

#define TIMESLICE_s			10
#define TIMESLICE_ms		(TIMESLICE_s*1000)		//Ê®Ãë
void CAddrMgr::Update(int& timeElapse)
{
	m_nTimeElapse += timeElapse;
	while(m_nTimeElapse > TIMESLICE_ms)
	{
		m_nTimeElapse -= TIMESLICE_ms;
		map< unsigned int, int >::iterator itLast, it;
		for(it = mapAddr.begin(); it != mapAddr.end(); )
		{
			it->second -= TIMESLICE_s;
			if(it->second <= 0)
			{
				if(it == mapAddr.begin())
				{
					mapAddr.erase(it);
					it = mapAddr.begin();
				}
				else
				{
					itLast-- = it;
					mapAddr.erase(it);
					it = ++itLast;
				}
			}
			else
			{
				it++;
			}
		}
	}
} 

//file end

