#include "ItemBase.h"
#include "EquipItem.h"
#include "ItemErrorInfo.h"
#include "LogicInitData.h"
#include "../server/share/ServerDataStruct.h"
#include "datainfor/ConfigManager.h"
#include "../socket/ParamPool.h"
#include "DataStruct_Base.h"
#include "DataStruct_Game.h"
#include "../server/util/IDCreater.h"

// 全局的ID生成器
extern CIDCreater *g_pIDCreater;

ItemBase::ItemBase()
    : m_nItemID(0)
    , m_nColumn(-1)
    , m_nGrid(-1)
    , m_nDueTime(0)
    , m_nCount(0)
    , m_nLeftUseTimes(0)
    , m_nDynamicBoxRelatedValue(0)
    , m_pItemConfig(NULL)
{
}

ItemBase::ItemBase(const ItemConfig * pItemConfig)
    : m_nItemID(0)
    , m_nColumn(-1)
    , m_nGrid(-1)
    , m_nDueTime(0)
    , m_nCount(0)
    , m_nLeftUseTimes(0)
    , m_nDynamicBoxRelatedValue(0)
    , m_pItemConfig(pItemConfig)
{
}

ItemBase::~ItemBase(void)
{
}

void ItemBase::InitFromDBItem( const DBItem &dbItem )
{
    SetItemID(dbItem.m_nItemID);
    SetGrid((EItemColumn)dbItem.m_nColumn, dbItem.m_nIndex);
    SetCount(dbItem.m_nCount);
    SetLeftUseTimes(dbItem.m_nUseTimes);
    SetDueTime(dbItem.m_nDuration);
    SetDynamicBoxRelatedValue(dbItem.m_nDynamicBoxRelatedValue);
}

void ItemBase::SaveToDBItem( DBItem &dbItem ) const
{
    dbItem.m_nItemID = m_nItemID;
    if ( m_pItemConfig != NULL )
        dbItem.m_nTypeID = m_pItemConfig->GetType();
    dbItem.m_nIndex = (unsigned short)m_nGrid;
    dbItem.m_nColumn = (unsigned short)m_nColumn;
    dbItem.m_nCount = m_nCount;
    dbItem.m_nUseTimes = m_nLeftUseTimes;
    dbItem.m_nDuration = m_nDueTime;
    dbItem.m_bEquiped = false;
    dbItem.m_nDynamicBoxRelatedValue = m_nDynamicBoxRelatedValue;
}

int ItemBase::OnUse()
{
    ASSERT(IsValid());

    int nError = EItemError_CannotUse;
    if (IsValid())
    {
        // 这里只需要处理有使用次数限制的物品
        if (m_nLeftUseTimes > 0)
        {
            m_nLeftUseTimes--;

            // 使用过之后，若该物品，则重置剩余物品使用次数为配置表中的次数
            if (m_nLeftUseTimes == 0) 
            {
                m_nCount--;

                if (m_nCount > 0)
                {
                    m_nLeftUseTimes = m_pItemConfig->m_nUseTimes;
                }
            }
        }

        nError = EItemError_Success;
    }

    return nError;
}

int ItemBase::OnBatchUse(int nCount)
{
    ASSERT(IsValid());

    EItemError nError = EItemError_CannotUse;
    if (IsValid() && nCount > 0)
    {
        if (m_nLeftUseTimes > 0)
        {
            m_nLeftUseTimes--;

            if (m_nLeftUseTimes == 0)
            {
                if (m_nCount >= nCount)
                {
                    m_nCount -= (unsigned short)nCount;
                }
                else
                {
                    m_nCount = 0;
                }

                if (m_nCount > 0 && m_pItemConfig != NULL)
                {
                    ASSERT(m_pItemConfig->m_nUseTimes == 1);
                    m_nLeftUseTimes = m_pItemConfig->m_nUseTimes;
                }
            }
        }

        nError = EItemError_Success;
    }

    return nError;
}

bool ItemBase::IsValid() const
{
    return (m_pItemConfig != NULL) 
        && (m_nCount > 0) 
        && (m_nLeftUseTimes == -1 || m_nLeftUseTimes > 0);
}

void ItemBase::Encode(CParamPool *pPool) const
{
    pPool->AddShort(m_nLeftUseTimes);
    pPool->AddUShort(m_nCount);

    if (m_nDueTime > 0)
    {
        int nDuration = m_nDueTime - (int)time(NULL);
        pPool->AddInt(nDuration);
    }
    else
    {
        pPool->AddInt(-1);
    }
}

void ItemBase::Decode(CParamPool * pPool)
{
    m_nLeftUseTimes = pPool->GetShort();
    m_nCount = pPool->GetUShort();

    int nDuration = pPool->GetInt();
    if (nDuration == -1)
    {
        m_nDueTime = -1;
    }
    else
    {
        m_nDueTime = (int)time(NULL) + nDuration;
    }
}


itemid_t ItemBase::GetItemID() const
{
    return m_nItemID;
}

void ItemBase::SetItemID(itemid_t nItemID)
{
    m_nItemID = nItemID;
}

int ItemBase::GetColumn() const
{
    return m_nColumn;
}

int ItemBase::GetGrid() const
{
    return m_nGrid;
}

void ItemBase::SetGrid(int nColumn, int nGrid)
{
	m_nColumn = nColumn;
    m_nGrid = nGrid;
}

unsigned short ItemBase::GetCount() const
{
    return m_nCount;
}

void ItemBase::SetCount(unsigned short nCount, bool bCheckUsetimes)
{
    int nTempCount = m_nCount;
    m_nCount = nCount;
  
    unsigned short nMaxStackCount = (unsigned short)GetMaxStackCount();
    if (nMaxStackCount < m_nCount)
    {
        m_nCount = nMaxStackCount;
    }

    if (bCheckUsetimes)
    {
        if (m_pItemConfig != NULL && m_pItemConfig->m_nUseTimes == -1 && m_nCount == 0)
        {
            m_nCount = (unsigned short)nTempCount;
        }
    }
}

void ItemBase::ModifyCount(short nChangeCount)
{
    if(nChangeCount < 0)
    {
        unsigned short nUCount = (unsigned short)abs(nChangeCount);
        m_nCount = m_nCount <  nUCount? 0:m_nCount- nUCount;
    }
    else
    {
        m_nCount = m_nCount + (unsigned short)nChangeCount;
    }

    if(m_pItemConfig != NULL && m_nCount > m_pItemConfig->m_nMaxStackCount)
    {
        m_nCount = m_pItemConfig->m_nMaxStackCount;
    }
}

bool ItemBase::IsDeleteByUsed(int nCount)
{
    if (m_pItemConfig != NULL && m_pItemConfig->m_nUseTimes == -1)
    {
        return false;
    }

    if (m_nCount + nCount <= 0)
    {
        return true;
    }

    return false;
}

short ItemBase::GetLeftUseTimes() const
{
    return m_nLeftUseTimes;
}

void ItemBase::SetLeftUseTimes(short nUseTimes)
{
    m_nLeftUseTimes = nUseTimes;
}

bool ItemBase::IsOverDue(int nCurTime) const
{
    return m_nDueTime > 0 && nCurTime >= m_nDueTime;
}

int ItemBase::GetDueTime() const
{
    return m_nDueTime;
}

void ItemBase::SetDueTime(int nDueTime)
{
    m_nDueTime = nDueTime;
}

void ItemBase::AddDuration(int nAddDuration)
{
    if (m_pItemConfig == NULL)
    {
        return;
    }

    if (nAddDuration > 0)
    {
        m_nDueTime += nAddDuration;
    }
    else if (nAddDuration == -1)
    {
        m_nDueTime = 0;
    }
}

int ItemBase::GetDynamicBoxRelatedValue() const
{
    return m_nDynamicBoxRelatedValue;
}

void ItemBase::SetDynamicBoxRelatedValue(int nRelatedValue)
{
    m_nDynamicBoxRelatedValue = nRelatedValue;
}

unsigned int ItemBase::GetLeftStackCount() const
{
    if (m_pItemConfig != NULL && m_nCount < m_pItemConfig->m_nMaxStackCount)
    {
        return m_pItemConfig->m_nMaxStackCount - m_nCount;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
itemtype_t ItemBase::GetType() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->GetType() : 0;
}

unsigned char ItemBase::GetAnType(int i) const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_anType[i] : 0;
}

bool ItemBase::IsExpandable() const 
{
    return m_pItemConfig != NULL ? m_pItemConfig->IsExpandable() : false;
}

bool ItemBase::IsEquip() const 
{
    return m_pItemConfig != NULL ? m_pItemConfig->IsEquip() : false;
}

bool ItemBase::IsCloth() const 
{
    return m_pItemConfig != NULL? m_pItemConfig->IsCloth() : false;
}

bool ItemBase::IsHorse() const 
{
    return m_pItemConfig != NULL? m_pItemConfig->IsHorse() : false;
}

bool ItemBase::IsWeddingRing() const 
{
    if ( m_pItemConfig != NULL)
    {
        return m_pItemConfig->IsBadge_Type_WeddingRing();
    }

    return false;
}

bool ItemBase::IsBadge() const 
{
    if (m_pItemConfig != NULL)
    {
        return m_pItemConfig->IsBadge();
    }

    return false;
}

const char* ItemBase::GetName() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_strName.c_str() : NULL;
}


unsigned int ItemBase::GetMaxStackCount() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_nMaxStackCount : 0;
}


int ItemBase::GetCoolDownTime() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_nCooldownTime : 0;
}

unsigned short ItemBase::GetCoolDownTag() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_nCoolDownTag : 0;
}

unsigned short ItemBase::GetVipLevelConfig() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_nVIP : 0;
}

int ItemBase::GetUseTimesConfig() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_nUseTimes : 0;
}

int ItemBase::GetDueTimeConfig() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_nDueTime : 0;
};

void ItemBase::GetGene(std::list<GeneInfo*> &listGene) const
{
    if (m_pItemConfig != NULL)
    {
        listGene = m_pItemConfig->m_listGene;
    }
}

ESexType ItemBase::GetSexConfig() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_SexNeed : ESexType_No;
}

int ItemBase::GetLevelConfig() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_nNeedLevel : 0;
}

unsigned int ItemBase::GetEffectIDConfig() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->m_nClothEffectID : 0;
}

bool ItemBase::IsCanAddEffectCloth()
{
    if (m_pItemConfig != NULL && IsCloth() && !m_pItemConfig->m_bOldEffectCloth)
    {
        return true;
    }

    return false;
}

bool ItemBase::IsItemCloth_Type_Suit() const
{
    if (m_pItemConfig != NULL)
    {
        return m_pItemConfig->IsItemCloth_Type_Suit();
    }

    return false;
}

bool ItemBase::IsPackage() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->IsPackage() : false;
}

bool ItemBase::IsPacketExpandable() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->IsPacketExpandable() : false;
}

bool ItemBase::IsBoxExpandable() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->IsBoxExpandable() : false;
}

bool ItemBase::IsDynamicBoxExpandable() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->IsDynamicBoxExpandable() : false;
}

bool ItemBase::IsPetEgg() const
{
    return m_pItemConfig != NULL ? m_pItemConfig->IsPetEgg() : false;
}

bool ItemBase::IsTransform()
{
    return m_pItemConfig == NULL ? false:m_pItemConfig->m_bTransform;
}


ItemEquip::ItemEquip()
: ItemBase()
, m_bEquiped(false)
, m_bInitStauts(false)
, m_nEffectID(0)
{
}

ItemEquip::ItemEquip(const ItemConfig *pItemConfig)
: ItemBase(pItemConfig)
, m_bEquiped(false)
, m_bInitStauts(false)
, m_nEffectID(0)
{
}

ItemEquip::~ItemEquip(void)
{
}

void ItemEquip::InitFromDBItem( const DBItem &dbItem )
{
    ItemBase::InitFromDBItem( dbItem );
	m_bEquiped = dbItem.m_bEquiped;
    m_bInitStauts = m_bEquiped;
}

void ItemEquip::SaveToDBItem( DBItem &dbItem ) const
{
    ItemBase::SaveToDBItem( dbItem );
    dbItem.m_bEquiped = m_bEquiped;
}

void ItemEquip::Encode(CParamPool * pPool) const
{
	ItemBase::Encode(pPool);
	pPool->AddUChar(m_bEquiped ? 1 : 0);
	pPool->AddUInt(m_nEffectID);
}

void ItemEquip::Decode(CParamPool * pPool)
{
    ItemBase::Decode(pPool);
    m_bEquiped = (pPool->GetUChar() == 1);
    m_nEffectID = pPool->GetUInt();

    return;
}


void ItemEquip::OnEquip()
{
	m_bEquiped = true;
}

void ItemEquip::UnEquip()
{
	m_bEquiped = false;
}

bool ItemEquip::IsEquiped() const
{
	return m_bEquiped;
}

unsigned char ItemEquip::GetEquipConfigPos() const
{
	if (m_pItemConfig != NULL)
		return m_pItemConfig->GetClothPos();

	return 0;
}

bool ItemEquip::RequireIntegrityCheck() const
{
	if (m_pItemConfig != NULL && !m_pItemConfig->IsItemCloth_Type_Skin())
		return true;

	return false;
}

void ItemEquip::SetEffectID(unsigned int nEffectID)
{
	m_nEffectID = nEffectID;
}

unsigned int ItemEquip::GetEffectID() const
{
	return m_nEffectID;
}

bool ItemEquip::IsOldEffectCloth()
{
	if (m_pItemConfig != NULL)
	{
		return m_pItemConfig->m_bOldEffectCloth;
	}

	return true;
}

bool ItemEquip::IsStautsChange()
{
    return m_bInitStauts != m_bEquiped;
}

bool ItemEquip::IsPerpetual()
{
    return GetDueTime() <= 0;
}


