#ifndef		__GIFTCODEINFOMGR__H__
#define		__GIFTCODEINFOMGR__H__

#include <string>

class CGiftCodeInfoMgr
{
private:
	CGiftCodeInfoMgr();
	~CGiftCodeInfoMgr();
public:
	static CGiftCodeInfoMgr & Instance();
	void AddBriefInfo(bool bExhibit, int nWeight, 
					  std::string & strRegularImageName, std::string & strThumbnailsName, 
					  std::string & strActivityTitle, std::string & strActivityContent,
					  time_t tShowBeginTime, time_t tShowEndTime, 
					  time_t tActivityBeginTime, time_t tActivityEndTime );

	void Init();
	bool IsActivityOpening();

	void SetIsShowing(bool isShowing);
	bool IsShowing();
	bool Exhibit();
	int Weight();
	const std::string & RegularImageName();
	const std::string & ThumbnailsName();
	time_t ActivityStartTime();
	time_t ActivityEndTime();
	std::string & ActivityTitle();
	std::string & ActivityContent();
public:
	bool m_bIsShowing;
	bool m_bExhibit;
	int m_nWeight;
	std::string	m_strRegularImageName;
	std::string m_strThumbnailsName;
	time_t m_tShowBeginTime;
	time_t m_tShowEndTime;
	time_t m_tActivityBeginTime;
	time_t m_tActivityEndTime;
	std::string m_strActivityTitle;
	std::string m_strActivityContent;
};




#endif


