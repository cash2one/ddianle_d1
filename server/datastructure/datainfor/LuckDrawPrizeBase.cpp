#include "LuckDrawPrizeBase.h"
#include "FileStream.h"

#include "../Macro_Define.h"

#include "../../socket/ParamPool.h"
#include "../../socket/Windefine.h"


CLuckDrawPrizeBase::CLuckDrawPrizeBase(unsigned char nType)
    : m_nType(nType)
    , m_nLevel(ELuckDrawPrizeLevel_White)
    , m_nSexNeed(ESexType_No)
    , m_bAnnounce(false)
{
}

CLuckDrawPrizeBase::CLuckDrawPrizeBase(const CLuckDrawPrizeBase &ref)
    : m_nType(ref.m_nType)
    , m_nLevel(ref.m_nLevel)
    , m_nSexNeed(ref.m_nSexNeed)
    , m_bAnnounce(ref.m_bAnnounce)
{
}

CLuckDrawPrizeBase::CLuckDrawPrizeBase(unsigned char nType, unsigned char nLevel, unsigned char nSexNeed, bool bAnnounce)
    : m_nType(nType)
    , m_nLevel(nLevel)
    , m_nSexNeed(nSexNeed)
    , m_bAnnounce(bAnnounce)
{
}

void CLuckDrawPrizeBase::LoadFromFile(CFileStream &file)
{
    // Factory loads m_nType outside.
    file.ReadUChar(m_nLevel);
    file.ReadUChar(m_nSexNeed);
    file.ReadBool(m_bAnnounce);

    ASSERT(m_nType < ELuckDrawPrizeType_Max);
    ASSERT(m_nLevel < ELuckDrawPrizeLevel_Max);
    ASSERT(m_nSexNeed < ESexType_Max);
}

void CLuckDrawPrizeBase::doEncodeToClient(CParamPool &IOBuff) const
{
    IOBuff.AddUChar(m_nType);
    IOBuff.AddUChar(m_nLevel);
    // m_nSexNeed is not needed for client
    // m_bAnnounce is not needed for client
}

void CLuckDrawPrizeBase::doDecodeFromClient(CParamPool &IOBuff)
{
    // CLuckDrawPrizeFactory will get type first.
    m_nLevel = IOBuff.GetUChar();
}

void CLuckDrawPrizeBase::doEncodeToServer(CParamPool &IOBuff) const
{
    doEncodeToClient(IOBuff);

    IOBuff.AddUChar(m_nSexNeed);
    IOBuff.AddBool(m_bAnnounce);
}

void CLuckDrawPrizeBase::doDecodeFromServer(CParamPool &IOBuff)
{
    doDecodeFromClient(IOBuff);

    m_nSexNeed = IOBuff.GetUChar();
    m_bAnnounce = IOBuff.GetBool();
}

