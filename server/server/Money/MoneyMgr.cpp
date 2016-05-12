#include "MoneyMgr.h"
#include <time.h>
#include "../util/CommonFunc.h"


CRestDate::CRestDate()
:m_nDay(0)
,m_nHour(0)
,m_nMin(0)
,m_nSec(0)
{
}

CRestDate::~CRestDate()
{
}

CMoneyMgr::CMoneyMgr()
:m_nNexMoneyRstTime(0)
,m_nRemindDaysBeforeMoneyRst(0)
{

}

CMoneyMgr::~CMoneyMgr()
{

}

CMoneyMgr & CMoneyMgr::Instance()
{
	static CMoneyMgr stcMoneyMgr;

	return stcMoneyMgr;
}

void CMoneyMgr::ReadMoneyRst(int nDay, int nHour, int nMinute, int nSecond, int nRemaindDaysBeforeMoneyRst, std::string &strTitle, std::string &strContent)
{
	m_MoneyRestDate.m_nDay = nDay;
	m_MoneyRestDate.m_nHour = nHour;
	m_MoneyRestDate.m_nMin = nMinute;
	m_MoneyRestDate.m_nSec = nSecond;

	m_nRemindDaysBeforeMoneyRst = nRemaindDaysBeforeMoneyRst;

	m_strTitle = strTitle;
	m_strContent = strContent;
}


void CMoneyMgr::Initialize()
{
}

unsigned int CMoneyMgr::GetCurMonthMoneyRstTime(unsigned int nTime)
{
	time_t nNow = nTime;

	struct tm tTime;
	LocalTime(&nNow, &tTime);
	tTime.tm_mday = m_MoneyRestDate.m_nDay;
	tTime.tm_hour = m_MoneyRestDate.m_nHour;
	tTime.tm_min = m_MoneyRestDate.m_nMin;
	tTime.tm_sec = m_MoneyRestDate.m_nSec;

	return (unsigned int)mktime(&tTime);
}

unsigned int CMoneyMgr::GetNextMonthMoneyRstTime(unsigned int nTime)
{
	time_t nNow = nTime;
	struct tm tTime;
	LocalTime(&nNow, &tTime);
	tTime.tm_mday = m_MoneyRestDate.m_nDay;
	tTime.tm_hour = m_MoneyRestDate.m_nHour;
	tTime.tm_min = m_MoneyRestDate.m_nMin;
	tTime.tm_sec = m_MoneyRestDate.m_nSec;

	if(tTime.tm_mon == 11)
	{
		++tTime.tm_year;
		tTime.tm_mon = 0;
	} else {
		++tTime.tm_mon;
	}

	return (unsigned int)mktime(&tTime);
}

unsigned int CMoneyMgr::GetCompareRstTime(unsigned int nLastRstTime)
{
	unsigned int nCompareTime = 0;
	if (nLastRstTime >= GetCurMonthMoneyRstTime(nLastRstTime) ) {
		nCompareTime = GetNextMonthMoneyRstTime(nLastRstTime);	
	} else {
		nCompareTime = GetCurMonthMoneyRstTime(nLastRstTime);
	}

	return nCompareTime;
}

unsigned int CMoneyMgr::GetCurMonthRemindTime(unsigned int nTime)
{
	return GetNextMonthMoneyRstTime(nTime) - m_nRemindDaysBeforeMoneyRst * 24 * 60 * 60;
}

void CMoneyMgr::PlusOneMon(struct tm &rDate)
{
	if (rDate.tm_mon == 11) {
		++rDate.tm_year;
		rDate.tm_mon = 0;
	} else {
		++rDate.tm_mon;
	}
}

const std::string& CMoneyMgr::MailTitle()
{
    return m_strTitle;
}

const std::string& CMoneyMgr::MailContent()
{
    return m_strContent;
}

