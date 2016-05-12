#include "DesiredItem.h"
#include <algorithm>
#include "../../datastructure/ItemErrorInfo.h"
#include "../libServerFrame/QueryManager.h"
#include "../../datastructure/LogicInitData.h"

extern CQueryManager* g_pQueryMgr;

DesiredItem::DesiredItem()
    : m_nMaxWishItemCount(120)
{

}

DesiredItem::~DesiredItem()
{

}

void DesiredItem::Initialize(const std::list<itemtype_t> & listDesire)
{
    m_nMaxWishItemCount = CLogicInitData::Instance().GetDesireMaxSize();
    m_listDesiredItem = listDesire;
}

bool DesiredItem::IsExist(itemtype_t nItemType)
{
    if (m_listDesiredItem.end() != find(m_listDesiredItem.begin(), m_listDesiredItem.end(), nItemType))
    {
        return  true;
    }

    return false;
}

bool DesiredItem::IsFull()
{
    if (m_listDesiredItem.size() >= (size_t)m_nMaxWishItemCount)
    {
        return true;
    }

    return false;
}

int DesiredItem::AddDesiredItem(itemtype_t nItemType)
{
    if (IsFull())
    {
        return EItemUse_ColumnFull;
    }

    if (IsExist(nItemType))
    {
        return EItemUse_UniqueExist;
    }

    m_listDesiredItem.push_back(nItemType);
    return EItemError_Success;
}

bool DesiredItem::RemoveDesiredItem(itemtype_t nItemType)
{
    std::list<itemtype_t>::iterator it = find(m_listDesiredItem.begin(), m_listDesiredItem.end(), nItemType);
    if (it == m_listDesiredItem.end())
    {
        return false;
    }

    m_listDesiredItem.erase(it);
    return true;
}

void DesiredItem::GetListOfDesiredItem(std::list<itemtype_t> & listItemType) const
{
	listItemType = m_listDesiredItem;
}


