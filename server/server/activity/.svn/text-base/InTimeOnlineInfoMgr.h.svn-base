#ifndef		__INTIMEONLINEINFOMGR_H__
#define		__INTIMEONLINEINFOMGR_H__

#include <string>
#include <list>
#include "../../datastructure/DataStruct_Base.h"
#include "InTimeOnlineActivity.h"

class CParamPool;

class CInTimeActivityInfo
{
public:
	CInTimeActivityInfo();
	~CInTimeActivityInfo();
public:
	void doEncode(CParamPool & IOBuff);
public:
	int m_nActivityIndex;
	time_t m_tActivityBeginTime;
	time_t m_tActivityEndTime;
	int m_nTriggerHour;
	int m_nTriggerMin;
	std::list<CItem> m_listMaleItems;
	std::list<CItem> m_listFemaleItems;
	int m_nMoney;
	std::string m_strMailTitle;
	std::string m_strMailContent;
};

typedef	std::list<CInTimeActivityInfo *> InTimeActivityInfo_t;

class CInTimeOnlineInfoMgr
{
public:
	CInTimeOnlineInfoMgr();
	~CInTimeOnlineInfoMgr();
public:
	static CInTimeOnlineInfoMgr & Instance();
	void Init();
public:
	void AddActivity(int nActivityIndex, const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime,
					 const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward, 
					 const char * szTitle, const char * szContent);
	void AddBriefInfo(bool bExhibit, int nWeight,
					  std::string & strRegularImageName, std::string & strThumblnailName, 
					  std::string & rStrActivityName, std::string & strDetail,
		              time_t tShowTimeBegin, time_t tShowTimeEnd,
					  std::string & strShowBeginAnnounce,
					  std::string & strShowEndAnnounce);

	void SetIsShowOpening(bool bIsOpening);
	bool IsShowOpening();
	bool Exhibit();
	int Weight();
	const std::string & RegularImageName();
	const std::string & ThumblnailsName();
	const std::string & ActivityName();
	const std::string & ActivityDetail();
	time_t ShowTimeBegin();
	time_t ShowTimeEnd();
	const std::string & ActivityBeginAnnounce();
	const std::string & ActivityEndAnnounce();
	const InTimeActivityInfo_t & Activities();
public:
	bool m_bIsShowOpening;
	bool m_bExhibit;
	int m_nWeight;
	std::string m_strRegularImageName;
	std::string m_strThumblnailsName;
	std::string m_strActivityName;
	std::string m_strActivityDetail;
	time_t m_tShowTimeBegin;
	time_t m_tShowTimeEnd;
	std::string m_strActivityBeginAnnounce;
	std::string m_strActivityEndAnnounce;

	InTimeActivityInfo_t m_listActivities;
};











#endif



