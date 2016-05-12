/*
* DacmeGroupFairyLandCfgMgr.h
* Description:
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: luhaoting
* Create time: 2015-8-19 10:22:20
*/
#include "../../socket/Utility.h"
#include "DanceGroupFairyLandCfgMgr.h"
#include "../../socket/Log.h"
#include "../../datastructure/datainfor/FileStream.h"
#include "../../datastructure/DataStruct_Base.h"

#include "../util/CommonFunc.h"

extern std::string g_sPath;
#define DG_FAIRYLAND_FILE_NAME     "data/bytes/DanceGroupFairlyLand.bytes"
#define CELL_STRINGLEN 2
#define CELL_DATALEN 3

enum 
{
    eDataIndex,
    eProbabilityIndex,
};

enum
{
    eItemIndex,
    eTimeIndex,
    eCountIndex,
};

enum 
{
    eHour,
    eMinute,
    eSecond,
};

typedef std::map<int, CDropPoolCfg>::const_iterator DropPoolMapConstIt;
typedef std::map<int, CDanceGroupFairyLandCfg>::const_iterator FairyLandMapIConstIt;

CDanceGroupFairyLandCfgMgr::CDanceGroupFairyLandCfgMgr()
: m_strMailSender("")
, m_strMailTitle("")
, m_strMailContext("")
, m_strMailStorageBoxContext("")
, m_strStorageBoxSendTime("")
, m_strEndAnnounceMentTime("")
, m_strEndAnnounceMentContext("")
, m_strRankRule("")
, m_strStorageBoxRule("")
, m_nBlessBuff(0)
, m_nStorageBoxRewardCycleWeek(0)
, m_strBlessRule("")
, m_nBlessAddTime(0)
, m_nBlessPrice(0)
, m_strStartTime("")
, m_strEndTime("")
, m_strAllowDanceBeginTime("")
, m_strAllowDanceEndTime("")
, m_strRewardSendTime("")
{
}

CDanceGroupFairyLandCfgMgr::~CDanceGroupFairyLandCfgMgr()
{

}

void CDanceGroupFairyLandCfgMgr::Initialize()
{
    LoadCfgFromFile();
}

void CDanceGroupFairyLandCfgMgr::LoadCfgFromLua(std::string& mailSender, std::string &mailTitle, std::string &mailcontext, std::string & storageBoxmailContext,
    int nStorageBoxRewardCycleWeek, std::string & StorageBoxSendTime,
    std::string& EndAnnouncementTime, std::string &EndAnnounceMentContext,
    std::string& StartTime, std::string &EndTime,
    std::string& AllowDanceBeginTime, std::string& AllowDanceEndTime,
    std::string& RewardSendTime, int nBuff, std::string &RankRule, std::string &StroageBoxRule,
    std::string strBlessRule, int nBuffAddTime, int nBuffPrice)
{
    SetMailSender(mailSender);
    SetMailTitle(mailTitle);
    SetMailContext(mailcontext);
    SetMailStorageBoxContext(storageBoxmailContext);
    SetStorageBoxRewardCycleWeek(nStorageBoxRewardCycleWeek);
    SetStorageBoxSendTime(StorageBoxSendTime);
    SetEndAnnounceMentTime(EndAnnouncementTime);
    SetEndAnnounceMentContext(EndAnnounceMentContext);
    SetStartTime(StartTime);
    SetEndTime(EndTime);
    SetAllowDanceBeginTime(AllowDanceBeginTime);
    SetAllowDanceEndTime(AllowDanceEndTime);
    SetRewardSendTime(RewardSendTime);
    SetMailSender(mailSender);
    SetBlessBuff(nBuff);
    SetRankRule(RankRule);
    SetStorageBoxRule(StroageBoxRule);
    SetBlessRule(strBlessRule);
    SetBlessAddTime(nBuffAddTime);
    SetBlessPrice(nBuffPrice);
}

void CDanceGroupFairyLandCfgMgr::LoadCfgFromFile()
{
    std::string strFileName(g_sPath);
    strFileName.append(DG_FAIRYLAND_FILE_NAME);

    CFileStream file;
    file.open(strFileName.c_str(), "rb");

    LoadDropCfg(file);
    LoadDGFairyLandCfg(file);
    LoadDGFariyLandRankCfg(file);

    file.close();
}

void CDanceGroupFairyLandCfgMgr::LoadDropCfg(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
        return;

    m_mapDropPoolCfg.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        CDropPoolCfg dropPool;

        if (dropPool.loadInfoFromFile(rFileStream))
        {
            m_mapDropPoolCfg.insert(std::make_pair(dropPool.GetnDropId(), dropPool));
        }
    }
}

void CDanceGroupFairyLandCfgMgr::LoadDGFairyLandCfg(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
    {
        return;
    }

    m_mapFairyLandCfg.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        CDanceGroupFairyLandCfg fairylandCfg;

        if (fairylandCfg.LoadInfoFromFile(rFileStream))
        {
            m_mapFairyLandCfg.insert(std::make_pair(fairylandCfg.GetnChapterLevel(), fairylandCfg));
        }
    }

}

void CDanceGroupFairyLandCfgMgr::LoadDGFariyLandRankCfg(CFileStream &rFileStream)
{
    if (!rFileStream.IsOpen())
    {
        return;
    }

    m_mapFairyLandRankCfg.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort(nCount);

    for (unsigned short i = 0; i < nCount; ++i)
    {
        CDanceGroupFairyLandRankCfg fairylandRankCfg;

        if (fairylandRankCfg.LoadInfoFromFile(rFileStream))
        {
            m_mapFairyLandRankCfg.insert(std::make_pair(fairylandRankCfg.GetnRankId(), fairylandRankCfg));
        }
    }

}


bool CDanceGroupFairyLandCfgMgr::str2RewardInfo(const std::string& str, CRewardInfo& cell)
{
    if (str.size() <= 0)
    {
        return false;
    }

    std::vector<std::string> strTypeVec;
    Split(str, std::string(":"), strTypeVec);
    if (strTypeVec.size() != CELL_STRINGLEN)
    {
        return false;
    }

    std::vector<std::string> strVec;
    Split(strTypeVec[1], std::string("|"), strVec);
    if (strTypeVec.size() != CELL_STRINGLEN)
    {
        return false;
    }

    if (atoi(strTypeVec[0].c_str()) == eBindMBType)
    {
        cell.m_nType = eBindMBType;
        cell.m_nBindMB = atoi(strVec[0].c_str());
    }
    else if (atoi(strTypeVec[0].c_str()) == eMoneyType)
    {
        cell.m_nType = eMoneyType;
        cell.m_nMoney = atoi(strVec[0].c_str());
    }
    else if (atoi(strTypeVec[0].c_str()) == eItemType)
    {
        cell.m_nType = eItemType;

        std::vector<int> vecData;
        Split(strVec[eDataIndex], std::string(","), vecData);
        if (vecData.size() != CELL_DATALEN)
        {
            return false;
        }

        cell.m_Item.m_nItemId = vecData[eItemIndex];
        cell.m_Item.m_nTime = vecData[eTimeIndex];
        cell.m_Item.m_nCount = (unsigned short)vecData[eCountIndex];
        cell.m_Item.m_nProbability = atoi(strVec[eProbabilityIndex].c_str());
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
//FairyLandFunc
//////////////////////////////////////////////////////////////////////////
int CDanceGroupFairyLandCfgMgr::GetMaxChapterLevel() const
{
    FairyLandMapIConstIt it;
    int nMaxChapterLevel = 0;
    for (it = m_mapFairyLandCfg.begin(); it != m_mapFairyLandCfg.end(); ++it)
    {
        ++nMaxChapterLevel;
    }
    return nMaxChapterLevel;
}

int CDanceGroupFairyLandCfgMgr::GetChapterPassScore(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if ( it != m_mapFairyLandCfg.end() )
    {
        return it->second.GetnPassScore();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetChapterDanceType(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnDanceType();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetChapterDifficultyLevel(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnDifficultyLevel();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetChapterSceneId(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnSceneId();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetChapterMusicId(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnMusicId();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetJoinRewardBindMB(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnJoinRewardBindMB();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetJoinRewardMoney(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnJoinRewardMoney();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetJoinRewardMB(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnJoinRewardMB();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

bool CDanceGroupFairyLandCfgMgr::GetJoinRewardUnit(int nChapterLevel, int nSex, std::list<CItem> &listunit) const
{
    std::string str;
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        if (nSex == ESexType_Male)
        {
            str = it->second.GetstrMaleJoinItems();
        }
        else
        {
            str = it->second.GetstrFemaleJoinItems();
        }
        
        std::vector<std::string>  vecStrItem;
        Split(str, std::string("|"), vecStrItem);
        if (vecStrItem.size() <= 0)
        {
            return false;
        }

        CItem unit;
        for (std::vector<std::string>::iterator it = vecStrItem.begin();
            it != vecStrItem.end(); ++it)
        {
            std::vector<int> vecData;
            Split(*it, std::string(","), vecData);
            if (vecData.size() != CELL_DATALEN)
            {
                WriteLog(LOGLEVEL_ERROR, "DanceGroupFairyLand Reward config error info by ChapterLevel=%d", nChapterLevel);
                return false;
            }

            unit = CItem(vecData[eItemIndex], (unsigned short)vecData[eCountIndex], vecData[eTimeIndex], 0);
            listunit.push_back(unit);
        }
    }
    return true;
}

// int CDanceGroupFairyLandCfgMgr::GetJoinRewardContribute(int nChapterLevel) const
// {
//     FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
//     if (it != m_mapFairyLandCfg.end())
//     {
// 
//         return it->second.GetnJoinRewardContribute();
//     }
// 
//     WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
//     return 0;
// }

int CDanceGroupFairyLandCfgMgr::GetPassRewardBindMB(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnPassRewardBindMB();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetPassRewardMoney(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnPassRewardMoney();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetPassRewardMB(int nChapterLevel) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        return it->second.GetnPassRewardMB();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
    return 0;
}

bool CDanceGroupFairyLandCfgMgr::GetPassRewardUnit(int nChapterLevel, int nSex, std::list<CRewardUnit> &listunit) const
{
    std::string str;
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
    if (it != m_mapFairyLandCfg.end())
    {
        if (nSex == ESexType_Male)
        {
            str = it->second.GetstrMalePassItems();
        }
        else
        {
            str = it->second.GetstrFemalePassItems();
        }

        std::vector<std::string>  vecStrItem;
        Split(str, std::string("|"), vecStrItem);
        if (vecStrItem.size() <= 0)
        {
            WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
            return false;
        }

        CRewardUnit unit;
        for (std::vector<std::string>::iterator it = vecStrItem.begin();
            it != vecStrItem.end(); ++it)
        {
            std::vector<int> vecData;
            Split(*it, std::string(","), vecData);
            if (vecData.size() != CELL_DATALEN)
            {
                WriteLog(LOGLEVEL_ERROR, "DanceGroupFairyLand Reward config error info by ChapterLevel=%d", nChapterLevel);
                return false;
            }

            unit.m_nItemId = vecData[eItemIndex];
            unit.m_nTime = vecData[eTimeIndex];
            unit.m_nCount = (unsigned short)vecData[eCountIndex];
            unit.m_nProbability = 0;
            listunit.push_back(unit);
        }
    }
    return true;
}

bool CDanceGroupFairyLandCfgMgr::GetStroageBoxReward(int nChapterLvl, std::list<CRewardUnit> &listunit) const
{
    std::string str;
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLvl);
    if (it != m_mapFairyLandCfg.end())
    {
        str = it->second.GetstrStorageBoxReward();
        std::vector<std::string>  vecStrItem;
        Split(str, std::string("|"), vecStrItem);
        if (vecStrItem.size() <= 0)
        {
            WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLvl);
            return false;
        }

        CRewardUnit unit;
        for (std::vector<std::string>::iterator it = vecStrItem.begin();
            it != vecStrItem.end(); ++it)
        {
            std::vector<int> vecData;
            Split(*it, std::string(","), vecData);
            if (vecData.size() != CELL_DATALEN)
            {
                WriteLog(LOGLEVEL_ERROR, "DanceGroupFairyLand stragebox Reward config error info by ChapterLevel=%d", nChapterLvl);
                return false;
            }

            unit.m_nItemId = vecData[eItemIndex];
            unit.m_nTime = vecData[eTimeIndex];
            unit.m_nCount = (unsigned short)vecData[eCountIndex];
            unit.m_nProbability = 0;
            listunit.push_back(unit);
        }
    }
    return true;
}

// int CDanceGroupFairyLandCfgMgr::GetPassRewardContribute(int nChapterLevel) const
// {
//     FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLevel);
//     if (it != m_mapFairyLandCfg.end())
//     {
//         return it->second.GetnPassRewardContribute();
//     }
// 
//     WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLevel);
//     return 0;
// }

int CDanceGroupFairyLandCfgMgr::GetOverDropPoolId(int nChapterLvl,int nSex) const
{
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLvl);
    if (it != m_mapFairyLandCfg.end())
    {
        if (nSex == ESexType_Male)
        {
            return it->second.GetnMaleDropPoolId();
        }
        else
        {
            return it->second.GetnFemaleDropPoolId();
        }
        
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLvl);
    return 0;
}

bool CDanceGroupFairyLandCfgMgr::GetChapterShowRewardItems(int nChapterLvl,int nSex, std::list<CItem> & showItemsList )const
{
    std::string str;
    FairyLandMapIConstIt it = m_mapFairyLandCfg.find(nChapterLvl);
    if (it != m_mapFairyLandCfg.end())
    {
        if (nSex == ESexType_Male)
        {
            str =  it->second.GetstrMaleShowItems();
        }
        else
        {
            str = it->second.GetstrFemaleShowItems();
        }
        std::vector<std::string>  vecStrItem;
        Split(str, std::string("|"), vecStrItem);
        if (vecStrItem.size() <= 0)
        {
            WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by ChapterLevel=%d", nChapterLvl);
            return false;
        }

        CItem unit;
        for (std::vector<std::string>::iterator it = vecStrItem.begin();
            it != vecStrItem.end(); ++it)
        {
            std::vector<int> vecData;
            Split(*it, std::string(","), vecData);
            if (vecData.size() != CELL_DATALEN)
            {
                WriteLog(LOGLEVEL_ERROR, "DanceGroupFairyLand Reward config error info by ChapterLevel=%d", nChapterLvl);
                return false;
            }

            unit.Set(vecData[eItemIndex], (unsigned short)vecData[eCountIndex], vecData[eTimeIndex]);
            showItemsList.push_back(unit);
        }
    }
    return true;
}

int CDanceGroupFairyLandCfgMgr::GetStartTimeSecond()
{
    std::vector<int> vectime;
    Split(m_strStartTime, ":", vectime);
    if (vectime.size() != 3)
    {
        WriteLog(LOGLEVEL_ERROR, "FairyLand lua startTime error");
        return 0;
    }

    return (vectime[eHour] * 3600 + vectime[eMinute] * 60 + vectime[eSecond]);
}
//获取每天第几秒结束活动
int CDanceGroupFairyLandCfgMgr::GetEndTimeSecond()
{
    std::vector<int> vectime;
    Split(m_strEndTime, ":", vectime);
    if (vectime.size() != 3)
    {
        WriteLog(LOGLEVEL_ERROR, "FairyLand lua endTime error");
        return 0;
    }

    return (vectime[eHour] * 3600 + vectime[eMinute] * 60 + vectime[eSecond]);
}

//获取每天第几秒允许开始秘境跳舞
int CDanceGroupFairyLandCfgMgr::GetAllowDanceBeginTimeSecond()
{
    std::vector<int> vectime;
    Split(m_strAllowDanceBeginTime, ":", vectime);
    if (vectime.size() != 3)
    {
        WriteLog(LOGLEVEL_ERROR, "FairyLand lua AllowDanceBeginTime error");
        return 0;
    }

    return (vectime[eHour] * 3600 + vectime[eMinute] * 60 + vectime[eSecond]);
}
//获取每天第几秒不允许开始秘境跳舞
int CDanceGroupFairyLandCfgMgr::GetAllowDanceEndTimeSecond()
{
    std::vector<int> vectime;
    Split(m_strAllowDanceEndTime, ":", vectime);
    if (vectime.size() != 3)
    {
        WriteLog(LOGLEVEL_ERROR, "FairyLand lua AllowDanceEndTime error");
        return 0;
    }

    return (vectime[eHour] * 3600 + vectime[eMinute] * 60 + vectime[eSecond]);
}

//获取每天第几秒发放参与奖励
int CDanceGroupFairyLandCfgMgr::GetNeedRewardTimeSecond()
{
    std::vector<int> vectime;
    Split(m_strRewardSendTime, ":", vectime);
    if (vectime.size() != 3)
    {
        WriteLog(LOGLEVEL_ERROR, "FairyLand lua RewardSendTime error");
        return 0;
    }

    return (vectime[eHour] * 3600 + vectime[eMinute] * 60 + vectime[eSecond]);
}
//获取每天第几秒不允许开始秘境跳舞
int CDanceGroupFairyLandCfgMgr::GetNeedStorageBoxRewardTimeSecond()
{
    std::vector<int> vectime;
    Split(m_strStorageBoxSendTime, ":", vectime);
    if (vectime.size() != 3)
    {
        WriteLog(LOGLEVEL_ERROR, "FairyLand lua StorageBoxSendTime error");
        return 0;
    }

    return (vectime[eHour] * 3600 + vectime[eMinute] * 60 + vectime[eSecond]);
}

int CDanceGroupFairyLandCfgMgr::GetNeedAnnounMentTimeSecond()
{
    std::vector<int> vectime;
    Split(m_strEndAnnounceMentTime, ":", vectime);
    if (vectime.size() != 3)
    {
        WriteLog(LOGLEVEL_ERROR, "FairyLand lua EndAnnounceMentTime error");
        return 0;
    }

    return (vectime[eHour] * 3600 + vectime[eMinute] * 60 + vectime[eSecond]);
}


//////////////////////////////////////////////////////////////////////////
//DropPoolFunc
//////////////////////////////////////////////////////////////////////////
bool CDanceGroupFairyLandCfgMgr::GetDropPoolUnit(int nDropPoolId, CRewardInfo& cell)
{
    DropPoolMapConstIt it = m_mapDropPoolCfg.find(nDropPoolId);
    if ( it == m_mapDropPoolCfg.end() )
    {
        return false;
    }

    CDropPoolCfg cfg = it->second;
    if (! RandomUnit(cfg, cell) )
    {
        return false;
    }

    return true;
}

bool CDanceGroupFairyLandCfgMgr::RandomUnit(CDropPoolCfg &cfg, CRewardInfo &cell)
{
    int nMaxRandomValue = 0;

    std::list<CRewardInfo> listCell;
    FillCellStrList(listCell, cfg);

    std::map<int, CRewardInfo> RandomMap;
    for (std::list<CRewardInfo>::iterator it = listCell.begin();
        it != listCell.end(); ++it)
    {
        nMaxRandomValue += it->m_Item.m_nProbability;
        RandomMap.insert(std::make_pair(nMaxRandomValue, *it));
    }

    int randret = Random(1, nMaxRandomValue);

    std::map<int, CRewardInfo>::reverse_iterator pervit = RandomMap.rbegin();
    for (std::map<int, CRewardInfo>::reverse_iterator it = RandomMap.rbegin();
        it != RandomMap.rend(); )
    {
        if (pervit->first >= randret && it->first <= randret)
        {
            cell = pervit->second;
        }
        pervit = it;
        it++;

        if ( it == RandomMap.rend() )
        {
            cell = pervit->second;
            break;
        }
    }
    return true;
}

void CDanceGroupFairyLandCfgMgr::FillCellStrList(std::list < CRewardInfo > &list, CDropPoolCfg &cfg)
{
    CRewardInfo cell;
    if (!cfg.GetstrItme_1().empty())
    {
        str2RewardInfo(cfg.GetstrItme_1(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_2().empty())
    {
        str2RewardInfo(cfg.GetstrItme_2(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_3().empty())
    {
        str2RewardInfo(cfg.GetstrItme_3(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_4().empty())
    {
        str2RewardInfo(cfg.GetstrItme_4(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_5().empty())
    {
        str2RewardInfo(cfg.GetstrItme_5(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_6().empty())
    {
        str2RewardInfo(cfg.GetstrItme_6(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_7().empty())
    {
        str2RewardInfo(cfg.GetstrItme_7(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_8().empty())
    {
        str2RewardInfo(cfg.GetstrItme_8(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_9().empty())
    {
        str2RewardInfo(cfg.GetstrItme_9(), cell);
        list.push_back(cell);
    }
    if (!cfg.GetstrItme_10().empty())
    {
        str2RewardInfo(cfg.GetstrItme_10(), cell);
        list.push_back(cell);
    }
}

bool CDanceGroupFairyLandCfgMgr::GetRankItems(int nRankId, int nSex ,std::list<CItem> &list)
{
    std::string str;
    std::map<int, CDanceGroupFairyLandRankCfg>::iterator it = m_mapFairyLandRankCfg.find(nRankId);
    if (it != m_mapFairyLandRankCfg.end())
    {
        if (nSex == ESexType_Male)
        {
            str = it->second.GetstrMaleRewardItem();
        }
        else
        {
            str = it->second.GetstrFemaleRewardItem();
        }

        std::vector<std::string>  vecStrItem;
        Split(str, std::string("|"), vecStrItem);
        if (vecStrItem.size() <= 0)
        {
            return false;
        }

        CItem unit;
        for (std::vector<std::string>::iterator it = vecStrItem.begin();
            it != vecStrItem.end(); ++it)
        {
            std::vector<int> vecData;
            Split(*it, std::string(","), vecData);
            if (vecData.size() != CELL_DATALEN)
            {
                WriteLog(LOGLEVEL_ERROR, "DanceGroupFairyLand Reward config error info by rankid=%d", nRankId);
                return false;
            }

            unit.Set(vecData[eItemIndex], (unsigned short)vecData[eCountIndex], vecData[eTimeIndex]);
            list.push_back(unit);
        }
    }

    return true;
}

int CDanceGroupFairyLandCfgMgr::GetRankBindMb(int nRankId)
{
    std::map<int, CDanceGroupFairyLandRankCfg>::iterator it = m_mapFairyLandRankCfg.find(nRankId);
    if (it != m_mapFairyLandRankCfg.end())
    {
        return it->second.GetnBindMb();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by rankid=%d", nRankId);
    return 0;
}

int CDanceGroupFairyLandCfgMgr::GetRankBindMoney(int nRankId)
{
    std::map<int, CDanceGroupFairyLandRankCfg>::iterator it = m_mapFairyLandRankCfg.find(nRankId);
    if (it != m_mapFairyLandRankCfg.end())
    {
        return it->second.GetnMoney();
    }

    WriteLog(LOGLEVEL_ERROR, "Can not find DanceGroupFairyLand config info by rankid=%d", nRankId);
    return 0;
}

