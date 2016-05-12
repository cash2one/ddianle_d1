#ifndef __PUSH_RATING_DATA_MANAGER_H_H__
#define __PUSH_RATING_DATA_MANAGER_H_H__ 1

#include <set>
#include <string>

class CPushRatingDataManager
{
public:
	static CPushRatingDataManager & Instance();

public:
	CPushRatingDataManager();
	virtual ~CPushRatingDataManager();

public:
	bool IsEnabled();
	int Interval();
	std::string & Message();

	void SetPushRatingSystem(const char *pszPushMessage, int nInterval, bool bEnable, int nMaxCount, int nResetWeekday);

	bool CanPushOnLevel(int nLevel) const;
	bool CanPushOnlineTime(int nOnlineTime) const;
	bool CanPushFirstDailyQuest() const;
	bool CanPushFirstDailyShoping() const;
	bool ExceedMaxCount(int nCount) const;
	int ResetWeekday() const;

	void AddPushLevel(int nLevel);
	void SetPushOnlineTime(int nOnlineTime, bool enable = true);
	void SetPushFinishFirstDailyQuest(bool enable = true);
	void SetPushFinishFirstDailyShoping(bool enable = true);

protected:
	bool m_bEnable;						// 开启推送
	std::string m_strPushMessage;		// 推送消息内容
	int m_nPushInterval;				// 推送时间间隔
	int m_nMaxCount;					// 每周最大推送次数
	int m_nResetWeekday;				// 周几重置推送次数

	std::set<int> m_levelSet;			// 升级时推送

	int m_nOnlineTime;
	bool m_bEnableOnlineTimePush;		// 在线达到一定时长推送

	bool m_bEnableFisrtDailyQuest;		// 完成每日第一个任务推送
	bool m_bEnableFisrtDailyShoping;	// 完成每日的第一次购物推送
};

#endif //__PUSH_RATING_DATA_MANAGER_H_H__

