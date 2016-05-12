#include "DungeonDefine.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"

CDungeonRank::CDungeonRank()
    :m_nFriendID(0)
    ,m_strFriendName("")
    ,m_nRank(0)
    ,m_nStarCount(0)
    ,m_strDungeonName("")
    ,m_strParcloseName("")
{
}

CStarRewardConfig::CStarRewardConfig()
    :m_nID(0)
    ,m_nCostStar(0)
    ,m_nMoney(0)
{
}

CDustRewardConfig::CDustRewardConfig()
    :m_nCostDust(0)
    ,m_strActivityName("")
    ,m_strIconName("")
    ,m_nMoney(0)
    ,m_nStoryID(0)
{
}

CMagicLampRewardMail::CMagicLampRewardMail()
{

}

CMagicLampDesc::CMagicLampDesc()
    :m_nMaxInviteFriendNum(0)
{

}

CCrystalConfig::CCrystalConfig()
    :m_nInitialValue(0)
    ,m_nMaxValue(0)
    ,m_nRestoreTime(0)
    ,m_nMinDustCount(0)
    ,m_nMaxDustCount(0)
    ,m_nFavorableBuyVitCount(0)
{
}


CBuyCrystalConfig::CBuyCrystalConfig()
    :m_nIndex(0)
    ,m_strIconName("")
    ,m_nCrystalCount(0)
    ,m_nPrice(0)
    ,m_nFavorablePrice(0)
{

}

void CBuyCrystalConfig::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddChar( (char)m_nIndex);
    IOBuff.AddStringW(m_strIconName);
    IOBuff.AddUShort((unsigned short)m_nCrystalCount);
    IOBuff.AddInt(m_nPrice);
    IOBuff.AddInt(m_nFavorablePrice);
}

CRoomEndPlayerScoreInfo::CRoomEndPlayerScoreInfo()
    :m_nPos(0)
    ,m_nScore(0)
{

}

ParcloseReward::ParcloseReward()
    :m_nExp(0)
{

}

ParcloseReward::~ParcloseReward()
{

}

void ParcloseReward::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nExp);

    IOBuff.AddUShort((unsigned short)m_listItem.size());
    std::list<CItem>::iterator it = m_listItem.begin();
    for (; it != m_listItem.end(); ++it)
    {
        it->doEncode(IOBuff);
    }

    IOBuff.AddUShort((unsigned short)m_listDebris.size());
    std::list<DebrisReward>::iterator itr = m_listDebris.begin();
    for (; itr != m_listDebris.end(); ++itr)
   {
        IOBuff.AddUInt(itr->nDebrisID);
        IOBuff.AddUInt(itr->nDebrisCount);

        CDebrisConfig * pDebrisConfig = ConfigManager::Instance().GetMagicLampConfigMgr().GetDebrisConfig(itr->nDebrisID);
        if (pDebrisConfig != NULL)
        {
            IOBuff.AddStringW(pDebrisConfig->m_strName);
            IOBuff.AddStringW(pDebrisConfig->m_strIconName);
        }
        else
        {
            IOBuff.AddStringW("");
            IOBuff.AddStringW("");
        }
    }
}

void ParcloseReward::doDecode(CParamPool &IOBuff)
{
    m_nExp = IOBuff.GetUInt();

    unsigned short nItemCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nItemCount; ++i)
    {
        CItem item;
        item.doDecode(IOBuff);

        m_listItem.push_back(item);
    }

    unsigned short nDebrisCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nDebrisCount; ++i)
    {
        DebrisReward reward;
        reward.nDebrisID = IOBuff.GetUInt();
        reward.nDebrisCount = IOBuff.GetUInt();

        m_listDebris.push_back(reward);
    }
}


