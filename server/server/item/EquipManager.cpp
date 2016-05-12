#include "EquipManager.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "ItemMsgDefine.h"
#include "../../datastructure/RoleEntity.h"
#include "../logic/PlayerManager.h"
#include "../../datastructure/ItemBase.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "EntityItemComponent.h"

Equip::Equip(EEquipColumn eColumn)
: m_eColumn(eColumn)
{

}

Equip::~Equip()
{

}

void Equip::Initialize(int nMaxSlot)
{
    m_vecItemID.resize(nMaxSlot);
}

int Equip::Initialize(int nSlot, itemid_t nItemID)
{
    if (!IsSlotValid(nSlot))
    {
        return EItemUse_InvalidTarget;
    }

    if (IsExist(nSlot))
    {
        return EItemUse_EquipPos;
    }

    m_vecItemID[nSlot] = nItemID;
    return EItemError_Success;
}

int Equip::CanEquip(ItemEquip * pEquip)
{
    return EItemError_Success;
}

int Equip::OnEquip(ItemEquip * pEquip)
{
    return EItemError_Success;
}

int Equip::UnEquip(ItemEquip * pEquip)
{
    return EItemError_Success;
}

bool Equip::HasEquip(int nPos, itemid_t nItemID)
{
    if (!IsSlotValid(nPos))
    {
        return false;
    }

    return m_vecItemID[nPos] > 0;
}

bool Equip::HasEquip(itemid_t nItemID)
{
    std::vector<itemid_t>::iterator it = m_vecItemID.begin();
    for (; it != m_vecItemID.end(); ++it)
    {
        if (*it == nItemID)
        {
            return true;
        }
    }

    return false;
}

itemid_t Equip::GetItemID(int nSlot)
{
    if (IsSlotValid(nSlot))
    {
        return m_vecItemID[nSlot];
    }

    return 0;
}

void Equip::GetAllItemID(std::list<itemid_t> & listItemID)
{
    for (size_t i = 0; i < m_vecItemID.size(); ++i)
    {
        if (m_vecItemID[i] > 0)
        {
            listItemID.push_back(m_vecItemID[i]);
        }
    }
}

void Equip::AddSlotCount(int nAddCount)
{
    size_t nOldSize = m_vecItemID.size();
    m_vecItemID.resize(nOldSize + (size_t)nAddCount);
}

bool Equip::IsExist(int nSlot)
{
    if (!IsSlotValid(nSlot))
    {
        return false;
    }

    return m_vecItemID[nSlot] > 0 ? true : false;
}

bool Equip::IsSlotValid(int nSlot)
{
    if (nSlot < 0 || nSlot >= (int)m_vecItemID.size())
    {
        WriteLog(LOGLEVEL_ERROR, "[Item] Equip::IsSlotValid nSlot = %d", nSlot);
        return false;
    }

    return true;
}

unsigned int Equip::GetItemCount()
{
    unsigned int nItemCount = 0;
    for (size_t i = 0; i < m_vecItemID.size(); ++i)
    {
        if (m_vecItemID[i] > 0)
        {
            nItemCount++;
        }
    }

    return nItemCount;
}

BadgeEquip::BadgeEquip()
: Equip(EEquipColumn_BadgeOnPlayer)
{

}

BadgeEquip::~BadgeEquip()
{
}

int BadgeEquip::CanEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    if (HasEquip(pEquip->GetItemID()))
    {
        return EItemUse_ParaError;
    }

    int nNewPos = GetFirstFreePos();
    if (nNewPos == -1)
    {
        return EItemUse_ColumnFull;
    }

    return EItemError_Success;
}

int BadgeEquip::OnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    int nError = CanEquip(pEquip);
    if (nError == EItemError_Success)
    {
        int nNewPos = GetFirstFreePos();
        if (nNewPos == -1)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] BadgeEquip_Full: ItemID = %lld", pEquip->GetItemID());
            return EItemUse_ColumnFull;
        }
        m_vecItemID[nNewPos] = pEquip->GetItemID();
        pEquip->SetGrid((int)m_eColumn, nNewPos);
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "[Item] BadgeEquip_On: ItemID = %lld", pEquip->GetItemID());
    }

    return nError;
}

int BadgeEquip::GetFirstFreePos()
{
    for (size_t i = 0; i < m_vecItemID.size(); ++i)
    {
        if (m_vecItemID[i] > 0)
        {
            continue;
        }

        return i;
    }

    return (int)-1;
}

int BadgeEquip::UnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL || !HasEquip(pEquip->GetGrid(), pEquip->GetItemID()))
    {
        if (pEquip != NULL)
        {
            WriteLog(LOGLEVEL_ERROR, "[Item] BadgeEquip_Un: ItemID = %lld, nSlot = %d", pEquip->GetItemID(), pEquip->GetGrid());

            if (IsSlotValid(pEquip->GetGrid()))
            {
                WriteLog(LOGLEVEL_ERROR, "[Item] BadgeEquip_Un: ItemID = %lld, ExistItemID = %lld", pEquip->GetItemID(), m_vecItemID[pEquip->GetGrid()]);
            }
        }

        return EItemUse_ParaError;
    }

    m_vecItemID[pEquip->GetGrid()] = 0;
    pEquip->SetGrid(-1, -1);
    return EItemError_Success;
}

WeddingRingEquip::WeddingRingEquip()
:Equip(EEquipColumn_WeddingRingOnPlayer)
{
}

WeddingRingEquip::~WeddingRingEquip()
{
}

int WeddingRingEquip::CanEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL || !pEquip->IsWeddingRing())
    {
        return EItemUse_ParaError;
    }

    if (m_vecItemID[0] > 0)
    {
        return EItemUse_ColumnFull;
    }

    return EItemError_Success;
}

int WeddingRingEquip::OnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    int nError = CanEquip(pEquip);
    if (nError == EItemError_Success)
    {
        m_vecItemID[0] = pEquip->GetItemID();
        pEquip->SetGrid((int)m_eColumn, 0);
    }

    return nError;
}

int WeddingRingEquip::UnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL || !pEquip->IsWeddingRing())
    {
        return EItemUse_ParaError;
    }

    if (m_vecItemID[0] == 0)
    {
        return EItemUse_ParaError;
    }

    m_vecItemID[0] = 0;
    pEquip->SetGrid(-1, -1);
    return EItemError_Success;
}

ClothEquip::ClothEquip()
: Equip(EEquipColumn_ClothOnPlayer)
{
}

ClothEquip::~ClothEquip()
{

}

int ClothEquip::CanEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL || HasEquip(pEquip->GetEquipConfigPos(), pEquip->GetItemID()))
    {
        return EItemUse_ParaError;
    }

    return EItemError_Success;
}

int ClothEquip::OnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    int nError = CanEquip(pEquip);
    if (nError == EItemError_Success)
    {
        m_vecItemID[pEquip->GetEquipConfigPos()] = pEquip->GetItemID();
        pEquip->SetGrid((int)m_eColumn, pEquip->GetEquipConfigPos());
    }

    return nError;
}

int ClothEquip::UnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL || !HasEquip(pEquip->GetEquipConfigPos(), pEquip->GetItemID()))
    {
        return EItemUse_ParaError;
    }

    m_vecItemID[pEquip->GetEquipConfigPos()] = 0;
    pEquip->SetGrid(-1, -1);
    return EItemError_Success;
}

void ClothEquip::UnEquipAll(std::list<itemid_t> & listItemID)
{
    for (size_t i = 0; i < m_vecItemID.size(); ++i)
    {
        if (m_vecItemID[i] != 0)
        {
            listItemID.push_back(m_vecItemID[i]);
            m_vecItemID[i] = 0;
        }
    }
}



TransformEquip::TransformEquip()
:Equip(EEquipColumn_TransformClothOnPlayer)
{
}

TransformEquip::~TransformEquip()
{

}

int TransformEquip::CanEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL || HasEquip(pEquip->GetEquipConfigPos(), pEquip->GetItemID()))
    {
        return EItemUse_ParaError;
    }

    return EItemError_Success;
}

int TransformEquip::OnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {   
        return EItemUse_ParaError;
    }

    int nError = CanEquip(pEquip);
    if (nError == EItemError_Success)
    {
        m_vecItemID[pEquip->GetEquipConfigPos()] = pEquip->GetItemID();
        pEquip->SetGrid((int)m_eColumn, pEquip->GetEquipConfigPos());
    }

    return nError;
}

int TransformEquip::UnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL || !pEquip->IsEquip())
    {
        return EItemUse_ParaError;
    }

    m_vecItemID[pEquip->GetEquipConfigPos()] = 0;
    pEquip->SetGrid(-1, -1);
    return EItemUse_ParaError;
}

EquipManager::EquipManager()
{

}

EquipManager::~EquipManager()
{

}

void EquipManager::Initialize(int nBadgeSlotCount)
{
    ClothEquip * pCloth = new ClothEquip();
    if (pCloth != NULL)
    {
        int nMaxSlotCount = CLogicInitData::Instance().GetColumnMaxSlotNum(EEquipColumn_ClothOnPlayer);
        pCloth->Initialize(nMaxSlotCount);
        m_mapEquip[EEquipColumn_ClothOnPlayer] = pCloth;
    }

    BadgeEquip * pBadge = new BadgeEquip();
    if (pBadge != NULL)
    {
        pBadge->Initialize(nBadgeSlotCount);
        m_mapEquip[EEquipColumn_BadgeOnPlayer] = pBadge;
    }

    WeddingRingEquip * pWeddingRing = new WeddingRingEquip();
    if (pWeddingRing != NULL)
    {
        int nMaxSlotCount = CLogicInitData::Instance().GetColumnMaxSlotNum(EEquipColumn_WeddingRingOnPlayer);
        pWeddingRing->Initialize(nMaxSlotCount);
        m_mapEquip[EEquipColumn_WeddingRingOnPlayer] = pWeddingRing;
    }

    TransformEquip * pTransform = new TransformEquip();
    if (pTransform != NULL)
    {
        int nMaxSlotCount = CLogicInitData::Instance().GetColumnMaxSlotNum(EEquipColumn_TransformClothOnPlayer);
        pTransform->Initialize(nMaxSlotCount);
        m_mapEquip[EEquipColumn_TransformClothOnPlayer] = pTransform;
    }
}

void EquipManager::Destory()
{
    std::map<EEquipColumn, Equip*>::iterator it = m_mapEquip.begin();
    for (; it != m_mapEquip.end(); ++it)
    {
        delete it->second; it->second = NULL;
    }
    m_mapEquip.clear();
}

int EquipManager::InitializeDBData(EEquipColumn eColumn, int nSlot, itemid_t nItemID)
{
    Equip * pEquip = GetEquip(eColumn);
    if (pEquip != NULL)
    {
        return pEquip->Initialize(nSlot, nItemID);
    }

    return EItemUse_ParameterErr;
}

Equip * EquipManager::GetEquip(EEquipColumn eColumn)
{
    std::map<EEquipColumn, Equip*>::iterator it = m_mapEquip.find(eColumn);
    if (it != m_mapEquip.end())
    {
        return it->second;
    }

    return NULL;
}

EEquipColumn EquipManager::GetEquipColumn(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EEquipColumn_Invalid;
    }

    if (pEquip->IsCloth())
    {
        return EEquipColumn_ClothOnPlayer;
    }
    else if (pEquip->IsWeddingRing())
    {
        return EEquipColumn_WeddingRingOnPlayer;
    }
    else if (pEquip->IsBadge())
    {
        return EEquipColumn_BadgeOnPlayer;
    }

    return EEquipColumn_Invalid;
}

int EquipManager::OnEquip(EEquipColumn eColumn, ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    Equip * pEquipColumn = GetEquip(eColumn);
    if (pEquipColumn == NULL)
    {
        return EItemUse_ParaError;
    }

    pEquipColumn->OnEquip(pEquip);
    pEquip->OnEquip();
    return EItemError_Success;
}

int EquipManager::OnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    EEquipColumn eColumn = GetEquipColumn(pEquip);
    Equip * pEquipColumn = GetEquip(eColumn);
    if (pEquipColumn == NULL)
    {
        return EItemUse_ParaError;
    }

    int nError = pEquipColumn->OnEquip(pEquip);
    if (nError == EItemError_Success)
    {
        pEquip->OnEquip();
    }
    
    return nError;
}

int EquipManager::UnEquip(EEquipColumn eColumn, ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    Equip * pEquipColumn = GetEquip(eColumn);
    if (pEquipColumn == NULL)
    {
        return EItemUse_ParaError;
    }

    pEquipColumn->UnEquip(pEquip);
    pEquip->UnEquip();

    return EItemUse_ParaError;
}

int EquipManager::UnEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    EEquipColumn eColumn = GetEquipColumn(pEquip);
    Equip * pEquipColumn = GetEquip(eColumn);
    if (pEquipColumn == NULL)
    {
        return EItemUse_ParaError;
    }

    int nError = pEquipColumn->UnEquip(pEquip);
    if (nError == EItemError_Success)
    {
        pEquip->UnEquip();
    }
    
    return nError;
}

int EquipManager::UnEquipAll(EEquipColumn eColumn, std::list<itemid_t> & listItemID)
{
    std::map<EEquipColumn, Equip*>::iterator it = m_mapEquip.find(eColumn);
    if (it != m_mapEquip.end())
    {
        it->second->UnEquipAll(listItemID);
    }

    return EItemError_Success;
}

itemid_t EquipManager::GetItemID(EEquipColumn eColumn, int nSlot)
{
    std::map<EEquipColumn, Equip*>::iterator it = m_mapEquip.find(eColumn);
    if (it != m_mapEquip.end())
    {
        return it->second->GetItemID(nSlot);
    }

    return EItemError_Success;
}

itemid_t EquipManager::GetClothItemID(int nSlot)
{
    return GetItemID(EEquipColumn_ClothOnPlayer, nSlot);
}

void EquipManager::GetEquipedCloth(std::list<itemid_t> & listItemID)
{
    GetAllItemID(EEquipColumn_ClothOnPlayer, listItemID);
}

itemid_t EquipManager::GetEquipedCloth(EItemClothType ePos)
{
    Equip * pEquipColumn = GetEquip(EEquipColumn_ClothOnPlayer);
    if (pEquipColumn != NULL)
    {
        return pEquipColumn->GetItemID(ePos);
    }

    return 0;
}

unsigned int EquipManager::GetColumnItemCount(EEquipColumn eColumn)
{
    std::map<EEquipColumn, Equip*>::iterator it = m_mapEquip.find(eColumn);
    if (it != m_mapEquip.end())
    {
        return it->second->GetItemCount();
    }

    return 0;
}

bool EquipManager::IsEquip(ItemEquip * pEquip)
{
    Equip * pEquipColumn = GetEquip((EEquipColumn)pEquip->GetColumn());
    if (pEquipColumn != NULL)
    {
        return pEquipColumn->GetItemID(pEquip->GetEquipConfigPos()) > 0;
    }

    return false;
}

int EquipManager::CanEquip(ItemEquip * pEquip)
{
    if (pEquip == NULL)
    {
        return EItemUse_ParaError;
    }

    Equip * pEquipColumn = GetEquip(GetEquipColumn(pEquip));
    if (pEquipColumn != NULL)
    {
        return pEquipColumn->CanEquip(pEquip);
    }

    return EItemUse_ParaError;
}

void EquipManager::GetAllItemID(EEquipColumn eColumn, std::list<itemid_t> & listItemID)
{
    Equip * pEquipColumn = GetEquip(eColumn);
    if (pEquipColumn != NULL)
    {
        pEquipColumn->GetAllItemID(listItemID);
    }
}

void EquipManager::AddSlotCount(EEquipColumn eColumn, int nAddCount)
{
    Equip * pEquipColumn = GetEquip(eColumn);
    if (pEquipColumn != NULL)
    {
        pEquipColumn->AddSlotCount(nAddCount);
    }
}
