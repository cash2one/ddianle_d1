#include "MagicArrayConfig.h"



MagicArrayConfig::MagicArrayConfig()
: m_nMagicArrayID(0)
, m_nFreeCallCD(0)
, m_nAccumulatedCount(0)
, m_nCurrencyType(0)
, m_nOnceCallValue(0)
, m_nTenCallValue(0)
, m_nFirstTenCallRateCount(0)
, m_nSecondTenCallRateCount(0)
, m_nRefreshExchangeCType(0)
, m_nRefreshExchangeCValue(0)
, m_nRefreshBeginTime(0)
, m_nRefreshEndTime(0)
{
}

MagicArrayConfig::~MagicArrayConfig()
{}

bool MagicArrayConfig::Load(CFileStream & file)
{
    file.ReadUInt(m_nMagicArrayID);
    file.ReadUInt(m_nFreeCallCD);
    file.ReadUInt(m_nAccumulatedCount);

    unsigned short nCount = 0;
    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        GroupRate grouprate;
        file.ReadUInt(grouprate.nGroupID);
        file.ReadUInt(grouprate.nRate);
        m_listAccumulatedRollRate.push_back(grouprate);
    }

    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        GroupRate grouprate;
        file.ReadUInt(grouprate.nGroupID);
        file.ReadUInt(grouprate.nRate);
        m_listFirstRollRate.push_back(grouprate);
    }

    file.ReadUInt(m_nCurrencyType);
    file.ReadUInt(m_nOnceCallValue);

    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        CItem item;
        file.ReadUInt(item.m_nItemType);
        file.ReadUShort(item.m_nItemCount);
        file.ReadInt(item.m_nValidTime);
        m_listOnceCostItem.push_back(item);
    }

    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        GroupRate grouprate;
        file.ReadUInt(grouprate.nGroupID);
        file.ReadUInt(grouprate.nRate);
        m_listOnceCallRate.push_back(grouprate);
    }

    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        CItem item;
        file.ReadUInt(item.m_nItemType);
        file.ReadUShort(item.m_nItemCount);
        file.ReadInt(item.m_nValidTime);
        m_listOnceFixedItem.push_back(item);
    }

    file.ReadUInt(m_nCurrencyType);
    file.ReadUInt(m_nTenCallValue);

    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        CItem item;
        file.ReadUInt(item.m_nItemType);
        file.ReadUShort(item.m_nItemCount);
        file.ReadInt(item.m_nValidTime);
        m_listTenCostItem.push_back(item);
    }

    file.ReadUInt(m_nFirstTenCallRateCount);
    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        GroupRate grouprate;
        file.ReadUInt(grouprate.nGroupID);
        file.ReadUInt(grouprate.nRate);
        m_listFirstTenCallRate.push_back(grouprate);
    }

    file.ReadUInt(m_nSecondTenCallRateCount);
    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        GroupRate grouprate;
        file.ReadUInt(grouprate.nGroupID);
        file.ReadUInt(grouprate.nRate);
        m_listSecondTenCallRate.push_back(grouprate);
    }

    file.ReadUShort(nCount);
    for (unsigned short i = 0; i < nCount; ++i)
    {
        CItem item;
        file.ReadUInt(item.m_nItemType);
        file.ReadUShort(item.m_nItemCount);
        file.ReadInt(item.m_nValidTime);
        m_listTenFixedItem.push_back(item);
    }

    file.ReadUInt(m_nRefreshExchangeCType);
    file.ReadUInt(m_nRefreshExchangeCValue);

    file.ReadUInt(m_nRefreshBeginTime);
    file.ReadUInt(m_nRefreshEndTime);

    return true;
}

typedef std::pair<unsigned short, unsigned short>   callitem;
void MagicArrayConfig::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_nMagicArrayID);

    std::map<itemtype_t, callitem> costitem;
    std::list<CItem>::iterator it = m_listOnceCostItem.begin();
    for (; it != m_listOnceCostItem.end(); ++it)
    {
        std::map<itemtype_t, callitem>::iterator itr = costitem.find(it->m_nItemType);
        if (itr != costitem.end())
        {
            itr->second.first = it->m_nItemCount;
        }
        else
        {
            callitem temp;
            temp.first = it->m_nItemCount;
            costitem[it->m_nItemType] = temp;
        }
    }

    it = m_listTenCostItem.begin();
    for (; it != m_listTenCostItem.end(); ++it)
    {
        std::map<itemtype_t, callitem>::iterator itr = costitem.find(it->m_nItemType);
        if (itr != costitem.end())
        {
            itr->second.second = it->m_nItemCount;
        }
        else
        {
            callitem temp;
            temp.second = it->m_nItemCount;
            costitem[it->m_nItemType] = temp;
        }
    }

    IOBuff.AddUShort((unsigned short)costitem.size());
    std::map<itemtype_t, callitem>::iterator itr = costitem.begin();
    for (; itr != costitem.end(); ++itr)
    {
        IOBuff.AddUInt(itr->first);
        IOBuff.AddInt(itr->second.first);
        IOBuff.AddInt(itr->second.second);
    }

	IOBuff.AddChar((char)m_nCurrencyType);
	IOBuff.AddInt(m_nOnceCallValue);
	IOBuff.AddInt(m_nTenCallValue);
}

MagicArrayReward::MagicArrayReward()
: m_nMagicArrayID(0)
, m_nGroupID(0)
, m_nSex(0)
, m_nWeight(0)
, m_nCurrencyType(0)
{

}

MagicArrayReward::~MagicArrayReward()
{
}

bool MagicArrayReward::Load(CFileStream & file)
{
	file.ReadUInt(m_nMagicArrayID);
	file.ReadUInt(m_Item.m_nItemType);
	file.ReadUShort(m_Item.m_nItemCount);
	file.ReadInt(m_Item.m_nValidTime);
	file.ReadInt(m_nSex);
	file.ReadBool(m_Item.m_bAnnounce);
	file.ReadInt(m_nWeight);

	return true;
};

MagicArrayPreview::MagicArrayPreview()
: m_nMagicArrayID(0)
, m_nGroupID(0)
, m_nSex(0)
{

}

MagicArrayPreview::~MagicArrayPreview()
{
}

bool MagicArrayPreview::Load(CFileStream & file)
{
    file.ReadUInt(m_nMagicArrayID);
    file.ReadUInt(m_Item.m_nItemType);
    file.ReadUShort(m_Item.m_nItemCount);
    file.ReadInt(m_Item.m_nValidTime);
    file.ReadInt(m_nSex);

    return true;
};

MagicArrayCompensation::MagicArrayCompensation()
: m_nMagicArrayID(0)
, m_nGroupID(0)
, m_nSex(0)
{}

MagicArrayCompensation::~MagicArrayCompensation()
{}

bool MagicArrayCompensation::Load(CFileStream & file)
{
	file.ReadUInt(m_nMagicArrayID);
	file.ReadUInt(m_nCompessationItem.m_nItemType);
	file.ReadUShort(m_nCompessationItem.m_nItemCount);
	file.ReadInt(m_nCompessationItem.m_nValidTime);
	file.ReadInt(m_nSex);

	return true;
};

MagicArrayExchange::MagicArrayExchange()
: m_nExchangeID(0)
, m_nTeamID(0)
, m_nGroupID(0)
, m_nSex(0)
{}

MagicArrayExchange::~MagicArrayExchange()
{}

bool MagicArrayExchange::Load(CFileStream & file)
{
    file.ReadUInt(m_nExchangeID);
	file.ReadUInt(m_nTeamID);
	file.ReadUInt(m_nGroupID);
	file.ReadUInt(m_ExchangeItem.m_nItemType);
	file.ReadUShort(m_ExchangeItem.m_nItemCount);
	file.ReadInt(m_ExchangeItem.m_nValidTime);
	file.ReadInt(m_nSex);
	file.ReadUInt(m_CostItem.m_nItemType);
	file.ReadUShort(m_CostItem.m_nItemCount);

	return true;
};

MagicArrayBaseExchange::MagicArrayBaseExchange()
: m_nExchangeID(0)
, m_nSex(0)

{}

MagicArrayBaseExchange::~MagicArrayBaseExchange()
{}

bool MagicArrayBaseExchange::Load(CFileStream & file)
{
    file.ReadUInt(m_nExchangeID);
    file.ReadUInt(m_ExchangeItem.m_nItemType);
    file.ReadUShort(m_ExchangeItem.m_nItemCount);
    file.ReadInt(m_ExchangeItem.m_nValidTime);
    file.ReadInt(m_nSex);
    file.ReadUInt(m_CostItem.m_nItemType);
    file.ReadUShort(m_CostItem.m_nItemCount);

    return true;
};


