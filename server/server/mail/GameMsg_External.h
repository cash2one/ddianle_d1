#ifndef	__GAMEMSG_EXTERNAL_H__
#define __GAMEMSG_EXTERNAL_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

#include <list>

using namespace std;

class CParamPool;


class GameMsg_EX2G_SendMail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_EX2G_SendMail);
	GameMsg_EX2G_SendMail();
	~GameMsg_EX2G_SendMail();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short      m_nPID;                     //平台id
	std::string         m_strPUID;                    //平台用户id
	std::string			m_strTitle;					//邮件标题
	std::string			m_strContent;				//邮件内容
	unsigned int		m_nMoney;					//金钱
	unsigned int		m_nItemType;				//类型
	unsigned short		m_nCount;					//数量
	int					m_nValidTime;				//时效
	std::string			m_strOrderID;				//序列号
};

class GameMsg_EX2G_SendMailGift : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_EX2G_SendMailGift);
	GameMsg_EX2G_SendMailGift();
	~GameMsg_EX2G_SendMailGift();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short      m_nPID;                     //平台id
	std::string         m_strPUID;                    //平台用户id
	unsigned int		m_nMailGiftID;				//郵件禮品ＩＤ
	std::string			m_strOrderID;				//序列号
};



#endif


