#include "Bag.h"
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
    friend class ItemBag;

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
            return new EquipItem(pItemConfig);
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

ItemColumn::ItemColumn()
    :m_nEmptySlotCount(0)
    ,m_nMaxOpenSlot(0)
    ,m_nFirstEmptySlot(0)
{

}

ItemColumn::~ItemColumn()
{

}

bool ItemColumn::IsFull()
{
    return m_nEmptySlotCount <= 0;
}

void ItemColumn::Initialize(int nMaxSlotCount, int nMaxOpenSlot)
{
    if (nMaxSlotCount <= 0 || nMaxOpenSlot > nMaxSlotCount)
    {
        return;
    }

    m_vecItemID.clear();
    m_vecItemID.resize(nMaxSlotCount);
    m_nMaxOpenSlot = nMaxOpenSlot;
    m_nEmptySlotCount = nMaxOpenSlot;
}

int ItemColumn::GetFirstEmptySlot()
{
    return m_nFirstEmptySlot;
}

int ItemColumn::GetEmptySlotCount() const
{
    return m_nEmptySlotCount;
}

void ItemColumn::AddMaxOpenSlot(int nAddCount)
{
    if (nAddCount <= 0)
    {
        return;
    }

    if (m_nMaxOpenSlot + nAddCount > (int)m_vecItemID.size())
    {
        return;
    }

    m_nMaxOpenSlot += nAddCount;
    m_nEmptySlotCount += nAddCount;
}

unsigned int ItemColumn::GetItemCount()
{
    return m_nMaxOpenSlot - m_nEmptySlotCount;
}

int ItemColumn::AddItem(int nSlot, itemid_t nItemID)
{
    if (IsFull())
    {
        return EItemUse_ColumnFull;
    }

    if (ExistItem(nSlot))
    {
        return EItemUse_EquipPos;
    }

    UpdateSlotItemID(nItemID, nSlot);
    DeleteEmptySlotCount();
    UpdateFirstEmptySlot(nSlot+1);

    return EItemError_Success;
}

itemid_t ItemColumn::GetItemID(int nSlot)
{
    if (!IsValidSlot(nSlot))
    {
        return 0;
    }

    return m_vecItemID[nSlot];
}

bool ItemColumn::IsValidSlot(int nSlot)
{
    if (nSlot >= m_nMaxOpenSlot || nSlot >= (int)m_vecItemID.size())
    {
        return false;
    }

    return true;
}

void ItemColumn::Clear()
{
    m_vecItemID.clear();
    m_nMaxOpenSlot = 0;
    m_nEmptySlotCount = 0;
    m_nFirstEmptySlot = 0;
}

bool ItemColumn::ExchangeItem(int nFirstSlot, int nSecondSlot)
{
    if (!IsValidSlot(nFirstSlot) || !IsValidSlot(nSecondSlot))
    {
        return false;
    }

    if (!ExistItem(nFirstSlot) || !ExistItem(nSecondSlot))
    {
        return false;
    }

    itemid_t nFirstItemID = m_vecItemID[nFirstSlot];
    m_vecItemID[nFirstSlot] = m_vecItemID[nSecondSlot];
    m_vecItemID[nSecondSlot] = nFirstItemID;

    return true;
}

bool ItemColumn::ExistItem(int nSlot)
{
    if (!IsValidSlot(nSlot))
    {
        return true;
    }

    return m_vecItemID[nSlot] > 0;
}

void ItemColumn::UpdateSlotItemID(itemid_t nItemID, int nSlot)
{
    m_vecItemID[nSlot] = nItemID;
}

void ItemColumn::UpdateFirstEmptySlot(int nNewEmptySlot)
{
    if (nNewEmptySlot == m_nFirstEmptySlot)
    {
        return;
    }

    bool bNewExistItem = ExistItem(nNewEmptySlot);
    bool bOldExistItem = ExistItem(m_nFirstEmptySlot);
    int nOldEmptySlot = m_nFirstEmptySlot;

    if (bNewExistItem)
    {
        if (bOldExistItem)
        {
            m_nFirstEmptySlot = SearchFirstEmptySlot();
        }
    }
    else
    {
        if (bOldExistItem)
        {
            m_nFirstEmptySlot = nNewEmptySlot;
        }
        else
        {
            if (nNewEmptySlot < nOldEmptySlot)
            {
                m_nFirstEmptySlot = nNewEmptySlot;
            }
        }
    }
}

int ItemColumn::SearchFirstEmptySlot()
{
    for (size_t i = 0 ; i < m_vecItemID.size(); ++i)
    {
        if (!ExistItem(i))
        {
            return (int)i;
        }
    }

    return m_nMaxOpenSlot;
}

void ItemColumn::AddEmptySlotCount()
{
    m_nEmptySlotCount++;
}

void ItemColumn::DeleteEmptySlotCount()
{
    m_nEmptySlotCount--;
}

void ItemColumn::RemoveItem(int nSlot)
{
    if (!IsValidSlot(nSlot))
    {
        return;
    }

    UpdateSlotItemID(0, nSlot);
    AddEmptySlotCount();
    UpdateFirstEmptySlot(nSlot);
}

int ItemColumn::GetMaxOpenIndex() const
{
    return m_nMaxOpenSlot;
}


ItemBag::ItemBag()
    : m_bCollecttingGridChange(false)
{

}

ItemBag::~ItemBag()
{

}

void ItemBag::InitializeBag(EItemColumn eColumn, int nMaxSolotCount, int nMaxOpenSlot)
{
    if (IsValidColumn(eColumn))
    {
        m_ItemBag[eColumn].Initialize(nMaxSolotCount, nMaxOpenSlot);
    }
}

void ItemBag::Destroy()
{
    for (int i = EItemColumn_ClothBag; i < EItemColumn_Max; ++i)
    {
        m_ItemBag[(EItemColumn)i].Clear();
    }

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

void ItemBag::CheckItemDueTime(std::list<ItemBase*> & listDueTimeItem)
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

void ItemBag::SerializeComponent(CParamPool &IOBuff)
{
    int nOldPos = IOBuff.GetCurPosition();
    IOBuff.AddUShort(0);

    char szItemBuff[ITEM_BUFF_SIZE] = {0};
    unsigned short nItemNumber = 0;

    for (unsigned short i = 0; i < EItemColumn_Max; i++)
    {
        // 身上衣服栏位的衣服保存在服饰饰品栏
        if (i == EItemColumn_ClothOnPlayer)
        {
            continue;
        }
            
        unsigned int nOpenNum = m_ItemBag[i].GetMaxOpenIndex();
        for (unsigned int j = 0; j < nOpenNum; ++j)
        {
            ItemBase * pItemBase = GetItemBySlot((EItemColumn)i, j);
            if (pItemBase != NULL)
            {
                IOBuff.AddUInt(pItemBase->GetType());
                IOBuff.AddUShort(i);
                IOBuff.AddUShort((unsigned short)j);

                memset(szItemBuff, 0, sizeof(szItemBuff));
                CParamPool itemBuff(szItemBuff, sizeof(szItemBuff));
                pItemBase->Encode(&itemBuff);
                unsigned int nItemSize = itemBuff.GetValidDataSize();
                ASSERT(nItemSize == itemBuff.GetCurPosition());

                IOBuff.AddUChar((unsigned char)nItemSize);
                IOBuff.AddMemory(itemBuff.GetData(), (unsigned short)nItemSize);

                nItemNumber++;
            }
        }
    }

    int nFinalPos = IOBuff.GetCurPosition();
    IOBuff.SetCurPosition(nOldPos );
    IOBuff.ReplaceShort(nItemNumber);
    IOBuff.SetCurPosition(nFinalPos);
}

void ItemBag::ProcessWrongDBData(const std::list<DBItem> & listWrongData, std::list<itemtype_t> & listRepeatItem)
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
       
        if (!IsValidColumn(eColumn) || IsColumnFull(eColumn))
        {
            continue;
        }

        DBItem tempDBItem(*it);
        tempDBItem.m_nColumn = (unsigned short)eColumn;
        if (ExistItem(eColumn, tempDBItem.m_nIndex))
        {
            tempDBItem.m_nIndex = (unsigned short)(m_ItemBag[eColumn].GetFirstEmptySlot());
        }

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

        InternalAddItem(eColumn, tempDBItem.m_nIndex, pItemBase);
    }
}

bool ItemBag::CheckUniqueItem(EItemColumn eColumn, const DBItem & dbItemData)
{
    std::list<itemid_t> listItemID;
    SearchItemByType(eColumn, dbItemData.m_nTypeID, listItemID);

    if (!listItemID.empty())
    {
        return false;
    }

    return true;
}

bool ItemBag::LoadItemData(const DBItem & dbItemData)
{
    // 数据库数据不存在下面栏位数据
    if (dbItemData.m_nColumn == EItemColumn_ClothOnPlayer || dbItemData.m_nColumn == EItemColumn_TransformClothOnPlayer)
    {
        return false;
    }

    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(dbItemData.m_nTypeID);
    if (pItemConfig == NULL)
    {
        return false;
    }

    if (!IsValidGrid((EItemColumn)dbItemData.m_nColumn, dbItemData.m_nIndex))
    {
        return false;
    }

    if (ExistItem((EItemColumn)dbItemData.m_nColumn, dbItemData.m_nIndex))
    {
        return false;
    }

    bool bColumnFalse = (dbItemData.m_nColumn != EItemColumn_TransformClothOnPlayer && dbItemData.m_nColumn != EItemColumn_WishList && dbItemData.m_nColumn != EItemColumn_BadgeOnPlayer  && dbItemData.m_nColumn != EItemColumn_WeddingRingOnPlayer && dbItemData.m_nColumn != pItemConfig->GetItemColumnByType()) || 
        (dbItemData.m_nColumn == EItemColumn_BadgeOnPlayer && !pItemConfig->IsBadge());
    if(bColumnFalse)
    {
        return true;
    }

    ItemBase * pItemBase = CreateItem(pItemConfig, dbItemData);
    if (pItemBase == NULL)
    {
        return false;
    }

    if (pItemBase->GetColumn() == EItemColumn_BadgeOnPlayer || pItemBase->GetColumn() == EItemColumn_WeddingRingOnPlayer)
    {
        if (!((EquipItem*)pItemBase)->HasEquiped())
        {
            ((EquipItem*)pItemBase)->OnEquip();
        }
    }

    InternalAddItem((EItemColumn)dbItemData.m_nColumn, dbItemData.m_nIndex, pItemBase);
    return true;
}

bool ItemBag::CreateFromDB(const std::list<DBItem> & listItem, const std::list<CoolDownData> & listCooling, std::list<itemtype_t> & listRepeatItem)
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

void ItemBag::PacketToDB(std::list<DBItem> & listItem, std::list<CoolDownData> & listCooling) const
{
    listItem.clear();
    std::map<itemid_t,ItemBase*>::const_iterator itItem = m_mapItem.begin();
    for (; itItem != m_mapItem.end(); ++itItem)
    {
        ItemBase * pItemBase = itItem->second;
        if (pItemBase != NULL && pItemBase->IsEquip())
        {
            EquipItem *pEquip = (EquipItem *)pItemBase;
            if (!pEquip->IsEquipStatusChanged() || CLogicInitData::Instance().IsDefaultColth(pEquip->GetType()))
                continue;

            pEquip->ResetLoginEquipStatus();

            DBItem itemInfo;
            itemInfo.m_nItemID = pItemBase->GetItemID();
            itemInfo.m_nTypeID = pItemBase->GetType();
            itemInfo.m_nIndex = (unsigned short)pItemBase->GetGrid();
            itemInfo.m_nColumn = (unsigned short)pItemBase->GetColumn();
            itemInfo.m_nCount = pItemBase->GetCount();
            itemInfo.m_nUseTimes = pItemBase->GetLeftUseTimes();
            itemInfo.m_nDuration = pItemBase->GetDueTime();
            itemInfo.m_nIsEquiped = pEquip->HasEquiped();

            listItem.push_back(itemInfo);
        }
    }

    //cooldown
    /*std::map<unsigned short,unsigned int>::const_iterator it = m_mapItemCooling.begin();
    for (; it != m_mapItemCooling.end(); ++it)
    {
        listCooling.push_back(CoolDownData(it->first, it->second));
    }*/
}

bool ItemBag::IsExist(itemtype_t nItemType, unsigned int nCount)
{
    unsigned int nTotalCount = 0;
    std::list<itemid_t> listItemID;
    SearchItemByType(nItemType, listItemID);
    for (std::list<itemid_t>::iterator itItemID = listItemID.begin();
        itItemID != listItemID.end(); ++itItemID)
    {
        ItemBase * pItemBase = GetItemByID(*itItemID);
        if (pItemBase == NULL)
        {
            continue;
        }

        nTotalCount += pItemBase->GetCount();
    }

    return nTotalCount >= nCount;
}

bool ItemBag::IsValidGrid(EItemColumn eColumn, int nSlot)
{
    if (!IsValidColumn(eColumn))
    {
        return false;
    }

    return m_ItemBag[eColumn].IsValidSlot(nSlot);
}

void ItemBag::RemoveItem(EItemColumn eColumn, int nSlot)
{
    if (IsValidColumn(eColumn))
    {
        itemid_t nItemID = m_ItemBag[eColumn].GetItemID(nSlot);
        RemoveItem(nItemID);
    }
}

void ItemBag::ClearAllInCoolingItem()
{
    m_mapItemCooling.clear();
}

void ItemBag::UpdateItemCount(ItemBase * pItemBase, int nItemCount)
{
    if (pItemBase == NULL)
    {
        return;
    }

    pItemBase->SetCount((unsigned short)nItemCount);

    AddChangeGrid(pItemBase->GetColumn(), pItemBase->GetGrid());
}

int ItemBag::BatchUpdateItemCount(ItemBase * pItemBase, int nItemCount)
{
    if (pItemBase == NULL)
    {
        return EItemError_CannotUse;
    }

    int nResult = pItemBase->OnBatchUse((unsigned short)nItemCount);

    if (nResult == EItemError_Success)
    {
        AddChangeGrid(pItemBase->GetColumn(), pItemBase->GetGrid());
    }

    return nResult;
}

void ItemBag::BeginCollectChangeGrid()
{
    m_listChangeGrid.clear();
    m_bCollecttingGridChange = true;
}

void ItemBag::EndCollectChangeGrid(std::list<itemgrid_t> & listGridChange)
{
    m_bCollecttingGridChange = false;
    listGridChange = m_listChangeGrid;
}

void ItemBag::AddChangeGrid(EItemColumn eColumn, int nSlot)
{
    if (m_bCollecttingGridChange)
    {
        itemgrid_t grid(eColumn, nSlot);
        if (m_listChangeGrid.end() == find(m_listChangeGrid.begin(), m_listChangeGrid.end(), grid))
        {
            m_listChangeGrid.push_back(grid);
        }
    }
}

void ItemBag::UpdateItemDueTime(int nOldDueTime, int nNewDueTime, itemid_t nItemID)
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
            AddChangeGrid(pItemBase->GetColumn(), pItemBase->GetGrid());
        }
    }
}

void ItemBag::AddColumnMaxOpenSlot(EItemColumn eColumn, int nAddSlot)
{
    if (!IsValidColumn(eColumn))
    {
        return;
    }

    m_ItemBag[eColumn].AddMaxOpenSlot(nAddSlot);
}

void ItemBag::UpdateCoolingCD(unsigned short nCoolingFlag, unsigned int nCoolingTime)
{
    m_mapItemCooling[nCoolingFlag] = nCoolingTime;
}

void ItemBag::GetItemCoolingData(std::list<CoolDownData> & coolingData)
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

void ItemBag::InternalAddItem(EItemColumn eColumn, int nSlot, ItemBase * pItemBase)
{
    // 添加到物品管理器
    InsertInItemManager(pItemBase);
    
    // 添加到类型查找管理器
    InsertInTypeManager(pItemBase->GetType(), pItemBase->GetItemID());

    // 添加到背包
    pItemBase->SetGrid(eColumn, nSlot);
    InsertInBagManager(eColumn, nSlot, pItemBase->GetItemID());

    // 添加到过期管理器
    if (pItemBase->GetColumn() != EItemColumn_TransformClothOnPlayer && pItemBase->GetColumn() != EItemColumn_WishList)
    {
        InsertInDueTimeManager(pItemBase->GetDueTime(), pItemBase->GetItemID());
    }
}

void ItemBag::InsertInItemManager(ItemBase* pItemBase)
{
    m_mapItem.insert(std::make_pair(pItemBase->GetItemID(), pItemBase));
}

void ItemBag::InsertInTypeManager(itemtype_t nItemType, itemid_t nItemID)
{
    m_mapItemType.insert(std::make_pair(nItemType, nItemID));
}

void ItemBag::InsertInBagManager(EItemColumn eColumn, int nSlot, itemid_t nItemID)
{
    m_ItemBag[eColumn].AddItem(nSlot, nItemID);
}

void ItemBag::InsertInDueTimeManager(unsigned int nDueTime, itemid_t nItemID)
{
    if (nDueTime > 0)
    {
        m_mapDueTimeItem.insert(std::make_pair(nDueTime, nItemID));
    }
}

ItemBase * ItemBag::GetItemBySlot(EItemColumn eColumn, int nSlot)
{
    if (IsValidColumn(eColumn))
    {
        itemid_t nItemID = m_ItemBag[eColumn].GetItemID(nSlot);
        return GetItemByID(nItemID);
    }

    return NULL;
}

ItemBase * ItemBag::GetItemByType(EItemColumn eColumn, itemtype_t nItemType)
{
    std::list<itemid_t> listItemID;
    SearchItemByType(nItemType, listItemID);

    std::list<itemid_t>::iterator it = listItemID.begin();
    for (; it != listItemID.end(); ++it)
    {
        ItemBase * pItemBase = GetItemByID(*it);
        if (pItemBase != NULL && pItemBase->GetColumn() == eColumn)
        {
            return pItemBase;
        }
    }

    return NULL;
}

void ItemBag::SearchItemByType(itemtype_t nItemType, std::list<itemid_t> & listItemID, EItemColumn eExcludeColumn)
{
    std::multimap<itemtype_t,itemid_t>::const_iterator itTypeLower = m_mapItemType.lower_bound(nItemType);
    std::multimap<itemtype_t,itemid_t>::const_iterator itTypeUpper = m_mapItemType.upper_bound(nItemType);
    std::multimap<itemtype_t,itemid_t>::const_iterator itType = itTypeLower;
    for ( ; itType != itTypeUpper; ++itType )
    {
        ItemBase * pItemBase = GetItemByID(itType->second);
        if (pItemBase != NULL)
        {
            if (eExcludeColumn != EItemColumn_Invalid && pItemBase->GetColumn() != eExcludeColumn)
            {
                listItemID.push_back(itType->second);
            }
            else if (eExcludeColumn == EItemColumn_Invalid)
            {
                listItemID.push_back(itType->second);
            }
        }
    }
}

void ItemBag::SearchItemByType(EItemColumn eColumn, itemtype_t nItemType, std::list<itemid_t> & listItemID)
{
    std::list<itemid_t> listTempItemID;
    SearchItemByType(nItemType, listTempItemID);

    for (std::list<itemid_t>::iterator itItemID = listTempItemID.begin();
        itItemID != listTempItemID.end(); ++itItemID)
    {
        ItemBase * pItemBase = GetItemByID(*itItemID);
        if (pItemBase != NULL && pItemBase->GetColumn() == eColumn)
        {
            listItemID.push_back(*itItemID);
        }
    }
}

void ItemBag::SearchItem(EItemColumn eColumn, std::list<itemid_t> & listItemID)
{
   if (!IsValidColumn(eColumn))
   {
       return;
   }

   int nMaxSlot = m_ItemBag[eColumn].GetMaxOpenIndex();
   for (int i = 0; i < nMaxSlot; ++i)
   {
       if (m_ItemBag[eColumn].ExistItem(i))
       {
           listItemID.push_back(m_ItemBag[eColumn].GetItemID(i));
       }
   }
}

ItemBase * ItemBag::GetItemByID(itemid_t nItemID)
{
    std::map<itemid_t,ItemBase*>::iterator it = m_mapItem.find(nItemID);
    if (it != m_mapItem.end())
    {
        return it->second;
    }

    return NULL;
}

bool ItemBag::IsInCooling(unsigned short nCoolingFlag)
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

unsigned int ItemBag::GetColumnSlotNum(EItemColumn eColumn)
{
    if (!IsValidColumn(eColumn))
    {
        return 0;
    }

    return m_ItemBag[eColumn].GetMaxOpenIndex();
}

unsigned int ItemBag::GetColumnEmptyNum(EItemColumn eColumn)
{
    if (!IsValidColumn(eColumn))
    {
        return 0;
    }

    return m_ItemBag[eColumn].GetEmptySlotCount();
}

bool ItemBag::IsColumnFull(EItemColumn eColumn)
{
    if (!IsValidColumn(eColumn))
    {
        return false;
    }

    return m_ItemBag[eColumn].IsFull();
}

int ItemBag::GetColumnFirstEmptySlot(EItemColumn eColumn)
{
    if (!IsValidColumn(eColumn))
    {
        return -1;
    }

    return m_ItemBag[eColumn].GetFirstEmptySlot();
}

ItemBase * ItemBag::AddItem(EItemColumn eColumn, ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID, int nDynamicBoxRelatedValue)
{
    if (!IsValidColumn(eColumn) || pItemConfig == NULL || nCount <= 0 || IsColumnFull(eColumn))
    {
        return NULL;
    }

    // 创建物品
    ItemBase * pItemBase = CreateItem(eColumn, pItemConfig, nCount, nDueTime, nItemID, nDynamicBoxRelatedValue);
    if(pItemBase != NULL)
    {
        // 放入背包
        InternalAddItem(eColumn, pItemBase->GetGrid(), pItemBase);

        AddChangeGrid(eColumn, pItemBase->GetGrid());
    }

    return pItemBase;
}

unsigned int ItemBag::GetColumnItemCount(EItemColumn eColumn)
{
    if (IsValidColumn(eColumn))
    {
        return m_ItemBag[eColumn].GetItemCount();
    }

    return 0;
}

unsigned int ItemBag::GetColumnPutInMaxCount(EItemColumn eColumn, itemtype_t nItemType)
{
    if (!IsValidColumn(eColumn))
    {
        return 0;
    }

    unsigned int nCanAddCount = 0;
    std::list<itemid_t> listItemID;
    SearchItemByType(nItemType, listItemID);
    for (std::list<itemid_t>::iterator itItemID = listItemID.begin();
        itItemID != listItemID.end(); ++itItemID)
    {
        ItemBase * pItemBase = GetItemByID(*itItemID);
        if (pItemBase != NULL && pItemBase->GetColumn() == eColumn)
        {
            nCanAddCount += pItemBase->GetLeftStackCount();
        }
    }

    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
    if (pItemConfig != NULL)
    {
        nCanAddCount += pItemConfig->m_nMaxStackCount*m_ItemBag[eColumn].GetEmptySlotCount();
    }

    return nCanAddCount;
}

void ItemBag::MoveItem(EItemColumn eFromColumn, int nFromSlot, EItemColumn eToColumn, int nToSlot)
{
    if (eFromColumn == eToColumn && nFromSlot == nToSlot)
    {
        return;
    }
    
    // 检查栏位和格子
    if (!IsValidGrid(eFromColumn, nFromSlot) || !IsValidGrid(eToColumn, nToSlot))
    {
        return;
    }

    // 检查物品
    if (!m_ItemBag[eFromColumn].ExistItem(nFromSlot) || m_ItemBag[eToColumn].ExistItem(nToSlot))
    {
        return;
    }

    // 移动位置
    itemid_t nItemID = m_ItemBag[eFromColumn].GetItemID(nFromSlot);
    m_ItemBag[eToColumn].AddItem(nToSlot, nItemID);
    m_ItemBag[eFromColumn].RemoveItem(nFromSlot);

    // 更改物品位置信息
    ItemBase * pItemBase = GetItemByID(nItemID);
    if (pItemBase != NULL)
    {
        pItemBase->SetGrid(eToColumn, nToSlot);
    }
}

void ItemBag::ExchangeItem(ItemBase * pFirstItem, ItemBase * pSecondItem)
{
    if (pFirstItem == NULL || pSecondItem == NULL || pFirstItem == pSecondItem)
    {
        return;
    }

    if(pFirstItem->GetColumn() != pSecondItem->GetColumn())
    {
        return;
    }

    int nFirstSlot = pFirstItem->GetGrid();
    int nSecondSlot = pSecondItem->GetGrid();

    // 交换格子
    if (m_ItemBag[pFirstItem->GetColumn()].ExchangeItem(nFirstSlot, nSecondSlot))
    {
        pFirstItem->SetGrid(pFirstItem->GetColumn(), nSecondSlot);
        pSecondItem->SetGrid(pSecondItem->GetColumn(), nFirstSlot);
    }
}

void ItemBag::RemoveItem(itemid_t nItemID)
{
    ItemBase * pItemBase = GetItemByID(nItemID);
    if (pItemBase == NULL)
    {
        return;
    }
    
    EItemColumn eColumn = pItemBase->GetColumn();
    int nSlot = pItemBase->GetGrid();
    itemtype_t nItemType = pItemBase->GetType();
    unsigned int nDueTime = pItemBase->GetDueTime();

    // 去除物品管理器
    RemoveFromItemManager(nItemID);
    
    // 去除过期时间
    RemoveFromDueTimeManager(nDueTime, nItemID);

    // 去除type- > 位置对应
    RemoveFromTypeManager(nItemType, nItemID);

    // 背包中移除
    RemoveFromBagManager(eColumn, nSlot);
}

void ItemBag::RemoveFromItemManager(itemid_t nItemID)
{
    std::map<itemid_t,ItemBase*>::iterator it = m_mapItem.find(nItemID);
    if (it != m_mapItem.end())
    {
        ItemFactory::Instance().DestroyItem(it->second);
        it->second = NULL;

        m_mapItem.erase(it);
    }
}

void ItemBag::RemoveFromBagManager(EItemColumn eColumn, int nSlot)
{
    m_ItemBag[eColumn].RemoveItem(nSlot);
}

void ItemBag::RemoveFromDueTimeManager(unsigned int nDueTime, itemid_t nItemID)
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

void ItemBag::RemoveFromTypeManager(itemtype_t nItemType, itemid_t nItemID)
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

bool ItemBag::IsValidColumn(EItemColumn eColumn)
{
    if (eColumn <= EItemColumn_Invalid || eColumn >= EItemColumn_Max)
    {
        return false;
    }

    return true;
}

bool ItemBag::ExistItem(EItemColumn eColumn, int nSlot)
{
    if (!IsValidColumn(eColumn))
    {
        return false;
    }

    return m_ItemBag[eColumn].ExistItem(nSlot);
}

void ItemBag::GetDueTimeItem(int nDueTime, std::list<ItemBase*> & listDueTimeItem)
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

ItemBase * ItemBag::CreateItem(ItemConfig * pItemConfig, const DBItem & dbItemData)
{
    ItemBase * pItemBase = ItemFactory::Instance().CreateItem(pItemConfig);
    if (pItemBase != NULL)
    {
        pItemBase->Init(dbItemData);
    }

    return pItemBase;
}

ItemBase * ItemBag::CreateItem(EItemColumn eColumn, ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID, int nDynamicBoxRelatedValue)
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
       
        // 设置物品栏位和格子
        int nSlot = m_ItemBag[eColumn].GetFirstEmptySlot();
        pItemBase->SetGrid(eColumn, nSlot);
    }

    return pItemBase;
}

