#include "AuctionGoodsInfo.h"
#include "FileStream.h"
#include "../../socket/Windefine.h"

AuctionGoodsInfo::AuctionGoodsInfo(void)
{
    m_uSerializeID = 1;
    m_uAddPercent = 100;
    m_uMinAdd = 0;
    m_fCountDown = 0;
    m_uCurPrice = (unsigned int)-1;
    m_uRoleID = 0;
}

AuctionGoodsInfo::~AuctionGoodsInfo()
{

}

bool AuctionGoodsInfo::LoadFromFile(CFileStream &file)
{
    if (!m_cGoodsInfo.LoadFromFile(file))
        return false;

    file.ReadUShort(m_uSerializeID);
    file.ReadUShort(m_uAddPercent);
    file.ReadUInt(m_uMinAdd);
    file.ReadFloat(m_fCountDown);
    file.ReadUInt(m_uCurPrice);
    file.ReadUInt(m_uRoleID);
    file.ReadString2(m_strRoleName);

    ASSERT(0 < m_uSerializeID);
    ASSERT(m_uAddPercent <= 100);

    if (0 == m_uCurPrice)
        m_uCurPrice = m_cGoodsInfo.m_uSellPrice;

    return true;
}

bool AuctionGoodsInfo::SaveToFile(CFileStream &file)
{
    if (0 == m_uSerializeID || m_uAddPercent > 100 || !m_cGoodsInfo.SaveToFile(file))
        return false;

    file.WriteUShort(m_uSerializeID);
    file.WriteUShort(m_uAddPercent);
    file.WriteUInt(m_uMinAdd);
    file.WriteFloat(m_fCountDown);
    file.WriteUInt(m_uCurPrice);
    file.WriteUInt(m_uRoleID);
    file.WriteString2(m_strRoleName);

    return true;
}


