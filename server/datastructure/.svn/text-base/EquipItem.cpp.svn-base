#include "StdAfx.h"
#include "EquipItem.h"
#include "datainfor/ItemType.h"
#include "ItemErrorInfo.h"
#include "../socket/ParamPool.h"


EquipItem::EquipItem()
    : ItemBase()
    , m_nIsEquiped(0)
    , m_nLoginEquipedStatus(0)
    , m_nEffectID(0)
{
}

EquipItem::EquipItem(const ItemConfig *pItemConfig)
    : ItemBase(pItemConfig)
    , m_nIsEquiped(0)
    , m_nLoginEquipedStatus(0)
    , m_nEffectID(0)
{
}

EquipItem::~EquipItem(void)
{
}

void EquipItem::Init(const DBItem &dbItem)
{
    ItemBase::Init(dbItem);
    m_nIsEquiped = dbItem.m_nIsEquiped;
    m_nLoginEquipedStatus = m_nIsEquiped;
}

void EquipItem::Encode(CParamPool * pPool) const
{
    ItemBase::Encode(pPool);
    pPool->AddUChar(m_nIsEquiped);
    pPool->AddUInt(m_nEffectID);
}

void EquipItem::OnEquip(unsigned char nPos)
{
    SetEquipedPos(nPos);
}

void EquipItem::OnUnEquip()
{
    SetEquipedPos(0);
}

void EquipItem::SetEquipedPos(unsigned char nEquipedPos)
{
    m_nIsEquiped = nEquipedPos;
}

bool EquipItem::HasEquiped() const
{ 
    return m_nIsEquiped > 0;
}

unsigned char EquipItem::GetEquipedPos() const
{
    return m_nIsEquiped;
}

unsigned char EquipItem::GetEquipConfigPos() const
{
    if (m_pItemConfig != NULL)
        return m_pItemConfig->GetClothPos();

    return 0;
}

bool EquipItem::RequireIntegrityCheck() const
{
    if (m_pItemConfig != NULL && !m_pItemConfig->IsItemCloth_Type_Skin())
        return true;

    return false;
}

void EquipItem::SetEffectID(unsigned int nEffectID)
{
    m_nEffectID = nEffectID;
}

unsigned int EquipItem::GetEffectID() const
{
    return m_nEffectID;
}

bool EquipItem::IsOldEffectCloth()
{
    if (m_pItemConfig != NULL)
    {
        return m_pItemConfig->m_bOldEffectCloth;
    }

    return true;
}

bool EquipItem::IsEquipStatusChanged() const
{
    return m_nIsEquiped != m_nLoginEquipedStatus;
}

void EquipItem::ResetLoginEquipStatus()
{
    m_nLoginEquipedStatus = m_nIsEquiped;
}

