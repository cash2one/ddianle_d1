/************************************************************************
 * MedalCfgMgr.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-8-20 10:22:20
 ***********************************************************************/

#ifndef __MEDALCFGMGR_H__
#define __MEDALCFGMGR_H__

#include "MedalCfg.h"

#include <map>

/************************************************************************/
/* MedalCfgMgr                                                                     */
/************************************************************************/

class CMedalCfgMgr
{
protected:
    CMedalCfgMgr();

public:
    CLASS_INSTANCE_FUNC(CMedalCfgMgr)
    virtual ~CMedalCfgMgr();

    void Initialize();

    int GetMedalMoneyID() const;
    void SetMedalMoneyID(int medalmoneyid);

    int GetVersionMedalType() const;
    void SetVersionMedalType(int type);

    void GetMedalScoreDescribe(std::string &) const;
    void SetMedalScoreDescribe(const std::string &);

    void GetAllMedalIDList( std::list<int> &rListID ) const;
    bool IsConfigContainMedal(int nMedalId) const;

    bool IsMedalForevery( int nMedalID ) const;
    int GetMedalDuration( int nMedalID ) const;
    int GetMedalType( int nMedalID ) const;
    int GetNeedMedalMoney( int nMedalID ) const;
    int GetMedalSortPriority( int nMedalID ) const;
    int GetMedalScore(int nMedalId) const;

    void GetMedalName( int nMedalID, std::string &rName ) const;
    void GetMedalTypeDescribe( int nMedalID, std::string &rTypeDescribe ) const;
    void GetMedalDescribe( int nMedalID, std::string &rDescribe ) const;
    void GetMedalIcon( int nMedalID, std::string &rIcon ) const;

    void GetAllProgressIDList( std::list<int> &rListID ) const;

    int GetTotalProgress( int nProgressID ) const;
    int GetProgressType( int nProgressID ) const;
    int GetRewardMedal( int nProgressID ) const;
    int GetProgressSortPriority( int nProgressID ) const;
    int GetVisibilityIndex(int nProgressID) const;
    unsigned int GetAllowGetStartTime( int nProgressID ) const;
    unsigned int GetAllowGetEndTime( int nProgressID ) const;
    void GetProgressDescribe( int nProgressID, std::string &rDescribe ) const;
    void GetSpecialParams( int nProgressID, std::list<unsigned int> &rParams ) const;

protected:
    void LoadConfigFromFile();

    void LoadMedalConfigFromFile( CFileStream &rFileStream );
    void LoadProgressConfigFromFile( CFileStream &rFileStream );

private:
    int m_nType;                            //勋章版本类型
    int m_nMedalMoneyID;                    //勋章币道具ID
    std::string m_strMedalScoreDescribe;
    std::map<int, CMedalCfg> m_mapMedalCfg; //map< MedalID , cMedalCdg >
    std::map<int, CMedalProgressCfg> m_mapMedalProgressCfg; //map<ProgressID,CMedalProgressCfg >
};


#endif  //__MEDALCFGMGR_H__
