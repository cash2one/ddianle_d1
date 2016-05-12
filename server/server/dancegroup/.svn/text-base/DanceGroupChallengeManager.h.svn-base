/*
 * DanceGroupChallengeManager.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-9-17 15:35:50
 */

#ifndef __DANCEGROUPCHALLENGEMANAGER_H__
#define __DANCEGROUPCHALLENGEMANAGER_H__

#include "../Macros/Macros.h"
#include "../mall/Currency.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"

#include <map>

class DanceGroupChallengeStoreyInfo
{
public:
    DanceGroupChallengeStoreyInfo();
    virtual ~DanceGroupChallengeStoreyInfo();

public:
    void LoadConfigFromFile( CFileStream& file );

    CLASS_PROPERTY_READONLY(int, m_nStorey, Storey)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strAtlas, Atlas)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strIcon, Icon)
    CLASS_PROPERTY_READONLY(int, m_nSceneID, Scene)
    CLASS_PROPERTY_READONLY(int, m_nMusicID, Music)
    CLASS_PROPERTY_READONLY(int, m_nMusicMode, MusicMode)
    CLASS_PROPERTY_READONLY(int, m_nMusicLevel, MusicLevel)
    CLASS_PROPERTY_READONLY(unsigned char, m_nTargetType, TargetType)
    CLASS_PROPERTY_READONLY(unsigned int, m_nTargetScore, TargetScore)
    CLASS_PROPERTY_READONLY(int, m_nBossID, BossID)
    CLASS_PROPERTY_READONLY(unsigned int, m_nActiveValue, ActiveValue)
};

//////////////////////////////////////////////////////////////////////////

class DanceGroupChallengeReward
{
public:
    DanceGroupChallengeReward();
    virtual ~DanceGroupChallengeReward();

    void LoadConfigFromFile( CFileStream &file );

    void GetAllRewardInfos( bool bMale, std::list<CItem> &rList ) const;
    void GetRewardInfos( bool bMale, unsigned char nRandCount, std::list<CItem> &rList ) const;

private:
    void ReadItemInfos( CFileStream &file, std::vector<CItem> &rInfo, bool bMale ) const;

    CLASS_PROPERTY_READONLY(int, m_nStorey, Storey)

private:
    std::vector<CItem> m_vectMaleItem;
    std::vector<CItem> m_vectMaleRandItem;
    std::vector<CItem> m_vectFemaleItem;
    std::vector<CItem> m_vectFemaleRandItem;
};

//////////////////////////////////////////////////////////////////////////

class DanceGroupChallengeManager
{
private:
    DanceGroupChallengeManager();
    virtual ~DanceGroupChallengeManager();

public:
    CLASS_INSTANCE_FUNC(DanceGroupChallengeManager)

    bool Initialize();

public:
    int GetTotalStorey() const;

    bool IsOpenNow() const;
    unsigned int GetStartTime() const;
    unsigned int GetNextStartTime( unsigned int nStartTime ) const;
    unsigned int GetOpenTime( unsigned int nStartTime ) const;
    unsigned int GetCloseTime( unsigned int nNextStartTime ) const;
    unsigned int GetAnnouncementTime( unsigned int nNextStartTime ) const;

    const std::map<int, DanceGroupChallengeStoreyInfo>& GetStoreyInfo() const;
    const DanceGroupChallengeStoreyInfo* GetStoreyInfo( int nStorey ) const;

    const CBossConfig* GetBossInfo( int nStorey ) const;

    void GetAllRewardInfos( int nStorey, bool bMale, std::list<CItem> &rList ) const;
    void GetRewardInfos( int nStorey, short nRanking, bool bMale, std::list<CItem> &rList ) const;

    void GetNextBuyChanceInfo( unsigned short nCurrentBuyCount, Currency &rCost ) const;

public:
    void SetRandItemCount( short ranking, unsigned char nCount );
    void SetInspireInfos( Currency &rCost, unsigned short nMaxCount );
    void SetResetInfos( int nWeekday, int nHour, unsigned int nLockTime, unsigned int nAnnouncementTime );
    void SetMailInfo( std::string &rMailSender, std::string &rMailTitle, std::string &rMailContent );

protected:
    void LoadConfigFromFile();
    void LoadLevelInfos( CFileStream &file );
    void LoadRewardInfos( CFileStream &file );
    void LoadBuyChanceInfos( CFileStream &file );
    void LoadBossInfos( CFileStream &file );

    bool CheckConfig() const;

private:
    std::map<int, DanceGroupChallengeStoreyInfo> m_mapStoreyInfos;  // key storey
    std::map<int, DanceGroupChallengeReward> m_mapRewardInfos;      // key storey
    std::map<unsigned short, Currency> m_mapBuyChanceInfos;         // buy count
    std::map<int, CBossConfig> m_mapBossInfos;                      // boss id
    std::map<short, unsigned char> m_mapRandItemCount;              // ranking -> count

    CLASS_PROPERTY(unsigned char, m_nFreeChance, FreeChance)
    CLASS_PROPERTY(unsigned short, m_nOpenLevel, OpenLevel);
    CLASS_PROPERTY_READONLY_BY_REF(Currency, m_inspireCost, InspireCost)
    CLASS_PROPERTY_READONLY(unsigned short, m_nInspireMaxCount, InspireMaxCount)
    CLASS_PROPERTY_READONLY(int, m_nResetWeekday, ResetWeekday)
    CLASS_PROPERTY_READONLY(int, m_nResetHour, ResetHour)
    CLASS_PROPERTY_READONLY(unsigned int, m_nLockSeconds, LockSeconds)
    CLASS_PROPERTY_READONLY(unsigned int, m_nAnnouncementSeconds, AnnouncementSeconds)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMailSender, MailSender)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMailTitle, MailTitle)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMailContent, MailContent)
    CLASS_PROPERTY_BY_REF(std::string, m_strHelp, Help)
};

#endif // __DANCEGROUPCHALLENGEMANAGER_H__

