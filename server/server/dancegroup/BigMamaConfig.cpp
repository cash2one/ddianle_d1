
#include "BigMamaConfig.h"
#include "../../datastructure/datainfor/FileStream.h"

BigMamaStageInfo::BigMamaStageInfo()
    : m_nStageID(0)
    , m_nMusicID(0)
    , m_nModeID(0)
    , m_nLevel(0)
    , m_nNpcID(0)
    , m_nTargetScore(0)
    , m_nSceneID(0)
    , m_nRatio(0)
{

}
BigMamaStageInfo::~BigMamaStageInfo()
{

}
bool BigMamaStageInfo::Load(CFileStream & fs)
{
    fs.ReadUShort(m_nStageID);
    fs.ReadUShort(m_nMusicID);
    fs.ReadUChar(m_nModeID);
    fs.ReadUChar(m_nLevel);
    fs.ReadUChar(m_nNpcID);
    fs.ReadUInt(m_nTargetScore);
    fs.ReadUChar(m_nSceneID);
    fs.ReadUInt(m_nRatio);

    return true;
}

// void BigMamaStageInfo::doEncode(CParamPool & IOBuff)
// {
// 
//     return;
// }



BigMamaExchange::BigMamaExchange()
    :m_nSex(0)
    ,m_nTargetItemID(0)
    ,m_nTargetMatune(0)
    ,m_nTargetCount(0)
    ,m_nM1ItemID(0)
    ,m_nM1Matune(0)
    ,m_nM1Count(0)
    ,m_nM2ItemID(0)
    ,m_nM2Matune(0)
    ,m_nM2Count(0)
    , m_nDanceGroupContibute(0)
    ,m_nMoneyCount(0)
    ,m_nBindMCoinCount(0)
    ,m_nMCoinCount(0)
{


}
BigMamaExchange::~BigMamaExchange()
{

}
bool BigMamaExchange::Load(CFileStream & fs)
{
    fs.ReadUChar(m_nSex);
    fs.ReadUInt(m_nTargetItemID);
    fs.ReadInt(m_nTargetMatune);
    fs.ReadUInt(m_nTargetCount);

    fs.ReadUInt(m_nM1ItemID);
    fs.ReadInt(m_nM1Matune);
    fs.ReadUInt(m_nM1Count);

    fs.ReadUInt(m_nM2ItemID);
    fs.ReadInt(m_nM2Matune);
    fs.ReadUInt(m_nM2Count);

    fs.ReadUInt(m_nM3ItemID);
    fs.ReadInt(m_nM3Matune);
    fs.ReadUInt(m_nM3Count);

    fs.ReadUInt(m_nDanceGroupContibute);

    fs.ReadUInt(m_nMoneyCount);
    fs.ReadUInt(m_nBindMCoinCount);
    fs.ReadUInt(m_nMCoinCount);

    return true;
}


BigMamaBossConfig::BigMamaBossConfig()
    : m_nBossID(0)
    , m_cSex(0)
{}
//BigMamaBossConfig::~BigMamaBossConfig(){}

bool BigMamaBossConfig::Load(CFileStream &file)
{
    unsigned char temp = 0; // Ã»ÓÃµÄ×Ö¶Î
    unsigned char cBossId = 0;
    file.ReadUChar(cBossId);
    m_nBossID = cBossId;

    unsigned short nNameLength = 0;
    file.ReadUShort(nNameLength);
    file.ReadString(m_strName, nNameLength);

    file.ReadUChar(temp); // story ID.

    file.ReadUChar(m_cSex);

    unsigned int cColor = 0;
    file.ReadUInt(cColor);
    m_cSKinColor = cColor;

    itemtype_t nItemType = 0;
    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Hair] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Face] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Body] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Glove] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Leg] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Cap] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_FacialContent] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Shoulder] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Wing] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_LeftHand] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_RightHand] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Wrist] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Hip] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Sock] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_Feet] = nItemType;

    file.ReadUInt(nItemType);
    m_Equips[eItemClothType_All] = nItemType;

    return true;

}

//////////////////////////////////////////////////////////////////////////
// BigMamaLuckData
BigMamaLuckData::BigMamaLuckData()
    : m_nNpcID(0)
    , m_nBaseProb(0)
    , m_nProbGrowRatio(0)
{

}
BigMamaLuckData::~BigMamaLuckData()
{

}


BigMamaReward::BigMamaReward()
    : m_rewardType(EBigMamaRewardsType_UnJoin)
    , m_nMoney(0)
    , m_nBindCoin(0)
    , m_nDanceGroupContribution(0)
{


}

BigMamaReward::~BigMamaReward()
{}
