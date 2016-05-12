/** DanceGroupFairyLandCfgMgr.h
* Description:
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: lht
* Create time: 2016-02-18 11:48:00
*/


#ifndef __DANCEGROUP_FAIRYLADN_CFGMGR_H__
#define __DANCEGROUP_FAIRYLADN_CFGMGR_H__


#include "DanceGroupFairyLandCfg.h"
#include "../util/Split.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>
#include <map>
#include <string>

class CRewardUnit
{
public:
    CRewardUnit(){};
    ~CRewardUnit(){}
    
    CRewardUnit(itemtype_t nItemId, int nTime, unsigned short nCount,int nProbability)
        : m_nItemId(nItemId)
        , m_nTime(nTime)
        , m_nCount(nCount)
        , m_nProbability(nProbability)
    {}

    CRewardUnit(const CRewardUnit& other)
    {
        m_nItemId = other.m_nItemId;
        m_nTime = other.m_nTime;
        m_nCount = other.m_nCount;
        m_nProbability = other.m_nProbability;
    }

    itemtype_t m_nItemId;
    int m_nTime;
    unsigned short m_nCount;
    int m_nProbability;
};

enum 
{
    eBindMBType = 1,
    eMoneyType,
    eItemType ,
};

class CRewardInfo
{
public:
    int m_nType;
    union 
    {
        int m_nBindMB;
        int m_nMoney;
    };
    CRewardUnit m_Item;
};

class CDanceGroupFairyLandCfgMgr
{
protected:
    CDanceGroupFairyLandCfgMgr();

public:
    CLASS_INSTANCE_FUNC(CDanceGroupFairyLandCfgMgr);
    virtual ~CDanceGroupFairyLandCfgMgr();

    void Initialize();

    void LoadCfgFromLua(std::string& mailSender, std::string &mailTitle, std::string &mailcontext, std::string &mailStroageContext,
        int nStorageBoxRewardCycleWeek, std::string & StorageBoxSendTime,
        std::string& EndAnnouncementTime, std::string &EndAnnounceMentContext,
        std::string& StartTime, std::string &EndTime,
        std::string& AllowDanceBeginTime, std::string& AllowDanceEndTime,
        std::string& RewardSendTime,
        int nBuff, std::string &RankRule, std::string &StroageBoxRule,
        std::string strBlessRule, int nBuffAddTime, int nBuffPrice);

    int GetMaxChapterLevel() const;
    int GetChapterPassScore(int nChapterLevel) const;
    int GetChapterDanceType(int nChapterLevel) const;
    int GetChapterDifficultyLevel (int nChapterLevel) const;
    int GetChapterSceneId(int nChapterLevel) const;
    int GetChapterMusicId(int nChapterLevel) const;
    
    int GetJoinRewardBindMB(int nChapterLevel) const;
    int GetJoinRewardMoney(int nChapterLevel) const;
    int GetJoinRewardMB(int nChapterLevel) const;
    bool GetJoinRewardUnit(int nChapterLevel, int nSex,std::list<CItem> &listunit) const;
    //int GetJoinRewardContribute(int nChapterLevel) const;

    int GetPassRewardBindMB(int nChapterLevel) const;
    int GetPassRewardMoney(int nChapterLevel) const;
    int GetPassRewardMB(int nChapterLevel) const;
    bool GetPassRewardUnit(int nChapterLevel, int nSex, std::list<CRewardUnit> &listunit) const;
    bool GetStroageBoxReward(int nChapterLvl, std::list<CRewardUnit> &listunit) const;
    //int GetPassRewardContribute(int nChapterLevel) const;

    int GetOverDropPoolId(int nChapterLvl, int nSex) const;
    bool GetChapterShowRewardItems(int nChapterLvl, int nSex, std::list<CItem> & showItemsList) const;

    bool GetDropPoolUnit(int nDropPoolId, CRewardInfo& cell);
    bool RandomUnit(CDropPoolCfg &cfg, CRewardInfo &cell);

    bool GetRankItems(int nRankId,int nSex ,std::list<CItem> &list);
    int GetRankBindMb(int nRankId);
    int GetRankBindMoney(int nRankId);

    CLASS_PROPERTY_BY_REF(std::string, m_strMailSender, MailSender);
    CLASS_PROPERTY_BY_REF(std::string, m_strMailTitle, MailTitle);
    CLASS_PROPERTY_BY_REF(std::string, m_strMailContext, MailContext);
    CLASS_PROPERTY_BY_REF(std::string, m_strMailStorageBoxContext, MailStorageBoxContext);
    CLASS_PROPERTY_BY_REF(std::string, m_strStorageBoxSendTime, StorageBoxSendTime);
    CLASS_PROPERTY_BY_REF(std::string, m_strEndAnnounceMentTime, EndAnnounceMentTime);
    CLASS_PROPERTY_BY_REF(std::string, m_strEndAnnounceMentContext, EndAnnounceMentContext);
    CLASS_PROPERTY_BY_REF(std::string, m_strRankRule, RankRule);
    CLASS_PROPERTY_BY_REF(std::string, m_strStorageBoxRule, StorageBoxRule);
    CLASS_PROPERTY(int, m_nBlessBuff, BlessBuff);
    CLASS_PROPERTY(int, m_nStorageBoxRewardCycleWeek, StorageBoxRewardCycleWeek);
    CLASS_PROPERTY_BY_REF(std::string, m_strBlessRule, BlessRule);
    CLASS_PROPERTY(int, m_nBlessAddTime, BlessAddTime);
    CLASS_PROPERTY(int, m_nBlessPrice, BlessPrice);

    //获取每天第几秒开始活动
    int GetStartTimeSecond();
    //获取每天第几秒结束活动
    int GetEndTimeSecond();

    //获取每天第几秒允许开始秘境跳舞
    int GetAllowDanceBeginTimeSecond();
    //获取每天第几秒不允许开始秘境跳舞
    int GetAllowDanceEndTimeSecond();

    //获取每天第几秒发放参与奖励
    int GetNeedRewardTimeSecond();

    //获取每天第几秒不允许开始秘境跳舞
    int GetNeedStorageBoxRewardTimeSecond();

    int GetNeedAnnounMentTimeSecond();

protected:
    void LoadCfgFromFile();
    void LoadDropCfg(CFileStream &rFileStream);
    void LoadDGFairyLandCfg(CFileStream &rFileStream);
    void LoadDGFariyLandRankCfg(CFileStream &rFileStream);

private:
    bool str2RewardInfo(const std::string& str, CRewardInfo& cell);
    void FillCellStrList(std::list < CRewardInfo > &list, CDropPoolCfg &cfg);
    std::map<int, CDropPoolCfg> m_mapDropPoolCfg;
    std::map<int, CDanceGroupFairyLandCfg> m_mapFairyLandCfg;
    std::map<int, CDanceGroupFairyLandRankCfg> m_mapFairyLandRankCfg;

    CLASS_PROPERTY_BY_REF(std::string, m_strStartTime, StartTime);
    CLASS_PROPERTY_BY_REF(std::string, m_strEndTime, EndTime);
    CLASS_PROPERTY_BY_REF(std::string, m_strAllowDanceBeginTime, AllowDanceBeginTime);
    CLASS_PROPERTY_BY_REF(std::string, m_strAllowDanceEndTime, AllowDanceEndTime);
    CLASS_PROPERTY_BY_REF(std::string, m_strRewardSendTime, RewardSendTime);
};

#endif