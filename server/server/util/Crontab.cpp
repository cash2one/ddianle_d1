#include "Crontab.h"
#include "Split.h"
#include "CommonFunc.h"

bool CCrontab::IsBetween( const CCrontab& beginTimer, const CCrontab& endTimer, time_t tTime, bool bCheckFormat, bool bCheckInCurTimeCycle )
{
	const std::vector<int>& vecBegin = beginTimer.GetTimeVec();
	const std::vector<int>& vecEnd = endTimer.GetTimeVec();
	if ( vecBegin.size() == ETimeUnit_Max && vecBegin.size() == vecEnd.size() )
	{
		std::vector<int> dt;
		ToTimeVec(tTime, dt);
		bool bWeekOnly = (vecBegin[ETimeUnit_Year] < 0 && vecBegin[ETimeUnit_Month] < 0 && vecBegin[ETimeUnit_Day] < 0 && vecBegin[ETimeUnit_WeekDay] >= 0);
		if ( bCheckInCurTimeCycle && bWeekOnly )
		{
			return IsBetweenInCurWeek(beginTimer, endTimer, tTime, bCheckFormat);
		}
		else
		{
			if ( bCheckFormat && !IsSameFormat(vecBegin, vecEnd) )
			{
				return false;
			}
			for (int i = 0; i < ETimeUnit_Max; ++i)
			{
				if ( vecBegin[i] < 0 )
				{
					dt[i] = -1;
				}
			}

			__int64 nBeginN = ToNumber(vecBegin);
			__int64 nDTN = ToNumber(dt);
			__int64 nEndN = ToNumber(vecEnd);
			return nBeginN <= nDTN && nDTN <= nEndN;
		}
	}
	return false;
}

bool CCrontab::IsBetweenInCurWeek( const CCrontab& beginTimer, const CCrontab& endTimer, time_t tTime, bool bCheckFormat )
{
	const std::vector<int>& vecBegin = beginTimer.GetTimeVec();
	const std::vector<int>& vecEnd = endTimer.GetTimeVec();
	if ( vecBegin.size() == ETimeUnit_Max && vecBegin.size() == vecEnd.size() )
	{
		if ( bCheckFormat && !IsSameFormat(vecBegin, vecEnd) )
		{
			return false;
		}
		
		if ( vecBegin[ETimeUnit_Year] < 0 && vecBegin[ETimeUnit_Month] < 0 && vecBegin[ETimeUnit_Day] < 0 && vecBegin[ETimeUnit_WeekDay] >= 0 )
		{
			time_t nNow = time(NULL);
			std::vector<int> vecTimeNow;
			ToTimeVec(nNow, vecTimeNow);
			
			time_t tBegin = nNow + (vecBegin[ETimeUnit_WeekDay] - vecTimeNow[ETimeUnit_WeekDay])*24*3600;
			std::vector<int> vecTimeBegin;
			ToTimeVec(tBegin, vecTimeBegin);
			tBegin = SetTime(vecTimeBegin[ETimeUnit_Year], vecTimeBegin[ETimeUnit_Month], vecTimeBegin[ETimeUnit_Day], 
				(vecBegin[ETimeUnit_Hour]>=0?vecBegin[ETimeUnit_Hour]:0), 
				(vecBegin[ETimeUnit_Minute]>=0?vecBegin[ETimeUnit_Minute]:0), 
				(vecBegin[ETimeUnit_Second]>=0?vecBegin[ETimeUnit_Second]:0));
			
			time_t tEnd = nNow + (vecEnd[ETimeUnit_WeekDay] - vecTimeNow[ETimeUnit_WeekDay])*24*3600;
			std::vector<int> vecTimeEnd;
			ToTimeVec(tEnd, vecTimeEnd);
			tEnd = SetTime(vecTimeEnd[ETimeUnit_Year], vecTimeEnd[ETimeUnit_Month], vecTimeEnd[ETimeUnit_Day], 
				(vecEnd[ETimeUnit_Hour]>=0?vecEnd[ETimeUnit_Hour]:0), 
				(vecEnd[ETimeUnit_Minute]>=0?vecEnd[ETimeUnit_Minute]:0), 
				(vecEnd[ETimeUnit_Second]>=0?vecEnd[ETimeUnit_Second]:0));

			return (tBegin <= tTime && tTime <= tEnd);
		}
	}
	return false;
}

bool CCrontab::AssignFormat( const std::string& strFormat, const std::string& strSep )
{
	std::vector<std::string> vec;
	if ( Split(strFormat, strSep, vec) && vec.size() == ETimeUnit_Max )
	{
		std::vector<int> vecTime;
		for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			if ( *it == "*" )
			{
				vecTime.push_back(-1);
			}
			else
			{
				int n = 0;
				if ( !ToType(*it, n) )
				{
					return false;
				}
				vecTime.push_back(n);
			}
		}
		if ( !CheckFormat(vecTime) )
		{
			return false;
		}
		m_strFormat = strFormat;
		m_vecTime = vecTime;
		return true;
	}
	return false;
}

bool CCrontab::IsTimeDone( time_t tNow )
{
	if ( m_vecTime.size() == ETimeUnit_Max )
	{
		std::vector<int> dt;
		ToTimeVec(tNow, dt);
		for (int i = 0; i < ETimeUnit_Max; ++i)
		{
			if ( m_vecTime[i] < 0 )
			{
				dt[i] = -1;
			}
		}
		if ( dt == m_vecTime )
		{
			return true;
		}
		for (int i = 0; i < ETimeUnit_Max; ++i)
		{
			if ( m_vecTime[i] > 0 )
			{
				if ( m_vecTime[i] < dt[i] )
				{
					return true;
				}
				else if ( m_vecTime[i] > dt[i] )
				{
					return false;
				}
			}
		}
	}
	return false;
}

bool CCrontab::IsTimeDone()
{
	return IsTimeDone(time(NULL));
}

const std::vector<int>& CCrontab::GetTimeVec() const
{
	return m_vecTime;
}

void CCrontab::ToTimeVec( time_t tTime, std::vector<int>& vecTime, bool bSpecial )
{
	struct tm tmTime;
	LocalTime(&tTime, &tmTime);
	if ( bSpecial )
	{
		int dt[] = {tmTime.tm_year+1900, tmTime.tm_mon+1, tmTime.tm_wday, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec};
		if ( dt[ETimeUnit_WeekDay] == 0 )
		{
			dt[ETimeUnit_WeekDay] = 7;
		}
		vecTime.clear();
		vecTime.assign(dt, dt+sizeof(dt)/sizeof(dt[0]));
	}
	else
	{
		int dt[] = {tmTime.tm_year, tmTime.tm_mon, tmTime.tm_wday, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec};
		vecTime.clear();
		vecTime.assign(dt, dt+sizeof(dt)/sizeof(dt[0]));
	}
}

__int64 CCrontab::ToNumber( const std::vector<int>& vecTime )
{
	__int64 n = 0;
	if ( vecTime.size() == ETimeUnit_Max )
	{
		int nWeight[] = {365*24*3600, 31*24*3600, 7*24*3600, 24*3600, 3600, 60, 1};
		for (int i = 0; i < ETimeUnit_Max; ++i)
		{
			if ( vecTime[i] > 0 )
			{
				n += vecTime[i] * nWeight[i];
			}
		}
	}
	return n;
}

bool CCrontab::IsSameFormat( const CCrontab& a, const CCrontab& b )
{
	const std::vector<int>& vecBegin = a.GetTimeVec();
	const std::vector<int>& vecEnd = b.GetTimeVec();
	return IsSameFormat(vecBegin, vecEnd);
}

bool CCrontab::IsSameFormat( const std::vector<int>& a, const std::vector<int>& b )
{
	if ( a.size() == b.size() && b.size() == ETimeUnit_Max )
	{
		for (int i = 0; i < ETimeUnit_Max; ++i)
		{
			if ( (a[i] >= 0 && b[i] < 0) || (a[i] < 0 && b[i] >= 0) )
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

#define BetweenNumber(n, a, b) ((n < 0) || (a <= n && n <= b))

bool CCrontab::CheckFormat( const std::vector<int>& vecTime )
{
	if ( vecTime.size() == ETimeUnit_Max )
	{
		bool bValid = true;
		bValid = bValid && BetweenNumber(vecTime[ETimeUnit_Month], 1, 12);
		bValid = bValid && BetweenNumber(vecTime[ETimeUnit_WeekDay], 1, 7);
		bValid = bValid && BetweenNumber(vecTime[ETimeUnit_Day], 1, 31);
		bValid = bValid && BetweenNumber(vecTime[ETimeUnit_Hour], 0, 23);
		bValid = bValid && BetweenNumber(vecTime[ETimeUnit_Minute], 0, 59);
		bValid = bValid && BetweenNumber(vecTime[ETimeUnit_Second], 0, 59);
		return bValid;
	}
	return false;
}

bool CCrontab::IsValid()
{
	return m_vecTime.size() == ETimeUnit_Max;
}

