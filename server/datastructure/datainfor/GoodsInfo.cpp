#include "GoodsInfo.h"
#include "FileStream.h"
#include "../../socket/Windefine.h"
#include "../../datastructure/DataStruct_Base.h"

#define ITEMID_WEDDING_RING_START   21000
#define ITEMID_WEDDING_RING_END     22000

GoodsInfo::GoodsInfo(void)
    :m_uIdx(0)
    ,m_uGoodsID(0)
    ,m_uNum(0)
    ,m_nValidTime(0)
    ,m_nSellType(1)
    ,m_uSellPrice((unsigned int)-1)
{
}

GoodsInfo::~GoodsInfo()
{

}

bool GoodsInfo::LoadFromFile(CFileStream &file)
{
    file.ReadUShort(m_uIdx);
    file.ReadUInt(m_uGoodsID);
    file.ReadUShort(m_uNum);
    file.ReadInt(m_nValidTime);
    file.ReadUChar(m_nSellType);
    file.ReadUInt(m_uSellPrice);

    ASSERT(0 < m_uGoodsID);
    ASSERT(!(ITEMID_WEDDING_RING_START <= m_uGoodsID && m_uGoodsID < ITEMID_WEDDING_RING_END));// 不可贩卖结婚戒指
    ASSERT(0 < m_uNum);
    ASSERT(1 <= m_nSellType && m_nSellType <= 4 && 2 != m_nSellType);
    ASSERT(0 < m_uSellPrice);

    return true;
}

bool GoodsInfo::SaveToFile(CFileStream &file)
{
    if (0 == m_uGoodsID || 0 == m_uNum || 0 == m_uSellPrice || 
        0 == m_nSellType || m_nSellType > 4 || 2 == m_nSellType || 
        (ITEMID_WEDDING_RING_START <= m_uGoodsID && m_uGoodsID < ITEMID_WEDDING_RING_END))
        return false;

    file.WriteUShort(m_uIdx);
    file.WriteUInt(m_uGoodsID);
    file.WriteUShort(m_uNum);
    file.WriteInt(m_nValidTime);
    file.WriteUChar(m_nSellType);
    file.WriteUInt(m_uSellPrice);

    return true;
}

void GoodsInfo::CopyInfoToCItem(CItem *pItem) const
{
    if (NULL != pItem)
    {
        pItem->m_nItemType = m_uGoodsID;
        pItem->m_nItemCount = m_uNum;
        pItem->m_nValidTime = m_nValidTime;
    }
}


