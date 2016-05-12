#ifndef __GUIDE_REWARD_MGR_H__
#define __GUIDE_REWARD_MGR_H__

#include <map>
#include <list>
#include "../../datastructure/DataStruct_Base.h"

class GuideRewardManager
{
public:
	GuideRewardManager();
	~GuideRewardManager();

public:
	static GuideRewardManager& Instance();
	void AddGuideReward(int nStep,int sex,int nIndex,std::string& itemText);
	void AddMailInfo(std::string& title,std::string& content);

	bool GetGuideReward(int nStep,int sex,int nIndex,std::list<CItem>& items);

	std::string& MailTitle() { return mailTitle; }
	std::string& MailContent() { return mailContent ;}

 
    void SerializeSystemSettingInfo( CParamPool & IOBuff );

private:
	std::map<int,std::list<CItem> > m_MaleReward;
	std::map<int,std::list<CItem> > m_FemaleReward;

	int m_GuideStep;
	std::string mailTitle;
	std::string mailContent;
};

#endif
