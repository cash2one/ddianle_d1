#ifndef _PROPRIETARY_ACTIVITY_H__
#define _PROPRIETARY_ACTIVITY_H__

#include <string>
#include <list>
#include "../../datastructure/DataStruct_Base.h"
#include "TimerActivity.h"

class CProprietaryActivityShow : public CTimeActivity
{
public:
	CProprietaryActivityShow(){}
	~CProprietaryActivityShow(){}

public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};

/////////////////////////////////////
class CProprietaryActivityReward
{
public:
	CProprietaryActivityReward(){}
	~CProprietaryActivityReward(){}

public:
	std::list<CItem> m_listMaleReward;
	std::list<CItem> m_listFemaleReward;
	int m_nMoney;
};

class CProprietaryActivityInfoMgr
{
public:
	CProprietaryActivityInfoMgr();
	~CProprietaryActivityInfoMgr(){}

public:
	static CProprietaryActivityInfoMgr &Instance()
	{
		static CProprietaryActivityInfoMgr s_proprietaryActivityInfoMgr;
		return s_proprietaryActivityInfoMgr;
	}

public:
	inline bool IsOpening(){ return m_bIsOpening; }
	inline void SetisOpening( bool bIsOpening ){ m_bIsOpening = bIsOpening; }
	inline bool Exhibit(){ return m_bExhibit; }
	inline int Weight(){ return m_nWeight; }
	inline const std::string& RegularImageName(){ return m_strRegularImageName; }
	inline const std::string & ThumbnailsName(){ return m_strThumbnailName; }
	inline const std::string & ActivityTitle(){ return m_strActivityTitle; }
	inline const std::string & ActivityContent(){ return m_strActivityContent; }
	inline const CProprietaryActivityReward & Rewards(){ return m_reward; }

public:
	void Init();
	void AddInfo( bool bExhibit, int nWeight,
		std::string& strRegularImageName, std::string& strThumbnailName,
		std::string& strActivityTitle, std::string& strActivityContent,
		unsigned int nShowBeginTime, unsigned int nShowEndTime,
		std::list<CItem>& maleItemList, std::list<CItem>& femaleItemList, int nMoney );


public:
	bool m_bIsOpening;
	bool m_bExhibit;
	int m_nWeight;
	std::string m_strRegularImageName;
	std::string m_strThumbnailName;
	std::string m_strActivityTitle;
	std::string m_strActivityContent;
	unsigned int m_nShowBeginTime;
	unsigned int m_nShowEndTime;

	CProprietaryActivityReward m_reward;
};

#endif //_PROPRIETARY_ACTIVITY_H__
