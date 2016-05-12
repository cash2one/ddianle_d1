#include "ItemManager.h"
#include "../../datastructure/datainfor/ItemConfigManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/LogicInitData.h"
#include "../../socket/Log.h"
#include "../../datastructure/ItemBase.h"
#include "../../datastructure/datainfor/ItemType.h"
#include "../util/IDCreater.h"
#include "../../datastructure/ItemErrorInfo.h"


//////////////////////////////////////////////////////////////////////////
class ItemFactory
{
private:
    ItemFactory();
    virtual ~ItemFactory();
    friend class ItemManager;

protected:
    static ItemFactory & Instance();
    ItemBase *CreateItem(ItemConfig *pItemConfig);
    void DestroyItem(ItemBase *pItem);
};

ItemFactory::ItemFactory()
{

}

ItemFactory::~ItemFactory()
{

}

ItemFactory & ItemFactory::Instance()
{
    static ItemFactory g_stcItemFactory;
    return g_stcItemFactory;
}

ItemBase * ItemFactory::CreateItem(ItemConfig *pItemConfig)
{
    if (pItemConfig == NULL)
    {
        return NULL;
    }

    switch(pItemConfig->m_anType[0])
    {
    case eItemClassType_Equip:
        {
            return new ItemEquip(pItemConfig);
        }
    case eItemClassType_Expendable:
        {
            return new ItemBase(pItemConfig);
        }
    default:return NULL;
    }
}

void ItemFactory::DestroyItem(ItemBase *pItem)
{
    if (pItem)
    {
        delete pItem;
        pItem = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////

extern CIDCreater     *  g_pIDCreater;

#define ITEM_BUFF_SIZE 256
ItemManager::ItemManager()
{

}

ItemManager::~ItemManager()
{

}

void ItemManager::Destroy()
{
    m_mapItemType.clear();
    m_mapDueTimeItem.clear();
    m_mapItemCooling.clear();

    std::map<itemid_t,ItemBase*>::iterator it = m_mapItem.begin();
    for ( ; it != m_mapItem.end(); ++it )
    {
        if (it->second != NULL)
        {
            ItemFactory::Instance().DestroyItem(it->second);
            it->second = NULL;
        }
    }
    m_mapItem.clear();
}

void ItemManager::InitializeCoolDown(const std::list<CoolDownData> & listCooling)
{
    std::list<CoolDownData>::const_iterator itCooling = listCooling.begin();
    for (; itCooling != listCooling.end(); ++itCooling)
    {
        UpdateCoolingCD(itCooling->m_nType, itCooling->m_nDuration);
    }
}

void ItemManager::CheckItemDueTime(std::list<ItemBase*> & listDueTimeItem)
{
    unsigned int nCurrentNow = (unsigned int)time(NULL);
    std::multimap<unsigned int,itemid_t>::iterator it = m_mapDueTimeItem.begin();
    for ( ; it != m_mapDueTimeItem.end(); ++it )
    {
        if (nCurrentNow >= it->first)
        {
            GetDueTimeItem(it->first, listDueTimeItem);
            m_mapDueTimeItem.erase(it);
            break;
        }
    }
}

void ItemManager::SerializeToBuff( std::string &rBuff ) const
{
    char szHeadBuff[ITEM_BUFF_SIZE] = { 0 };
    char szItemBuff[ITEM_BUFF_SIZE] = { 0 };
    CParamPool headPool( szHeadBuff, sizeof( szHeadBuff ) );
    CParamPool itemPool( szItemBuff, sizeof( szItemBuff ) );
    unsigned short nItemNumber = 0;
    std::string tmpBuff;

    for ( std::map<itemid_t, ItemBase *>::const_iterator itr = m_mapItem.begin();
        itr != m_mapItem.end(); ++itr )
    {
        ItemBase *pBase = itr->second;
        if ( pBase == NULL )
            continue;

        headPool.ClearData();
        itemPool.ClearData();

        pBase->Encode( &itemPool );
        unsigned int nItemSize = itemPool.GetValidDataSize();

        headPool.AddUInt( pBase->GetType() );
        headPool.AddUShort( (unsigned short)pBase->GetColumn() );
        headPool.AddUShort( (unsigned short)pBase->GetGrid() );
        headPool.AddUChar( (unsigned char)nItemSize );

        tmpBuff.append( headPool.GetData(), headPool.GetValidDataSize() );
        tmpBuff.append( itemPool.GetData(), nItemSize );

        ++nItemNumber;
    }

    headPool.ClearData();
    headPool.AddUShort( nItemNumber );

    rBuff.append( headPool.GetData(), headPool.GetValidDataSize() );
    rBuff.append( tmpBuff );
}

void ItemManager::ProcessWrongDBData(const std::list<DBItem> & listWrongData, std::list<itemtype_t> & listRepeatItem)
{
    // 处理错误物品
    std::list<DBItem>::const_iterator it = listWrongData.begin();
    for ( ; it != listWrongData.end(); ++it )
    {
        ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(it->m_nTypeID);
        if (pItemConfig == NULL)
        {
            continue;
        }

        EItemColumn eColumn = (EItemColumn)it->m_nColumn;
        if (it->m_nColumn != EItemColumn_WishList)
        {
            eColumn = pItemConfig->GetItemColumnByType();
        }
       
 
        DBItem tempDBItem(*it);
        tempDBItem.m_nColumn = (unsigned short)eColumn;
   

        ItemBase * pItemBase = CreateItem(pItemConfig, *it);
        if (pItemBase == NULL)
        {
            continue;
        }

        // check repeat cloth
        if (!CheckUniqueItem(eColumn, tempDBItem))
        {
            listRepeatItem.push_back(tempDBItem.m_nTypeID);
        }

        InternalAddItem(pItemBase);
    }
}

bool ItemManager::CheckUniqueItem(EItemColumn eColumn, const DBItem & dbItemData)
{
    std::list<itemid_t> listItemID;
    SearchItemByType(dbItemData.m_nTypeID, listItemID);

    if (!listItemID.empty())
    {
        return false;
    }

    return true;
}

bool ItemManager::LoadItemData(const DBItem & dbItemData)
{
    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(dbItemData.m_nTypeID);
    if (pItemConfig == NULL)
    {
        return false;
    }

    ItemBase * pItemBase = CreateItem(pItemConfig, dbItemData);
    if (pItemBase == NULL)
    {
        return false;
    }

    InternalAddItem(pItemBase);
    return true;
}

bool ItemManager::CreateFromDB(const std::list<DBItem> & listItem, const std::list<CoolDownData> & listCooling, std::list<itemtype_t> & listRepeatItem)
{
    // 加载DB数据
    std::list<DBItem> listWrongData;
    std::list<DBItem>::const_iterator it = listItem.begin();
    for ( ; it != listItem.end(); ++it )
    {
        if (!CheckUniqueItem((EItemColumn)it->m_nColumn, *it))
        {
            listRepeatItem.push_back(it->m_nTypeID);
        }

        if (!LoadItemData(*it))
        {
            listWrongData.push_back(*it);
        }
    }

    // 处理错误数据
    ProcessWrongDBData(listWrongData, listRepeatItem);

    // 初始化冷却信息
    std::list<CoolDownData>::const_iterator itCooling = listCooling.begin();
    for ( ; itCooling != listCooling.end(); ++itCooling )
    {
        UpdateCoolingCD(itCooling->m_nType, itCooling->m_nDuration);
    }

    return true;
}

void ItemManager::PacketToDB(std::list<DBItem> & listItem) const
{
    listItem.clear();
    std::map<itemid_t,ItemBase*>::const_iterator itItem = m_mapItem.begin();
    for (; itItem != m_mapItem.end(); ++itItem)
    {
        ItemBase * pItemBase = itItem->second;
        if (pItemBase != NULL && pItemBase->IsEquip() && ((ItemEquip*)pItemBase)->IsStautsChange())
        {
            DBItem itemInfo;
			itemInfo.Initialize(pItemBase->GetItemID(), pItemBase->GetType(), pItemBase->GetColumn(), pItemBase->GetGrid(), pItemBase->GetCount(), pItemBase->GetLeftUseTimes(), pItemBase->GetDueTime(), 
                        pItemBase->GetDynamicBoxRelatedValue(), ((ItemEquip*)pItemBase)->IsEquiped());
            listItem.push_back(itemInfo);
        }
    }
}

void ItemManager::PacketToCache( std::list<DBItem> &rItem, std::map<itemid_t, unsigned int> &rClothEffect, std::list<CoolDownData> &rCoolDown ) const
{
    for ( std::map<itemid_t, ItemBase *>::const_iterator itemItr = m_mapItem.begin();
        itemItr != m_mapItem.end(); ++itemItr )
    {
        ItemBase *pItemBase = itemItr->second;
        if ( pItemBase == NULL || !pItemBase->IsValid() )
            continue;

        DBItem dbItem;
        pItemBase->SaveToDBItem( dbItem );
        rItem.push_back( dbItem );

        if ( !pItemBase->IsCanAddEffectCloth() )
            continue;

        unsigned int nEffectID = ((ItemEquip *)pItemBase)->GetEffectID();
        if ( nEffectID == 0 )
            continue;

        rClothEffect.insert( std::make_pair( itemItr->first, nEffectID ) );
    }

    for ( std::map<unsigned short, unsigned int>::const_iterator coolItr = m_mapItemCooling.begin();
        coolItr != m_mapItemCooling.end(); ++coolItr )
    {
        rCoolDown.push_back( CoolDownData( coolItr->first, coolItr->second ) );
    }
}

void ItemManager::ClearAllInCoolingItem()
{
    m_mapItemCooling.clear();
}

void ItemManager::UpdateItemCount(ItemBase * pItemBase, int nItemCount)
{
    if (pItemBase == NULL)
    {
        return;
    }

    pItemBase->SetCount((unsigned short)nItemCount);
}

int ItemManager::BatchUpdateItemCount(ItemBase * pItemBase, int nItemCount)
{
    if (pItemBase == NULL)
    {
        return EItemError_CannotUse;
    }

    int nResult = pItemBase->OnBatchUse((unsigned short)nItemCount);

    if (nResult == EItemError_Success)
    {
        //AddChangeGrid(pItemBase->GetColumn(), pItemBase->GetGrid());
    }

    return nResult;
}

void ItemManager::UpdateItemDueTime(int nOldDueTime, int nNewDueTime, itemid_t nItemID)
{
    if (nOldDueTime > 0 )
    {
        std::multimap<unsigned int,itemid_t>::iterator itLower = m_mapDueTimeItem.lower_bound(nOldDueTime);
        std::multimap<unsigned int,itemid_t>::iterator itUpper = m_mapDueTimeItem.upper_bound(nOldDueTime);
        std::multimap<unsigned int,itemid_t>::iterator itDueTime = itLower;
        for ( ; itDueTime != itUpper; ++itDueTime )
        {
            if (itDueTime->second == nItemID)
            {
                m_mapDueTimeItem.erase(itDueTime);
                break;
            }
        }
    }

    if (nNewDueTime > 0)
    {
        InsertInDueTimeManager(nNewDueTime, nItemID);

        ItemBase * pItemBase = GetItemByID(nItemID);
        if (pItemBase != NULL)
        {
           // AddChangeGrid(pItemBase->GetColumn(), pItemBase->GetGrid());
        }
    }
}

void ItemManager::UpdateCoolingCD(unsigned short nCoolingFlag, unsigned int nCoolingTime)
{
    m_mapItemCooling[nCoolingFlag] = nCoolingTime;
}

void ItemManager::GetItemCoolingData(std::list<CoolDownData> & coolingData)
{
    unsigned int nNowTime = (unsigned int)time(NULL);
    std::map<unsigned short,unsigned int>::const_iterator it = m_mapItemCooling.begin();
    for (; it != m_mapItemCooling.end(); ++it)
    {
        CoolDownData data;
        data.m_nType = it->first;
        data.m_nDuration = it->second - nNowTime;

        if (data.m_nDuration > 0)
        {
            coolingData.push_back(data);
        }
    }
}

void ItemManager::InternalAddItem(ItemBase * pItemBase)
{
    // 添加到物品管理器
    InsertInItemManager(pItemBase);
    
    // 添加到类型查找管理器
    InsertInTypeManager(pItemBase->GetType(), pItemBase->GetItemID());

	// 添加到过期管理器
	InsertInDueTimeManager(pItemBase->GetDueTime(), pItemBase->GetItemID());
}

void ItemManager::InsertInItemManager(ItemBase* pItemBase)
{
    m_mapItem.insert(std::make_pair(pItemBase->GetItemID(), pItemBase));
}

void ItemManager::InsertInTypeManager(itemtype_t nItemType, itemid_t nItemID)
{
    m_mapItemType.insert(std::make_pair(nItemType, nItemID));
}

void ItemManager::InsertInDueTimeManager(int nDueTime, itemid_t nItemID)
{
    if (nDueTime > 0)
    {
        m_mapDueTimeItem.insert(std::make_pair(nDueTime, nItemID));
    }
}

ItemBase * ItemManager::GetItemByType(itemtype_t nItemType)
{
    std::list<itemid_t> listItemID;
    SearchItemByType(nItemType, listItemID);

    std::list<itemid_t>::iterator it = listItemID.begin();
	if (it != listItemID.end())
	{
		return GetItemByID(*it);
	}

    return NULL;
}

ItemBase * ItemManager::GetItemByID(itemid_t nItemID) const
{
    std::map<itemid_t,ItemBase*>::const_iterator it = m_mapItem.find(nItemID);
    if (it != m_mapItem.end())
    {
        return it->second;
    }

    return NULL;
}

void ItemManager::SearchItemByType(itemtype_t nItemType, std::list<itemid_t> & listItemID)
{
	std::multimap<itemtype_t, itemid_t>::const_iterator itTypeLower = m_mapItemType.lower_bound(nItemType);
	std::multimap<itemtype_t, itemid_t>::const_iterator itTypeUpper = m_mapItemType.upper_bound(nItemType);
	std::multimap<itemtype_t, itemid_t>::const_iterator itType = itTypeLower;
	for (; itType != itTypeUpper; ++itType)
	{
		listItemID.push_back(itType->second);
	}
}

bool ItemManager::IsInCooling(unsigned short nCoolingFlag)
{
    std::map<unsigned short,unsigned int>::iterator it = m_mapItemCooling.find(nCoolingFlag);
    if ( it != m_mapItemCooling.end())
    {
        unsigned int nTimeNow = (unsigned int)time(NULL);
        if (nTimeNow >= it->second)
        {
            m_mapItemCooling.erase(it);
            return  false;
        }
        return true;
    }

    return false;
}

ItemBase * ItemManager::AddItem(ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID, int nDynamicBoxRelatedValue)
{
    if (pItemConfig == NULL || nCount <= 0)
    {
        return NULL;
    }

    // 创建物品
    ItemBase * pItemBase = CreateItem1(pItemConfig, nCount, nDueTime, nItemID, nDynamicBoxRelatedValue);
    if(pItemBase != NULL)
    {
        // 放入背包
        InternalAddItem(pItemBase);
    }

    return pItemBase;
}

void ItemManager::RemoveItem(itemid_t nItemID)
{
    ItemBase * pItemBase = GetItemByID(nItemID);
    if (pItemBase == NULL)
    {
        return;
    }
    
    itemtype_t nItemType = pItemBase->GetType();
    unsigned int nDueTime = pItemBase->GetDueTime();

    // 去除物品管理器
    RemoveFromItemManager(nItemID);
    
    // 去除过期时间
    RemoveFromDueTimeManager(nDueTime, nItemID);

    // 去除type- > 位置对应
    RemoveFromTypeManager(nItemType, nItemID);
}

void ItemManager::RemoveFromItemManager(itemid_t nItemID)
{
    std::map<itemid_t,ItemBase*>::iterator it = m_mapItem.find(nItemID);
    if (it != m_mapItem.end())
    {
        ItemFactory::Instance().DestroyItem(it->second);
        it->second = NULL;

        m_mapItem.erase(it);
    }
}

void ItemManager::RemoveFromDueTimeManager(unsigned int nDueTime, itemid_t nItemID)
{
    if (nDueTime > 0)
    {
        std::multimap<unsigned int,itemid_t>::iterator itLower = m_mapDueTimeItem.lower_bound(nDueTime);
        std::multimap<unsigned int,itemid_t>::iterator itUpper = m_mapDueTimeItem.upper_bound(nDueTime);
        std::multimap<unsigned int,itemid_t>::iterator itDueTime = itLower;
        for ( ; itDueTime != itUpper; ++itDueTime )
        {
            if (itDueTime->second == nItemID)
            {
                m_mapDueTimeItem.erase(itDueTime);
                break;
            }
        }
    }
}

void ItemManager::RemoveFromTypeManager(itemtype_t nItemType, itemid_t nItemID)
{
    std::multimap<itemtype_t,itemid_t>::iterator itTypeLower = m_mapItemType.lower_bound(nItemType);
    std::multimap<itemtype_t,itemid_t>::iterator itTypeUpper = m_mapItemType.upper_bound(nItemType);
    std::multimap<itemtype_t,itemid_t>::iterator itType = itTypeLower;
    for ( ; itType != itTypeUpper; ++itType )
    {
        if (itType->second == nItemID)
        {
            m_mapItemType.erase(itType);
            break;
        }
    }
}

void ItemManager::GetDueTimeItem(int nDueTime, std::list<ItemBase*> & listDueTimeItem)
{
    std::multimap<unsigned int,itemid_t>::iterator itLower = m_mapDueTimeItem.lower_bound(nDueTime);
    std::multimap<unsigned int,itemid_t>::iterator itUpper = m_mapDueTimeItem.upper_bound(nDueTime);
    std::multimap<unsigned int,itemid_t>::iterator itDueTime = itLower;
    for ( ; itDueTime != itUpper; ++itDueTime )
    {
        ItemBase * pItemBase = GetItemByID(itDueTime->second);
        if (pItemBase != NULL)
        {
            listDueTimeItem.push_back(pItemBase);
        }
    }
}

ItemBase * ItemManager::CreateItem(ItemConfig * pItemConfig, const DBItem & dbItemData)
{
    ItemBase * pItemBase = ItemFactory::Instance().CreateItem(pItemConfig);
    if (pItemBase != NULL)
    {
        pItemBase->InitFromDBItem( dbItemData );
    }

    return pItemBase;
}

ItemBase * ItemManager::CreateItem(EItemColumn eColumn, ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID, int nDynamicBoxRelatedValue)
{
    // 创建物品
    ItemBase * pItemBase = ItemFactory::Instance().CreateItem(pItemConfig);
    if(pItemBase != NULL)
    {
        // 设置装备数目
        pItemBase->SetCount((unsigned short)nCount);
        pItemBase->SetLeftUseTimes((unsigned short)pItemBase->GetUseTimesConfig());

        // 设置过期时间
        pItemBase->SetDueTime(nDueTime);

        // 设置物品ID
        if (nItemID == 0)
        {
            pItemBase->SetItemID(g_pIDCreater->GetID64());
        }
        else
        {
            pItemBase->SetItemID(nItemID);
        }

        // 设置动态礼盒变量值
        pItemBase->SetDynamicBoxRelatedValue(nDynamicBoxRelatedValue);
    }

    return pItemBase;
}

ItemBase * ItemManager::CreateItem1(ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID, int nDynamicBoxRelatedValue)
{
    // 创建物品
    ItemBase * pItemBase = ItemFactory::Instance().CreateItem(pItemConfig);
    if(pItemBase != NULL)
    {
        // 设置装备数目
        pItemBase->SetCount((unsigned short)nCount);
        pItemBase->SetLeftUseTimes((unsigned short)pItemBase->GetUseTimesConfig());

        // 设置过期时间
        pItemBase->SetDueTime(nDueTime);

        // 设置物品ID
        if (nItemID == 0)
        {
            pItemBase->SetItemID(g_pIDCreater->GetID64());
        }
        else
        {
            pItemBase->SetItemID(nItemID);
        }

        // 设置动态礼盒变量值
        pItemBase->SetDynamicBoxRelatedValue(nDynamicBoxRelatedValue);
    }

    return pItemBase;
}

