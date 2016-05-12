#ifndef __ANTI_ADDICTION_H__
#define __ANTI_ADDICTION_H__

#include <map>
#include <vector>
#include <string>

enum eAddicEffect
{
	eAddicEffect_DanceExp,//跳舞活动经验
	eAddicEffect_AmuseExp,//在开放性场景中获得的经验
	eAddicEffect_QuestExp,//任务获得的经验
	eAddicEffect_QuestGoldTick,//任务获得的金券
	eAddicEffect_FightItemDropCount,//战斗掉落数量

	eAddicEffect_Max
};

struct AntiAddictionConfigItem
{
	std::pair<int, int> timeRange;//时间范围，单位分钟
	int tipDiffMinutes;//提示间隔分钟数
	std::string tipText;//提示文字
	float decrPercent[eAddicEffect_Max];//各项减量百分比
};

class CAntiAddictionDataMgr
{
public:
	CAntiAddictionDataMgr();
	~CAntiAddictionDataMgr();

	static CAntiAddictionDataMgr& Instance();

	bool IsOpenAntiaddiction();
	void SetOpenAntiaddiction(bool bOpenAntiaddiction);
	int GetOfflineMinutes() const { return m_nOfflineMinutes; }
	void SetOfflineMinutes(int val) { m_nOfflineMinutes = val; }
	void AddAntiAddictionConfig(int nID, AntiAddictionConfigItem* pConfig);
	void AddPlatformConfig(int nPlatformID, int nConfigID);
	AntiAddictionConfigItem* FindConfig(int nPlatformID, int nOnlineTime, int& nIndex);
	AntiAddictionConfigItem* FindConfig(int nPlatformID, int nIndex);
	bool HasConfig(int nPlatformID);
private:
	bool m_bOpenAntiaddiction;
	int m_nOfflineMinutes;
	std::map<int, std::vector<AntiAddictionConfigItem> > m_mapAntiAddictionConfig;
	std::map<int, int> m_mapPlatformConfig;
};

#endif

