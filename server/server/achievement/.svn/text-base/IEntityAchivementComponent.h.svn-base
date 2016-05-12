#ifndef		__IENTITYACHIEVEMENTCOMPONENT_H__
#define		__IENTITYACHIEVEMENTCOMPONENT_H__

#include "../../datastructure/DataStruct_Base.h"

class CEntityAttributeComponent;
class CEntityQuestComponent;
class CEntityChatComponent;
class CEntityNetComponent;
class CEntityItemComponent;
class CEntityCheckInComponent;

class IEntityAchievementComponent
{
public:
	IEntityAchievementComponent();
	virtual ~IEntityAchievementComponent();

public:
	virtual CEntityAttributeComponent *		GetRoleAttr() const = 0;
	virtual CEntityQuestComponent *			GetRoleQuest() const = 0;
	virtual CEntityChatComponent	*		GetRoleChat() const = 0;
	virtual CEntityNetComponent *			GetRoleNet() const = 0;
	virtual CEntityItemComponent*			GetRoleItem()const = 0;
	virtual CEntityCheckInComponent*       GetRoleCheck() const=0;
	unsigned int GetHistoryTitleCount() const {return (unsigned int)m_listTitle.size();}
	unsigned int GetHistoryAchievementCount() const {return (unsigned int)m_listAchievement.size();}
	unsigned int GetTotalScore() const;
	unsigned int GetSmallSpeakersUseCountInSevenDays() const;
	unsigned int GetBigSpeakersUseCountInSevenDays() const;
	unsigned int GetSendFlowersCountInSevenDays() const;
public:
	void SetFightResult(unsigned char nMode, unsigned char nDifficulty, unsigned );

public:
	// 获得的称号
	list<CTitleInfo>			m_listTitle;							// 已经获得的称号
	list<CAchievementInfo>		m_listAchievement;						// 已经完成的成就
	unsigned int				m_nScore;								// 成就积分
	// 物品相关
	unsigned int				m_nHistoryClothNum;						// 累计拥有的任意时效的衣服数量 
	unsigned int				m_nHistoryWishItemNum;					// 累计加入愿望栏的物品数量
	unsigned int				m_nHistoryBadageNum;					// 累计拥有的徽章数
	unsigned int				m_nGainWishItemNum;						// 获得的愿望物品数量 
	unsigned int				m_nHistoryUseBouquetNum;				// 使用花束的数量
	CWeeklyEvent				m_BigSpeakersUseCountWeekly;			// 一周内使用大喇叭的数量
	CWeeklyEvent				m_SmallSpeakersUseCountWeekly;			// 一周内使用小喇叭的数量
	CWeeklyEvent				m_SendFlowersCountWeekly;				// 一周内送花数量
	// 比赛相关
	unsigned int				m_nHistoryTotalScore;					// 累计跳舞得分，任意模式
	unsigned short				m_nNumWinBetween2To4Persons;			// 2人以上第一名
	unsigned short				m_nNumWinIn4Persons;					// 4人中第一名
	unsigned short				m_nHistoryTaikoComboMusicNum;			// 累计太鼓模式无miss全连歌曲数
	unsigned short				m_nTaikoComboNum;						// 太鼓模式combo数
	unsigned int				m_nAutoMatchWithOppositeSexNum;			// 通过快速匹配模式与异性一起游戏的人数

	map<int, map<int, CFightResult > > m_mapFightResult;				// <模式 <难度，实体>>.比赛历史战斗数据
};


#endif

//end file


