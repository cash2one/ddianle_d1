#ifndef	__VIPSTRUCT_H__
#define __VIPSTRUCT_H__

#include <map>
#include <list>
#include <string>
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class CVIPExpInfo 
{
public:
	CVIPExpInfo();
	~CVIPExpInfo();
public:
	void Serialize(CParamPool & IOBuff);
public:
	int m_nCurLevel;
	int m_nExpLevepUpNeed;
};


class CVIPLevelUpReward
{
public:
	CVIPLevelUpReward();
	~CVIPLevelUpReward();
public:
	void Serialize(CParamPool & IOBuff, unsigned char nSex);
public:
	int m_nLevel;
	std::list<CItem> m_listMaleItem;
	std::list<CItem> m_listFemaleItem;
	int m_nMoney;
};

class CVIPLevelPrivInfo
{
public:
	CVIPLevelPrivInfo();
	~CVIPLevelPrivInfo();

	void Serialize(CParamPool & IOBuff, unsigned char nSex);
public:
	int m_nLevel;											// VIP等级
	int m_nExp;												// 升级到该级别所需的经验
	int m_nDailyExpBonus;									// 每日VIP可领取成长值
	bool m_bSpeakerFontEffe;								// VIP喇叭字体特效
	bool m_bCanKickRoomPlayer;								// 房间踢人
	bool m_bExclusiveSelfInfoBoard;							// 专属个人信息面板
	bool m_bExclusiveRoomAction;							// 专属房间动作
	bool m_bEnterRoomSoundEffe;								// 专属进入房间音效
	int m_nExpRateBonus;									// 经验加成
	int m_nRoleNameColor;									// 角色名颜色
	int m_nLevelUpAnnounce;									// VIP升级公告 0 表示无， 1表示 红色系统公告 2表示走马灯公告
	bool m_bVIPLevelUpReward;								// VIP升级礼包
	std::string m_strEnterRoomViewEffe;						// 进入房间特效
	CVIPLevelUpReward m_VIPLevelUpReward;					// VIP升级奖励 
	bool m_bOnlineAnnounce;									// 大V上线公告
	int m_nOnlineAnnounceCoolDown;							// 上线公告冷却时间
	int m_nMaxFriendCount;									// 好友上限
	int m_nMaxBlackListCount;								// 黑名单上限
	int m_nConstellationMatchCount;							// 星座魔法阵匹配次数
	int m_nClothGroupCount;									// 可以拥有的服饰搭配个数
    int m_nDailyBindMBill;                                  // 每日绑定M币奖励
    std::list<CItem> m_listDailyItemReward;                 // 每日物品奖励
    int m_nVipBuyVitCount;                                  // 魔法神灯优惠购买体力次数
    unsigned short m_nDanceGroupChallengeExtraChance;       // 舞团试炼额外次数
    int m_nStorageCapacity;                                 // 仓库初始容量
    unsigned int m_nDanceGroupFairyLandExChance;           // 舞团秘境额外次数
    unsigned int m_nDanceGroupFairyLandVipBuf;             // 秘境vip增益
};

#endif

