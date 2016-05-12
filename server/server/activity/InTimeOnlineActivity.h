#ifndef	__INTIME_ONLINE_ACTIVITY_H__
#define __INTIME_ONLINE_ACTIVITY_H__

#include "TimerActivity.h"
#include <list>
#include <string>

class CItem;
class CInTimeOnlineActivity : public CDayActivity
{
public:
	CInTimeOnlineActivity();
	virtual ~CInTimeOnlineActivity();

	//��ʼ��
	void Init(int nActivityID, const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime,
		const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward, const char * szTitle, const char * szContent);
	//����
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//�����ʱ
	virtual void OnTime(const unsigned long &lTimeElapsed, time_t tNow);
private:
	//������
	void _DealReward(const unsigned long &lTimeElapsed);
private:
	int m_nActivityID;			//����
	std::string m_strTitle;		//�ʼ�����
	std::string m_strContent;	//�ʼ�����
	std::list< CItem > m_MaleRewardItemList;	//���Խ�����Ʒ�б�
	std::list< CItem > m_FemaleRewardItemList;	//Ů�Խ�����Ʒ�б�
	unsigned int m_nMoneyReward;				//��Ǯ����

	std::list< unsigned int > m_RoleList;	//������Ⱥ
};

class CShowInTimeOnlineActivityInfo: 
	public CTimeActivity
{
public:
	CShowInTimeOnlineActivityInfo();
	~CShowInTimeOnlineActivityInfo();
public:

public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};

#endif


