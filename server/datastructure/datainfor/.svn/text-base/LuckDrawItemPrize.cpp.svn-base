#include "LuckDrawItemPrize.h"
#include "FileStream.h"

#include "../../socket/ParamPool.h"
#include "../../socket/Windefine.h"

#define ITEMID_WEDDING_RING_START   21000
#define ITEMID_WEDDING_RING_END     22000


CLuckDrawItemPrize::CLuckDrawItemPrize(void)
    : CLuckDrawPrizeBase(ELuckDrawPrizeType_Item)
    , m_nItemType(0)
    , m_nItemCount(0)
    , m_nValidTime(0)
{
}

CLuckDrawItemPrize::CLuckDrawItemPrize(const CLuckDrawItemPrize &ref)
    : CLuckDrawPrizeBase(ref)
    , m_nItemType(ref.m_nItemType)
    , m_nItemCount(ref.m_nItemCount)
    , m_nValidTime(ref.m_nValidTime)
{
}

void CLuckDrawItemPrize::LoadFromFile(CFileStream &file)
{
    CLuckDrawPrizeBase::LoadFromFile(file);

    file.ReadUInt(m_nItemType);
    file.ReadUShort(m_nItemCount);
    file.ReadInt(m_nValidTime);

    ASSERT(!(ITEMID_WEDDING_RING_START <= m_nItemType && m_nItemType < ITEMID_WEDDING_RING_END) );// 不可贩卖结婚戒指
}

void CLuckDrawItemPrize::doEncodeToClient(CParamPool &IOBuff) const
{
    CLuckDrawPrizeBase::doEncodeToClient(IOBuff);

    doEncodeSelf(IOBuff);
}

void CLuckDrawItemPrize::doDecodeFromClient(CParamPool &IOBuff)
{
    CLuckDrawPrizeBase::doDecodeFromClient(IOBuff);

    doDecodeSelf(IOBuff);
}

void CLuckDrawItemPrize::doEncodeToServer(CParamPool &IOBuff) const
{
    CLuckDrawPrizeBase::doEncodeToServer(IOBuff);

    doEncodeSelf(IOBuff);
}

void CLuckDrawItemPrize::doDecodeFromServer(CParamPool &IOBuff)
{
    CLuckDrawPrizeBase::doDecodeFromServer(IOBuff);

    doDecodeSelf(IOBuff);
}

void CLuckDrawItemPrize::doEncodeSelf(CParamPool &IOBuff) const
{
    IOBuff.AddUInt(m_nItemType);
    IOBuff.AddUShort(m_nItemCount);
    IOBuff.AddInt(m_nValidTime);
}

void CLuckDrawItemPrize::doDecodeSelf(CParamPool &IOBuff)
{
    m_nItemType = IOBuff.GetUInt();
    m_nItemCount = IOBuff.GetUShort();
    m_nValidTime = IOBuff.GetInt();
}

