#include <algorithm>
#include "CheckInfoMgr.h"
#include "../../socket/Utility.h"
#include "../../datastructure/RoleEntity.h"
#include "../item/ItemProcess.h"


CCheckInConfig::CCheckInConfig()
	:m_nDayIndex(0)
	,m_nMoney(0)
	,m_nBindCoin(0)
	,m_nVipRewardLevel(0)
	,m_nVipRewardRate(0)
{

}

void CCheckInConfig::doDecode(CParamPool & IOBuff)
{

}

void CCheckInConfig::doEncode(CParamPool & IOBuff, char cSex)
{
	IOBuff.AddUInt( m_nDayIndex );
	
	if ( cSex == ESexType_Male )
	{
		m_maleItemReward.doEncode( IOBuff );
	}
	else
	{
		m_femaleItemReward.doEncode( IOBuff );
	}
	IOBuff.AddUInt( m_nMoney );
	IOBuff.AddUInt( m_nBindCoin );
	IOBuff.AddUInt( m_nVipRewardLevel );
	IOBuff.AddUInt( m_nVipRewardRate );
}

CCheckInDataManager::CCheckInDataManager()
    :m_nCurrentMonth(0)
{

}

CCheckInDataManager & CCheckInDataManager::Instance()
{
	static CCheckInDataManager s_stCheckInDataMgr;
	return s_stCheckInDataMgr;
}

void CCheckInDataManager::AddCheckInConfig( unsigned int nMonth, const CCheckInConfig & info )
{
    std::map<unsigned int, std::map<unsigned int, CCheckInConfig> >::iterator it = m_CheckInDataConfig.find( nMonth );
    if ( it != m_CheckInDataConfig.end())
    {
        it->second[ info.m_nDayIndex ] = info;
    }
    else
    {
        m_CheckInDataConfig[ nMonth].insert( make_pair(info.m_nDayIndex, info));
    }
}

void CCheckInDataManager::SetCurrentMonthConfig()
{
    time_t tNowTime = time(NULL);
    tm * pNowTime = localtime( &tNowTime );
    tm tmNowTime( *pNowTime );

    std::map<unsigned int, std::map<unsigned int, CCheckInConfig> >::iterator it = m_CheckInDataConfig.find( tmNowTime.tm_mon + 1 );
    if ( it != m_CheckInDataConfig.end() )
    {
        m_CurrentMonthConfig.clear();
        m_CurrentMonthConfig = it->second;
        m_nCurrentMonth = it->first;
    }
    else
    {
        it = m_CheckInDataConfig.begin();
        if ( it != m_CheckInDataConfig.end() )
        {
            m_CurrentMonthConfig = it->second;
            m_nCurrentMonth = it->first;
        }
    }
}

void CCheckInDataManager::SetCurrentMonthConfig( unsigned int nCurrentMonth )
{
    if ( m_nCurrentMonth == nCurrentMonth )
    {
        return;
    }

    std::map<unsigned int, std::map<unsigned int, CCheckInConfig> >::iterator it = m_CheckInDataConfig.find( nCurrentMonth );
    if ( it != m_CheckInDataConfig.end() )
    {
        m_CurrentMonthConfig.clear();
        m_CurrentMonthConfig = it->second;
        m_nCurrentMonth = it->first;
    }
    else
    {
        it = m_CheckInDataConfig.begin();
        if ( it != m_CheckInDataConfig.end() )
        {
            m_CurrentMonthConfig.clear();
            m_CurrentMonthConfig = it->second;
            m_nCurrentMonth = nCurrentMonth;
        }
    }
}

void CCheckInDataManager::Init()
{
    SetCurrentMonthConfig();
}

void CCheckInDataManager::AddCheckInDescConfig( std::string & strDesc )
{
	m_strDescribe = strDesc;
}

CCheckInConfig * CCheckInDataManager::GetNextCheckInConfig( unsigned int nDayIndex )
{
	// find first
	if ( nDayIndex == 0)
	{
		std::map<unsigned int,CCheckInConfig>::iterator it = m_CurrentMonthConfig.begin();
		if ( it != m_CurrentMonthConfig.end() )
		{
			return &(it->second);
		}
	}
	else
	{
		std::map<unsigned int,CCheckInConfig>::iterator it = m_CurrentMonthConfig.find( nDayIndex );
		if ( it != m_CurrentMonthConfig.end() )
		{
			//next
			it++;
			if ( it != m_CurrentMonthConfig.end())
			{
				return &(it->second);
			}
		}
	}

	return NULL;
}

CCheckInConfig * CCheckInDataManager::GetLastCheckInConfig( unsigned int nDayIndex )
{
    CCheckInConfig * pCheckInConfig = NULL;
    std::map<unsigned int,CCheckInConfig>::iterator it = m_CurrentMonthConfig.begin();
    for ( ; it != m_CurrentMonthConfig.end(); ++it )
    {
        if ( it->second.m_nDayIndex >= nDayIndex )
        {
            break;
        }

        pCheckInConfig = &(it->second);
    }

    return pCheckInConfig;
}

unsigned int CCheckInDataManager::GetHaveCheckInCount( unsigned int nDayIndex )
{
    unsigned int nCheckInCount = 0;
    std::map<unsigned int,CCheckInConfig>::iterator it = m_CurrentMonthConfig.find( nDayIndex );
    for ( ; it != m_CurrentMonthConfig.end(); ++it )
    {
        if ( it->second.m_nDayIndex <= nDayIndex )
        {
            nCheckInCount++;
        }
    }

    return nCheckInCount;
}

void CCheckInDataManager::doEncode(CParamPool & IOBuff,char cSex)
{
	IOBuff.AddUShort( (unsigned short)m_CurrentMonthConfig.size());
	std::map<unsigned int,CCheckInConfig>::iterator it = m_CurrentMonthConfig.begin();
	for ( ; it != m_CurrentMonthConfig.end(); ++it )
	{
		it->second.doEncode(IOBuff,cSex);
	}
}

CCheckInConfig * CCheckInDataManager::GetCheckInConfig( unsigned int nDayIndex )
{
	std::map<unsigned int,CCheckInConfig>::iterator it = m_CurrentMonthConfig.find( nDayIndex );
	if ( it != m_CurrentMonthConfig.end())
	{
		return &(it->second);
	}

	return NULL;
}
