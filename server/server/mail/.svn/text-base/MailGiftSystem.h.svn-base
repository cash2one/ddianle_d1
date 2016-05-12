#ifndef	__MAIL_GIFT_SYSTEM_H__
#define __MAIL_GIFT_SYSTEM_H__

#include <map>
#include "../../datastructure/DataStruct_Base.h"

class CMailGiftInfo
{
public:
	CMailGiftInfo();
	~CMailGiftInfo();
public:
	//初始化
	void Init(int nMailGiftID, const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward,
		const char * szTitle, const char * szContent);
	//发送礼物给玩家
	void SendMailGiftToRole(unsigned int nRoleID, bool bMale);
public:
	int						m_nMailGiftID;			//邮件礼物ID
	std::string				m_strTitle;				//邮件标题
	std::string				m_strContent;			//邮件内容
	unsigned int			m_nMoneyReward;			//金钱奖励
	std::list< CItem >		m_MaleRewardItemList;	//男性奖励物品列表
	std::list< CItem >		m_FemaleRewardItemList;	//女性奖励物品列表
};

//邮件礼品系统管理器
class CMailGiftSystem
{
protected:
	CMailGiftSystem();
	~CMailGiftSystem();

public:
	static CMailGiftSystem & Instance();
	//添加礼物信息
	void AddMailGift(CMailGiftInfo * pMailGiftInfo);
	//获取礼物信息
	CMailGiftInfo * GetMailGiftInfo(int nMailGiftID);
private:
	std::map< int, CMailGiftInfo * > m_MailGiftRewardMap;		//MailGiftID -> MailGiftInfo
};

#endif


