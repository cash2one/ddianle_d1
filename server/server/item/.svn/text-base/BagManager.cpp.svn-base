#include "BagManager.h"
#include "../../datastructure/LogicInitData.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "EntityItemComponent.h"
#include "BagConfigManager.h"

#ifdef WIN32
#pragma warning(disable:4061)
#endif


Bag::Bag(EBagColumn eColumn)
    :m_eColumn(eColumn)
    ,m_nMaxOpenSlot(0)
    ,m_nEmptySlotCount(0)
    ,m_nFirstEmptySlot(0)
{

}

Bag::~Bag()
{

}

bool Bag::IsFull()
{
    return m_nEmptySlotCount <= 0;
}

void Bag::Initialize(int nMaxSlotCount, int nMaxOpenSlot)
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

int Bag::GetFirstEmptySlot()
{
    return m_nFirstEmptySlot;
}

int Bag::GetEmptySlotCount() const
{
    return m_nEmptySlotCount;
}

void Bag::AddMaxOpenSlot(int nAddCount)
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

unsigned int Bag::GetItemCount()
{
    return m_nMaxOpenSlot - m_nEmptySlotCount;
}

int Bag::AddItem(int nSlot, itemid_t nItemID)
{
    if (IsFull())
    {
        return EItemUse_ColumnFull;
    }

	if (!IsValidSlot(nSlot))
	{
		return EItemUse_InvalidTarget;
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

int Bag::AddItem(itemid_t nItemID, int & nSlot)
{
    int nFreeSlot = GetFirstEmptySlot();
    if (nFreeSlot == m_nMaxOpenSlot)
    {
        return EItemUse_ColumnFull;
    }

	nSlot = nFreeSlot;
    return AddItem(nFreeSlot, nItemID);
}

itemid_t Bag::GetItemID(int nSlot)
{
    if (!IsValidSlot(nSlot))
    {
        return 0;
    }

    return m_vecItemID[nSlot];
}

bool Bag::IsValidSlot(int nSlot, bool bWriteLog)
{
    if (nSlot < 0 || nSlot >= m_nMaxOpenSlot || nSlot >= (int)m_vecItemID.size())
    {
        if (bWriteLog)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] Bag:IsValidSlot Column = %d, nSlot = %d, MaxSlot = %d, vecSize = %d", (int)m_eColumn, 
                nSlot, m_nMaxOpenSlot, (int)m_vecItemID.size());
        }
        
        return false;
    }

    return true;
}

void Bag::Clear()
{
    m_vecItemID.clear();
    m_nMaxOpenSlot = 0;
    m_nEmptySlotCount = 0;
    m_nFirstEmptySlot = 0;
}

bool Bag::ExistItem(int nSlot, bool bWriteLog)
{
    if (!IsValidSlot(nSlot, bWriteLog))
    {
        return true;
    }

    return m_vecItemID[nSlot] > 0;
}

void Bag::UpdateSlotItemID(itemid_t nItemID, int nSlot)
{
    m_vecItemID[nSlot] = nItemID;
}

void Bag::UpdateFirstEmptySlot(int nNewEmptySlot)
{
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

void Bag::GetAllItemID(std::list<itemid_t> & listItemID)
{
    for (size_t i = 0; i < m_vecItemID.size(); ++i)
    {
        if (m_vecItemID[i] > 0)
        {
            listItemID.push_back(m_vecItemID[i]);
        }
    }
}

int Bag::SearchFirstEmptySlot()
{
    for (size_t i = 0 ; i < m_vecItemID.size(); ++i)
    {
        if (!ExistItem(i, false))
        {
            return (int)i;
        }
    }

    return m_nMaxOpenSlot;
}

void Bag::AddEmptySlotCount()
{
    m_nEmptySlotCount++;
}

void Bag::DeleteEmptySlotCount()
{
    m_nEmptySlotCount--;
}

int Bag::RemoveItem(ItemBase * pItemBase)
{
    if (pItemBase == NULL)
    {
        return EItemUse_ParaError;
    }

    if (!IsValidSlot(pItemBase->GetGrid()))
    {
        return EItemUse_EquipPos;
    }

    UpdateSlotItemID(0, pItemBase->GetGrid());
    AddEmptySlotCount();
    UpdateFirstEmptySlot(pItemBase->GetGrid());

    pItemBase->SetGrid(-1, -1);
    return EItemError_Success;
}

int Bag::GetMaxOpenIndex() const
{
    return m_nMaxOpenSlot;
}


ConsumableBag::ConsumableBag()
    : Bag(EBagColumn_ExpandableBag)
{

}

ConsumableBag::~ConsumableBag()
{

}

void ConsumableBag::Initialize()
{
    int nMaxSlotCount = CLogicInitData::Instance().GetColumnMaxSlotNum(m_eColumn);
	Bag::Initialize(nMaxSlotCount, nMaxSlotCount);
}

EquipBag::EquipBag()
    : Bag(EBagColumn_ClothBag)
{

}

EquipBag::~EquipBag()
{

}

void EquipBag::Initialize()
{
    int nMaxSlotCount = CLogicInitData::Instance().GetColumnMaxSlotNum(m_eColumn);
	Bag::Initialize(nMaxSlotCount, nMaxSlotCount);
}

int EquipBag::RemoveItem(ItemBase * pItemBase)
{
    return EItemError_Success;
}

BadgeBag::BadgeBag()
    : Bag(EBagColumn_BadgeBag)
{
}

BadgeBag::~BadgeBag()
{


}

void BadgeBag::Initialize()
{
    int nMaxSlotCount = CLogicInitData::Instance().GetColumnMaxSlotNum(m_eColumn);
    Bag::Initialize(nMaxSlotCount, nMaxSlotCount);
}

void BadgeBag::SerializeComponent(CParamPool & IOBuff)
{

}

StorageBag::StorageBag()
:Bag(EBagColumn_Storage)
{

}
StorageBag::~StorageBag()
{}

void StorageBag::Initialize(int nOpenSlot)
{
    int nMaxSlotCount = BagConfigManager::Instance().GetStorageMaxCapacity();
    Bag::Initialize(nMaxSlotCount, nOpenSlot);
}

void StorageBag::AddCapacity(int nAddCapacity)
{
    if (m_nMaxOpenSlot + nAddCapacity >= (int)m_vecItemID.size() || nAddCapacity <= 0)
    {
        return;
    }

    m_nMaxOpenSlot += nAddCapacity;
    m_nEmptySlotCount += nAddCapacity;
}

unsigned int StorageBag::CanAddCapacity()
{
    return (int)(m_vecItemID.size() - m_nMaxOpenSlot);
}

BagManager::BagManager()
	:m_bCollecting(false)
{
    
}

BagManager::~BagManager()
{

}

void BagManager::Initalize(int nStorageCapacity)
{
	BadgeBag * pBadgeBag = new BadgeBag();
	if (pBadgeBag != NULL)
	{
		pBadgeBag->Initialize();
		m_mapBag[EBagColumn_BadgeBag] = pBadgeBag;
	}

	ConsumableBag * pConsumableBag = new ConsumableBag();
	if (pConsumableBag != NULL)
	{
		pConsumableBag->Initialize();
		m_mapBag[EBagColumn_ExpandableBag] = pConsumableBag;
	}

	EquipBag * pEquipBag = new EquipBag();
	if (pEquipBag != NULL)
	{
		pEquipBag->Initialize();
		m_mapBag[EBagColumn_ClothBag] = pEquipBag;
	}

    StorageBag * pStorageBag = new StorageBag();
    if (pStorageBag != NULL)
    {
        pStorageBag->Initialize(nStorageCapacity);
        m_mapBag[EBagColumn_Storage] = pStorageBag;
    }
}

void BagManager::Destory()
{
	std::map<EBagColumn, Bag*>::iterator it = m_mapBag.begin();
	for (; it != m_mapBag.end(); ++it)
	{
		delete it->second; it->second = NULL;
	}
	m_mapBag.clear();
}

int BagManager::InitalizeDBData(EBagColumn eColumn, int nSlot, itemid_t nItemID)
{
	Bag * pBag = GetBag(eColumn);
	if (pBag != NULL)
	{
		return pBag->AddItem(nSlot, nItemID);
	}

    return EItemUse_ParameterErr;
}

int BagManager::AddItem(EBagColumn eColumn, ItemBase * pItemBase)
{
    std::map<EBagColumn, Bag*>::iterator it = m_mapBag.find(eColumn);
    if (it != m_mapBag.end())
    {
        int nSlot = 0;
        int nError = it->second->AddItem(pItemBase->GetItemID(), nSlot);
        if (EItemError_Success == nError)
        {
            pItemBase->SetGrid((int)eColumn, nSlot);
        }

        if (m_bCollecting)
        {
            itemgrid_t grid;
            grid.first = eColumn;
            grid.second = nSlot;
            m_listBagChange.push_back(grid);
        }

        return nError;
    }

    return EItemUse_ParameterErr;
}

int BagManager::AddItem(ItemBase * pItemBase)
{
    EBagColumn eColumn = GetBagColumn(pItemBase->GetType());
	std::map<EBagColumn, Bag*>::iterator it = m_mapBag.find(eColumn);
	if (it != m_mapBag.end())
	{
		int nSlot = 0;
		int nError = it->second->AddItem(pItemBase->GetItemID(), nSlot);
		if (EItemError_Success == nError)
		{
			pItemBase->SetGrid((int)eColumn, nSlot);
		}

		if (m_bCollecting)
		{
            itemgrid_t grid;
            grid.first = eColumn;
            grid.second = nSlot;
            m_listBagChange.push_back(grid);
		}

		return nError;
	}

    return EItemUse_ParameterErr;
}

void BagManager::SyncItemChange(ItemBase * pItemBase)
{
	if (pItemBase != NULL && m_bCollecting)
	{
        m_listBagChange.push_back(make_pair(pItemBase->GetColumn(), pItemBase->GetGrid()));
	}
}

EBagColumn BagManager::GetBagColumn(itemtype_t nItemType)
{
	EItemColumn eItemColumn = ConfigManager::Instance().GetItemConfigManager().GetColumnByItemType(nItemType);
	return GetBagColumn(eItemColumn);
}

EBagColumn BagManager::GetBagColumn(EItemColumn eItemColumn)
{
	switch (eItemColumn)
	{
	case EItemColumn_BadgeBag:return EBagColumn_BadgeBag;
	case EItemColumn_ClothBag:return EBagColumn_ClothBag;
	case EItemColumn_ExpandableBag:return EBagColumn_ExpandableBag;
	default: return EBagColumn_Invalid;
	}
}

int BagManager::RemoveItem(ItemBase * pItemBase)
{
	if (pItemBase == NULL)
	{
		return EItemUse_ParaError;
	}

    Bag * pBag = GetBag((EBagColumn)pItemBase->GetColumn());
    if (pBag == NULL)
    {
        return EItemUse_ParaError;
    }

    if (m_bCollecting)
    {
        itemgrid_t grid;
        grid.first = pItemBase->GetColumn();
        grid.second = pItemBase->GetGrid();
        m_listBagChange.push_back(grid);
    }

    return pBag->RemoveItem(pItemBase);
}

void BagManager::GetAllItemID(EBagColumn eColumn, std::list<itemid_t> & listItemID)
{
    Bag * pBag = GetBag(eColumn);
    if (pBag != NULL)
    {
        pBag->GetAllItemID(listItemID);
    }
}

itemid_t BagManager::GetItemID(EBagColumn eColumn, int nSlot)
{
	std::map<EBagColumn, Bag*>::iterator it = m_mapBag.find(eColumn);
	if (it != m_mapBag.end())
	{
		return it->second->GetItemID(nSlot);
	}

	return 0;
}

unsigned int BagManager::GetColumnPutInMaxCount(EBagColumn eColumn, itemtype_t nItemType, const std::list<itemid_t> & listItemID)
{
	std::map<EBagColumn, Bag*>::iterator it = m_mapBag.find(eColumn);
	if (it == m_mapBag.end())
	{
		return 0;
	}

	unsigned int nCanAddCount = 0;
	for (std::list<itemid_t>::const_iterator itItemID = listItemID.begin();
		itItemID != listItemID.end(); ++itItemID)
	{
		ItemBase * pItemBase = NULL;
		if (pItemBase != NULL && pItemBase->GetColumn() == (int)eColumn)
		{
			nCanAddCount += pItemBase->GetLeftStackCount();
		}
	}

	ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
	if (pItemConfig != NULL)
	{
		nCanAddCount += pItemConfig->m_nMaxStackCount*it->second->GetEmptySlotCount();
	}

	return nCanAddCount;
}

unsigned int BagManager::GetColumnEmptyCount(EBagColumn eColumn)
{
	std::map<EBagColumn, Bag*>::iterator it = m_mapBag.find(eColumn);
	if (it != m_mapBag.end())
	{
		return it->second->GetEmptySlotCount();
	}

	return 0;
}

bool BagManager::IsColumnValid(EBagColumn eColumn)
{
	if (eColumn <= EBagColumn_Invalid || eColumn >= EBagColumn_Max)
	{
		return false;
	}

	return true;
}

Bag * BagManager::GetBag(EBagColumn eColumn)
{
	std::map<EBagColumn, Bag*>::iterator it = m_mapBag.find(eColumn);
	if (it != m_mapBag.end())
	{
		return it->second;
	}

	return NULL;
}

void BagManager::BeginCollectBagChange()
{
	m_bCollecting = true;
	m_listBagChange.clear();
}

void BagManager::AddStorageCapacity(int nAddCapacity)
{
    StorageBag * pStorageBag = (StorageBag*)GetBag(EBagColumn_Storage);
    if (pStorageBag == NULL)
    {
        return;
    }

    pStorageBag->AddCapacity(nAddCapacity);
}

void BagManager::EndCollectBagChange(std::list<itemgrid_t> & listGrid) 
{
	m_bCollecting = false;
    listGrid = m_listBagChange;
}

unsigned int BagManager::GetStoragesCapacity()
{
    StorageBag * pStorageBag = (StorageBag*)GetBag(EBagColumn_Storage);
    if (pStorageBag == NULL)
    {
        return 0;
    }

    return pStorageBag->GetMaxOpenIndex();
}

unsigned int BagManager::GetStorageCanAddCapacity()
{
    StorageBag * pStorageBag = (StorageBag*)GetBag(EBagColumn_Storage);
    if (pStorageBag == NULL)
    {
        return 0;
    }

    return pStorageBag->CanAddCapacity();
}

