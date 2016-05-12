#include "LimitedGoodsInfo.h"
#include "FileStream.h"
#include "../../socket/Windefine.h"

LimitedGoodsInfo::LimitedGoodsInfo(void)
{
    m_uSerializeID = 1;
    m_nBuyType = 1;
    m_uBuyPrice = (unsigned int)-1;
    m_nCount = 0;
    m_nLockCount = 0;
}

bool LimitedGoodsInfo::LoadFromFile(CFileStream &file)
{
    if (!m_cGoodsInfo.LoadFromFile(file))
        return false;

    file.ReadUShort(m_uSerializeID);
    file.ReadUChar(m_nBuyType);
    file.ReadUInt(m_uBuyPrice);
    file.ReadInt(m_nCount);

    ASSERT(0 < m_uSerializeID);
    ASSERT(1 <= m_nBuyType && m_nBuyType <= 4 && 2 != m_nBuyType);
    ASSERT(0 < m_uBuyPrice);
    ASSERT(0 <= m_nCount);

    return true;
}

bool LimitedGoodsInfo::SaveToFile(CFileStream &file)
{
    if (0 == m_uSerializeID || 0 == m_nBuyType || m_nBuyType > 4 || 2 == m_nBuyType || 
        m_nCount < 0 || 0 == m_uBuyPrice || !m_cGoodsInfo.SaveToFile(file))
        return false;

    file.WriteUShort(m_uSerializeID);
    file.WriteUChar(m_nBuyType);
    file.WriteUInt(m_uBuyPrice);
    file.WriteInt(m_nCount);

    return true;
}


