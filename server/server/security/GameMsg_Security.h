#ifndef __GAMEMSG_SECURITY_H__
#define __GAMEMSG_SECURITY_H__ 1

#include "../../socket/GameMsg_Base.h"
#include "SecurityQuizDataManager.h"


class GameMsg_C2S_SetSecurityQuiz :
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetSecurityQuiz);
	GameMsg_C2S_SetSecurityQuiz();
	~GameMsg_C2S_SetSecurityQuiz();
public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	SecurityQuizList m_quizList;
};


class GameMsg_S2C_SetSecurityQuizSuc:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SetSecurityQuizSuc);
	GameMsg_S2C_SetSecurityQuizSuc();	
	~GameMsg_S2C_SetSecurityQuizSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::list<CItem> m_rewardItem;
	int m_nMoney;
};


class GameMsg_S2C_SetSecurityQuizFail:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SetSecurityQuizFail);
	GameMsg_S2C_SetSecurityQuizFail();	
	~GameMsg_S2C_SetSecurityQuizFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;
};


#endif //__GAMEMSG_SECURITY_H__

