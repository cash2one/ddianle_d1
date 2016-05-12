#ifndef	__GIFT_SYSTEM_H__
#define __GIFT_SYSTEM_H__

#include <map>
#include "../../datastructure/DataStruct_Base.h"

class CRoleEntity;

class CGiftInfo
{
public:
	CGiftInfo();
	~CGiftInfo();
public:
	//初始化
	void Init(int nGiftID, bool bSendOnLogin, const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward,
		const char * szMessage, const char * szTitle, const char * szContent);
	//发送礼物给玩家
	void SendGiftToRole(CRoleEntity & rRole);
public:
	int						m_nGiftID;				//礼物ID
	bool					m_bSendOnLogin;			//登陆就送
	std::string				m_strMessage;			//提示内容
	std::string				m_strTitle;				//邮件标题
	std::string				m_strContent;			//邮件内容
	unsigned int			m_nMoneyReward;			//金钱奖励
	std::list< CItem >		m_MaleRewardItemList;	//男性奖励物品列表
	std::list< CItem >		m_FemaleRewardItemList;	//女性奖励物品列表
};

//礼品系统管理器
class CGiftSystem
{
public:
	CGiftSystem();
	~CGiftSystem();

public:
	//添加礼物信息
	void AddGift(CGiftInfo * pGiftInfo);
	//获取礼物信息
	CGiftInfo * GetGiftInfo(int nGiftID);
private:
	std::map< int, CGiftInfo * > m_GiftRewardMap;		//GiftID -> GiftInfo
};

#endif


