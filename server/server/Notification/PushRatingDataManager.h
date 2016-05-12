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
	bool m_bEnable;						// ��������
	std::string m_strPushMessage;		// ������Ϣ����
	int m_nPushInterval;				// ����ʱ����
	int m_nMaxCount;					// ÿ��������ʹ���
	int m_nResetWeekday;				// �ܼ��������ʹ���

	std::set<int> m_levelSet;			// ����ʱ����

	int m_nOnlineTime;
	bool m_bEnableOnlineTimePush;		// ���ߴﵽһ��ʱ������

	bool m_bEnableFisrtDailyQuest;		// ���ÿ�յ�һ����������
	bool m_bEnableFisrtDailyShoping;	// ���ÿ�յĵ�һ�ι�������
};

#endif //__PUSH_RATING_DATA_MANAGER_H_H__

