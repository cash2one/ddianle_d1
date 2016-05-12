#include "guidereward.h"
#include "../util/CommonFunc.h"
#include "../../socket/Log.h"

GuideRewardManager::GuideRewardManager()
{
	m_GuideStep = 0;
}

GuideRewardManager::~GuideRewardManager()
{
	m_GuideStep = 0;
	m_MaleReward.clear();
	m_FemaleReward.clear();
}

GuideRewardManager& GuideRewardManager::Instance()
{
	static GuideRewardManager g_GuideRewardManager;
	return g_GuideRewardManager;
}

void GuideRewardManager::AddGuideReward(int nStep , int sex,int nIndex,std::string& itemText)
{
	m_GuideStep = nStep;
	std::list<CItem> items;
	bool bRet = TransferItemList(itemText.c_str(),items);
	if (!bRet)
	{
		WriteLog(LOGLEVEL_ERROR, "GuideRewardManager::AddGuideReward error,invalid item text!");
		return;
	}

	if (sex == ESexType_Male)
	{
		if (m_MaleReward.find(nIndex) == m_MaleReward.end())
		{
			m_MaleReward.insert(make_pair(nIndex,items));
		}
	}
	else if (sex == ESexType_Female)
	{
		if (m_FemaleReward.find(nIndex) == m_FemaleReward.end())
		{
			m_FemaleReward.insert(make_pair(nIndex,items));
		}
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "GuideRewardManager::AddGuideReward error,invalid sex type!");
	}
}

bool GuideRewardManager::GetGuideReward(int nStep , int sex,int nIndex,std::list<CItem>& items)
{
	if (nStep != m_GuideStep)
	{
		return false;
	}

	if (sex == ESexType_Male)
	{
		std::map<int,std::list<CItem> >::iterator it = m_MaleReward.find(nIndex);
		if (it == m_MaleReward.end())
		{
			return false;
		}
		std::list<CItem>::iterator itItem = it->second.begin();
		for (;itItem != it->second.end() ; itItem++)
		{
			items.push_back(*itItem);
		}
		return true;
	}
	else if (sex == ESexType_Female)
	{
		std::map<int,std::list<CItem> >::iterator it = m_FemaleReward.find(nIndex);
		if (it == m_FemaleReward.end())
		{
			return false;
		}
		std::list<CItem>::iterator itItem = it->second.begin();
		for (;itItem != it->second.end() ; itItem++)
		{
			items.push_back(*itItem);
		}
		return true;
	}
	return false;
}

void GuideRewardManager::AddMailInfo(std::string& title,std::string& content)
{
	mailTitle = title;
	mailContent = content;
}



void GuideRewardManager::SerializeSystemSettingInfo( CParamPool & IOBuff )
{
    IOBuff.AddUShort( (unsigned short)m_MaleReward.size() );
    std::map<int,std::list<CItem> >::iterator itMale = m_MaleReward.begin();
    for( ; itMale != m_MaleReward.end(); ++itMale )
    {
        IOBuff.AddUShort( (unsigned short)itMale->second.size());
        std::list<CItem>::iterator iter = itMale->second.begin();
        for ( ; iter != itMale->second.end(); ++iter )
        {
            IOBuff.AddUInt( iter->m_nItemType );
        }
    }

    IOBuff.AddUShort( (unsigned short)m_FemaleReward.size() );
    std::map<int,std::list<CItem> >::iterator itFemale = m_FemaleReward.begin();
    for( ; itFemale != m_FemaleReward.end(); ++itFemale )
    {
        IOBuff.AddUShort( (unsigned short)itFemale->second.size());
        std::list<CItem>::iterator iter = itFemale->second.begin();
        for ( ; iter != itFemale->second.end(); ++iter )
        {
            IOBuff.AddUInt( iter->m_nItemType );
        }
    }
}
