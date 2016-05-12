#ifndef __ROOM_REWARD_DATA_MGR_H__
#define __ROOM_REWARD_DATA_MGR_H__

#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../item/ItemProcess.h"
#include <vector>
#include <list>


class CCountRandCollect
{
public:
	CCountRandCollect();
	~CCountRandCollect();

	//添加可随机物品
	bool AddCountRate(int nCount, int nRate);
	//获取随机物品
	int RandCount();
private:
	std::vector< int >		m_CountVec;			//数量表
	std::vector< int >		m_RateVec;			//物品随机根据表
};

class CRoomRewardDataMgr
{
protected:
	CRoomRewardDataMgr();
	~CRoomRewardDataMgr();

public:
	static CRoomRewardDataMgr & Instance();

	//初始化基础数据
	void InitBaseData(const char * szTitle, const char * szContent);
	//添加随机物品数量
	bool AddRandCount(int nCount, int nRate);
	//添加必掉随机物品
	void AddCertainItem(const char * szMaleReward, const char * szFemaleReward, int nRate);
	//添加随机随机物品
	void AddModeRandItem(int nMusicMode, const char * szMaleReward, const char * szFemaleReward, int nRate);
	//获取奖励的物品数
	int GetItemRewardCount();
	//获取必掉物品列表
	void GetCertainItem(bool bIsMale, std::list< CItem > & rItemList);
	//获取随机物品列表
	void GetRandomItem(int nMusicMode, bool bIsMale, int nNum, std::list< CItem > & rItemList);
	//获取邮件标题
	const std::string & GetMailTitle();
	//获取邮件内容
	const std::string & GetMailContent();
private:
	//添加随机随机物品
	void _AddRandItem(const char * szReward, int nRate, CItemRandCollect & rIRC);
private:
	CCountRandCollect				m_CountRC;		//中奖物品数集合
	std::string						m_strTitle;		//邮件标题
	std::string						m_strContent;	//邮件内容

	CItemRandCollect		m_MaleCertainIRC;		//必掉物品随机集合
	CItemRandCollect		m_arMaleRandomIRC[MusicMode_Max];		//随机物品随机集合

	CItemRandCollect		m_FemaleCertainIRC;		//必掉物品随机集合
	CItemRandCollect		m_arFemaleRandomIRC[MusicMode_Max];		//随机物品随机集合
};

#endif

