#include "SecurityQuizDataManager.h"

#include "../util/CommonFunc.h"

SecurityQuizDataManager & SecurityQuizDataManager::instance()
{
	static SecurityQuizDataManager s_instance;
	return s_instance;
}

SecurityQuizDataManager::SecurityQuizDataManager()
: m_nMoney(0), m_strMailTitle(""), m_strMailContent("")
{
}

SecurityQuizDataManager::~SecurityQuizDataManager()
{
}

void SecurityQuizDataManager::AddSecurityQuiz(unsigned char nQuizID, const char *pszQuiz, const char *pszAnswer)
{
	SecurityQuiz quiz(nQuizID, pszQuiz, pszAnswer);
	m_listQuiz.push_back(quiz);
}

const SecurityQuiz * SecurityQuizDataManager::GetSecurityQuiz(unsigned char nQuizID)
{
	for (SecurityQuizList::iterator it = m_listQuiz.begin(); it != m_listQuiz.end(); ++it)
	{
		if (it->m_nQuizID == nQuizID)
			return &(*it);
	}
	return NULL;
}

SecurityQuizList & SecurityQuizDataManager::QuizList()
{
	return m_listQuiz;
}

void SecurityQuizDataManager::SetSecurityQuizReward(
	const char *pszMaleItem, const char *pszFemaleItem, int money,
	const char *pszMailTitle, const char *pszMailContent)
{
	TransferItemList(pszMaleItem, m_listMaleItem);
	TransferItemList(pszFemaleItem, m_listFemaleItem);
	m_nMoney = money;
	m_strMailTitle = pszMailTitle;
	m_strMailContent = pszMailContent;
}

std::list<CItem> & SecurityQuizDataManager::RewardItem(unsigned char nSex)
{
	if (nSex == ESexType_Male)
		return m_listMaleItem;

	return m_listFemaleItem;
}

int SecurityQuizDataManager::RewardMoney()
{
	return m_nMoney;
}

std::string & SecurityQuizDataManager::MailTitle()
{
	return m_strMailTitle;
}

std::string & SecurityQuizDataManager::MailContent()
{
	return m_strMailContent;
}

