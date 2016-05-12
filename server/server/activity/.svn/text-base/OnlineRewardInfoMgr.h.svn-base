#ifndef		__ONLINEREWARDINFOMGR_H__
#define		__ONLINEREWARDINFOMGR_H__

#include <map>
#include "../../datastructure/DataStruct_Base.h"
#include "TimerActivity.h"

class CRoleEntity;

class COnlineRewardInfo
{
public:
	COnlineRewardInfo();
	~COnlineRewardInfo();
public:
	int					m_nActivityIndex;
	int					m_nIndex;	
	unsigned int		m_nRequireTime;
	std::list<CItem>	m_listMaleReward;
	std::list<CItem>	m_listFemaleReward;
	int                 m_nMoney;
	time_t				m_tStartTime;
	time_t				m_tEndTime;
};

class COnlineRewardAcitvity:
	public CTimeActivity
{
public:
	COnlineRewardAcitvity();
	~COnlineRewardAcitvity();
public:
	void Init(time_t tBeginTime, time_t tEndTime, unsigned short nActivityIdx);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
private:
	unsigned short m_nActivityIdx;
};

class COnlineRewardInfoMgr
{
private:
	COnlineRewardInfoMgr();
	~COnlineRewardInfoMgr();
public:
	static COnlineRewardInfoMgr & Instance();
	bool Init();

	bool IsShowing();
	bool Exhibit();
	int Weight();
	unsigned int ShowBeginTime();
	unsigned int ShowEndTime();
	unsigned int ActivityBeginTime();
	unsigned int ActivityEndTime();
	bool ActivityTimeToShow(unsigned int& nBeginTime, unsigned int& nEndTime, std::string& strContent);
	std::string & RegularImageName();
	std::string & ThumbnailName();
	std::string & ActivityName();
	const std::map<unsigned short, COnlineRewardInfo *> & OnlineRewardInfoMap();
	const std::map<unsigned short, COnlineRewardInfo *> & GetMapOnlineRewardInfoToShow() const { return m_mapOnlineRewardInfoToShow; }

	void ReadBriefInfo(int nWeight, bool bExhibit,
					   unsigned int nShowStarttime, unsigned int nShowEndtime, 
					   std::string & rStrRegularImageName, std::string & rStrThumbnailName,
					   std::string & rStrActivityName, std::string & rStrContent,
					   int nResetHour, int nResetMin, int nResetSec);
	void ReadOnlineRewardInfo(COnlineRewardInfo &onlineRewardInfo);
	COnlineRewardInfo *GetOnlineRewardInfo(int nIndex);
	void GetRewardList(int nIndex, std::list<CItem> & listReward, unsigned char nSex);
	int GetRewardMoney(int nIndex);
	time_t GetResetTime(time_t nNow);
	void SetIsOpening(bool bOpening);
	bool IsOpening();
	unsigned short GetCurActivityIdx() const { return m_nCurActivityIndex; }
	void SetCurActivityIdx(unsigned short nActivityIndex) ;
	unsigned short GetActivityIdxToShow() const { return m_nActivityShowIndex; }
	void SetActivityIdxToShow(unsigned short val) { m_nActivityShowIndex = val; }
	bool GetCurActivityTime(time_t& nBeginTime, time_t& nEndTime);
	bool IsTimeInCurActivityTimeRange(time_t t);
    bool IsEndActivityIndex(int nIndex);
public:
	bool m_bIsOpening;
	bool m_bExhibit;
	int m_nWeight;
	unsigned int m_nShowBeginTime;
	unsigned int m_nShowEndTime;
	unsigned int m_nActivityBeginTime;
	unsigned int m_nActivityEndTime;
	std::string m_strRegularImageName;
	std::string m_strThumbnailName;
	std::string m_strActivityName;
	std::string m_strContentFormat;
	int          m_nResetHour;
	int          m_nResetMin;
	int          m_nResetSec;
	unsigned short m_nCurActivityIndex;
	unsigned short m_nActivityShowIndex;
	std::map<unsigned short, COnlineRewardInfo *> m_mapOnlineRewardInfo;
	std::map<unsigned short, COnlineRewardInfo *> m_mapOnlineRewardInfoToShow;
	std::map<time_t, time_t> m_mapActivityTimeRange;
	std::map<time_t, unsigned short> m_mapActivityTime2Idx;//start time <=> activity index
};




#endif

