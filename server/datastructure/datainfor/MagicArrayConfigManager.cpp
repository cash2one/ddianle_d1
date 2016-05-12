#include "MagicArrayConfigManager.h"

MagicArrayConfigManager::MagicArrayConfigManager()
{

}

MagicArrayConfigManager::~MagicArrayConfigManager()
{
    Destory();
}

MagicArrayConfigManager & MagicArrayConfigManager::Instance()
{
	static MagicArrayConfigManager g_staticConfigManager;
	return g_staticConfigManager;
}

MagicArrayConfig * MagicArrayConfigManager::GetMagicArrayConfig(unsigned int nMagicArrayID)
{
	std::map<unsigned int, MagicArrayConfig*>::iterator it = m_mapMagicArrayConfig.find(nMagicArrayID);
	if (it != m_mapMagicArrayConfig.end())
	{
		return it->second;
	}

	return NULL;
}

void MagicArrayConfigManager::GetRefreshExchangeCost(int & nCurrencyType, int & nCurrencyValue)
{
    std::map<unsigned int, MagicArrayConfig*>::iterator it = m_mapMagicArrayConfig.begin();
    if (it != m_mapMagicArrayConfig.end())
    {
        nCurrencyType = it->second->m_nRefreshExchangeCType;
        nCurrencyValue = it->second->m_nRefreshExchangeCValue;
    }
}

bool MagicArrayConfigManager::IsInRefreshValidTime(unsigned int nLastRefreshTime)
{
    std::map<unsigned int, MagicArrayConfig*>::iterator it = m_mapMagicArrayConfig.begin();
    if (it != m_mapMagicArrayConfig.end())
    {
       if (nLastRefreshTime >= (unsigned int)it->second->m_nRefreshBeginTime && nLastRefreshTime <= (unsigned int)it->second->m_nRefreshEndTime)
       {
           return true;
       }
    }

    return false;
}

int MagicArrayConfigManager::GetGroupRollTotalRate(int nSex, unsigned int nMagicArrayID, unsigned int nGroupID)
{
    if (nSex == (int)ESexType_Male)
    {
        std::map<unsigned int, std::map<unsigned int, int> >::iterator it = m_mapMaleRewardTotalRate.find(nMagicArrayID);
        if (it != m_mapMaleRewardTotalRate.end())
        {
            std::map<unsigned int, int>::iterator itr = it->second.find(nGroupID);
            if (itr != it->second.end())
            {
                return itr->second;
            }
        }
    }
    else
    {
        std::map<unsigned int, std::map<unsigned int, int> >::iterator it = m_mapFemaleRewardTotalRate.find(nMagicArrayID);
        if (it != m_mapFemaleRewardTotalRate.end())
        {
            std::map<unsigned int, int>::iterator itr = it->second.find(nGroupID);
            if (itr != it->second.end())
            {
                return itr->second;
            }
        }
    }
    
    return 0;
}

void MagicArrayConfigManager::GetMagicArrayCompensation(unsigned int nMagicArrayID, unsigned int nGroupID, int nSex, std::list<CItem> & listItem)
{
    if (nSex == (int)ESexType_Male)
    {
        std::map<unsigned int, std::map<unsigned int, std::list<MagicArrayCompensation> > >::iterator it = m_mapMaleCompensation.find(nMagicArrayID);
        if (it != m_mapMaleCompensation.end())
        {
            std::map<unsigned int, std::list<MagicArrayCompensation> >::iterator itr = it->second.find(nGroupID);
            if (itr != it->second.end())
            {
                std::list<MagicArrayCompensation>::iterator itt = itr->second.begin();
                for (; itt != itr->second.end(); ++itt)
                {
                    listItem.push_back(itt->m_nCompessationItem);
                }
            }
        }
    }
    else
    {
        std::map<unsigned int, std::map<unsigned int, std::list<MagicArrayCompensation> > >::iterator it = m_mapFemaleCompensation.find(nMagicArrayID);
        if (it != m_mapFemaleCompensation.end())
        {
            std::map<unsigned int, std::list<MagicArrayCompensation> >::iterator itr = it->second.find(nGroupID);
            if (itr != it->second.end())
            {
                std::list<MagicArrayCompensation>::iterator itt = itr->second.begin();
                for (; itt != itr->second.end(); ++itt)
                {
                    listItem.push_back(itt->m_nCompessationItem);
                }
            }
        }
    }
}

void MagicArrayConfigManager::RollMagicArrayReward(unsigned int nMagicArrayID, int nSex, const std::list<GroupRate> & listRate, std::list<MagicArrayReward> & listReward)
{
	// roll group 
	unsigned int nGroupID = 0;
	int nTempRate = 0;
	int nRollValue = Random()%10000;
	std::list<GroupRate>::const_iterator it = listRate.begin();
	for (; it != listRate.end(); ++it)
	{
		if (it->nRate + nTempRate >= (unsigned int)nRollValue)
		{
			nGroupID = it->nGroupID;
			break;
		}

		nTempRate += it->nRate;
	}

    int nGroupTotalRate = GetGroupRollTotalRate(nSex, nMagicArrayID, nGroupID);
    nRollValue = Random() % nGroupTotalRate;

    if (nSex == (int)ESexType_Male)
    {
        std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayReward> > >::iterator itr = m_mapMaleReward.find(nMagicArrayID);
        if (itr != m_mapMaleReward.end())
        {
            std::map<unsigned int, std::vector<MagicArrayReward> >::iterator itt = itr->second.find(nGroupID);
            if (itt != itr->second.end())
            {
                int nTemp = 0;
                for (size_t i = 0; i < itt->second.size(); ++i)
                {
                    if (itt->second[i].m_nWeight + nTemp >= nRollValue)
                    {
                        listReward.push_back(itt->second[i]);
                        break;
                    }
                    nTemp += itt->second[i].m_nWeight;
                }
            }
        }
    }
    else
    {
        std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayReward> > >::iterator itr = m_mapFemaleReward.find(nMagicArrayID);
        if (itr != m_mapFemaleReward.end())
        {
            std::map<unsigned int, std::vector<MagicArrayReward> >::iterator itt = itr->second.find(nGroupID);
            if (itt != itr->second.end())
            {
                int nTemp = 0;
                for (size_t i = 0; i < itt->second.size(); ++i)
                {
                    if (itt->second[i].m_nWeight + nTemp >= nRollValue)
                    {
                        listReward.push_back(itt->second[i]);
                        break;
                    }
                    nTemp += itt->second[i].m_nWeight;
                }
            }
        }
    }
    
}

void MagicArrayConfigManager::GetBaseExchangeConfig(unsigned int nExchangeID, int nSex, CItem & exchangeitem, CItem & costitem)
{
    if (nSex == (int)ESexType_Male)
    {
        std::map<unsigned int, MagicArrayBaseExchange>::iterator it = m_mapMaleBaseExchangeConfig.find(nExchangeID);
        if (it != m_mapMaleBaseExchangeConfig.end())
        {
            exchangeitem = it->second.m_ExchangeItem;
            costitem = it->second.m_CostItem;
        }
    }
    else
    {
        std::map<unsigned int, MagicArrayBaseExchange>::iterator it = m_mapFemaleBaseExchangeConfig.find(nExchangeID);
        if (it != m_mapFemaleBaseExchangeConfig.end())
        {
            exchangeitem = it->second.m_ExchangeItem;
            costitem = it->second.m_CostItem;
        }
    }
}

void MagicArrayConfigManager::doBaseExchangeEncode(int nSex, CParamPool & IOBuff)
{
    if (nSex == (int)ESexType_Male)
    {
        std::map<unsigned int, MagicArrayBaseExchange>::iterator it = m_mapMaleBaseExchangeConfig.begin();
        if (it != m_mapMaleBaseExchangeConfig.end())
        {
            IOBuff.AddUInt(it->second.m_CostItem.m_nItemType);
        }

        IOBuff.AddUShort((unsigned short)m_mapMaleBaseExchangeConfig.size());
        it = m_mapMaleBaseExchangeConfig.begin();
        for (; it != m_mapMaleBaseExchangeConfig.end(); ++it)
        {
            IOBuff.AddUShort((unsigned short)it->second.m_nExchangeID);
            IOBuff.AddInt(it->second.m_CostItem.m_nItemCount);
            it->second.m_ExchangeItem.doEncode(IOBuff);
        }
    }
    else
    {
        std::map<unsigned int, MagicArrayBaseExchange>::iterator it = m_mapFemaleBaseExchangeConfig.begin();
        if (it != m_mapFemaleBaseExchangeConfig.end())
        {
            IOBuff.AddUInt(it->second.m_CostItem.m_nItemType);
        }

        IOBuff.AddUShort((unsigned short)m_mapFemaleBaseExchangeConfig.size());
        it = m_mapFemaleBaseExchangeConfig.begin();
        for (; it != m_mapFemaleBaseExchangeConfig.end(); ++it)
        {
            IOBuff.AddUShort((unsigned short)it->second.m_nExchangeID);
            IOBuff.AddInt(it->second.m_CostItem.m_nItemCount);
            it->second.m_ExchangeItem.doEncode(IOBuff);
        }
    }
}

void MagicArrayConfigManager::RefreshExchangeGroup(std::map<unsigned int, unsigned int> & mapExchangeGroup)
{
    std::map<unsigned int, std::vector<exchangegroup_t> >::iterator it = m_mapExchangeTeam.begin();
    for (; it != m_mapExchangeTeam.end(); ++it)
    {
        if (!it->second.empty())
        {
            int nRollValue = Random(1, (int)it->second.size());
            mapExchangeGroup[it->first] = it->second[nRollValue-1].first;
        }
    }
}

void MagicArrayConfigManager::GetReward(unsigned int nMagicArrayID, unsigned int nGroupID, int nSex, std::vector<CItem> & vecPreviewItem)
{
    if (nSex == (int)ESexType_Male)
    {
        std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayPreview> > >::iterator itp = m_mapMalePreview.find(nMagicArrayID);
        if (itp != m_mapMalePreview.end())
        {
            std::map<unsigned int, std::vector<MagicArrayPreview> >::iterator itr = itp->second.find(nGroupID);
            if (itr != itp->second.end())
            {
                for (std::vector<MagicArrayPreview>::iterator itt = itr->second.begin(); itt != itr->second.end(); ++itt)
                {
                    vecPreviewItem.push_back(itt->m_Item);
                }
            }
        }
    }
    else
    {
        std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayPreview> > >::iterator itp = m_mapFemalePreview.find(nMagicArrayID);
        if (itp != m_mapFemalePreview.end())
        {
            std::map<unsigned int, std::vector<MagicArrayPreview> >::iterator itr = itp->second.find(nGroupID);
            if (itr != itp->second.end())
            {
                for (std::vector<MagicArrayPreview>::iterator itt = itr->second.begin(); itt != itr->second.end(); ++itt)
                {
                    vecPreviewItem.push_back(itt->m_Item);
                }
            }
        }
    }
}

void MagicArrayConfigManager::GetRrfreshExchangeConfig(unsigned int nExchangeID, int nSex, CItem & exchangeitem, CItem & costitem)
{
    if (nSex == (int)ESexType_Male)
    {
        std::map<unsigned int, MagicArrayExchange>::iterator it = m_mapMaleExchangeConfig.find(nExchangeID);
        if (it != m_mapMaleExchangeConfig.end())
        {
            exchangeitem = it->second.m_ExchangeItem;
            costitem = it->second.m_CostItem;
        }
    }
    else
    {
        std::map<unsigned int, MagicArrayExchange>::iterator it = m_mapFemaleExchangeConfig.find(nExchangeID);
        if (it != m_mapFemaleExchangeConfig.end())
        {
            exchangeitem = it->second.m_ExchangeItem;
            costitem = it->second.m_CostItem;
        }
    }
}

void MagicArrayConfigManager::GetRrfreshExchangeConfig(int nSex, unsigned int nExchangeID, std::list<MagicArrayExchange> & listExchange)
{
    if (nSex == (int)ESexType_Male)
    {
        std::map<unsigned int, MagicArrayExchange>::iterator it = m_mapMaleExchangeConfig.find(nExchangeID);
        if (it != m_mapMaleExchangeConfig.end())
        {
            listExchange.push_back(it->second);
        }
    }
    else
    {
        std::map<unsigned int, MagicArrayExchange>::iterator it = m_mapFemaleExchangeConfig.find(nExchangeID);
        if (it != m_mapFemaleExchangeConfig.end())
        {
            listExchange.push_back(it->second);
        }
    }
}

void MagicArrayConfigManager::GetExchangeInfo(int nSex, unsigned int nTeamID, unsigned int nGroupID, std::list<MagicArrayExchange> & listExchange)
{
    std::map<unsigned int, std::vector<exchangegroup_t> >::iterator it = m_mapExchangeTeam.find(nTeamID);
    if (it != m_mapExchangeTeam.end())
    {
        std::vector<exchangegroup_t>::iterator itr = it->second.begin();
        for (; itr != it->second.end(); ++itr)
        {
            if (itr->first == nGroupID)
            {
                GetRrfreshExchangeConfig(nSex, itr->second, listExchange);
                break;
            }
        }
    }
}

itemtype_t MagicArrayConfigManager::GetExchangeCostItemType()
{
    std::map<unsigned int, MagicArrayExchange>::iterator it = m_mapMaleExchangeConfig.begin();
    if (it != m_mapMaleExchangeConfig.end())
    {
        return it->second.m_CostItem.m_nItemType;
    }

    return 0;
}

bool MagicArrayConfigManager::LoadConfig(const char* pszFileName)
{
	bool bRes = true;

	CFileStream file;
	file.open(pszFileName, "rb");
	if (file.IsOpen())
	{
		unsigned short ulNumber = 0;
		file.ReadUShort(ulNumber);
		unsigned short i;

		if (ulNumber > 0)
		{
			for (i = 0; i<ulNumber; i++)
			{
				MagicArrayReward reward;
                reward.m_nGroupID = 1;
				reward.Load(file);

				AddReward(reward);
			}
		}

        file.ReadUShort(ulNumber);
        for (i = 0; i < ulNumber; i++)
        {
            MagicArrayReward reward;
            reward.m_nGroupID = 2;
            reward.Load(file);

            AddReward(reward);
        }

        file.ReadUShort(ulNumber);
        for (i = 0; i < ulNumber; i++)
        {
            MagicArrayReward reward;
            reward.m_nGroupID = 3;
            reward.Load(file);

            AddReward(reward);
        }

        file.ReadUShort(ulNumber);
        for (i = 0; i < ulNumber; i++)
        {
            MagicArrayReward reward;
            reward.m_nGroupID = 4;
            reward.Load(file);

            AddReward(reward);
        }

        CalcMaleRewardTotalRate();
        CalcFemaleRewardTotalRate();

        // preview
        file.ReadUShort(ulNumber);
        for (i = 0; i < ulNumber; i++)
        {
            MagicArrayPreview preview;
            preview.m_nGroupID = 1;
            preview.Load(file);

            AddPreview(preview);
        }

        file.ReadUShort(ulNumber);
        for (i = 0; i < ulNumber; i++)
        {
            MagicArrayPreview preview;
            preview.m_nGroupID = 2;
            preview.Load(file);

            AddPreview(preview);
        }

        file.ReadUShort(ulNumber);
        for (i = 0; i < ulNumber; i++)
        {
            MagicArrayPreview preview;
            preview.m_nGroupID = 3;
            preview.Load(file);

            AddPreview(preview);
        }

        file.ReadUShort(ulNumber);
        for (i = 0; i < ulNumber; i++)
        {
            MagicArrayPreview preview;
            preview.m_nGroupID = 4;
            preview.Load(file);

            AddPreview(preview);
        }

        // compensation
		file.ReadUShort(ulNumber);
		if (ulNumber>0)
		{
			for (i = 0; i < ulNumber; i++)
			{
				MagicArrayCompensation compensation;
                compensation.m_nGroupID = 1;
				compensation.Load(file);
				AddCompensation(compensation);
			}
		}

        file.ReadUShort(ulNumber);
        if (ulNumber>0)
        {
            for (i = 0; i < ulNumber; i++)
            {
                MagicArrayCompensation compensation;
                compensation.m_nGroupID = 2;
                compensation.Load(file);
                AddCompensation(compensation);
            }
        }

        file.ReadUShort(ulNumber);
        if (ulNumber>0)
        {
            for (i = 0; i < ulNumber; i++)
            {
                MagicArrayCompensation compensation;
                compensation.m_nGroupID = 3;
                compensation.Load(file);
                AddCompensation(compensation);
            }
        }

        file.ReadUShort(ulNumber);
        if (ulNumber>0)
        {
            for (i = 0; i < ulNumber; i++)
            {
                MagicArrayCompensation compensation;
                compensation.m_nGroupID = 4;
                compensation.Load(file);
                AddCompensation(compensation);
            }
        }

        // exchange
        file.ReadUShort(ulNumber);
        if (ulNumber>0)
        {
            for (i = 0; i < ulNumber; i++)
            {
                MagicArrayBaseExchange exchange;
                exchange.Load(file);
                AddBaseExchange(exchange);
            }
        }

		file.ReadUShort(ulNumber);
		if (ulNumber>0)
		{
			for (i = 0; i < ulNumber; i++)
			{
				MagicArrayExchange exchange;
				exchange.Load(file);
				AddExchange(exchange);
			}
		}

        // config
        file.ReadUShort(ulNumber);
        if (ulNumber>0)
        {
            for (i = 0; i < ulNumber; i++)
            {
                MagicArrayConfig *pConfig = new MagicArrayConfig();
                pConfig->Load(file);
                m_mapMagicArrayConfig[pConfig->m_nMagicArrayID] = pConfig;
            }
        }

		file.close();
	}
	else
	{
		bRes = false;
	}
	return bRes;
}

void MagicArrayConfigManager::doMaleEncode(CParamPool &IOBuff)
{

}

void MagicArrayConfigManager::RollFemaleReward(int nGroupID, std::list<CItem> & listReward)
{

}

void MagicArrayConfigManager::GetMagicArrayID(std::list<unsigned int> & listMagicArrayID)
{
    std::map<unsigned int, MagicArrayConfig*>::iterator it = m_mapMagicArrayConfig.begin();
    for (; it != m_mapMagicArrayConfig.end(); ++it)
    {
        listMagicArrayID.push_back(it->first);
    }
}

void MagicArrayConfigManager::doConfigEndoce(unsigned int nMagicArrayID, CParamPool &IOBuff)
{
    std::map<unsigned int, MagicArrayConfig*>::iterator it = m_mapMagicArrayConfig.find(nMagicArrayID);
    if (it != m_mapMagicArrayConfig.end())
    {
        it->second->doEncode(IOBuff);
    }
}

void MagicArrayConfigManager::AddReward(const MagicArrayReward & reward)
{
    if (reward.m_nSex == (int)ESexType_Male)
    {
        AddMaleReward(reward);
    }
    else if (reward.m_nSex == (int)ESexType_Female)
    {
        AddFemaleReward(reward);
    }
    else
    {
        AddMaleReward(reward);
        AddFemaleReward(reward);
    }
}

void MagicArrayConfigManager::AddMaleReward(const MagicArrayReward & reward)
{
    std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayReward> > >::iterator it = m_mapMaleReward.find(reward.m_nMagicArrayID);
    if (it != m_mapMaleReward.end())
    {
        std::map<unsigned int, std::vector<MagicArrayReward> >::iterator itr = it->second.find(reward.m_nGroupID);
        if (itr != it->second.end())
        {
            itr->second.push_back(reward);
        }
        else
        {
            std::vector<MagicArrayReward> vecTemp;
            vecTemp.push_back(reward);
            it->second.insert(make_pair(reward.m_nGroupID, vecTemp));
        }
    }
    else
    {
        std::vector<MagicArrayReward> vecTemp;
        vecTemp.push_back(reward);
        m_mapMaleReward[reward.m_nMagicArrayID].insert(make_pair(reward.m_nGroupID, vecTemp));
    }
}

void MagicArrayConfigManager::CalcMaleRewardTotalRate()
{
    std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayReward> > >::iterator it = m_mapMaleReward.begin();
    for (; it != m_mapMaleReward.end(); ++it)
    {
        std::map<unsigned int, std::vector<MagicArrayReward> >::iterator itr = it->second.begin();
        for (; itr != it->second.end(); ++itr)
        {
            int nTotalRate = 0;
            std::vector<MagicArrayReward>::iterator itt = itr->second.begin();
            for (; itt != itr->second.end(); ++itt)
            {
                nTotalRate += itt->m_nWeight;
            }
            m_mapMaleRewardTotalRate[it->first].insert(make_pair(itr->first, nTotalRate));
        }
    }
}

void MagicArrayConfigManager::CalcFemaleRewardTotalRate()
{
    std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayReward> > >::iterator it = m_mapFemaleReward.begin();
    for (; it != m_mapFemaleReward.end(); ++it)
    {
        std::map<unsigned int, std::vector<MagicArrayReward> >::iterator itr = it->second.begin();
        for (; itr != it->second.end(); ++itr)
        {
            int nTotalRate = 0;
            std::vector<MagicArrayReward>::iterator itt = itr->second.begin();
            for (; itt != itr->second.end(); ++itt)
            {
                nTotalRate += itt->m_nWeight;
            }
            m_mapFemaleRewardTotalRate[it->first].insert(make_pair(itr->first, nTotalRate));
        }
    }
}

void MagicArrayConfigManager::AddFemaleReward(const MagicArrayReward & reward)
{
    std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayReward> > >::iterator it = m_mapFemaleReward.find(reward.m_nMagicArrayID);
    if (it != m_mapFemaleReward.end())
    {
        std::map<unsigned int, std::vector<MagicArrayReward> >::iterator itr = it->second.find(reward.m_nGroupID);
        if (itr != it->second.end())
        {
            itr->second.push_back(reward);
        }
        else
        {
            std::vector<MagicArrayReward> vecTemp;
            vecTemp.push_back(reward);
            it->second.insert(make_pair(reward.m_nGroupID, vecTemp));
        }
    }
    else
    {
        std::vector<MagicArrayReward> vecTemp;
        vecTemp.push_back(reward);
        m_mapFemaleReward[reward.m_nMagicArrayID].insert(make_pair(reward.m_nGroupID, vecTemp));
    }
}

void MagicArrayConfigManager::AddPreview(const MagicArrayPreview & preview)
{
    if (preview.m_nSex == (int)ESexType_Male)
    {
        AddMalePreview(preview);
    }
    else if (preview.m_nSex == (int)ESexType_Female)
    {
        AddFemalePreview(preview);
    }
    else
    {
        AddMalePreview(preview);
        AddFemalePreview(preview);
    }
}

void MagicArrayConfigManager::AddMalePreview(const MagicArrayPreview & preview)
{
    std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayPreview> > >::iterator it = m_mapMalePreview.find(preview.m_nMagicArrayID);
    if (it != m_mapMalePreview.end())
    {
        std::map<unsigned int, std::vector<MagicArrayPreview> >::iterator itr = it->second.find(preview.m_nGroupID);
        if (itr != it->second.end())
        {
            itr->second.push_back(preview);
        }
        else
        {
            std::vector<MagicArrayPreview> vecTemp;
            vecTemp.push_back(preview);
            it->second.insert(make_pair(preview.m_nGroupID, vecTemp));
        }
    }
    else
    {
        std::vector<MagicArrayPreview> vecTemp;
        vecTemp.push_back(preview);
        m_mapMalePreview[preview.m_nMagicArrayID].insert(make_pair(preview.m_nGroupID, vecTemp));
    }
}

void MagicArrayConfigManager::AddFemalePreview(const MagicArrayPreview & preview)
{
    std::map<unsigned int, std::map<unsigned int, std::vector<MagicArrayPreview> > >::iterator it = m_mapFemalePreview.find(preview.m_nMagicArrayID);
    if (it != m_mapFemalePreview.end())
    {
        std::map<unsigned int, std::vector<MagicArrayPreview> >::iterator itr = it->second.find(preview.m_nGroupID);
        if (itr != it->second.end())
        {
            itr->second.push_back(preview);
        }
        else
        {
            std::vector<MagicArrayPreview> vecTemp;
            vecTemp.push_back(preview);
            it->second.insert(make_pair(preview.m_nGroupID, vecTemp));
        }
    }
    else
    {
        std::vector<MagicArrayPreview> vecTemp;
        vecTemp.push_back(preview);
        m_mapFemalePreview[preview.m_nMagicArrayID].insert(make_pair(preview.m_nGroupID, vecTemp));
    }
}

void MagicArrayConfigManager::AddCompensation(const MagicArrayCompensation & compensation)
{
    if (compensation.m_nSex == (int)ESexType_Male)
    {
        AddMaleCompensation(compensation);
    }
    else if (compensation.m_nSex == (int)ESexType_Female)
    {
        AddFemaleCompensation(compensation);
    }
    else
    {
        AddMaleCompensation(compensation);
        AddFemaleCompensation(compensation);
    }
}

void MagicArrayConfigManager::Destory()
{
    std::map<unsigned int, MagicArrayConfig*>::iterator it = m_mapMagicArrayConfig.begin();
    for (; it != m_mapMagicArrayConfig.end(); ++it)
    {
        delete it->second; it->second = NULL;
    }
    m_mapMagicArrayConfig.clear();

    m_mapMaleReward.clear();
    m_mapMalePreview.clear();
    m_mapMaleRewardTotalRate.clear();
    m_mapFemaleReward.clear();
    m_mapFemalePreview.clear();
    m_mapFemaleRewardTotalRate.clear();
    m_mapMaleCompensation.clear();
    m_mapFemaleCompensation.clear();
    m_mapMaleBaseExchangeConfig.clear();
    m_mapFemaleBaseExchangeConfig.clear();
    m_mapExchangeTeam.clear();
    m_mapMaleExchangeConfig.clear();
    m_mapFemaleExchangeConfig.clear();
}

void MagicArrayConfigManager::AddMaleCompensation(const MagicArrayCompensation & compensation)
{
    std::map<unsigned int, std::map<unsigned int, std::list<MagicArrayCompensation> > >::iterator it = m_mapMaleCompensation.find(compensation.m_nMagicArrayID);
    if (it != m_mapMaleCompensation.end())
    {
        std::map<unsigned int, std::list<MagicArrayCompensation> >::iterator itr = it->second.find(compensation.m_nGroupID);
        if (itr != it->second.end())
        {
            itr->second.push_back(compensation);
        }
        else
        {
            std::list<MagicArrayCompensation> listTemp;
            listTemp.push_back(compensation);
            it->second[compensation.m_nGroupID] = listTemp;
        }
    }
    else
    {
        std::list<MagicArrayCompensation> listTemp;
        listTemp.push_back(compensation);
        m_mapMaleCompensation[compensation.m_nMagicArrayID].insert(make_pair(compensation.m_nGroupID, listTemp));
    }
}

void MagicArrayConfigManager::AddFemaleCompensation(const MagicArrayCompensation & compensation)
{
    std::map<unsigned int, std::map<unsigned int, std::list<MagicArrayCompensation> > >::iterator it = m_mapFemaleCompensation.find(compensation.m_nMagicArrayID);
    if (it != m_mapFemaleCompensation.end())
    {
        std::map<unsigned int, std::list<MagicArrayCompensation> >::iterator itr = it->second.find(compensation.m_nGroupID);
        if (itr != it->second.end())
        {
            itr->second.push_back(compensation);
        }
        else
        {
            std::list<MagicArrayCompensation> listTemp;
            listTemp.push_back(compensation);
            it->second[compensation.m_nGroupID] = listTemp;
        }
    }
    else
    {
        std::list<MagicArrayCompensation> listTemp;
        listTemp.push_back(compensation);
        m_mapFemaleCompensation[compensation.m_nMagicArrayID].insert(make_pair(compensation.m_nGroupID, listTemp));
    }
}

void MagicArrayConfigManager::AddExchange(const MagicArrayExchange & exchange)
{
    if (exchange.m_nSex == (int)ESexType_Male)
    {
        m_mapMaleExchangeConfig[exchange.m_nExchangeID] = exchange;

        exchangegroup_t group(exchange.m_nGroupID, exchange.m_nExchangeID);
        m_mapExchangeTeam[exchange.m_nTeamID].push_back(group);
    }
    else if (exchange.m_nSex == (int)ESexType_Female)
    {
        m_mapFemaleExchangeConfig[exchange.m_nExchangeID] = exchange;

        exchangegroup_t group(exchange.m_nGroupID, exchange.m_nExchangeID);
        m_mapExchangeTeam[exchange.m_nTeamID].push_back(group);
    }
    else
    {
        m_mapMaleExchangeConfig[exchange.m_nExchangeID] = exchange;
        m_mapFemaleExchangeConfig[exchange.m_nExchangeID] = exchange;

        exchangegroup_t group(exchange.m_nGroupID, exchange.m_nExchangeID);
        m_mapExchangeTeam[exchange.m_nTeamID].push_back(group);
    }
}

void MagicArrayConfigManager::AddBaseExchange(const MagicArrayBaseExchange & exchange)
{
    if (exchange.m_nSex == (int)ESexType_Male)
    {
        m_mapMaleBaseExchangeConfig[exchange.m_nExchangeID] = exchange;
    }
    else if (exchange.m_nSex == (int)ESexType_Female)
    {
        m_mapFemaleBaseExchangeConfig[exchange.m_nExchangeID] = exchange;
    }
    else
    {
        m_mapMaleBaseExchangeConfig[exchange.m_nExchangeID] = exchange;
        m_mapFemaleBaseExchangeConfig[exchange.m_nExchangeID] = exchange;
    }
}


