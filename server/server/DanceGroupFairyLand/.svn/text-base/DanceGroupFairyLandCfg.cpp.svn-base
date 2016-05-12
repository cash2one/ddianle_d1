#include "DanceGroupFairyLandCfg.h"
#include "../../datastructure/datainfor/FileStream.h"


SDancerScore::SDancerScore()
    : m_nRoleId(0)
    , m_nScore(0)
    , m_nVipBuf(0)
    , m_nGrade(0)
{

}
SDancerScore::~SDancerScore()
{}

CDropPoolCfg::CDropPoolCfg()
    : m_DropId(0)
    , m_strItem1("")
    , m_strItem2("")
    , m_strItem3("")
    , m_strItem4("")
    , m_strItem5("")
    , m_strItem6("")
    , m_strItem7("")
    , m_strItem8("")
    , m_strItem9("")
    , m_strItem10("")
{

}

CDropPoolCfg::~CDropPoolCfg()
{

}

bool CDropPoolCfg::loadInfoFromFile(CFileStream &rFileStream)
{
    if ( !rFileStream.IsOpen() )
    {
        return false;
    }

    rFileStream.ReadInt(m_DropId);
    rFileStream.ReadString2(m_strItem1);
    rFileStream.ReadString2(m_strItem2);
    rFileStream.ReadString2(m_strItem3);
    rFileStream.ReadString2(m_strItem4);
    rFileStream.ReadString2(m_strItem5);
    rFileStream.ReadString2(m_strItem6);
    rFileStream.ReadString2(m_strItem7);
    rFileStream.ReadString2(m_strItem8);
    rFileStream.ReadString2(m_strItem9);
    rFileStream.ReadString2(m_strItem10);

    return true;
}

CDanceGroupFairyLandCfg::CDanceGroupFairyLandCfg()
    : m_nChapterLevel(0)
    , m_nPassScore(0)
    , m_nDanceType(0)
    , m_nJoinRewardBindMB(0)
    , m_nJoinRewardMoney(0)
    , m_nJoinRewardMB(0)
    , m_strMaleJoinItems("")
    , m_strFemaleJoinItems("")
    , m_nPassRewardBindMB(0)
    , m_nPassRewardMoney(0)
    , m_nPassRewardMB(0)
    , m_strMalePassItems("")
    , m_strFemalePassItems("")
    , m_nMaleDropPoolId(0)
    , m_nFemaleDropPoolId(0)
    , m_strMaleShowItems("")
    , m_strFemaleShowItems("")
    , m_strStorageBoxReward("")
{}

bool CDanceGroupFairyLandCfg::LoadInfoFromFile(CFileStream &rFileStream)
{
    if ( !rFileStream.IsOpen() )
    {
        return false;
    }

    rFileStream.ReadInt(m_nChapterLevel);
    rFileStream.ReadInt(m_nPassScore);
    rFileStream.ReadInt(m_nMusicId);
    rFileStream.ReadInt(m_nDifficultyLevel);
    rFileStream.ReadInt(m_nSceneId);
    rFileStream.ReadInt(m_nDanceType);

    rFileStream.ReadInt(m_nJoinRewardBindMB);
    rFileStream.ReadInt(m_nJoinRewardMoney);
//     rFileStream.ReadInt(m_nJoinRewardMB);
    rFileStream.ReadString2(m_strMaleJoinItems);
    rFileStream.ReadString2(m_strFemaleJoinItems);
    
    rFileStream.ReadInt(m_nPassRewardBindMB);
    rFileStream.ReadInt(m_nPassRewardMoney);
//     rFileStream.ReadInt(m_nPassRewardMB);
    rFileStream.ReadString2(m_strMalePassItems);
    rFileStream.ReadString2(m_strFemalePassItems);

    rFileStream.ReadInt(m_nMaleDropPoolId);
    rFileStream.ReadInt(m_nFemaleDropPoolId);
    rFileStream.ReadString2(m_strMaleShowItems);
    rFileStream.ReadString2(m_strFemaleShowItems);
    rFileStream.ReadString2(m_strStorageBoxReward);
    return true;
}


CDanceGroupFairyLandCfg::~CDanceGroupFairyLandCfg()
{}


CDanceGroupFairyLandRankCfg::CDanceGroupFairyLandRankCfg()
    : m_nBindMb(0)
    , m_nMoney(0)
    , m_strMaleRewardItem("")
    , m_strFemaleRewardItem("")
{}

bool CDanceGroupFairyLandRankCfg::LoadInfoFromFile(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
    {
        return false;
    }

    rFileStream.ReadInt(m_nRankId);
    rFileStream.ReadInt(m_nBindMb);
    rFileStream.ReadInt(m_nMoney);
    rFileStream.ReadString2(m_strMaleRewardItem);
    rFileStream.ReadString2(m_strFemaleRewardItem);
    return true;
}

CDanceGroupFairyLandRankCfg::~CDanceGroupFairyLandRankCfg()
{}