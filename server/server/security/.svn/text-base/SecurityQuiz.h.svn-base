#ifndef __SECURITY_QUIZ_H__
#define __SECURITY_QUIZ_H__ 1

#include <string>
#include <list>

class SecurityQuiz
{
public:
	SecurityQuiz(unsigned char nQuizID = 0, const char *pszQuiz = "", const char *pszAnswer = "");
	~SecurityQuiz();

public:
	unsigned char m_nQuizID;
	std::string   m_strQuiz;
	std::string   m_strAnswer;
};

typedef std::list<SecurityQuiz> SecurityQuizList;

#endif //__SECURITY_QUIZ_H__

