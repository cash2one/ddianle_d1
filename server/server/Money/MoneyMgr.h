#ifndef		__MONEYMGR_H__
#define		__MONEYMGR_H__

#include <string>


class CRestDate
{
public:
	CRestDate();
	~CRestDate();
public:
	int m_nDay;
	int m_nHour;
	int m_nMin;
	int m_nSec;
};

class	CMoneyMgr
{
private:
	CMoneyMgr();
	~CMoneyMgr();
public:
	static CMoneyMgr & Instance();

	void Initialize();

	void ReadMoneyRst(int nDay, int nHour, int nMinute, int nSecond, int nRemaindDaysBeforeMoneyRst, std::string &strTitle, std::string &strContent);

	unsigned int GetCurMonthMoneyRstTime(unsigned int nTime);
	unsigned int GetNextMonthMoneyRstTime(unsigned int nTime);
	unsigned int GetCompareRstTime(unsigned int nLastRstTime);
	unsigned int GetCurMonthRemindTime(unsigned int nTime);

    const std::string& MailTitle();
    const std::string& MailContent();
private:
	void PlusOneMon(struct tm &rDate);
	// void GetNextMon(struct tm &rData);

public:
	CRestDate m_MoneyRestDate;
	unsigned int m_nNexMoneyRstTime;
	int m_nRemindDaysBeforeMoneyRst;	// days before we start to remind character the money will be reset throught mail
	std::string m_strTitle;
	std::string m_strContent;
};


#endif

