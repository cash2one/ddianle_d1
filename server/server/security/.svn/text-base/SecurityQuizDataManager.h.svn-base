#ifndef __SECURITY_QUIZ_DATA_MANAGER_H__
#define __SECURITY_QUIZ_DATA_MANAGER_H__ 1

#include "../../datastructure/DataStruct_Base.h"
#include "SecurityQuiz.h"

class SecurityQuizDataManager
{
public:
	SecurityQuizDataManager();
	~SecurityQuizDataManager();

	static SecurityQuizDataManager & instance();

	void AddSecurityQuiz(unsigned char nQuizID, const char *pszQuiz, const char *pszAnswer = "");
	void SetSecurityQuizReward(const char *pszMaleItem, const char *pszFemaleItem, int money,
		const char *pszMailTitle, const char *pszMailContent);
	
	const SecurityQuiz * GetSecurityQuiz(unsigned char nQuizID);
	SecurityQuizList & QuizList();

	std::list<CItem> & RewardItem(unsigned char nSex);
	int RewardMoney();

	std::string & MailTitle();
	std::string & MailContent();

protected:
	// Quiz
	SecurityQuizList m_listQuiz;

	// Reward
	std::list<CItem> m_listMaleItem;
	std::list<CItem> m_listFemaleItem;
	int m_nMoney;

	// Reward mail
	std::string m_strMailTitle;
	std::string m_strMailContent;

};

#endif //__SECURITY_QUIZ_DATA_MANAGER_H__

