#ifndef		__INFOBOARDMGR_H__
#define		__INFOBOARDMGR_H__

#include <string>
#include <list>
#include "../../datastructure/DataStruct_Base.h"

class CPlainTextActivityReward
{
public:
	CPlainTextActivityReward();
	~CPlainTextActivityReward();
public:
	std::string m_strRewardName;
	std::list<CItem> m_listMaleReward;
	std::list<CItem> m_listFemaleReward;
	int m_nMoney;
};

typedef std::list<CPlainTextActivityReward> PlainTextRewardContainer_t;

class CPlainTextActivityInfoMgr
{
private:
	CPlainTextActivityInfoMgr();
	~CPlainTextActivityInfoMgr();
public:
	static CPlainTextActivityInfoMgr & Instance();

	void Init();
public:
	void AddRewardInfo(std::string & strRewardName, std::list<CItem> & listMaleItem, std::list<CItem> & listFemale, int nMoney);
	void AddBriefInfo(bool bExhibit, int nWeight, 
					  std::string & strRegularImageName, std::string & strThumbnailName, 
					  std::string & strActivityTitle, std::string & strActivityContent,
					  unsigned int nShowBeginTime, unsigned int nShowEndTime );

	bool IsOpening();
	void SetIsOpening(bool isOpening);
	bool Exhibit();
	int Weight();
	const std::string & RegularImageName();
	const std::string & ThumbnailsName();
	const std::string & ActivityTitle();
	const std::string & ActivityContent();
	const PlainTextRewardContainer_t & Rewards();
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

	PlainTextRewardContainer_t m_rewards;
};



#endif



