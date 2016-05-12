#include "MonthCardDataMgr.h"

CMonthCardDataMgr::CMonthCardDataMgr()
{

}

CMonthCardDataMgr::~CMonthCardDataMgr()
{

}

CMonthCardDataMgr & CMonthCardDataMgr::Instance()
{
	static CMonthCardDataMgr  g_MonthCardDataMgr;
	return g_MonthCardDataMgr;
}

void CMonthCardDataMgr::AddMonthCardConfig( const MonthCardInfo & info )
{
	m_MonthCardConfig.insert( make_pair( info.nType, info));
}

const MonthCardInfo * CMonthCardDataMgr::GetMonthCardInfo( EMonthCardType etype )
{
	std::map<unsigned int,MonthCardInfo>::iterator it = m_MonthCardConfig.find( etype );
	if ( it != m_MonthCardConfig.end() )
	{
		return &(it->second);
	}

	return NULL;
}
